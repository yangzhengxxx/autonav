#include "timer.h"
#include "misc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "system_stm32f4xx.h"

volatile uint32_t g_tim6_ms = 0U;

static void delay_dwt_init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0U;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void delay_ms(uint32_t ms)
{
    static uint8_t inited = 0U;
    uint32_t start;
    uint32_t ticks;

    if (inited == 0U)
    {
        SystemCoreClockUpdate();
        delay_dwt_init();
        inited = 1U;
    }

    if (ms == 0U)
    {
        return;
    }

    ticks = ms * (SystemCoreClock / 1000U);
    start = DWT->CYCCNT;
    while ((DWT->CYCCNT - start) < ticks)
    {

    }
}

void TIM1_PWM_Init(void)
{
    GPIO_InitTypeDef gpio_init;
    TIM_TimeBaseInitTypeDef time_base;
    TIM_OCInitTypeDef output_compare;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);

    GPIO_StructInit(&gpio_init);
    gpio_init.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 |
                         GPIO_Pin_13 | GPIO_Pin_14;
    gpio_init.GPIO_Mode = GPIO_Mode_AF;
    gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOE, &gpio_init);

    TIM_TimeBaseStructInit(&time_base);
    time_base.TIM_Prescaler = 0U;
    time_base.TIM_CounterMode = TIM_CounterMode_Up;
    time_base.TIM_Period = TIM1_PWM_PERIOD;
    time_base.TIM_ClockDivision = TIM_CKD_DIV1;
    time_base.TIM_RepetitionCounter = 0U;
    TIM_TimeBaseInit(TIM1, &time_base);

    TIM_OCStructInit(&output_compare);
    output_compare.TIM_OCMode = TIM_OCMode_PWM1;
    output_compare.TIM_OutputState = TIM_OutputState_Enable;
    output_compare.TIM_Pulse = 0U;
    output_compare.TIM_OCPolarity = TIM_OCPolarity_High;
    output_compare.TIM_OCIdleState = TIM_OCIdleState_Reset;

    TIM_OC1Init(TIM1, &output_compare);
    TIM_OC2Init(TIM1, &output_compare);
    TIM_OC3Init(TIM1, &output_compare);
    TIM_OC4Init(TIM1, &output_compare);

    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM1, ENABLE);

    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_Cmd(TIM1, ENABLE);
}

void TIM1_PWM_SetCompare(uint8_t channel, uint16_t compare)
{
    if (compare > TIM1_PWM_PERIOD)
    {
        compare = TIM1_PWM_PERIOD;
    }

    switch (channel)
    {
    case 1U:
        TIM_SetCompare1(TIM1, compare);
        break;
    case 2U:
        TIM_SetCompare2(TIM1, compare);
        break;
    case 3U:
        TIM_SetCompare3(TIM1, compare);
        break;
    case 4U:
        TIM_SetCompare4(TIM1, compare);
        break;
    default:
        break;
    }
}

static void TIM_EncoderBaseInit32(TIM_TypeDef *tim_x, uint32_t period)
{
    TIM_TimeBaseInitTypeDef time_base;
    TIM_ICInitTypeDef input_capture;

    TIM_TimeBaseStructInit(&time_base);
    time_base.TIM_Prescaler = 0U;
    time_base.TIM_CounterMode = TIM_CounterMode_Up;
    time_base.TIM_Period = period;
    time_base.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(tim_x, &time_base);

    TIM_EncoderInterfaceConfig(tim_x,
                               TIM_EncoderMode_TI12,
                               TIM_ICPolarity_Rising,
                               TIM_ICPolarity_Rising);

    TIM_ICStructInit(&input_capture);
    input_capture.TIM_ICPolarity = TIM_ICPolarity_Rising;
    input_capture.TIM_ICSelection = TIM_ICSelection_DirectTI;
    input_capture.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    input_capture.TIM_ICFilter = 6U;

    input_capture.TIM_Channel = TIM_Channel_1;
    TIM_ICInit(tim_x, &input_capture);
    input_capture.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(tim_x, &input_capture);

    TIM_SetCounter(tim_x, 0U);
    TIM_Cmd(tim_x, ENABLE);
}

static void TIM_EncoderBaseInit16(TIM_TypeDef *tim_x, uint16_t period)
{
    TIM_TimeBaseInitTypeDef time_base;
    TIM_ICInitTypeDef input_capture;

    TIM_TimeBaseStructInit(&time_base);
    time_base.TIM_Prescaler = 0U;
    time_base.TIM_CounterMode = TIM_CounterMode_Up;
    time_base.TIM_Period = period;
    time_base.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(tim_x, &time_base);

    TIM_EncoderInterfaceConfig(tim_x,
                               TIM_EncoderMode_TI12,
                               TIM_ICPolarity_Rising,
                               TIM_ICPolarity_Rising);

    TIM_ICStructInit(&input_capture);
    input_capture.TIM_ICPolarity = TIM_ICPolarity_Rising;
    input_capture.TIM_ICSelection = TIM_ICSelection_DirectTI;
    input_capture.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    input_capture.TIM_ICFilter = 6U;

    input_capture.TIM_Channel = TIM_Channel_1;
    TIM_ICInit(tim_x, &input_capture);
    input_capture.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(tim_x, &input_capture);

    TIM_SetCounter(tim_x, 0U);
    TIM_Cmd(tim_x, ENABLE);
}

