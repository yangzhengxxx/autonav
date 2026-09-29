#ifndef __PID_H
#define __PID_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	float Target;
	float Actual;
	int16_t Out;

	float Kp;
	float Ki;
	float Kd;
	float Kf;
	float Kc;

	float Error0;
	float Error1;
	float ErrorInt;

	float OutMax;
	float OutMin;
	float IntMax;
} PID_t;

int16_t UpdatePID(PID_t *p);

void ResetPID(PID_t *p);

void PID_Init(void);

void PID_ResetMotor(uint8_t motor);

int16_t PID_Compute(uint8_t motor, float target, float actual);

#ifdef __cplusplus
}
#endif

#endif
