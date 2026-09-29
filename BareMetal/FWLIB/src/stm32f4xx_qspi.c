#include "stm32f4xx_qspi.h"

#if defined(STM32F412xG) || defined(STM32F413_423xx) || defined(STM32F446xx) || defined(STM32F469_479xx)

#define QSPI_CR_CLEAR_MASK                           0x00FFFFCF
#define QSPI_DCR_CLEAR_MASK                          0xFFE0F7FE
#define QSPI_CCR_CLEAR_MASK                          0x90800000
#define QSPI_PIR_CLEAR_MASK                          0xFFFF0000
#define QSPI_LPTR_CLEAR_MASK                         0xFFFF0000
#define QSPI_CCR_CLEAR_INSTRUCTION_MASK              0xFFFFFF00
#define QSPI_CCR_CLEAR_DCY_MASK                      0xFFC3FFFF
#define QSPI_CR_CLEAR_FIFOTHRESHOLD_MASK             0xFFFFF0FF
#define QSPI_CR_INTERRUPT_MASK                       0x001F0000
#define QSPI_SR_INTERRUPT_MASK                       0x0000001F
#define QSPI_FSR_INTERRUPT_MASK                      0x0000001B

void QSPI_DeInit(void)
{

  RCC_AHB3PeriphResetCmd(RCC_AHB3Periph_QSPI, ENABLE);

  RCC_AHB3PeriphResetCmd(RCC_AHB3Periph_QSPI, DISABLE);
}

void QSPI_StructInit(QSPI_InitTypeDef* QSPI_InitStruct)
{

  QSPI_InitStruct->QSPI_SShift = QSPI_SShift_NoShift ;

  QSPI_InitStruct->QSPI_Prescaler = 0 ;

  QSPI_InitStruct->QSPI_CKMode = QSPI_CKMode_Mode0 ;

  QSPI_InitStruct->QSPI_CSHTime = QSPI_CSHTime_1Cycle ;

  QSPI_InitStruct->QSPI_FSize = 0 ;

  QSPI_InitStruct->QSPI_FSelect = QSPI_FSelect_1 ;

  QSPI_InitStruct->QSPI_DFlash = QSPI_DFlash_Disable ;
}

void QSPI_ComConfig_StructInit(QSPI_ComConfig_InitTypeDef* QSPI_ComConfig_InitStruct)
{

  QSPI_ComConfig_InitStruct->QSPI_ComConfig_DDRMode = QSPI_ComConfig_DDRMode_Disable ;

  QSPI_ComConfig_InitStruct->QSPI_ComConfig_DHHC = QSPI_ComConfig_DHHC_Disable ;

  QSPI_ComConfig_InitStruct->QSPI_ComConfig_SIOOMode = QSPI_ComConfig_SIOOMode_Disable ;

  QSPI_ComConfig_InitStruct->QSPI_ComConfig_FMode = QSPI_ComConfig_FMode_Indirect_Write ;

  QSPI_ComConfig_InitStruct->QSPI_ComConfig_DMode = QSPI_ComConfig_DMode_NoData ;

  QSPI_ComConfig_InitStruct->QSPI_ComConfig_DummyCycles = 0 ;

  QSPI_ComConfig_InitStruct->QSPI_ComConfig_ABSize = QSPI_ComConfig_ABSize_8bit ;

  QSPI_ComConfig_InitStruct->QSPI_ComConfig_ABMode = QSPI_ComConfig_ABMode_NoAlternateByte ;

  QSPI_ComConfig_InitStruct->QSPI_ComConfig_ADSize = QSPI_ComConfig_ADSize_8bit ;

  QSPI_ComConfig_InitStruct->QSPI_ComConfig_ADMode = QSPI_ComConfig_ADMode_NoAddress ;

  QSPI_ComConfig_InitStruct->QSPI_ComConfig_IMode = QSPI_ComConfig_IMode_NoInstruction ;

  QSPI_ComConfig_InitStruct->QSPI_ComConfig_Ins = 0 ;
}

