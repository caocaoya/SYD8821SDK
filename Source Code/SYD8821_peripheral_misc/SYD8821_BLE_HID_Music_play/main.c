#include "ARMCM0.h"
#include "gpio.h"
#include "debug.h"
#include "delay.h"
#include "ota.h"
#include "config.h"
#include "ble_slave.h"
#include "SYD_ble_service_Profile.h"
#include <string.h>
#include "timer.h"
#include "pad_mux_ctrl.h"
#include "Debuglog.h"
#include "ancs.h"


#define VOL_UP_KEY_VALUE 	233
#define VOL_DOWN_KEY_VALUE  234

#define PLAY_PAUSE_KEY_BIT 	0x01
#define FORWARD_KEY_BIT  0x08
#define BACK_KEY_BIT  0x04

struct gap_evt_callback GAP_Event;
static struct gap_att_report_handle *g_report;

//notify 标志 1代表已经使能 0代表未使能
static uint8_t start_tx=0;
uint8_t battery_tx=0;
uint8_t hid_key_tx=0;

//connection
uint8_t  connect_flag=0;

//HID
//static struct HID_KeyBoard_Report kb_report;
static struct HID_KeyBoard_Media_Report kb_media_report;
static struct HID_Media_Player_Control media_player_control;


//key status 0空闲 1按下 
uint8_t KEY1_Status = 0; 
uint8_t KEY2_Status = 0;
uint8_t KEY3_Status = 0;

uint8_t ADV_DATA[] = {
		0x03,// length
		0x19,
		0x00,
		0x00,
		0x02,// length
		0x01,// AD Type: Flags
		0x05,// LE Limited Discoverable Mode & BR/EDR Not Supported
		0x03,// length
		0x03,// AD Type: Complete list of 16-bit UUIDs 
		0x12,// UUID:  (0x0001)
		0x18,// UUID:  (0x0001)
		0X09,// length
		0XFF,// AD Type: MANUFACTURER SPECIFIC DATA
		0X00,// Company Identifier (0x00)
		0X00,// Company Identifier (0x00)
		0X00,
		0X00,
		0X00,
		0X00,
		0X00,
		0X00,
		0x06,// length
		0x09,// AD Type: Complete local name
		'U',
		'A',
		'R',
		'T',
		'F',
};


uint16_t ADV_DATA_SZ = sizeof(ADV_DATA); 
uint8_t SCAN_DATA[]={
#ifdef _WECHAT_
	0x03,// length
	0x03,// AD Type: Complete list of 16-bit UUIDs 
	0xe7,// UUID: 0xfee7
	0xfe,// UUID: 0xfee7
#endif
#ifdef _HID_
	0x03,// length
	0x03,// AD Type: Complete list of 16-bit UUIDs 
	0x12,// UUID: 0xfee7
	0x18,// UUID: 0xfee7
#endif		
};
uint16_t SCAN_DATA_SZ = sizeof(SCAN_DATA);  

static void ble_init(void);
uint8_t BLE_SendData(uint8_t *buf, uint8_t len);
uint8_t Battery_SendData(uint8_t *buf, uint8_t len);