void TIM2_Encoder_Init(void)
{
    GPIO_InitTypeDef gpio_init;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA |
                               RCC_AHB1Periph_GPIOB,
                           ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);

    GPIO_StructInit(&gpio_init);
    gpio_init.GPIO_Mode = GPIO_Mode_AF;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd = GPIO_PuPd_UP;

    gpio_init.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOA, &gpio_init);
    gpio_init.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOB, &gpio_init);

    TIM_EncoderBaseInit32(TIM2, 0xFFFFFFFFU);
}

void TIM3_Encoder_Init(void)
{
    GPIO_InitTypeDef gpio_init;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);

    GPIO_StructInit(&gpio_init);
    gpio_init.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    gpio_init.GPIO_Mode = GPIO_Mode_AF;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &gpio_init);

    TIM_EncoderBaseInit16(TIM3, 0xFFFFU);
}

void TIM4_Encoder_Init(void)
{
    GPIO_InitTypeDef gpio_init;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);

    GPIO_StructInit(&gpio_init);
    gpio_init.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    gpio_init.GPIO_Mode = GPIO_Mode_AF;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &gpio_init);

    TIM_EncoderBaseInit16(TIM4, 0xFFFFU);
}

void TIM5_Encoder_Init(void)
{
    GPIO_InitTypeDef gpio_init;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);

    GPIO_StructInit(&gpio_init);
    gpio_init.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    gpio_init.GPIO_Mode = GPIO_Mode_AF;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &gpio_init);

    TIM_EncoderBaseInit32(TIM5, 0xFFFFFFFFU);
}

void Encoder_TIM_Init(void)
{
    TIM2_Encoder_Init();
    TIM3_Encoder_Init();
    TIM4_Encoder_Init();
    TIM5_Encoder_Init();
}

int16_t Encoder_Get(TIM_TypeDef *TIM)
{
    int16_t Temp;
    Temp = TIM_GetCounter(TIM);
    TIM_SetCounter(TIM, 0);
    return Temp;
}

volatile int32_t g_enc_delta[MOTOR_NUM] = {0, 0, 0, 0};
volatile int32_t g_enc_total[MOTOR_NUM] = {0, 0, 0, 0};

int32_t Encoder_GetDelta(uint8_t motor)
{

    TIM_TypeDef *const enc_tim[4] = {
        M1_ENCODER_TIM, M2_ENCODER_TIM, M3_ENCODER_TIM, M4_ENCODER_TIM};

    static const int8_t enc_sign[4] = {
        M1_ENC_SIGN, M2_ENC_SIGN, M3_ENC_SIGN, M4_ENC_SIGN};

    if (motor >= 4U)
    {
        return 0;
    }

    return (int32_t)enc_sign[motor] * (int32_t)Encoder_Get(enc_tim[motor]);
}

void ReadAllEncoder(void)
{
    uint8_t i;

    for (i = 0U; i < MOTOR_NUM; i++)
    {
        int32_t delta = Encoder_GetDelta(i);

        g_enc_delta[i] = delta;
        g_enc_total[i] += delta;
    }
}

static uint32_t TIM6_GetClockHz(void)
{
    RCC_ClocksTypeDef clocks;

    RCC_GetClocksFreq(&clocks);

    if ((RCC->CFGR & RCC_CFGR_PPRE1) == RCC_CFGR_PPRE1_DIV1)
    {
        return clocks.PCLK1_Frequency;
    }

    return clocks.PCLK1_Frequency * 2U;
}

void TIM6_1ms_Init(void)
{
    TIM_TimeBaseInitTypeDef time_base;
    NVIC_InitTypeDef nvic_init;
    uint32_t tim6_clock;
    uint32_t prescaler = 8399U;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

    SystemCoreClockUpdate();
    tim6_clock = TIM6_GetClockHz();

    TIM_TimeBaseStructInit(&time_base);
    time_base.TIM_Prescaler = prescaler;
    time_base.TIM_CounterMode = TIM_CounterMode_Up;

    time_base.TIM_Period = (tim6_clock / ((prescaler + 1U) * 1000U)) - 1U;
    time_base.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM6, &time_base);

    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

    nvic_init.NVIC_IRQChannel = TIM6_DAC_IRQn;
    nvic_init.NVIC_IRQChannelPreemptionPriority = 2U;
    nvic_init.NVIC_IRQChannelSubPriority = 0U;
    nvic_init.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_init);

    TIM_Cmd(TIM6, ENABLE);
}

void BSP_TIM_Init(void)
{
    TIM1_PWM_Init();
    Encoder_TIM_Init();
    TIM6_1ms_Init();
}

#if defined(__GNUC__)
#define AUTONAV_WEAK __attribute__((weak))
#else
#define AUTONAV_WEAK __weak
#endif

AUTONAV_WEAK void PID_Callback(void)
{

}

void TIM6_DAC_IRQHandler(void)
{
    static uint32_t ctrl_div = 0U;

    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
        g_tim6_ms += TIM6_INTERRUPT_PERIOD_MS;

        ctrl_div += TIM6_INTERRUPT_PERIOD_MS;
        if (ctrl_div >= CTRL_PERIOD_MS)
        {
            ctrl_div = 0U;
            ReadAllEncoder();
            PID_Callback();
        }
    }
}