void QSPI_Init(QSPI_InitTypeDef* QSPI_InitStruct)
{
  uint32_t tmpreg = 0;

  assert_param(IS_QSPI_SSHIFT(QSPI_InitStruct->QSPI_SShift));
  assert_param(IS_QSPI_PRESCALER(QSPI_InitStruct->QSPI_Prescaler));
  assert_param(IS_QSPI_CKMODE(QSPI_InitStruct->QSPI_CKMode));
  assert_param(IS_QSPI_CSHTIME(QSPI_InitStruct->QSPI_CSHTime));
  assert_param(IS_QSPI_FSIZE(QSPI_InitStruct->QSPI_FSize));
	assert_param(IS_QSPI_FSEL(QSPI_InitStruct->QSPI_FSelect));
	assert_param(IS_QSPI_DFM(QSPI_InitStruct->QSPI_DFlash));

  tmpreg = QUADSPI->CR;

  tmpreg &= QSPI_CR_CLEAR_MASK;

  tmpreg |= (uint32_t)(((QSPI_InitStruct->QSPI_Prescaler)<<24)
                        |(QSPI_InitStruct->QSPI_SShift)
	                      |(QSPI_InitStruct->QSPI_FSelect)
	                      |(QSPI_InitStruct->QSPI_DFlash));

  QUADSPI->CR = tmpreg;

  tmpreg = QUADSPI->DCR;

  tmpreg &= QSPI_DCR_CLEAR_MASK;

  tmpreg |= (uint32_t)(((QSPI_InitStruct->QSPI_FSize)<<16)
                        |(QSPI_InitStruct->QSPI_CSHTime)
                        |(QSPI_InitStruct->QSPI_CKMode));

  QUADSPI->DCR = tmpreg;
}

void QSPI_ComConfig_Init(QSPI_ComConfig_InitTypeDef* QSPI_ComConfig_InitStruct)
{
  uint32_t tmpreg = 0;

  assert_param(IS_QSPI_FMODE       (QSPI_ComConfig_InitStruct->QSPI_ComConfig_FMode));
  assert_param(IS_QSPI_SIOOMODE    (QSPI_ComConfig_InitStruct->QSPI_ComConfig_SIOOMode));
  assert_param(IS_QSPI_DMODE       (QSPI_ComConfig_InitStruct->QSPI_ComConfig_DMode));
  assert_param(IS_QSPI_DCY         (QSPI_ComConfig_InitStruct->QSPI_ComConfig_DummyCycles));
  assert_param(IS_QSPI_ABSIZE      (QSPI_ComConfig_InitStruct->QSPI_ComConfig_ABSize));
  assert_param(IS_QSPI_ABMODE      (QSPI_ComConfig_InitStruct->QSPI_ComConfig_ABMode));
  assert_param(IS_QSPI_ADSIZE      (QSPI_ComConfig_InitStruct->QSPI_ComConfig_ADSize));
  assert_param(IS_QSPI_ADMODE      (QSPI_ComConfig_InitStruct->QSPI_ComConfig_ADMode));
  assert_param(IS_QSPI_IMODE       (QSPI_ComConfig_InitStruct->QSPI_ComConfig_IMode));
  assert_param(IS_QSPI_INSTRUCTION (QSPI_ComConfig_InitStruct->QSPI_ComConfig_Ins));
	assert_param(IS_QSPI_DDRMODE     (QSPI_ComConfig_InitStruct->QSPI_ComConfig_DDRMode));
	assert_param(IS_QSPI_DHHC        (QSPI_ComConfig_InitStruct->QSPI_ComConfig_DHHC));

  tmpreg = QUADSPI->CCR;

  tmpreg &= QSPI_CCR_CLEAR_MASK;

  tmpreg |=  (uint32_t)( (QSPI_ComConfig_InitStruct->QSPI_ComConfig_FMode)
                       | (QSPI_ComConfig_InitStruct->QSPI_ComConfig_DDRMode)
											 | (QSPI_ComConfig_InitStruct->QSPI_ComConfig_DHHC)
                       | (QSPI_ComConfig_InitStruct->QSPI_ComConfig_SIOOMode)
                       | (QSPI_ComConfig_InitStruct->QSPI_ComConfig_DMode)
                       | (QSPI_ComConfig_InitStruct->QSPI_ComConfig_ABSize)
                       | (QSPI_ComConfig_InitStruct->QSPI_ComConfig_ABMode)
                       | (QSPI_ComConfig_InitStruct->QSPI_ComConfig_ADSize)
                       | (QSPI_ComConfig_InitStruct->QSPI_ComConfig_ADMode)
                       | (QSPI_ComConfig_InitStruct->QSPI_ComConfig_IMode)
                       | (QSPI_ComConfig_InitStruct->QSPI_ComConfig_Ins)
                       |((QSPI_ComConfig_InitStruct->QSPI_ComConfig_DummyCycles)<<18));

  QUADSPI->CCR = tmpreg;
}

