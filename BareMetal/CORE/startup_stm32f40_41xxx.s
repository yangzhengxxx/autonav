Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp

Heap_Size       EQU     0x00000200

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp
                DCD     Reset_Handler
                DCD     NMI_Handler
                DCD     HardFault_Handler
                DCD     MemManage_Handler
                DCD     BusFault_Handler
                DCD     UsageFault_Handler
                DCD     0
                DCD     0
                DCD     0
                DCD     0
                DCD     SVC_Handler
                DCD     DebugMon_Handler
                DCD     0
                DCD     PendSV_Handler
                DCD     SysTick_Handler

                DCD     WWDG_IRQHandler
                DCD     PVD_IRQHandler
                DCD     TAMP_STAMP_IRQHandler
                DCD     RTC_WKUP_IRQHandler
                DCD     FLASH_IRQHandler
                DCD     RCC_IRQHandler
                DCD     EXTI0_IRQHandler
                DCD     EXTI1_IRQHandler
                DCD     EXTI2_IRQHandler
                DCD     EXTI3_IRQHandler
                DCD     EXTI4_IRQHandler
                DCD     DMA1_Stream0_IRQHandler
                DCD     DMA1_Stream1_IRQHandler
                DCD     DMA1_Stream2_IRQHandler
                DCD     DMA1_Stream3_IRQHandler
                DCD     DMA1_Stream4_IRQHandler
                DCD     DMA1_Stream5_IRQHandler
                DCD     DMA1_Stream6_IRQHandler
                DCD     ADC_IRQHandler
                DCD     CAN1_TX_IRQHandler
                DCD     CAN1_RX0_IRQHandler
                DCD     CAN1_RX1_IRQHandler
                DCD     CAN1_SCE_IRQHandler
                DCD     EXTI9_5_IRQHandler
                DCD     TIM1_BRK_TIM9_IRQHandler
                DCD     TIM1_UP_TIM10_IRQHandler
                DCD     TIM1_TRG_COM_TIM11_IRQHandler
                DCD     TIM1_CC_IRQHandler
                DCD     TIM2_IRQHandler
                DCD     TIM3_IRQHandler
                DCD     TIM4_IRQHandler
                DCD     I2C1_EV_IRQHandler
                DCD     I2C1_ER_IRQHandler
                DCD     I2C2_EV_IRQHandler
                DCD     I2C2_ER_IRQHandler
                DCD     SPI1_IRQHandler
                DCD     SPI2_IRQHandler
                DCD     USART1_IRQHandler
                DCD     USART2_IRQHandler
                DCD     USART3_IRQHandler
                DCD     EXTI15_10_IRQHandler
                DCD     RTC_Alarm_IRQHandler
                DCD     OTG_FS_WKUP_IRQHandler
                DCD     TIM8_BRK_TIM12_IRQHandler
                DCD     TIM8_UP_TIM13_IRQHandler
                DCD     TIM8_TRG_COM_TIM14_IRQHandler
                DCD     TIM8_CC_IRQHandler
                DCD     DMA1_Stream7_IRQHandler
                DCD     FSMC_IRQHandler
                DCD     SDIO_IRQHandler
                DCD     TIM5_IRQHandler
                DCD     SPI3_IRQHandler
                DCD     UART4_IRQHandler
                DCD     UART5_IRQHandler
                DCD     TIM6_DAC_IRQHandler
                DCD     TIM7_IRQHandler
                DCD     DMA2_Stream0_IRQHandler
                DCD     DMA2_Stream1_IRQHandler
                DCD     DMA2_Stream2_IRQHandler
                DCD     DMA2_Stream3_IRQHandler
                DCD     DMA2_Stream4_IRQHandler
                DCD     ETH_IRQHandler
                DCD     ETH_WKUP_IRQHandler
                DCD     CAN2_TX_IRQHandler
                DCD     CAN2_RX0_IRQHandler
                DCD     CAN2_RX1_IRQHandler
                DCD     CAN2_SCE_IRQHandler
                DCD     OTG_FS_IRQHandler
                DCD     DMA2_Stream5_IRQHandler
                DCD     DMA2_Stream6_IRQHandler
                DCD     DMA2_Stream7_IRQHandler
                DCD     USART6_IRQHandler
                DCD     I2C3_EV_IRQHandler
                DCD     I2C3_ER_IRQHandler
                DCD     OTG_HS_EP1_OUT_IRQHandler
                DCD     OTG_HS_EP1_IN_IRQHandler
                DCD     OTG_HS_WKUP_IRQHandler
                DCD     OTG_HS_IRQHandler
                DCD     DCMI_IRQHandler
                DCD     CRYP_IRQHandler
                DCD     HASH_RNG_IRQHandler
                DCD     FPU_IRQHandler

__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

Reset_Handler    PROC
                 EXPORT  Reset_Handler             [WEAK]
        IMPORT  SystemInit
        IMPORT  __main

                 LDR     R0, =SystemInit
                 BLX     R0
                 LDR     R0, =__main
                 BX      R0
                 ENDP

