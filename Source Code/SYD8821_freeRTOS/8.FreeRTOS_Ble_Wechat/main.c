/* BLE */
#include "ARMCM0.h"
#include "config.h"
#include "ble_slave.h"
#include "SYD_ble_service_Profile.h"
#include "ble.h"
#include "ota.h"

/* freeRTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"

/*drive*/
#include "debug.h"
#include "delay.h"
#include "gpio.h"
#include "pad_mux_ctrl.h"
#include "led_key.h"
#include <string.h>

#define LED_TASK_DELAY        200           /**< Task delay. Delays a LED0 task for 200 ms */
#define LED_TIMER_PERIOD      1000           /**< Task delay. Delays a LED0 task for 200 ms */

QueueHandle_t Ble_RX_Queue =NULL;
#define BLE_RX_QUEUE_LEN  10 /* 队列的长度，最大可包含多少个消息 */
#define BLE_RX_QUEUE_SIZE sizeof(struct queue_att_write_data) /* 队列中每个消息大小（字节） */

TimerHandle_t led_toggle_timer_handle;  /**< Reference to LED2 toggling FreeRTOS timer. */

xQueueHandle KeyMsgQueue;
#define KEY_QUEUE_LEN  10 /* 队列的长度，最大可包含多少个消息 */
#define KEY_QUEUE_SIZE 4  /* 队列中每个消息大小（字节） */

//wechat test data
#define test_data 10000
uint32_t current_step 		= test_data;
uint32_t current_distance   = test_data * 0.6;
uint32_t current_calorie    = (0.6 * 65* test_data *78)/10000;	
uint32_t target_step 		= 15000;

