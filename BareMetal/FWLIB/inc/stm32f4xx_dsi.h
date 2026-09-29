#ifndef __STM32F4xx_DSI_H
#define __STM32F4xx_DSI_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

#if defined(STM32F469_479xx)

typedef struct
{
  uint32_t AutomaticClockLaneControl;

  uint32_t TXEscapeCkdiv;

  uint32_t NumberOfLanes;

}DSI_InitTypeDef;

typedef struct
{
  uint32_t PLLNDIV;

  uint32_t PLLIDF;

  uint32_t PLLODF;

}DSI_PLLInitTypeDef;

typedef struct
{
  uint32_t VirtualChannelID;

  uint32_t ColorCoding;

  uint32_t LooselyPacked;

  uint32_t Mode;

  uint32_t PacketSize;

  uint32_t NumberOfChunks;

  uint32_t NullPacketSize;

  uint32_t HSPolarity;

  uint32_t VSPolarity;

  uint32_t DEPolarity;

  uint32_t HorizontalSyncActive;

  uint32_t HorizontalBackPorch;

  uint32_t HorizontalLine;

  uint32_t VerticalSyncActive;

  uint32_t VerticalBackPorch;

  uint32_t VerticalFrontPorch;

  uint32_t VerticalActive;

  uint32_t LPCommandEnable;

  uint32_t LPLargestPacketSize;

  uint32_t LPVACTLargestPacketSize;

  uint32_t LPHorizontalFrontPorchEnable;

  uint32_t LPHorizontalBackPorchEnable;

  uint32_t LPVerticalActiveEnable;

  uint32_t LPVerticalFrontPorchEnable;

  uint32_t LPVerticalBackPorchEnable;

  uint32_t LPVerticalSyncActiveEnable;

  uint32_t FrameBTAAcknowledgeEnable;

}DSI_VidCfgTypeDef;

typedef struct
{
  uint32_t VirtualChannelID;

  uint32_t ColorCoding;

  uint32_t CommandSize;

  uint32_t TearingEffectSource;

  uint32_t TearingEffectPolarity;

  uint32_t HSPolarity;

  uint32_t VSPolarity;

  uint32_t DEPolarity;

  uint32_t VSyncPol;

  uint32_t AutomaticRefresh;

  uint32_t TEAcknowledgeRequest;

}DSI_CmdCfgTypeDef;

typedef struct
{
  uint32_t LPGenShortWriteNoP;

  uint32_t LPGenShortWriteOneP;

  uint32_t LPGenShortWriteTwoP;

  uint32_t LPGenShortReadNoP;

  uint32_t LPGenShortReadOneP;

  uint32_t LPGenShortReadTwoP;

  uint32_t LPGenLongWrite;

  uint32_t LPDcsShortWriteNoP;

  uint32_t LPDcsShortWriteOneP;

  uint32_t LPDcsShortReadNoP;

  uint32_t LPDcsLongWrite;

  uint32_t LPMaxReadPacket;

  uint32_t AcknowledgeRequest;

}DSI_LPCmdTypeDef;

typedef struct
{
  uint32_t ClockLaneHS2LPTime;

  uint32_t ClockLaneLP2HSTime;

  uint32_t DataLaneHS2LPTime;

  uint32_t DataLaneLP2HSTime;

  uint32_t DataLaneMaxReadTime;

  uint32_t StopWaitTime;

}DSI_PHY_TimerTypeDef;

typedef struct
{
  uint32_t TimeoutCkdiv;

  uint32_t HighSpeedTransmissionTimeout;

  uint32_t LowPowerReceptionTimeout;

  uint32_t HighSpeedReadTimeout;

  uint32_t LowPowerReadTimeout;

  uint32_t HighSpeedWriteTimeout;

  uint32_t HighSpeedWritePrespMode;

  uint32_t LowPowerWriteTimeout;

  uint32_t BTATimeout;

}DSI_HOST_TimeoutTypeDef;

