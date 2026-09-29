#include "stm32f4xx_fmpi2c.h"
#include "stm32f4xx_rcc.h"

#if defined(STM32F410xx) || defined(STM32F412xG)|| defined(STM32F413_423xx) || defined(STM32F446xx)

#define CR1_CLEAR_MASK          ((uint32_t)0x00CFE0FF)
#define CR2_CLEAR_MASK          ((uint32_t)0x07FF7FFF)
#define TIMING_CLEAR_MASK       ((uint32_t)0xF0FFFFFF)
#define ERROR_IT_MASK           ((uint32_t)0x00003F00)
#define TC_IT_MASK              ((uint32_t)0x000000C0)

void FMPI2C_DeInit(FMPI2C_TypeDef* FMPI2Cx)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));

  if (FMPI2Cx == FMPI2C1)
  {

    RCC_APB1PeriphResetCmd(RCC_APB1Periph_FMPI2C1, ENABLE);

    RCC_APB1PeriphResetCmd(RCC_APB1Periph_FMPI2C1, DISABLE);
  }
}

void FMPI2C_Init(FMPI2C_TypeDef* FMPI2Cx, FMPI2C_InitTypeDef* FMPI2C_InitStruct)
{
  uint32_t tmpreg = 0;

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FMPI2C_ANALOG_FILTER(FMPI2C_InitStruct->FMPI2C_AnalogFilter));
  assert_param(IS_FMPI2C_DIGITAL_FILTER(FMPI2C_InitStruct->FMPI2C_DigitalFilter));
  assert_param(IS_FMPI2C_MODE(FMPI2C_InitStruct->FMPI2C_Mode));
  assert_param(IS_FMPI2C_OWN_ADDRESS1(FMPI2C_InitStruct->FMPI2C_OwnAddress1));
  assert_param(IS_FMPI2C_ACK(FMPI2C_InitStruct->FMPI2C_Ack));
  assert_param(IS_FMPI2C_ACKNOWLEDGE_ADDRESS(FMPI2C_InitStruct->FMPI2C_AcknowledgedAddress));

  FMPI2Cx->CR1 &= (uint32_t)~((uint32_t)FMPI2C_CR1_PE);

  tmpreg = FMPI2Cx->CR1;

  tmpreg &= CR1_CLEAR_MASK;

  tmpreg |= (uint32_t)FMPI2C_InitStruct->FMPI2C_AnalogFilter |(FMPI2C_InitStruct->FMPI2C_DigitalFilter << 8);

  FMPI2Cx->CR1 = tmpreg;

  FMPI2Cx->TIMINGR = FMPI2C_InitStruct->FMPI2C_Timing & TIMING_CLEAR_MASK;

  FMPI2Cx->CR1 |= FMPI2C_CR1_PE;

  tmpreg = 0;

  FMPI2Cx->OAR1 = (uint32_t)tmpreg;

  FMPI2Cx->OAR2 = (uint32_t)tmpreg;

  tmpreg = (uint32_t)((uint32_t)FMPI2C_InitStruct->FMPI2C_AcknowledgedAddress | \
                      (uint32_t)FMPI2C_InitStruct->FMPI2C_OwnAddress1);

  FMPI2Cx->OAR1 = tmpreg;

  FMPI2Cx->OAR1 |= FMPI2C_OAR1_OA1EN;

  tmpreg = FMPI2C_InitStruct->FMPI2C_Mode;

  FMPI2Cx->CR1 |= tmpreg;

  tmpreg = FMPI2Cx->CR2;

  tmpreg &= CR2_CLEAR_MASK;

  tmpreg |= FMPI2C_InitStruct->FMPI2C_Ack;

  FMPI2Cx->CR2 = tmpreg;
}

void FMPI2C_StructInit(FMPI2C_InitTypeDef* FMPI2C_InitStruct)
{

  FMPI2C_InitStruct->FMPI2C_Timing = 0;

  FMPI2C_InitStruct->FMPI2C_AnalogFilter = FMPI2C_AnalogFilter_Enable;

  FMPI2C_InitStruct->FMPI2C_DigitalFilter = 0;

  FMPI2C_InitStruct->FMPI2C_Mode = FMPI2C_Mode_FMPI2C;

  FMPI2C_InitStruct->FMPI2C_OwnAddress1 = 0;

  FMPI2C_InitStruct->FMPI2C_Ack = FMPI2C_Ack_Disable;

  FMPI2C_InitStruct->FMPI2C_AcknowledgedAddress = FMPI2C_AcknowledgedAddress_7bit;
}

