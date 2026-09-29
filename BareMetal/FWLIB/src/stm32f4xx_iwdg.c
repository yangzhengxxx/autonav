#include "stm32f4xx_iwdg.h"

#define KR_KEY_RELOAD    ((uint16_t)0xAAAA)
#define KR_KEY_ENABLE    ((uint16_t)0xCCCC)

void IWDG_WriteAccessCmd(uint16_t IWDG_WriteAccess)
{

  assert_param(IS_IWDG_WRITE_ACCESS(IWDG_WriteAccess));
  IWDG->KR = IWDG_WriteAccess;
}

void IWDG_SetPrescaler(uint8_t IWDG_Prescaler)
{

  assert_param(IS_IWDG_PRESCALER(IWDG_Prescaler));
  IWDG->PR = IWDG_Prescaler;
}

void IWDG_SetReload(uint16_t Reload)
{

  assert_param(IS_IWDG_RELOAD(Reload));
  IWDG->RLR = Reload;
}

void IWDG_ReloadCounter(void)
{
  IWDG->KR = KR_KEY_RELOAD;
}

void IWDG_Enable(void)
{
  IWDG->KR = KR_KEY_ENABLE;
}

FlagStatus IWDG_GetFlagStatus(uint16_t IWDG_FLAG)
{
  FlagStatus bitstatus = RESET;

  assert_param(IS_IWDG_FLAG(IWDG_FLAG));
  if ((IWDG->SR & IWDG_FLAG) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }

  return bitstatus;
}