#define DSI_ENTER_IDLE_MODE       0x39
#define DSI_ENTER_INVERT_MODE     0x21
#define DSI_ENTER_NORMAL_MODE     0x13
#define DSI_ENTER_PARTIAL_MODE    0x12
#define DSI_ENTER_SLEEP_MODE      0x10
#define DSI_EXIT_IDLE_MODE        0x38
#define DSI_EXIT_INVERT_MODE      0x20
#define DSI_EXIT_SLEEP_MODE       0x11
#define DSI_GET_3D_CONTROL        0x3F
#define DSI_GET_ADDRESS_MODE      0x0B
#define DSI_GET_BLUE_CHANNEL      0x08
#define DSI_GET_DIAGNOSTIC_RESULT 0x0F
#define DSI_GET_DISPLAY_MODE      0x0D
#define DSI_GET_GREEN_CHANNEL     0x07
#define DSI_GET_PIXEL_FORMAT      0x0C
#define DSI_GET_POWER_MODE        0x0A
#define DSI_GET_RED_CHANNEL       0x06
#define DSI_GET_SCANLINE          0x45
#define DSI_GET_SIGNAL_MODE       0x0E
#define DSI_NOP                   0x00
#define DSI_READ_DDB_CONTINUE     0xA8
#define DSI_READ_DDB_START        0xA1
#define DSI_READ_MEMORY_CONTINUE  0x3E
#define DSI_READ_MEMORY_START     0x2E
#define DSI_SET_3D_CONTROL        0x3D
#define DSI_SET_ADDRESS_MODE      0x36
#define DSI_SET_COLUMN_ADDRESS    0x2A
#define DSI_SET_DISPLAY_OFF       0x28
#define DSI_SET_DISPLAY_ON        0x29
#define DSI_SET_GAMMA_CURVE       0x26
#define DSI_SET_PAGE_ADDRESS      0x2B
#define DSI_SET_PARTIAL_COLUMNS   0x31
#define DSI_SET_PARTIAL_ROWS      0x30
#define DSI_SET_PIXEL_FORMAT      0x3A
#define DSI_SET_SCROLL_AREA       0x33
#define DSI_SET_SCROLL_START      0x37
#define DSI_SET_TEAR_OFF          0x34
#define DSI_SET_TEAR_ON           0x35
#define DSI_SET_TEAR_SCANLINE     0x44
#define DSI_SET_VSYNC_TIMING      0x40
#define DSI_SOFT_RESET            0x01
#define DSI_WRITE_LUT             0x2D
#define DSI_WRITE_MEMORY_CONTINUE 0x3C
#define DSI_WRITE_MEMORY_START    0x2C

#define DSI_VID_MODE_NB_PULSES 0
#define DSI_VID_MODE_NB_EVENTS 1
#define DSI_VID_MODE_BURST     2
#define IS_DSI_VIDEO_MODE_TYPE(VideoModeType)       (((VideoModeType) == DSI_VID_MODE_NB_PULSES) || \
                                                     ((VideoModeType) == DSI_VID_MODE_NB_EVENTS) || \
                                                     ((VideoModeType) == DSI_VID_MODE_BURST))

#define DSI_COLOR_MODE_FULL  0
#define DSI_COLOR_MODE_EIGHT DSI_WCR_COLM
#define IS_DSI_COLOR_MODE(ColorMode)                (((ColorMode) == DSI_COLOR_MODE_FULL) || ((ColorMode) == DSI_COLOR_MODE_EIGHT))

#define DSI_DISPLAY_ON  0
#define DSI_DISPLAY_OFF DSI_WCR_SHTDN
#define IS_DSI_SHUT_DOWN(ShutDown)                  (((ShutDown) == DSI_DISPLAY_ON) || ((ShutDown) == DSI_DISPLAY_OFF))

#define DSI_LP_COMMAND_DISABLE 0
#define DSI_LP_COMMAND_ENABLE  DSI_VMCR_LPCE
#define IS_DSI_LP_COMMAND(LPCommand)                (((LPCommand) == DSI_LP_COMMAND_DISABLE) || ((LPCommand) == DSI_LP_COMMAND_ENABLE))

#define DSI_LP_HFP_DISABLE 0
#define DSI_LP_HFP_ENABLE  DSI_VMCR_LPHFPE
#define IS_DSI_LP_HFP(LPHFP)                        (((LPHFP) == DSI_LP_HFP_DISABLE) || ((LPHFP) == DSI_LP_HFP_ENABLE))

#define DSI_LP_HBP_DISABLE 0
#define DSI_LP_HBP_ENABLE  DSI_VMCR_LPHBPE
#define IS_DSI_LP_HBP(LPHBP)                        (((LPHBP) == DSI_LP_HBP_DISABLE) || ((LPHBP) == DSI_LP_HBP_ENABLE))

#define DSI_LP_VACT_DISABLE 0
#define DSI_LP_VACT_ENABLE  DSI_VMCR_LPVAE
#define IS_DSI_LP_VACTIVE(LPVActive)                (((LPVActive) == DSI_LP_VACT_DISABLE) || ((LPVActive) == DSI_LP_VACT_ENABLE))

