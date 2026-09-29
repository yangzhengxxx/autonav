#include "stm32f4xx_dsi.h"

#if defined(STM32F469_479xx)

#define DSI_TIMEOUT_VALUE ((uint32_t)1000)

#define DSI_ERROR_ACK_MASK (DSI_ISR0_AE0 | DSI_ISR0_AE1 | DSI_ISR0_AE2 | DSI_ISR0_AE3 | \
                            DSI_ISR0_AE4 | DSI_ISR0_AE5 | DSI_ISR0_AE6 | DSI_ISR0_AE7 | \
                            DSI_ISR0_AE8 | DSI_ISR0_AE9 | DSI_ISR0_AE10 | DSI_ISR0_AE11 | \
                            DSI_ISR0_AE12 | DSI_ISR0_AE13 | DSI_ISR0_AE14 | DSI_ISR0_AE15)
#define DSI_ERROR_PHY_MASK (DSI_ISR0_PE0 | DSI_ISR0_PE1 | DSI_ISR0_PE2 | DSI_ISR0_PE3 | DSI_ISR0_PE4)
#define DSI_ERROR_TX_MASK  DSI_ISR1_TOHSTX
#define DSI_ERROR_RX_MASK  DSI_ISR1_TOLPRX
#define DSI_ERROR_ECC_MASK (DSI_ISR1_ECCSE | DSI_ISR1_ECCME)
#define DSI_ERROR_CRC_MASK DSI_ISR1_CRCE
#define DSI_ERROR_PSE_MASK DSI_ISR1_PSE
#define DSI_ERROR_EOT_MASK DSI_ISR1_EOTPE
#define DSI_ERROR_OVF_MASK DSI_ISR1_LPWRE
#define DSI_ERROR_GEN_MASK (DSI_ISR1_GCWRE | DSI_ISR1_GPWRE | DSI_ISR1_GPTXE | DSI_ISR1_GPRDE | DSI_ISR1_GPRXE)

#define DSI_MAX_RETURN_PKT_SIZE ((uint32_t)0x00000037)

static void DSI_ConfigPacketHeader(DSI_TypeDef *DSIx, uint32_t ChannelID, uint32_t DataType, uint32_t Data0, uint32_t Data1);

void DSI_DeInit(DSI_TypeDef *DSIx)
{

  DSIx->WCR &= ~DSI_WCR_DSIEN;

  DSIx->CR &= ~DSI_CR_EN;

  DSIx->PCTLR &= ~(DSI_PCTLR_CKE | DSI_PCTLR_DEN);

  DSIx->WRPCR &= ~DSI_WRPCR_PLLEN;

  DSIx->WRPCR &= ~DSI_WRPCR_REGEN;

  assert_param(IS_DSI_ALL_PERIPH(DSIx));
  if(DSIx == DSI)
  {

    RCC_APB2PeriphResetCmd(RCC_APB2Periph_DSI, ENABLE);

    RCC_APB2PeriphResetCmd(RCC_APB2Periph_DSI, DISABLE);
  }
}

void DSI_Init(DSI_TypeDef *DSIx,DSI_InitTypeDef* DSI_InitStruct, DSI_PLLInitTypeDef *PLLInit)
{
  uint32_t unitIntervalx4 = 0;
  uint32_t tempIDF = 0;

  assert_param(IS_DSI_PLL_NDIV(PLLInit->PLLNDIV));
  assert_param(IS_DSI_PLL_IDF(PLLInit->PLLIDF));
  assert_param(IS_DSI_PLL_ODF(PLLInit->PLLODF));
  assert_param(IS_DSI_AUTO_CLKLANE_CONTROL(DSI_InitStruct->AutomaticClockLaneControl));
  assert_param(IS_DSI_NUMBER_OF_LANES(DSI_InitStruct->NumberOfLanes));

  DSIx->WRPCR |= DSI_WRPCR_REGEN;

  while(DSI_GetFlagStatus(DSIx, DSI_FLAG_RRS) == RESET )
  {}

  DSIx->WRPCR &= ~(DSI_WRPCR_PLL_NDIV | DSI_WRPCR_PLL_IDF | DSI_WRPCR_PLL_ODF);
  DSIx->WRPCR |= (((PLLInit->PLLNDIV)<<2) | ((PLLInit->PLLIDF)<<11) | ((PLLInit->PLLODF)<<16));

  DSIx->WRPCR |= DSI_WRPCR_PLLEN;

  while(DSI_GetFlagStatus(DSIx, DSI_FLAG_PLLLS) == RESET)
  {}

  DSIx->PCTLR |= (DSI_PCTLR_CKE | DSI_PCTLR_DEN);

  DSIx->CLCR &= ~(DSI_CLCR_DPCC | DSI_CLCR_ACR);
  DSIx->CLCR |= (DSI_CLCR_DPCC | DSI_InitStruct->AutomaticClockLaneControl);

  DSIx->PCONFR &= ~DSI_PCONFR_NL;
  DSIx->PCONFR |= DSI_InitStruct->NumberOfLanes;

  DSIx->CCR &= ~DSI_CCR_TXECKDIV;
  DSIx->CCR = DSI_InitStruct->TXEscapeCkdiv;

  tempIDF = (PLLInit->PLLIDF > 0) ? PLLInit->PLLIDF : 1;
  unitIntervalx4 = (4000000 * tempIDF * (1 << PLLInit->PLLODF)) / ((HSE_VALUE/1000) * PLLInit->PLLNDIV);

  DSIx->WPCR[0] &= ~DSI_WPCR0_UIX4;
  DSIx->WPCR[0] |= unitIntervalx4;

  DSIx->IER[0] = 0;
  DSIx->IER[1] = 0;
}

