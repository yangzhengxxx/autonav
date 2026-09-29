#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "board_config.h"
#include "can.h"
#include "gpio.h"
#include "imu.h"
#include "kinematics.h"
#include "pid.h"
#include "timer.h"
#include "usart.h"
#include "stm32f4xx_dbgmcu.h"
#include "stm32f4xx_iwdg.h"
#include "stm32f4xx_rcc.h"

#if (USE_DEBUG_UART != 0)
#include <stdio.h>
#endif

#define BT_LINEAR_SPEED 0.2f
#define BT_WZ_SPEED 0.3f

#define TASK_SPEED_LOOP_STACK_WORDS 512U
#define TASK_SPEED_LOOP_PRIORITY 4U
#define TASK_CAN_LINK_STACK_WORDS 512U
#define TASK_CAN_LINK_PRIORITY 3U
#define TASK_BT_REMOTE_STACK_WORDS 256U
#define TASK_BT_REMOTE_PRIORITY 2U
#define TASK_SUPERVISOR_STACK_WORDS 384U
#define TASK_SUPERVISOR_PRIORITY 1U

#define DIAGNOSTIC_PROTOCOL_VERSION 2U
#define IWDG_RELOAD_VALUE 500U

typedef enum
{
	CONTROL_MODE_IDLE = 0,
	CONTROL_MODE_CAN_AUTO,
	CONTROL_MODE_BT_MANUAL,
	CONTROL_MODE_FAULT,
	CONTROL_MODE_ESTOP
} ControlMode_t;

typedef enum
{
	APP_FAULT_NONE = 0U,
	APP_FAULT_CAN_INIT = (1U << 0),
	APP_FAULT_IMU_INIT = (1U << 1),
	APP_FAULT_CAN_TIMEOUT = (1U << 2),
	APP_FAULT_SPEED_TASK_STALL = (1U << 3),
	APP_FAULT_CAN_TASK_STALL = (1U << 4),
	APP_FAULT_STACK_LOW = (1U << 5),
	APP_FAULT_CAN_BUS_OFF = (1U << 6),
	APP_FAULT_MALLOC = (1U << 7),
	APP_FAULT_STACK_OVERFLOW = (1U << 8),
	APP_FAULT_ASSERT = (1U << 9)
} AppFault_t;

static volatile ControlMode_t g_control_mode = CONTROL_MODE_IDLE;
static volatile uint16_t g_app_fault_flags = APP_FAULT_NONE;
static volatile uint32_t g_speed_task_heartbeat = 0U;
static volatile uint32_t g_can_task_heartbeat = 0U;
static uint8_t g_reset_reason = 0U;
static const char * volatile g_assert_file = NULL;
static volatile int g_assert_line = 0;

static TaskHandle_t g_speed_task_handle = NULL;
static TaskHandle_t g_can_task_handle = NULL;
static TaskHandle_t g_supervisor_task_handle = NULL;

static volatile uint32_t g_last_can_cmd_ms = 0U;

static uint16_t SaturateU16(uint32_t value)
{
	return (value > 0xFFFFU) ? 0xFFFFU : (uint16_t)value;
}

static uint8_t SaturateU8(uint32_t value)
{
	return (value > 0xFFU) ? 0xFFU : (uint8_t)value;
}

static void PackU16LE(uint8_t *data, uint16_t value)
{
	data[0] = (uint8_t)(value & 0xFFU);
	data[1] = (uint8_t)((value >> 8) & 0xFFU);
}

#if (ENABLE_CONTROL_TELEMETRY != 0)
static int16_t FloatToS16(float value)
{
	if (value > 32767.0f) return 32767;
	if (value < -32768.0f) return -32768;
	return (int16_t)value;
}

static void SendNextMotorTelemetry(void)
{
	static uint8_t motor = 0U;
	static uint16_t sequence = 0U;
	PID_RuntimeSnapshot_t snapshot;
	uint8_t data[8];

	PID_GetRuntimeSnapshot(motor, &snapshot);
	PackU16LE(&data[0], (uint16_t)FloatToS16(snapshot.target));
	PackU16LE(&data[2], (uint16_t)FloatToS16(snapshot.actual));
	PackU16LE(&data[4], (uint16_t)snapshot.output);
	PackU16LE(&data[6], sequence++);
	(void)can_send_frame(CAN_TX_MOTOR_TELEMETRY_BASE_ID + motor, data, 8U);

	motor = (uint8_t)((motor + 1U) % MOTOR_NUM);
}
#endif