void key1_irq_handle(void)
{
	BaseType_t xReturn = pdPASS;
	BaseType_t xHigherPriorityTaskWoken;
	uint32_t ulReturn;
	uint32_t send_data = 0;
	
	//xReturn = xQueueSendFromISR(Test_Queue,send_data1,NULL);
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	
	send_data = 1;
	xReturn = xQueueSendFromISR(KeyMsgQueue,&send_data,&xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	
	if (pdPASS == xReturn) 
	{
		DBGPRINTF(("\r\nkey1 irq send queue success! data:%d\r\n",send_data));
	}
	else
	{	
		DBGPRINTF(("\r\nkey1 irq send queue error!\r\n"));		
	}
	taskEXIT_CRITICAL_FROM_ISR(ulReturn);	
}

void key2_irq_handle(void)
{
	BaseType_t xReturn = pdPASS;
	BaseType_t xHigherPriorityTaskWoken;
	uint32_t ulReturn;
	uint32_t send_data = 0;
	
	//xReturn = xQueueSendFromISR(Test_Queue,send_data1,NULL);
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	
	send_data = 2;
	
	xReturn = xQueueSendFromISR(KeyMsgQueue,&send_data,&xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	
	if (pdPASS == xReturn) 
	{
		DBGPRINTF(("\r\nkey2 irq send queue success! data:%d\r\n",send_data));
	}
	else
	{	
		DBGPRINTF(("\r\nkey2 irq send queue error!\r\n"));		
	}
	taskEXIT_CRITICAL_FROM_ISR(ulReturn);	
}


void key3_irq_handle(void)
{
	BaseType_t xReturn = pdPASS;
	BaseType_t xHigherPriorityTaskWoken;
	uint32_t ulReturn;
	uint32_t send_data = 0;
	
	//xReturn = xQueueSendFromISR(Test_Queue,send_data1,NULL);
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	
	send_data = 3;
	
	xReturn = xQueueSendFromISR(KeyMsgQueue,&send_data,&xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	
	if (pdPASS == xReturn) 
	{
		DBGPRINTF(("\r\nkey3 irq send queue success! data:%d\r\n",send_data));
	}
	else
	{	
		DBGPRINTF(("\r\nkey3 irq send queue error!\r\n"));		
	}
	taskEXIT_CRITICAL_FROM_ISR(ulReturn);	
}



void gpio_callback(GPIO_BANK_TYPE type, uint32_t bitmask)
{
	switch(type)
	{
		case GPIO_BANK_0: //GPIO_0 - GPIO_31
			switch (bitmask)
			{
				case U32BIT(KEY1):key1_irq_handle();
					break;
				case U32BIT(KEY2):key2_irq_handle();
					break;
				case U32BIT(KEY3):key3_irq_handle();
					break;
				case U32BIT(KEY4):gpo_toggle(LED4);
					break;
				default:
					break;
			}
			break;
		case GPIO_BANK_1: // GPIO 32 ~ 38
			break;
		default:break;
	}
}

void board_init(void)
{
	
#ifdef CONFIG_UART_ENABLE
	//Uart init GPIO 20 21
	pad_mux_write(GPIO_20, 7);
	pad_mux_write(GPIO_21, 7);

	dbg_init();
	DBGPRINTF(("FreeRTOS BLE %s:%s ...\r\n",__DATE__ ,__TIME__));
#endif
	
	//led 1 2 3 4 --> GPIO25 23 34 10
	pad_mux_write(LED1,0);
	pad_mux_write(LED2,0);
	pad_mux_write(LED3,0);
	pad_mux_write(LED4,0);
	
	gpo_config(LED1,1);
	gpo_config(LED2,1);
	gpo_config(LED3,1);
	gpo_config(LED4,1);
	
	
	//KEY 1 2 3 4 -->GPIO14 15 16 17
	pad_mux_write(KEY1,0);
	pad_mux_write(KEY2,0);
	pad_mux_write(KEY3,0);
	pad_mux_write(KEY4,0);
	gpi_config(KEY1,PULL_UP);
	gpi_config(KEY2,PULL_UP);
	gpi_config(KEY3,PULL_UP);
	gpi_config(KEY4,PULL_UP);
	
	//config KEY irq
	gpi_irq_set_cb(gpio_callback);
	gpi_enable_int(KEY1, EDGE_TRIGGER,POL_FALLING_LOW);
	gpi_enable_int(KEY2, EDGE_TRIGGER,POL_FALLING_LOW);
	gpi_enable_int(KEY3, EDGE_TRIGGER,POL_FALLING_LOW);
}


/**@brief The function to call when the LED1 FreeRTOS timer expires.
 *
 * @param[in] pvParameter   Pointer that will be used as the parameter for the timer.
 */
static void led_toggle_timer_callback (void * pvParameter)
{
	gpo_toggle(LED2);
}

//接收数据处理
void ble_stack_write_data_handle(void)
{
	struct queue_att_write_data r_data = {0};
	BaseType_t xReturn = pdTRUE;
	//xReturn = xQueueReceive( Test_Queue, &r_queue,portMAX_DELAY);//暂时不支持使用这种方式
	xReturn = xQueueReceive(Ble_RX_Queue,(void*)&r_data,0);
	
	if (pdTRUE == xReturn)
	{
		DBGPRINTF(("BLE Queue Recevie\r\n"));
		DBGPRINTF(("UUID:%04x\r\n",r_data.uuid));
		DBGHEXDUMP("data\r\n",r_data.data,r_data.sz);
		DBGPRINTF(("\r\n"));
		if(r_data.uuid == BLE_UART_Write_UUID)
		{
			if(r_data.data[0] == 0xa1)
			{	
				gap_s_security_req(1, 0);
			}
		}
	}
}

//OTA Handle
void ota_user_handle(void)
{
	if(ota_state == 1)
	{
		
	}
	else if(ota_state == 3)
	{
		ota_state = 0;
		delay_ms(500);
		pmu_mcu_reset();
		delay_ms(1000);
	}
}

//ble stack task
void ble_stack_thread( void *pvParameters )
{
	for( ;; )
	{
		ble_sched_execute();
		ble_stack_write_data_handle();
		ota_user_handle();
		vTaskDelay(10);//10ms
	}	
}

void wechat_report_data_to_app(void)
{
	uint8_t gatt_buf[10]={0x00};
				
	#if defined(CONFIG_UART_ENABLE)
	dbg_printf("Reported sport data\r\n");
	dbg_printf("current step: %d\r\n", current_step);
	dbg_printf("current distance: %d\r\n", current_distance);
	dbg_printf("current calorie:%d\r\n", current_calorie);
	#endif
	gatt_buf[0] = WX_STEP_FLAG|WX_DISTANCE_FLAG|WX_CALORIE_FLAG;
	
	gatt_buf[1] = (uint8_t)(current_step);
	gatt_buf[2] = (uint8_t)(current_step >> 8);
	gatt_buf[3] = (uint8_t)(current_step >> 16);
	
	gatt_buf[4] = (uint8_t)(current_distance);
	gatt_buf[5] = (uint8_t)(current_distance >> 8);
	gatt_buf[6] = (uint8_t)(current_distance >> 16);
		
	gatt_buf[7] = (uint8_t)(current_calorie);
	gatt_buf[8] = (uint8_t)(current_calorie >> 8);
	gatt_buf[9] = (uint8_t)(current_calorie >> 16);
	
	Wechat_SendData(BLE_Pedometer_Measurement_UUID, gatt_buf,10);
	
	
	gatt_buf[0] = WX_STEP_FLAG;
	gatt_buf[1] = (uint8_t)(target_step);
	gatt_buf[2] = (uint8_t)(target_step >> 8);
	gatt_buf[3] = (uint8_t)(target_step >> 16);
	
	Wechat_SendData(BLE_Sport_Target_UUID, gatt_buf,4);
}


//key irq queue  handle task
void Key_IRQ_Queue_Receive_Task( void *pvParameters )
{
	BaseType_t xReturn = pdTRUE;
	uint32_t key_value = 0;
	
	for( ;; )
	{
//		xReturn = xQueueReceive( KeyMsgQueue, &r_data,portMAX_DELAY);//暂时不能使用这种方式
		xReturn = xQueueReceive(KeyMsgQueue,(void*)&key_value,0);
		if (pdTRUE == xReturn)
		{
			DBGPRINTF(("key %d irq queue receive\r\n",key_value));
			if(key_value == 3)
			{
				wechat_report_data_to_app();
			}
			
		}
		vTaskDelay(10);//10ms
	}	
}


void vStartThreadTasks( void )
{
	BaseType_t xReturn = pdPASS;/* Create Result*/
	
	DBGPRINTF(("Init task and queue\r\n"));
	
	/* Start timer for LED2 blinking */
	led_toggle_timer_handle = xTimerCreate( "LED2", LED_TIMER_PERIOD, pdTRUE, NULL, led_toggle_timer_callback);
	xReturn = xTimerStart(led_toggle_timer_handle, 0);
	if(pdPASS == xReturn) DBGPRINTF(("Create LED2 Softtimer Task Success!\r\n"));
	
		
	xReturn = xTaskCreate( ble_stack_thread, "BLE", configMINIMAL_STACK_SIZE, NULL, 3, ( xTaskHandle * ) NULL );
	if(pdPASS == xReturn) DBGPRINTF(("Create ble_stack_thread Success!\r\n"));
	
	xReturn = xTaskCreate( Key_IRQ_Queue_Receive_Task, "KEY_R", configMINIMAL_STACK_SIZE, NULL, 2, ( xTaskHandle * ) NULL );
	if(pdPASS == xReturn) DBGPRINTF(("Create Key_IRQ_Queue_Receive_Task Success!\r\n"));
	
	Ble_RX_Queue = xQueueCreate((UBaseType_t)BLE_RX_QUEUE_LEN, (UBaseType_t)BLE_RX_QUEUE_SIZE);	
	if (NULL != Ble_RX_Queue) DBGPRINTF(("Create Ble_RX_Queue Success!\r\n"));
	
	KeyMsgQueue = xQueueCreate((UBaseType_t)KEY_QUEUE_LEN, (UBaseType_t)KEY_QUEUE_SIZE);	
	if (NULL != KeyMsgQueue) DBGPRINTF(("Create KeyMsgQueue Success!\r\n"));
	
	DBGPRINTF(("\r\nSYD8821 freeRTOS Start\r\n"));
	DBGPRINTF(("\r\n"));
}


/*-----------------------------------------------------------*/
int main( void )
{
	__disable_irq();
	
	//ble stack init
	ble_init();
	
	// RC bumping
	sys_mcu_rc_calibration();	
	// Select External XO
	sys_32k_clock_set(SYSTEM_32K_CLOCK_XO);
	// Set MCU Clock 64M
	sys_mcu_clock_set(MCU_CLOCK_64_MHZ);
	
	
	board_init();
	DBGPRINTF(("board hardware init\r\n"));
	gap_s_adv_start();
	DBGPRINTF(("start ble adv\r\n\r\n"));

	__enable_irq();	

	/* Start user demo tasks. */
	vStartThreadTasks();

	/* Start the scheduler. */
	vTaskStartScheduler();
	/* Will only get here if there was not enough heap space to create the
	idle task. */
	return 0;
}


