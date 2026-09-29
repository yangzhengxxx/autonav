#include "stm32f4xx_dfsdm.h"
#include "stm32f4xx_rcc.h"

#if defined(STM32F412xG) || defined(STM32F413_423xx)

#define CHCFGR_INIT_CLEAR_MASK               (uint32_t) 0xFFFE0F10

void DFSDM_DeInit(void)
{

  RCC_APB2PeriphResetCmd(RCC_APB2Periph_DFSDM1, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_DFSDM1, DISABLE);
#if defined(STM32F413_423xx)
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_DFSDM2, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_DFSDM2, DISABLE);
#endif
}

void DFSDM_TransceiverInit(DFSDM_Channel_TypeDef* DFSDM_Channelx, DFSDM_TransceiverInitTypeDef* DFSDM_TransceiverInitStruct)
{
    uint32_t tmpreg1 = 0;
    uint32_t tmpreg2 = 0;

    assert_param(IS_DFSDM_ALL_CHANNEL(DFSDM_Channelx));
    assert_param(IS_DFSDM_INTERFACE(DFSDM_TransceiverInitStruct->DFSDM_Interface));
    assert_param(IS_DFSDM_Input_MODE(DFSDM_TransceiverInitStruct->DFSDM_Input));
    assert_param(IS_DFSDM_Redirection_STATE(DFSDM_TransceiverInitStruct->DFSDM_Redirection));
    assert_param(IS_DFSDM_PACK_MODE(DFSDM_TransceiverInitStruct->DFSDM_PackingMode));
    assert_param(IS_DFSDM_CLOCK(DFSDM_TransceiverInitStruct->DFSDM_Clock));
    assert_param(IS_DFSDM_DATA_RIGHT_BIT_SHIFT(DFSDM_TransceiverInitStruct->DFSDM_DataRightShift));
    assert_param(IS_DFSDM_OFFSET(DFSDM_TransceiverInitStruct->DFSDM_Offset));
    assert_param(IS_DFSDM_CLK_DETECTOR_STATE(DFSDM_TransceiverInitStruct->DFSDM_CLKAbsenceDetector));
    assert_param(IS_DFSDM_SC_DETECTOR_STATE(DFSDM_TransceiverInitStruct->DFSDM_ShortCircuitDetector));

    tmpreg1 = DFSDM_Channelx->CHCFGR1;

    tmpreg1 &= CHCFGR_INIT_CLEAR_MASK;

    tmpreg1 |= (DFSDM_TransceiverInitStruct->DFSDM_Interface |
                DFSDM_TransceiverInitStruct->DFSDM_Clock |
                DFSDM_TransceiverInitStruct->DFSDM_Input |
                DFSDM_TransceiverInitStruct->DFSDM_Redirection |
                DFSDM_TransceiverInitStruct->DFSDM_PackingMode |
                DFSDM_TransceiverInitStruct->DFSDM_CLKAbsenceDetector |
                DFSDM_TransceiverInitStruct->DFSDM_ShortCircuitDetector);

    DFSDM_Channelx->CHCFGR1 = tmpreg1;

    tmpreg2 = DFSDM_Channelx->CHCFGR2;

    tmpreg2 &= ~(DFSDM_CHCFGR2_DTRBS | DFSDM_CHCFGR2_OFFSET);

    tmpreg2 |= (((DFSDM_TransceiverInitStruct->DFSDM_DataRightShift) <<3 ) |
                ((DFSDM_TransceiverInitStruct->DFSDM_Offset) <<8 ));

    DFSDM_Channelx->CHCFGR2 = tmpreg2;
}

void DFSDM_TransceiverStructInit(DFSDM_TransceiverInitTypeDef* DFSDM_TransceiverInitStruct)
{

    DFSDM_TransceiverInitStruct->DFSDM_Interface = DFSDM_Interface_SPI_FallingEdge;

    DFSDM_TransceiverInitStruct->DFSDM_Clock = DFSDM_Clock_Internal;

    DFSDM_TransceiverInitStruct->DFSDM_DataRightShift = 0x0;

    DFSDM_TransceiverInitStruct->DFSDM_Offset = 0x0;

    DFSDM_TransceiverInitStruct->DFSDM_CLKAbsenceDetector = DFSDM_CLKAbsenceDetector_Enable;
}