void DSI_StructInit(DSI_InitTypeDef* DSI_InitStruct, DSI_HOST_TimeoutTypeDef* DSI_HOST_TimeoutInitStruct)
{

  DSI_InitStruct->AutomaticClockLaneControl = DSI_AUTO_CLK_LANE_CTRL_DISABLE;

  DSI_InitStruct->NumberOfLanes = DSI_ONE_DATA_LANE;

  DSI_InitStruct->TXEscapeCkdiv = 0;

  DSI_HOST_TimeoutInitStruct->TimeoutCkdiv = 0;

  DSI_HOST_TimeoutInitStruct->HighSpeedTransmissionTimeout = 0;

  DSI_HOST_TimeoutInitStruct->LowPowerReceptionTimeout = 0;

  DSI_HOST_TimeoutInitStruct->HighSpeedReadTimeout = 0;

  DSI_HOST_TimeoutInitStruct->LowPowerReadTimeout = 0;

  DSI_HOST_TimeoutInitStruct->HighSpeedWriteTimeout = 0;

  DSI_HOST_TimeoutInitStruct->HighSpeedWritePrespMode = 0;

  DSI_HOST_TimeoutInitStruct->LowPowerWriteTimeout = 0;

  DSI_HOST_TimeoutInitStruct->BTATimeout = 0;
}

void DSI_SetGenericVCID(DSI_TypeDef *DSIx, uint32_t VirtualChannelID)
{

  DSIx->GVCIDR &= ~DSI_GVCIDR_VCID;
  DSIx->GVCIDR |= VirtualChannelID;
}

void DSI_ConfigVideoMode(DSI_TypeDef *DSIx, DSI_VidCfgTypeDef *VidCfg)
{

  assert_param(IS_DSI_COLOR_CODING(VidCfg->ColorCoding));
  assert_param(IS_DSI_VIDEO_MODE_TYPE(VidCfg->Mode));
  assert_param(IS_DSI_LP_COMMAND(VidCfg->LPCommandEnable));
  assert_param(IS_DSI_LP_HFP(VidCfg->LPHorizontalFrontPorchEnable));
  assert_param(IS_DSI_LP_HBP(VidCfg->LPHorizontalBackPorchEnable));
  assert_param(IS_DSI_LP_VACTIVE(VidCfg->LPVerticalActiveEnable));
  assert_param(IS_DSI_LP_VFP(VidCfg->LPVerticalFrontPorchEnable));
  assert_param(IS_DSI_LP_VBP(VidCfg->LPVerticalBackPorchEnable));
  assert_param(IS_DSI_LP_VSYNC(VidCfg->LPVerticalSyncActiveEnable));
  assert_param(IS_DSI_FBTAA(VidCfg->FrameBTAAcknowledgeEnable));
  assert_param(IS_DSI_DE_POLARITY(VidCfg->DEPolarity));
  assert_param(IS_DSI_VSYNC_POLARITY(VidCfg->VSPolarity));
  assert_param(IS_DSI_HSYNC_POLARITY(VidCfg->HSPolarity));

  if(VidCfg->ColorCoding == DSI_RGB666)
  {
    assert_param(IS_DSI_LOOSELY_PACKED(VidCfg->LooselyPacked));
  }

  DSIx->MCR &= ~DSI_MCR_CMDM;
  DSIx->WCFGR &= ~DSI_WCFGR_DSIM;

  DSIx->VMCR &= ~DSI_VMCR_VMT;
  DSIx->VMCR |= VidCfg->Mode;

  DSIx->VPCR &= ~DSI_VPCR_VPSIZE;
  DSIx->VPCR |= VidCfg->PacketSize;

  DSIx->VCCR &= ~DSI_VCCR_NUMC;
  DSIx->VCCR |= VidCfg->NumberOfChunks;

  DSIx->VNPCR &= ~DSI_VNPCR_NPSIZE;
  DSIx->VNPCR |= VidCfg->NullPacketSize;

  DSIx->LVCIDR &= ~DSI_LVCIDR_VCID;
  DSIx->LVCIDR |= VidCfg->VirtualChannelID;

  DSIx->LPCR &= ~(DSI_LPCR_DEP | DSI_LPCR_VSP | DSI_LPCR_HSP);
  DSIx->LPCR |= (VidCfg->DEPolarity | VidCfg->VSPolarity | VidCfg->HSPolarity);

  DSIx->LCOLCR &= ~DSI_LCOLCR_COLC;
  DSIx->LCOLCR |= VidCfg->ColorCoding;

  DSIx->WCFGR &= ~DSI_WCFGR_COLMUX;
  DSIx->WCFGR |= ((VidCfg->ColorCoding)<<1);

  if(VidCfg->ColorCoding == DSI_RGB666)
  {
    DSIx->LCOLCR &= ~DSI_LCOLCR_LPE;
    DSIx->LCOLCR |= VidCfg->LooselyPacked;
  }

  DSIx->VHSACR &= ~DSI_VHSACR_HSA;
  DSIx->VHSACR |= VidCfg->HorizontalSyncActive;

  DSIx->VHBPCR &= ~DSI_VHBPCR_HBP;
  DSIx->VHBPCR |= VidCfg->HorizontalBackPorch;

  DSIx->VLCR &= ~DSI_VLCR_HLINE;
  DSIx->VLCR |= VidCfg->HorizontalLine;

  DSIx->VVSACR &= ~DSI_VVSACR_VSA;
  DSIx->VVSACR |= VidCfg->VerticalSyncActive;

  DSIx->VVBPCR &= ~DSI_VVBPCR_VBP;
  DSIx->VVBPCR |= VidCfg->VerticalBackPorch;

  DSIx->VVFPCR &= ~DSI_VVFPCR_VFP;
  DSIx->VVFPCR |= VidCfg->VerticalFrontPorch;

  DSIx->VVACR &= ~DSI_VVACR_VA;
  DSIx->VVACR |= VidCfg->VerticalActive;

  DSIx->VMCR &= ~DSI_VMCR_LPCE;
  DSIx->VMCR |= VidCfg->LPCommandEnable;

  DSIx->LPMCR &= ~DSI_LPMCR_LPSIZE;
  DSIx->LPMCR |= ((VidCfg->LPLargestPacketSize)<<16);

  DSIx->LPMCR &= ~DSI_LPMCR_VLPSIZE;
  DSIx->LPMCR |= VidCfg->LPVACTLargestPacketSize;

  DSIx->VMCR &= ~DSI_VMCR_LPHFPE;
  DSIx->VMCR |= VidCfg->LPHorizontalFrontPorchEnable;

  DSIx->VMCR &= ~DSI_VMCR_LPHBPE;
  DSIx->VMCR |= VidCfg->LPHorizontalBackPorchEnable;

  DSIx->VMCR &= ~DSI_VMCR_LPVAE;
  DSIx->VMCR |= VidCfg->LPVerticalActiveEnable;

  DSIx->VMCR &= ~DSI_VMCR_LPVFPE;
  DSIx->VMCR |= VidCfg->LPVerticalFrontPorchEnable;

  DSIx->VMCR &= ~DSI_VMCR_LPVBPE;
  DSIx->VMCR |= VidCfg->LPVerticalBackPorchEnable;

  DSIx->VMCR &= ~DSI_VMCR_LPVSAE;
  DSIx->VMCR |= VidCfg->LPVerticalSyncActiveEnable;

  DSIx->VMCR &= ~DSI_VMCR_FBTAAE;
  DSIx->VMCR |= VidCfg->FrameBTAAcknowledgeEnable;
}

