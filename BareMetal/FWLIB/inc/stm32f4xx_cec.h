#ifndef __STM32F4XX_CEC_H
#define __STM32F4XX_CEC_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

#if defined(STM32F446xx)

typedef struct
{
  uint32_t CEC_SignalFreeTime;

  uint32_t CEC_RxTolerance;

  uint32_t CEC_StopReception;

  uint32_t CEC_BitRisingError;

  uint32_t CEC_LongBitPeriodError;

  uint32_t CEC_BRDNoGen;

  uint32_t CEC_SFTOption;

}CEC_InitTypeDef;

#define CEC_SignalFreeTime_Standard     ((uint32_t)0x00000000)
#define CEC_SignalFreeTime_1T           ((uint32_t)0x00000001)
#define CEC_SignalFreeTime_2T           ((uint32_t)0x00000002)
#define CEC_SignalFreeTime_3T           ((uint32_t)0x00000003)
#define CEC_SignalFreeTime_4T           ((uint32_t)0x00000004)
#define CEC_SignalFreeTime_5T           ((uint32_t)0x00000005)
#define CEC_SignalFreeTime_6T           ((uint32_t)0x00000006)
#define CEC_SignalFreeTime_7T           ((uint32_t)0x00000007)

#define IS_CEC_SIGNAL_FREE_TIME(TIME) (((TIME) == CEC_SignalFreeTime_Standard) || \
                                       ((TIME) == CEC_SignalFreeTime_1T)|| \
                                       ((TIME) == CEC_SignalFreeTime_2T)|| \
                                       ((TIME) == CEC_SignalFreeTime_3T)|| \
                                       ((TIME) == CEC_SignalFreeTime_4T)|| \
                                       ((TIME) == CEC_SignalFreeTime_5T)|| \
                                       ((TIME) == CEC_SignalFreeTime_6T)|| \
                                       ((TIME) == CEC_SignalFreeTime_7T))

#define CEC_RxTolerance_Standard        ((uint32_t)0x00000000)
#define CEC_RxTolerance_Extended        CEC_CFGR_RXTOL

#define IS_CEC_RX_TOLERANCE(TOLERANCE) (((TOLERANCE) == CEC_RxTolerance_Standard) || \
                                        ((TOLERANCE) == CEC_RxTolerance_Extended))

#define CEC_StopReception_Off           ((uint32_t)0x00000000)
#define CEC_StopReception_On            CEC_CFGR_BRESTP

#define IS_CEC_STOP_RECEPTION(RECEPTION) (((RECEPTION) == CEC_StopReception_On) || \
                                          ((RECEPTION) == CEC_StopReception_Off))

#define CEC_BitRisingError_Off          ((uint32_t)0x00000000)
#define CEC_BitRisingError_On           CEC_CFGR_BREGEN

#define IS_CEC_BIT_RISING_ERROR(ERROR) (((ERROR) == CEC_BitRisingError_Off) || \
                                        ((ERROR) == CEC_BitRisingError_On))

#define CEC_LongBitPeriodError_Off      ((uint32_t)0x00000000)
#define CEC_LongBitPeriodError_On       CEC_CFGR_LREGEN

#define IS_CEC_LONG_BIT_PERIOD_ERROR(ERROR) (((ERROR) == CEC_LongBitPeriodError_Off) || \
                                             ((ERROR) == CEC_LongBitPeriodError_On))

#define CEC_BRDNoGen_Off      ((uint32_t)0x00000000)
#define CEC_BRDNoGen_On       CEC_CFGR_BRDNOGEN

#define IS_CEC_BDR_NO_GEN_ERROR(ERROR) (((ERROR) == CEC_BRDNoGen_Off) || \
                                        ((ERROR) == CEC_BRDNoGen_On))

#define CEC_SFTOption_Off              ((uint32_t)0x00000000)
#define CEC_SFTOption_On               CEC_CFGR_SFTOPT

#define IS_CEC_SFT_OPTION(OPTION) (((OPTION) == CEC_SFTOption_Off) || \
                                  ((OPTION) == CEC_SFTOption_On))

#define IS_CEC_ADDRESS(ADDRESS)         ((ADDRESS) < 0x10)