void DFSDM_FilterInit(DFSDM_Filter_TypeDef* DFSDMx, DFSDM_FilterInitTypeDef* DFSDM_FilterInitStruct)
{
    uint32_t tmpreg1 = 0;

    assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));
    assert_param(IS_DFSDM_SINC_ORDER(DFSDM_FilterInitStruct->DFSDM_SincOrder));
    assert_param(IS_DFSDM_SINC_OVRSMPL_RATIO(DFSDM_FilterInitStruct->DFSDM_FilterOversamplingRatio));
    assert_param(IS_DFSDM_INTG_OVRSMPL_RATIO(DFSDM_FilterInitStruct->DFSDM_IntegratorOversamplingRatio));

    tmpreg1 = DFSDMx->FLTFCR;

    tmpreg1 &= ~(DFSDM_FLTFCR_FORD | DFSDM_FLTFCR_FOSR | DFSDM_FLTFCR_IOSR);

    tmpreg1 |= (DFSDM_FilterInitStruct->DFSDM_SincOrder |
               ((DFSDM_FilterInitStruct->DFSDM_FilterOversamplingRatio -1) << 16) |
               (DFSDM_FilterInitStruct->DFSDM_IntegratorOversamplingRatio -1));

    DFSDMx->FLTFCR = tmpreg1;
}

void DFSDM_FilterStructInit(DFSDM_FilterInitTypeDef* DFSDM_FilterInitStruct)
{

    DFSDM_FilterInitStruct->DFSDM_SincOrder = DFSDM_SincOrder_Sinc3;

    DFSDM_FilterInitStruct->DFSDM_FilterOversamplingRatio  = 64 ;

    DFSDM_FilterInitStruct->DFSDM_IntegratorOversamplingRatio = 4;
}

#if defined(STM32F412xG)

void DFSDM_Command(FunctionalState NewState)
{

  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    DFSDM1_Channel0 -> CHCFGR1 |= DFSDM_CHCFGR1_DFSDMEN;
  }
  else
  {

    DFSDM1_Channel0 -> CHCFGR1 &= ~(DFSDM_CHCFGR1_DFSDMEN);
  }
}
#endif

#if defined(STM32F413_423xx)

void DFSDM_Cmd(uint32_t Instance, FunctionalState NewState)
{

  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if(Instance == 1)
  {
    if (NewState != DISABLE)
    {

      DFSDM1_Channel0 -> CHCFGR1 |= DFSDM_CHCFGR1_DFSDMEN;
    }
    else
    {

      DFSDM1_Channel0 -> CHCFGR1 &= ~(DFSDM_CHCFGR1_DFSDMEN);
    }
  }
  else
  {
    if (NewState != DISABLE)
    {

      DFSDM2_Channel0 -> CHCFGR1 |= DFSDM_CHCFGR1_DFSDMEN;
    }
    else
    {

      DFSDM2_Channel0 -> CHCFGR1 &= ~(DFSDM_CHCFGR1_DFSDMEN);
    }
  }
}
#endif

void DFSDM_ChannelCmd(DFSDM_Channel_TypeDef* DFSDM_Channelx, FunctionalState NewState)
{

  assert_param(IS_DFSDM_ALL_CHANNEL(DFSDM_Channelx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    DFSDM_Channelx->CHCFGR1 |= DFSDM_CHCFGR1_CHEN;
  }
  else
  {

    DFSDM_Channelx->CHCFGR1 &= ~(DFSDM_CHCFGR1_CHEN);
  }
}

void DFSDM_FilterCmd(DFSDM_Filter_TypeDef* DFSDMx, FunctionalState NewState)
{

  assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    DFSDMx->FLTCR1 |= DFSDM_FLTCR1_DFEN;
  }
  else
  {

    DFSDMx->FLTCR1 &= ~(DFSDM_FLTCR1_DFEN);
  }
}

#if defined(STM32F412xG)

void DFSDM_ConfigClkOutputDivider(uint32_t DFSDM_ClkOutDivision)
{
    uint32_t tmpreg1 = 0;

    assert_param(IS_DFSDM_CLOCK_OUT_DIVIDER(DFSDM_ClkOutDivision));

    tmpreg1 = DFSDM1_Channel0 -> CHCFGR1;

    tmpreg1 &= (uint32_t)(~DFSDM_CHCFGR1_CKOUTDIV);

    tmpreg1 |= (uint32_t)((DFSDM_ClkOutDivision - 1) << 16);

    DFSDM1_Channel0 -> CHCFGR1 = tmpreg1;
}