void FMPI2C_Cmd(FMPI2C_TypeDef* FMPI2Cx, FunctionalState NewState)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {

    FMPI2Cx->CR1 |= FMPI2C_CR1_PE;
  }
  else
  {

    FMPI2Cx->CR1 &= (uint32_t)~((uint32_t)FMPI2C_CR1_PE);
  }
}

void FMPI2C_SoftwareResetCmd(FMPI2C_TypeDef* FMPI2Cx)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));

  FMPI2Cx->CR1 &= (uint32_t)~((uint32_t)FMPI2C_CR1_PE);

  *(__IO uint32_t *)(uint32_t)FMPI2Cx;

  FMPI2Cx->CR1 |= FMPI2C_CR1_PE;
}

void FMPI2C_ITConfig(FMPI2C_TypeDef* FMPI2Cx, uint32_t FMPI2C_IT, FunctionalState NewState)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_FMPI2C_CONFIG_IT(FMPI2C_IT));

  if (NewState != DISABLE)
  {

    FMPI2Cx->CR1 |= FMPI2C_IT;
  }
  else
  {

    FMPI2Cx->CR1 &= (uint32_t)~((uint32_t)FMPI2C_IT);
  }
}

void FMPI2C_StretchClockCmd(FMPI2C_TypeDef* FMPI2Cx, FunctionalState NewState)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    FMPI2Cx->CR1 &= (uint32_t)~((uint32_t)FMPI2C_CR1_NOSTRETCH);
  }
  else
  {

    FMPI2Cx->CR1 |= FMPI2C_CR1_NOSTRETCH;
  }
}

void FMPI2C_DualAddressCmd(FMPI2C_TypeDef* FMPI2Cx, FunctionalState NewState)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    FMPI2Cx->OAR2 |= FMPI2C_OAR2_OA2EN;
  }
  else
  {

    FMPI2Cx->OAR2 &= (uint32_t)~((uint32_t)FMPI2C_OAR2_OA2EN);
  }
}

void FMPI2C_OwnAddress2Config(FMPI2C_TypeDef* FMPI2Cx, uint16_t Address, uint8_t Mask)
{
  uint32_t tmpreg = 0;

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FMPI2C_OWN_ADDRESS2(Address));
  assert_param(IS_FMPI2C_OWN_ADDRESS2_MASK(Mask));

  tmpreg = FMPI2Cx->OAR2;

  tmpreg &= (uint32_t)~((uint32_t)(FMPI2C_OAR2_OA2 | FMPI2C_OAR2_OA2MSK));

  tmpreg |= (uint32_t)(((uint32_t)Address & FMPI2C_OAR2_OA2) | \
            (((uint32_t)Mask << 8) & FMPI2C_OAR2_OA2MSK)) ;

  FMPI2Cx->OAR2 = tmpreg;
}

void FMPI2C_GeneralCallCmd(FMPI2C_TypeDef* FMPI2Cx, FunctionalState NewState)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    FMPI2Cx->CR1 |= FMPI2C_CR1_GCEN;
  }
  else
  {

    FMPI2Cx->CR1 &= (uint32_t)~((uint32_t)FMPI2C_CR1_GCEN);
  }
}

void FMPI2C_SlaveByteControlCmd(FMPI2C_TypeDef* FMPI2Cx, FunctionalState NewState)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    FMPI2Cx->CR1 |= FMPI2C_CR1_SBC;
  }
  else
  {

    FMPI2Cx->CR1 &= (uint32_t)~((uint32_t)FMPI2C_CR1_SBC);
  }
}

void FMPI2C_SlaveAddressConfig(FMPI2C_TypeDef* FMPI2Cx, uint16_t Address)
{
  uint32_t tmpreg = 0;

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FMPI2C_SLAVE_ADDRESS(Address));

  tmpreg = FMPI2Cx->CR2;

  tmpreg &= (uint32_t)~((uint32_t)FMPI2C_CR2_SADD);

  tmpreg |= (uint32_t)((uint32_t)Address & FMPI2C_CR2_SADD);

  FMPI2Cx->CR2 = tmpreg;
}