NMI_Handler     PROC
                EXPORT  NMI_Handler                [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler          [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler          [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler           [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler           [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler             [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler            [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  WWDG_IRQHandler                   [WEAK]
                EXPORT  PVD_IRQHandler                    [WEAK]
                EXPORT  TAMP_STAMP_IRQHandler             [WEAK]
                EXPORT  RTC_WKUP_IRQHandler               [WEAK]
                EXPORT  FLASH_IRQHandler                  [WEAK]
                EXPORT  RCC_IRQHandler                    [WEAK]
                EXPORT  EXTI0_IRQHandler                  [WEAK]
                EXPORT  EXTI1_IRQHandler                  [WEAK]
                EXPORT  EXTI2_IRQHandler                  [WEAK]
                EXPORT  EXTI3_IRQHandler                  [WEAK]
                EXPORT  EXTI4_IRQHandler                  [WEAK]
                EXPORT  DMA1_Stream0_IRQHandler           [WEAK]
                EXPORT  DMA1_Stream1_IRQHandler           [WEAK]
                EXPORT  DMA1_Stream2_IRQHandler           [WEAK]
                EXPORT  DMA1_Stream3_IRQHandler           [WEAK]
                EXPORT  DMA1_Stream4_IRQHandler           [WEAK]
                EXPORT  DMA1_Stream5_IRQHandler           [WEAK]
                EXPORT  DMA1_Stream6_IRQHandler           [WEAK]
                EXPORT  ADC_IRQHandler                    [WEAK]
                EXPORT  CAN1_TX_IRQHandler                [WEAK]
                EXPORT  CAN1_RX0_IRQHandler               [WEAK]
                EXPORT  CAN1_RX1_IRQHandler               [WEAK]
                EXPORT  CAN1_SCE_IRQHandler               [WEAK]
                EXPORT  EXTI9_5_IRQHandler                [WEAK]
                EXPORT  TIM1_BRK_TIM9_IRQHandler          [WEAK]
                EXPORT  TIM1_UP_TIM10_IRQHandler          [WEAK]
                EXPORT  TIM1_TRG_COM_TIM11_IRQHandler     [WEAK]
                EXPORT  TIM1_CC_IRQHandler                [WEAK]
                EXPORT  TIM2_IRQHandler                   [WEAK]
                EXPORT  TIM3_IRQHandler                   [WEAK]
                EXPORT  TIM4_IRQHandler                   [WEAK]
                EXPORT  I2C1_EV_IRQHandler                [WEAK]
                EXPORT  I2C1_ER_IRQHandler                [WEAK]
                EXPORT  I2C2_EV_IRQHandler                [WEAK]
                EXPORT  I2C2_ER_IRQHandler                [WEAK]
                EXPORT  SPI1_IRQHandler                   [WEAK]
                EXPORT  SPI2_IRQHandler                   [WEAK]
                EXPORT  USART1_IRQHandler                 [WEAK]
                EXPORT  USART2_IRQHandler                 [WEAK]
                EXPORT  USART3_IRQHandler                 [WEAK]
                EXPORT  EXTI15_10_IRQHandler              [WEAK]
                EXPORT  RTC_Alarm_IRQHandler              [WEAK]
                EXPORT  OTG_FS_WKUP_IRQHandler            [WEAK]
                EXPORT  TIM8_BRK_TIM12_IRQHandler         [WEAK]
                EXPORT  TIM8_UP_TIM13_IRQHandler          [WEAK]
                EXPORT  TIM8_TRG_COM_TIM14_IRQHandler     [WEAK]
                EXPORT  TIM8_CC_IRQHandler                [WEAK]
                EXPORT  DMA1_Stream7_IRQHandler           [WEAK]
                EXPORT  FSMC_IRQHandler                   [WEAK]
                EXPORT  SDIO_IRQHandler                   [WEAK]
                EXPORT  TIM5_IRQHandler                   [WEAK]
                EXPORT  SPI3_IRQHandler                   [WEAK]
                EXPORT  UART4_IRQHandler                  [WEAK]
                EXPORT  UART5_IRQHandler                  [WEAK]
                EXPORT  TIM6_DAC_IRQHandler               [WEAK]
                EXPORT  TIM7_IRQHandler                   [WEAK]
                EXPORT  DMA2_Stream0_IRQHandler           [WEAK]
                EXPORT  DMA2_Stream1_IRQHandler           [WEAK]
                EXPORT  DMA2_Stream2_IRQHandler           [WEAK]
                EXPORT  DMA2_Stream3_IRQHandler           [WEAK]
                EXPORT  DMA2_Stream4_IRQHandler           [WEAK]
                EXPORT  ETH_IRQHandler                    [WEAK]
                EXPORT  ETH_WKUP_IRQHandler               [WEAK]
                EXPORT  CAN2_TX_IRQHandler                [WEAK]
                EXPORT  CAN2_RX0_IRQHandler               [WEAK]
                EXPORT  CAN2_RX1_IRQHandler               [WEAK]
                EXPORT  CAN2_SCE_IRQHandler               [WEAK]
                EXPORT  OTG_FS_IRQHandler                 [WEAK]
                EXPORT  DMA2_Stream5_IRQHandler           [WEAK]
                EXPORT  DMA2_Stream6_IRQHandler           [WEAK]
                EXPORT  DMA2_Stream7_IRQHandler           [WEAK]
                EXPORT  USART6_IRQHandler                 [WEAK]
                EXPORT  I2C3_EV_IRQHandler                [WEAK]
                EXPORT  I2C3_ER_IRQHandler                [WEAK]
                EXPORT  OTG_HS_EP1_OUT_IRQHandler         [WEAK]
                EXPORT  OTG_HS_EP1_IN_IRQHandler          [WEAK]
                EXPORT  OTG_HS_WKUP_IRQHandler            [WEAK]
                EXPORT  OTG_HS_IRQHandler                 [WEAK]
                EXPORT  DCMI_IRQHandler                   [WEAK]
                EXPORT  CRYP_IRQHandler                   [WEAK]
                EXPORT  HASH_RNG_IRQHandler               [WEAK]
                EXPORT  FPU_IRQHandler                    [WEAK]

WWDG_IRQHandler
PVD_IRQHandler
TAMP_STAMP_IRQHandler
RTC_WKUP_IRQHandler
FLASH_IRQHandler
RCC_IRQHandler
EXTI0_IRQHandler
EXTI1_IRQHandler
EXTI2_IRQHandler
EXTI3_IRQHandler
EXTI4_IRQHandler
DMA1_Stream0_IRQHandler
DMA1_Stream1_IRQHandler
DMA1_Stream2_IRQHandler
DMA1_Stream3_IRQHandler
DMA1_Stream4_IRQHandler
DMA1_Stream5_IRQHandler
DMA1_Stream6_IRQHandler
ADC_IRQHandler
CAN1_TX_IRQHandler
CAN1_RX0_IRQHandler
CAN1_RX1_IRQHandler
CAN1_SCE_IRQHandler
EXTI9_5_IRQHandler
TIM1_BRK_TIM9_IRQHandler
TIM1_UP_TIM10_IRQHandler
TIM1_TRG_COM_TIM11_IRQHandler
TIM1_CC_IRQHandler
TIM2_IRQHandler
TIM3_IRQHandler
TIM4_IRQHandler
I2C1_EV_IRQHandler
I2C1_ER_IRQHandler
I2C2_EV_IRQHandler
I2C2_ER_IRQHandler
SPI1_IRQHandler
SPI2_IRQHandler
USART1_IRQHandler
USART2_IRQHandler
USART3_IRQHandler
EXTI15_10_IRQHandler
RTC_Alarm_IRQHandler
OTG_FS_WKUP_IRQHandler
TIM8_BRK_TIM12_IRQHandler
TIM8_UP_TIM13_IRQHandler
TIM8_TRG_COM_TIM14_IRQHandler
TIM8_CC_IRQHandler
DMA1_Stream7_IRQHandler
FSMC_IRQHandler
SDIO_IRQHandler
TIM5_IRQHandler
SPI3_IRQHandler
UART4_IRQHandler
UART5_IRQHandler
TIM6_DAC_IRQHandler
TIM7_IRQHandler
DMA2_Stream0_IRQHandler
DMA2_Stream1_IRQHandler
DMA2_Stream2_IRQHandler
DMA2_Stream3_IRQHandler
DMA2_Stream4_IRQHandler
ETH_IRQHandler
ETH_WKUP_IRQHandler
CAN2_TX_IRQHandler
CAN2_RX0_IRQHandler
CAN2_RX1_IRQHandler
CAN2_SCE_IRQHandler
OTG_FS_IRQHandler
DMA2_Stream5_IRQHandler
DMA2_Stream6_IRQHandler
DMA2_Stream7_IRQHandler
USART6_IRQHandler
I2C3_EV_IRQHandler
I2C3_ER_IRQHandler
OTG_HS_EP1_OUT_IRQHandler
OTG_HS_EP1_IN_IRQHandler
OTG_HS_WKUP_IRQHandler
OTG_HS_IRQHandler
DCMI_IRQHandler
CRYP_IRQHandler
HASH_RNG_IRQHandler
FPU_IRQHandler

                B       .

                ENDP

                ALIGN

                 IF      :DEF:__MICROLIB

                 EXPORT  __initial_sp
                 EXPORT  __heap_base
                 EXPORT  __heap_limit

                 ELSE

                 IMPORT  __use_two_region_memory
                 EXPORT  __user_initial_stackheap

__user_initial_stackheap

                 LDR     R0, =  Heap_Mem
                 LDR     R1, =(Stack_Mem + Stack_Size)
                 LDR     R2, = (Heap_Mem +  Heap_Size)
                 LDR     R3, = Stack_Mem
                 BX      LR

                 ALIGN

                 ENDIF

                 END