void DSI_ConfigAdaptedCommandMode(DSI_TypeDef *DSIx, DSI_CmdCfgTypeDef *CmdCfg)
{

  assert_param(IS_DSI_COLOR_CODING(CmdCfg->ColorCoding));
  assert_param(IS_DSI_TE_SOURCE(CmdCfg->TearingEffectSource));
  assert_param(IS_DSI_TE_POLARITY(CmdCfg->TearingEffectPolarity));
  assert_param(IS_DSI_AUTOMATIC_REFRESH(CmdCfg->AutomaticRefresh));
  assert_param(IS_DSI_VS_POLARITY(CmdCfg->VSyncPol));
  assert_param(IS_DSI_TE_ACK_REQUEST(CmdCfg->TEAcknowledgeRequest));
  assert_param(IS_DSI_DE_POLARITY(CmdCfg->DEPolarity));
  assert_param(IS_DSI_VSYNC_POLARITY(CmdCfg->VSPolarity));
  assert_param(IS_DSI_HSYNC_POLARITY(CmdCfg->HSPolarity));

  DSIx->MCR |= DSI_MCR_CMDM;
  DSIx->WCFGR &= ~DSI_WCFGR_DSIM;
  DSIx->WCFGR |= DSI_WCFGR_DSIM;

  DSIx->LVCIDR &= ~DSI_LVCIDR_VCID;
  DSIx->LVCIDR |= CmdCfg->VirtualChannelID;

  DSIx->LPCR &= ~(DSI_LPCR_DEP | DSI_LPCR_VSP | DSI_LPCR_HSP);
  DSIx->LPCR |= (CmdCfg->DEPolarity | CmdCfg->VSPolarity | CmdCfg->HSPolarity);

  DSIx->LCOLCR &= ~DSI_LCOLCR_COLC;
  DSIx->LCOLCR |= CmdCfg->ColorCoding;

  DSIx->WCFGR &= ~DSI_WCFGR_COLMUX;
  DSIx->WCFGR |= ((CmdCfg->ColorCoding)<<1);

  DSIx->LCCR &= ~DSI_LCCR_CMDSIZE;
  DSIx->LCCR |= CmdCfg->CommandSize;

  DSIx->WCFGR &= ~(DSI_WCFGR_TESRC | DSI_WCFGR_TEPOL | DSI_WCFGR_AR | DSI_WCFGR_VSPOL);
  DSIx->WCFGR |= (CmdCfg->TearingEffectSource | CmdCfg->TearingEffectPolarity | CmdCfg->AutomaticRefresh | CmdCfg->VSyncPol);

  DSIx->CMCR &= ~DSI_CMCR_TEARE;
  DSIx->CMCR |= CmdCfg->TEAcknowledgeRequest;

  DSI_ITConfig(DSIx, DSI_IT_TE, ENABLE);

  DSI_ITConfig(DSIx, DSI_IT_ER, ENABLE);
}