void DFSDM_ConfigClkOutputSource(uint32_t DFSDM_ClkOutSource)
{
  uint32_t tmpreg1 = 0;

  assert_param(IS_DFSDM_CLOCK_OUT_SOURCE(DFSDM_ClkOutSource));

  tmpreg1 = DFSDM1_Channel0 -> CHCFGR1;

  tmpreg1 &= ~(DFSDM_CHCFGR1_CKOUTSRC);

  tmpreg1 |= DFSDM_ClkOutSource;

  DFSDM1_Channel0 -> CHCFGR1 = tmpreg1;
}
#endif
#if defined(STM32F413_423xx)

void DFSDM_ConfigClkOutputDivider(uint32_t Instance, uint32_t DFSDM_ClkOutDivision)
{
  uint32_t tmpreg1 = 0;

  if(Instance == 1)
  {

    assert_param(IS_DFSDM_CLOCK_OUT_DIVIDER(DFSDM_ClkOutDivision));

    tmpreg1 = DFSDM1_Channel0 -> CHCFGR1;

    tmpreg1 &= (uint32_t)(~DFSDM_CHCFGR1_CKOUTDIV);

    tmpreg1 |= (uint32_t)((DFSDM_ClkOutDivision - 1) << 16);

    DFSDM1_Channel0 -> CHCFGR1 = tmpreg1;
  }
  else
  {

    assert_param(IS_DFSDM_CLOCK_OUT_DIVIDER(DFSDM_ClkOutDivision));

    tmpreg1 = DFSDM2_Channel0 -> CHCFGR1;

    tmpreg1 &= (uint32_t)(~DFSDM_CHCFGR1_CKOUTDIV);

    tmpreg1 |= (uint32_t)((DFSDM_ClkOutDivision - 1) << 16);

    DFSDM2_Channel0 -> CHCFGR1 = tmpreg1;
  }
}

void DFSDM_ConfigClkOutputSource(uint32_t Instance, uint32_t DFSDM_ClkOutSource)
{
  uint32_t tmpreg1 = 0;

  if(Instance == 1)
  {

    assert_param(IS_DFSDM_CLOCK_OUT_SOURCE(DFSDM_ClkOutSource));

    tmpreg1 = DFSDM1_Channel0 -> CHCFGR1;

    tmpreg1 &= ~(DFSDM_CHCFGR1_CKOUTSRC);

    tmpreg1 |= DFSDM_ClkOutSource;

    DFSDM1_Channel0 -> CHCFGR1 = tmpreg1;
  }
  else
  {

    assert_param(IS_DFSDM_CLOCK_OUT_SOURCE(DFSDM_ClkOutSource));

    tmpreg1 = DFSDM2_Channel0 -> CHCFGR1;

    tmpreg1 &= ~(DFSDM_CHCFGR1_CKOUTSRC);

    tmpreg1 |= DFSDM_ClkOutSource;

    DFSDM2_Channel0 -> CHCFGR1 = tmpreg1;
  }
}
#endif

void DFSDM_ConfigBRKAnalogWatchDog(DFSDM_Channel_TypeDef* DFSDM_Channelx, uint32_t DFSDM_SCDBreak_i, FunctionalState NewState)
{

  assert_param(IS_DFSDM_ALL_CHANNEL(DFSDM_Channelx));
  assert_param(IS_DFSDM_SCD_BREAK_SIGNAL(DFSDM_SCDBreak_i));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

     DFSDM_Channelx -> CHAWSCDR |= DFSDM_SCDBreak_i;
  }
  else
  {

    DFSDM_Channelx -> CHAWSCDR &= ~(DFSDM_SCDBreak_i);
  }
}

void DFSDM_ConfigBRKShortCircuitDetector(DFSDM_Channel_TypeDef* DFSDM_Channelx, uint32_t DFSDM_SCDBreak_i, FunctionalState NewState)
{

  assert_param(IS_DFSDM_ALL_CHANNEL(DFSDM_Channelx));
  assert_param(IS_DFSDM_SCD_BREAK_SIGNAL(DFSDM_SCDBreak_i));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

     DFSDM_Channelx -> CHAWSCDR |= DFSDM_SCDBreak_i;
  }
  else
  {

    DFSDM_Channelx -> CHAWSCDR &= ~(DFSDM_SCDBreak_i);
  }
}

void DFSDM_ConfigShortCircuitThreshold(DFSDM_Channel_TypeDef* DFSDM_Channelx, uint32_t DFSDM_SCDThreshold)
{
    uint32_t tmpreg1 = 0;

    assert_param(IS_DFSDM_ALL_CHANNEL(DFSDM_Channelx));
    assert_param(IS_DFSDM_CSD_THRESHOLD_VALUE(DFSDM_SCDThreshold));

    tmpreg1 = DFSDM_Channelx -> CHAWSCDR;

    tmpreg1 &= ~(DFSDM_CHAWSCDR_SCDT);

    tmpreg1 |= DFSDM_SCDThreshold;

    DFSDM_Channelx -> CHAWSCDR = tmpreg1;
}

