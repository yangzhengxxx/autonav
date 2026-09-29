#ifndef __CAN_H
#define __CAN_H

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "queue.h"

#define BSP_CAN_BITRATE 500000U
#define CAN_RX_DATA_LEN 8U

#define CAN_RX_STD_ID 0x010U
#define CAN_TX_STD_ID 0x011U
#define CAN_TX_STATUS_ID 0x012U
#define CAN_TX_RT_METRICS_ID 0x013U
#define CAN_TX_RESOURCE_ID 0x014U
#define CAN_TX_MOTOR_TELEMETRY_BASE_ID 0x120U

typedef struct
{
    uint32_t std_id;
    uint8_t dlc;
    uint8_t data[CAN_RX_DATA_LEN];
} can_queue_msg_t;

typedef struct
{
    uint32_t std_id;
    uint8_t dlc;
    uint8_t data[CAN_RX_DATA_LEN];
    volatile uint8_t updated;
    volatile uint32_t rx_count;
} can_rx_frame_t;

extern volatile can_rx_frame_t g_can_rx;

extern QueueHandle_t q_CanRx;

typedef struct
{
    uint32_t rx_count;
    uint32_t rx_overwrite_count;
    uint32_t rx_fifo_overrun_count;
    uint32_t tx_queued_count;
    uint32_t tx_no_mailbox_count;
    uint32_t error_warning_count;
    uint32_t error_passive_count;
    uint32_t bus_off_count;
} CanRuntimeStats_t;

void can_get_runtime_stats(CanRuntimeStats_t *snapshot);
void can_poll_error_state(void);

ErrorStatus can_init(void);

BaseType_t can_queue_init(void);

ErrorStatus can_send_frame(uint32_t std_id, const uint8_t *data, uint8_t dlc);

#endif
