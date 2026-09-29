#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f4xx.h"

#define TIM1_PWM_TIMER_CLOCK_HZ 168000000U
#define TIM1_PWM_FREQUENCY_HZ 20000U

#define TIM1_PWM_PERIOD \
    ((TIM1_PWM_TIMER_CLOCK_HZ / TIM1_PWM_FREQUENCY_HZ) - 1U)

void TIM1_PWM_Init(void);
void TIM1_PWM_SetCompare(uint8_t channel, uint16_t compare);

#define MOTOR_NUM 4U

#define M1_ENCODER_TIM TIM2
#define M2_ENCODER_TIM TIM3
#define M3_ENCODER_TIM TIM4
#define M4_ENCODER_TIM TIM5

#define ENC_PPR 500L
#define ENC_QUAD 4L
#define ENC_CNT_PER_MOTOR_REV (ENC_PPR * ENC_QUAD)
#define MOTOR_GEAR_RATIO 30L
#define ENC_CNT_PER_WHEEL_REV \
    (ENC_CNT_PER_MOTOR_REV * MOTOR_GEAR_RATIO)

#define M1_ENC_SIGN (+1)
#define M2_ENC_SIGN (-1)
#define M3_ENC_SIGN (+1)
#define M4_ENC_SIGN (-1)

void TIM2_Encoder_Init(void);
void TIM3_Encoder_Init(void);
void TIM4_Encoder_Init(void);
void TIM5_Encoder_Init(void);
void Encoder_TIM_Init(void);

int16_t Encoder_Get(TIM_TypeDef *TIM);

int32_t Encoder_GetDelta(uint8_t motor);

extern volatile int32_t g_enc_delta[MOTOR_NUM];

extern volatile int32_t g_enc_total[MOTOR_NUM];

void ReadAllEncoder(void);

#define TIM6_INTERRUPT_PERIOD_MS 1U

#define CTRL_PERIOD_MS 10U

void TIM6_1ms_Init(void);
void BSP_TIM_Init(void);

extern volatile uint32_t g_tim6_ms;

void PID_Callback(void);

void delay_ms(uint32_t ms);

#endif
