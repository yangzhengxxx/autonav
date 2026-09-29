#include "can.h"

#include "task.h"
#include "misc.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#define CAN_BS1_VALUE CAN_BS1_11tq
#define CAN_BS2_VALUE CAN_BS2_2tq
#define CAN_PRESCALER_VALUE 6U

volatile can_rx_frame_t g_can_rx = {0};

QueueHandle_t q_CanRx = NULL;
static volatile CanRuntimeStats_t g_can_stats = {0};

BaseType_t can_queue_init(void)
{

    q_CanRx = xQueueCreate(1U, sizeof(can_queue_msg_t));
    return (q_CanRx != NULL) ? pdPASS : pdFAIL;
}

void can_get_runtime_stats(CanRuntimeStats_t *snapshot)
{
    if (snapshot == NULL)
    {
        return;
    }

    taskENTER_CRITICAL();
    *snapshot = g_can_stats;
    taskEXIT_CRITICAL();
}

void can_poll_error_state(void)
{

    if ((CAN_GetFlagStatus(CAN1, CAN_FLAG_EWG) != RESET) &&
        (g_can_stats.error_warning_count == 0U))
    {
        g_can_stats.error_warning_count = 1U;
    }
    if ((CAN_GetFlagStatus(CAN1, CAN_FLAG_EPV) != RESET) &&
        (g_can_stats.error_passive_count == 0U))
    {
        g_can_stats.error_passive_count = 1U;
    }
    if ((CAN_GetFlagStatus(CAN1, CAN_FLAG_BOF) != RESET) &&
        (g_can_stats.bus_off_count == 0U))
    {
        g_can_stats.bus_off_count = 1U;
    }
}

ErrorStatus can_init(void)
{
    GPIO_InitTypeDef gpio_init;
    CAN_InitTypeDef can_init;
    CAN_FilterInitTypeDef filter_init;
    NVIC_InitTypeDef nvic_init;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_CAN1);

    GPIO_StructInit(&gpio_init);
    gpio_init.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    gpio_init.GPIO_Mode = GPIO_Mode_AF;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &gpio_init);

    CAN_DeInit(CAN1);
    CAN_StructInit(&can_init);

    can_init.CAN_TTCM = DISABLE;
    can_init.CAN_ABOM = ENABLE;
    can_init.CAN_AWUM = DISABLE;
    can_init.CAN_NART = DISABLE;
    can_init.CAN_RFLM = DISABLE;
    can_init.CAN_TXFP = DISABLE;
    can_init.CAN_Mode = CAN_Mode_Normal;

    can_init.CAN_SJW = CAN_SJW_1tq;
    can_init.CAN_BS1 = CAN_BS1_VALUE;
    can_init.CAN_BS2 = CAN_BS2_VALUE;
    can_init.CAN_Prescaler = CAN_PRESCALER_VALUE;

    if (CAN_Init(CAN1, &can_init) != CAN_InitStatus_Success)
    {
        return ERROR;
    }

    filter_init.CAN_FilterNumber = 0U;
    filter_init.CAN_FilterMode = CAN_FilterMode_IdMask;
    filter_init.CAN_FilterScale = CAN_FilterScale_32bit;
    filter_init.CAN_FilterIdHigh = (uint16_t)(CAN_RX_STD_ID << 5);
    filter_init.CAN_FilterIdLow = 0U;
    filter_init.CAN_FilterMaskIdHigh = (uint16_t)(0x7FFU << 5);
    filter_init.CAN_FilterMaskIdLow = 0U;
    filter_init.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    filter_init.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&filter_init);

    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
    CAN_ITConfig(CAN1, CAN_IT_EWG, ENABLE);
    CAN_ITConfig(CAN1, CAN_IT_EPV, ENABLE);
    CAN_ITConfig(CAN1, CAN_IT_BOF, ENABLE);
    CAN_ITConfig(CAN1, CAN_IT_ERR, ENABLE);

    nvic_init.NVIC_IRQChannel = CAN1_RX0_IRQn;
    nvic_init.NVIC_IRQChannelPreemptionPriority = 5U;
    nvic_init.NVIC_IRQChannelSubPriority = 0U;
    nvic_init.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_init);

    nvic_init.NVIC_IRQChannel = CAN1_SCE_IRQn;
    nvic_init.NVIC_IRQChannelPreemptionPriority = 7U;
    nvic_init.NVIC_IRQChannelSubPriority = 0U;
    nvic_init.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_init);

    return SUCCESS;
}

void CAN1_SCE_IRQHandler(void)
{
    uint32_t esr = CAN1->ESR;

    if ((esr & CAN_ESR_EWGF) != 0U)
    {
        g_can_stats.error_warning_count++;
    }
    if ((esr & CAN_ESR_EPVF) != 0U)
    {
        g_can_stats.error_passive_count++;
    }
    if ((esr & CAN_ESR_BOFF) != 0U)
    {
        g_can_stats.bus_off_count++;
    }

    CAN_ClearITPendingBit(CAN1, CAN_IT_ERR);
}

void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg rx_msg;
    can_queue_msg_t qmsg;
    uint8_t i;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (CAN_GetFlagStatus(CAN1, CAN_FLAG_FOV0) != RESET)
    {
        g_can_stats.rx_fifo_overrun_count++;
        CAN_ClearFlag(CAN1, CAN_FLAG_FOV0);
    }

    while (CAN_MessagePending(CAN1, CAN_FIFO0) > 0U)
    {
        CAN_Receive(CAN1, CAN_FIFO0, &rx_msg);

        qmsg.std_id = rx_msg.StdId;
        qmsg.dlc = rx_msg.DLC;
        for (i = 0U; i < CAN_RX_DATA_LEN; i++)
        {
            qmsg.data[i] = (i < rx_msg.DLC) ? rx_msg.Data[i] : 0U;
        }

        g_can_rx.std_id = qmsg.std_id;
        g_can_rx.dlc = qmsg.dlc;
        for (i = 0U; i < CAN_RX_DATA_LEN; i++)
        {
            g_can_rx.data[i] = qmsg.data[i];
        }
        g_can_rx.rx_count++;
        g_can_rx.updated = 1U;
        g_can_stats.rx_count++;

        if (q_CanRx != NULL)
        {
            if (uxQueueMessagesWaitingFromISR(q_CanRx) != 0U)
            {
                g_can_stats.rx_overwrite_count++;
            }
            (void)xQueueOverwriteFromISR(q_CanRx, &qmsg, &xHigherPriorityTaskWoken);
        }
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

ErrorStatus can_send_frame(uint32_t std_id, const uint8_t *data, uint8_t dlc)
{
    CanTxMsg tx_msg;
    uint8_t mailbox;
    uint8_t i;

    if ((data == 0) || (dlc > 8U))
    {
        return ERROR;
    }

    tx_msg.StdId = std_id;
    tx_msg.ExtId = 0U;
    tx_msg.IDE = CAN_Id_Standard;
    tx_msg.RTR = CAN_RTR_Data;
    tx_msg.DLC = dlc;

    for (i = 0U; i < dlc; i++)
    {
        tx_msg.Data[i] = data[i];
    }
    for (; i < 8U; i++)
    {
        tx_msg.Data[i] = 0U;
    }

    mailbox = CAN_Transmit(CAN1, &tx_msg);
    if (mailbox == CAN_TxStatus_NoMailBox)
    {
        g_can_stats.tx_no_mailbox_count++;
        return ERROR;
    }

    g_can_stats.tx_queued_count++;
    return SUCCESS;
}