void DSI_ConfigCommand(DSI_TypeDef *DSIx, DSI_LPCmdTypeDef *LPCmd)
{
  assert_param(IS_DSI_LP_GSW0P(LPCmd->LPGenShortWriteNoP));
  assert_param(IS_DSI_LP_GSW1P(LPCmd->LPGenShortWriteOneP));
  assert_param(IS_DSI_LP_GSW2P(LPCmd->LPGenShortWriteTwoP));
  assert_param(IS_DSI_LP_GSR0P(LPCmd->LPGenShortReadNoP));
  assert_param(IS_DSI_LP_GSR1P(LPCmd->LPGenShortReadOneP));
  assert_param(IS_DSI_LP_GSR2P(LPCmd->LPGenShortReadTwoP));
  assert_param(IS_DSI_LP_GLW(LPCmd->LPGenLongWrite));
  assert_param(IS_DSI_LP_DSW0P(LPCmd->LPDcsShortWriteNoP));
  assert_param(IS_DSI_LP_DSW1P(LPCmd->LPDcsShortWriteOneP));
  assert_param(IS_DSI_LP_DSR0P(LPCmd->LPDcsShortReadNoP));
  assert_param(IS_DSI_LP_DLW(LPCmd->LPDcsLongWrite));
  assert_param(IS_DSI_LP_MRDP(LPCmd->LPMaxReadPacket));
  assert_param(IS_DSI_ACK_REQUEST(LPCmd->AcknowledgeRequest));

  DSIx->CMCR &= ~(DSI_CMCR_GSW0TX |\
                            DSI_CMCR_GSW1TX |\
                            DSI_CMCR_GSW2TX |\
                            DSI_CMCR_GSR0TX |\
                            DSI_CMCR_GSR1TX |\
                            DSI_CMCR_GSR2TX |\
                            DSI_CMCR_GLWTX  |\
                            DSI_CMCR_DSW0TX |\
                            DSI_CMCR_DSW1TX |\
                            DSI_CMCR_DSR0TX |\
                            DSI_CMCR_DLWTX  |\
                            DSI_CMCR_MRDPS);
  DSIx->CMCR |= (LPCmd->LPGenShortWriteNoP  |\
                           LPCmd->LPGenShortWriteOneP |\
                           LPCmd->LPGenShortWriteTwoP |\
                           LPCmd->LPGenShortReadNoP   |\
                           LPCmd->LPGenShortReadOneP  |\
                           LPCmd->LPGenShortReadTwoP  |\
                           LPCmd->LPGenLongWrite      |\
                           LPCmd->LPDcsShortWriteNoP  |\
                           LPCmd->LPDcsShortWriteOneP |\
                           LPCmd->LPDcsShortReadNoP   |\
                           LPCmd->LPDcsLongWrite      |\
                           LPCmd->LPMaxReadPacket);

  DSIx->CMCR &= ~DSI_CMCR_ARE;
  DSIx->CMCR |= LPCmd->AcknowledgeRequest;
}

void DSI_ConfigFlowControl(DSI_TypeDef *DSIx, uint32_t FlowControl)
{

  assert_param(IS_DSI_FLOW_CONTROL(FlowControl));

  DSIx->PCR &= ~DSI_FLOW_CONTROL_ALL;
  DSIx->PCR |= FlowControl;
}

void DSI_ConfigPhyTimer(DSI_TypeDef *DSIx, DSI_PHY_TimerTypeDef *PhyTimers)
{
  uint32_t maxTime = 0;

  maxTime = (PhyTimers->ClockLaneLP2HSTime > PhyTimers->ClockLaneHS2LPTime)? PhyTimers->ClockLaneLP2HSTime: PhyTimers->ClockLaneHS2LPTime;

  DSIx->CLTCR &= ~(DSI_CLTCR_LP2HS_TIME | DSI_CLTCR_HS2LP_TIME);
  DSIx->CLTCR |= (maxTime | ((maxTime)<<16));

  DSIx->DLTCR &= ~(DSI_DLTCR_MRD_TIME | DSI_DLTCR_LP2HS_TIME | DSI_DLTCR_HS2LP_TIME);
  DSIx->DLTCR |= (PhyTimers->DataLaneMaxReadTime | ((PhyTimers->DataLaneLP2HSTime)<<16) | ((PhyTimers->DataLaneHS2LPTime)<<24));

  DSIx->PCONFR &= ~DSI_PCONFR_SW_TIME;
  DSIx->PCONFR |= ((PhyTimers->StopWaitTime)<<8);
}

void DSI_ConfigHostTimeouts(DSI_TypeDef *DSIx, DSI_HOST_TimeoutTypeDef *HostTimeouts)
{

  DSIx->CCR &= ~DSI_CCR_TOCKDIV;
  DSIx->CCR = ((HostTimeouts->TimeoutCkdiv)<<8);

  DSIx->TCCR[0] &= ~DSI_TCCR0_HSTX_TOCNT;
  DSIx->TCCR[0] |= ((HostTimeouts->HighSpeedTransmissionTimeout)<<16);

  DSIx->TCCR[0] &= ~DSI_TCCR0_LPRX_TOCNT;
  DSIx->TCCR[0] |= HostTimeouts->LowPowerReceptionTimeout;

  DSIx->TCCR[1] &= ~DSI_TCCR1_HSRD_TOCNT;
  DSIx->TCCR[1] |= HostTimeouts->HighSpeedReadTimeout;

  DSIx->TCCR[2] &= ~DSI_TCCR2_LPRD_TOCNT;
  DSIx->TCCR[2] |= HostTimeouts->LowPowerReadTimeout;

  DSIx->TCCR[3] &= ~DSI_TCCR3_HSWR_TOCNT;
  DSIx->TCCR[3] |= HostTimeouts->HighSpeedWriteTimeout;

  DSIx->TCCR[3] &= ~DSI_TCCR3_PM;
  DSIx->TCCR[3] |= HostTimeouts->HighSpeedWritePrespMode;

  DSIx->TCCR[4] &= ~DSI_TCCR4_LPWR_TOCNT;
  DSIx->TCCR[4] |= HostTimeouts->LowPowerWriteTimeout;

  DSIx->TCCR[5] &= ~DSI_TCCR5_BTA_TOCNT;
  DSIx->TCCR[5] |= HostTimeouts->BTATimeout;
}

void DSI_Start(DSI_TypeDef *DSIx)
{

  DSIx->CR |= DSI_CR_EN;

  DSIx->WCR |= DSI_WCR_DSIEN;
}

