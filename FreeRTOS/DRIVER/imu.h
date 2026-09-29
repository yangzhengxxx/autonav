#ifndef __IMU_H
#define __IMU_H

#include "stm32f4xx.h"

#ifdef __cplusplus
extern "C" {
#endif

extern float imu_gz;

extern float imu_gz_bias;

ErrorStatus IMU_Init(void);

ErrorStatus IMU_CalibGzBias(void);

float IMU_ReadGz(void);

#ifdef __cplusplus
}
#endif

#endif