void FMPI2C_10BitAddressingModeCmd(FMPI2C_TypeDef* FMPI2Cx, FunctionalState NewState)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    FMPI2Cx->CR2 |= FMPI2C_CR2_ADD10;
  }
  else
  {

    FMPI2Cx->CR2 &= (uint32_t)~((uint32_t)FMPI2C_CR2_ADD10);
  }
}

void FMPI2C_AutoEndCmd(FMPI2C_TypeDef* FMPI2Cx, FunctionalState NewState)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    FMPI2Cx->CR2 |= FMPI2C_CR2_AUTOEND;
  }
  else
  {

    FMPI2Cx->CR2 &= (uint32_t)~((uint32_t)FMPI2C_CR2_AUTOEND);
  }
}

void FMPI2C_ReloadCmd(FMPI2C_TypeDef* FMPI2Cx, FunctionalState NewState)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    FMPI2Cx->CR2 |= FMPI2C_CR2_RELOAD;
  }
  else
  {

    FMPI2Cx->CR2 &= (uint32_t)~((uint32_t)FMPI2C_CR2_RELOAD);
  }
}

void FMPI2C_NumberOfBytesConfig(FMPI2C_TypeDef* FMPI2Cx, uint8_t Number_Bytes)
{
  uint32_t tmpreg = 0;

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));

  tmpreg = FMPI2Cx->CR2;

  tmpreg &= (uint32_t)~((uint32_t)FMPI2C_CR2_NBYTES);

  tmpreg |= (uint32_t)(((uint32_t)Number_Bytes << 16 ) & FMPI2C_CR2_NBYTES);

  FMPI2Cx->CR2 = tmpreg;
}

void FMPI2C_MasterRequestConfig(FMPI2C_TypeDef* FMPI2Cx, uint16_t FMPI2C_Direction)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FMPI2C_DIRECTION(FMPI2C_Direction));

  if (FMPI2C_Direction == FMPI2C_Direction_Transmitter)
  {

    FMPI2Cx->CR2 &= (uint32_t)~((uint32_t)FMPI2C_CR2_RD_WRN);
  }
  else
  {

    FMPI2Cx->CR2 |= FMPI2C_CR2_RD_WRN;
  }
}

void FMPI2C_GenerateSTART(FMPI2C_TypeDef* FMPI2Cx, FunctionalState NewState)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    FMPI2Cx->CR2 |= FMPI2C_CR2_START;
  }
  else
  {

    FMPI2Cx->CR2 &= (uint32_t)~((uint32_t)FMPI2C_CR2_START);
  }
}

void FMPI2C_GenerateSTOP(FMPI2C_TypeDef* FMPI2Cx, FunctionalState NewState)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    FMPI2Cx->CR2 |= FMPI2C_CR2_STOP;
  }
  else
  {

    FMPI2Cx->CR2 &= (uint32_t)~((uint32_t)FMPI2C_CR2_STOP);
  }
}

void FMPI2C_10BitAddressHeaderCmd(FMPI2C_TypeDef* FMPI2Cx, FunctionalState NewState)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    FMPI2Cx->CR2 |= FMPI2C_CR2_HEAD10R;
  }
  else
  {

    FMPI2Cx->CR2 &= (uint32_t)~((uint32_t)FMPI2C_CR2_HEAD10R);
  }
}

void FMPI2C_AcknowledgeConfig(FMPI2C_TypeDef* FMPI2Cx, FunctionalState NewState)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    FMPI2Cx->CR2 &= (uint32_t)~((uint32_t)FMPI2C_CR2_NACK);
  }
  else
  {

    FMPI2Cx->CR2 |= FMPI2C_CR2_NACK;
  }
}

uint8_t FMPI2C_GetAddressMatched(FMPI2C_TypeDef* FMPI2Cx)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));

  return (uint8_t)(((uint32_t)FMPI2Cx->ISR & FMPI2C_ISR_ADDCODE) >> 16) ;
}

uint16_t FMPI2C_GetTransferDirection(FMPI2C_TypeDef* FMPI2Cx)
{
  uint32_t tmpreg = 0;
  uint16_t direction = 0;

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));

  tmpreg = (uint32_t)(FMPI2Cx->ISR & FMPI2C_ISR_DIR);

  if (tmpreg == 0)
  {

    direction = FMPI2C_Direction_Transmitter;
  }
  else
  {

    direction = FMPI2C_Direction_Receiver;
  }
  return direction;
}