//广播数据初始化
static void setup_adv_data()
{
	struct gap_adv_params adv_params;	
	static struct gap_ble_addr dev_addr;
	
	adv_params.type = ADV_IND;
	adv_params.channel = 0x07;    // advertising channel : 37 & 38 & 39
	adv_params.interval = 0x640;  // advertising interval : unit 0.625ms)
	adv_params.timeout = 0x3FFF;    // timeout : uint seconds
	adv_params.hop_interval = 0x03;  //0x1c 
	adv_params.policy = 0x00;   

	gap_s_adv_parameters_set(&adv_params);

	/*get bluetooth address */
	gap_s_ble_address_get(&dev_addr);

	#ifdef _WECHAT_
	ADV_DATA[20] = dev_addr.addr[0];
	ADV_DATA[19] = dev_addr.addr[1];
	ADV_DATA[18] = dev_addr.addr[2];
	ADV_DATA[17] = dev_addr.addr[3];
	ADV_DATA[16] = dev_addr.addr[4];
	ADV_DATA[15] = dev_addr.addr[5];
	#else
	ADV_DATA[15] = dev_addr.addr[0];
	ADV_DATA[16] = dev_addr.addr[1];
	ADV_DATA[17] = dev_addr.addr[2];
	ADV_DATA[18] = dev_addr.addr[3];
	ADV_DATA[19] = dev_addr.addr[4];
	ADV_DATA[20] = dev_addr.addr[5];
	#endif
	DBGPRINTF(("addr type %02x \r\n",dev_addr.type));
	DBGHEXDUMP("addr", dev_addr.addr, 6);

	gap_s_adv_data_set(ADV_DATA, ADV_DATA_SZ, SCAN_DATA, SCAN_DATA_SZ); 
}

/*
uint8_t target
0:fast
1:slow
*/
//连接更新函数
void BLSetConnectionUpdate(uint8_t target){
	struct gap_link_params  link_app;
	struct gap_smart_update_params smart_params;
	uint8_t buffer_cha1[5]={0XFC,0X01,0X00,0X00,0X00};
	gap_s_link_parameters_get(&link_app);
	#if defined(CONFIG_UART_ENABLE) || defined(_SYD_RTT_DEBUG_)
	dbg_printf("interval:%x latency:%x\r\n",link_app.interval,link_app.latency);
	#endif
	switch(target){
		case 0: 
				if((link_app.latency !=0) && (link_app.interval >0x10)){
					/* connection parameters */
					smart_params.updateitv_target=0x0010;  //target connection interval (60 * 1.25ms = 75 ms)
					smart_params.updatesvto=0x00c8;  //supervisory timeout (400 * 10 ms = 4s)
					smart_params.updatelatency=0x0000;
					smart_params.updatectrl=SMART_CONTROL_LATENCY | SMART_CONTROL_UPDATE;
					smart_params.updateadj_num=MAX_UPDATE_ADJ_NUM;
					gap_s_smart_update_latency(&smart_params);
				}
			  #if defined(CONFIG_UART_ENABLE) || defined(_SYD_RTT_DEBUG_)
				DBGPRINTF(("SetUpdate ota link\r\n"));
			  #endif
				BLE_SendData(buffer_cha1,5);
		break;
		case 1:
				if((link_app.latency <0x000A) && (link_app.interval <0x0050)){
					/* connection parameters */
					smart_params.updateitv_target=0x0050;
					smart_params.updatelatency=0x000A;
					smart_params.updatesvto=0x0258;
					smart_params.updatectrl=SMART_CONTROL_LATENCY | SMART_CONTROL_UPDATE;
					smart_params.updateadj_num=MAX_UPDATE_ADJ_NUM;
					gap_s_smart_update_latency(&smart_params);	   
			#if defined(CONFIG_UART_ENABLE) || defined(_SYD_RTT_DEBUG_)
					DBGPRINTF(("SetUpdate ios link\r\n"));
			#endif
				}
		break;
	}
}