void DFSDM_ConfigAnalogWatchdog(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_AWDChannelx, uint32_t DFSDM_AWDFastMode)
{
  uint32_t tmpreg1 = 0;
  uint32_t tmpreg2 = 0;

  assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));
  assert_param(IS_DFSDM_AWD_CHANNEL(DFSDM_AWDChannelx));
  assert_param(IS_DFSDM_AWD_MODE(DFSDM_AWDFastMode));

  tmpreg1 = DFSDMx -> FLTCR2;

  tmpreg1 &= ~(DFSDM_FLTCR2_AWDCH);

  tmpreg1 |= DFSDM_AWDChannelx;

  DFSDMx -> FLTCR2 |= tmpreg1;

  tmpreg2 = DFSDMx->FLTCR1;

  tmpreg2 &= ~(DFSDM_FLTCR1_AWFSEL);

  tmpreg2 |= DFSDM_AWDFastMode;

  DFSDMx->FLTCR1 = tmpreg2;
}

void DFSDM_SelectExtremesDetectorChannel(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_ExtremChannelx)
{
  uint32_t tmpreg1 = 0;

  assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));
  assert_param(IS_DFSDM_EXTREM_CHANNEL(DFSDM_ExtremChannelx));

  tmpreg1 = DFSDMx -> FLTCR2;

  tmpreg1 &= ~(DFSDM_FLTCR2_EXCH);

  tmpreg1 |= DFSDM_ExtremChannelx;

  DFSDMx -> FLTCR2 = tmpreg1;
}

int32_t DFSDM_GetRegularConversionData(DFSDM_Filter_TypeDef* DFSDMx)
{
  uint32_t reg = 0;
  int32_t  value = 0;

  assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));

  reg = DFSDMx -> FLTRDATAR;

  value = (((reg & 0xFFFFFF00) >> 8));

  return  value;
}

int32_t DFSDM_GetInjectedConversionData(DFSDM_Filter_TypeDef* DFSDMx)
{
  uint32_t reg = 0;
  int32_t  value = 0;

  assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));

  reg = DFSDMx -> FLTJDATAR;

  value = ((reg & 0xFFFFFF00) >> 8);

  return  value;
}

int32_t DFSDM_GetMaxValue(DFSDM_Filter_TypeDef* DFSDMx)
{
  int32_t value = 0;

  assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));

  value = ((DFSDMx -> FLTEXMAX) >> 8);

  return value;
}

int32_t DFSDM_GetMinValue(DFSDM_Filter_TypeDef* DFSDMx)
{
  int32_t value = 0;

  assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));

  value = ((DFSDMx -> FLTEXMIN) >> 8);

  return value;
}

int32_t DFSDM_GetMaxValueChannel(DFSDM_Filter_TypeDef* DFSDMx)
{

  assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));

  return  ((DFSDMx -> FLTEXMAX) & (~DFSDM_FLTEXMAX_EXMAXCH));
}

int32_t DFSDM_GetMinValueChannel(DFSDM_Filter_TypeDef* DFSDMx)
{

  assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));

  return  ((DFSDMx -> FLTEXMIN) & (~DFSDM_FLTEXMIN_EXMINCH));
}

uint32_t DFSDM_GetConversionTime(DFSDM_Filter_TypeDef* DFSDMx)
{

  assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));

  return  ((DFSDMx -> FLTCNVTIMR >> 4) & 0x0FFFFFFF);
}

void DFSDM_ConfigAWDFilter(DFSDM_Channel_TypeDef* DFSDM_Channelx, uint32_t DFSDM_AWDSincOrder, uint32_t DFSDM_AWDSincOverSampleRatio)
{
    uint32_t tmpreg1 = 0;

    assert_param(IS_DFSDM_ALL_CHANNEL(DFSDM_Channelx));
    assert_param(IS_DFSDM_AWD_SINC_ORDER(DFSDM_AWDSincOrder));
    assert_param(IS_DFSDM_AWD_OVRSMPL_RATIO(DFSDM_AWDSincOverSampleRatio));

    tmpreg1 = DFSDM_Channelx -> CHAWSCDR;

    tmpreg1 &= ~(DFSDM_CHAWSCDR_AWFORD | DFSDM_CHAWSCDR_AWFOSR);

    tmpreg1 |= (DFSDM_AWDSincOrder | ((DFSDM_AWDSincOverSampleRatio -1) << 16)) ;

    DFSDM_Channelx -> CHAWSCDR = tmpreg1;
}