void QSPI_Cmd(FunctionalState NewState)
{

  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    QUADSPI->CR |= QUADSPI_CR_EN;
  }
  else
  {

    QUADSPI->CR &= ~ QUADSPI_CR_EN;
  }
}

void QSPI_AutoPollingMode_Config(uint32_t QSPI_Match, uint32_t QSPI_Mask , uint32_t QSPI_Match_Mode)
{

  assert_param(IS_QSPI_PMM(QSPI_Match_Mode));

  if ((QUADSPI->SR & QUADSPI_SR_BUSY) == RESET)

  {

    QUADSPI->PSMAR = QSPI_Match ;

    QUADSPI->PSMKR = QSPI_Mask ;

    if(QSPI_Match_Mode)

    {

      QUADSPI->CR |= QUADSPI_CR_PMM;
    }
    else

    {

      QUADSPI->CR &= ~ QUADSPI_CR_PMM;
    }
  }
}

void QSPI_AutoPollingMode_SetInterval(uint32_t QSPI_Interval)
{
  uint32_t tmpreg = 0;

  assert_param(IS_QSPI_PIR(QSPI_Interval));

  if ((QUADSPI->SR & QUADSPI_SR_BUSY) == RESET)

  {

    tmpreg = QUADSPI->PIR ;

    tmpreg &= QSPI_PIR_CLEAR_MASK ;

    tmpreg |= QSPI_Interval;

    QUADSPI->PIR = tmpreg;
  }
}

void QSPI_MemoryMappedMode_SetTimeout(uint32_t QSPI_Timeout)
{
  uint32_t tmpreg = 0;

  assert_param(IS_QSPI_TIMEOUT(QSPI_Timeout));

  if ((QUADSPI->SR & QUADSPI_SR_BUSY) == RESET)

  {

    tmpreg = QUADSPI->LPTR ;

    tmpreg &= QSPI_LPTR_CLEAR_MASK ;

    tmpreg |= QSPI_Timeout;

    QUADSPI->LPTR = tmpreg;
  }
}

void QSPI_SetAddress(uint32_t QSPI_Address)
{
  if((QUADSPI->SR & QUADSPI_SR_BUSY) == RESET)

  {

    QUADSPI->AR = QSPI_Address;
  }
}

void QSPI_SetAlternateByte(uint32_t QSPI_AlternateByte)
{
  if((QUADSPI->SR & QUADSPI_SR_BUSY) == RESET)

  {

    QUADSPI->ABR = QSPI_AlternateByte;
  }
}

void QSPI_SetFIFOThreshold(uint32_t QSPI_FIFOThreshold)
{
  uint32_t tmpreg = 0;

  assert_param(IS_QSPI_FIFOTHRESHOLD(QSPI_FIFOThreshold));

  tmpreg = QUADSPI->CR ;

  tmpreg &= QSPI_CR_CLEAR_FIFOTHRESHOLD_MASK ;

  tmpreg |= (QSPI_FIFOThreshold << 8);

  QUADSPI->CR = tmpreg;
}

void QSPI_SetDataLength(uint32_t QSPI_DataLength)
{
  if ((QUADSPI->SR & QUADSPI_SR_BUSY) == RESET)

  {

    QUADSPI->DLR = QSPI_DataLength;
  }
}

void QSPI_TimeoutCounterCmd(FunctionalState NewState)
{

  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if ((QUADSPI->SR & QUADSPI_SR_BUSY) == RESET)

  {
    if (NewState != DISABLE)
    {

      QUADSPI->CR |= QUADSPI_CR_TCEN;
    }
    else
    {

      QUADSPI->CR &= ~ QUADSPI_CR_TCEN;
    }
  }
}

void QSPI_AutoPollingModeStopCmd(FunctionalState NewState)
{

  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if ((QUADSPI->SR & QUADSPI_SR_BUSY) == RESET)

  {
    if (NewState != DISABLE)
    {

      QUADSPI->CR |= QUADSPI_CR_APMS;
    }
    else
    {

      QUADSPI->CR &= ~ QUADSPI_CR_APMS;
    }
  }
}

