#include "stm32f4xx_spdifrx.h"
#include "stm32f4xx_rcc.h"

#if defined(STM32F446xx)

#define CR_CLEAR_MASK 0x000000FE7

void SPDIFRX_DeInit(void)
{

  RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPDIFRX, ENABLE);

  RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPDIFRX, DISABLE);
}

void SPDIFRX_Init(SPDIFRX_InitTypeDef* SPDIFRX_InitStruct)
{
  uint32_t tmpreg = 0;

  assert_param(IS_STEREO_MODE(SPDIFRX_InitStruct->SPDIFRX_StereoMode));
  assert_param(IS_SPDIFRX_INPUT_SELECT(SPDIFRX_InitStruct->SPDIFRX_InputSelection));
  assert_param(IS_SPDIFRX_MAX_RETRIES(SPDIFRX_InitStruct->SPDIFRX_Retries));
  assert_param(IS_SPDIFRX_WAIT_FOR_ACTIVITY(SPDIFRX_InitStruct->SPDIFRX_WaitForActivity));
  assert_param(IS_SPDIFRX_CHANNEL(SPDIFRX_InitStruct->SPDIFRX_ChannelSelection));
  assert_param(IS_SPDIFRX_DATA_FORMAT(SPDIFRX_InitStruct->SPDIFRX_DataFormat));

  tmpreg = SPDIFRX->CR;

  tmpreg &= CR_CLEAR_MASK;

  tmpreg |= (uint32_t)(SPDIFRX_InitStruct->SPDIFRX_InputSelection   | SPDIFRX_InitStruct->SPDIFRX_WaitForActivity   |
                       SPDIFRX_InitStruct->SPDIFRX_Retries          | SPDIFRX_InitStruct->SPDIFRX_ChannelSelection  |
                       SPDIFRX_InitStruct->SPDIFRX_DataFormat       | SPDIFRX_InitStruct->SPDIFRX_StereoMode
                       );

  SPDIFRX->CR = tmpreg;
}

void SPDIFRX_StructInit(SPDIFRX_InitTypeDef* SPDIFRX_InitStruct)
{

  SPDIFRX_InitStruct->SPDIFRX_InputSelection = SPDIFRX_Input_IN0;

  SPDIFRX_InitStruct->SPDIFRX_WaitForActivity = SPDIFRX_WaitForActivity_On;

  SPDIFRX_InitStruct->SPDIFRX_Retries = SPDIFRX_16MAX_RETRIES;

  SPDIFRX_InitStruct->SPDIFRX_ChannelSelection = SPDIFRX_Select_Channel_A;

  SPDIFRX_InitStruct->SPDIFRX_DataFormat = SPDIFRX_MSB_DataFormat;

  SPDIFRX_InitStruct->SPDIFRX_StereoMode = SPDIFRX_StereoMode_Enabled;
}

void SPDIFRX_SetPreambleTypeBit(FunctionalState NewState)
{

  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    SPDIFRX->CR |= SPDIFRX_CR_PTMSK;
  }
  else
  {

    SPDIFRX->CR &= ~(SPDIFRX_CR_PTMSK);
  }
}

void SPDIFRX_SetUserDataChannelStatusBits(FunctionalState NewState)
{

  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    SPDIFRX->CR |= SPDIFRX_CR_CUMSK;
  }
  else
  {

    SPDIFRX->CR &= ~(SPDIFRX_CR_CUMSK);
  }
}

void SPDIFRX_SetValidityBit(FunctionalState NewState)
{

  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    SPDIFRX->CR |= SPDIFRX_CR_VMSK;
  }
  else
  {

    SPDIFRX->CR &= ~(SPDIFRX_CR_VMSK);
  }
}

void SPDIFRX_SetParityBit(FunctionalState NewState)
{

  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    SPDIFRX->CR |= SPDIFRX_CR_PMSK;
  }
  else
  {

    SPDIFRX->CR &= ~(SPDIFRX_CR_PMSK);
  }
}

void SPDIFRX_RxDMACmd(FunctionalState NewState)
{

  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    SPDIFRX->CR |= SPDIFRX_CR_RXDMAEN;
  }
  else
  {

    SPDIFRX->CR &= ~(SPDIFRX_CR_RXDMAEN);
  }
}

void SPDIFRX_CbDMACmd(FunctionalState NewState)
{

  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    SPDIFRX->CR |= SPDIFRX_CR_CBDMAEN;
  }
  else
  {

    SPDIFRX->CR &= ~(SPDIFRX_CR_CBDMAEN);
  }
}

void SPDIFRX_Cmd(uint32_t SPDIFRX_State)
{

  assert_param(IS_SPDIFRX_STATE(SPDIFRX_State));

    SPDIFRX->CR &= ~(SPDIFRX_CR_SPDIFEN);

    SPDIFRX->CR |= SPDIFRX_State;
}

void SPDIFRX_ITConfig(uint32_t SPDIFRX_IT, FunctionalState NewState)
{

  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_SPDIFRX_CONFIG_IT(SPDIFRX_IT));

  if (NewState != DISABLE)
  {

    SPDIFRX->IMR |= SPDIFRX_IT;
  }
  else
  {

    SPDIFRX->IMR &= ~(SPDIFRX_IT);
  }
}

FlagStatus SPDIFRX_GetFlagStatus(uint32_t SPDIFRX_FLAG)
{
  FlagStatus bitstatus = RESET;

  assert_param(IS_SPDIFRX_FLAG(SPDIFRX_FLAG));

  if ((SPDIFRX->SR & SPDIFRX_FLAG) != (uint32_t)RESET)
  {

    bitstatus = SET;
  }
  else
  {

    bitstatus = RESET;
  }

  return  bitstatus;
}

void SPDIFRX_ClearFlag(uint32_t SPDIFRX_FLAG)
{

  assert_param(IS_SPDIFRX_CLEAR_FLAG(SPDIFRX_FLAG));

  SPDIFRX->IFCR |= SPDIFRX_FLAG;
}

ITStatus SPDIFRX_GetITStatus(uint32_t SPDIFRX_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t  enablestatus = 0;

  assert_param(IS_SPDIFRX_CONFIG_IT(SPDIFRX_IT));

  enablestatus = (SPDIFRX->IMR & SPDIFRX_IT) ;

  if (((SPDIFRX->SR & SPDIFRX_IT) != (uint32_t)RESET) && (enablestatus != (uint32_t)RESET))
  {

    bitstatus = SET;
  }
  else
  {

    bitstatus = RESET;
  }

  return bitstatus;
}

void SPDIFRX_ClearITPendingBit(uint32_t SPDIFRX_IT)
{

  assert_param(IS_SPDIFRX_CLEAR_FLAG(SPDIFRX_IT));

  SPDIFRX->IFCR |= SPDIFRX_IT;
}

#endif
