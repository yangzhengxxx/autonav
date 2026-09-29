#ifndef __STM32F4XX_DFSDM_H
#define __STM32F4XX_DFSDM_H

#ifdef __cplusplus
 extern "C" {
#endif

#if defined(STM32F412xG) || defined(STM32F413_423xx)

#include "stm32f4xx.h"

typedef struct
{
  uint32_t DFSDM_Interface;

  uint32_t DFSDM_Clock;

  uint32_t DFSDM_Input;

  uint32_t DFSDM_Redirection;

  uint32_t DFSDM_PackingMode;

  uint32_t DFSDM_DataRightShift;

  uint32_t DFSDM_Offset;

  uint32_t DFSDM_CLKAbsenceDetector;

  uint32_t DFSDM_ShortCircuitDetector;

}DFSDM_TransceiverInitTypeDef;

typedef struct
{
  uint32_t DFSDM_SincOrder;

  uint32_t DFSDM_FilterOversamplingRatio;

  uint32_t DFSDM_IntegratorOversamplingRatio;

}DFSDM_FilterInitTypeDef;

#define DFSDM_Interface_SPI_RisingEdge   ((uint32_t)0x00000000)
#define DFSDM_Interface_SPI_FallingEdge  ((uint32_t)0x00000001)
#define DFSDM_Interface_Manchester1      ((uint32_t)0x00000002)
#define DFSDM_Interface_Manchester2      ((uint32_t)0x00000003)

#define IS_DFSDM_INTERFACE(INTERFACE)    (((INTERFACE) == DFSDM_Interface_SPI_RisingEdge) || \
                                         ((INTERFACE) == DFSDM_Interface_SPI_FallingEdge) || \
                                         ((INTERFACE) == DFSDM_Interface_Manchester1)     || \
                                         ((INTERFACE) == DFSDM_Interface_Manchester2))

#define DFSDM_Clock_External             ((uint32_t)0x00000000)
#define DFSDM_Clock_Internal             ((uint32_t)0x00000004)
#define DFSDM_Clock_InternalDiv2_Mode1   ((uint32_t)0x00000008)

#define DFSDM_Clock_InternalDiv2_Mode2   ((uint32_t)0x0000000C)

#define IS_DFSDM_CLOCK(CLOCK)            (((CLOCK) == DFSDM_Clock_External)          || \
                                         ((CLOCK) == DFSDM_Clock_Internal)           || \
                                         ((CLOCK) == DFSDM_Clock_InternalDiv2_Mode1) || \
                                         ((CLOCK) == DFSDM_Clock_InternalDiv2_Mode2))

#define DFSDM_Input_External     ((uint32_t)0x00000000)
#define DFSDM_Input_ADC          ((uint32_t)0x00001000)
#define DFSDM_Input_Internal     ((uint32_t)0x00002000)

#define IS_DFSDM_Input_MODE(INPUT)      (((INPUT) == DFSDM_Input_External) || \
                                         ((INPUT) == DFSDM_Input_ADC)      || \
                                         ((INPUT) == DFSDM_Input_Internal))

#define DFSDM_Redirection_Disabled       ((uint32_t)0x00000000)
#define DFSDM_Redirection_Enabled         DFSDM_CHCFGR1_CHINSEL

#define IS_DFSDM_Redirection_STATE(STATE)      (((STATE) == DFSDM_Redirection_Disabled) || \
                                                ((STATE) == DFSDM_Redirection_Enabled))

#define DFSDM_PackingMode_Standard     ((uint32_t)0x00000000)
#define DFSDM_PackingMode_Interleaved  ((uint32_t)0x00004000)

#define DFSDM_PackingMode_Dual         ((uint32_t)0x00008000)

#define IS_DFSDM_PACK_MODE(MODE)        (((MODE) == DFSDM_PackingMode_Standard)    || \
                                         ((MODE) == DFSDM_PackingMode_Interleaved) || \
                                         ((MODE) == DFSDM_PackingMode_Dual))

#define DFSDM_CLKAbsenceDetector_Enable     DFSDM_CHCFGR1_CKABEN
#define DFSDM_CLKAbsenceDetector_Disable    ((uint32_t)0x00000000)

#define IS_DFSDM_CLK_DETECTOR_STATE(STATE)  (((STATE) == DFSDM_CLKAbsenceDetector_Enable) || \
                                             ((STATE) == DFSDM_CLKAbsenceDetector_Disable))

#define DFSDM_ShortCircuitDetector_Enable   DFSDM_CHCFGR1_SCDEN
#define DFSDM_ShortCircuitDetector_Disable  ((uint32_t)0x00000000)

#define IS_DFSDM_SC_DETECTOR_STATE(STATE)  (((STATE) == DFSDM_ShortCircuitDetector_Enable) || \
                                            ((STATE) == DFSDM_ShortCircuitDetector_Disable))

#define DFSDM_SincOrder_FastSinc        ((uint32_t)0x00000000)
#define DFSDM_SincOrder_Sinc1           ((uint32_t)0x20000000)
#define DFSDM_SincOrder_Sinc2           ((uint32_t)0x40000000)
#define DFSDM_SincOrder_Sinc3           ((uint32_t)0x60000000)
#define DFSDM_SincOrder_Sinc4           ((uint32_t)0x80000000)
#define DFSDM_SincOrder_Sinc5           ((uint32_t)0xA0000000)