static uint8_t CaptureResetReason(void)
{
	uint8_t reason = 0U;

	if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET) reason |= (1U << 0);
	if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET) reason |= (1U << 1);
	if (RCC_GetFlagStatus(RCC_FLAG_SFTRST) != RESET) reason |= (1U << 2);
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET) reason |= (1U << 3);
	if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET) reason |= (1U << 4);
	if (RCC_GetFlagStatus(RCC_FLAG_BORRST) != RESET) reason |= (1U << 5);

	RCC_ClearFlag();
	return reason;
}

static void IWDG_Start(void)
{
#if (USE_IWDG != 0)
#if (IWDG_FREEZE_IN_DEBUG != 0)
	DBGMCU_Config(DBGMCU_IWDG_STOP, ENABLE);
#endif
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_64);
	IWDG_SetReload(IWDG_RELOAD_VALUE);
	IWDG_ReloadCounter();
	IWDG_Enable();
#endif
}

static void EmergencyMotorOff(void)
{
	uint8_t i;

	for (i = 0U; i < MOTOR_NUM; i++)
	{
		Motor_SetSpeedByIndex(i, 0);
	}
}

void App_AssertFailed(const char *file, int line)
{
	g_assert_file = file;
	g_assert_line = line;
	g_app_fault_flags |= APP_FAULT_ASSERT;
	taskDISABLE_INTERRUPTS();
	EmergencyMotorOff();
	for (;;) { }
}

void vApplicationMallocFailedHook(void)
{
	g_app_fault_flags |= APP_FAULT_MALLOC;
	taskDISABLE_INTERRUPTS();
	EmergencyMotorOff();
	for (;;) { }
}

void vApplicationStackOverflowHook(TaskHandle_t task, char *task_name)
{
	(void)task;
	(void)task_name;
	g_app_fault_flags |= APP_FAULT_STACK_OVERFLOW;
	taskDISABLE_INTERRUPTS();
	EmergencyMotorOff();
	for (;;) { }
}

static void Task_SpeedLoop(void *argument)
{
	(void)argument;

	for (;;)
	{

		if (xSemaphoreTake(sem_SpeedLoop, portMAX_DELAY) == pdTRUE)
		{
			uint32_t release_cycle = g_speed_loop_release_cycle;
			uint32_t start_cycle = DWT->CYCCNT;

			ReadAllEncoder();
			PID_Callback();
			SpeedLoopMetrics_Record(release_cycle, start_cycle, DWT->CYCCNT);
			g_speed_task_heartbeat++;
		}
	}
}

static void Task_CanLink(void *argument)
{
	uint32_t last_fb_ms = 0U;
#if (ENABLE_CONTROL_TELEMETRY != 0)
	uint32_t last_telemetry_ms = 0U;
#endif
	can_queue_msg_t msg;

	(void)argument;

	for (;;)
	{

		if (xQueueReceive(q_CanRx, &msg, pdMS_TO_TICKS(5)) == pdPASS)
		{

			if ((msg.std_id == CAN_RX_STD_ID) && (msg.dlc >= 6U))
			{

				int16_t vx = (int16_t)(msg.data[0] | (msg.data[1] << 8));
				int16_t vy = (int16_t)(msg.data[2] | (msg.data[3] << 8));
				int16_t wz = (int16_t)(msg.data[4] | (msg.data[5] << 8));

				if ((g_control_mode == CONTROL_MODE_IDLE) ||
					(g_control_mode == CONTROL_MODE_CAN_AUTO))
				{
					Chassis_SetVelocity(vx / 1000.0f, vy / 1000.0f, wz / 1000.0f);
					g_last_can_cmd_ms = g_tim6_ms;
					g_control_mode = CONTROL_MODE_CAN_AUTO;
					g_app_fault_flags &= (uint16_t)(~APP_FAULT_CAN_TIMEOUT);
				}
			}
		}

		if ((g_control_mode == CONTROL_MODE_CAN_AUTO) &&
			((g_tim6_ms - g_last_can_cmd_ms) >= CAN_CMD_TIMEOUT_MS))
		{
			Chassis_Stop();
			g_control_mode = CONTROL_MODE_IDLE;
			g_app_fault_flags |= APP_FAULT_CAN_TIMEOUT;
		}

		if ((g_tim6_ms - last_fb_ms) >= 20U)
		{
			last_fb_ms = g_tim6_ms;
			Chassis_CanFeedback();
		}

#if (ENABLE_CONTROL_TELEMETRY != 0)
		if ((g_tim6_ms - last_telemetry_ms) >= 5U)
		{
			last_telemetry_ms = g_tim6_ms;
			SendNextMotorTelemetry();
		}
#endif

		g_can_task_heartbeat++;
	}
}

