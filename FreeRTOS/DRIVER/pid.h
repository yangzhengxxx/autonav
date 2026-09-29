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
	float ActualRaw;
	float ActualFiltered;
	uint8_t FilterInitialized;
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

typedef struct
{
	float target;
	float actual;
	int16_t output;
} PID_RuntimeSnapshot_t;

int16_t UpdatePID(PID_t *p);

void ResetPID(PID_t *p);

void PID_Init(void);

void PID_ResetMotor(uint8_t motor);

int16_t PID_Compute(uint8_t motor, float target, float actual);

void PID_GetRuntimeSnapshot(uint8_t motor, PID_RuntimeSnapshot_t *snapshot);

#ifdef __cplusplus
}
#endif

#endif