#define IS_DFSDM_SINC_ORDER(ORDER)        (((ORDER) == DFSDM_SincOrder_FastSinc) || \
                                          ((ORDER) == DFSDM_SincOrder_Sinc1)     || \
                                          ((ORDER) == DFSDM_SincOrder_Sinc2)     || \
                                          ((ORDER) == DFSDM_SincOrder_Sinc3)     || \
                                          ((ORDER) == DFSDM_SincOrder_Sinc4)     || \
                                          ((ORDER) == DFSDM_SincOrder_Sinc5))

#define DFSDM_SCDBreak_0                 ((uint32_t)0x00001000)
#define DFSDM_SCDBreak_1                 ((uint32_t)0x00002000)
#define DFSDM_SCDBreak_2                 ((uint32_t)0x00004000)
#define DFSDM_SCDBreak_3                 ((uint32_t)0x00008000)

#define IS_DFSDM_SCD_BREAK_SIGNAL(RANK)    (((RANK) == DFSDM_SCDBreak_0) || \
                                           ((RANK) == DFSDM_SCDBreak_1)  || \
                                           ((RANK) == DFSDM_SCDBreak_2)  || \
                                           ((RANK) == DFSDM_SCDBreak_3))

#define DFSDM_AWDSincOrder_Fast            ((uint32_t)0x00000000)
#define DFSDM_AWDSincOrder_Sinc1           ((uint32_t)0x00400000)
#define DFSDM_AWDSincOrder_Sinc2           ((uint32_t)0x00800000)
#define DFSDM_AWDSincOrder_Sinc3           ((uint32_t)0x00C00000)

#define IS_DFSDM_AWD_SINC_ORDER(ORDER)    (((ORDER) == DFSDM_AWDSincOrder_Fast)  || \
                                           ((ORDER) == DFSDM_AWDSincOrder_Sinc1) || \
                                           ((ORDER) == DFSDM_AWDSincOrder_Sinc2) || \
                                           ((ORDER) == DFSDM_AWDSincOrder_Sinc3))

#define DFSDM_AWDChannel0               ((uint32_t)0x00010000)
#define DFSDM_AWDChannel1               ((uint32_t)0x00020000)
#define DFSDM_AWDChannel2               ((uint32_t)0x00040000)
#define DFSDM_AWDChannel3               ((uint32_t)0x00080000)
#define DFSDM_AWDChannel4               ((uint32_t)0x00100000)
#define DFSDM_AWDChannel5               ((uint32_t)0x00200000)
#define DFSDM_AWDChannel6               ((uint32_t)0x00400000)
#define DFSDM_AWDChannel7               ((uint32_t)0x00800000)

#define IS_DFSDM_AWD_CHANNEL(CHANNEL)    (((CHANNEL) == DFSDM_AWDChannel0) || \
                                          ((CHANNEL) == DFSDM_AWDChannel1) || \
                                          ((CHANNEL) == DFSDM_AWDChannel2) || \
                                          ((CHANNEL) == DFSDM_AWDChannel3) || \
                                          ((CHANNEL) == DFSDM_AWDChannel4) || \
                                          ((CHANNEL) == DFSDM_AWDChannel5) || \
                                          ((CHANNEL) == DFSDM_AWDChannel6) || \
                                          ((CHANNEL) == DFSDM_AWDChannel7))

#define DFSDM_Threshold_Low               ((uint8_t)0x00)
#define DFSDM_Threshold_High              ((uint8_t)0x08)

#define IS_DFSDM_Threshold(THR)          (((THR) == DFSDM_Threshold_Low) || \
                                          ((THR) == DFSDM_Threshold_High))

#define DFSDM_AWDFastMode_Disable         ((uint32_t)0x00000000)
#define DFSDM_AWDFastMode_Enable          ((uint32_t)0x40000000)

#define IS_DFSDM_AWD_MODE(MODE)          (((MODE) == DFSDM_AWDFastMode_Disable) || \
                                          ((MODE) == DFSDM_AWDFastMode_Enable))

#define DFSDM_ClkOutSource_SysClock        ((uint32_t)0x00000000)
#define DFSDM_ClkOutSource_AudioClock      DFSDM_CHCFGR1_CKOUTSRC

#define IS_DFSDM_CLOCK_OUT_SOURCE(SRC)    (((SRC) == DFSDM_ClkOutSource_SysClock) || \
                                           ((SRC) == DFSDM_ClkOutSource_AudioClock))

#define DFSDM_DMAConversionMode_Regular     ((uint32_t)0x00000010)
#define DFSDM_DMAConversionMode_Injected    ((uint32_t)0x00000000)

#define IS_DFSDM_CONVERSION_MODE(MODE)    (((MODE) == DFSDM_DMAConversionMode_Regular) || \
                                           ((MODE) == DFSDM_DMAConversionMode_Injected))