#define DSI_LP_VFP_DISABLE 0
#define DSI_LP_VFP_ENABLE  DSI_VMCR_LPVFPE
#define IS_DSI_LP_VFP(LPVFP)                        (((LPVFP) == DSI_LP_VFP_DISABLE) || ((LPVFP) == DSI_LP_VFP_ENABLE))

#define DSI_LP_VBP_DISABLE 0
#define DSI_LP_VBP_ENABLE  DSI_VMCR_LPVBPE
#define IS_DSI_LP_VBP(LPVBP)                        (((LPVBP) == DSI_LP_VBP_DISABLE) || ((LPVBP) == DSI_LP_VBP_ENABLE))

#define DSI_LP_VSYNC_DISABLE 0
#define DSI_LP_VSYNC_ENABLE  DSI_VMCR_LPVSAE
#define IS_DSI_LP_VSYNC(LPVSYNC)                    (((LPVSYNC) == DSI_LP_VSYNC_DISABLE) || ((LPVSYNC) == DSI_LP_VSYNC_ENABLE))

#define DSI_FBTAA_DISABLE 0
#define DSI_FBTAA_ENABLE  DSI_VMCR_FBTAAE
#define IS_DSI_FBTAA(FrameBTAAcknowledge)           (((FrameBTAAcknowledge) == DSI_FBTAA_DISABLE) || ((FrameBTAAcknowledge) == DSI_FBTAA_ENABLE))

#define DSI_TE_DSILINK  0
#define DSI_TE_EXTERNAL DSI_WCFGR_TESRC
#define IS_DSI_TE_SOURCE(TESource)                  (((TESource) == DSI_TE_DSILINK) || ((TESource) == DSI_TE_EXTERNAL))

#define DSI_TE_RISING_EDGE  0
#define DSI_TE_FALLING_EDGE DSI_WCFGR_TEPOL
#define IS_DSI_TE_POLARITY(TEPolarity)              (((TEPolarity) == DSI_TE_RISING_EDGE) || ((TEPolarity) == DSI_TE_FALLING_EDGE))

#define DSI_VSYNC_FALLING 0
#define DSI_VSYNC_RISING  DSI_WCFGR_VSPOL
#define IS_DSI_VS_POLARITY(VSPolarity)              (((VSPolarity) == DSI_VSYNC_FALLING) || ((VSPolarity) == DSI_VSYNC_RISING))

#define DSI_AR_DISABLE 0
#define DSI_AR_ENABLE  DSI_WCFGR_AR
#define IS_DSI_AUTOMATIC_REFRESH(AutomaticRefresh)  (((AutomaticRefresh) == DSI_AR_DISABLE) || ((AutomaticRefresh) == DSI_AR_ENABLE))

#define DSI_TE_ACKNOWLEDGE_DISABLE 0
#define DSI_TE_ACKNOWLEDGE_ENABLE DSI_CMCR_TEARE
#define IS_DSI_TE_ACK_REQUEST(TEAcknowledgeRequest) (((TEAcknowledgeRequest) == DSI_TE_ACKNOWLEDGE_DISABLE) || ((TEAcknowledgeRequest) == DSI_TE_ACKNOWLEDGE_ENABLE))

#define DSI_ACKNOWLEDGE_DISABLE 0
#define DSI_ACKNOWLEDGE_ENABLE DSI_CMCR_ARE
#define IS_DSI_ACK_REQUEST(AcknowledgeRequest)      (((AcknowledgeRequest) == DSI_ACKNOWLEDGE_DISABLE) || ((AcknowledgeRequest) == DSI_ACKNOWLEDGE_ENABLE))

#define DSI_LP_GSW0P_DISABLE 0
#define DSI_LP_GSW0P_ENABLE DSI_CMCR_GSW0TX
#define IS_DSI_LP_GSW0P(LP_GSW0P)                   (((LP_GSW0P) == DSI_LP_GSW0P_DISABLE) || ((LP_GSW0P) == DSI_LP_GSW0P_ENABLE))

#define DSI_LP_GSW1P_DISABLE 0
#define DSI_LP_GSW1P_ENABLE DSI_CMCR_GSW1TX
#define IS_DSI_LP_GSW1P(LP_GSW1P)                   (((LP_GSW1P) == DSI_LP_GSW1P_DISABLE) || ((LP_GSW1P) == DSI_LP_GSW1P_ENABLE))

