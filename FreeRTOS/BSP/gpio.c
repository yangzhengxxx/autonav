#include "gpio.h"

#include "timer.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#define M1_PWM_CH    1U
#define M1_DIR_PORT  GPIOC
#define M1_DIR_A_PIN GPIO_Pin_6
#define M1_DIR_B_PIN GPIO_Pin_7

#define M2_PWM_CH    2U
#define M2_DIR_PORT  GPIOC
#define M2_DIR_A_PIN GPIO_Pin_8
#define M2_DIR_B_PIN GPIO_Pin_9

#define M3_PWM_CH    3U
#define M3_DIR_PORT  GPIOD
#define M3_DIR_A_PIN GPIO_Pin_8
#define M3_DIR_B_PIN GPIO_Pin_9

#define M4_PWM_CH    4U
#define M4_DIR_PORT  GPIOD
#define M4_DIR_A_PIN GPIO_Pin_10
#define M4_DIR_B_PIN GPIO_Pin_11

static void Motor_SetSpeed(uint8_t pwm_ch,
                           GPIO_TypeDef *dir_port,
                           uint16_t dir_a_pin,
                           uint16_t dir_b_pin,
                           int16_t pwm)
{
    uint16_t duty;

    if (pwm > 0)
    {
        GPIO_SetBits(dir_port, dir_a_pin);
        GPIO_ResetBits(dir_port, dir_b_pin);
        duty = (uint16_t)pwm;
    }
    else if (pwm < 0)
    {
        GPIO_ResetBits(dir_port, dir_a_pin);
        GPIO_SetBits(dir_port, dir_b_pin);
        duty = (uint16_t)(-pwm);
    }
    else
    {
        GPIO_ResetBits(dir_port, dir_a_pin | dir_b_pin);
        duty = 0U;
    }

    TIM1_PWM_SetCompare(pwm_ch, duty);
}

void Motor_Direction_GPIO_Init(void)
{
    GPIO_InitTypeDef gpio_init;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_StructInit(&gpio_init);
    gpio_init.GPIO_Mode = GPIO_Mode_OUT;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd = GPIO_PuPd_DOWN;

    gpio_init.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_Init(GPIOC, &gpio_init);

    gpio_init.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_Init(GPIOD, &gpio_init);

    GPIO_ResetBits(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
    GPIO_ResetBits(GPIOD, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11);
}

void Motor1_SetSpeed(int16_t pwm)
{
    Motor_SetSpeed(M1_PWM_CH, M1_DIR_PORT, M1_DIR_A_PIN, M1_DIR_B_PIN, pwm);
}

void Motor2_SetSpeed(int16_t pwm)
{
    Motor_SetSpeed(M2_PWM_CH, M2_DIR_PORT, M2_DIR_A_PIN, M2_DIR_B_PIN, pwm);
}

void Motor3_SetSpeed(int16_t pwm)
{
    Motor_SetSpeed(M3_PWM_CH, M3_DIR_PORT, M3_DIR_A_PIN, M3_DIR_B_PIN, pwm);
}

void Motor4_SetSpeed(int16_t pwm)
{
    Motor_SetSpeed(M4_PWM_CH, M4_DIR_PORT, M4_DIR_A_PIN, M4_DIR_B_PIN, pwm);
}

#define M1_PWM_SIGN (+1)
#define M2_PWM_SIGN (+1)
#define M3_PWM_SIGN (+1)
#define M4_PWM_SIGN (+1)

void Motor_SetSpeedByIndex(uint8_t motor, int16_t pwm)
{

    static const int8_t pwm_sign[MOTOR_NUM] = {
        M1_PWM_SIGN, M2_PWM_SIGN, M3_PWM_SIGN, M4_PWM_SIGN};
    int16_t out;

    if (motor >= MOTOR_NUM)
    {
        return;
    }

    out = (int16_t)(pwm * pwm_sign[motor]);

    switch (motor)
    {
    case 0U:
        Motor1_SetSpeed(out);
        break;
    case 1U:
        Motor2_SetSpeed(out);
        break;
    case 2U:
        Motor3_SetSpeed(out);
        break;
    case 3U:
        Motor4_SetSpeed(out);
        break;
    default:
        break;
    }
}