#define DFSDM_ExtremChannel0              ((uint32_t)0x00000100)
#define DFSDM_ExtremChannel1              ((uint32_t)0x00000200)
#define DFSDM_ExtremChannel2              ((uint32_t)0x00000400)
#define DFSDM_ExtremChannel3              ((uint32_t)0x00000800)
#define DFSDM_ExtremChannel4              ((uint32_t)0x00001000)
#define DFSDM_ExtremChannel5              ((uint32_t)0x00002000)
#define DFSDM_ExtremChannel6              ((uint32_t)0x00004000)
#define DFSDM_ExtremChannel7              ((uint32_t)0x00008000)

#define IS_DFSDM_EXTREM_CHANNEL(CHANNEL) (((CHANNEL) == DFSDM_ExtremChannel0) || \
                                          ((CHANNEL) == DFSDM_ExtremChannel1) || \
                                          ((CHANNEL) == DFSDM_ExtremChannel2) || \
                                          ((CHANNEL) == DFSDM_ExtremChannel3) || \
                                          ((CHANNEL) == DFSDM_ExtremChannel4) || \
                                          ((CHANNEL) == DFSDM_ExtremChannel5) || \
                                          ((CHANNEL) == DFSDM_ExtremChannel6) || \
                                          ((CHANNEL) == DFSDM_ExtremChannel7))

#define DFSDM_InjectedChannel0            ((uint32_t)0x00000001)
#define DFSDM_InjectedChannel1            ((uint32_t)0x00000002)
#define DFSDM_InjectedChannel2            ((uint32_t)0x00000004)
#define DFSDM_InjectedChannel3            ((uint32_t)0x00000008)
#define DFSDM_InjectedChannel4            ((uint32_t)0x00000010)
#define DFSDM_InjectedChannel5            ((uint32_t)0x00000020)
#define DFSDM_InjectedChannel6            ((uint32_t)0x00000040)
#define DFSDM_InjectedChannel7            ((uint32_t)0x00000080)

#define IS_DFSDM_INJECT_CHANNEL(CHANNEL) (((CHANNEL) == DFSDM_InjectedChannel0) || \
                                          ((CHANNEL) == DFSDM_InjectedChannel1) || \
                                          ((CHANNEL) == DFSDM_InjectedChannel2) || \
                                          ((CHANNEL) == DFSDM_InjectedChannel3) || \
                                          ((CHANNEL) == DFSDM_InjectedChannel4) || \
                                          ((CHANNEL) == DFSDM_InjectedChannel5) || \
                                          ((CHANNEL) == DFSDM_InjectedChannel6) || \
                                          ((CHANNEL) == DFSDM_InjectedChannel7))

#define DFSDM_RegularChannel0             ((uint32_t)0x00000000)
#define DFSDM_RegularChannel1             ((uint32_t)0x01000000)
#define DFSDM_RegularChannel2             ((uint32_t)0x02000000)
#define DFSDM_RegularChannel3             ((uint32_t)0x03000000)
#define DFSDM_RegularChannel4             ((uint32_t)0x04000000)
#define DFSDM_RegularChannel5             ((uint32_t)0x05000000)
#define DFSDM_RegularChannel6             ((uint32_t)0x06000000)
#define DFSDM_RegularChannel7             ((uint32_t)0x07000000)

#define IS_DFSDM_REGULAR_CHANNEL(CHANNEL) (((CHANNEL) == DFSDM_RegularChannel0) || \
                                           ((CHANNEL) == DFSDM_RegularChannel1) || \
                                           ((CHANNEL) == DFSDM_RegularChannel2) || \
                                           ((CHANNEL) == DFSDM_RegularChannel3) || \
                                           ((CHANNEL) == DFSDM_RegularChannel4) || \
                                           ((CHANNEL) == DFSDM_RegularChannel5) || \
                                           ((CHANNEL) == DFSDM_RegularChannel6) || \
                                           ((CHANNEL) == DFSDM_RegularChannel7))

#define DFSDM_Trigger_TIM1_TRGO          ((uint32_t)0x00000000)
#define DFSDM_Trigger_TIM1_TRGO2         ((uint32_t)0x00000100)
#define DFSDM_Trigger_TIM8_TRGO          ((uint32_t)0x00000200)
#define DFSDM_Trigger_TIM8_TRGO2         ((uint32_t)0x00000300)
#define DFSDM_Trigger_TIM3_TRGO          ((uint32_t)0x00000300)
#define DFSDM_Trigger_TIM4_TRGO          ((uint32_t)0x00000400)
#define DFSDM_Trigger_TIM16_OC1          ((uint32_t)0x00000400)
#define DFSDM_Trigger_TIM6_TRGO          ((uint32_t)0x00000500)
#define DFSDM_Trigger_TIM7_TRGO          ((uint32_t)0x00000500)
#define DFSDM_Trigger_EXTI11             ((uint32_t)0x00000600)
#define DFSDM_Trigger_EXTI15             ((uint32_t)0x00000700)