#define DSI_LP_GSW2P_DISABLE 0
#define DSI_LP_GSW2P_ENABLE DSI_CMCR_GSW2TX
#define IS_DSI_LP_GSW2P(LP_GSW2P)                   (((LP_GSW2P) == DSI_LP_GSW2P_DISABLE) || ((LP_GSW2P) == DSI_LP_GSW2P_ENABLE))

#define DSI_LP_GSR0P_DISABLE 0
#define DSI_LP_GSR0P_ENABLE DSI_CMCR_GSR0TX
#define IS_DSI_LP_GSR0P(LP_GSR0P)                   (((LP_GSR0P) == DSI_LP_GSR0P_DISABLE) || ((LP_GSR0P) == DSI_LP_GSR0P_ENABLE))

#define DSI_LP_GSR1P_DISABLE 0
#define DSI_LP_GSR1P_ENABLE DSI_CMCR_GSR1TX
#define IS_DSI_LP_GSR1P(LP_GSR1P)                   (((LP_GSR1P) == DSI_LP_GSR1P_DISABLE) || ((LP_GSR1P) == DSI_LP_GSR1P_ENABLE))

#define DSI_LP_GSR2P_DISABLE 0
#define DSI_LP_GSR2P_ENABLE DSI_CMCR_GSR2TX
#define IS_DSI_LP_GSR2P(LP_GSR2P)                   (((LP_GSR2P) == DSI_LP_GSR2P_DISABLE) || ((LP_GSR2P) == DSI_LP_GSR2P_ENABLE))

#define DSI_LP_GLW_DISABLE 0
#define DSI_LP_GLW_ENABLE DSI_CMCR_GLWTX
#define IS_DSI_LP_GLW(LP_GLW)                       (((LP_GLW) == DSI_LP_GLW_DISABLE) || ((LP_GLW) == DSI_LP_GLW_ENABLE))

#define DSI_LP_DSW0P_DISABLE 0
#define DSI_LP_DSW0P_ENABLE DSI_CMCR_DSW0TX
#define IS_DSI_LP_DSW0P(LP_DSW0P)                   (((LP_DSW0P) == DSI_LP_DSW0P_DISABLE) || ((LP_DSW0P) == DSI_LP_DSW0P_ENABLE))

#define DSI_LP_DSW1P_DISABLE 0
#define DSI_LP_DSW1P_ENABLE DSI_CMCR_DSW1TX
#define IS_DSI_LP_DSW1P(LP_DSW1P)                   (((LP_DSW1P) == DSI_LP_DSW1P_DISABLE) || ((LP_DSW1P) == DSI_LP_DSW1P_ENABLE))

#define DSI_LP_DSR0P_DISABLE 0
#define DSI_LP_DSR0P_ENABLE DSI_CMCR_DSR0TX
#define IS_DSI_LP_DSR0P(LP_DSR0P)                   (((LP_DSR0P) == DSI_LP_DSR0P_DISABLE) || ((LP_DSR0P) == DSI_LP_DSR0P_ENABLE))

#define DSI_LP_DLW_DISABLE 0
#define DSI_LP_DLW_ENABLE DSI_CMCR_DLWTX
#define IS_DSI_LP_DLW(LP_DLW)                       (((LP_DLW) == DSI_LP_DLW_DISABLE) || ((LP_DLW) == DSI_LP_DLW_ENABLE))

#define DSI_LP_MRDP_DISABLE 0
#define DSI_LP_MRDP_ENABLE DSI_CMCR_MRDPS
#define IS_DSI_LP_MRDP(LP_MRDP)                     (((LP_MRDP) == DSI_LP_MRDP_DISABLE) || ((LP_MRDP) == DSI_LP_MRDP_ENABLE))

#define DSI_HS_PM_DISABLE 0
#define DSI_HS_PM_ENABLE DSI_TCCR3_PM

#define DSI_AUTO_CLK_LANE_CTRL_DISABLE 0
#define DSI_AUTO_CLK_LANE_CTRL_ENABLE  DSI_CLCR_ACR
#define IS_DSI_AUTO_CLKLANE_CONTROL(AutoClkLane)    (((AutoClkLane) == DSI_AUTO_CLK_LANE_CTRL_DISABLE) || ((AutoClkLane) == DSI_AUTO_CLK_LANE_CTRL_ENABLE))

#define DSI_ONE_DATA_LANE  0
#define DSI_TWO_DATA_LANES 1
#define IS_DSI_NUMBER_OF_LANES(NumberOfLanes)       (((NumberOfLanes) == DSI_ONE_DATA_LANE) || ((NumberOfLanes) == DSI_TWO_DATA_LANES))