static void ble_gatt_read(struct gap_att_read_evt evt)
{
	if(evt.uuid == BLE_DEVICE_NAME_UUID)
	{
		uint8_t gatt_buf[]={'U', 'A', 'R', 'T','F'};
		uint8_t gatt_buf_sz = sizeof(gatt_buf); 

		gap_s_gatt_read_rsp_set(gatt_buf_sz, gatt_buf);
	}
	else if(evt.uuid == BLE_APPEARANCE_UUID)
	{
		uint8_t gatt_buf[]={0xff, 0xff};
		uint8_t gatt_buf_sz = sizeof(gatt_buf); 

		gap_s_gatt_read_rsp_set(gatt_buf_sz, gatt_buf);
	}
	else if(evt.uuid == BLE_MANUFACTURER_NAME_STRING_UUID)
	{
		uint8_t gatt_buf[]={'S','Y','D',' ','I', 'n', 'c', '.'};
		uint8_t gatt_buf_sz = sizeof(gatt_buf); 

		gap_s_gatt_read_rsp_set(gatt_buf_sz, gatt_buf);
	}
	else if(evt.uuid == BLE_MODEL_NUMBER_STRING_UUID)
	{
		uint8_t gatt_buf[]={'B', 'L', 'E', ' ', '1', '.', '0'};
		uint8_t gatt_buf_sz = sizeof(gatt_buf); 

		gap_s_gatt_read_rsp_set(gatt_buf_sz, gatt_buf);
	}
	else if(evt.uuid == BLE_SERIAL_NUMBER_STRING_UUID)
	{
		uint8_t gatt_buf[]={'1','.','0','.','0'};
		uint8_t gatt_buf_sz = sizeof(gatt_buf); 

		gap_s_gatt_read_rsp_set(gatt_buf_sz, gatt_buf);
	}
	else if(evt.uuid == BLE_HARDWARE_REVISION_STRING_UUID)
	{
		uint8_t gatt_buf[]={'2','.','0','0'};
		uint8_t gatt_buf_sz = sizeof(gatt_buf); 

		gap_s_gatt_read_rsp_set(gatt_buf_sz, gatt_buf);
	}
	else if(evt.uuid == BLE_FIRMWARE_REVISION_STRING_UUID)
	{
		uint8_t gatt_buf[]={'3','.','0','0'};
		uint8_t gatt_buf_sz = sizeof(gatt_buf); 

		gap_s_gatt_read_rsp_set(gatt_buf_sz, gatt_buf);
	}
	else if(evt.uuid == BLE_SOFTWARE_REVISION_STRING_UUID)
	{
		uint8_t gatt_buf[]={'4','.','0','0'};
		uint8_t gatt_buf_sz = sizeof(gatt_buf); 

		gap_s_gatt_read_rsp_set(gatt_buf_sz, gatt_buf);
	}
	else if(evt.uuid == BLE_PNP_ID_UUID)
	{
		uint8_t gatt_buf[]={ 0x02, 						//		Vendor ID Source
						     0x3a,0x09,					//		USB Vendor ID
						    0x05,0x0a,					//		Product ID
						    0x02,0x00	};				//		Product Version
												 
		uint8_t gatt_buf_sz = sizeof(gatt_buf); 

		gap_s_gatt_read_rsp_set(gatt_buf_sz, gatt_buf);
	}
	else if(evt.uuid == BLE_WECHAT_Read_UUID)
	{
		
		uint8_t temp[20] = {0};
		static struct gap_ble_addr dev_addr;
		gap_s_ble_address_get(&dev_addr);
		
		/*get bluetooth address */
		temp[0] = dev_addr.addr[5];
		temp[1] = dev_addr.addr[4];
		temp[2] = dev_addr.addr[3];
		temp[3] = dev_addr.addr[2];
		temp[4] = dev_addr.addr[1];
		temp[5] = dev_addr.addr[0];
		gap_s_gatt_read_rsp_set(6,temp);
	}
	#ifdef _OTA_
	else if(evt.uuid == BLE_OTA_Read_Write_UUID)
	{
		uint8_t sz=0;

		uint8_t rsp[sizeof(struct hci_evt)]={0};

		ota_rsp(rsp, &sz);
		
		gap_s_gatt_read_rsp_set(sz, rsp);
	}
	#endif
	else if(evt.uuid == BLE_Battry_Level_UUID)
	{
		//gap_s_gatt_read_rsp_set(0x01, 0X64);
	}
	
}