#define IS_DFSDM0_INJ_TRIGGER(TRIG)      (((TRIG) == DFSDM_Trigger_TIM1_TRGO)  || \
                                          ((TRIG) == DFSDM_Trigger_TIM1_TRGO2) || \
                                          ((TRIG) == DFSDM_Trigger_TIM8_TRGO)  || \
                                          ((TRIG) == DFSDM_Trigger_TIM8_TRGO2) || \
                                          ((TRIG) == DFSDM_Trigger_TIM4_TRGO)  || \
                                          ((TRIG) == DFSDM_Trigger_TIM6_TRGO)  || \
                                          ((TRIG) == DFSDM_Trigger_TIM7_TRGO)  || \
                                          ((TRIG) == DFSDM_Trigger_EXTI15)     || \
                                          ((TRIG) == DFSDM_Trigger_TIM3_TRGO)  || \
                                          ((TRIG) == DFSDM_Trigger_TIM16_OC1)  || \
                                          ((TRIG) == DFSDM_Trigger_EXTI11))

#define IS_DFSDM1_INJ_TRIGGER(TRIG)      IS_DFSDM0_INJ_TRIGGER(TRIG)

#define DFSDM_TriggerEdge_Disabled        ((uint32_t)0x00000000)
#define DFSDM_TriggerEdge_Rising          ((uint32_t)0x00002000)
#define DFSDM_TriggerEdge_Falling         ((uint32_t)0x00004000)
#define DFSDM_TriggerEdge_BothEdges       ((uint32_t)0x00006000)

#define IS_DFSDM_TRIGGER_EDGE(EDGE)      (((EDGE) == DFSDM_TriggerEdge_Disabled) || \
                                          ((EDGE) == DFSDM_TriggerEdge_Rising) || \
                                          ((EDGE) == DFSDM_TriggerEdge_Falling) || \
                                          ((EDGE) == DFSDM_TriggerEdge_BothEdges))

#define DFSDM_InjectConvMode_Single        ((uint32_t)0x00000000)
#define DFSDM_InjectConvMode_Scan          ((uint32_t)0x00000010)

#define IS_DFSDM_INJ_CONV_MODE(MODE)      (((MODE) == DFSDM_InjectConvMode_Single) || \
                                           ((MODE) == DFSDM_InjectConvMode_Scan))

#define DFSDM_IT_JEOC                            DFSDM_FLTCR2_JEOCIE
#define DFSDM_IT_REOC                            DFSDM_FLTCR2_REOCIE
#define DFSDM_IT_JOVR                            DFSDM_FLTCR2_JOVRIE
#define DFSDM_IT_ROVR                            DFSDM_FLTCR2_ROVRIE
#define DFSDM_IT_AWD                             DFSDM_FLTCR2_AWDIE
#define DFSDM_IT_SCD                             DFSDM_FLTCR2_SCDIE
#define DFSDM_IT_CKAB                            DFSDM_FLTCR2_CKABIE

#define IS_DFSDM_IT(IT)                         (((IT) == DFSDM_IT_JEOC) || \
                                                 ((IT) == DFSDM_IT_REOC) || \
                                                 ((IT) == DFSDM_IT_JOVR) || \
                                                 ((IT) == DFSDM_IT_ROVR) || \
                                                 ((IT) == DFSDM_IT_AWD)  || \
                                                 ((IT) == DFSDM_IT_SCD)  || \
                                                 ((IT) == DFSDM_IT_CKAB))

#define DFSDM_FLAG_JEOC                          DFSDM_FLTISR_JEOCF
#define DFSDM_FLAG_REOC                          DFSDM_FLTISR_REOCF
#define DFSDM_FLAG_JOVR                          DFSDM_FLTISR_JOVRF
#define DFSDM_FLAG_ROVR                          DFSDM_FLTISR_ROVRF
#define DFSDM_FLAG_AWD                           DFSDM_FLTISR_AWDF
#define DFSDM_FLAG_JCIP                          DFSDM_FLTISR_JCIP
#define DFSDM_FLAG_RCIP                          DFSDM_FLTISR_RCIP

#define IS_DFSDM_FLAG(FLAG)                     (((FLAG) == DFSDM_FLAG_JEOC) || \
                                                 ((FLAG) == DFSDM_FLAG_REOC) || \
                                                 ((FLAG) == DFSDM_FLAG_JOVR) || \
                                                 ((FLAG) == DFSDM_FLAG_ROVR) || \
                                                 ((FLAG) == DFSDM_FLAG_AWD)  || \
                                                 ((FLAG) == DFSDM_FLAG_JCIP) || \
                                                 ((FLAG) == DFSDM_FLAG_RCIP))

#define DFSDM_FLAG_CLKAbsence_Channel0           ((uint32_t)0x00010000)
#define DFSDM_FLAG_CLKAbsence_Channel1           ((uint32_t)0x00020000)
#define DFSDM_FLAG_CLKAbsence_Channel2           ((uint32_t)0x00040000)
#define DFSDM_FLAG_CLKAbsence_Channel3           ((uint32_t)0x00080000)
#define DFSDM_FLAG_CLKAbsence_Channel4           ((uint32_t)0x00100000)
#define DFSDM_FLAG_CLKAbsence_Channel5           ((uint32_t)0x00200000)
#define DFSDM_FLAG_CLKAbsence_Channel6           ((uint32_t)0x00400000)
#define DFSDM_FLAG_CLKAbsence_Channel7           ((uint32_t)0x00800000)

