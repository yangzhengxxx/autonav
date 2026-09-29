#include "usart.h"

#include "board_config.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"

void usart_init(void)
{
    GPIO_InitTypeDef gpio_init;
    USART_InitTypeDef usart_init;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    GPIO_StructInit(&gpio_init);
    gpio_init.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    gpio_init.GPIO_Mode = GPIO_Mode_AF;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &gpio_init);

    USART_DeInit(DEBUG_USART);
    USART_StructInit(&usart_init);
    usart_init.USART_BaudRate = DEBUG_USART_BAUD;
    usart_init.USART_WordLength = USART_WordLength_8b;
    usart_init.USART_StopBits = USART_StopBits_1;
    usart_init.USART_Parity = USART_Parity_No;
    usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(DEBUG_USART, &usart_init);

    USART_Cmd(DEBUG_USART, ENABLE);
}

void bt_usart_init(void)
{
    GPIO_InitTypeDef gpio_init;
    USART_InitTypeDef usart_init;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

    GPIO_StructInit(&gpio_init);
    gpio_init.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    gpio_init.GPIO_Mode = GPIO_Mode_AF;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &gpio_init);

    USART_DeInit(BT_USART);
    USART_StructInit(&usart_init);
    usart_init.USART_BaudRate = BT_USART_BAUD;
    usart_init.USART_WordLength = USART_WordLength_8b;
    usart_init.USART_StopBits = USART_StopBits_1;
    usart_init.USART_Parity = USART_Parity_No;
    usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(BT_USART, &usart_init);

    USART_Cmd(BT_USART, ENABLE);
}

uint8_t bt_usart_data_available(void)
{
    return (USART_GetFlagStatus(BT_USART, USART_FLAG_RXNE) != RESET) ? 1U : 0U;
}

uint8_t bt_usart_read_byte(void)
{
    return (uint8_t)USART_ReceiveData(BT_USART);
}

void usart_send_byte(uint8_t data)
{
    while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET)
    {
    }
    USART_SendData(DEBUG_USART, data);
}

void usart_send_string(const char *str)
{
    while (*str != '\0')
    {
        usart_send_byte((uint8_t)*str);
        str++;
    }
}

uint8_t usart_data_available(void)
{
    return (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) != RESET) ? 1U : 0U;
}

uint8_t usart_read_byte(void)
{
    return (uint8_t)USART_ReceiveData(DEBUG_USART);
}

#if defined(__CC_ARM) && !defined(__MICROLIB)
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
};

FILE __stdout;

void _sys_exit(int x)
{
    (void)x;
    while (1)
    {
    }
}

char *_sys_command_string(char *cmd, int len)
{
    (void)len;
    return cmd;
}
#endif

int fputc(int ch, FILE *f)
{
    (void)f;
#if (USE_DEBUG_UART != 0)
    usart_send_byte((uint8_t)ch);
#else

    (void)ch;
#endif
    return ch;
}

#if defined(__GNUC__)
int autonav_platform_write(const char *data, int length)
{
    int i;

#if (USE_DEBUG_UART != 0)
    for (i = 0; i < length; ++i)
    {
        usart_send_byte((uint8_t)data[i]);
    }
#else
    (void)data;
    (void)i;
#endif
    return length;
}
#endif