uint32_t DFSDM_GetAWDConversionValue(DFSDM_Channel_TypeDef* DFSDM_Channelx)
{

  assert_param(IS_DFSDM_ALL_CHANNEL(DFSDM_Channelx));

  return  DFSDM_Channelx -> CHWDATAR;
}

void DFSDM_SetAWDThreshold(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_HighThreshold, uint32_t DFSDM_LowThreshold)
{
    uint32_t tmpreg1 = 0;
    uint32_t tmpreg2 = 0;

    assert_param(IS_DFSDM_HIGH_THRESHOLD(DFSDM_HighThreshold));
    assert_param(IS_DFSDM_LOW_THRESHOLD(DFSDM_LowThreshold));

    tmpreg1 = DFSDMx -> FLTAWHTR;

    tmpreg1 &= ~(DFSDM_FLTAWHTR_AWHT);

    tmpreg1 |= (DFSDM_HighThreshold  << 8 );

    DFSDMx -> FLTAWHTR = tmpreg1;

    tmpreg2 = DFSDMx -> FLTAWLTR;

    tmpreg2 &= ~(DFSDM_FLTAWLTR_AWLT);

    tmpreg2 |= (DFSDM_LowThreshold  << 8 );

    DFSDMx -> FLTAWLTR = tmpreg2;
}

void DFSDM_SelectInjectedChannel(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_InjectedChannelx)
{
  uint32_t tmpreg1 = 0;

  assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));
  assert_param(IS_DFSDM_INJECT_CHANNEL(DFSDM_InjectedChannelx));

  tmpreg1 = DFSDMx -> FLTJCHGR;

  tmpreg1 &= ~(DFSDM_FLTJCHGR_JCHG);

  tmpreg1 |= DFSDM_InjectedChannelx;

  DFSDMx -> FLTJCHGR |= tmpreg1;
}

void DFSDM_SelectRegularChannel(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_RegularChannelx)
{
  uint32_t tmpreg1 = 0;

  assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));
  assert_param(IS_DFSDM_REGULAR_CHANNEL(DFSDM_RegularChannelx));

  tmpreg1 = DFSDMx -> FLTCR1;

  tmpreg1 &= ~(DFSDM_FLTCR1_RCH);

  tmpreg1 |= DFSDM_RegularChannelx;

  DFSDMx -> FLTCR1 = tmpreg1;
}

void DFSDM_StartSoftwareInjectedConversion(DFSDM_Filter_TypeDef* DFSDMx)
{

  assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));

  DFSDMx -> FLTCR1 |=  DFSDM_FLTCR1_JSWSTART;
}

void DFSDM_StartSoftwareRegularConversion(DFSDM_Filter_TypeDef* DFSDMx)
{

  assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));

  DFSDMx -> FLTCR1 |=  DFSDM_FLTCR1_RSWSTART;
}

void DFSDM_ConfigInjectedTrigger(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_Trigger, uint32_t DFSDM_TriggerEdge)
{
  uint32_t tmpreg1 = 0;

  assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));

  if (DFSDMx == DFSDM0)
  {
    assert_param(IS_DFSDM0_INJ_TRIGGER(DFSDM_Trigger));
  }
  else
  {
    assert_param(IS_DFSDM1_INJ_TRIGGER(DFSDM_Trigger));
  }

  assert_param(IS_DFSDM_TRIGGER_EDGE(DFSDM_TriggerEdge));

  tmpreg1 = DFSDMx -> FLTCR1;

  tmpreg1 &= ~(DFSDM_FLTCR1_JEXTSEL | DFSDM_FLTCR1_JEXTEN);

  tmpreg1 |= (DFSDM_Trigger | DFSDM_TriggerEdge);

  DFSDMx -> FLTCR1 = tmpreg1;
}

void DFSDM_SynchronousFilter0InjectedStart(DFSDM_Filter_TypeDef* DFSDMx)
{

  assert_param(IS_DFSDM_SYNC_FILTER(DFSDMx));

  DFSDMx -> FLTCR1 |=  DFSDM_FLTCR1_JSYNC;
}