#define IS_DFSDM_CLK_ABS_FLAG(FLAG)             (((FLAG) == DFSDM_FLAG_CLKAbsence_Channel0) || \
                                                 ((FLAG) == DFSDM_FLAG_CLKAbsence_Channel1) || \
                                                 ((FLAG) == DFSDM_FLAG_CLKAbsence_Channel2) || \
                                                 ((FLAG) == DFSDM_FLAG_CLKAbsence_Channel3) || \
                                                 ((FLAG) == DFSDM_FLAG_CLKAbsence_Channel4) || \
                                                 ((FLAG) == DFSDM_FLAG_CLKAbsence_Channel5) || \
                                                 ((FLAG) == DFSDM_FLAG_CLKAbsence_Channel6) || \
                                                 ((FLAG) == DFSDM_FLAG_CLKAbsence_Channel7))

#define DFSDM_FLAG_SCD_Channel0                  ((uint32_t)0x01000000)
#define DFSDM_FLAG_SCD_Channel1                  ((uint32_t)0x02000000)
#define DFSDM_FLAG_SCD_Channel2                  ((uint32_t)0x04000000)
#define DFSDM_FLAG_SCD_Channel3                  ((uint32_t)0x08000000)
#define DFSDM_FLAG_SCD_Channel4                  ((uint32_t)0x10000000)
#define DFSDM_FLAG_SCD_Channel5                  ((uint32_t)0x20000000)
#define DFSDM_FLAG_SCD_Channel6                  ((uint32_t)0x40000000)
#define DFSDM_FLAG_SCD_Channel7                  ((uint32_t)0x80000000)

#define IS_DFSDM_SCD_FLAG(FLAG)                 (((FLAG) == DFSDM_FLAG_SCD_Channel0) || \
                                                 ((FLAG) == DFSDM_FLAG_SCD_Channel1) || \
                                                 ((FLAG) == DFSDM_FLAG_SCD_Channel2) || \
                                                 ((FLAG) == DFSDM_FLAG_SCD_Channel3) || \
                                                 ((FLAG) == DFSDM_FLAG_SCD_Channel4) || \
                                                 ((FLAG) == DFSDM_FLAG_SCD_Channel5) || \
                                                 ((FLAG) == DFSDM_FLAG_SCD_Channel6) || \
                                                 ((FLAG) == DFSDM_FLAG_SCD_Channel7))

#define DFSDM_CLEARF_JOVR                          DFSDM_FLTICR_CLRJOVRF
#define DFSDM_CLEARF_ROVR                          DFSDM_FLTICR_CLRROVRF

#define IS_DFSDM_CLEAR_FLAG(FLAG)                (((FLAG) == DFSDM_CLEARF_JOVR) || \
                                                  ((FLAG) == DFSDM_CLEARF_ROVR))

#define DFSDM_CLEARF_CLKAbsence_Channel0           ((uint32_t)0x00010000)
#define DFSDM_CLEARF_CLKAbsence_Channel1           ((uint32_t)0x00020000)
#define DFSDM_CLEARF_CLKAbsence_Channel2           ((uint32_t)0x00040000)
#define DFSDM_CLEARF_CLKAbsence_Channel3           ((uint32_t)0x00080000)
#define DFSDM_CLEARF_CLKAbsence_Channel4           ((uint32_t)0x00100000)
#define DFSDM_CLEARF_CLKAbsence_Channel5           ((uint32_t)0x00200000)
#define DFSDM_CLEARF_CLKAbsence_Channel6           ((uint32_t)0x00400000)
#define DFSDM_CLEARF_CLKAbsence_Channel7           ((uint32_t)0x00800000)

#define IS_DFSDM_CLK_ABS_CLEARF(FLAG)           (((FLAG) == DFSDM_CLEARF_CLKAbsence_Channel0) || \
                                                 ((FLAG) == DFSDM_CLEARF_CLKAbsence_Channel1) || \
                                                 ((FLAG) == DFSDM_CLEARF_CLKAbsence_Channel2) || \
                                                 ((FLAG) == DFSDM_CLEARF_CLKAbsence_Channel3) || \
                                                 ((FLAG) == DFSDM_CLEARF_CLKAbsence_Channel4) || \
                                                 ((FLAG) == DFSDM_CLEARF_CLKAbsence_Channel5) || \
                                                 ((FLAG) == DFSDM_CLEARF_CLKAbsence_Channel6) || \
                                                 ((FLAG) == DFSDM_CLEARF_CLKAbsence_Channel7))

#define DFSDM_CLEARF_SCD_Channel0           ((uint32_t)0x01000000)
#define DFSDM_CLEARF_SCD_Channel1           ((uint32_t)0x02000000)
#define DFSDM_CLEARF_SCD_Channel2           ((uint32_t)0x04000000)
#define DFSDM_CLEARF_SCD_Channel3           ((uint32_t)0x08000000)
#define DFSDM_CLEARF_SCD_Channel4           ((uint32_t)0x10000000)
#define DFSDM_CLEARF_SCD_Channel5           ((uint32_t)0x20000000)
#define DFSDM_CLEARF_SCD_Channel6           ((uint32_t)0x40000000)
#define DFSDM_CLEARF_SCD_Channel7           ((uint32_t)0x80000000)

