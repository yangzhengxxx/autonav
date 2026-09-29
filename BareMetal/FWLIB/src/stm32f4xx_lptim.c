#include "stm32f4xx_lptim.h"

#if defined(STM32F410xx) || defined(STM32F413_423xx)

#define CFGR_INIT_CLEAR_MASK                 ((uint32_t) 0xFFCFF1FE)
#define CFGR_TRIG_AND_POL_CLEAR_MASK         ((uint32_t) 0xFFF91FFF)

void LPTIM_DeInit(LPTIM_TypeDef* LPTIMx)
{

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));

  if(LPTIMx == LPTIM1)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_LPTIM1, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_LPTIM1, DISABLE);
  }
}

void LPTIM_Init(LPTIM_TypeDef* LPTIMx, LPTIM_InitTypeDef* LPTIM_InitStruct)
{
  uint32_t tmpreg1 = 0;

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));
  assert_param(IS_LPTIM_CLOCK_SOURCE(LPTIM_InitStruct->LPTIM_ClockSource));
  assert_param(IS_LPTIM_CLOCK_PRESCALER(LPTIM_InitStruct->LPTIM_Prescaler));
  assert_param(IS_LPTIM_WAVEFORM(LPTIM_InitStruct->LPTIM_Waveform));
  assert_param(IS_LPTIM_OUTPUT_POLARITY(LPTIM_InitStruct->LPTIM_OutputPolarity));

  tmpreg1 = LPTIMx->CFGR;

  tmpreg1 &= CFGR_INIT_CLEAR_MASK;

  tmpreg1 |= (LPTIM_InitStruct->LPTIM_ClockSource | LPTIM_InitStruct->LPTIM_Prescaler
              |LPTIM_InitStruct->LPTIM_Waveform | LPTIM_InitStruct->LPTIM_OutputPolarity);

  LPTIMx->CFGR = tmpreg1;
}

void LPTIM_StructInit(LPTIM_InitTypeDef* LPTIM_InitStruct)
{

  LPTIM_InitStruct->LPTIM_ClockSource = LPTIM_ClockSource_APBClock_LPosc;

  LPTIM_InitStruct->LPTIM_OutputPolarity = LPTIM_OutputPolarity_High;

  LPTIM_InitStruct->LPTIM_Prescaler = LPTIM_Prescaler_DIV1;

  LPTIM_InitStruct->LPTIM_Waveform = LPTIM_Waveform_PWM_OnePulse;
}

void LPTIM_Cmd(LPTIM_TypeDef* LPTIMx, FunctionalState NewState)
{

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if(NewState != DISABLE)
  {

    LPTIMx->CR |= LPTIM_CR_ENABLE;
  }
  else
  {

    LPTIMx->CR &= ~(LPTIM_CR_ENABLE);
  }
}

void LPTIM_SelectClockSource(LPTIM_TypeDef* LPTIMx, uint32_t LPTIM_ClockSource)
{

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));
  assert_param(IS_LPTIM_CLOCK_SOURCE(LPTIM_ClockSource));

  LPTIMx->CFGR &= ~(LPTIM_CFGR_CKSEL);

  LPTIMx->CFGR |= LPTIM_ClockSource;
}

void LPTIM_SelectULPTIMClockPolarity(LPTIM_TypeDef* LPTIMx, uint32_t LPTIM_ClockPolarity)
{
  uint32_t tmpreg1 = 0;

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));
  assert_param(IS_LPTIM_CLOCK_POLARITY(LPTIM_ClockPolarity));

  tmpreg1 = LPTIMx->CFGR;

  tmpreg1 &= ~(LPTIM_CFGR_CKPOL);

  tmpreg1 |= LPTIM_ClockPolarity;

  LPTIMx->CFGR = tmpreg1;
}