#if (USE_BT_REMOTE != 0)

static void Task_BtRemote(void *argument)
{
	(void)argument;

	for (;;)
	{

		while (bt_usart_data_available() != 0U)
		{
			uint8_t cmd = bt_usart_read_byte();

			switch (cmd)
			{
			case 0x00U:
				Chassis_Stop();
				if ((g_control_mode != CONTROL_MODE_FAULT) &&
					(g_control_mode != CONTROL_MODE_ESTOP))
				{
					g_control_mode = CONTROL_MODE_BT_MANUAL;
				}
				break;
			case 0x01U:
				if ((g_control_mode == CONTROL_MODE_FAULT) ||
					(g_control_mode == CONTROL_MODE_ESTOP)) break;
				g_control_mode = CONTROL_MODE_BT_MANUAL;
				Chassis_SetVelocity(BT_LINEAR_SPEED, 0.0f, 0.0f);
				break;
			case 0x03U:
				if ((g_control_mode == CONTROL_MODE_FAULT) ||
					(g_control_mode == CONTROL_MODE_ESTOP)) break;
				g_control_mode = CONTROL_MODE_BT_MANUAL;
				Chassis_SetVelocity(-BT_LINEAR_SPEED, 0.0f, 0.0f);
				break;
			case 0x02U:
				if ((g_control_mode == CONTROL_MODE_FAULT) ||
					(g_control_mode == CONTROL_MODE_ESTOP)) break;
				g_control_mode = CONTROL_MODE_BT_MANUAL;
				Chassis_SetVelocity(0.0f, 0.0f, BT_WZ_SPEED);
				break;
			case 0x04U:
				if ((g_control_mode == CONTROL_MODE_FAULT) ||
					(g_control_mode == CONTROL_MODE_ESTOP)) break;
				g_control_mode = CONTROL_MODE_BT_MANUAL;
				Chassis_SetVelocity(0.0f, 0.0f, -BT_WZ_SPEED);
				break;
			case 0x05U:

				Chassis_Stop();
				if ((g_control_mode != CONTROL_MODE_FAULT) &&
					(g_control_mode != CONTROL_MODE_ESTOP))
				{
					g_control_mode = CONTROL_MODE_IDLE;
				}
				break;
			default:
				break;
			}
		}

		vTaskDelay(pdMS_TO_TICKS(10));
	}
}
#endif

static void SendDiagnosticFrames(void)
{
	static uint8_t diagnostic_page = 0U;
	SpeedLoopMetrics_t rt;
	CanRuntimeStats_t can_stats;
	uint8_t data[8];
	uint32_t cycles_per_us = SystemCoreClock / 1000000U;
	UBaseType_t speed_stack = uxTaskGetStackHighWaterMark(g_speed_task_handle);
	UBaseType_t can_stack = uxTaskGetStackHighWaterMark(g_can_task_handle);

	SpeedLoopMetrics_GetSnapshot(&rt);
	can_poll_error_state();
	can_get_runtime_stats(&can_stats);

	if (can_stats.bus_off_count != 0U)
	{
		g_app_fault_flags |= APP_FAULT_CAN_BUS_OFF;
	}
	if ((speed_stack < TASK_STACK_LOW_WATERMARK) ||
		(can_stack < TASK_STACK_LOW_WATERMARK))
	{
		g_app_fault_flags |= APP_FAULT_STACK_LOW;
	}

	if (diagnostic_page == 0U)
	{
		data[0] = DIAGNOSTIC_PROTOCOL_VERSION;
		data[1] = (uint8_t)g_control_mode;
		PackU16LE(&data[2], g_app_fault_flags);
		data[4] = g_reset_reason;
		data[5] = SaturateU8(can_stats.error_warning_count);
		data[6] = SaturateU8(can_stats.error_passive_count);
		data[7] = SaturateU8(can_stats.bus_off_count);
		(void)can_send_frame(CAN_TX_STATUS_ID, data, 8U);
	}
	else if (diagnostic_page == 1U)
	{
		PackU16LE(&data[0], SaturateU16(rt.release_missed));
		PackU16LE(&data[2], SaturateU16(rt.deadline_missed));
		PackU16LE(&data[4], SaturateU16(rt.exec_max_cycles / cycles_per_us));
		PackU16LE(&data[6], SaturateU16(rt.wake_max_cycles / cycles_per_us));
		(void)can_send_frame(CAN_TX_RT_METRICS_ID, data, 8U);
	}
	else
	{
		PackU16LE(&data[0], SaturateU16(can_stats.rx_overwrite_count));
		PackU16LE(&data[2], SaturateU16(can_stats.tx_no_mailbox_count));
		PackU16LE(&data[4], SaturateU16((uint32_t)speed_stack));
		PackU16LE(&data[6], SaturateU16((uint32_t)can_stack));
		(void)can_send_frame(CAN_TX_RESOURCE_ID, data, 8U);
	}

	diagnostic_page = (uint8_t)((diagnostic_page + 1U) % 3U);
}