//接收函数
static void ble_gatt_write(struct gap_att_write_evt evt)
{
	// rx data
	//evt.data是收取到的数据buf
	if(evt.uuid== BLE_UART_Write_UUID)
	{
		if(evt.data[0] == 0xa1)
		{	
			gap_s_security_req(1, 0);
		}
	}
	#ifdef _OTA_
	else if(evt.uuid== BLE_OTA_Read_Write_UUID)
	{
		ota_cmd(evt.data, evt.sz);
	}
	#endif
}
//发送函数
uint8_t BLE_SendData(uint8_t *buf, uint8_t len)
{
	struct gap_att_report report;
	
	if(start_tx == 1)
	{
		report.primary = BLE_UART;
		report.uuid = BLE_UART_NOTIFY_UUID;
		report.hdl = BLE_UART_NOTIFY_VALUE_HANDLE;					
		report.value = BLE_GATT_NOTIFICATION;
		return gap_s_gatt_data_send(BLE_GATT_NOTIFICATION, &report, len, buf);
	}
	return 0;
}

//电池发送函数
uint8_t Battery_SendData(uint8_t *buf, uint8_t len)
{
	struct gap_att_report report;
	
	if(battery_tx == 1)
	{
		report.primary = BLE_BATTERY_SERVICE;
		report.uuid = BLE_Battry_Level_UUID;
		report.hdl = BLE_Battry_Level_VALUE_HANDLE;					
		report.value = BLE_GATT_NOTIFICATION;
		return gap_s_gatt_data_send(BLE_GATT_NOTIFICATION, &report, len, buf);
	}
	return 0;
}


//HID发送函数
//键值发送函数
uint8_t HID_Key_Vlaue_Send(uint8_t *buf, uint8_t len)
{
	struct gap_att_report report;
	
	if(hid_key_tx == 1)
	{
		report.primary = BLE_HIDO_SERVICE;
		report.uuid = BLE_Consumer_REPORT_UUID;
		report.config = BLE_Consumer_REPORT_VALUE_HANDLE +1;
		report.hdl = BLE_Consumer_REPORT_VALUE_HANDLE;					
		report.value = BLE_GATT_NOTIFICATION;
		return gap_s_gatt_data_send(BLE_GATT_NOTIFICATION, &report, len, buf);
	}
	return 0;
}

