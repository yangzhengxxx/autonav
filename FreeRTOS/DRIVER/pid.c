#include "pid.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timer.h"

#define PID_KP 12.0f
#define PID_KI 0.20f
#define PID_KD 0.0f
#define PID_KF 2.296f
#define PID_KC 117.0f
#define PID_INT_MAX 3000.0f

#define PID_ACTUAL_LPF_ALPHA 0.35f

static PID_t g_pid[MOTOR_NUM];

void ResetPID(PID_t *p)
{
	if (p == 0)
	{
		return;
	}

	p->Error0 = 0.0f;
	p->Error1 = 0.0f;
	p->ErrorInt = 0.0f;
	p->Target = 0.0f;
	p->Actual = 0.0f;
	p->ActualRaw = 0.0f;
	p->ActualFiltered = 0.0f;
	p->FilterInitialized = 0U;
	p->Out = 0;
}

int16_t UpdatePID(PID_t *p)
{
	float out;
	float ff;
	float previous_integral;

	if (p == 0)
	{
		return 0;
	}

	p->Error1 = p->Error0;
	p->Error0 = p->Target - p->Actual;
	previous_integral = p->ErrorInt;

	if (p->Ki != 0.0f)
	{
		p->ErrorInt += p->Error0;

		if ((p->Ki * p->ErrorInt) > p->IntMax)
		{
			p->ErrorInt = p->IntMax / p->Ki;
		}
		else if ((p->Ki * p->ErrorInt) < -p->IntMax)
		{
			p->ErrorInt = -p->IntMax / p->Ki;
		}
	}
	else
	{
		p->ErrorInt = 0.0f;
	}

	ff = p->Kf * p->Target;

	if (p->Target > 1.0f)
	{
		ff += p->Kc;
	}
	else if (p->Target < -1.0f)
	{
		ff -= p->Kc;
	}

	out = ff + (p->Kp * p->Error0) + (p->Ki * p->ErrorInt) +
	      (p->Kd * (p->Error0 - p->Error1));

	if (((out > p->OutMax) && (p->Error0 > 0.0f)) ||
	    ((out < p->OutMin) && (p->Error0 < 0.0f)))
	{
		p->ErrorInt = previous_integral;
		out = ff + (p->Kp * p->Error0) + (p->Ki * p->ErrorInt) +
		      (p->Kd * (p->Error0 - p->Error1));
	}

	if (out > p->OutMax)
	{
		out = p->OutMax;
	}
	else if (out < p->OutMin)
	{
		out = p->OutMin;
	}

	p->Out = (int16_t)out;
	return p->Out;
}

void PID_Init(void)
{
	uint8_t i;

	for (i = 0U; i < MOTOR_NUM; i++)
	{
		g_pid[i].Kp = PID_KP;
		g_pid[i].Ki = PID_KI;
		g_pid[i].Kd = PID_KD;
		g_pid[i].Kf = PID_KF;
		g_pid[i].Kc = PID_KC;
		g_pid[i].OutMax = (float)TIM1_PWM_PERIOD;
		g_pid[i].OutMin = -(float)TIM1_PWM_PERIOD;
		g_pid[i].IntMax = PID_INT_MAX;
		ResetPID(&g_pid[i]);
	}
}

void PID_ResetMotor(uint8_t motor)
{
	if (motor >= MOTOR_NUM)
	{
		return;
	}
	ResetPID(&g_pid[motor]);
}

int16_t PID_Compute(uint8_t motor, float target, float actual)
{
	if (motor >= MOTOR_NUM)
	{
		return 0;
	}

	g_pid[motor].Target = target;
	g_pid[motor].ActualRaw = actual;

	if (g_pid[motor].FilterInitialized == 0U)
	{
		g_pid[motor].ActualFiltered = actual;
		g_pid[motor].FilterInitialized = 1U;
	}
	else
	{
		g_pid[motor].ActualFiltered += PID_ACTUAL_LPF_ALPHA *
			(actual - g_pid[motor].ActualFiltered);
	}

	g_pid[motor].Actual = g_pid[motor].ActualFiltered;
	return UpdatePID(&g_pid[motor]);
}

void PID_GetRuntimeSnapshot(uint8_t motor, PID_RuntimeSnapshot_t *snapshot)
{
	if ((motor >= MOTOR_NUM) || (snapshot == 0))
	{
		return;
	}

	taskENTER_CRITICAL();
	snapshot->target = g_pid[motor].Target;
	snapshot->actual = g_pid[motor].ActualRaw;
	snapshot->output = g_pid[motor].Out;
	taskEXIT_CRITICAL();
}