void DSI_Stop(DSI_TypeDef *DSIx)
{

  DSIx->CR &= ~DSI_CR_EN;

  DSIx->WCR &= ~DSI_WCR_DSIEN;
}

void DSI_Refresh(DSI_TypeDef *DSIx)
{

  DSIx->WCR |= DSI_WCR_LTDCEN;
}

void DSI_ColorMode(DSI_TypeDef *DSIx, uint32_t ColorMode)
{

  assert_param(IS_DSI_COLOR_MODE(ColorMode));

  DSIx->WCR &= ~DSI_WCR_COLM;
  DSIx->WCR |= ColorMode;
}

void DSI_Shutdown(DSI_TypeDef *DSIx, uint32_t Shutdown)
{

  assert_param(IS_DSI_SHUT_DOWN(Shutdown));

  DSIx->WCR &= ~DSI_WCR_SHTDN;
  DSIx->WCR |= Shutdown;
}

void DSI_ShortWrite(DSI_TypeDef *DSIx,
                                 uint32_t ChannelID,
                                 uint32_t Mode,
                                 uint32_t Param1,
                                 uint32_t Param2)
{

  assert_param(IS_DSI_SHORT_WRITE_PACKET_TYPE(Mode));

  while((DSIx->GPSR & DSI_GPSR_CMDFE) == 0)
  {}

  DSI_ConfigPacketHeader(DSIx,
                         ChannelID,
                         Mode,
                         Param1,
                         Param2);
}

void DSI_LongWrite(DSI_TypeDef *DSIx,
                                uint32_t ChannelID,
                                uint32_t Mode,
                                uint32_t NbParams,
                                uint32_t Param1,
                                uint8_t* ParametersTable)
{
  uint32_t uicounter = 0;

  assert_param(IS_DSI_LONG_WRITE_PACKET_TYPE(Mode));

  while((DSIx->GPSR & DSI_GPSR_CMDFE) == 0)
  {}

  while(uicounter < NbParams)
  {
    if(uicounter == 0x00)
    {
      DSIx->GPDR=(Param1 | \
                            ((uint32_t)(*(ParametersTable+uicounter))<<8) | \
                            ((uint32_t)(*(ParametersTable+uicounter+1))<<16) | \
                            ((uint32_t)(*(ParametersTable+uicounter+2))<<24));
      uicounter += 3;
    }
    else
    {
      DSIx->GPDR=((*(ParametersTable+uicounter)) | \
                            ((uint32_t)(*(ParametersTable+uicounter+1))<<8) | \
                            ((uint32_t)(*(ParametersTable+uicounter+2))<<16) | \
                            ((uint32_t)(*(ParametersTable+uicounter+3))<<24));
      uicounter+=4;
    }
  }

  DSI_ConfigPacketHeader(DSIx,
                         ChannelID,
                         Mode,
                         ((NbParams+1)&0x00FF),
                         (((NbParams+1)&0xFF00)>>8));
}

void DSI_Read(DSI_TypeDef *DSIx,
                               uint32_t ChannelNbr,
                               uint8_t* Array,
                               uint32_t Size,
                               uint32_t Mode,
                               uint32_t DCSCmd,
                               uint8_t* ParametersTable)
{

  assert_param(IS_DSI_READ_PACKET_TYPE(Mode));

  if(Size > 2)
  {

    DSI_ShortWrite(DSIx, ChannelNbr, DSI_MAX_RETURN_PKT_SIZE, ((Size)&0xFF), (((Size)>>8)&0xFF));
  }

  if (Mode == DSI_DCS_SHORT_PKT_READ)
  {
    DSI_ConfigPacketHeader(DSIx, ChannelNbr, Mode, DCSCmd, 0);
  }
  else if (Mode == DSI_GEN_SHORT_PKT_READ_P0)
  {
    DSI_ConfigPacketHeader(DSIx, ChannelNbr, Mode, 0, 0);
  }
  else if (Mode == DSI_GEN_SHORT_PKT_READ_P1)
  {
    DSI_ConfigPacketHeader(DSIx, ChannelNbr, Mode, ParametersTable[0], 0);
  }
  else
  {
    DSI_ConfigPacketHeader(DSIx, ChannelNbr, Mode, ParametersTable[0], ParametersTable[1]);
  }

  while((DSIx->GPSR & DSI_GPSR_PRDFE) == DSI_GPSR_PRDFE)
  {}

  *((uint32_t *)Array) = (DSIx->GPDR);
  if (Size > 4)
  {
    Size -= 4;
    Array += 4;
  }

  while(((int)(Size)) > 0)
  {
    if((DSIx->GPSR & DSI_GPSR_PRDFE) == 0)
    {
      *((uint32_t *)Array) = (DSIx->GPDR);
      Size -= 4;
      Array += 4;
    }
  }
}

static void DSI_ConfigPacketHeader(DSI_TypeDef *DSIx,
                                   uint32_t ChannelID,
                                   uint32_t DataType,
                                   uint32_t Data0,
                                   uint32_t Data1)
{

  DSIx->GHCR = (DataType | (ChannelID<<6) | (Data0<<8) | (Data1<<16));
}

void DSI_EnterULPMData(DSI_TypeDef *DSIx)
{

  DSIx->PUCR |= DSI_PUCR_URDL;

  if((DSIx->PCONFR & DSI_PCONFR_NL) == DSI_ONE_DATA_LANE)
  {
    while((DSIx->PSR & DSI_PSR_UAN0) != 0)
    {}
  }
  else
  {
    while((DSIx->PSR & (DSI_PSR_UAN0 | DSI_PSR_UAN1)) != 0)
    {}
  }
}

