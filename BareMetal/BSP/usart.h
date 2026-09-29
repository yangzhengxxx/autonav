#ifndef __USART_H
#define __USART_H

#include "stm32f4xx.h"

#define BT_USART          USART2
#define BT_USART_BAUD     9600U

void bt_usart_init(void);

uint8_t bt_usart_data_available(void);

uint8_t bt_usart_read_byte(void);

#endif
