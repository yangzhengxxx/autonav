#include "imu.h"

#include <stdio.h>

#include "board_config.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_rcc.h"
#include "timer.h"

#if (USE_DEBUG_UART != 0)
#define IMU_LOG(...) printf(__VA_ARGS__)
#else
#define IMU_LOG(...) ((void)0)
#endif

#define MPU_I2C_ADDR 0x68U

#define MPU_REG_SMPLRT_DIV 0x19U
#define MPU_REG_CONFIG 0x1AU
#define MPU_REG_GYRO_CONFIG 0x1BU
#define MPU_REG_PWR_MGMT_1 0x6BU
#define MPU_REG_WHO_AM_I 0x75U
#define MPU_REG_GYRO_ZOUT_H 0x47U

#define MPU_GYRO_LSB_PER_DPS 131.0f
#define DEG2RAD 0.0174532925f

#define IMU_GZ_SIGN (-1)

#define IMU_GZ_BIAS_SAMPLES 300U
#define IMU_GZ_BIAS_INTERVAL_MS 10U

float imu_gz = 0.0f;
float imu_gz_bias = 0.0f;

static uint8_t imu_i2c_wait_flag(uint32_t i2c_flag, FlagStatus status)
{
	uint32_t timeout = 100000U;

	while (I2C_GetFlagStatus(I2C2, i2c_flag) != status)
	{
		if (--timeout == 0U)
		{
			return 0U;
		}
	}
	return 1U;
}

static uint8_t imu_i2c_wait_event(uint32_t event)
{
	uint32_t timeout = 100000U;

	while (I2C_CheckEvent(I2C2, event) != SUCCESS)
	{
		if (--timeout == 0U)
		{
			return 0U;
		}
	}
	return 1U;
}

static uint8_t imu_i2c_write_reg(uint8_t reg, uint8_t value)
{
	if (imu_i2c_wait_flag(I2C_FLAG_BUSY, RESET) == 0U)
	{
		return 0U;
	}

	I2C_GenerateSTART(I2C2, ENABLE);
	if (imu_i2c_wait_event(I2C_EVENT_MASTER_MODE_SELECT) == 0U)
	{
		return 0U;
	}

	I2C_Send7bitAddress(I2C2, (uint8_t)(MPU_I2C_ADDR << 1), I2C_Direction_Transmitter);
	if (imu_i2c_wait_event(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == 0U)
	{
		return 0U;
	}

	I2C_SendData(I2C2, reg);
	if (imu_i2c_wait_event(I2C_EVENT_MASTER_BYTE_TRANSMITTED) == 0U)
	{
		return 0U;
	}

	I2C_SendData(I2C2, value);
	if (imu_i2c_wait_event(I2C_EVENT_MASTER_BYTE_TRANSMITTED) == 0U)
	{
		return 0U;
	}

	I2C_GenerateSTOP(I2C2, ENABLE);
	return 1U;
}

static uint8_t imu_i2c_read_regs(uint8_t reg, uint8_t *buf, uint8_t len)
{
	uint8_t i;

	if ((buf == 0) || (len == 0U))
	{
		return 0U;
	}

	if (imu_i2c_wait_flag(I2C_FLAG_BUSY, RESET) == 0U)
	{
		return 0U;
	}

	I2C_GenerateSTART(I2C2, ENABLE);
	if (imu_i2c_wait_event(I2C_EVENT_MASTER_MODE_SELECT) == 0U)
	{
		return 0U;
	}

	I2C_Send7bitAddress(I2C2, (uint8_t)(MPU_I2C_ADDR << 1), I2C_Direction_Transmitter);
	if (imu_i2c_wait_event(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == 0U)
	{
		return 0U;
	}

	I2C_SendData(I2C2, reg);
	if (imu_i2c_wait_event(I2C_EVENT_MASTER_BYTE_TRANSMITTED) == 0U)
	{
		return 0U;
	}

	I2C_GenerateSTART(I2C2, ENABLE);
	if (imu_i2c_wait_event(I2C_EVENT_MASTER_MODE_SELECT) == 0U)
	{
		return 0U;
	}

	I2C_Send7bitAddress(I2C2, (uint8_t)(MPU_I2C_ADDR << 1), I2C_Direction_Receiver);
	if (imu_i2c_wait_event(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) == 0U)
	{
		return 0U;
	}

	for (i = 0U; i < len; i++)
	{
		if (i == (uint8_t)(len - 1U))
		{
			I2C_AcknowledgeConfig(I2C2, DISABLE);
			I2C_GenerateSTOP(I2C2, ENABLE);
		}

		if (imu_i2c_wait_event(I2C_EVENT_MASTER_BYTE_RECEIVED) == 0U)
		{
			I2C_AcknowledgeConfig(I2C2, ENABLE);
			return 0U;
		}
		buf[i] = I2C_ReceiveData(I2C2);
	}

	I2C_AcknowledgeConfig(I2C2, ENABLE);
	return 1U;
}

static void imu_i2c2_gpio_init(void)
{
	GPIO_InitTypeDef gpio_init;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_I2C2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_I2C2);

	GPIO_StructInit(&gpio_init);
	gpio_init.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	gpio_init.GPIO_Mode = GPIO_Mode_AF;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init.GPIO_OType = GPIO_OType_OD;
	gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &gpio_init);
}