void FMPI2C_TransferHandling(FMPI2C_TypeDef* FMPI2Cx, uint16_t Address, uint8_t Number_Bytes, uint32_t ReloadEndMode, uint32_t StartStopMode)
{
  uint32_t tmpreg = 0;

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FMPI2C_SLAVE_ADDRESS(Address));
  assert_param(IS_RELOAD_END_MODE(ReloadEndMode));
  assert_param(IS_START_STOP_MODE(StartStopMode));

  tmpreg = FMPI2Cx->CR2;

  tmpreg &= (uint32_t)~((uint32_t)(FMPI2C_CR2_SADD | FMPI2C_CR2_NBYTES | FMPI2C_CR2_RELOAD | FMPI2C_CR2_AUTOEND | FMPI2C_CR2_RD_WRN | FMPI2C_CR2_START | FMPI2C_CR2_STOP));

  tmpreg |= (uint32_t)(((uint32_t)Address & FMPI2C_CR2_SADD) | (((uint32_t)Number_Bytes << 16 ) & FMPI2C_CR2_NBYTES) | \
            (uint32_t)ReloadEndMode | (uint32_t)StartStopMode);

  FMPI2Cx->CR2 = tmpreg;
}

void FMPI2C_SMBusAlertCmd(FMPI2C_TypeDef* FMPI2Cx, FunctionalState NewState)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    FMPI2Cx->CR1 |= FMPI2C_CR1_ALERTEN;
  }
  else
  {

    FMPI2Cx->CR1 &= (uint32_t)~((uint32_t)FMPI2C_CR1_ALERTEN);
  }
}

void FMPI2C_ClockTimeoutCmd(FMPI2C_TypeDef* FMPI2Cx, FunctionalState NewState)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    FMPI2Cx->TIMEOUTR |= FMPI2C_TIMEOUTR_TIMOUTEN;
  }
  else
  {

    FMPI2Cx->TIMEOUTR &= (uint32_t)~((uint32_t)FMPI2C_TIMEOUTR_TIMOUTEN);
  }
}

void FMPI2C_ExtendedClockTimeoutCmd(FMPI2C_TypeDef* FMPI2Cx, FunctionalState NewState)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    FMPI2Cx->TIMEOUTR |= FMPI2C_TIMEOUTR_TEXTEN;
  }
  else
  {

    FMPI2Cx->TIMEOUTR &= (uint32_t)~((uint32_t)FMPI2C_TIMEOUTR_TEXTEN);
  }
}

void FMPI2C_IdleClockTimeoutCmd(FMPI2C_TypeDef* FMPI2Cx, FunctionalState NewState)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    FMPI2Cx->TIMEOUTR |= FMPI2C_TIMEOUTR_TIDLE;
  }
  else
  {

    FMPI2Cx->TIMEOUTR &= (uint32_t)~((uint32_t)FMPI2C_TIMEOUTR_TIDLE);
  }
}

void FMPI2C_TimeoutAConfig(FMPI2C_TypeDef* FMPI2Cx, uint16_t Timeout)
{
  uint32_t tmpreg = 0;

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FMPI2C_TIMEOUT(Timeout));

  tmpreg = FMPI2Cx->TIMEOUTR;

  tmpreg &= (uint32_t)~((uint32_t)FMPI2C_TIMEOUTR_TIMEOUTA);

  tmpreg |= (uint32_t)((uint32_t)Timeout & FMPI2C_TIMEOUTR_TIMEOUTA) ;

  FMPI2Cx->TIMEOUTR = tmpreg;
}

void FMPI2C_TimeoutBConfig(FMPI2C_TypeDef* FMPI2Cx, uint16_t Timeout)
{
  uint32_t tmpreg = 0;

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FMPI2C_TIMEOUT(Timeout));

  tmpreg = FMPI2Cx->TIMEOUTR;

  tmpreg &= (uint32_t)~((uint32_t)FMPI2C_TIMEOUTR_TIMEOUTB);

  tmpreg |= (uint32_t)(((uint32_t)Timeout << 16) & FMPI2C_TIMEOUTR_TIMEOUTB) ;

  FMPI2Cx->TIMEOUTR = tmpreg;
}

void FMPI2C_CalculatePEC(FMPI2C_TypeDef* FMPI2Cx, FunctionalState NewState)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    FMPI2Cx->CR1 |= FMPI2C_CR1_PECEN;
  }
  else
  {

    FMPI2Cx->CR1 &= (uint32_t)~((uint32_t)FMPI2C_CR1_PECEN);
  }
}