//回调函数
void ble_evt_callback(struct gap_ble_evt *p_evt)
{
	if(p_evt->evt_code == GAP_EVT_ADV_END)
	{		
		gap_s_adv_start();
		#if defined(CONFIG_UART_ENABLE) || defined(_SYD_RTT_DEBUG_)
		DBGPRINTF(("GAP_EVT_ADV_END\r\n"));
		#endif
	}
	else if(p_evt->evt_code == GAP_EVT_CONNECTED)	 //连接事件
	{
		start_tx = 0;
		connect_flag=1;								 //连接状态
		ota_state =0;
		
		#if defined(CONFIG_UART_ENABLE) || defined(_SYD_RTT_DEBUG_) 
		DBGHEXDUMP("GAP_EVT_CONNECTED addr:",p_evt->evt.bond_dev_evt.addr,sizeof(p_evt->evt.bond_dev_evt.addr));
		#endif
		
		//BLSetConnectionUpdate(1);
	}
	else if(p_evt->evt_code == GAP_EVT_DISCONNECTED) //断连事件
	{	
		connect_flag=0;								 //连接状态
		ota_state =0;
		start_tx = 0;
		#if defined(CONFIG_UART_ENABLE)
			DBGPRINTF(("GAP_EVT_DISCONNECTED(%02x)\r\n",p_evt->evt.disconn_evt.reason));
		#elif defined(_SYD_RTT_DEBUG_)
			DBGPRINTF("GAP_EVT_DISCONNECTED(%02x)\r\n",p_evt->evt.disconn_evt.reason);
		#endif   
		gap_s_adv_start();
	}
	else if(p_evt->evt_code == GAP_EVT_ATT_HANDLE_CONFIGURE)
	{	
//		#if defined(CONFIG_UART_ENABLE) || defined(_SYD_RTT_DEBUG_) 
//			dbg_printf("config_evt.hdl: %02x  \r\n",p_evt->evt.att_handle_config_evt.hdl);
//			dbg_printf("config_evt.value:%02x \r\n",p_evt->evt.att_handle_config_evt.value);
//			dbg_printf("config_evt.uuid:%02x  \r\n",p_evt->evt.att_handle_config_evt.uuid);
//		#endif	
		if(p_evt->evt.att_handle_config_evt.uuid == BLE_UART)
		{
			if(p_evt->evt.att_handle_config_evt.hdl == (BLE_UART_NOTIFY_VALUE_HANDLE + 1))
			{
				if(p_evt->evt.att_handle_config_evt.value == BLE_GATT_NOTIFICATION)
				{
					#if defined(CONFIG_UART_ENABLE) || defined(_SYD_RTT_DEBUG_) 
					DBGPRINTF(("start_tx enable\r\n"));
					#endif
					start_tx = ENABLE;
				}
				else
				{			
					start_tx = DISABLE;
					#if defined(CONFIG_UART_ENABLE) || defined(_SYD_RTT_DEBUG_) 
					DBGPRINTF(("start_tx disable\r\n"));
					#endif
				}
			}
		}
		else if(p_evt->evt.att_handle_config_evt.uuid == BLE_BATTERY_SERVICE)
		{
			if(p_evt->evt.att_handle_config_evt.hdl == (BLE_Battry_Level_VALUE_HANDLE + 1))
			{
				if(p_evt->evt.att_handle_config_evt.value == BLE_GATT_NOTIFICATION)
				{
					#if defined(CONFIG_UART_ENABLE) || defined(_SYD_RTT_DEBUG_) 
					DBGPRINTF(("battery_tx enable\r\n"));
					#endif
					battery_tx = ENABLE;
				}
				else
				{			
					battery_tx = DISABLE;
					#if defined(CONFIG_UART_ENABLE) || defined(_SYD_RTT_DEBUG_) 
					DBGPRINTF(("battery_tx disable\r\n"));
					#endif
				}
			}
		}
		else if(p_evt->evt.att_handle_config_evt.uuid == BLE_HIDO_SERVICE)
		{
			if(p_evt->evt.att_handle_config_evt.hdl == (BLE_Consumer_REPORT_VALUE_HANDLE + 1))
			{
				if(p_evt->evt.att_handle_config_evt.value == BLE_GATT_NOTIFICATION)
				{
					#if defined(CONFIG_UART_ENABLE) || defined(_SYD_RTT_DEBUG_) 
					DBGPRINTF(("hid_key_tx enable\r\n"));
					#endif
					hid_key_tx = ENABLE;
				}
				else
				{			
					hid_key_tx = DISABLE;
					#if defined(CONFIG_UART_ENABLE) || defined(_SYD_RTT_DEBUG_) 
					DBGPRINTF(("hid_key_tx disable\r\n"));
					#endif
				}
			}
		}
		
	}
	else if(p_evt->evt_code == GAP_EVT_ATT_WRITE)
	{
		ble_gatt_write(p_evt->evt.att_write_evt);
		//DBGPRINTF(("GAP_EVT_ATT_WRITE uuid:(%02x)\r\n",p_evt->evt.att_write_evt.uuid));
	}
	else if(p_evt->evt_code == GAP_EVT_ATT_READ)
	{
		ble_gatt_read(p_evt->evt.att_read_evt);
    //DBGPRINTF(("GAP_EVT_ATT_READ uuid:(%02x)\r\n",p_evt->evt.att_write_evt.uuid));
	}
	else if(p_evt->evt_code == GAP_EVT_ATT_HANDLE_CONFIRMATION)
	{
		//DBGPRINTF(("GAP_EVT_ATT_HANDLE_CONFIRMATION uuid:(%02x)\r\n",p_evt->evt.att_handle_config_evt.uuid));
	}
	else if(p_evt->evt_code == GAP_EVT_ENC_KEY)
	{	
		#if defined(CONFIG_UART_ENABLE) || defined(_SYD_RTT_DEBUG_) 
			DBGPRINTF(("GAP_EVT_ENC_KEY\r\n"));
		#endif
	}
	else if(p_evt->evt_code == GAP_EVT_ENC_START)
	{
		//p_evt->evt.enc_key_evt.
		#if defined(CONFIG_UART_ENABLE) || defined(_SYD_RTT_DEBUG_) 
			DBGPRINTF(("GAP_EVT_ENC_START\r\n"));
		#endif
	}
  else if(p_evt->evt_code == GAP_EVT_CONNECTION_UPDATE_RSP)
	{
		switch(p_evt->evt.connection_update_rsp_evt.result)
		{
			case CONN_PARAM_ACCEPTED:
				#if defined(CONFIG_UART_ENABLE) || defined(_SYD_RTT_DEBUG_) 
				DBGPRINTF(("update rsp ACCEPTED\r\n"));
				#endif
				break;
			case CONN_PARAM_REJECTED:
				#if defined(CONFIG_UART_ENABLE) || defined(_SYD_RTT_DEBUG_)
				DBGPRINTF(("update rsp REJECTED\r\n"));
				#endif
				break;
			case CONN_PARAM_SMART_TIMEROUT:
				#if defined(CONFIG_UART_ENABLE) || defined(_SYD_RTT_DEBUG_)
				DBGPRINTF(("update rsp TIMEROUT\r\n"));
				#endif
				break;
			case CONN_PARAM_SMART_SUCCEED:
				#if defined(CONFIG_UART_ENABLE) || defined(_SYD_RTT_DEBUG_)
				DBGPRINTF(("update rsp SUCCEED\r\n"));
				#endif
				break;
			case CONN_PARAM_LATENCY_ENABLE:
				#if defined(CONFIG_UART_ENABLE) || defined(_SYD_RTT_DEBUG_)
				DBGPRINTF(("Enable latency\r\n"));
				#endif
				break;
			case CONN_PARAM_LATENCY_DISABLE:
				#if defined(CONFIG_UART_ENABLE) || defined(_SYD_RTT_DEBUG_)
				DBGPRINTF(("Disable latency\r\n"));
				#endif
				break;
		}
	}
}

