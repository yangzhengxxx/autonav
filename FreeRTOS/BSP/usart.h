#ifndef __USART_H
#define __USART_H

#include "stm32f4xx.h"

#include <stdio.h>

#define DEBUG_USART       USART1
#define DEBUG_USART_BAUD  115200U

#define BT_USART          USART2
#define BT_USART_BAUD     9600U

void usart_init(void);
void bt_usart_init(void);
void usart_send_byte(uint8_t data);
void usart_send_string(const char *str);

uint8_t usart_data_available(void);

uint8_t usart_read_byte(void);

uint8_t bt_usart_data_available(void);

uint8_t bt_usart_read_byte(void);

#endif