void FMPI2C_PECRequestCmd(FMPI2C_TypeDef* FMPI2Cx, FunctionalState NewState)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    FMPI2Cx->CR1 |= FMPI2C_CR2_PECBYTE;
  }
  else
  {

    FMPI2Cx->CR1 &= (uint32_t)~((uint32_t)FMPI2C_CR2_PECBYTE);
  }
}

uint8_t FMPI2C_GetPEC(FMPI2C_TypeDef* FMPI2Cx)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));

  return (uint8_t)((uint32_t)FMPI2Cx->PECR & FMPI2C_PECR_PEC);
}

uint32_t FMPI2C_ReadRegister(FMPI2C_TypeDef* FMPI2Cx, uint8_t FMPI2C_Register)
{
  __IO uint32_t tmp = 0;

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FMPI2C_REGISTER(FMPI2C_Register));

  tmp = (uint32_t)FMPI2Cx;
  tmp += FMPI2C_Register;

  return (*(__IO uint32_t *) tmp);
}

void FMPI2C_SendData(FMPI2C_TypeDef* FMPI2Cx, uint8_t Data)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));

  FMPI2Cx->TXDR = (uint8_t)Data;
}

uint8_t FMPI2C_ReceiveData(FMPI2C_TypeDef* FMPI2Cx)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));

  return (uint8_t)FMPI2Cx->RXDR;
}

void FMPI2C_DMACmd(FMPI2C_TypeDef* FMPI2Cx, uint32_t FMPI2C_DMAReq, FunctionalState NewState)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_FMPI2C_DMA_REQ(FMPI2C_DMAReq));

  if (NewState != DISABLE)
  {

    FMPI2Cx->CR1 |= FMPI2C_DMAReq;
  }
  else
  {

    FMPI2Cx->CR1 &= (uint32_t)~FMPI2C_DMAReq;
  }
}

FlagStatus FMPI2C_GetFlagStatus(FMPI2C_TypeDef* FMPI2Cx, uint32_t FMPI2C_FLAG)
{
  uint32_t tmpreg = 0;
  FlagStatus bitstatus = RESET;

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FMPI2C_GET_FLAG(FMPI2C_FLAG));

  tmpreg = FMPI2Cx->ISR;

  tmpreg &= FMPI2C_FLAG;

  if(tmpreg != 0)
  {

    bitstatus = SET;
  }
  else
  {

    bitstatus = RESET;
  }
  return bitstatus;
}

void FMPI2C_ClearFlag(FMPI2C_TypeDef* FMPI2Cx, uint32_t FMPI2C_FLAG)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FMPI2C_CLEAR_FLAG(FMPI2C_FLAG));

  FMPI2Cx->ICR = FMPI2C_FLAG;
  }

ITStatus FMPI2C_GetITStatus(FMPI2C_TypeDef* FMPI2Cx, uint32_t FMPI2C_IT)
{
  uint32_t tmpreg = 0;
  ITStatus bitstatus = RESET;
  uint32_t enablestatus = 0;

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FMPI2C_GET_IT(FMPI2C_IT));

  if((uint32_t)(FMPI2C_IT & ERROR_IT_MASK))
  {
    enablestatus = (uint32_t)((FMPI2C_CR1_ERRIE) & (FMPI2Cx->CR1));
  }

  else if((uint32_t)(FMPI2C_IT & TC_IT_MASK))
  {
    enablestatus = (uint32_t)((FMPI2C_CR1_TCIE) & (FMPI2Cx->CR1));
  }
  else
  {
    enablestatus = (uint32_t)((FMPI2C_IT) & (FMPI2Cx->CR1));
  }

  tmpreg = FMPI2Cx->ISR;

  tmpreg &= FMPI2C_IT;

  if((tmpreg != RESET) && enablestatus)
  {

    bitstatus = SET;
  }
  else
  {

    bitstatus = RESET;
  }

  return bitstatus;
}

void FMPI2C_ClearITPendingBit(FMPI2C_TypeDef* FMPI2Cx, uint32_t FMPI2C_IT)
{

  assert_param(IS_FMPI2C_ALL_PERIPH(FMPI2Cx));
  assert_param(IS_FMPI2C_CLEAR_IT(FMPI2C_IT));

  FMPI2Cx->ICR = FMPI2C_IT;
}

#endif