//BLE 初始化函数
static void ble_init()
{	
	struct gap_wakeup_config pw_cfg;
	struct gap_profile_struct gatt;
	struct gap_pairing_req sec_params;
	struct gap_connection_param_rsp_pdu connection_params;

	gap_s_ble_init();
	
	//set profile
	gatt.report_handle_address = (uint32_t)_gatt_database_report_handle;
	gatt.primary_address	= (uint32_t)_gatt_database_primary;
	gatt.include_address	= (uint32_t)_gatt_database_include;
	gatt.characteristic_address	= (uint32_t)_gatt_database_characteristic;
	gatt.value_address = (uint32_t)_gatt_database_value;
	gap_s_gatt_profiles_set(&gatt);

	//set device bond configure
	sec_params.io = IO_NO_INPUT_OUTPUT;
	sec_params.oob = OOB_AUTH_NOT_PRESENT;
	sec_params.flags = AUTHREQ_BONDING;
	sec_params.mitm = 0;
	sec_params.max_enc_sz = 16;
	sec_params.init_key = 0;
	sec_params.rsp_key = (GAP_KEY_MASTER_IDEN |GAP_KEY_ADDR_INFO);
	gap_s_security_parameters_set(&sec_params);
 
	//set ble connect params
	connection_params.Interval_Min = 6;
	connection_params.Interval_Max = 9;
	connection_params.Latency = 100;
	connection_params.Timeout = 100;
	connection_params.PeferredPeriodicity = 6;
	connection_params.ReferenceConnEventCount = 50;
	connection_params.Offset[0] = 0;
	connection_params.Offset[1] = 1;
	connection_params.Offset[2] = 2;
	connection_params.Offset[3] = 3;
	connection_params.Offset[4] = 4;
	connection_params.Offset[5] = 5;
	gap_s_connection_param_set(&connection_params);

	//set connect event callback 
	GAP_Event.evt_mask=(GAP_EVT_CONNECTION_EVENT);
	GAP_Event.p_callback=&ble_evt_callback;
	gap_s_evt_handler_set(&GAP_Event);

	gap_s_gatt_report_handle_get(&g_report);

	bm_s_bond_manager_idx_set(0);
	
	setup_adv_data();

	//set MCU wakup source
	pw_cfg.timer_wakeup_en = 1;
	pw_cfg.gpi_wakeup_en = 1;
	pw_cfg.gpi_wakeup_cfg = U32BIT(KEY1_Pin) | U32BIT(KEY2_Pin); 
	pw_cfg.gpi_wakeup_pol = U32BIT(KEY1_Pin) | U32BIT(KEY2_Pin); 
	pmu_wakeup_config(&pw_cfg);
}


