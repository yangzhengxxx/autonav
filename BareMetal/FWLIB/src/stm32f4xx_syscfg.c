#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_rcc.h"

#define SYSCFG_OFFSET             (SYSCFG_BASE - PERIPH_BASE)

#define MEMRMP_OFFSET             SYSCFG_OFFSET
#define UFB_MODE_BitNumber        ((uint8_t)0x8)
#define UFB_MODE_BB               (PERIPH_BB_BASE + (MEMRMP_OFFSET * 32) + (UFB_MODE_BitNumber * 4))

#define PMC_OFFSET                (SYSCFG_OFFSET + 0x04)
#define MII_RMII_SEL_BitNumber    ((uint8_t)0x17)
#define PMC_MII_RMII_SEL_BB       (PERIPH_BB_BASE + (PMC_OFFSET * 32) + (MII_RMII_SEL_BitNumber * 4))

#define CMPCR_OFFSET              (SYSCFG_OFFSET + 0x20)
#define CMP_PD_BitNumber          ((uint8_t)0x00)
#define CMPCR_CMP_PD_BB           (PERIPH_BB_BASE + (CMPCR_OFFSET * 32) + (CMP_PD_BitNumber * 4))

#define MCHDLYCR_OFFSET            (SYSCFG_OFFSET + 0x30)
#define BSCKSEL_BIT_NUMBER         POSITION_VAL(SYSCFG_MCHDLYCR_BSCKSEL)
#define MCHDLYCR_BSCKSEL_BB        (uint32_t)(PERIPH_BB_BASE + (MCHDLYCR_OFFSET * 32) + (BSCKSEL_BIT_NUMBER * 4))

void SYSCFG_DeInit(void)
{
   RCC_APB2PeriphResetCmd(RCC_APB2Periph_SYSCFG, ENABLE);
   RCC_APB2PeriphResetCmd(RCC_APB2Periph_SYSCFG, DISABLE);
}

void SYSCFG_MemoryRemapConfig(uint8_t SYSCFG_MemoryRemap)
{

  assert_param(IS_SYSCFG_MEMORY_REMAP_CONFING(SYSCFG_MemoryRemap));

  SYSCFG->MEMRMP = SYSCFG_MemoryRemap;
}

void SYSCFG_MemorySwappingBank(FunctionalState NewState)
{

  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(__IO uint32_t *) UFB_MODE_BB = (uint32_t)NewState;
}

void SYSCFG_EXTILineConfig(uint8_t EXTI_PortSourceGPIOx, uint8_t EXTI_PinSourcex)
{
  uint32_t tmp = 0x00;

  assert_param(IS_EXTI_PORT_SOURCE(EXTI_PortSourceGPIOx));
  assert_param(IS_EXTI_PIN_SOURCE(EXTI_PinSourcex));

  tmp = ((uint32_t)0x0F) << (0x04 * (EXTI_PinSourcex & (uint8_t)0x03));
  SYSCFG->EXTICR[EXTI_PinSourcex >> 0x02] &= ~tmp;
  SYSCFG->EXTICR[EXTI_PinSourcex >> 0x02] |= (((uint32_t)EXTI_PortSourceGPIOx) << (0x04 * (EXTI_PinSourcex & (uint8_t)0x03)));
}

void SYSCFG_ETH_MediaInterfaceConfig(uint32_t SYSCFG_ETH_MediaInterface)
{
  assert_param(IS_SYSCFG_ETH_MEDIA_INTERFACE(SYSCFG_ETH_MediaInterface));

  *(__IO uint32_t *) PMC_MII_RMII_SEL_BB = SYSCFG_ETH_MediaInterface;
}

void SYSCFG_CompensationCellCmd(FunctionalState NewState)
{

  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(__IO uint32_t *) CMPCR_CMP_PD_BB = (uint32_t)NewState;
}