void DSI_ExitULPMData(DSI_TypeDef *DSIx)
{

  DSIx->PUCR |= DSI_PUCR_UEDL;

  if((DSIx->PCONFR & DSI_PCONFR_NL) == DSI_ONE_DATA_LANE)
  {
    while((DSIx->PSR & DSI_PSR_UAN0) != DSI_PSR_UAN0)
    {}
  }
  else
  {
    while((DSIx->PSR & (DSI_PSR_UAN0 | DSI_PSR_UAN1)) != (DSI_PSR_UAN0 | DSI_PSR_UAN1))
    {}
  }

  DSIx->PUCR = 0;
}

void DSI_EnterULPM(DSI_TypeDef *DSIx)
{

  DSIx->CLCR &= ~DSI_CLCR_DPCC;

  RCC_DSIClockSourceConfig(RCC_DSICLKSource_PLLR);

  DSIx->PUCR |= (DSI_PUCR_URCL | DSI_PUCR_URDL);

  if((DSIx->PCONFR & DSI_PCONFR_NL) == DSI_ONE_DATA_LANE)
  {
    while((DSIx->PSR & (DSI_PSR_UAN0 | DSI_PSR_UANC)) != 0)
    {}
  }
  else
  {
    while((DSIx->PSR & (DSI_PSR_UAN0 | DSI_PSR_UAN1 | DSI_PSR_UANC)) != 0)
    {}
  }

  DSIx->WRPCR &= ~DSI_WRPCR_PLLEN;
}

void DSI_ExitULPM(DSI_TypeDef *DSIx)
{

  DSIx->WRPCR |= DSI_WRPCR_PLLEN;

  while(DSI_GetFlagStatus(DSIx, DSI_FLAG_PLLLS) == RESET)
  {}

  DSIx->PUCR |= (DSI_PUCR_UECL | DSI_PUCR_UEDL);

  if((DSIx->PCONFR & DSI_PCONFR_NL) == DSI_ONE_DATA_LANE)
  {
    while((DSIx->PSR & (DSI_PSR_UAN0 | DSI_PSR_UANC)) != (DSI_PSR_UAN0 | DSI_PSR_UANC))
    {}
  }
  else
  {
    while((DSIx->PSR & (DSI_PSR_UAN0 | DSI_PSR_UAN1 | DSI_PSR_UANC)) != (DSI_PSR_UAN0 | DSI_PSR_UAN1 | DSI_PSR_UANC))
    {}
  }

  DSIx->PUCR = 0;

  RCC_DSIClockSourceConfig(RCC_DSICLKSource_PHY);

  DSIx->CLCR |= DSI_CLCR_DPCC;
}

void DSI_PatternGeneratorStart(DSI_TypeDef *DSIx, uint32_t Mode, uint32_t Orientation)
{

  DSIx->VMCR &= ~(DSI_VMCR_PGM | DSI_VMCR_PGO);
  DSIx->VMCR |= ((Mode<<20) | (Orientation<<24));

  DSIx->VMCR |= DSI_VMCR_PGE;

}

void DSI_PatternGeneratorStop(DSI_TypeDef *DSIx)
{

  DSIx->VMCR &= ~DSI_VMCR_PGE;
}

void DSI_SetSlewRateAndDelayTuning(DSI_TypeDef *DSIx, uint32_t CommDelay, uint32_t Lane, uint32_t Value)
{

  assert_param(IS_DSI_COMMUNICATION_DELAY(CommDelay));
  assert_param(IS_DSI_LANE_GROUP(Lane));

  switch(CommDelay)
  {
  case DSI_SLEW_RATE_HSTX:
    if(Lane == DSI_CLOCK_LANE)
    {

      DSIx->WPCR[1] &= ~DSI_WPCR1_HSTXSRCCL;
      DSIx->WPCR[1] |= Value<<16;
    }
    else
    {

      DSIx->WPCR[1] &= ~DSI_WPCR1_HSTXSRCDL;
      DSIx->WPCR[1] |= Value<<18;
    }
    break;
  case DSI_SLEW_RATE_LPTX:
    if(Lane == DSI_CLOCK_LANE)
    {

      DSIx->WPCR[1] &= ~DSI_WPCR1_LPSRCCL;
      DSIx->WPCR[1] |= Value<<6;
    }
    else
    {

      DSIx->WPCR[1] &= ~DSI_WPCR1_LPSRCDL;
      DSIx->WPCR[1] |= Value<<8;
    }
    break;
  case DSI_HS_DELAY:
    if(Lane == DSI_CLOCK_LANE)
    {

      DSIx->WPCR[1] &= ~DSI_WPCR1_HSTXDCL;
      DSIx->WPCR[1] |= Value;
    }
    else
    {

      DSIx->WPCR[1] &= ~DSI_WPCR1_HSTXDDL;
      DSIx->WPCR[1] |= Value<<2;
    }
    break;
  default:
    break;
  }
}

void DSI_SetLowPowerRXFilter(DSI_TypeDef *DSIx, uint32_t Frequency)
{

  DSIx->WPCR[1] &= ~DSI_WPCR1_LPRXFT;
  DSIx->WPCR[1] |= Frequency<<25;
}

void DSI_SetSDD(DSI_TypeDef *DSIx, FunctionalState State)
{

  assert_param(IS_FUNCTIONAL_STATE(State));

  DSIx->WPCR[1] &= ~DSI_WPCR1_SDDC;
  DSIx->WPCR[1] |= ((uint32_t)State<<12);
}