#define DSI_FLOW_CONTROL_CRC_RX  DSI_PCR_CRCRXE
#define DSI_FLOW_CONTROL_ECC_RX  DSI_PCR_ECCRXE
#define DSI_FLOW_CONTROL_BTA     DSI_PCR_BTAE
#define DSI_FLOW_CONTROL_EOTP_RX DSI_PCR_ETRXE
#define DSI_FLOW_CONTROL_EOTP_TX DSI_PCR_ETTXE
#define DSI_FLOW_CONTROL_ALL     (DSI_FLOW_CONTROL_CRC_RX | DSI_FLOW_CONTROL_ECC_RX | \
                                  DSI_FLOW_CONTROL_BTA | DSI_FLOW_CONTROL_EOTP_RX | \
                                  DSI_FLOW_CONTROL_EOTP_TX)
#define IS_DSI_FLOW_CONTROL(FlowControl)            (((FlowControl) | DSI_FLOW_CONTROL_ALL) == DSI_FLOW_CONTROL_ALL)

#define DSI_RGB565 ((uint32_t)0x00000000)
#define DSI_RGB666 ((uint32_t)0x00000003)
#define DSI_RGB888 ((uint32_t)0x00000005)
#define IS_DSI_COLOR_CODING(ColorCoding)            ((ColorCoding) <= 5)

#define DSI_LOOSELY_PACKED_ENABLE  DSI_LCOLCR_LPE
#define DSI_LOOSELY_PACKED_DISABLE 0
#define IS_DSI_LOOSELY_PACKED(LooselyPacked)        (((LooselyPacked) == DSI_LOOSELY_PACKED_ENABLE) || ((LooselyPacked) == DSI_LOOSELY_PACKED_DISABLE))

#define DSI_HSYNC_ACTIVE_HIGH       0
#define DSI_HSYNC_ACTIVE_LOW        DSI_LPCR_HSP
#define IS_DSI_HSYNC_POLARITY(HSYNC)                (((HSYNC) == DSI_HSYNC_ACTIVE_HIGH) || ((HSYNC) == DSI_HSYNC_ACTIVE_LOW))

#define DSI_VSYNC_ACTIVE_HIGH       0
#define DSI_VSYNC_ACTIVE_LOW        DSI_LPCR_VSP
#define IS_DSI_VSYNC_POLARITY(VSYNC)                (((VSYNC) == DSI_VSYNC_ACTIVE_HIGH) || ((VSYNC) == DSI_VSYNC_ACTIVE_LOW))

#define DSI_DATA_ENABLE_ACTIVE_HIGH 0
#define DSI_DATA_ENABLE_ACTIVE_LOW  DSI_LPCR_DEP
#define IS_DSI_DE_POLARITY(DataEnable)              (((DataEnable) == DSI_DATA_ENABLE_ACTIVE_HIGH) || ((DataEnable) == DSI_DATA_ENABLE_ACTIVE_LOW))

#define DSI_PLL_IN_DIV1 ((uint32_t)0x00000001)
#define DSI_PLL_IN_DIV2 ((uint32_t)0x00000002)
#define DSI_PLL_IN_DIV3 ((uint32_t)0x00000003)
#define DSI_PLL_IN_DIV4 ((uint32_t)0x00000004)
#define DSI_PLL_IN_DIV5 ((uint32_t)0x00000005)
#define DSI_PLL_IN_DIV6 ((uint32_t)0x00000006)
#define DSI_PLL_IN_DIV7 ((uint32_t)0x00000007)
#define IS_DSI_PLL_IDF(IDF)                         (((IDF) == DSI_PLL_IN_DIV1) || \
                                                     ((IDF) == DSI_PLL_IN_DIV2) || \
                                                     ((IDF) == DSI_PLL_IN_DIV3) || \
                                                     ((IDF) == DSI_PLL_IN_DIV4) || \
                                                     ((IDF) == DSI_PLL_IN_DIV5) || \
                                                     ((IDF) == DSI_PLL_IN_DIV6) || \
                                                     ((IDF) == DSI_PLL_IN_DIV7))

#define DSI_PLL_OUT_DIV1 ((uint32_t)0x00000000)
#define DSI_PLL_OUT_DIV2 ((uint32_t)0x00000001)
#define DSI_PLL_OUT_DIV4 ((uint32_t)0x00000002)
#define DSI_PLL_OUT_DIV8 ((uint32_t)0x00000003)
#define IS_DSI_PLL_ODF(ODF)                         (((ODF) == DSI_PLL_OUT_DIV1) || \
                                                     ((ODF) == DSI_PLL_OUT_DIV2) || \
                                                     ((ODF) == DSI_PLL_OUT_DIV4) || \
                                                     ((ODF) == DSI_PLL_OUT_DIV8))
