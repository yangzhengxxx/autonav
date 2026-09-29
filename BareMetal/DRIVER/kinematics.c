#include "kinematics.h"

#include <math.h>

#include "can.h"
#include "gpio.h"
#include "imu.h"
#include "pid.h"
#include "timer.h"

#define WHEEL_RADIUS 0.0375f
#define HALF_WHEEL_BASE 0.06875f
#define HALF_WHEEL_TRACK 0.095f

#define CHASSIS_ROTATION_RADIUS (HALF_WHEEL_BASE + HALF_WHEEL_TRACK)

#define TWO_PI 6.2831853f

#define WHEEL_MAX_OMEGA (CHASSIS_MAX_LINEAR / WHEEL_RADIUS)

static void LimitTranslation(float *vx, float *vy)
{
    float magnitude = sqrtf((*vx) * (*vx) + (*vy) * (*vy));

    if (magnitude > CHASSIS_MAX_LINEAR)
    {
        float scale = CHASSIS_MAX_LINEAR / magnitude;

        *vx *= scale;
        *vy *= scale;
    }
}

static float LimitScalar(float value, float max_abs)
{
    if (value > max_abs)
    {
        return max_abs;
    }

    if (value < -max_abs)
    {
        return -max_abs;
    }

    return value;
}

static void ScaleWheelSpeeds(WheelVelocity_t *wheel)
{
    float max_abs = 0.0f;
    uint32_t i;

    for (i = 0U; i < 4U; i++)
    {
        float abs_speed = fabsf(wheel->wheel_speed[i]);

        if (abs_speed > max_abs)
        {
            max_abs = abs_speed;
        }
    }

    if (max_abs > WHEEL_MAX_OMEGA)
    {
        float scale = WHEEL_MAX_OMEGA / max_abs;

        for (i = 0U; i < 4U; i++)
        {
            wheel->wheel_speed[i] *= scale;
        }
    }
}

void CalculateMotorSpeed(const int32_t *enc_delta,
                         uint32_t period_ms,
                         WheelFeedback_t *feedback)
{
    float omega_per_count;
    uint32_t i;

    if ((enc_delta == 0) || (feedback == 0) || (period_ms == 0U))
    {
        return;
    }

    omega_per_count = (TWO_PI * 1000.0f) /
                      ((float)ENC_CNT_PER_WHEEL_REV * (float)period_ms);

    for (i = 0U; i < 4U; i++)
    {
        feedback->omega[i] = (float)enc_delta[i] * omega_per_count;
        feedback->linear[i] = feedback->omega[i] * WHEEL_RADIUS;
    }
}

float LinearSpeedToCounts(float linear_mps, uint32_t period_ms)
{
    return linear_mps *
           ((float)ENC_CNT_PER_WHEEL_REV / (TWO_PI * WHEEL_RADIUS)) *
           ((float)period_ms / 1000.0f);

}

float WheelOmegaToCounts(float omega, uint32_t period_ms)
{
    return LinearSpeedToCounts(omega * WHEEL_RADIUS, period_ms);
}

void Mecanum_InverseKinematics(float vx,
                               float vy,
                               float wz,
                               WheelVelocity_t *wheel)
{
    float rotational_speed;

    if (wheel == 0)
    {
        return;
    }

    LimitTranslation(&vx, &vy);
    wz = LimitScalar(wz, CHASSIS_MAX_WZ);

    rotational_speed = CHASSIS_ROTATION_RADIUS * wz;

    wheel->wheel_speed[MOTOR_M1] =
        (vx - vy - rotational_speed) / WHEEL_RADIUS;

    wheel->wheel_speed[MOTOR_M2] =
        (vx + vy + rotational_speed) / WHEEL_RADIUS;

    wheel->wheel_speed[MOTOR_M3] =
        (vx + vy - rotational_speed) / WHEEL_RADIUS;

    wheel->wheel_speed[MOTOR_M4] =
        (vx - vy + rotational_speed) / WHEEL_RADIUS;

    ScaleWheelSpeeds(wheel);

}

