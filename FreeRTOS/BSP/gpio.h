#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f4xx.h"
#include "timer.h"

void Motor_Direction_GPIO_Init(void);

void Motor1_SetSpeed(int16_t pwm);

void Motor2_SetSpeed(int16_t pwm);

void Motor3_SetSpeed(int16_t pwm);

void Motor4_SetSpeed(int16_t pwm);

void Motor_SetSpeedByIndex(uint8_t motor, int16_t pwm);

#endif
