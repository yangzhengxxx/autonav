#ifndef __KINEMATICS_H
#define __KINEMATICS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define CHASSIS_MAX_LINEAR 0.8f
#define CHASSIS_MAX_WZ 2.0f

typedef struct
{
    float vx;
    float vy;
    float wz;
} ChassisVelocity_t;

typedef struct
{
    float wheel_speed[4];
} WheelVelocity_t;

typedef struct
{
    float omega[4];
    float linear[4];
} WheelFeedback_t;

typedef enum
{
    MOTOR_M1 = 0,
    MOTOR_M2,
    MOTOR_M3,
    MOTOR_M4
} MotorNumber_t;

void Mecanum_InverseKinematics(float vx,
                               float vy,
                               float wz,
                               WheelVelocity_t *wheel);

void Mecanum_ForwardKinematics(const WheelVelocity_t *wheel,
                               ChassisVelocity_t *chassis);

void CalculateMotorSpeed(const int32_t *enc_delta,
                         uint32_t period_ms,
                         WheelFeedback_t *feedback);

float LinearSpeedToCounts(float linear_mps, uint32_t period_ms);

float WheelOmegaToCounts(float omega, uint32_t period_ms);

void Chassis_Init(void);

void Chassis_SetVelocity(float vx, float vy, float wz);

void Chassis_Stop(void);

void PID_Callback(void);

void Chassis_CanFeedback(void);

#ifdef __cplusplus
}
#endif

#endif