void DFSDM_SynchronousFilter0RegularStart(DFSDM_Filter_TypeDef* DFSDMx)
{

  assert_param(IS_DFSDM_SYNC_FILTER(DFSDMx));

  DFSDMx -> FLTCR1 |=  DFSDM_FLTCR1_RSYNC;
}

void DFSDM_RegularContinuousModeCmd(DFSDM_Filter_TypeDef* DFSDMx, FunctionalState NewState)
{

  assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

 if (NewState != DISABLE)
  {

    DFSDMx -> FLTCR1 |=  DFSDM_FLTCR1_RCONT;
  }
  else
  {

    DFSDMx -> FLTCR1 &=  ~(DFSDM_FLTCR1_RCONT);
  }
}

void DFSDM_FastModeCmd(DFSDM_Filter_TypeDef* DFSDMx, FunctionalState NewState)
{

  assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

 if (NewState != DISABLE)
  {

    DFSDMx -> FLTCR1 |=  DFSDM_FLTCR1_FAST;
  }
  else
  {

    DFSDMx -> FLTCR1 &=  ~(DFSDM_FLTCR1_FAST);
  }
}

void DFSDM_SelectInjectedConversionMode(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_InjectConvMode)
{

  assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));
  assert_param(IS_DFSDM_INJ_CONV_MODE(DFSDM_InjectConvMode));

  DFSDMx -> FLTCR1 &= ~(DFSDM_FLTCR1_JSCAN);

  DFSDMx -> FLTCR1 |= DFSDM_InjectConvMode;
}

void DFSDM_DMATransferConfig(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_DMAConversionMode, FunctionalState NewState)
{

  assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));
  assert_param(IS_DFSDM_CONVERSION_MODE(DFSDM_DMAConversionMode));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

 if (NewState != DISABLE)
  {

    DFSDMx -> FLTCR1 |=  (DFSDM_FLTCR1_JDMAEN << DFSDM_DMAConversionMode) ;
  }
  else
  {

    DFSDMx -> FLTCR1 &=  ~(DFSDM_FLTCR1_JDMAEN << DFSDM_DMAConversionMode);
  }
}

void DFSDM_ITConfig(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_IT, FunctionalState NewState)
 {

  assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));
  assert_param(IS_DFSDM_IT(DFSDM_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    DFSDMx->FLTCR2 |= DFSDM_IT;
  }
  else
  {

    DFSDMx->FLTCR2 &= ~(DFSDM_IT);
  }
}

#if defined(STM32F412xG)

void DFSDM_ITClockAbsenceCmd(FunctionalState NewState)
 {

  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    DFSDM1_0->FLTCR2 |= DFSDM_IT_CKAB;
  }
  else
  {

    DFSDM1_0->FLTCR2 &= ~(DFSDM_IT_CKAB);
  }
}

void DFSDM_ITShortCircuitDetectorCmd(FunctionalState NewState)
 {

  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    DFSDM1_0->FLTCR2 |= DFSDM_IT_SCD;
  }
  else
  {

    DFSDM1_0->FLTCR2 &= ~(DFSDM_IT_SCD);
  }
}
#endif

#if defined(STM32F413_423xx)

void DFSDM_ITClockAbsenceCmd(uint32_t Instance, FunctionalState NewState)
 {

  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if(Instance == 1)
  {
    if (NewState != DISABLE)
    {

      DFSDM1_0->FLTCR2 |= DFSDM_IT_CKAB;
    }
    else
    {

      DFSDM1_0->FLTCR2 &= ~(DFSDM_IT_CKAB);
    }
  }
  else
  {
    if (NewState != DISABLE)
    {

      DFSDM2_0->FLTCR2 |= DFSDM_IT_CKAB;
    }
    else
    {

      DFSDM2_0->FLTCR2 &= ~(DFSDM_IT_CKAB);
    }
  }
}

void DFSDM_ITShortCircuitDetectorCmd(uint32_t Instance, FunctionalState NewState)
 {

  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if(Instance == 1)
  {
    if (NewState != DISABLE)
    {

      DFSDM1_0->FLTCR2 |= DFSDM_IT_SCD;
    }
    else
    {

      DFSDM1_0->FLTCR2 &= ~(DFSDM_IT_SCD);
    }
  }
  else
  {
    if (NewState != DISABLE)
    {

      DFSDM2_0->FLTCR2 |= DFSDM_IT_SCD;
    }
    else
    {

      DFSDM2_0->FLTCR2 &= ~(DFSDM_IT_SCD);
    }
  }

}
#endif