#define IS_DSI_PLL_NDIV(NDIV)                       ((10 <= (NDIV)) && ((NDIV) <= 125))

#define DSI_FLAG_TE    DSI_WISR_TEIF
#define DSI_FLAG_ER    DSI_WISR_ERIF
#define DSI_FLAG_BUSY  DSI_WISR_BUSY
#define DSI_FLAG_PLLLS DSI_WISR_PLLLS
#define DSI_FLAG_PLLL  DSI_WISR_PLLLIF
#define DSI_FLAG_PLLU  DSI_WISR_PLLUIF
#define DSI_FLAG_RRS   DSI_WISR_RRS
#define DSI_FLAG_RR    DSI_WISR_RRIF

#define IS_DSI_CLEAR_FLAG(FLAG) (((FLAG) == DSI_FLAG_TE) || ((FLAG) == DSI_FLAG_ER) || \
                                 ((FLAG) == DSI_FLAG_PLLL) || ((FLAG) == DSI_FLAG_PLLU) || \
                                 ((FLAG) == DSI_FLAG_RR))
#define IS_DSI_GET_FLAG(FLAG) (((FLAG) == DSI_FLAG_TE) || ((FLAG) == DSI_FLAG_ER) || \
                               ((FLAG) == DSI_FLAG_BUSY) || ((FLAG) == DSI_FLAG_PLLLS) || \
                               ((FLAG) == DSI_FLAG_PLLL) || ((FLAG) == DSI_FLAG_PLLU) || \
                               ((FLAG) == DSI_FLAG_RRS) || ((FLAG) == DSI_FLAG_RR))

#define DSI_IT_TE   DSI_WIER_TEIE
#define DSI_IT_ER   DSI_WIER_ERIE
#define DSI_IT_PLLL DSI_WIER_PLLLIE
#define DSI_IT_PLLU DSI_WIER_PLLUIE
#define DSI_IT_RR   DSI_WIER_RRIE

#define IS_DSI_IT(IT) (((IT) == DSI_IT_TE) || ((IT) == DSI_IT_ER) || \
                       ((IT) == DSI_IT_PLLL) || ((IT) == DSI_IT_PLLU) || \
                       ((IT) == DSI_IT_RR))

#define DSI_DCS_SHORT_PKT_WRITE_P0 ((uint32_t)0x00000005)
#define DSI_DCS_SHORT_PKT_WRITE_P1 ((uint32_t)0x00000015)
#define DSI_GEN_SHORT_PKT_WRITE_P0 ((uint32_t)0x00000003)
#define DSI_GEN_SHORT_PKT_WRITE_P1 ((uint32_t)0x00000013)
#define DSI_GEN_SHORT_PKT_WRITE_P2 ((uint32_t)0x00000023)
#define IS_DSI_SHORT_WRITE_PACKET_TYPE(MODE)        (((MODE) == DSI_DCS_SHORT_PKT_WRITE_P0) || \
                                                     ((MODE) == DSI_DCS_SHORT_PKT_WRITE_P1) || \
                                                     ((MODE) == DSI_GEN_SHORT_PKT_WRITE_P0) || \
                                                     ((MODE) == DSI_GEN_SHORT_PKT_WRITE_P1) || \
                                                     ((MODE) == DSI_GEN_SHORT_PKT_WRITE_P2))

#define DSI_DCS_LONG_PKT_WRITE ((uint32_t)0x00000039)
#define DSI_GEN_LONG_PKT_WRITE ((uint32_t)0x00000029)
#define IS_DSI_LONG_WRITE_PACKET_TYPE(MODE)         (((MODE) == DSI_DCS_LONG_PKT_WRITE) || \
                                                     ((MODE) == DSI_GEN_LONG_PKT_WRITE))

#define DSI_DCS_SHORT_PKT_READ    ((uint32_t)0x00000006)
#define DSI_GEN_SHORT_PKT_READ_P0 ((uint32_t)0x00000004)
#define DSI_GEN_SHORT_PKT_READ_P1 ((uint32_t)0x00000014)
#define DSI_GEN_SHORT_PKT_READ_P2 ((uint32_t)0x00000024)
#define IS_DSI_READ_PACKET_TYPE(MODE)               (((MODE) == DSI_DCS_SHORT_PKT_READ) || \
                                                     ((MODE) == DSI_GEN_SHORT_PKT_READ_P0) || \
                                                     ((MODE) == DSI_GEN_SHORT_PKT_READ_P1) || \
                                                     ((MODE) == DSI_GEN_SHORT_PKT_READ_P2))