void LPTIM_ConfigPrescaler(LPTIM_TypeDef* LPTIMx, uint32_t LPTIM_Prescaler)
{
  uint32_t tmpreg1 = 0;

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));
  assert_param(IS_LPTIM_CLOCK_PRESCALER(LPTIM_Prescaler));

  tmpreg1 = LPTIMx->CFGR;

  tmpreg1 &= ~(LPTIM_CFGR_PRESC);

  tmpreg1 |= LPTIM_Prescaler;

  LPTIMx->CFGR = tmpreg1;
}

void LPTIM_ConfigExternalTrigger(LPTIM_TypeDef* LPTIMx, uint32_t LPTIM_ExtTRGSource, uint32_t LPTIM_ExtTRGPolarity)
{
  uint32_t tmpreg1 = 0;

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));
  assert_param(IS_LPTIM_EXT_TRG_SOURCE(LPTIM_ExtTRGSource));
  assert_param(IS_LPTIM_EXT_TRG_POLARITY(LPTIM_ExtTRGPolarity));

  tmpreg1 = LPTIMx->CFGR;

  tmpreg1 &= CFGR_TRIG_AND_POL_CLEAR_MASK;

  tmpreg1 |= (LPTIM_ExtTRGSource | LPTIM_ExtTRGPolarity);

  LPTIMx->CFGR = tmpreg1;
}

void LPTIM_SelectSoftwareStart(LPTIM_TypeDef* LPTIMx)
{

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));

  LPTIMx->CFGR &= ~(LPTIM_CFGR_TRIGEN);
}

void LPTIM_ConfigTriggerGlitchFilter(LPTIM_TypeDef* LPTIMx, uint32_t LPTIM_TrigSampleTime)
{
  uint32_t tmpreg1 = 0;

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));
  assert_param(IS_LPTIM_TRIG_SAMPLE_TIME(LPTIM_TrigSampleTime));

  tmpreg1 = LPTIMx->CFGR;

  tmpreg1 &= ~(LPTIM_CFGR_TRGFLT);

  tmpreg1 |= (LPTIM_TrigSampleTime);

  LPTIMx->CFGR = tmpreg1;
}

void LPTIM_ConfigClockGlitchFilter(LPTIM_TypeDef* LPTIMx, uint32_t LPTIM_ClockSampleTime)
{
  uint32_t tmpreg1 = 0;

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));
  assert_param(IS_LPTIM_CLOCK_SAMPLE_TIME(LPTIM_ClockSampleTime));

  tmpreg1 = LPTIMx->CFGR;

  tmpreg1 &= ~(LPTIM_CFGR_CKFLT);

  tmpreg1 |= LPTIM_ClockSampleTime;

  LPTIMx->CFGR = tmpreg1;
}

void LPTIM_SelectOperatingMode(LPTIM_TypeDef* LPTIMx, uint32_t LPTIM_Mode)
{

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));
  assert_param(IS_LPTIM_MODE(LPTIM_Mode));

  if(LPTIM_Mode == LPTIM_Mode_Continuous)
  {

    LPTIMx->CR |= LPTIM_Mode_Continuous;
  }
  else
  {

    LPTIMx->CR |= LPTIM_Mode_Single;
  }
}

void LPTIM_TimoutCmd(LPTIM_TypeDef* LPTIMx, FunctionalState NewState)
{

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if(NewState != DISABLE)
  {

    LPTIMx->CFGR |= LPTIM_CFGR_TIMOUT;
  }
  else
  {

    LPTIMx->CFGR &= ~(LPTIM_CFGR_TIMOUT);
  }
}

void LPTIM_ConfigWaveform(LPTIM_TypeDef* LPTIMx, uint32_t LPTIM_Waveform)
{

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));
  assert_param(IS_LPTIM_WAVEFORM(LPTIM_Waveform));

  LPTIMx->CFGR &= ~(LPTIM_CFGR_CKFLT);

  LPTIMx->CFGR |= (LPTIM_Waveform);
}

