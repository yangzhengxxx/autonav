#ifndef __CAN_H
#define __CAN_H

#include "stm32f4xx.h"

#define BSP_CAN_BITRATE 500000U
#define CAN_RX_DATA_LEN 8U

#define CAN_RX_STD_ID 0x010U
#define CAN_TX_STD_ID 0x011U

typedef struct
{
    uint32_t std_id;
    uint8_t dlc;
    uint8_t data[CAN_RX_DATA_LEN];
    volatile uint8_t updated;
    volatile uint32_t rx_count;
} can_rx_frame_t;

extern volatile can_rx_frame_t g_can_rx;

ErrorStatus can_init(void);

ErrorStatus can_send_frame(uint32_t std_id, const uint8_t *data, uint8_t dlc);

#endif