#define IS_DFSDM_SCD_CHANNEL_FLAG(FLAG)         (((FLAG) == DFSDM_CLEARF_SCD_Channel0) || \
                                                 ((FLAG) == DFSDM_CLEARF_SCD_Channel1) || \
                                                 ((FLAG) == DFSDM_CLEARF_SCD_Channel2) || \
                                                 ((FLAG) == DFSDM_CLEARF_SCD_Channel3) || \
                                                 ((FLAG) == DFSDM_CLEARF_SCD_Channel4) || \
                                                 ((FLAG) == DFSDM_CLEARF_SCD_Channel5) || \
                                                 ((FLAG) == DFSDM_CLEARF_SCD_Channel6) || \
                                                 ((FLAG) == DFSDM_CLEARF_SCD_Channel7))

#define DFSDM_IT_CLKAbsence_Channel0           ((uint32_t)0x00010000)
#define DFSDM_IT_CLKAbsence_Channel1           ((uint32_t)0x00020000)
#define DFSDM_IT_CLKAbsence_Channel2           ((uint32_t)0x00040000)
#define DFSDM_IT_CLKAbsence_Channel3           ((uint32_t)0x00080000)
#define DFSDM_IT_CLKAbsence_Channel4           ((uint32_t)0x00100000)
#define DFSDM_IT_CLKAbsence_Channel5           ((uint32_t)0x00200000)
#define DFSDM_IT_CLKAbsence_Channel6           ((uint32_t)0x00400000)
#define DFSDM_IT_CLKAbsence_Channel7           ((uint32_t)0x00800000)

#define IS_DFSDM_CLK_ABS_IT(IT)               (((IT) == DFSDM_IT_CLKAbsence_Channel0) || \
                                               ((IT) == DFSDM_IT_CLKAbsence_Channel1) || \
                                               ((IT) == DFSDM_IT_CLKAbsence_Channel2) || \
                                               ((IT) == DFSDM_IT_CLKAbsence_Channel3) || \
                                               ((IT) == DFSDM_IT_CLKAbsence_Channel4) || \
                                               ((IT) == DFSDM_IT_CLKAbsence_Channel5) || \
                                               ((IT) == DFSDM_IT_CLKAbsence_Channel6) || \
                                               ((IT) == DFSDM_IT_CLKAbsence_Channel7))

#define DFSDM_IT_SCD_Channel0                  ((uint32_t)0x01000000)
#define DFSDM_IT_SCD_Channel1                  ((uint32_t)0x02000000)
#define DFSDM_IT_SCD_Channel2                  ((uint32_t)0x04000000)
#define DFSDM_IT_SCD_Channel3                  ((uint32_t)0x08000000)
#define DFSDM_IT_SCD_Channel4                  ((uint32_t)0x10000000)
#define DFSDM_IT_SCD_Channel5                  ((uint32_t)0x20000000)
#define DFSDM_IT_SCD_Channel6                  ((uint32_t)0x40000000)
#define DFSDM_IT_SCD_Channel7                  ((uint32_t)0x80000000)

#define IS_DFSDM_SCD_IT(IT)                   (((IT) == DFSDM_IT_SCD_Channel0) || \
                                               ((IT) == DFSDM_IT_SCD_Channel1) || \
                                               ((IT) == DFSDM_IT_SCD_Channel2) || \
                                               ((IT) == DFSDM_IT_SCD_Channel3) || \
                                               ((IT) == DFSDM_IT_SCD_Channel4) || \
                                               ((IT) == DFSDM_IT_SCD_Channel5) || \
                                               ((IT) == DFSDM_IT_SCD_Channel6) || \
                                               ((IT) == DFSDM_IT_SCD_Channel7))

#define IS_DFSDM_DATA_RIGHT_BIT_SHIFT(SHIFT)  ((SHIFT) < 0x20 )

#define IS_DFSDM_OFFSET(OFFSET)               ((OFFSET) < 0x01000000 )

#if defined(STM32F413_423xx)
#define IS_DFSDM_ALL_CHANNEL(CHANNEL)   (((CHANNEL) == DFSDM1_Channel0) || \
                                         ((CHANNEL) == DFSDM1_Channel1) || \
                                         ((CHANNEL) == DFSDM1_Channel2) || \
                                         ((CHANNEL) == DFSDM1_Channel3) || \
                                         ((CHANNEL) == DFSDM2_Channel0) || \
                                         ((CHANNEL) == DFSDM2_Channel1) || \
                                         ((CHANNEL) == DFSDM2_Channel2) || \
                                         ((CHANNEL) == DFSDM2_Channel3) || \
                                         ((CHANNEL) == DFSDM2_Channel4) || \
                                         ((CHANNEL) == DFSDM2_Channel5) || \
                                         ((CHANNEL) == DFSDM2_Channel6) || \
                                         ((CHANNEL) == DFSDM2_Channel7))