void DSI_SetLanePinsConfiguration(DSI_TypeDef *DSIx, uint32_t CustomLane, uint32_t Lane, FunctionalState State)
{

  assert_param(IS_DSI_CUSTOM_LANE(CustomLane));
  assert_param(IS_DSI_LANE(Lane));
  assert_param(IS_FUNCTIONAL_STATE(State));

  switch(CustomLane)
  {
  case DSI_SWAP_LANE_PINS:
    if(Lane == DSI_CLOCK_LANE)
    {

      DSIx->WPCR[0] &= ~DSI_WPCR0_SWCL;
      DSIx->WPCR[0] |= ((uint32_t)State<<6);
    }
    else if(Lane == DSI_DATA_LANE0)
    {

      DSIx->WPCR[0] &= ~DSI_WPCR0_SWDL0;
      DSIx->WPCR[0] |= ((uint32_t)State<<7);
    }
    else
    {

      DSIx->WPCR[0] &= ~DSI_WPCR0_SWDL1;
      DSIx->WPCR[0] |= ((uint32_t)State<<8);
    }
    break;
  case DSI_INVERT_HS_SIGNAL:
    if(Lane == DSI_CLOCK_LANE)
    {

      DSIx->WPCR[0] &= ~DSI_WPCR0_HSICL;
      DSIx->WPCR[0] |= ((uint32_t)State<<9);
    }
    else if(Lane == DSI_DATA_LANE0)
    {

      DSIx->WPCR[0] &= ~DSI_WPCR0_HSIDL0;
      DSIx->WPCR[0] |= ((uint32_t)State<<10);
    }
    else
    {

      DSIx->WPCR[0] &= ~DSI_WPCR0_HSIDL1;
      DSIx->WPCR[0] |= ((uint32_t)State<<11);
    }
    break;
  default:
    break;
  }
}

void DSI_SetPHYTimings(DSI_TypeDef *DSIx, uint32_t Timing, FunctionalState State, uint32_t Value)
{

  assert_param(IS_DSI_PHY_TIMING(Timing));
  assert_param(IS_FUNCTIONAL_STATE(State));

  switch(Timing)
  {
  case DSI_TCLK_POST:

    DSIx->WPCR[0] &= ~DSI_WPCR0_TCLKPOSTEN;
    DSIx->WPCR[0] |= ((uint32_t)State<<27);

    if(State)
    {

      DSIx->WPCR[4] &= ~DSI_WPCR4_TCLKPOST;
      DSIx->WPCR[4] |= Value;
    }

    break;
  case DSI_TLPX_CLK:

    DSIx->WPCR[0] &= ~DSI_WPCR0_TLPXCEN;
    DSIx->WPCR[0] |= ((uint32_t)State<<26);

    if(State)
    {

      DSIx->WPCR[3] &= ~DSI_WPCR3_TLPXC;
      DSIx->WPCR[3] |= Value;
    }

    break;
  case DSI_THS_EXIT:

    DSIx->WPCR[0] &= ~DSI_WPCR0_THSEXITEN;
    DSIx->WPCR[0] |= ((uint32_t)State<<25);

    if(State)
    {

      DSIx->WPCR[3] &= ~DSI_WPCR3_THSEXIT;
      DSIx->WPCR[3] |= Value;
    }

    break;
  case DSI_TLPX_DATA:

    DSIx->WPCR[0] &= ~DSI_WPCR0_TLPXDEN;
    DSIx->WPCR[0] |= ((uint32_t)State<<24);

    if(State)
    {

      DSIx->WPCR[3] &= ~DSI_WPCR3_TLPXD;
      DSIx->WPCR[3] |= Value;
    }

    break;
  case DSI_THS_ZERO:

    DSIx->WPCR[0] &= ~DSI_WPCR0_THSZEROEN;
    DSIx->WPCR[0] |= ((uint32_t)State<<23);

    if(State)
    {

      DSIx->WPCR[3] &= ~DSI_WPCR3_THSZERO;
      DSIx->WPCR[3] |= Value;
    }

    break;
  case DSI_THS_TRAIL:

    DSIx->WPCR[0] &= ~DSI_WPCR0_THSTRAILEN;
    DSIx->WPCR[0] |= ((uint32_t)State<<22);

    if(State)
    {

      DSIx->WPCR[2] &= ~DSI_WPCR2_THSTRAIL;
      DSIx->WPCR[2] |= Value;
    }

    break;
  case DSI_THS_PREPARE:

    DSIx->WPCR[0] &= ~DSI_WPCR0_THSPREPEN;
    DSIx->WPCR[0] |= ((uint32_t)State<<21);

    if(State)
    {

      DSIx->WPCR[2] &= ~DSI_WPCR2_THSPREP;
      DSIx->WPCR[2] |= Value;
    }

    break;
  case DSI_TCLK_ZERO:

    DSIx->WPCR[0] &= ~DSI_WPCR0_TCLKZEROEN;
    DSIx->WPCR[0] |= ((uint32_t)State<<20);

    if(State)
    {

      DSIx->WPCR[2] &= ~DSI_WPCR2_TCLKZERO;
      DSIx->WPCR[2] |= Value;
    }

    break;
  case DSI_TCLK_PREPARE:

    DSIx->WPCR[0] &= ~DSI_WPCR0_TCLKPREPEN;
    DSIx->WPCR[0] |= ((uint32_t)State<<19);

    if(State)
    {

      DSIx->WPCR[2] &= ~DSI_WPCR2_TCLKPREP;
      DSIx->WPCR[2] |= Value;
    }

    break;
  default:
    break;
  }
}

void DSI_ForceTXStopMode(DSI_TypeDef *DSIx, uint32_t Lane, FunctionalState State)
{

  assert_param(IS_DSI_LANE_GROUP(Lane));
  assert_param(IS_FUNCTIONAL_STATE(State));

  if(Lane == DSI_CLOCK_LANE)
  {

    DSIx->WPCR[0] &= ~DSI_WPCR0_FTXSMCL;
    DSIx->WPCR[0] |= ((uint32_t)State<<12);
  }
  else
  {

    DSIx->WPCR[0] &= ~DSI_WPCR0_FTXSMDL;
    DSIx->WPCR[0] |= ((uint32_t)State<<13);
  }
}