//IO初始化
void gpio_init(void)
{
	uint8_t i;
	for(i=0;i<39;i++)
    {
        switch(i)
        {
            case GPIO_0:
                pad_mux_write(i, 0);
                gpi_config(i,PULL_DOWN);
                gpo_config(i,0);
            break;
			#ifdef CONFIG_UART_ENABLE
			case UART_RXD_0:
			case UART_TXD_0:
				pad_mux_write(i, 7);
			break;
			#endif
			case LED1_Pin:
			case LED2_Pin:
			case LED3_Pin:
			case LED4_Pin:
                pad_mux_write(i, 0);
                gpo_config(i,1);
            break;
			
			case KEY1_Pin:
			case KEY2_Pin:
                pad_mux_write(i, 0);
                gpi_config(i,PULL_UP);
            break;
						
			case GPIO_2:
			case GPIO_4:
			#ifdef _SWDDEBUG_DISENABLE_
			case GPIO_31:
			#endif
                pad_mux_write(i, 0);
                gpo_config(i,0);
            break;
						
			case GPIO_24:
			case GPIO_30:
			#ifndef _SWDDEBUG_DISENABLE_
			case GPIO_31:
			#endif
            break;
						
            default:
                pad_mux_write(i, 0);
                gpi_config(i,PULL_UP);
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


int main()
{	
	__disable_irq();
	
	ble_init();
	
	sys_mcu_clock_set(MCU_CLOCK_64_MHZ);
	// RC bumping
    sys_mcu_rc_calibration();
	
	#ifdef USER_32K_CLOCK_RCOSC
	sys_32k_clock_set(SYSTEM_32K_CLOCK_LPO);
	delay_ms(500);
	sys_32k_lpo_calibration();						//这是内部RC32k晶振的校准函数	经过该函数后定时器能够得到一个比较准确的值
	#else
	sys_32k_clock_set(SYSTEM_32K_CLOCK_XO);
	#endif
	
	gpio_init();								 //gpio初始化
	
	#ifdef _SYD_RTT_DEBUG_
		DebugLogInit();
		dbg_printf("SYD-TEK.Inc\r\n");
		dbg_printf("SYD RTT Init %s:%s\r\n",__DATE__ ,__TIME__);
	#elif defined(CONFIG_UART_ENABLE) 
		dbg_init();
		dbg_printf("SYD-TEK.Inc\r\n");
		dbg_printf("SYD8821 BLE HID %s:%s\r\n",__DATE__ ,__TIME__);
	#endif
	
	__enable_irq();	
	
	gap_s_adv_start();
	
	#if defined(CONFIG_UART_ENABLE) || defined(_SYD_RTT_DEBUG_)
	DBGPRINTF(("gap_s_adv_start\r\n"));
	#endif

	while(1)
	{				
		ble_sched_execute();
		
        gpo_toggle(LED2_Pin);
		
		//播放暂停
		if(!gpi_get_val(KEY1_Pin))
		{
			KEY1_Status = 1;
			#if defined(CONFIG_UART_ENABLE) || defined(_SYD_RTT_DEBUG_) 
				DBGPRINTF(("KEY1_Pin ctrl PLAY_PAUSE\r\n"));
			#endif
			media_player_control.data = PLAY_PAUSE_KEY_BIT;//VOL+ key value
			
			HID_Key_Vlaue_Send((uint8_t*)&media_player_control,sizeof(struct HID_Media_Player_Control));
			while(!gpi_get_val(KEY1_Pin));
		}
		else
		{
			//需要再发送一个0，作用类似于释放按键。
			//安卓系统有的是没问题，但苹果系统中，音量+发送后相当于一直按住音量+键。
			if(KEY1_Status == 1)
			{
				KEY1_Status = 0;
				media_player_control.data = 0; 
				HID_Key_Vlaue_Send((uint8_t*)&media_player_control,sizeof(struct HID_Media_Player_Control));	
				delay_ms(1000);    //这里只是为了做粗暴的消抖延时，并不是发送键值需要那么长的时间，放到你的项目中请另外做消抖操作
			}
			
		}
		
		//上一首
		if(!gpi_get_val(KEY2_Pin))
		{
			KEY2_Status = 1;
			#if defined(CONFIG_UART_ENABLE) || defined(_SYD_RTT_DEBUG_) 
				DBGPRINTF(("KEY2_Pin ctrl FORWARD\r\n"));
			#endif
			media_player_control.data = FORWARD_KEY_BIT;//VOL- key value
			
			HID_Key_Vlaue_Send((uint8_t*)&media_player_control,sizeof(struct HID_Media_Player_Control));
			while(!gpi_get_val(KEY2_Pin));
		}
		else
		{
			//需要再发送一个0，作用类似于释放按键。
			//安卓系统有的是没问题，但苹果系统中，音量-发送后相当于一直按住音量-键。
			if(KEY2_Status == 1)
			{
				KEY2_Status = 0;
				media_player_control.data = 0;
				HID_Key_Vlaue_Send((uint8_t*)&media_player_control,sizeof(struct HID_Media_Player_Control));
				delay_ms(1000);    //这里只是为了做粗暴的消抖延时，并不是发送键值需要那么长的时间，放到你的项目中请另外做消抖操作
			}
		}
		
		//下一首
		if(!gpi_get_val(KEY3_Pin))
		{
			KEY3_Status = 1;
			#if defined(CONFIG_UART_ENABLE) || defined(_SYD_RTT_DEBUG_) 
				DBGPRINTF(("KEY3_Pin ctrl BACK\r\n"));
			#endif
			media_player_control.data = BACK_KEY_BIT;//VOL- key value
			
			HID_Key_Vlaue_Send((uint8_t*)&media_player_control,sizeof(struct HID_Media_Player_Control));
			while(!gpi_get_val(KEY3_Pin));
		}
		else
		{
			//需要再发送一个0，作用类似于释放按键。
			//安卓系统有的是没问题，但苹果系统中，音量-发送后相当于一直按住音量-键。
			if(KEY3_Status == 1)
			{
				KEY3_Status = 0;
				media_player_control.data = 0;
				HID_Key_Vlaue_Send((uint8_t*)&media_player_control,sizeof(struct HID_Media_Player_Control));
				delay_ms(1000);		//这里只是为了做粗暴的消抖延时，并不是发送键值需要那么长的时间，放到你的项目中请另外做消抖操作
			}
		}
		ota_user_handle();
		
		#ifdef _SWDDEBUG_DISENABLE_
		PMU_CTRL->UART_EN = 0;
		SystemSleep(POWER_SAVING_RC_OFF, FLASH_LDO_MODULE, 11000 , (PMU_WAKEUP_CONFIG_TYPE)(FSM_SLEEP_EN|TIMER_WAKE_EN|RTC_WAKE_EN));
		#endif
	}	
}

