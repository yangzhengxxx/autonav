#include "stm32f4xx.h"
#include "can.h"
#include "gpio.h"
#include "imu.h"
#include "kinematics.h"
#include "timer.h"
#include "usart.h"

#define BT_LINEAR_SPEED 0.2f
#define BT_WZ_SPEED 0.3f

int main(void)
{
	uint32_t last_fb_ms = 0U;

	Motor_Direction_GPIO_Init();
	BSP_TIM_Init();
	bt_usart_init();
	(void)can_init();
	Chassis_Init();
	(void)IMU_Init();

	while (1)
	{
		if (g_can_rx.updated != 0U)
		{
			int16_t vx = (int16_t)(g_can_rx.data[0] | (g_can_rx.data[1] << 8));
			int16_t vy = (int16_t)(g_can_rx.data[2] | (g_can_rx.data[3] << 8));
			int16_t wz = (int16_t)(g_can_rx.data[4] | (g_can_rx.data[5] << 8));

			g_can_rx.updated = 0U;
			Chassis_SetVelocity(vx / 1000.0f, vy / 1000.0f, wz / 1000.0f);
		}

		while (bt_usart_data_available() != 0U)
		{
			uint8_t cmd = bt_usart_read_byte();

			switch (cmd)
			{
			case 0x00U:
				Chassis_Stop();
				break;
			case 0x01U:
				Chassis_SetVelocity(BT_LINEAR_SPEED, 0.0f, 0.0f);
				break;
			case 0x03U:
				Chassis_SetVelocity(-BT_LINEAR_SPEED, 0.0f, 0.0f);
				break;
			case 0x02U:
				Chassis_SetVelocity(0.0f, 0.0f, BT_WZ_SPEED);
				break;
			case 0x04U:
				Chassis_SetVelocity(0.0f, 0.0f, -BT_WZ_SPEED);
				break;
			default:
				break;
			}
		}

		if ((g_tim6_ms - last_fb_ms) >= 20U)
		{
			last_fb_ms = g_tim6_ms;
			Chassis_CanFeedback();
		}
	}
}