#define CEC_IT_TXACKE                   CEC_IER_TXACKEIE
#define CEC_IT_TXERR                    CEC_IER_TXERRIE
#define CEC_IT_TXUDR                    CEC_IER_TXUDRIE
#define CEC_IT_TXEND                    CEC_IER_TXENDIE
#define CEC_IT_TXBR                     CEC_IER_TXBRIE
#define CEC_IT_ARBLST                   CEC_IER_ARBLSTIE
#define CEC_IT_RXACKE                   CEC_IER_RXACKEIE
#define CEC_IT_LBPE                     CEC_IER_LBPEIE
#define CEC_IT_SBPE                     CEC_IER_SBPEIE
#define CEC_IT_BRE                      CEC_IER_BREIEIE
#define CEC_IT_RXOVR                    CEC_IER_RXOVRIE
#define CEC_IT_RXEND                    CEC_IER_RXENDIE
#define CEC_IT_RXBR                     CEC_IER_RXBRIE

#define IS_CEC_IT(IT) ((((IT) & (uint32_t)0xFFFFE000) == 0x00) && ((IT) != 0x00))

#define IS_CEC_GET_IT(IT) (((IT) == CEC_IT_TXACKE) || \
                           ((IT) == CEC_IT_TXERR)|| \
                           ((IT) == CEC_IT_TXUDR)|| \
                           ((IT) == CEC_IT_TXEND)|| \
                           ((IT) == CEC_IT_TXBR)|| \
                           ((IT) == CEC_IT_ARBLST)|| \
                           ((IT) == CEC_IT_RXACKE)|| \
                           ((IT) == CEC_IT_LBPE)|| \
                           ((IT) == CEC_IT_SBPE)|| \
                           ((IT) == CEC_IT_BRE)|| \
                           ((IT) == CEC_IT_RXOVR)|| \
                           ((IT) == CEC_IT_RXEND)|| \
                           ((IT) == CEC_IT_RXBR))

#define CEC_FLAG_TXACKE                 CEC_ISR_TXACKE
#define CEC_FLAG_TXERR                  CEC_ISR_TXERR
#define CEC_FLAG_TXUDR                  CEC_ISR_TXUDR
#define CEC_FLAG_TXEND                  CEC_ISR_TXEND
#define CEC_FLAG_TXBR                   CEC_ISR_TXBR
#define CEC_FLAG_ARBLST                 CEC_ISR_ARBLST
#define CEC_FLAG_RXACKE                 CEC_ISR_RXACKE
#define CEC_FLAG_LBPE                   CEC_ISR_LBPE
#define CEC_FLAG_SBPE                   CEC_ISR_SBPE
#define CEC_FLAG_BRE                    CEC_ISR_BRE
#define CEC_FLAG_RXOVR                  CEC_ISR_RXOVR
#define CEC_FLAG_RXEND                  CEC_ISR_RXEND
#define CEC_FLAG_RXBR                   CEC_ISR_RXBR

#define IS_CEC_CLEAR_FLAG(FLAG) ((((FLAG) & (uint32_t)0xFFFFE000) == 0x00) && ((FLAG) != 0x00))

#define IS_CEC_GET_FLAG(FLAG) (((FLAG) == CEC_FLAG_TXACKE) || \
                               ((FLAG) == CEC_FLAG_TXERR)|| \
                               ((FLAG) == CEC_FLAG_TXUDR)|| \
                               ((FLAG) == CEC_FLAG_TXEND)|| \
                               ((FLAG) == CEC_FLAG_TXBR)|| \
                               ((FLAG) == CEC_FLAG_ARBLST)|| \
                               ((FLAG) == CEC_FLAG_RXACKE)|| \
                               ((FLAG) == CEC_FLAG_LBPE)|| \
                               ((FLAG) == CEC_FLAG_SBPE)|| \
                               ((FLAG) == CEC_FLAG_BRE)|| \
                               ((FLAG) == CEC_FLAG_RXOVR)|| \
                               ((FLAG) == CEC_FLAG_RXEND)|| \
                               ((FLAG) == CEC_FLAG_RXBR))

void CEC_DeInit(void);

void CEC_Init(CEC_InitTypeDef* CEC_InitStruct);
void CEC_StructInit(CEC_InitTypeDef* CEC_InitStruct);
void CEC_Cmd(FunctionalState NewState);
void CEC_ListenModeCmd(FunctionalState NewState);
void CEC_OwnAddressConfig(uint8_t CEC_OwnAddress);
void CEC_OwnAddressClear(void);

void CEC_SendData(uint8_t Data);
uint8_t CEC_ReceiveData(void);
void CEC_StartOfMessage(void);
void CEC_EndOfMessage(void);

void CEC_ITConfig(uint16_t CEC_IT, FunctionalState NewState);
FlagStatus CEC_GetFlagStatus(uint16_t CEC_FLAG);
void CEC_ClearFlag(uint32_t CEC_FLAG);
ITStatus CEC_GetITStatus(uint16_t CEC_IT);
void CEC_ClearITPendingBit(uint16_t CEC_IT);
#endif

#ifdef __cplusplus
}
#endif

#endif