void DSI_ForceRXLowPower(DSI_TypeDef *DSIx, FunctionalState State)
{

  assert_param(IS_FUNCTIONAL_STATE(State));

  DSIx->WPCR[1] &= ~DSI_WPCR1_FLPRXLPM;
  DSIx->WPCR[1] |= ((uint32_t)State<<22);
}

void DSI_ForceDataLanesInRX(DSI_TypeDef *DSIx, FunctionalState State)
{

  assert_param(IS_FUNCTIONAL_STATE(State));

  DSIx->WPCR[0] &= ~DSI_WPCR0_TDDL;
  DSIx->WPCR[0] |= ((uint32_t)State<<16);
}

void DSI_SetPullDown(DSI_TypeDef *DSIx, FunctionalState State)
{

  assert_param(IS_FUNCTIONAL_STATE(State));

  DSIx->WPCR[0] &= ~DSI_WPCR0_PDEN;
  DSIx->WPCR[0] |= ((uint32_t)State<<18);
}

void DSI_SetContentionDetectionOff(DSI_TypeDef *DSIx, FunctionalState State)
{

  assert_param(IS_FUNCTIONAL_STATE(State));

  DSIx->WPCR[0] &= ~DSI_WPCR0_CDOFFDL;
  DSIx->WPCR[0] |= ((uint32_t)State<<14);
}

void DSI_ITConfig(DSI_TypeDef* DSIx, uint32_t DSI_IT, FunctionalState NewState)
{

  assert_param(IS_DSI_ALL_PERIPH(DSIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_DSI_IT(DSI_IT));

  if(NewState != DISABLE)
  {

    DSIx->WIER |= DSI_IT;
  }
  else
  {

    DSIx->WIER &= ~DSI_IT;
  }
}

FlagStatus DSI_GetFlagStatus(DSI_TypeDef* DSIx, uint16_t DSI_FLAG)
{
  FlagStatus bitstatus = RESET;

  assert_param(IS_DSI_ALL_PERIPH(DSIx));
  assert_param(IS_DSI_GET_FLAG(DSI_FLAG));

  if((DSIx->WISR & DSI_FLAG) != (uint32_t)RESET)
  {

    bitstatus = SET;
  }
  else
  {

    bitstatus = RESET;
  }

  return  bitstatus;
}

void DSI_ClearFlag(DSI_TypeDef* DSIx, uint16_t DSI_FLAG)
{

  assert_param(IS_DSI_ALL_PERIPH(DSIx));
  assert_param(IS_DSI_CLEAR_FLAG(DSI_FLAG));

  DSIx->WIFCR = (uint32_t)DSI_FLAG;
}

ITStatus DSI_GetITStatus(DSI_TypeDef* DSIx, uint32_t DSI_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t enablestatus = 0;

  assert_param(IS_DSI_ALL_PERIPH(DSIx));
  assert_param(IS_DSI_IT(DSI_IT));

  enablestatus = (DSIx->WIER & DSI_IT);

  if (((DSIx->WISR & DSI_IT) != (uint32_t)RESET) && enablestatus)
  {

    bitstatus = SET;
  }
  else
  {

    bitstatus = RESET;
  }

  return bitstatus;
}

void DSI_ClearITPendingBit(DSI_TypeDef* DSIx, uint32_t DSI_IT)
{

  assert_param(IS_DSI_ALL_PERIPH(DSIx));
  assert_param(IS_DSI_IT(DSI_IT));

  DSIx->WIFCR = (uint32_t)DSI_IT;
}

void DSI_ConfigErrorMonitor(DSI_TypeDef *DSIx, uint32_t ActiveErrors)
{
  DSIx->IER[0] = 0;
  DSIx->IER[1] = 0;

  if((ActiveErrors & DSI_ERROR_ACK) != RESET)
  {

    DSIx->IER[0] |= DSI_ERROR_ACK_MASK;
  }

  if((ActiveErrors & DSI_ERROR_PHY) != RESET)
  {

    DSIx->IER[0] |= DSI_ERROR_PHY_MASK;
  }

  if((ActiveErrors & DSI_ERROR_TX) != RESET)
  {

    DSIx->IER[1] |= DSI_ERROR_TX_MASK;
  }

  if((ActiveErrors & DSI_ERROR_RX) != RESET)
  {

    DSIx->IER[1] |= DSI_ERROR_RX_MASK;
  }

  if((ActiveErrors & DSI_ERROR_ECC) != RESET)
  {

    DSIx->IER[1] |= DSI_ERROR_ECC_MASK;
  }

  if((ActiveErrors & DSI_ERROR_CRC) != RESET)
  {

    DSIx->IER[1] |= DSI_ERROR_CRC_MASK;
  }

  if((ActiveErrors & DSI_ERROR_PSE) != RESET)
  {

    DSIx->IER[1] |= DSI_ERROR_PSE_MASK;
  }

  if((ActiveErrors & DSI_ERROR_EOT) != RESET)
  {

    DSIx->IER[1] |= DSI_ERROR_EOT_MASK;
  }

  if((ActiveErrors & DSI_ERROR_OVF) != RESET)
  {

    DSIx->IER[1] |= DSI_ERROR_OVF_MASK;
  }

  if((ActiveErrors & DSI_ERROR_GEN) != RESET)
  {

    DSIx->IER[1] |= DSI_ERROR_GEN_MASK;
  }
}

#endif