void LPTIM_ConfigUpdate(LPTIM_TypeDef* LPTIMx, uint32_t LPTIM_Update)
{

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));
  assert_param(IS_LPTIM_UPDATE(LPTIM_Update));

  LPTIMx->CFGR &= ~(LPTIM_CFGR_PRELOAD);

  LPTIMx->CFGR |= (LPTIM_Update);
}

void LPTIM_SetAutoreloadValue(LPTIM_TypeDef* LPTIMx, uint32_t LPTIM_Autoreload)
{

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));
  assert_param(IS_LPTIM_AUTORELOAD(LPTIM_Autoreload));

  LPTIMx->ARR = LPTIM_Autoreload;
}

void LPTIM_SetCompareValue(LPTIM_TypeDef* LPTIMx, uint32_t LPTIM_Compare)
{

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));
  assert_param(IS_LPTIM_COMPARE(LPTIM_Compare));

  LPTIMx->CMP = LPTIM_Compare;
}

void LPTIM_SelectCounterMode(LPTIM_TypeDef* LPTIMx, FunctionalState NewState)
{

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if(NewState != DISABLE)
  {

    LPTIMx->CFGR |= LPTIM_CFGR_COUNTMODE;
  }
  else
  {

    LPTIMx->CFGR &= ~(LPTIM_CFGR_COUNTMODE);
  }
}

void LPTIM_SelectEncoderMode(LPTIM_TypeDef* LPTIMx, FunctionalState NewState)
{

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if(NewState != DISABLE)
  {

    LPTIMx->CFGR |= LPTIM_CFGR_ENC;
  }
  else
  {

    LPTIMx->CFGR &= ~(LPTIM_CFGR_ENC);
  }
}

uint32_t LPTIM_GetCounterValue(LPTIM_TypeDef* LPTIMx)
{

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));

  return LPTIMx->CNT;
}

uint32_t LPTIM_GetAutoreloadValue(LPTIM_TypeDef* LPTIMx)
{

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));

  return LPTIMx->ARR;
}

uint32_t LPTIM_GetCompareValue(LPTIM_TypeDef* LPTIMx)
{

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));

  return LPTIMx->CMP;
}

void LPTIM_RemapConfig(LPTIM_TypeDef* LPTIMx, uint32_t LPTIM_OPTR)
{

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));

  LPTIMx->OR = LPTIM_OPTR;
}

void LPTIM_ITConfig(LPTIM_TypeDef* LPTIMx, uint32_t LPTIM_IT, FunctionalState NewState)
 {

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));
  assert_param(IS_LPTIM_IT(LPTIM_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if(NewState != DISABLE)
  {

    LPTIMx->IER |= LPTIM_IT;
  }
  else
  {

    LPTIMx->IER &= ~(LPTIM_IT);
  }
}

FlagStatus LPTIM_GetFlagStatus(LPTIM_TypeDef* LPTIMx, uint32_t LPTIM_FLAG)
{
  ITStatus bitstatus = RESET;

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));
  assert_param(IS_LPTIM_GET_FLAG(LPTIM_FLAG));

  if((LPTIMx->ISR & LPTIM_FLAG) != (RESET))
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

void LPTIM_ClearFlag(LPTIM_TypeDef* LPTIMx, uint32_t LPTIM_CLEARF)
{

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));
  assert_param(IS_LPTIM_CLEAR_FLAG(LPTIM_CLEARF));

  LPTIMx->ICR |= LPTIM_CLEARF;
}

ITStatus LPTIM_GetITStatus(LPTIM_TypeDef* LPTIMx, uint32_t LPTIM_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t itstatus = 0x0, itenable = 0x0;

  assert_param(IS_LPTIM_ALL_PERIPH(LPTIMx));
  assert_param(IS_LPTIM_IT(LPTIM_IT));

  itstatus = LPTIMx->ISR & LPTIM_IT;

  itenable = LPTIMx->IER & LPTIM_IT;

  if((itstatus != RESET) && (itenable != RESET))
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

#endif