#define DSI_ERROR_NONE 0
#define DSI_ERROR_ACK  ((uint32_t)0x00000001)
#define DSI_ERROR_PHY  ((uint32_t)0x00000002)
#define DSI_ERROR_TX   ((uint32_t)0x00000004)
#define DSI_ERROR_RX   ((uint32_t)0x00000008)
#define DSI_ERROR_ECC  ((uint32_t)0x00000010)
#define DSI_ERROR_CRC  ((uint32_t)0x00000020)
#define DSI_ERROR_PSE  ((uint32_t)0x00000040)
#define DSI_ERROR_EOT  ((uint32_t)0x00000080)
#define DSI_ERROR_OVF  ((uint32_t)0x00000100)
#define DSI_ERROR_GEN  ((uint32_t)0x00000200)

#define DSI_CLOCK_LANE ((uint32_t)0x00000000)
#define DSI_DATA_LANES ((uint32_t)0x00000001)
#define IS_DSI_LANE_GROUP(Lane)                     (((Lane) == DSI_CLOCK_LANE) || ((Lane) == DSI_DATA_LANES))

#define DSI_SLEW_RATE_HSTX ((uint32_t)0x00000000)
#define DSI_SLEW_RATE_LPTX ((uint32_t)0x00000001)
#define DSI_HS_DELAY       ((uint32_t)0x00000002)
#define IS_DSI_COMMUNICATION_DELAY(CommDelay)       (((CommDelay) == DSI_SLEW_RATE_HSTX) || ((CommDelay) == DSI_SLEW_RATE_LPTX) || ((CommDelay) == DSI_HS_DELAY))

#define DSI_SWAP_LANE_PINS   ((uint32_t)0x00000000)
#define DSI_INVERT_HS_SIGNAL ((uint32_t)0x00000001)
#define IS_DSI_CUSTOM_LANE(CustomLane)              (((CustomLane) == DSI_SWAP_LANE_PINS) || ((CustomLane) == DSI_INVERT_HS_SIGNAL))

#define DSI_CLOCK_LANE ((uint32_t)0x00000000)
#define DSI_DATA_LANE0 ((uint32_t)0x00000001)
#define DSI_DATA_LANE1 ((uint32_t)0x00000002)
#define IS_DSI_LANE(Lane)                           (((Lane) == DSI_CLOCK_LANE) || ((Lane) == DSI_DATA_LANE0) || ((Lane) == DSI_DATA_LANE1))

#define DSI_TCLK_POST    ((uint32_t)0x00000000)
#define DSI_TLPX_CLK     ((uint32_t)0x00000001)
#define DSI_THS_EXIT     ((uint32_t)0x00000002)
#define DSI_TLPX_DATA    ((uint32_t)0x00000003)
#define DSI_THS_ZERO     ((uint32_t)0x00000004)
#define DSI_THS_TRAIL    ((uint32_t)0x00000005)
#define DSI_THS_PREPARE  ((uint32_t)0x00000006)
#define DSI_TCLK_ZERO    ((uint32_t)0x00000007)
#define DSI_TCLK_PREPARE ((uint32_t)0x00000008)
#define IS_DSI_PHY_TIMING(Timing)                   (((Timing) == DSI_TCLK_POST   ) || \
                                                     ((Timing) == DSI_TLPX_CLK    ) || \
                                                     ((Timing) == DSI_THS_EXIT    ) || \
                                                     ((Timing) == DSI_TLPX_DATA   ) || \
                                                     ((Timing) == DSI_THS_ZERO    ) || \
                                                     ((Timing) == DSI_THS_TRAIL   ) || \
                                                     ((Timing) == DSI_THS_PREPARE ) || \
                                                     ((Timing) == DSI_TCLK_ZERO   ) || \
                                                     ((Timing) == DSI_TCLK_PREPARE))

#define IS_DSI_ALL_PERIPH(PERIPH) ((PERIPH) == DSI)