void Mecanum_ForwardKinematics(const WheelVelocity_t *wheel,
                               ChassisVelocity_t *chassis)
{
    float v1;
    float v2;
    float v3;
    float v4;

    if ((wheel == 0) || (chassis == 0))
    {
        return;
    }

    v1 = wheel->wheel_speed[MOTOR_M1] * WHEEL_RADIUS;
    v2 = wheel->wheel_speed[MOTOR_M2] * WHEEL_RADIUS;
    v3 = wheel->wheel_speed[MOTOR_M3] * WHEEL_RADIUS;
    v4 = wheel->wheel_speed[MOTOR_M4] * WHEEL_RADIUS;

    chassis->vx = (v1 + v2 + v3 + v4) * 0.25f;
    chassis->vy = (-v1 + v2 + v3 - v4) * 0.25f;
    chassis->wz = (-v1 + v2 - v3 + v4) / (4.0f * CHASSIS_ROTATION_RADIUS);
}

#define TARGET_SLEW 20.0f

static volatile float g_target_cmd[MOTOR_NUM];
static float g_target_ramp[MOTOR_NUM];
static volatile uint8_t g_chassis_enable = 0U;

static float Target_Slew(float now, float cmd)
{
    if ((cmd - now) > TARGET_SLEW)
    {
        return now + TARGET_SLEW;
    }
    if ((now - cmd) > TARGET_SLEW)
    {
        return now - TARGET_SLEW;
    }
    return cmd;
}

void Chassis_Init(void)
{
    uint8_t i;

    PID_Init();

    for (i = 0U; i < MOTOR_NUM; i++)
    {
        g_target_cmd[i] = 0.0f;
        g_target_ramp[i] = 0.0f;
    }
    g_chassis_enable = 0U;
}

void Chassis_SetVelocity(float vx, float vy, float wz)
{
    WheelVelocity_t wheel;
    uint8_t i;

    Mecanum_InverseKinematics(vx, vy, wz, &wheel);

    for (i = 0U; i < MOTOR_NUM; i++)
    {

        g_target_cmd[i] = WheelOmegaToCounts(wheel.wheel_speed[i], CTRL_PERIOD_MS);
    }

    g_chassis_enable = 1U;
}

void Chassis_Stop(void)
{
    uint8_t i;

    g_chassis_enable = 0U;

    for (i = 0U; i < MOTOR_NUM; i++)
    {
        g_target_cmd[i] = 0.0f;
        g_target_ramp[i] = 0.0f;
        PID_ResetMotor(i);
        Motor_SetSpeedByIndex(i, 0);
    }
}

void PID_Callback(void)
{
    int32_t delta[MOTOR_NUM];
    uint8_t i;

    for (i = 0U; i < MOTOR_NUM; i++)
    {
        delta[i] = g_enc_delta[i];
    }

    if (g_chassis_enable == 0U)
    {
        return;
    }

    for (i = 0U; i < MOTOR_NUM; i++)
    {

        g_target_ramp[i] = Target_Slew(g_target_ramp[i], g_target_cmd[i]);

        Motor_SetSpeedByIndex(i, PID_Compute(i, g_target_ramp[i], (float)delta[i]));
    }
}

#define CAN_FB_SCALE 1000.0f

static void Pack_S16_LE(uint8_t *p, float value_si)
{
    int32_t v = (int32_t)(value_si * CAN_FB_SCALE);

    if (v > 32767)
    {
        v = 32767;
    }
    else if (v < -32768)
    {
        v = -32768;
    }

    p[0] = (uint8_t)((uint32_t)v & 0xFFU);
    p[1] = (uint8_t)(((uint32_t)v >> 8) & 0xFFU);
}

void Chassis_CanFeedback(void)
{
    int32_t delta[MOTOR_NUM];
    WheelFeedback_t feedback;
    WheelVelocity_t wheel;
    ChassisVelocity_t chassis;
    uint8_t data[8];
    uint8_t i;
    float gz;

    for (i = 0U; i < MOTOR_NUM; i++)
    {
        delta[i] = g_enc_delta[i];
    }

    CalculateMotorSpeed(delta, CTRL_PERIOD_MS, &feedback);

    for (i = 0U; i < MOTOR_NUM; i++)
    {
        wheel.wheel_speed[i] = feedback.omega[i];
    }

    Mecanum_ForwardKinematics(&wheel, &chassis);
    gz = IMU_ReadGz();

    Pack_S16_LE(&data[0], chassis.vx);
    Pack_S16_LE(&data[2], chassis.vy);
    Pack_S16_LE(&data[4], chassis.wz);
    Pack_S16_LE(&data[6], gz);

    (void)can_send_frame(CAN_TX_STD_ID, data, 8U);
}