#define IS_DFSDM_ALL_FILTER(FILTER)     (((FILTER) == DFSDM1_0) || \
                                         ((FILTER) == DFSDM1_1) || \
                                         ((FILTER) == DFSDM2_0) || \
                                         ((FILTER) == DFSDM2_1) || \
                                         ((FILTER) == DFSDM2_2) || \
                                         ((FILTER) == DFSDM2_3))

#define IS_DFSDM_SYNC_FILTER(FILTER)    (((FILTER) == DFSDM1_0) || \
                                         ((FILTER) == DFSDM1_1) || \
                                         ((FILTER) == DFSDM2_0) || \
                                         ((FILTER) == DFSDM2_1) || \
                                         ((FILTER) == DFSDM2_2) || \
                                         ((FILTER) == DFSDM2_3))
#else
#define IS_DFSDM_ALL_CHANNEL(CHANNEL)   (((CHANNEL) == DFSDM1_Channel0) || \
                                         ((CHANNEL) == DFSDM1_Channel1) || \
                                         ((CHANNEL) == DFSDM1_Channel2) || \
                                         ((CHANNEL) == DFSDM1_Channel3))

#define IS_DFSDM_ALL_FILTER(FILTER)     (((FILTER) == DFSDM1_0) || \
                                         ((FILTER) == DFSDM1_1))

#define IS_DFSDM_SYNC_FILTER(FILTER)    (((FILTER) == DFSDM1_0) || \
                                         ((FILTER) == DFSDM1_1))
#endif

#define IS_DFSDM_SINC_OVRSMPL_RATIO(RATIO)  (((RATIO) < 0x401) && ((RATIO) >= 0x001))

#define IS_DFSDM_INTG_OVRSMPL_RATIO(RATIO)  (((RATIO) < 0x101 ) && ((RATIO) >= 0x001))

#define IS_DFSDM_CLOCK_OUT_DIVIDER(DIVIDER) ((DIVIDER) < 0x101 )

#define IS_DFSDM_CSD_THRESHOLD_VALUE(VALUE) ((VALUE) < 256)

#define IS_DFSDM_AWD_OVRSMPL_RATIO(RATIO)   ((RATIO) < 33) && ((RATIO) >= 0x001)

#define IS_DFSDM_HIGH_THRESHOLD(VALUE)      ((VALUE) < 0x1000000)
#define IS_DFSDM_LOW_THRESHOLD(VALUE)       ((VALUE) < 0x1000000)

void DFSDM_DeInit(void);
void DFSDM_TransceiverInit(DFSDM_Channel_TypeDef* DFSDM_Channelx, DFSDM_TransceiverInitTypeDef* DFSDM_TransceiverInitStruct);
void DFSDM_TransceiverStructInit(DFSDM_TransceiverInitTypeDef* DFSDM_TransceiverInitStruct);
void DFSDM_FilterInit(DFSDM_Filter_TypeDef* DFSDMx, DFSDM_FilterInitTypeDef* DFSDM_FilterInitStruct);
void DFSDM_FilterStructInit(DFSDM_FilterInitTypeDef* DFSDM_FilterInitStruct);

