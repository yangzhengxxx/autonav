#ifndef __STM32F4xx_FLASH_RAMFUNC_H
#define __STM32F4xx_FLASH_RAMFUNC_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

#if defined ( __CC_ARM   )

#define __RAM_FUNC void

#elif defined ( __ICCARM__ )

#define __RAM_FUNC __ramfunc void

#elif defined   (  __GNUC__  )

#define __RAM_FUNC void  __attribute__((section(".RamFunc")))

#endif

__RAM_FUNC FLASH_FlashInterfaceCmd(FunctionalState NewState);
__RAM_FUNC FLASH_FlashSleepModeCmd(FunctionalState NewState);

#ifdef __cplusplus
}
#endif

#endif
