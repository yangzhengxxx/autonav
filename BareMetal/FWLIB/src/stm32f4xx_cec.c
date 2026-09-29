#include "stm32f4xx_cec.h"
#include "stm32f4xx_rcc.h"

#if defined(STM32F446xx)

#define BROADCAST_ADDRESS      ((uint32_t)0x0000F)
#define CFGR_CLEAR_MASK        ((uint32_t)0x7000FE00)

void CEC_DeInit(void)
{
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_CEC, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_CEC, DISABLE);
}

void CEC_Init(CEC_InitTypeDef* CEC_InitStruct)
{
  uint32_t tmpreg = 0;

  assert_param(IS_CEC_SIGNAL_FREE_TIME(CEC_InitStruct->CEC_SignalFreeTime));
  assert_param(IS_CEC_RX_TOLERANCE(CEC_InitStruct->CEC_RxTolerance));
  assert_param(IS_CEC_STOP_RECEPTION(CEC_InitStruct->CEC_StopReception));
  assert_param(IS_CEC_BIT_RISING_ERROR(CEC_InitStruct->CEC_BitRisingError));
  assert_param(IS_CEC_LONG_BIT_PERIOD_ERROR(CEC_InitStruct->CEC_LongBitPeriodError));
  assert_param(IS_CEC_BDR_NO_GEN_ERROR(CEC_InitStruct->CEC_BRDNoGen));
  assert_param(IS_CEC_SFT_OPTION(CEC_InitStruct->CEC_SFTOption));

  tmpreg = CEC->CFGR;

  tmpreg &= CFGR_CLEAR_MASK;

  tmpreg |= (CEC_InitStruct->CEC_SignalFreeTime | CEC_InitStruct->CEC_RxTolerance |
             CEC_InitStruct->CEC_StopReception  | CEC_InitStruct->CEC_BitRisingError |
             CEC_InitStruct->CEC_LongBitPeriodError| CEC_InitStruct->CEC_BRDNoGen |
             CEC_InitStruct->CEC_SFTOption);

  CEC->CFGR = tmpreg;
}

void CEC_StructInit(CEC_InitTypeDef* CEC_InitStruct)
{
  CEC_InitStruct->CEC_SignalFreeTime = CEC_SignalFreeTime_Standard;
  CEC_InitStruct->CEC_RxTolerance = CEC_RxTolerance_Standard;
  CEC_InitStruct->CEC_StopReception = CEC_StopReception_Off;
  CEC_InitStruct->CEC_BitRisingError = CEC_BitRisingError_Off;
  CEC_InitStruct->CEC_LongBitPeriodError = CEC_LongBitPeriodError_Off;
  CEC_InitStruct->CEC_BRDNoGen = CEC_BRDNoGen_Off;
  CEC_InitStruct->CEC_SFTOption = CEC_SFTOption_Off;
}

void CEC_Cmd(FunctionalState NewState)
{
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    CEC->CR |= CEC_CR_CECEN;
  }
  else
  {

    CEC->CR &= ~CEC_CR_CECEN;
  }
}

void CEC_ListenModeCmd(FunctionalState NewState)
{
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    CEC->CFGR |= CEC_CFGR_LSTN;
  }
  else
  {

    CEC->CFGR &= ~CEC_CFGR_LSTN;
  }
}

void CEC_OwnAddressConfig(uint8_t CEC_OwnAddress)
{
  uint32_t tmp =0x00;

  assert_param(IS_CEC_ADDRESS(CEC_OwnAddress));
  tmp = 1 <<(CEC_OwnAddress + 16);

  CEC->CFGR |= tmp;
}

void CEC_OwnAddressClear(void)
{

  CEC->CFGR = 0x0;
}

void CEC_SendData(uint8_t Data)
{

  CEC->TXDR = Data;
}

uint8_t CEC_ReceiveData(void)
{

  return (uint8_t)(CEC->RXDR);
}

void CEC_StartOfMessage(void)
{

  CEC->CR |= CEC_CR_TXSOM;
}

void CEC_EndOfMessage(void)
{

  CEC->CR |= CEC_CR_TXEOM;
}

void CEC_ITConfig(uint16_t CEC_IT, FunctionalState NewState)
{
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_CEC_IT(CEC_IT));

  if (NewState != DISABLE)
  {

    CEC->IER |= CEC_IT;
  }
  else
  {
    CEC_IT =~CEC_IT;

    CEC->IER &= CEC_IT;
  }
}

FlagStatus CEC_GetFlagStatus(uint16_t CEC_FLAG)
{
  FlagStatus bitstatus = RESET;

  assert_param(IS_CEC_GET_FLAG(CEC_FLAG));

  if ((CEC->ISR & CEC_FLAG) != (uint16_t)RESET)
  {

    bitstatus = SET;
  }
  else
  {

    bitstatus = RESET;
  }

  return  bitstatus;
}

void CEC_ClearFlag(uint32_t CEC_FLAG)
{
  assert_param(IS_CEC_CLEAR_FLAG(CEC_FLAG));

  CEC->ISR = CEC_FLAG;
}

ITStatus CEC_GetITStatus(uint16_t CEC_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t enablestatus = 0;

  assert_param(IS_CEC_GET_IT(CEC_IT));

  enablestatus = (CEC->IER & CEC_IT);

  if (((CEC->ISR & CEC_IT) != (uint32_t)RESET) && enablestatus)
  {

    bitstatus = SET;
  }
  else
  {

    bitstatus = RESET;
  }

  return  bitstatus;
}

void CEC_ClearITPendingBit(uint16_t CEC_IT)
{
  assert_param(IS_CEC_IT(CEC_IT));

  CEC->ISR = CEC_IT;
}

#endif