#if defined(STM32F412xG)
void DFSDM_Command(FunctionalState NewState);
#else
void DFSDM_Cmd(uint32_t Instance, FunctionalState NewState);
#endif
void DFSDM_ChannelCmd(DFSDM_Channel_TypeDef* DFSDM_Channelx, FunctionalState NewState);
void DFSDM_FilterCmd(DFSDM_Filter_TypeDef* DFSDMx, FunctionalState NewState);
#if defined(STM32F412xG)
void DFSDM_ConfigClkOutputDivider(uint32_t DFSDM_ClkOutDivision);
void DFSDM_ConfigClkOutputSource(uint32_t DFSDM_ClkOutSource);
#else
void DFSDM_ConfigClkOutputDivider(uint32_t Instance, uint32_t DFSDM_ClkOutDivision);
void DFSDM_ConfigClkOutputSource(uint32_t Instance, uint32_t DFSDM_ClkOutSource);
#endif
void DFSDM_SelectInjectedConversionMode(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_InjectConvMode);
void DFSDM_SelectInjectedChannel(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_InjectedChannelx);
void DFSDM_SelectRegularChannel(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_RegularChannelx);
void DFSDM_StartSoftwareInjectedConversion(DFSDM_Filter_TypeDef* DFSDMx);
void DFSDM_StartSoftwareRegularConversion(DFSDM_Filter_TypeDef* DFSDMx);
void DFSDM_SynchronousFilter0InjectedStart(DFSDM_Filter_TypeDef* DFSDMx);
void DFSDM_SynchronousFilter0RegularStart(DFSDM_Filter_TypeDef* DFSDMx);
void DFSDM_RegularContinuousModeCmd(DFSDM_Filter_TypeDef* DFSDMx, FunctionalState NewState);
void DFSDM_InjectedContinuousModeCmd(DFSDM_Filter_TypeDef* DFSDMx, FunctionalState NewState);
void DFSDM_FastModeCmd(DFSDM_Filter_TypeDef* DFSDMx, FunctionalState NewState);
void DFSDM_ConfigInjectedTrigger(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_Trigger, uint32_t DFSDM_TriggerEdge);
void DFSDM_ConfigBRKShortCircuitDetector(DFSDM_Channel_TypeDef* DFSDM_Channelx, uint32_t DFSDM_SCDBreak_i, FunctionalState NewState);
void DFSDM_ConfigBRKAnalogWatchDog(DFSDM_Channel_TypeDef* DFSDM_Channelx, uint32_t DFSDM_SCDBreak_i, FunctionalState NewState);
void DFSDM_ConfigShortCircuitThreshold(DFSDM_Channel_TypeDef* DFSDM_Channelx, uint32_t DFSDM_SCDThreshold);
void DFSDM_ConfigAnalogWatchdog(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_AWDChannelx, uint32_t DFSDM_AWDFastMode);
void DFSDM_ConfigAWDFilter(DFSDM_Channel_TypeDef* DFSDM_Channelx, uint32_t DFSDM_AWDSincOrder, uint32_t DFSDM_AWDSincOverSampleRatio);
uint32_t DFSDM_GetAWDConversionValue(DFSDM_Channel_TypeDef* DFSDM_Channelx);
void DFSDM_SetAWDThreshold(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_HighThreshold, uint32_t DFSDM_LowThreshold);
void DFSDM_SelectExtremesDetectorChannel(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_ExtremChannelx);
int32_t DFSDM_GetRegularConversionData(DFSDM_Filter_TypeDef* DFSDMx);
int32_t DFSDM_GetInjectedConversionData(DFSDM_Filter_TypeDef* DFSDMx);
int32_t DFSDM_GetMaxValue(DFSDM_Filter_TypeDef* DFSDMx);
int32_t DFSDM_GetMinValue(DFSDM_Filter_TypeDef* DFSDMx);
int32_t DFSDM_GetMaxValueChannel(DFSDM_Filter_TypeDef* DFSDMx);
int32_t DFSDM_GetMinValueChannel(DFSDM_Filter_TypeDef* DFSDMx);
uint32_t DFSDM_GetConversionTime(DFSDM_Filter_TypeDef* DFSDMx);
void DFSDM_DMATransferConfig(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_DMAConversionMode, FunctionalState NewState);

void DFSDM_ITConfig(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_IT, FunctionalState NewState);
#if defined(STM32F412xG)
void DFSDM_ITClockAbsenceCmd(FunctionalState NewState);
void DFSDM_ITShortCircuitDetectorCmd(FunctionalState NewState);
#else
void DFSDM_ITClockAbsenceCmd(uint32_t Instance, FunctionalState NewState);
void DFSDM_ITShortCircuitDetectorCmd(uint32_t Instance, FunctionalState NewState);
#endif

FlagStatus DFSDM_GetFlagStatus(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_FLAG);
#if defined(STM32F412xG)
FlagStatus DFSDM_GetClockAbsenceFlagStatus(uint32_t DFSDM_FLAG_CLKAbsence);
FlagStatus DFSDM_GetShortCircuitFlagStatus(uint32_t DFSDM_FLAG_SCD);
#else
FlagStatus DFSDM_GetClockAbsenceFlagStatus(uint32_t Instance, uint32_t DFSDM_FLAG_CLKAbsence);
FlagStatus DFSDM_GetShortCircuitFlagStatus(uint32_t Instance, uint32_t DFSDM_FLAG_SCD);
#endif
FlagStatus DFSDM_GetWatchdogFlagStatus(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_AWDChannelx, uint8_t DFSDM_Threshold);

void DFSDM_ClearFlag(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_CLEARF);
#if defined(STM32F412xG)
void DFSDM_ClearClockAbsenceFlag(uint32_t DFSDM_CLEARF_CLKAbsence);
void DFSDM_ClearShortCircuitFlag(uint32_t DFSDM_CLEARF_SCD);
#else
void DFSDM_ClearClockAbsenceFlag(uint32_t Instance, uint32_t DFSDM_CLEARF_CLKAbsence);
void DFSDM_ClearShortCircuitFlag(uint32_t Instance, uint32_t DFSDM_CLEARF_SCD);
#endif
void DFSDM_ClearAnalogWatchdogFlag(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_AWDChannelx, uint8_t DFSDM_Threshold);

ITStatus DFSDM_GetITStatus(DFSDM_Filter_TypeDef* DFSDMx, uint32_t DFSDM_IT);
#if defined(STM32F412xG)
ITStatus DFSDM_GetClockAbsenceITStatus(uint32_t DFSDM_IT_CLKAbsence);
ITStatus DFSDM_GetShortCircuitITStatus(uint32_t DFSDM_IT_SCR);
#else
ITStatus DFSDM_GetClockAbsenceITStatus(uint32_t Instance, uint32_t DFSDM_IT_CLKAbsence);
ITStatus DFSDM_GetShortCircuitITStatus(uint32_t Instance, uint32_t DFSDM_IT_SCR);
#endif

#endif

#ifdef __cplusplus
}
#endif

#endif