FlagStatus DFSDM_GetFlagStatus(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_FLAG)
{
  ITStatus bitstatus = RESET;

  assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));
  assert_param(IS_DFSDM_FLAG(DFSDM_FLAG));

  if ((DFSDMx->FLTISR & DFSDM_FLAG) != RESET )
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

#if defined(STM32F412xG)

FlagStatus DFSDM_GetClockAbsenceFlagStatus(uint32_t DFSDM_FLAG_CLKAbsence)
{
  ITStatus bitstatus = RESET;

  assert_param(IS_DFSDM_CLK_ABS_FLAG(DFSDM_FLAG_CLKAbsence));

  if((DFSDM1_0->FLTISR & DFSDM_FLAG_CLKAbsence) != RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

FlagStatus DFSDM_GetShortCircuitFlagStatus(uint32_t DFSDM_FLAG_SCD)
{
  ITStatus bitstatus = RESET;

  assert_param(IS_DFSDM_SCD_FLAG(DFSDM_FLAG_SCD));

  if ((DFSDM1_0->FLTISR & DFSDM_FLAG_SCD) != RESET)
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
#if defined(STM32F413_423xx)

FlagStatus DFSDM_GetClockAbsenceFlagStatus(uint32_t Instance, uint32_t DFSDM_FLAG_CLKAbsence)
{
  ITStatus bitstatus = RESET;

  assert_param(IS_DFSDM_CLK_ABS_FLAG(DFSDM_FLAG_CLKAbsence));

  if(Instance == 1)
  {
    if((DFSDM1_0->FLTISR & DFSDM_FLAG_CLKAbsence) != RESET)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
  else
  {

    assert_param(IS_DFSDM_CLK_ABS_FLAG(DFSDM_FLAG_CLKAbsence));

    if((DFSDM2_0->FLTISR & DFSDM_FLAG_CLKAbsence) != RESET)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
  return bitstatus;
}

FlagStatus DFSDM_GetShortCircuitFlagStatus(uint32_t Instance, uint32_t DFSDM_FLAG_SCD)
{
  ITStatus bitstatus = RESET;

  assert_param(IS_DFSDM_SCD_FLAG(DFSDM_FLAG_SCD));

  if(Instance == 1)
  {
    if ((DFSDM1_0->FLTISR & DFSDM_FLAG_SCD) != RESET)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
  else
  {
    if ((DFSDM2_0->FLTISR & DFSDM_FLAG_SCD) != RESET)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
  return bitstatus;
}
#endif

FlagStatus DFSDM_GetWatchdogFlagStatus(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_AWDChannelx, uint8_t DFSDM_Threshold)
{
  ITStatus bitstatus = RESET;

  assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));
  assert_param(IS_DFSDM_Threshold(DFSDM_Threshold));
  assert_param(IS_DFSDM_AWD_CHANNEL(DFSDM_AWDChannelx));

  if ((DFSDMx->FLTAWSR & ((DFSDM_AWDChannelx >> 16) << DFSDM_Threshold) ) != RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

void DFSDM_ClearFlag(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_CLEARF)
{

  assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));
  assert_param(IS_DFSDM_CLEAR_FLAG(DFSDM_CLEARF));

  DFSDMx->FLTICR |= DFSDM_CLEARF;
}

#if defined(STM32F412xG)

void DFSDM_ClearClockAbsenceFlag(uint32_t DFSDM_CLEARF_CLKAbsence)
{

  assert_param(IS_DFSDM_CLK_ABS_CLEARF(DFSDM_CLEARF_CLKAbsence));

  DFSDM1_0->FLTICR |= DFSDM_CLEARF_CLKAbsence;
}

void DFSDM_ClearShortCircuitFlag(uint32_t DFSDM_CLEARF_SCD)
{

  assert_param(IS_DFSDM_SCD_CHANNEL_FLAG(DFSDM_CLEARF_SCD));

  DFSDM1_0->FLTICR |= DFSDM_CLEARF_SCD;
}
#endif

#if defined(STM32F413_423xx)

void DFSDM_ClearClockAbsenceFlag(uint32_t Instance, uint32_t DFSDM_CLEARF_CLKAbsence)
{

  assert_param(IS_DFSDM_CLK_ABS_CLEARF(DFSDM_CLEARF_CLKAbsence));

  if(Instance == 1)
  {

    DFSDM1_0->FLTICR |= DFSDM_CLEARF_CLKAbsence;
  }
  else
  {

    DFSDM2_0->FLTICR |= DFSDM_CLEARF_CLKAbsence;
  }
}

void DFSDM_ClearShortCircuitFlag(uint32_t Instance, uint32_t DFSDM_CLEARF_SCD)
{

  assert_param(IS_DFSDM_SCD_CHANNEL_FLAG(DFSDM_CLEARF_SCD));

  if(Instance == 1)
  {

    DFSDM1_0->FLTICR |= DFSDM_CLEARF_SCD;
  }
  else
  {

    DFSDM2_0->FLTICR |= DFSDM_CLEARF_SCD;
  }
}
#endif

void DFSDM_ClearAnalogWatchdogFlag(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_AWDChannelx, uint8_t DFSDM_Threshold)
{

  assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));
  assert_param(IS_DFSDM_Threshold(DFSDM_Threshold));
  assert_param(IS_DFSDM_AWD_CHANNEL(DFSDM_AWDChannelx));

  if ((DFSDMx->FLTAWSR & ((DFSDM_AWDChannelx >> 16) << DFSDM_Threshold) ) != RESET)
  {

    DFSDMx->FLTAWCFR |= (DFSDM_AWDChannelx >> 16) << DFSDM_Threshold;
  }
}

ITStatus DFSDM_GetITStatus(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t itstatus = 0x0, itenable = 0x0;

  assert_param(IS_DFSDM_ALL_FILTER(DFSDMx));
  assert_param(IS_DFSDM_IT(DFSDM_IT));

  itstatus = DFSDMx->FLTISR & DFSDM_IT;

  itenable = DFSDMx->FLTCR2 & DFSDM_IT;

  if ((itstatus != RESET) && (itenable != RESET))
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

#if defined(STM32F412xG)

ITStatus DFSDM_GetClockAbsenceITStatus(uint32_t DFSDM_IT_CLKAbsence)
{
  ITStatus bitstatus = RESET;
  uint32_t itstatus = 0x0, itenable = 0x0;

  assert_param(IS_DFSDM_CLK_ABS_IT(DFSDM_IT_CLKAbsence));

  itstatus = DFSDM0->FLTISR & DFSDM_IT_CLKAbsence;

  itenable = DFSDM0->FLTCR2 & DFSDM_IT_CKAB;

  if ((itstatus != RESET) && (itenable != RESET))
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

ITStatus DFSDM_GetShortCircuitITStatus(uint32_t DFSDM_IT_SCR)
{
  ITStatus bitstatus = RESET;
  uint32_t itstatus = 0x0, itenable = 0x0;

  assert_param(IS_DFSDM_SCD_IT(DFSDM_IT_SCR));

  itstatus = DFSDM0->FLTISR & DFSDM_IT_SCR;

  itenable = DFSDM0->FLTCR2 & DFSDM_IT_SCD;

  if ((itstatus != RESET) && (itenable != RESET))
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

#if defined(STM32F413_423xx)

ITStatus DFSDM_GetClockAbsenceITStatus(uint32_t Instance, uint32_t DFSDM_IT_CLKAbsence)
{
  ITStatus bitstatus = RESET;
  uint32_t itstatus = 0x0, itenable = 0x0;

  assert_param(IS_DFSDM_CLK_ABS_IT(DFSDM_IT_CLKAbsence));

  if(Instance == 1)
  {

    itstatus = DFSDM1_0->FLTISR & DFSDM_IT_CLKAbsence;

    itenable = DFSDM1_0->FLTCR2 & DFSDM_IT_CKAB;
  }
  else
  {

    itstatus = DFSDM2_0->FLTISR & DFSDM_IT_CLKAbsence;

    itenable = DFSDM1_0->FLTCR2 & DFSDM_IT_CKAB;
  }

  if ((itstatus != RESET) && (itenable != RESET))
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

ITStatus DFSDM_GetShortCircuitITStatus(uint32_t Instance, uint32_t DFSDM_IT_SCR)
{
  ITStatus bitstatus = RESET;
  uint32_t itstatus = 0x0, itenable = 0x0;

  assert_param(IS_DFSDM_SCD_IT(DFSDM_IT_SCR));

  if(Instance == 1)
  {

    itstatus = DFSDM1_0->FLTISR & DFSDM_IT_SCR;

    itenable = DFSDM1_0->FLTCR2 & DFSDM_IT_SCD;
  }
  else
  {

    itstatus = DFSDM2_0->FLTISR & DFSDM_IT_SCR;

    itenable = DFSDM2_0->FLTCR2 & DFSDM_IT_SCD;
  }

  if ((itstatus != RESET) && (itenable != RESET))
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

#endif