static void imu_i2c2_bus_init(void)
{
	I2C_InitTypeDef i2c_init;

	I2C_DeInit(I2C2);
	I2C_StructInit(&i2c_init);
	i2c_init.I2C_Mode = I2C_Mode_I2C;
	i2c_init.I2C_DutyCycle = I2C_DutyCycle_2;
	i2c_init.I2C_OwnAddress1 = 0x00U;
	i2c_init.I2C_Ack = I2C_Ack_Enable;
	i2c_init.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	i2c_init.I2C_ClockSpeed = 100000U;
	I2C_Init(I2C2, &i2c_init);
	I2C_Cmd(I2C2, ENABLE);
}

ErrorStatus IMU_Init(void)
{
	uint8_t who = 0U;

	imu_i2c2_gpio_init();
	imu_i2c2_bus_init();

	if (imu_i2c_write_reg(MPU_REG_PWR_MGMT_1, 0x80U) == 0U)
	{
		return ERROR;
	}
	delay_ms(100U);

	if (imu_i2c_write_reg(MPU_REG_PWR_MGMT_1, 0x01U) == 0U)
	{
		return ERROR;
	}
	delay_ms(10U);

	if (imu_i2c_write_reg(MPU_REG_CONFIG, 0x03U) == 0U)
	{
		return ERROR;
	}
	if (imu_i2c_write_reg(MPU_REG_SMPLRT_DIV, 0x09U) == 0U)
	{
		return ERROR;
	}
	if (imu_i2c_write_reg(MPU_REG_GYRO_CONFIG, 0x00U) == 0U)
	{
		return ERROR;
	}

	if (imu_i2c_read_regs(MPU_REG_WHO_AM_I, &who, 1U) == 0U)
	{
		return ERROR;
	}

	if ((who != 0x71U) && (who != 0x70U) && (who != 0x73U))
	{
		IMU_LOG("IMU WHO_AM_I=0x%02X unexpected\r\n", (unsigned int)who);
		return ERROR;
	}

	IMU_LOG("IMU ok, WHO_AM_I=0x%02X, I2C2 PB10/PB11\r\n", (unsigned int)who);
	imu_gz = 0.0f;
	imu_gz_bias = 0.0f;

	if (IMU_CalibGzBias() != SUCCESS)
	{
		IMU_LOG("IMU bias calib failed\r\n");
		return ERROR;
	}

	return SUCCESS;
}

static uint8_t imu_read_gz_raw(float *gz_raw_rad)
{
	uint8_t raw[2];
	int16_t gz_raw;
	float gz_dps;

	if ((gz_raw_rad == 0) || (imu_i2c_read_regs(MPU_REG_GYRO_ZOUT_H, raw, 2U) == 0U))
	{
		return 0U;
	}

	gz_raw = (int16_t)(((uint16_t)raw[0] << 8) | raw[1]);
	gz_dps = (float)gz_raw / MPU_GYRO_LSB_PER_DPS;
	*gz_raw_rad = (float)IMU_GZ_SIGN * gz_dps * DEG2RAD;
	return 1U;
}

ErrorStatus IMU_CalibGzBias(void)
{
	float sum = 0.0f;
	float sample = 0.0f;
	uint16_t i;
	uint16_t ok = 0U;

	IMU_LOG("IMU bias calib: keep still ~3s...\r\n");

	for (i = 0U; i < IMU_GZ_BIAS_SAMPLES; i++)
	{
		if (imu_read_gz_raw(&sample) != 0U)
		{
			sum += sample;
			ok++;
		}
		delay_ms(IMU_GZ_BIAS_INTERVAL_MS);
	}

	if (ok < (IMU_GZ_BIAS_SAMPLES / 2U))
	{
		return ERROR;
	}

	imu_gz_bias = sum / (float)ok;
#if (USE_DEBUG_UART != 0)
	{
		int bias_mrad = (int)(imu_gz_bias * 1000.0f);
		IMU_LOG("IMU bias=%s%d.%03d rad/s\r\n",
			   (bias_mrad < 0) ? "-" : "",
			   ((bias_mrad < 0) ? -bias_mrad : bias_mrad) / 1000,
			   ((bias_mrad < 0) ? -bias_mrad : bias_mrad) % 1000);
	}
#endif

	return SUCCESS;
}

float IMU_ReadGz(void)
{
	float gz_raw_rad = 0.0f;

	if (imu_read_gz_raw(&gz_raw_rad) == 0U)
	{
		return imu_gz;
	}

	imu_gz = gz_raw_rad - imu_gz_bias;
	return imu_gz;
}