FlagStatus SYSCFG_GetCompensationCellStatus(void)
{
  FlagStatus bitstatus = RESET;

  if ((SYSCFG->CMPCR & SYSCFG_CMPCR_READY ) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

#if defined(STM32F410xx) || defined(STM32F412xG) || defined(STM32F413_423xx)

void SYSCFG_BreakConfig(uint32_t SYSCFG_Break)
{

  assert_param(IS_SYSCFG_LOCK_CONFIG(SYSCFG_Break));

  SYSCFG->CFGR2 |= (uint32_t) SYSCFG_Break;
}
#endif

#if defined(STM32F413_423xx)

void DFSDM_BitstreamClock_SourceSelection(uint32_t source)
{
  uint32_t tmp = 0;

  tmp = SYSCFG->MCHDLYCR;
  tmp = (tmp &(~SYSCFG_MCHDLYCR_BSCKSEL));

  SYSCFG->MCHDLYCR  = (tmp|source);
}

void DFSDM_DisableDelayClock(uint32_t MCHDLY)
{
  uint32_t tmp = 0;

  tmp = SYSCFG->MCHDLYCR;
  if(MCHDLY == MCHDLY_CLOCK_DFSDM2)
  {
    tmp =tmp &(~SYSCFG_MCHDLYCR_MCHDLY2EN);
  }
  else
  {
    tmp =tmp &(~SYSCFG_MCHDLYCR_MCHDLY1EN);
  }

  SYSCFG->MCHDLYCR  = tmp;
}

void DFSDM_EnableDelayClock(uint32_t MCHDLY)
{
  uint32_t tmp = 0;

  tmp = SYSCFG->MCHDLYCR;
  tmp = tmp & ~MCHDLY;

  SYSCFG->MCHDLYCR  = (tmp|MCHDLY);
}

void DFSDM_ClockIn_SourceSelection(uint32_t source)
{
  uint32_t tmp = 0;

  tmp = SYSCFG->MCHDLYCR;
  if((source == DFSDM2_CKIN_PAD) || (source == DFSDM2_CKIN_DM))
  {
    tmp =  (tmp & ~SYSCFG_MCHDLYCR_DFSDM2CFG);
  }
  else
  {
    tmp =  (tmp & ~SYSCFG_MCHDLYCR_DFSDM1CFG);
  }

  SYSCFG->MCHDLYCR |= (source|tmp);
}

void DFSDM_ClockOut_SourceSelection(uint32_t source)
{
  uint32_t tmp = 0;

  tmp = SYSCFG->MCHDLYCR;

  if((source == DFSDM2_CKOUT_DFSDM2) || (source == DFSDM2_CKOUT_M27))
  {
    tmp =  (tmp & ~SYSCFG_MCHDLYCR_DFSDM2CKOSEL);
  }
  else
  {
    tmp =  (tmp & ~SYSCFG_MCHDLYCR_DFSDM1CKOSEL);
  }

  SYSCFG->MCHDLYCR |= (source|tmp);
}

void DFSDM_DataIn0_SourceSelection(uint32_t source)
{
  uint32_t tmp = 0;

  tmp = SYSCFG->MCHDLYCR;

  if((source == DATAIN0_DFSDM2_PAD)|| (source == DATAIN0_DFSDM2_DATAIN1))
  {
    tmp =  (tmp & ~SYSCFG_MCHDLYCR_DFSDM2D0SEL);
  }
  else
  {
    tmp =  (tmp & ~SYSCFG_MCHDLYCR_DFSDM1D0SEL);
  }
  SYSCFG->MCHDLYCR |= (source|tmp);
}

void DFSDM_DataIn2_SourceSelection(uint32_t source)
{
  uint32_t tmp = 0;

  tmp = SYSCFG->MCHDLYCR;

  if((source == DATAIN2_DFSDM2_PAD)|| (source == DATAIN2_DFSDM2_DATAIN3))
  {
    tmp =  (tmp & ~SYSCFG_MCHDLYCR_DFSDM2D2SEL);
  }
  else
  {
    tmp =  (tmp & ~SYSCFG_MCHDLYCR_DFSDM1D2SEL);
  }
  SYSCFG->MCHDLYCR |= (source|tmp);
}

void DFSDM_DataIn4_SourceSelection(uint32_t source)
{
  uint32_t tmp = 0;

  tmp = SYSCFG->MCHDLYCR;
  tmp =  (tmp & ~SYSCFG_MCHDLYCR_DFSDM2D4SEL);

  SYSCFG->MCHDLYCR |= (source|tmp);
}

void DFSDM_DataIn6_SourceSelection(uint32_t source)
{
  uint32_t tmp = 0;

  tmp = SYSCFG->MCHDLYCR;

  tmp =  (tmp & ~SYSCFG_MCHDLYCR_DFSDM2D6SEL);

  SYSCFG->MCHDLYCR |= (source|tmp);
}

void DFSDM1_BitStreamClk_Config(uint32_t source)
{
  uint32_t tmp = 0;

  tmp = SYSCFG->MCHDLYCR;

  if ((source == DFSDM1_CLKIN0_TIM4OC2) || (source == DFSDM1_CLKIN2_TIM4OC2))
  {
    tmp =  (tmp & ~SYSCFG_MCHDLYCR_DFSDM1CK02SEL);
  }
  else
  {
    tmp =  (tmp & ~SYSCFG_MCHDLYCR_DFSDM1CK13SEL);
  }

  SYSCFG->MCHDLYCR |= (source|tmp);
}

void DFSDM2_BitStreamClk_Config(uint32_t source)
{
  uint32_t tmp = 0;

  tmp = SYSCFG->MCHDLYCR;

  if ((source == DFSDM2_CLKIN0_TIM3OC4) || (source == DFSDM2_CLKIN4_TIM3OC4))
  {
    tmp =  (tmp & ~SYSCFG_MCHDLYCR_DFSDM2CK04SEL);
  }
  else if ((source == DFSDM2_CLKIN1_TIM3OC3) || (source == DFSDM2_CLKIN5_TIM3OC3))
  {
    tmp =  (tmp & ~SYSCFG_MCHDLYCR_DFSDM2CK15SEL);

  }else  if ((source == DFSDM2_CLKIN2_TIM3OC2) || (source == DFSDM2_CLKIN6_TIM3OC2))
  {
    tmp =  (tmp & ~SYSCFG_MCHDLYCR_DFSDM2CK26SEL);
  }
  else
  {
    tmp =  (tmp & ~SYSCFG_MCHDLYCR_DFSDM2CK37SEL);
  }

  SYSCFG->MCHDLYCR |= (source|tmp);
}

#endif