void DSI_DeInit(DSI_TypeDef *DSIx);
void DSI_Init(DSI_TypeDef *DSIx,DSI_InitTypeDef* DSI_InitStruct, DSI_PLLInitTypeDef *PLLInit);
void DSI_StructInit(DSI_InitTypeDef* DSI_InitStruct, DSI_HOST_TimeoutTypeDef* DSI_HOST_TimeoutInitStruct);
void DSI_SetGenericVCID(DSI_TypeDef *DSIx, uint32_t VirtualChannelID);
void DSI_ConfigVideoMode(DSI_TypeDef *DSIx, DSI_VidCfgTypeDef *VidCfg);
void DSI_ConfigAdaptedCommandMode(DSI_TypeDef *DSIx, DSI_CmdCfgTypeDef *CmdCfg);
void DSI_ConfigCommand(DSI_TypeDef *DSIx, DSI_LPCmdTypeDef *LPCmd);
void DSI_ConfigFlowControl(DSI_TypeDef *DSIx, uint32_t FlowControl);
void DSI_ConfigPhyTimer(DSI_TypeDef *DSIx, DSI_PHY_TimerTypeDef *PhyTimers);
void DSI_ConfigHostTimeouts(DSI_TypeDef *DSIx, DSI_HOST_TimeoutTypeDef *HostTimeouts);
void DSI_PatternGeneratorStart(DSI_TypeDef *DSIx, uint32_t Mode, uint32_t Orientation);
void DSI_PatternGeneratorStop(DSI_TypeDef *DSIx);
void DSI_Start(DSI_TypeDef *DSIx);
void DSI_Stop(DSI_TypeDef *DSIx);
void DSI_Refresh(DSI_TypeDef *DSIx);
void DSI_ColorMode(DSI_TypeDef *DSIx, uint32_t ColorMode);
void DSI_Shutdown(DSI_TypeDef *DSIx, uint32_t Shutdown);

#define DSI_ConfigLowPowerCommand DSI_ConfigCommand

void DSI_ShortWrite(DSI_TypeDef *DSIx, uint32_t ChannelID, uint32_t Mode, uint32_t Param1, uint32_t Param2);
void DSI_LongWrite(DSI_TypeDef *DSIx, uint32_t ChannelID, uint32_t Mode, uint32_t NbParams, uint32_t Param1, uint8_t* ParametersTable);
void DSI_Read(DSI_TypeDef *DSIx, uint32_t ChannelNbr, uint8_t* Array, uint32_t Size, uint32_t Mode, uint32_t DCSCmd, uint8_t* ParametersTable);

void DSI_EnterULPMData(DSI_TypeDef *DSIx);
void DSI_ExitULPMData(DSI_TypeDef *DSIx);
void DSI_EnterULPM(DSI_TypeDef *DSIx);
void DSI_ExitULPM(DSI_TypeDef *DSIx);
void DSI_SetSlewRateAndDelayTuning(DSI_TypeDef *DSIx, uint32_t CommDelay, uint32_t Lane, uint32_t Value);
void DSI_SetLowPowerRXFilter(DSI_TypeDef *DSIx, uint32_t Frequency);
void DSI_SetSDD(DSI_TypeDef *DSIx, FunctionalState State);
void DSI_SetLanePinsConfiguration(DSI_TypeDef *DSIx, uint32_t CustomLane, uint32_t Lane, FunctionalState State);
void DSI_SetPHYTimings(DSI_TypeDef *DSIx, uint32_t Timing, FunctionalState State, uint32_t Value);
void DSI_ForceTXStopMode(DSI_TypeDef *DSIx, uint32_t Lane, FunctionalState State);
void DSI_ForceRXLowPower(DSI_TypeDef *DSIx, FunctionalState State);
void DSI_ForceDataLanesInRX(DSI_TypeDef *DSIx, FunctionalState State);
void DSI_SetPullDown(DSI_TypeDef *DSIx, FunctionalState State);
void DSI_SetContentionDetectionOff(DSI_TypeDef *DSIx, FunctionalState State);

void DSI_ITConfig(DSI_TypeDef* DSIx, uint32_t DSI_IT, FunctionalState NewState);
FlagStatus DSI_GetFlagStatus(DSI_TypeDef* DSIx, uint16_t DSI_FLAG);
void DSI_ClearFlag(DSI_TypeDef* DSIx, uint16_t DSI_FLAG);
ITStatus DSI_GetITStatus(DSI_TypeDef* DSIx, uint32_t DSI_IT);
void DSI_ClearITPendingBit(DSI_TypeDef* DSIx, uint32_t DSI_IT);
void DSI_ConfigErrorMonitor(DSI_TypeDef *DSIx, uint32_t ActiveErrors);

#endif

#ifdef __cplusplus
}
#endif

#endif