static void Task_Supervisor(void *argument)
{
	TickType_t last_wake = xTaskGetTickCount();
	uint32_t last_speed_heartbeat = g_speed_task_heartbeat;
	uint32_t last_can_heartbeat = g_can_task_heartbeat;
	uint32_t last_diagnostic_ms = 0U;
	uint8_t speed_miss = 0U;
	uint8_t can_miss = 0U;
	uint8_t watchdog_healthy = 1U;

	(void)argument;
	IWDG_Start();

	for (;;)
	{
		vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(SUPERVISOR_PERIOD_MS));

		if (g_speed_task_heartbeat == last_speed_heartbeat) speed_miss++;
		else speed_miss = 0U;

		if (g_can_task_heartbeat == last_can_heartbeat) can_miss++;
		else can_miss = 0U;

		last_speed_heartbeat = g_speed_task_heartbeat;
		last_can_heartbeat = g_can_task_heartbeat;

		if (speed_miss >= TASK_HEARTBEAT_MISS_LIMIT)
		{
			g_app_fault_flags |= APP_FAULT_SPEED_TASK_STALL;
			watchdog_healthy = 0U;
		}
		if (can_miss >= TASK_HEARTBEAT_MISS_LIMIT)
		{
			g_app_fault_flags |= APP_FAULT_CAN_TASK_STALL;
			watchdog_healthy = 0U;
		}

		if (watchdog_healthy == 0U)
		{
			Chassis_Stop();
			g_control_mode = CONTROL_MODE_FAULT;
		}
#if (USE_IWDG != 0)
		else
		{
			IWDG_ReloadCounter();
		}
#endif

		if ((g_tim6_ms - last_diagnostic_ms) >= DIAGNOSTIC_PERIOD_MS)
		{
			last_diagnostic_ms = g_tim6_ms;
			SendDiagnosticFrames();
		}
	}
}

int main(void)
{
	BaseType_t created;
	ErrorStatus can_status;
	ErrorStatus imu_status;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	g_reset_reason = CaptureResetReason();

	Motor_Direction_GPIO_Init();
	configASSERT(speed_loop_sync_init() == pdPASS);
#if (USE_DEBUG_UART != 0)
	usart_init();
#endif
#if (USE_BT_REMOTE != 0)
	bt_usart_init();
#endif

	configASSERT(can_queue_init() == pdPASS);

	Chassis_Init();
	imu_status = IMU_Init();
	if (imu_status != SUCCESS)
	{

		g_app_fault_flags |= APP_FAULT_IMU_INIT;
	}

#if (USE_DEBUG_UART != 0)
	printf("FreeRTOS ready BT=%d\r\n", USE_BT_REMOTE);
#endif

	created = xTaskCreate(Task_SpeedLoop,
						  "SpeedLoop",
						  TASK_SPEED_LOOP_STACK_WORDS,
						  NULL,
						  TASK_SPEED_LOOP_PRIORITY,
						  &g_speed_task_handle);
	configASSERT(created == pdPASS);

	created = xTaskCreate(Task_CanLink,
						  "CanLink",
						  TASK_CAN_LINK_STACK_WORDS,
						  NULL,
						  TASK_CAN_LINK_PRIORITY,
						  &g_can_task_handle);
	configASSERT(created == pdPASS);

#if (USE_BT_REMOTE != 0)
	created = xTaskCreate(Task_BtRemote,
						  "BtRemote",
						  TASK_BT_REMOTE_STACK_WORDS,
						  NULL,
						  TASK_BT_REMOTE_PRIORITY,
						  NULL);
	configASSERT(created == pdPASS);
#endif

	created = xTaskCreate(Task_Supervisor,
						  "Supervisor",
						  TASK_SUPERVISOR_STACK_WORDS,
						  NULL,
						  TASK_SUPERVISOR_PRIORITY,
						  &g_supervisor_task_handle);
	configASSERT(created == pdPASS);

	can_status = can_init();
	if (can_status != SUCCESS)
	{
		g_app_fault_flags |= APP_FAULT_CAN_INIT;
		g_control_mode = CONTROL_MODE_FAULT;
		Chassis_Stop();
	}
	BSP_TIM_Init();

	vTaskStartScheduler();
	for (;;)
	{
	}
}