void QSPI_AbortRequest(void)
{

  QUADSPI->CR |= QUADSPI_CR_ABORT;
}

void QSPI_SendData8(uint8_t Data)
{
  uint32_t quadspibase = 0;

  quadspibase = (uint32_t)QUADSPI;
  quadspibase += 0x20;

  *(__IO uint8_t *) quadspibase = Data;
}

void QSPI_SendData16(uint16_t Data)
{
  uint32_t quadspibase = 0;

  quadspibase = (uint32_t)QUADSPI;
  quadspibase += 0x20;

  *(__IO uint16_t *) quadspibase = Data;
}

void QSPI_SendData32(uint32_t Data)
{
  QUADSPI->DR = Data;
}

uint8_t QSPI_ReceiveData8(void)
{
  uint32_t quadspibase = 0;

  quadspibase = (uint32_t)QUADSPI;
  quadspibase += 0x20;

  return *(__IO uint8_t *) quadspibase;
}

uint16_t QSPI_ReceiveData16(void)
{
  uint32_t quadspibase = 0;

  quadspibase = (uint32_t)QUADSPI;
  quadspibase += 0x20;

  return *(__IO uint16_t *) quadspibase;
}

uint32_t QSPI_ReceiveData32(void)
{
  return QUADSPI->DR;
}

void QSPI_DMACmd(FunctionalState NewState)
{

  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    QUADSPI->CR |= QUADSPI_CR_DMAEN;
  }
  else
  {

    QUADSPI->CR &= ~ QUADSPI_CR_DMAEN;
  }
}

void QSPI_ITConfig(uint32_t QSPI_IT, FunctionalState NewState)
{
  uint32_t tmpreg = 0;

  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_QSPI_IT(QSPI_IT));

  tmpreg = QUADSPI->CR ;

  if(NewState != DISABLE)
  {

    tmpreg |= (uint32_t)(QSPI_IT & QSPI_CR_INTERRUPT_MASK);
  }
  else
  {

    tmpreg &= ~(uint32_t)(QSPI_IT & QSPI_CR_INTERRUPT_MASK);
  }

  QUADSPI->CR = tmpreg ;
}

uint32_t QSPI_GetFIFOLevel(void)
{

  return ((QUADSPI->SR & QUADSPI_SR_FLEVEL)>> 8);
}

uint32_t QSPI_GetFMode(void)
{

  return  (QUADSPI->CCR & QUADSPI_CCR_FMODE);
}

FlagStatus QSPI_GetFlagStatus(uint32_t QSPI_FLAG)
{
  FlagStatus bitstatus = RESET;

  assert_param(IS_QSPI_GET_FLAG(QSPI_FLAG));

  if((QUADSPI->SR & QSPI_FLAG) != RESET)
  {

    bitstatus = SET;
  }
  else
  {

    bitstatus = RESET;
  }

  return  bitstatus;
}

void QSPI_ClearFlag(uint32_t QSPI_FLAG)
{

  assert_param(IS_QSPI_CLEAR_FLAG(QSPI_FLAG));

  QUADSPI->FCR = QSPI_FLAG;
}

ITStatus QSPI_GetITStatus(uint32_t QSPI_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t tmpcreg = 0, tmpsreg = 0;

  assert_param(IS_QSPI_IT(QSPI_IT));

  tmpcreg = QUADSPI->CR;
  tmpcreg &= (uint32_t)(QSPI_IT & QSPI_CR_INTERRUPT_MASK);

  tmpsreg = QUADSPI->SR;
  tmpsreg &= (uint32_t)(QSPI_IT & QSPI_SR_INTERRUPT_MASK);

  if((tmpcreg != RESET) && (tmpsreg != RESET))
  {

    bitstatus = SET;
  }
  else
  {

    bitstatus = RESET;
  }

  return bitstatus;
}

void QSPI_ClearITPendingBit(uint32_t QSPI_IT)
{

  assert_param(IS_QSPI_CLEAR_IT(QSPI_IT));

  QUADSPI->FCR = (uint32_t)(QSPI_IT & QSPI_FSR_INTERRUPT_MASK);
}

void QSPI_DualFlashMode_Cmd(FunctionalState NewState)
{

  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    QUADSPI->CR |= QUADSPI_CR_DFM;
  }
  else
  {

    QUADSPI->CR &= ~ QUADSPI_CR_DFM;
  }
}

#endif
