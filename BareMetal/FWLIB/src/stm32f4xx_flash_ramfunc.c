#include "stm32f4xx_flash_ramfunc.h"

__RAM_FUNC FLASH_FlashInterfaceCmd(FunctionalState NewState)
{
  if (NewState != DISABLE)
  {

    CLEAR_BIT(PWR->CR, PWR_CR_FISSR);
  }
  else
  {

    SET_BIT(PWR->CR, PWR_CR_FISSR);
  }
}

__RAM_FUNC FLASH_FlashSleepModeCmd(FunctionalState NewState)
{
  if (NewState != DISABLE)
  {

    SET_BIT(PWR->CR, PWR_CR_FMSSR);
  }
  else
  {

    CLEAR_BIT(PWR->CR, PWR_CR_FMSSR);
  }
}
