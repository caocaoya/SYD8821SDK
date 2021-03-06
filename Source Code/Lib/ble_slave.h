#ifndef _BLE_LIB_H_
#define _BLE_LIB_H_

#include "ARMCM0.h"

#pragma pack(1)

/*************************************************************************************************
*宏定义：BD_ADDR_SZ
*说明： 蓝牙地址大小，根据规范，该宏为6个byte
**************************************************************************************************/
#define BD_ADDR_SZ 			6



/*************************************************************************************************
*宏定义：MAX_EDIV_SZ
*说明： 蓝牙EDIV参数大小，根据规范，该宏为2个byte
**************************************************************************************************/
#define MAX_EDIV_SZ			2



/*************************************************************************************************
*宏定义：MAX_RAND_SZ
*说明： 蓝牙RAND参数大小，根据规范，该宏为8个byte
**************************************************************************************************/
#define MAX_RAND_SZ		8



/*************************************************************************************************
*宏定义：MAX_KEY_SZ
*说明： 蓝牙LTK密匙的大小，根据规范，该宏为16个byte，128bit
**************************************************************************************************/
#define MAX_KEY_SZ			16



/*************************************************************************************************
*宏定义：MAX_IRK_SZ
*说明： 蓝牙IRK参数的大小，根据规范，该宏为16个byte，128bit
**************************************************************************************************/
#define MAX_IRK_SZ			16



/*************************************************************************************************
*宏定义：LL_WIN_OFFSET_SZ
*说明： 蓝牙BLE中LL_CONNECTION_PARAM_RSP响应中OFFSET的大小，根据规范，一共有6个OFFSET,因为该宏用于
LL_CONNECTION_PARAM_RSP结构体中的Offset数组大小，所以这里定义为最大的6
**************************************************************************************************/
#define LL_WIN_OFFSET_SZ	6




/*************************************************************************************************
*宏定义：MAX_ATT_DATA_SZ
*说明： 蓝牙BLE中ATT层的MTU值，也就是最大的ATT数据大小，根据4.2规范定义，该值为512
**************************************************************************************************/
#define MAX_ATT_DATA_SZ	512



/*************************************************************************************************
*宏定义：MAX_ADV_DATA_SZ
*说明： 蓝牙BLE中广播通道的数据量的最大值，也就是最大的ATT数据大小，根据4规范定义，该值为31
**************************************************************************************************/
#define MAX_ADV_DATA_SZ	31



/*************************************************************************************************
*宏定义：MAX_ATT_REPORT_HDL
*说明： SYD8821最大的notify或者indicate通道数目，也是gap_att_report_handle结构体中的gap_att_report
成员的最大个数
**************************************************************************************************/
#define MAX_ATT_REPORT_HDL 20



/*************************************************************************************************
*宏定义：MAX_UPDATE_ADJ_NUM
*说明： SYD8821最大的连接参数的调整次数，该宏用于smart params（智能连接参数管理），smart params过程
中如果对方拒绝了SYD8821请求的连接参数，那么SYD8821将会发送更加让对方容易接受的连接参数，如果调整次数
大于MAX_UPDATE_ADJ_NUM那么将结束连接参数的调整过程
**************************************************************************************************/
#define MAX_UPDATE_ADJ_NUM		4




/*************************************************************************************************
*枚举名：_BLE_ADDRESS_TYPE_
*成员： PUBLIC_ADDRESS_TYPE	公共地址类型
				RANDOM_ADDRESS_TYPE	随机地址类型
*说明： BLE的MAC地址类型有公共地址和随机地址两种，前者是制造商和SIG申请具有唯一性的地址，后者无需和
SIG申请，RANDOM_ADDRESS_TYPE又分为Static Device Address和Private Device Address
**************************************************************************************************/
enum _BLE_ADDRESS_TYPE_{
	PUBLIC_ADDRESS_TYPE	= 0x00,
	RANDOM_ADDRESS_TYPE 	= 0x01,
};




/*************************************************************************************************
*枚举名：_ADV_CH_PKT_TYPE_
*成员： ADV_IND	connectable undirected advertising event不定向可连接广播事件
				ADV_DIRECT_IND	connectable directed advertising event定向可连接广播事件
				ADV_NOCONN_IND	non-connectable undirected advertising event不可连接广播事件
				SCAN_REQ	扫描请求事件
				SCAN_RSP	扫描响应事件
				CONNECT_REQ	广播通道连接事件
				ADV_SCAN_IND	scannable undirected advertising event可扫描未定向广播事件，这是BLE4.2增加的
*说明： 该枚举定义了可能发生在BLE的广播通道上的事件，这里的事件更确切的说法应该是可能发生的行为，
引用不同的类型，低层就会发生相应的行为，连接状态之前的行为都是发生在广播通道上的，ADV_IND�
ADV_DIRECT_IND、ADV_NOCONN_IND、ADV_SCAN_IND属于BLE的广播事件，对应着相应的广播类型；SCAN_REQ和
SCAN_RSP属于BLE的扫描相关的事件，前者属于扫描请求后者为扫描响应；CONNECT_REQ为连接事件。其中SCAN_REQ
和CONNECT_REQ是对于SYD8821的master（主机）端才能够有的行为。
**************************************************************************************************/
enum _ADV_CH_PKT_TYPE_{
	ADV_IND 			= 0x00,
	ADV_DIRECT_IND 	= 0x01,
	ADV_NOCONN_IND	= 0x02,
	SCAN_REQ			= 0x03,
	SCAN_RSP			= 0x04,
	CONNECT_REQ		= 0x05,
	ADV_SCAN_IND		= 0x06,
};




/*************************************************************************************************
*枚举名：BLE_SEND_TYPE
*成员： BLE_GATT_NOTIFICATION 	Characteristic Value Notification.BLE消息类型为notify
				BLE_GATT_INDICATION		Characteristic Value Indications BLE消息类型为Indications
*说明： 对于SYD8821从机，Characteristic Value发送行为有两种，即Notification和Indications，前者是
SYD8821单单发送消息，并不需要等待master做出任何响应，而后者还需要等待master做出正确的响应才会进行
下一步发送
**************************************************************************************************/
enum BLE_SEND_TYPE {
	BLE_GATT_NOTIFICATION	= 0x0001,
	BLE_GATT_INDICATION		= 0x0002,
};



/*************************************************************************************************
*枚举名：_MCU_CLOCK_SEL_
*成员： MCU_CLOCK_16_MHZ 	MCU时钟为16Mhz
				MCU_CLOCK_20_MHZ	MCU时钟为20Mhz
				MCU_CLOCK_24_MHZ	MCU时钟为24Mhz
				MCU_CLOCK_64_MHZ	MCU时钟为64Mhz
				MCU_CLOCK_80_MHZ	MCU时钟为80Mhz
				MCU_CLOCK_96_MHZ	MCU时钟为96Mhz
*说明： SYD8821的MCU的时钟可以从该枚举中选取，并且上电的时候必须选择适当的MCU时钟，并且调用MCU的高频
RC振荡器校准函数：sys_mcu_rc_calibration，对于MCU的时钟，基本的原则是：时钟频率越高，相应的功耗也会
相应的增大（功耗并不会有成倍的增长，只是略微有些差异）。一般而言配置时钟之后会紧接着进行RC的校准。
				SYD8821的时钟设计从频率来分类有低频的32.768KHz的低频时钟，这是固定的频率，该时钟的时钟源
有内部RX振荡器和外部晶振两种以及内部32MHz的分频三种，低频的时钟主要供给低频的定时器和PWM使用；除了低
频时钟外，SYD8821芯片内部还有一个供MCU和高速的通讯接口（比如SPI、I2C等）使用的高速时钟振荡器，正常而
言，高速时钟的时钟源一般来自芯片内部的高频振荡器。
				SYD8821的高速时钟的时钟源只有内部的高速RC振荡器，所以在配置好后必须要经过校准的流程，但是对于
高速时钟而言，只需要在程序启动的时候进行一次校准即可！SYD8821的低频时钟源既有内部的（简称LPO）也有外部
的（简称XO)，如果使用内部的时钟源，那么必须得每隔一段时间（建议3分钟）进行一次内部的低频时钟的校准。
				进行高频RC时钟校准的时候必须先调用gap_s_ble_init函数初始化BLE协议栈！
**************************************************************************************************/
enum _MCU_CLOCK_SEL_{
	MCU_CLOCK_16_MHZ	= 0x00,
	MCU_CLOCK_20_MHZ	= 0x01,
	MCU_CLOCK_24_MHZ	= 0x02,
	MCU_CLOCK_64_MHZ	= 0x03,
	MCU_CLOCK_80_MHZ	= 0x04,
	MCU_CLOCK_96_MHZ	= 0x05,
};




/*************************************************************************************************
*枚举名：_32K_CLOCK_SEL_
*成员： SYSTEM_32K_CLOCK_LPO 	低频的32K时钟源为内部RC振荡器
				SYSTEM_32K_CLOCK_LPO 	低频的32K时钟源为外部晶振
				SYSTEM_32K_CLOCK_32M_DIV	低频的32K时钟源为外部晶振的分频，分频指数为977分频，最终的频率接近
																	32.768KHz，32000/977=32.7533Khz
*说明： SYD8821的低频时钟的频率固定为32.768KHz,主要是给低频的定时器和PWM已经BLE协议栈使用，低频时钟
源不准确的话将会对该时钟的消费者都有影响，比如BLE连接不上，定时器和PWM的时序不准确等。
				如果选择SYSTEM_32K_CLOCK_LPO为时钟源的话，必须要每隔一个固定的时间（建议3分钟）要校准一次，
因为如果RC振荡器会有温度漂移等特性，时间长后就会有偏差
**************************************************************************************************/
enum _32K_CLOCK_SEL_{
	SYSTEM_32K_CLOCK_LPO	= 0x00,
	SYSTEM_32K_CLOCK_XO	= 0x01,
	SYSTEM_32K_CLOCK_32M_DIV	= 0x02,
};




/*************************************************************************************************
*结构体名：gap_ble_addr
*成员： type 	BLE mac地址类型，为枚举_BLE_ADDRESS_TYPE_的成员
				addr 	BLE mac地址
*说明： SYD8821的设备地址可通过gap_s_ble_address_get函数来获取，通过gap_s_ble_address_set来设置
**************************************************************************************************/
struct gap_ble_addr {
	uint8_t	type;
	uint8_t	addr[BD_ADDR_SZ];
};




/*************************************************************************************************
*结构体名：gap_key_params
*成员： ediv 	BLE安全管理中的Encrypted Diversifier (EDIV)参数
				rand 	BLE安全管理中的Random Number (Rand)参数
				ltk 	BLE安全管理中的Long Term Key (LTK) 参数
				local_irk 	BLE安全管理中的本地Identity Resolving Key (IRK)参数
				peer_irk 	BLE安全管理中的对等设备Identity Resolving Key (IRK)参数
*说明： 在加密结束后交换密匙的时候SYD8821会上报enc_key_evt事件，该事件中包含有该结构体，包含了安全
管理中用到的各个参数
**************************************************************************************************/
struct gap_key_params{
	uint8_t	ediv[MAX_EDIV_SZ];
	uint8_t	rand[MAX_RAND_SZ];	
	uint8_t	ltk[MAX_KEY_SZ];
	uint8_t	local_irk[MAX_IRK_SZ];
	uint8_t	peer_irk[MAX_IRK_SZ];	
};




/*************************************************************************************************
*结构体名：gap_adv_params
*成员： type 	广播类型，为枚举_ADV_CH_PKT_TYPE_中的成员
				peer_addr	对等设备地址，该成员体现为定向广播中的InitA，只在定向广播有用
				policy	过滤策略，符合规范中提及的LINK LAYER DEVICE FILTERING  Policy
				channel	广播通道，该参数使用bitmask形式，37~39通道对应bit0~bit2,比如0x03代表在37、38通道广播
				interval	广播间隔，单位是0.625ms,比如0x640为1S的广播间隔
				timeout	广播超时时间，单位是1S，比如0x64为100S，最大值为0x3FFF，不能够传入0
				hop_interval	广播事件中通道和通道之间的间隔，比如广播通道为37,38和39，那么hop_interval即为
											一个广播事件中的37到38或者38到39的时间间隔
*说明： SYD8821的广播由广播参数和广播数据组成，广播参数控制具体的行为，广播数据为广播中具体的数据。
				广播间隔是基于功耗和连接灵敏度甚至连接稳定性的平衡，间隔也小连接越快，可能也越稳定，功耗也越高
广播间隔越大灵敏度越低，当然功耗就越低
				对于SYD8821，要求interval大于2ms,hop_interval<interval
**************************************************************************************************/
struct gap_adv_params {
	uint8_t				type;
	struct gap_ble_addr	peer_addr;
	uint8_t       policy;
	uint8_t				channel;
	uint16_t      interval; 		 /**< Between 0x0020 and 0x4000 in 0.625 ms units (20ms to 10.24s) */
	uint16_t      timeout;	 		 /**< Between 0x0001 and 0x3FFF in seconds, 0x0000 is disable */
	uint8_t       hop_interval;  /** 30.5us ~ 85583us in 335.5us units **/
};




/*************************************************************************************************
*结构体名：gap_scan_params
*成员： type 	扫描类型，有被动扫描和主动扫描
				interval	扫描间隔，单位是0.625ms,Time Range: 2.5 msec to 10.24 seconds
				window	扫描窗口，单位是0.625ms，Time Range: 2.5 msec to 10.24 seconds
*说明： 扫描间隔要大于扫描窗口，因为真正扫描发生在扫描窗口期间，所以有可能一个间隔内扫描不到广播的
情况，当SYD8821扫描到符合要求的广播的时候，协议栈将上报扫描响应事件！
**************************************************************************************************/
struct gap_scan_params {
	uint8_t	type;
	uint16_t	interval;
	uint16_t	window;
};




/*************************************************************************************************
*枚举名：_ADV_SCAN_MODE_
*成员： IDLE_MODE 	当前状态为空闲状态
		ADV_MODE 	当前状态为广播状态
		SCAN_MODE 	当前状态为扫描状态
		COEX_ADV_MODE 	当前状态为连接状态下的广播状态
		COEX_SCAN_MODE 	当前状态为连接状态下的扫描状态
*说明： 在广播通道上的行为既有可能是广播也有可能是扫描，而如果蓝牙处于连接状态下，广播和扫描又不一样！
**************************************************************************************************/
enum _ADV_SCAN_MODE_{
	IDLE_MODE			= 0x00,
	ADV_MODE			= 0x01,
	SCAN_MODE			= 0x02,
	COEX_ADV_MODE		= 0x03,
	COEX_SCAN_MODE	= 0x04,
};





/*************************************************************************************************
*结构体名：gap_profile_struct
*成员： report_handle_address 	report_handle的指针，指向代码区域中的_gatt_database_report_handle
				primary_address	主要服务指针，指向代码中的_gatt_database_primary
				include_address	次要服务指针，指向代码中的_gatt_database_include
				characteristic_address	特性指针，指向代码中的_gatt_database_characteristic
				value_address	特性值指针，指向代码中的_gatt_database_value
*说明： 相对于代码资源有点紧张的SYD8801，SYD8821的代码资源足够充足，所以这里直接把BLE的profile直接
放到代码区域中，这样原来获取profile是通过读取flash的方式，现在是通过直接代码指针的方式去获取，速度
和可修改性都有好处。主要服务、特性、特性值等通过工具《BtGatt.exe》获取
				gap_profile_struct囊括了整个profile的各个元素
**************************************************************************************************/
struct gap_profile_struct {
	uint32_t report_handle_address;
	uint32_t primary_address;
	uint32_t include_address;
	uint32_t characteristic_address;
	uint32_t value_address;
};





/*************************************************************************************************
*枚举名：GAP_IO_CAPABILITY
*成员： 
																		 \Local output |										|
													Local input \capacity    |   No output        |  Numeric output
													capacity		 \           |                    |
													-------------------------|--------------------|-------------------
													No input                 | NoInputNoOutput    |   DisplayOnly
													-------------------------|--------------------|-------------------
													Yes/No                   |  NoInputNoOutput		|		DisplayYesNo 
													-------------------------|--------------------|-------------------
													Keyboard 								 |	 KeyboardOnly 		|			KeyboardDisplay
*说明： 该枚举代表本机支持的加密的IO能力（能够以何种方式输入密码）	
**************************************************************************************************/
enum GAP_IO_CAPABILITY {
	IO_DISPLAY_ONLY		  = 0x00,
	IO_DISPLAY_YESNO		= 0x01,
	IO_KEYBOARD_ONLY		= 0x02,
	IO_NO_INPUT_OUTPUT	= 0x03,	
	IO_KEYBOARD_DISPLAY	= 0x04,	
};



/*************************************************************************************************
*枚举名：GAP_OOB_FLAG
*成员： OOB_AUTH_NOT_PRESENT	没有ODB功能
				OOB_AUTH_PRESENT	需要ODB功能
*说明： 所谓的ODB功能就是以带外的通讯方式传输密匙，比如BLE的密匙通过串口发送，这样单单以BLE的通讯
（BLE抓包工具）来破解BLE就不可能成功了
**************************************************************************************************/
enum GAP_OOB_FLAG {
	OOB_AUTH_NOT_PRESENT= 0x00,
	OOB_AUTH_PRESENT		= 0x01,
};



/*************************************************************************************************
*枚举名：GAP_BONDING_FLAGS
*成员： AUTHREQ_NO_BONDING	加密完成后不需要绑定
				AUTHREQ_BONDING	加密完成后需要绑定
*说明： 如果设置了绑定功能，再次连接的时候就不需要再次进行配对的过程了，配对信息保存在芯片内部的flash
**************************************************************************************************/
enum GAP_BONDING_FLAGS {
	AUTHREQ_NO_BONDING	= 0x00,
	AUTHREQ_BONDING		  = 0x01,
};




/*************************************************************************************************
*枚举名：GAP_KEY_DISTRIBUTION
*成员：  GAP_KEY_MASTER_IDEN	密匙分配的时候是否交换EDIV
	     GAP_KEY_ADDR_INFO	密匙分配的时候是否交换ADDR
	     GAP_KEY_SIGNIN_INFO	密匙分配的时候是否交换SIGNIN信息
*说明： 配对完成后主从双方在需要的时候会交换一些信息，包括IDEN和ADDR以及SIGNIN，这些信息是可选的，
另外的LTK等是必须要的。
**************************************************************************************************/
enum GAP_KEY_DISTRIBUTION {
	GAP_KEY_MASTER_IDEN	= 0x01,
	GAP_KEY_ADDR_INFO	  = 0x02,
	GAP_KEY_SIGNIN_INFO	= 0x04,
};




/*************************************************************************************************
*结构体名：gap_pairing_req
*成员： io	配对过程中的IO能力，为枚举GAP_IO_CAPABILITY成员
				oob	配过过程中是否需要ODB，为枚举GAP_OOB_FLAG成员
				flags 配对过程中是否需要绑定
							Bonding_Flags |
							b1b0          |   Bonding Type
							--------------|------------------
							00            |   No Bonding
							01 						|	 Bonding
							10 						|   Reserved
							11            |   Reserved
				mitm	配对过程中是否需要中间人保护set to one if the device is requesting	MITM protection, 
							otherwise it shall be set to 0
				sc	配对过程中是否使用安全连接配对，这是BLE4.2增加的， set to one to request LE Secure 
Connection pairing, othe rwise it shall be set to 0 based on the supported features of the 
initiator and responder
				keypress	 配对过程中是否使用Passkey Entry 
				rsvd	保留位
				max_enc_sz	配对过程中最大的密匙长度，The maximum key size shall be in the range 7 to 16 octets.
				init_key	配对过程中初始者（主机）分配的密匙
				rsp_key	配对过程中从机分配的密匙
*说明： SYD8821的安全参数在启动的时候调用api:gap_s_security_parameters_set进行设置，在配对开始的时候
调用api:gap_s_security_req开始配对流程，其中gap_s_security_req函数也有flags和mitm两个参数，这两个参数
和安全参数里的参数有冲突，这里优先使用调用gap_s_security_req传入的函数
**************************************************************************************************/
struct gap_pairing_req{
	uint8_t io;
	uint8_t oob;
	uint8_t flags:2;
	uint8_t mitm:1;
	uint8_t sc:1;
	uint8_t keypress:1;
	uint8_t rsvd:3;
	uint8_t max_enc_sz;
	uint8_t init_key;
	uint8_t rsp_key;
};




/*************************************************************************************************
*结构体名：gap_connection_param_rsp_pdu
*成员： Interval_Min	连接参数中的连接间隔最小值
				Interval_Max	连接参数中的连接间隔最大值
				Latency	连接参数中的latency
				Timeout	连接参数中的Timeout
				PeferredPeriodicity	连接参数中的PreferredPeriodicity
				ReferenceConnEventCount 连接参数中的ReferenceConnEventCount
				Offset 连接参数中的Offset数组
*说明： BLE4.2增加了LL_CONNECTION_PARAM_REQ和LL_CONNECTION_PARAM_RSP两个LL层的数据包，对连接参数做
了更大的完善，两者的参数是一样的。
				gap_connection_param_rsp_pdu结构体的具体数据最后会用在LL_CONNECTION_PARAM_RSP命令上，如果对
方式BLE4.0或者不支持LL_CONNECTION_PARAM的命令，该结构体是无用的
				建议在协议栈初始化函数中调用api:gap_s_connection_param_set进行设置
**************************************************************************************************/
struct gap_connection_param_rsp_pdu {
	uint16_t Interval_Min;
	uint16_t Interval_Max;
	uint16_t Latency;
	uint16_t Timeout;
	uint8_t PeferredPeriodicity;
	uint16_t ReferenceConnEventCount;
	uint16_t Offset[LL_WIN_OFFSET_SZ];	
};




/*************************************************************************************************
*结构体名：gap_update_params
*成员： updateitv_min	连接参数更新中的连接间隔最小值
				updateitv_max	连接参数更新中的连接间隔最大值
				updatelatency	连接参数更新中的latency
				updatesvto	连接参数更新中的超时时间
*说明： SYD8821的连接参数使用api:gap_s_connection_update进行设置，这里更确切的是连接参数更新，BLE中
有说明BLE的从机没有直接发送LL层的LL_CONNECTION_UPDATE_REQ命令，只能够通过L2CAP的信令通道发送
CONNECTION PARAMETER UPDATE REQUEST命令更新连接参数，该结构体为CONNECTION PARAMETER UPDATE REQUEST
命令中的各个参数
**************************************************************************************************/
struct gap_update_params {
	uint16_t  updateitv_min;
	uint16_t  updateitv_max;
	uint16_t  updatelatency;
	uint16_t  updatesvto;
};




/*************************************************************************************************
*枚举名：GAP_SMART_CONTROL_SET
*成员： SMART_CONTROL_LATENCY	使能latency自动控制机制
				SMART_CONTROL_UPDATE	使能连接参数自动控制
*说明： 该枚举使用bitmask的形，该枚举只用于smart_update_latency，可以使用或的形式传入，比如可传入：
				SMART_CONTROL_LATENCY|SMART_CONTROL_UPDATE
**************************************************************************************************/
enum GAP_SMART_CONTROL_SET {
	SMART_CONTROL_LATENCY	= 0x80,
	SMART_CONTROL_UPDATE	= 0x40,
};




/*************************************************************************************************
*结构体名：gap_smart_update_params
*成员： updatectrl	设置smart_update_latency的管理范围，为枚举GAP_SMART_CONTROL_SET的成员或值
				updateadj_num	设置smart_update_latency的调整次数，一般传入本文件的宏：MAX_UPDATE_ADJ_NUM
				updateitv_target	设置目标连接参数间隔，smart_update_latency会让BLE的连接参数最大的接近
													该参数
				updatelatency	设置连接参数的latency，smart_update_latency会直接把该参数用于连接参数请求中
				updatesvto 设置连接参数的超时时间，smart_update_latency会直接把该参数用于连接参数请求中
*说明： 该结构体只用于smart_update_latency，协议栈会根据该结构体自动管理连接参数，经过调整后最终连接
参数可能并不能够完全该结构体的要求，但是会最大程度的接近该结构体的参数
**************************************************************************************************/
struct gap_smart_update_params {
	uint8_t 	updatectrl;
	uint8_t 	updateadj_num;
	uint16_t  updateitv_target;
	uint16_t  updatelatency;
	uint16_t  updatesvto;
};




/*************************************************************************************************
*结构体名：gap_link_params
*成员： interval	目前链路层的连接参数间隔
				latency		目前链路层的连接参数的latency
				svto	目前链路层的连接参数的超时时间
*说明： SYD8821的协议栈保存有当前的连接参数，可通过gap_s_link_parameters_get函数获取，该参数会在收到
LL层的LL_CONNECTION_UPDATE_REQ命令的时候将会被更新
**************************************************************************************************/
struct gap_link_params {	
	uint16_t	interval;
	uint16_t	latency;
	uint16_t	svto;
};




/*************************************************************************************************
*枚举名：_GAP_EVT_
*成员： GAP_EVT_ADV_END	蓝牙广播完成事件，当广播参数中的超时时间到来后协议栈上报该事件
				GAP_EVT_ADV_REPORT		蓝牙扫描事件，
				GAP_EVT_CONNECTED	蓝牙连接事件，当连接成功的时候协议栈上报该事件，根据规范的说明：蓝牙连接
													成功的标志并不是收到主机发送CONNECT_REQ的时刻，而是在接收到CONNECT_REQ命
													令后还要交互一次数据包才算上真正的连接成功
				GAP_EVT_DISCONNECTED	蓝牙断线事件，协议栈会同时上报断线的原因，符合HCI的断线原因
				GAP_EVT_ENC_KEY	蓝牙配对完成事件，BLE配对的最后会进行主从密匙的交换，交换完成后上报该事件
				GAP_EVT_PASSKEY_REQ	蓝牙密码输入事件，如果BLE使用密码加密，在需要输入密码的时候上报该事件
														请求输入密码
				GAP_EVT_ENC_START	蓝牙开始加密事件，配对完成或者绑定重连后主机会要求开始加密
				GAP_EVT_CONNECTION_EVENT	连接间隔时刻事件，发生在连接事件结束的时候上报
				GAP_EVT_CONNECTION_UPDATE_RSP	连接参数更新响应事件，SYD8821从机发送CONNECTION PARAMETER 
																		  UPDATE REQUEST后主机会做出响应，收到响应后协议栈上报该事件
				GAP_EVT_ATT_READ	主机对SYD8821发送了读操作，协议栈将上报该事件
				GAP_EVT_ATT_WRITE	主机对SYD8821发送了写操作，协议栈将上报该事件
				GAP_EVT_ATT_PREPARE_WRITE	主机对SYD8821发送了预备写操作，协议栈将上报该事件
				GAP_EVT_ATT_EXECUTE_WRITE	主机对SYD8821发送了执行写操作，协议栈将上报该事件
				GAP_EVT_ATT_HANDLE_CONFIRMATION	蓝牙在收到指示确认时上报该事件，标志着指示操作完成
				GAP_EVT_ATT_HANDLE_CONFIGURE	蓝牙在收到配置CCCD也就是主机使能notify或者指示功能的时候上报
																			该事件，代表主机要开启相应功能
		    GAP_EVT_ADV_START  广播开始事件，在开始广播或者重新开始广播时调用
		    GAP_EVT_CONN_UPDATE_INSTANT_PASS  GAP_EVT_CONN_UPDATE_INSTANT_PASS 是 update成功的 evt，没有
		                                  带参数回调
*说明： SYD8821协议栈初始化的时候要调用api:gap_s_evt_handler_set设置gap_evt_callback结构体，该结构体
包含了协议栈上报BLE事件的接口，SYD8821的协议栈在发现BLE的状态发生变化（比如发生了连接或者断线）的时
候，协议栈将会往APP层（用户代码）上报BLE事件，同时还会上报gap_ble_evt结构体，该结构体中的evt_code变
量就是_GAP_EVT_枚举定义的成员
**************************************************************************************************/
enum _GAP_EVT_{
	GAP_EVT_ADV_END					= 0x0001,
	GAP_EVT_ADV_REPORT				= 0x0002,
	GAP_EVT_CONNECTED				= 0x0004,
	GAP_EVT_DISCONNECTED			= 0x0008,
	GAP_EVT_ENC_KEY					= 0x0010,
	GAP_EVT_PASSKEY_REQ				= 0x0020,
	GAP_EVT_SHOW_PASSKEY_REQ		= 0x0040,
	GAP_EVT_ENC_START				= 0x0080,
	GAP_EVT_CONNECTION_EVENT		= 0x0100,
	GAP_EVT_CONNECTION_UPDATE_RSP	= 0x0200,
	GAP_EVT_ATT_READ				= 0x0400,
	GAP_EVT_ATT_WRITE				= 0x0800,
	GAP_EVT_ATT_PREPARE_WRITE	    = 0x1000,
	GAP_EVT_ATT_EXECUTE_WRITE	    = 0x2000,
	GAP_EVT_ATT_HANDLE_CONFIRMATION	= 0x4000,
	GAP_EVT_ATT_HANDLE_CONFIGURE	= 0x8000,
	GAP_EVT_ADV_START                   = 1UL << 16,
	GAP_EVT_CONN_UPDATE_INSTANT_PASS    = 1UL << 17,
	GAP_EVT_SHOW_NUMERIC_COMPARISON    	= 1UL << 18,
	GAP_EVT_WARNING_REPORT				= 1UL << 19,
};




/*************************************************************************************************
*枚举名：CONNECTION_UPDATE_RSP_
*成员： CONN_PARAM_ACCEPTED	连接参数更新被主机接收
				CONN_PARAM_REJECTED	连接参数更新被主机拒绝
				CONN_PARAM_SMART_TIMEROUT	智能连接参数管理调整超时事件，该类型的事件并不是标准的BLE事件，而是
																	随着smart_update功能而添加的，不使用smart_update功能不会上报该事件
				CONN_PARAM_SMART_SUCCEED	智能连接参数管理调整成功事件，该类型的事件并不是标准的BLE事件，而是
																	随着smart_update功能而添加的，不使用smart_update功能不会上报该事件
				CONN_PARAM_LATENCY_ENABLE	智能连接参数管理使能latency，协议栈使能latency的时候上报该事件，
																	该类型的事件并不是标准的BLE事件，而是随着smart_update功能而添加的，
																	不使用smart_update功能不会上报该事件
				CONN_PARAM_LATENCY_DISABLE	智能连接参数管理失能latency，协议栈使能latency的时候上报该事件，
																	该类型的事件并不是标准的BLE事件，而是随着smart_update功能而添加的，
																	不使用smart_update功能不会上报该事件
*说明： BLE规范中只是定义了CONN_PARAM_ACCEPTED和CONN_PARAM_REJECTED两个事件，因为smart_update_latency
机制也需要上报一些事件给应用层，所以这里在CONNECTION_UPDATE_RSP中增加几个上报事件，增加的这些事件如果
不使用smart_update功能不会上报该事件
**************************************************************************************************/
enum CONNECTION_UPDATE_RSP_ {
	CONN_PARAM_ACCEPTED	= 0x0000,
	CONN_PARAM_REJECTED	= 0x0001,
	CONN_PARAM_SMART_TIMEROUT	= 0x0002,	
	CONN_PARAM_SMART_SUCCEED	= 0x0003,	
	CONN_PARAM_LATENCY_ENABLE	= 0x0004, 
	CONN_PARAM_LATENCY_DISABLE	= 0x0005, 
};




/*************************************************************************************************
*结构体名：gap_disconnected_evt
*成员： reason	断线原因，该原因符合HCI的ERROR CODE
*说明： BLE断线的原因有很多，SYD8821在发生断线的时候会把断线的原因上报上来，该原因可作为参考，具体
的缘由可看规范的第二章的Part D ERROR CODES的第二节ERROR CODE DESCRIPTIONS
**************************************************************************************************/
struct gap_disconnected_evt {
	uint8_t	reason;
};




/*************************************************************************************************
*结构体名：gap_att_read_evt
*成员： primary	BLE读事件的主服务的UUID
				uuid	BLE读事件的特性UUID
				hdl	BLE读事件的特性的hdl(句柄)
				offset	BLE读事件的偏移值，该项在某些读操作是无效的
*说明： BLE读操作的对象是BLE的特性值，应用层既可以通过UUID来判断对方想读取的具体特性值，也可以通过
特性的句柄来判断具体的特性，在同一个profile中句柄是唯一的，但是可能不同的profile即使uuid相同的特性
他们的句柄也可能有差异。在任何一个profile，特性的UUID是一致的，但是在同一个profile中可能会有几个相
同UUID的特性，这时候就得通过hdl来判断hdl,对于复杂的profile，可配合使用uuid和hdl
**************************************************************************************************/
struct gap_att_read_evt {
	uint16_t	primary;
	uint16_t	uuid;
	uint16_t	hdl;
	uint16_t	offset;
};



/*************************************************************************************************
*结构体名：gap_att_write_evt
*成员： primary	BLE写事件的主服务的UUID
				uuid	BLE写事件的特性UUID
				hdl	BLE写事件的特性的hdl(句柄)
				sz	BLE写数据的大小
				data	BLE写的具体数据
*说明：SYD8821的MAX_ATT_DATA_SZ（MTU)是512个byte，这些数据是放到堆栈中的，这就增加堆栈溢出的可能性，
所以不同的工程，不同的需求，可适当配置SYD8821的堆栈
		BLE写操作的对象是BLE的特性值，应用层既可以通过UUID来判断对方想读取的具体特性值，也可以通过
特性的句柄来判断具体的特性，在同一个profile中句柄是唯一的，但是可能不同的profile即使uuid相同的特性
他们的句柄也可能有差异。在任何一个profile，特性的UUID是一致的，但是在同一个profile中可能会有几个相
同UUID的特性，这时候就得通过hdl来判断hdl,对于复杂的profile，可配合使用uuid和hdl
**************************************************************************************************/
struct gap_att_write_evt {
	uint16_t	primary;
	uint16_t	uuid;
	uint16_t	hdl;
	uint8_t	sz;
	uint8_t	data[MAX_ATT_DATA_SZ];
};




/*************************************************************************************************
*结构体名：gap_att_pre_write_evt
*成员： primary	BLE预备写事件的主服务的UUID
				uuid	BLE预备写事件的特性UUID
				hdl	BLE预备写事件的特性的hdl(句柄)
				sz	BLE预备写数据的大小
				data	BLE预备写的具体数据
*说明：BLE的预备写和执行写是配套使用的，要写入大批量的数据的时候可能对方会先通过预备写命令把数据分段
发送给SYD8821，然后通过执行写来把输入写入特性值，这样保证写入数据的原子性
		SYD8821的MAX_ATT_DATA_SZ（MTU)是512个byte，这些数据是放到堆栈中的，这就增加堆栈溢出的可能性，
所以不同的工程，不同的需求，可适当配置SYD8821的堆栈
		BLE预备写操作的对象是BLE的特性值，应用层既可以通过UUID来判断对方想读取的具体特性值，也可以通过
特性的句柄来判断具体的特性，在同一个profile中句柄是唯一的，但是可能不同的profile即使uuid相同的特性
他们的句柄也可能有差异。在任何一个profile，特性的UUID是一致的，但是在同一个profile中可能会有几个相
同UUID的特性，这时候就得通过hdl来判断hdl,对于复杂的profile，可配合使用uuid和hdl
**************************************************************************************************/
struct gap_att_pre_write_evt {
	uint16_t	primary;
	uint16_t	uuid;
	uint16_t	hdl;
	uint16_t	offset;
	uint8_t	sz;
	uint8_t	data[MAX_ATT_DATA_SZ];
};



/*************************************************************************************************
*结构体名：gap_att_exec_write_evt
*成员： flags	0x00:Cancel all prepared writes 0x01:Immediately write all pending prepared values
*说明： 当预备写结束后会发送执行写来执行特性值写入的操作，该命令带有一个参数，表示是否写入之前的
数据
**************************************************************************************************/
struct gap_att_exec_write_evt {
	uint8_t	flags;
};




/*************************************************************************************************
*结构体名：gap_att_handle_configure_evt
*成员： uuid	BLE配置CCCD事件的服务的UUID
				hdl	BLE配置CCCD事件的描述符的hdl(句柄)
				value	对CCCD的行为，这个变量是bitmask的形式，为枚举BLE_SEND_TYPE的成员或者其成员的或值
*说明： 根据规范，BLE的notify(通知）或者indicate（指示）的使能或者失能都是由主机也就是master控制的，
当主机对SYD8821的notify或者indicate的开关进行操作的时候，底层协议栈会上报该事件
				handle_configure操作的对象是BLE的描述符，应用层既可以通过UUID来判断对方想读取的具体的服务
描述符，当时这种方式对于一个服务下面有多个带有CCCD的描述符，这种方式就不可行了，这种情况下只能够通
过描述符的句柄来判断具体的描述符,对于复杂的profile，可配合使用uuid和hdl
				一个特性可能存在notify和indicate两种，所以这里得通过判断具体的位来判断是否使能或者失能。比如：
如果value=0x03，那就是同时使能notify和indicate，value=0x01则单单使能notify
**************************************************************************************************/
struct gap_att_handle_configure_evt {
	uint16_t	uuid;
	uint16_t	hdl;
	uint16_t	value;
};




/*************************************************************************************************
*结构体名：gap_connection_update_rsp_evt
*成员： result	BLE连接参数更新结果 ，为枚举CONNECTION_UPDATE_RSP_的成员
*说明： SYD8821收到CONNECTION PARAMETER UPDATE RESPONSE响应会上报GAP_EVT_CONNECTION_UPDATE_RSP事件，
伴随该事件会带着该结构体的参数，可通过该参数判断主机是否同意连接参数更新
				BLE规范中只是定义了CONN_PARAM_ACCEPTED和CONN_PARAM_REJECTED两个事件，因为smart_update_latency
机制也需要上报一些事件给应用层，所以这里在CONNECTION_UPDATE_RSP中增加几个上报事件，增加的这些事件如果
不使用smart_update功能不会上报该事件
**************************************************************************************************/
struct gap_connection_update_rsp_evt {
	uint16_t result;
};




/*************************************************************************************************
*结构体名：gap_advertising_report_evt
*成员： type	扫描上报事件的类型
				peer_dev_addr	广播者地址
				len	扫描到的广播数据的长度
				buf	扫描到的广播的数据
				rssi	扫描到的广播的信号强度
*说明： SYD8821在扫描的时候如果收到正确的广播信号会上报相应的事件，该事件包含有该结构体，不过有些
成员在不同的广播类型下是没有的
**************************************************************************************************/
struct gap_advertising_report_evt {
	uint8_t type;
	struct gap_ble_addr	peer_dev_addr;
	int8_t	len;
	uint8_t	buf[MAX_ADV_DATA_SZ];
	uint8_t	rssi;
};




/*************************************************************************************************
*结构体名：gap_att_handle_confirmation_evt
*成员： primary	BLE指示确认事件的主服务的UUID
				uuid	BLE指示确认事件的特性UUID
				hdl	BLE指示确认事件的特性的特性值的hdl(句柄)
*说明： BLE的指示的步骤比通知功能多了一个返回的过程，当SYD8801收到主机返回来的响应时上报
GAP_EVT_ATT_HANDLE_CONFIRMATION事件，该事件包含该结构体。
				如果按照规范，GAP_EVT_ATT_HANDLE_CONFIRMATION响应不会有任何的数据，所以该结构体的内容是保存
在链路层的缓冲区中，由APP代码在发送指示的时候传给协议栈的				
**************************************************************************************************/
struct gap_att_handle_confirmation_evt {
	uint16_t	primary;
	uint16_t	uuid;
	uint16_t	hdl;
};



/*************************************************************************************************
*结构体名：gap_ble_evt
*成员： evt_type	协议栈上报事件的类型，对于APP只会上报GAP事件，所以只会GAP事件
				evt_code	协议栈上报事件的标识码（code)
				evt	协议栈上报事件的具体内容，是一个联合体，包含有各种事件的数据
					disconn_evt	断线事件数据
					bond_dev_evt	绑定设备的地址，该数据在连线的时候会上报该结构体，指示对方的设备地址
					enc_key_evt	密匙分配事件数据，包含了配对过程的各个密匙
					att_read_evt	BLE读操作事件，指示具体的要读取的对象，UUID,hdl等
					att_write_evt	BLE写操作事件，指示具体的要操作的对象和相应的数据，UUID,hdl以及数据等
					att_pre_write_evt	BLE预备写操作事件，指示具体的要操作的对象和相应的数据，UUID,hdl以及数据等
					att_exec_write_evt	BLE执行写操作事件，指示具体的要操作的对象和相应的数据，UUID,hdl以及数据等
					att_handle_config_evt	BLE开关notify(通知）或者indicate（指示）事件，
					att_handle_confirmation_evt	BLE指示功能确认响应事件，包含链路层的数据
					connection_update_rsp_evt	BLE连接参数更新响应事件具体数据
					advertising_report_evt	BLE扫描上报事件
*说明： SYD8821在收到蓝牙事件的时候通过p_callback上报蓝牙事件，gap_ble_evt结构体为蓝牙事件的具体
数据
**************************************************************************************************/
struct gap_ble_evt {
	uint8_t	evt_type;
	uint32_t	evt_code;
	union
	{
		struct gap_disconnected_evt 		disconn_evt;
		struct gap_ble_addr				bond_dev_evt;
		struct gap_key_params			enc_key_evt;
		struct gap_att_read_evt			att_read_evt;
		struct gap_att_write_evt			att_write_evt;
		struct gap_att_write_evt			att_pre_write_evt;
		struct gap_att_write_evt			att_exec_write_evt;
		struct gap_att_handle_configure_evt	att_handle_config_evt;
		struct gap_att_handle_confirmation_evt	att_handle_confirmation_evt;
		struct gap_connection_update_rsp_evt connection_update_rsp_evt;
		struct gap_advertising_report_evt 	advertising_report_evt;
	} evt; 
};





/*************************************************************************************************
*结构体名：gap_evt_callback
*成员： evt_mask	事件类型屏蔽位，该变量使用bitmask形式，为枚举_GAP_EVT_的值或者其值的或值
				p_callback	BLE事件上报的构造函数，为一个函数指针
*说明： SYD8821在收到蓝牙事件的时候通过p_callback上报蓝牙事件，gap_ble_evt结构体为蓝牙事件的具体
数据
**************************************************************************************************/
struct gap_evt_callback {
	uint32_t	evt_mask;
	void 	(*p_callback)(struct gap_ble_evt *p_evt);
};




/*************************************************************************************************
*结构体名：gap_att_report
*成员： primary	BLE报告结构体的主服务的UUID
				uuid	BLE报告结构体的特性UUID
				hdl	BLE报告结构体的特性的特征值hdl(句柄 val_hdl)
				config	BLE报告结构体的特性的CCCD描述符的handle(句柄)
				value 具体的行为，有notify和indicate两种
*说明： SYD8821的profile把notify和indicate两种GATT service行为集合称为report，专门设立结构体
gap_att_report_handle保存了report的相应属性信息，每个CCCD对应着结构体gap_att_report_handle中的一个
数组成员。包括本CCCD所在的位置（其主要服务，特性，val_hdl）等
**************************************************************************************************/
struct gap_att_report {
	uint16_t	primary;
	uint16_t	uuid;
	uint16_t	hdl;
	uint16_t	config;
	uint16_t	value;
};




/*************************************************************************************************
*结构体名：att_err_rsp
*成员： opcode	ATT错误响应中的Request Opcode In Error参数
				hdl	ATT错误响应中的Attribute Handle In Error参数
				err	ATT错误响应中的Error Code
*说明：	BLE在发现请求条件出错的时候回应Error Response事件，该ATT命令中包含有错误的原因或者hdl等
**************************************************************************************************/
struct att_err_rsp{
	uint8_t opcode;
	uint16_t hdl;
	uint8_t err;
};



/*************************************************************************************************
*结构体名：att_find_by_type_val_req
*成员： start_hdl		ATT客户端Find By Type Value Request命令的Starting Handle参数
				end_hdl		ATT客户端Find By Type Value Request命令的Ending Handle参数
				att_type	 ATT客户端Find By Type Value Request命令的Attribute Type参数
				att_val		ATT客户端Find By Type Value Request命令的Attribute Value参数
*说明：	本结构体对应着Find By Type Value Request的各个参数
				该结构体用于GATT客户端，对于GATT的角色为服务器，那么该结构体是无用的。SYD8821只是提供ATT客户
端相应的API，并没有提供GAP的处理流程，以留给APP最大的灵活性
**************************************************************************************************/
struct att_find_by_type_val_req{
	uint16_t start_hdl;
	uint16_t end_hdl;
	uint16_t att_type;
	uint8_t att_val[MAX_ATT_DATA_SZ-7];
};




/*************************************************************************************************
*结构体名：att_mtu_rsp
*成员： mtu		ATT层的Exchange MTU Response命令的Server Rx MTU参数
*说明：	本结构体对应着Exchange MTU Response的各个参数
				该结构体用于GATT客户端，对于GATT的角色为服务器，那么改结构体是无用的。SYD8821只是提供ATT客户
端相应的API，并没有提供GAP的处理流程，以留给APP最大的灵活性
**************************************************************************************************/
struct att_mtu_rsp{
	uint16_t mtu;
};




/*************************************************************************************************
*结构体名：att_find_info_128
*成员： hdl		Handle值
				uuid		16bit的UUID值
*说明：	Find Information Response命令的format为0x01时，其Information Data为本结构体
**************************************************************************************************/
struct att_find_info_16{
	uint16_t hdl;
	uint8_t uuid[2];
};



/*************************************************************************************************
*结构体名：att_find_info_128
*成员： hdl		Handle值
				uuid		128bit的UUID值
*说明：	Find Information Response命令的format为0x02时，其Information Data为本结构体
**************************************************************************************************/
struct att_find_info_128{
	uint16_t hdl;
	uint8_t uuid[16];
};




/*************************************************************************************************
*联合体名：att_find_info_payload
*成员： uuid16		16bituuid下的Information Data参数
				uuid128		128bituuid下的Information Data参数
*说明：	本联合体对应着Find Information Response的Information Data参数，根据规范因为format不一样
Information Data有可能是handle和16bit uuid的组合，也有可能是handle和128bit uuid的组合
**************************************************************************************************/
union  att_find_info_payload {
	struct att_find_info_16   uuid16[5];
	struct att_find_info_128 uuid128;
};



/*************************************************************************************************
*结构体名：att_find_info_rsp
*成员： format		该参数决定了Information Data的长度
				pair		Information Data参数
*说明：	该结构体用于GATT客户端，对于GATT的角色为服务器，那么该结构体是无用的。SYD8821只是提供ATT客户
端相应的API，并没有提供GAP的处理流程，以留给APP最大的灵活性
**************************************************************************************************/
struct att_find_info_rsp{
	uint8_t format;
	union att_find_info_payload pair;
};



/*************************************************************************************************
*结构体名：att_find_by_type_val_rsp
*成员： list		Handles Information List参数
*说明：	本结构体对应着Find By Type Value Response的参数，根据规范list是一个Found Attribute Handle和
Group End Handle的组合
				该结构体用于GATT客户端，对于GATT的角色为服务器，那么该结构体是无用的。SYD8821只是提供ATT客户
端相应的API，并没有提供GAP的处理流程，以留给APP最大的灵活性
**************************************************************************************************/
struct att_find_by_type_val_rsp{
	uint8_t list[MAX_ATT_DATA_SZ-1];
};




/*************************************************************************************************
*结构体名：att_read_by_type_16
*成员： hdl		Attribute Data List参数中的Attribute Handle参数
				property		Attribute Data List参数中的Attribute Value参数，在发现characteristic时
										Attribute Value中的Characteristic Properties
				val_hdl		Attribute Data List参数中的Attribute Value参数在发现characteristic时Attribute Value
									中的Characteristic Value Attribute Handle
				char_uuid		Attribute Data List参数中的Attribute Value参数在发现characteristic时Attribute Value
										中的Characteristic UUID
*说明：	本结构体对应着Read By Type Response参数中Attribute Data List的参数
**************************************************************************************************/
struct att_read_by_type_16{
	uint16_t hdl;
	uint8_t property;
	uint16_t val_hdl;
	uint8_t char_uuid[2];
};




/*************************************************************************************************
*结构体名：att_read_by_type_128
*成员： hdl		Attribute Data List参数中的Attribute Handle参数
				property		Attribute Data List参数中的Attribute Value参数，在发现characteristic时
										Attribute Value中的Characteristic Properties
				val_hdl		Attribute Data List参数中的Attribute Value参数在发现characteristic时Attribute Value
									中的Characteristic Value Attribute Handle
				char_uuid		Attribute Data List参数中的Attribute Value参数在发现characteristic时Attribute Value
										中的Characteristic UUID
*说明：	本结构体对应着Read By Type Response参数中Attribute Data List的参数
**************************************************************************************************/
struct att_read_by_type_128{
	uint16_t hdl;
	uint8_t property;
	uint16_t val_hdl;
	uint8_t char_uuid[16];
};




/*************************************************************************************************
*联合体名：att_read_by_type_payload
*成员： uuid16		16bituuid下的Attribute Data List参数
				uuid128		128bituuid下的Attribute Data List参数
*说明：	Read By Type Response中的Length决定了Attribute Data List的长度，区别16bit uuid和1258bit uuid
				本结构只是用于操作对象是characteristic的情况
**************************************************************************************************/
union  att_read_by_type_payload {
	struct att_read_by_type_16   uuid16[3];
	struct att_read_by_type_128 uuid128;
};




/*************************************************************************************************
*联合体名：att_read_by_type_rsp
*成员： length		ATT层的Read By Type Response命令下的Length参数
				pair		ATT层的Read By Type Response命令下的Attribute Data List参数
*说明：	本结构体对应着Read By Type Response的参数，根据规范因为length不一样，其中length决定了
Attribute Data List的长度
				Attribute Data List有可能是handle和16bit uuid的characteristic组合，也有可能是handle和128bit uuid的
characteristic组合
				本结构只是用于操作对象是characteristic的情况
				该结构体用于GATT客户端，对于GATT的角色为服务器，那么该结构体是无用的。SYD8821只是提供ATT客户
端相应的API，并没有提供GAP的处理流程，以留给APP最大的灵活性
**************************************************************************************************/
struct att_read_by_type_rsp{
	uint8_t length;
	union att_read_by_type_payload pair;
};




/*************************************************************************************************
*联合体名：att_read_by_type_include_rsp
*成员： length		ATT层的Read By Type Response命令下的Length参数
				hdl		ATT层的Read By Type Response命令下的Attribute Data List参数中的Attribute Handle
				buf		ATT层的Read By Type Response命令下的Attribute Data List参数中的Attribute Value
*说明：	本结构体对应着Read By Type Response的参数，根据规范因为length不一样，其中length决定了
Attribute Data List的长度
				本结构只是用于操作对象是次要服务（include）的情况
				该结构体用于GATT客户端，对于GATT的角色为服务器，那么该结构体是无用的。SYD8821只是提供ATT客户
端相应的API，并没有提供GAP的处理流程，以留给APP最大的灵活性
**************************************************************************************************/
struct att_read_by_type_include_rsp{
	uint8_t length;
	uint16_t hdl;
	uint8_t buf[MAX_ATT_DATA_SZ-2];
};




/*************************************************************************************************
*联合体名：att_read_by_type_pair_val
*成员： hdl		Attribute Data List参数中的Attribute Handle
				val		Attribute Data List参数中的Attribute Value
*说明：	本结构只是用于操作对象是单个pair值的情况
**************************************************************************************************/
struct att_read_by_type_pair_val{
	uint16_t hdl;
	uint8_t val[10];
};




/*************************************************************************************************
*联合体名：att_read_by_type_val_rsp
*成员： length		Length参数
				pair		Attribute Data List参数
*说明：	本结构体对应着Read By Type Response的参数，根据规范因为length不一样，其中length决定了
Attribute Data List的长度
				本结构只是用于操作对象是单个pair值的情况
				该结构体用于GATT客户端，对于GATT的角色为服务器，那么该结构体是无用的。SYD8821只是提供ATT客户
端相应的API，并没有提供GAP的处理流程，以留给APP最大的灵活性
**************************************************************************************************/
struct att_read_by_type_val_rsp{
	uint8_t length;
	struct att_read_by_type_pair_val pair[1];
};




/*************************************************************************************************
*联合体名：att_read_by_type_service_16
*成员:hdl		Attribute Data List参数中的Attribute Handle
			uuid	Attribute Data List参数中的Attribute Value
*说明：	本结构体对应着Read By Type Response的参数
				本结构只是用于操作对象是主要服务的情况
**************************************************************************************************/
struct att_read_by_type_service_16{
	uint16_t hdl;
	uint8_t uuid[2];
};



/*************************************************************************************************
*联合体名：att_read_by_type_service_128
*成员:hdl		Attribute Data List参数中的Attribute Handle
			uuid	Attribute Data List参数中的Attribute Value
*说明：	本结构体对应着Read By Type Response的参数
				本结构只是用于操作对象是主要服务的情况
**************************************************************************************************/
struct att_read_by_type_service_128{
	uint16_t hdl;
	uint8_t uuid[16];
};




/*************************************************************************************************
*联合体名：att_read_by_type_service_payload
*成员:uuid16	16bituuid下的Attribute Data List参数
			uuid128	128bituuid下的Attribute Data List参数
*说明：	本结构体对应着Read By Type Response的参数，根据规范因为length不一样Attribute Data List有可
能是handle和16bit uuid的主要服务组合，也有可能是handle和128bit uuid的主要服务组合
				本结构只是用于操作对象是主要服务的情况
**************************************************************************************************/
union  att_read_by_type_service_payload {
	struct att_read_by_type_service_16   uuid16[3];
	struct att_read_by_type_service_128 uuid128;
};




/*************************************************************************************************
*联合体名：att_read_by_type_service_rsp
*成员:length	Length参数
			pair	attribute Data List参数
*说明：	本结构体对应着Read By Type Response的参数，根据规范因为length不一样Attribute Data List有可
能是handle和16bit uuid的主要服务组合，也有可能是handle和128bit uuid的主要服务组合
				本结构只是用于操作对象是主要服务的情况
				该结构体用于GATT客户端，对于GATT的角色为服务器，那么该结构体是无用的。SYD8821只是提供ATT客户
端相应的API，并没有提供GAP的处理流程，以留给APP最大的灵活性
**************************************************************************************************/
struct att_read_by_type_service_rsp{
	uint8_t length;
	union att_read_by_type_service_payload pair;
};





/*************************************************************************************************
*联合体名：att_read_by_type_chartextend_rsp
*成员:length	Length参数
			hdl		Attribute Data List参数中的Attribute Handle
			val 	128bituuid下的Attribute Data List参数
*说明：	本结构体对应着Read By Type Response的参数，根据规范因为length代表Attribute Data List的长度
				本结构只是用于操作对象是特性，和att_read_by_type_rsp不同，这是一个扩展的结构体
				该结构体用于GATT客户端，对于GATT的角色为服务器，那么该结构体是无用的。SYD8821只是提供ATT客户
端相应的API，并没有提供GAP的处理流程，以留给APP最大的灵活性
**************************************************************************************************/
struct att_read_by_type_chartextend_rsp{
	uint8_t length;
	uint16_t hdl;
	uint8_t val[MAX_ATT_DATA_SZ-4];
};




/*************************************************************************************************
*联合体名：att_read_rsp
*成员:buf	Attribute Value参数
*说明：	该结构体用于GATT客户端，对于GATT的角色为服务器，那么该结构体是无用的。SYD8821只是提供ATT客户
端相应的API，并没有提供GAP的处理流程，以留给APP最大的灵活性
**************************************************************************************************/
struct att_read_rsp{
	uint8_t buf[MAX_ATT_DATA_SZ-1];
};




/*************************************************************************************************
*联合体名：att_read_blob_rsp
*成员:buf	Part Attribute Value参数
*说明：	该结构体用于GATT客户端，对于GATT的角色为服务器，那么该结构体是无用的。SYD8821只是提供ATT客户
端相应的API，并没有提供GAP的处理流程，以留给APP最大的灵活性
**************************************************************************************************/
struct att_read_blob_rsp{
	uint8_t buf[MAX_ATT_DATA_SZ-1];
};




/*************************************************************************************************
*联合体名：att_read_multiple_rsp
*成员:val	Set Of Values参数
*说明：	该结构体用于GATT客户端，对于GATT的角色为服务器，那么该结构体是无用的。SYD8821只是提供ATT客户
端相应的API，并没有提供GAP的处理流程，以留给APP最大的灵活性
**************************************************************************************************/
struct att_read_multiple_rsp{
	uint8_t val[MAX_ATT_DATA_SZ-1];
};




/*************************************************************************************************
*联合体名：att_read_by_group_type_16
*成员:hdl	16bituuid下的Attribute Data List的Attribute Handle参数
			end_hdl	16bituuid下的Attribute Data List的End Group Handle参数
			uuid 16bituuid下的Attribute Data List的Attribute Value参数
*说明：	本结构体对应着Read by Group Type Response的参数
**************************************************************************************************/
struct att_read_by_group_type_16{
	uint16_t hdl;
	uint16_t end_hdl;
	uint8_t uuid[2];
};




/*************************************************************************************************
*联合体名：att_read_by_group_type_128
*成员:hdl	128bituuid下的Attribute Data List的Attribute Handle参数
			end_hdl	128bituuid下的Attribute Data List的End Group Handle参数
			uuid 128bituuid下的Attribute Data List的Attribute Value参数
*说明：	本结构体对应着Read by Group Type Response的参数
**************************************************************************************************/
struct att_read_by_group_type_128{
	uint16_t hdl;
	uint16_t end_hdl;
	uint8_t uuid[16];
};




/*************************************************************************************************
*联合体名：att_read_by_group_type_payload
*成员:uuid16	16bituuid下的Attribute Data List参数
			uuid128	128bituuid下的Attribute Data List参数
*说明：	本结构体对应着Read by Group Type Response的参数，根据规范因为length不一样Attribute Data List
有可能是handle和16bit uuid的主要服务组合，也有可能是handle和128bit uuid的主要服务组合
**************************************************************************************************/
union  att_read_by_group_type_payload {
	struct att_read_by_group_type_16   uuid16[3];
	struct att_read_by_group_type_128 uuid128;
};




/*************************************************************************************************
*联合体名：att_read_by_group_type_rsp
*成员:length	Length参数
			pair	attribute Data List参数
*说明：	本结构体对应着Read by Group Type Response的参数，根据规范因为length不一样Attribute Data List
有可能是handle和16bit uuid的主要服务组合，也有可能是handle和128bit uuid的主要服务组合
				该结构体用于GATT客户端，对于GATT的角色为服务器，那么该结构体是无用的。SYD8821只是提供ATT客户
端相应的API，并没有提供GAP的处理流程，以留给APP最大的灵活性
**************************************************************************************************/
struct att_read_by_group_type_rsp{
	uint8_t length;
	union  att_read_by_group_type_payload pair;
};




/*************************************************************************************************
*联合体名：att_hdl_val_notifivation
*成员:hdl	Attribute Handle参数
			buf	Attribute Value参数
*说明：	本结构体对应着Handle Value Notification的参数，根据规范buf最大值为ATT_MTU�
				该结构体用于GATT客户端，对于GATT的角色为服务器，那么该结构体是无用的。SYD8821只是提供ATT客户
端相应的API，并没有提供GAP的处理流程，以留给APP最大的灵活性
**************************************************************************************************/
struct att_hdl_val_notifivation{
	uint16_t hdl;
	uint8_t buf[MAX_ATT_DATA_SZ-3];
};



/*************************************************************************************************
*联合体名：att_hdl_val_indication
*成员:hdl	Attribute Handle参数
			buf	Attribute Value参数
*说明：	本结构体对应譎andle Value Indication的参数，根据规范buf最大值为ATT_MTU�
				该结构体用于GATT客户端，对于GATT的角色为服务器，那么该结构体是无用的。SYD8821只是提供ATT客户
端相应的API，并没有提供GAP的处理流程，以留给APP最大的灵活性
**************************************************************************************************/
struct att_hdl_val_indication{
	uint16_t hdl;
	uint8_t buf[MAX_ATT_DATA_SZ-3];
};




/*************************************************************************************************
*结构体名：attc_ble_evt
*成员： attc_code	协议栈上报att客户端事件的标识码（code)
				attc_sz 协议栈上报att客户端事件的长度
				evt	协议栈上报att事件的具体内容，是一个联合体，包含有各种事件的数据
					AttErrRsp	ATT客户端错误事件的数据
					AttMtuRsp	ATT客户端交换MTU事件的数据
					AttFindInfoRsp	ATT客户端上报Find Information Response事件的数据
					AttFindByTypeValRsp	ATT客户端上报Read By Type Response事件的数据
					AttReadByTypeRsp	ATT客户端上报Read By Type Response事件的数据，该数据只适用于characteristic
					AttReadByTypeIncludeRsp	ATT客户端上报Read By Type Response事件的数据，该数据只适用于包含服务
					AttReadByTypeValRsp	ATT客户端上报Read By Type Response事件的数据，该数据只适用于单个pair值
					AttReadByTypeServiceRsp	ATT客户端上报Read By Type Response事件的数据，该数据只适用于服务
					AttReadByTypeChartExtendRsp	ATT客户端上报Read By Type Response事件的数据，该数据只适用于扩展
																			characteristic
					AttReadRsp	ATT客户端上报Read Response事件的数据
					AttReadBlobRsp	ATT客户端上报Read Blob Response事件的数据
					AttReadMultipleRsp	ATT客户端上报Read Multiple Response事件的数据
					AttReadByGroupTypeRsp	ATT客户端上报Read by Group Type Response事件的数据
					AttHdlValNotification	ATT客户端上报Handle Value Notification事件的数据
					AttHdlValIndication	ATT客户端上报Handle Value Indication事件的数据
*说明： SYD8821在收到蓝牙ATT客户端的事件的时候通过gap_s_att_c_evt_handler_set设置的接口上报蓝牙事件，
attc_ble_evt结构体为蓝ATT客户端牙事件的具体数据
				该结构体用于GATT客户端，对于GATT的角色为服务器，那么该结构体是无用的。SYD8821只是提供ATT客户
端相应的API，并没有提供GAP的处理流程，以留给APP最大的灵活性
**************************************************************************************************/
struct attc_ble_evt {
	uint8_t	attc_code;
	uint8_t	attc_sz;
	union
	{
		struct att_err_rsp AttErrRsp;
		struct att_mtu_rsp AttMtuRsp;
		struct att_find_info_rsp AttFindInfoRsp;
		struct att_find_by_type_val_rsp AttFindByTypeValRsp;
		struct att_read_by_type_rsp AttReadByTypeRsp;
		struct att_read_by_type_include_rsp AttReadByTypeIncludeRsp;
		struct att_read_by_type_val_rsp AttReadByTypeValRsp;
		struct att_read_by_type_service_rsp AttReadByTypeServiceRsp;
		struct att_read_by_type_chartextend_rsp AttReadByTypeChartExtendRsp;
		struct att_read_rsp AttReadRsp;
		struct att_read_blob_rsp AttReadBlobRsp;
		struct att_read_multiple_rsp AttReadMultipleRsp;
		struct att_read_by_group_type_rsp AttReadByGroupTypeRsp;
		struct att_hdl_val_notifivation AttHdlValNotification;
		struct att_hdl_val_indication AttHdlValIndication;
	} attc; 
};




/*************************************************************************************************
*结构体名：gap_att_report_handle
*成员： cnt	BLE的report(notify或者indicate)的个数，也就是CCCD描述符的个数
				report	BLE的report(notify或者indicate)的具体内容，该数组最大为20
*说明： SYD8821的profile把notify和indicate两种GATT service行为集合称为report，专门设立结构体
gap_att_report_handle保存了report的相应属性信息，每个CCCD对应着结构体gap_att_report_handle中的一个
数组成员。包括本CCCD所在的位置（其主要服务，特性，val_hdl）等
**************************************************************************************************/
struct gap_att_report_handle {
	uint8_t	cnt;
	struct	gap_att_report report[MAX_ATT_REPORT_HDL];
};




/*************************************************************************************************
*结构体名：gap_bond_dev
*成员： addr	BLE绑定设备的地址
				key	BLE绑定信息的具体参数（配对后生产的密匙）
*说明： BLE在配对之后根据需要可能会进行绑定的操作，这里会把本结构体存进芯片内部的flash中
可以通过api:bm_s_bond_info_get获取绑定信息
**************************************************************************************************/
struct gap_bond_dev {			
	struct gap_ble_addr 		addr;
	struct gap_key_params	key;	
};




/*************************************************************************************************
*结构体名：gap_wakeup_config
*成员： timer_wakeup_en	定时器是否作为睡眠后的唤醒源
				gpi_wakeup_en	GPIO是否作为睡眠后的唤醒源
				gpi_wakeup_cfg	设置作为睡眠后的唤醒源的GPIO
				gpi_wakeup_pol	设置作为睡眠后的唤醒源的GPIO的极性
*说明： 本结构体用于配置SYD8821睡眠的唤醒源，但是只是对定时器和gpio的唤醒源进行配置，整个睡眠的唤醒
源的设置要结合SystemPowerDown以及SystemSleep函数的调用参数
				gpi_wakeup_pol用于设置唤醒源的GPIO的极性，同时GPIO模块也有GPIOINT_POL_SET的极性设置，两个极性
设置是独立的，唤醒源的设置只是和gpi_wakeup_pol有关，和GPIO模块的设置无关
**************************************************************************************************/
struct gap_wakeup_config {
	uint8_t timer_wakeup_en;
	uint8_t gpi_wakeup_en;
	uint32_t gpi_wakeup_cfg;
	uint32_t gpi_wakeup_pol;
};




/*************************************************************************************************
*枚举名：POWER_SAVING_TYPE
*成员： POWER_SAVING_RC_ON	睡眠后高频RC时钟依旧打开
				POWER_SAVING_RC_OFF	睡眠后高频RC时钟将被关闭
				POWER_SAVING_DSLEEP_LPO_ON_RETAIN	睡眠后低频RC(LPO)依旧打开，并且唤醒后接着睡眠前的位置执行
				POWER_SAVING_DSLEEP_LPO_OFF_RETAIN	睡眠后低频RC(LPO)被关闭，并且唤醒后接着睡眠前的位置执行
				POWER_SAVING_DSLEEP_LPO_ON_RESET	睡眠后低频RC(LPO)依旧打开，并且唤醒后将会复位
				POWER_SAVING_DSLEEP_LPO_OFF_RESET	睡眠后低频RC(LPO)被关闭，并且唤醒后将会复位
				POWER_SAVING_TYPE_NUM	省电模式的数量
*说明： 省电模式类型决定高频RC振荡器和低频RC振荡器在睡眠后是否打开，数字越大越省电，并且设置LPO的
模式下RC振荡器是被关闭的，比如设置POWER_SAVING_DSLEEP_LPO_ON_RETAIN那么随带着也设置了
POWER_SAVING_RC_OFF
				带有“DSLEEP”字样的意味着将会进入深度睡眠模式
				带有“RESET”字样的意味着唤醒后将会复位，内存等将不复存在
**************************************************************************************************/
typedef enum {
	POWER_SAVING_RC_ON 	               = 0,
	POWER_SAVING_RC_OFF	               = 1,
	POWER_SAVING_DSLEEP_LPO_ON_RETAIN  = 2,
	POWER_SAVING_DSLEEP_LPO_OFF_RETAIN = 3,
  POWER_SAVING_DSLEEP_LPO_ON_RESET   = 4,
	POWER_SAVING_DSLEEP_LPO_OFF_RESET  = 5,
  POWER_SAVING_TYPE_NUM,    
} POWER_SAVING_TYPE;




/*************************************************************************************************
*枚举名：MODULE_CONTROL_TYPE
*成员： NO_MODULE	睡眠后不关闭任何模块
				PER_MODULE	睡眠后关闭外设模块，包括GPIO SPI iic等，设置该模块唤醒后必须要重设这些模块
				BLE_MODULE	睡眠后关闭BLE模块，连接上后不能够设置，当时在不广播的时候必须设置
				FLASH_LDO_MODULE	睡眠后关闭FLASH_LDO,也就是FLASH的电源
				PB_MODULE	为PER_MODULE和BLE_MODULE的集合
				PF_MODULE	为PER_MODULE和FLASH_LDO_MODULE的集合
				PBF_MODULE	为PER_MODULE和FLASH_LDO_MODULE以及FLASH_LDO_MODULE的集合
*说明： 模块控制类型用于控制在睡眠后某些模块的开关
				FLASH_LDO_MODULE模式下唤醒的时候必须要给flash足够的事件，也就是SystemSleep传入的第三个参数
**************************************************************************************************/
typedef enum {
    NO_MODULE            = 0,
    PER_MODULE           = BIT0,
    BLE_MODULE           = BIT1,
    FLASH_LDO_MODULE     = BIT2,
    PB_MODULE            = PER_MODULE | BLE_MODULE,
    PF_MODULE            = PER_MODULE | FLASH_LDO_MODULE,
    BF_MODULE            = BLE_MODULE | FLASH_LDO_MODULE,
    PBF_MODULE           = PER_MODULE | BLE_MODULE | FLASH_LDO_MODULE,
} MODULE_CONTROL_TYPE;




/*************************************************************************************************
*枚举名：PMU_WAKEUP_CONFIG_TYPE
*成员： PIN_WAKE_EN	GPIO是否作为睡眠后的唤醒源
				TIMER_WAKE_EN	定时器是否作为睡眠后的唤醒源
				FSM_SLEEP_EN	蓝牙事件是否作为睡眠后的唤醒源，只要使用BLE必须开启该功能
				ANA_WAKE_EN	ADC比较器是否作为睡眠后的唤醒源
				RTC_WAKE_EN	RTC是否作为睡眠后的唤醒源,rtc有三种唤醒源
				WDT_WAKE_EN	看门狗是否作为睡眠后的唤醒源,SYD8821的开门够有中断功能，使能中断必须开启该功能
				CAPDET_WAKE_EN	SYD8821触摸模块是否作为睡眠后的唤醒源,
				KEEP_ORIGINAL_WAKEUP	
*说明： 唤醒控制类型配置哪个模块能够作为唤醒源，gap_wakeup_config也有GPIO和TIMER的使能，其是无用的
真正起效的是SystemSleep传入的唤醒源
**************************************************************************************************/
typedef enum {
    PIN_WAKE_EN          = BIT0,
    TIMER_WAKE_EN        = BIT1,
    FSM_SLEEP_EN         = BIT2,
    ANA_WAKE_EN          = BIT3,
    RTC_WAKE_EN          = BIT4,
    WDT_WAKE_EN          = BIT5,
    CAPDET_WAKE_EN       = BIT6,
    KEEP_ORIGINAL_WAKEUP = BIT7,
} PMU_WAKEUP_CONFIG_TYPE;


typedef enum {
	BLE_TX_POWER_MINUS_31_DBM	= 0,
	BLE_TX_POWER_MINUS_25_DBM	= 1,
	BLE_TX_POWER_MINUS_19_DBM	= 2,
	BLE_TX_POWER_MINUS_13_DBM	= 3,
	BLE_TX_POWER_MINUS_8_DBM	= 4,
	BLE_TX_POWER_MINUS_3_DBM	= 5,
	BLE_TX_POWER_0_DBM    		= 6,
	BLE_TX_POWER_2_DBM    		= 7,
	BLE_TX_POWER_4_DBM    		= 8,
} BLE_TX_POWER;



/*************************************************************************************************
*枚举名：AMIC_BIAS
*成员：  AMIC_BIAS_2_94	amic的 adc bias 为2.94V
		AMIC_BIAS_2_5	amic的 adc bias 为2.5V
		AMIC_BIAS_1_4	amic的 adc bias 为1.4V
		AMIC_BIAS_1_25	amic的 adc bias 为1.25V
*说明： 设置AMIC的bias参数，amic_set_bias传入的参数
**************************************************************************************************/
typedef enum {
	AMIC_BIAS_2_94	= 0,
	AMIC_BIAS_2_5	= 1,
	AMIC_BIAS_1_4	= 2,
	AMIC_BIAS_1_25	= 3,
} AMIC_BIAS;


#pragma pack()

/*************************************************************************************************
*函数名：gap_s_ble_init
*输入参数：无
*输出参数：uint8_t -->初始化协议栈的结果
						0	-->协议栈初始化失败
						1	-->协议栈初始化成功
*说明： 1.协议栈初始化函数会把底层和RF相关的寄存器进行初始化
				2.该函数会把和BLE协议相关的内容进行初始化，比如profile、安全参数、连接参数等
**************************************************************************************************/
extern uint8_t gap_s_ble_init(void);




/*************************************************************************************************
*函数名：gap_s_disconnect
*输入参数：无
*输出参数：无
*说明： 该函数会发起LL_TERMINATE_IND,
				根据协议主机收到该请求后会回复一个空包，代表断线结束，断线结束后协议栈会上报一个断线事件
**************************************************************************************************/
extern void gap_s_disconnect(void);




/*************************************************************************************************
*函数名：gap_s_validate_irk
*输入参数：无
*输出参数：无
*说明： 设置随机私有地址的irk
**************************************************************************************************/
extern uint8_t gap_s_validate_irk(uint8_t *irk);




/*************************************************************************************************
*函数名：gap_s_ble_gen_random_private_address
*输入参数:struct gap_ble_addr * rpa	返回地址的指针
*输出参数:无
*说明:获取私有随机地址
**************************************************************************************************/
extern void gap_s_ble_gen_random_private_address(struct gap_ble_addr * rpa);




/*************************************************************************************************
*函数名：gap_s_ble_address_set
*输入参数:struct gap_ble_addr* p_dev	设置地址的指针
*输出参数:无
*说明:设置设备地址
			如果调用了这个接口，BLE的设置地址将以该地址为准，如果不调用这个接口，BLE的设备地址将以配置文
			件的为准
			注意：对于量产的程序，如果设置该接口，那么整批产品的地址将一样
**************************************************************************************************/
extern void gap_s_ble_address_set(struct gap_ble_addr* p_dev);




/*************************************************************************************************
*函数名：gap_s_ble_address_get
*输入参数:struct gap_ble_addr* p_dev	获取地址的指针
*输出参数:无
*说明:获取设备的地址
**************************************************************************************************/
extern void gap_s_ble_address_get(struct gap_ble_addr* p_dev);




/*************************************************************************************************
*函数名：gap_s_ble_feature_set
*输入参数:uint8_t *p_feature	设置feature的指针 依据规范要求，这里的传入的数据长度应该为8
*输出参数:无
*说明:该函数设置主机端的feature，相对于该项，BLE5.0在BLE4.0只有一个bit的基础上增加到了16个bit
			SYD8821该项的默认值是0x0000000000000133
			该函数只有在SYD8821处于主机模式才有用
**************************************************************************************************/
extern void gap_s_ble_feature_set(uint8_t *p_feature);




/*************************************************************************************************
*函数名：gap_s_ble_feature_get
*输入参数:uint8_t *p_feature	返回本机的feature的指针 依据规范要求，这里会返回8个byte的数据
*输出参数:无
*说明:该函数获取主机端的feature，相对于该项，BLE5.0在BLE4.0只有一个bit的基础上增加到了16个bit
			SYD8821该项的默认值是0x0000000000000133
			该函数只有在SYD8821处于主机模式才有用
**************************************************************************************************/
extern void gap_s_ble_feature_get(uint8_t *p_feature);




/*************************************************************************************************
*函数名：gap_s_ble_feature_set
*输入参数:uint8_t *p_feature	设置feature的指针 依据规范要求，这里的传入的数据长度应该为8
*输出参数:无
*说明:该函数设置从机端的feature，相对于该项，BLE5.0在BLE4.0只有一个bit的基础上增加到了16个bit
			SYD8821该项的默认值是0x0000000000000133
			该函数只有在SYD8821处于从机模式才有用
**************************************************************************************************/
extern void gap_s_ble_slave_feature_set(uint8_t *p_feature);




/*************************************************************************************************
*函数名：gap_s_ble_feature_get
*输入参数:uint8_t *p_feature	返回本机的feature的指针 依据规范要求，这里会返回8个byte的数据
*输出参数:无
*说明:该函数获取从机端的feature，相对于该项，BLE5.0在BLE4.0只有一个bit的基础上增加到了16个bit
			SYD8821该项的默认值是0x0000000000000133
			该函数只有在SYD8821处于从机模式才有用
**************************************************************************************************/
extern void gap_s_ble_slave_feature_get(uint8_t *p_feature);




/*************************************************************************************************
*函数名：gap_s_adv_access_code_set
*输入参数:uint8_t *p_acc	设置广播通道访问地址指针（4byte）
*输出参数:无
*说明:设置广播通道的访问地址，非特殊情况不建议使用
**************************************************************************************************/
extern void gap_s_adv_access_code_set(uint8_t *p_acc);





/*************************************************************************************************
*函数名：gap_s_adv_access_code_get
*输入参数:uint8_t *p_acc	返回广播通道访问地址指针（4byte）
*输出参数:无
*说明:获取广播访问地址，在不调用gap_s_adv_access_code_set的情况下返回0x8E89BED6,非特殊情况下不建议
			调用该接口
**************************************************************************************************/
extern void gap_s_adv_access_code_get(uint8_t *p_acc);





/*************************************************************************************************
*函数名：gap_s_adv_parameters_set
*输入参数:struct gap_adv_params * p_adv	广播参数指针
*输出参数:无
*说明:设置广播参数，具体参数请看gap_adv_params结构体
**************************************************************************************************/
extern void gap_s_adv_parameters_set(struct gap_adv_params * p_adv);





/*************************************************************************************************
*函数名：gap_s_adv_data_set
*输入参数:uint8_t *p_adv	广播数据指针
					uint8_t adv_sz	广播数据大小
					uint8_t *p_scan	扫描响应数据指针
					uint8_t sacn_sz	扫描响应数据大小
*输出参数:无
*说明:该函数只是把广播数据的设置保存到协议栈底层缓冲区，并没有真正起效，真正起效是在调gap_s_adv_start
			函数的时候
			因为主机进行扫描的时候留给从机的时间很短，所以这里提前把数据准备好，而不是扫描请求来了再填充数据
**************************************************************************************************/
extern void gap_s_adv_data_set(uint8_t *p_adv, uint8_t adv_sz,uint8_t *p_scan, uint8_t sacn_sz);





/*************************************************************************************************
*函数名：gap_s_adv_start
*输入参数:无
*输出参数:无
*说明:调用该函数开始广播，把gap_s_adv_data_set设置下来的数据发送到广播通道
**************************************************************************************************/
extern void gap_s_adv_start(void);





/*************************************************************************************************
*函数名：gap_s_adv_stop
*输入参数:无
*输出参数:无
*说明:调用该函数停止广播
**************************************************************************************************/
extern void gap_s_adv_stop(void);





/*************************************************************************************************
*函数名：gap_s_scan_parameters_set
*输入参数:struct gap_scan_params*p_scan	扫描参数设置
*输出参数:无
*说明:设置烧面参数，具体参数请看gap_scan_params结构体 注意：这里扫描的动作和广播不能够同时进行
**************************************************************************************************/
extern void gap_s_scan_parameters_set(struct gap_scan_params*p_scan);





/*************************************************************************************************
*函数名：gap_s_scan_start
*输入参数:无
*输出参数:无
*说明:调用该函数开始扫描
**************************************************************************************************/
extern void gap_s_scan_start(void);





/*************************************************************************************************
*函数名：gap_s_adv_state_get
*输入参数:无
*输出参数:uint8_t	蓝牙广播状态	该返回值为枚举_ADV_SCAN_MODE_的成员
*说明:调用该函数可获取底层蓝牙广播的状态，广播状态分为空闲状态、广播状态、扫描状态等，具体可看，枚举：
_ADV_SCAN_MODE_
**************************************************************************************************/
extern uint8_t gap_s_adv_state_get(void);





/*************************************************************************************************
*函数名：gap_s_scan_stop
*输入参数:无
*输出参数:无
*说明:调用该函数停止广播
**************************************************************************************************/
extern void gap_s_scan_stop(void);





/*************************************************************************************************
*函数名：gap_s_coex_adv_data_set
*输入参数:uint8_t advtype	连接状态下广播的类型
					uint8_t *buf		连接状态下广播的数据指针
					uint8_t sz			连接状态下广播的数据的大小
					uint8_t *p_addr	连接状态下广播的地址指针
					uint8_t addr_type	连接状态下广播的地址类型
*输出参数:无
*说明:该函数相对于gap_s_adv_data_set中的不同之处在于该函数是在连接状态中调用，连接状态中发起的广播
			不可连接，因为SYD8821的从机目前还不支持混合状态机的机制
**************************************************************************************************/
extern void gap_s_coex_adv_data_set(uint8_t advtype, uint8_t *buf, uint8_t sz, uint8_t *p_addr, uint8_t addr_type);





/*************************************************************************************************
*函数名：gap_s_coex_scan_rsp_data_set
*输入参数:uint8_t *buf		连接状态下扫描响应的数据指针
					uint8_t sz			连接状态下扫描响应的数据的大小
					uint8_t *p_addr	连接状态下扫描响应的地址指针
					uint8_t addr_type	连接状态下扫描响应的地址类型
*输出参数:无
*说明:该函数相对于gap_s_adv_data_set中的扫描响应数据不同之处在于该函数是在连接状态中调用
**************************************************************************************************/
extern void gap_s_coex_scan_rsp_data_set(uint8_t *buf, uint8_t sz, uint8_t *p_addr, uint8_t addr_type);





/*************************************************************************************************
*函数名：gap_s_coex_adv_start
*输入参数:无
*输出参数:无
*说明:调用该函数开始广播，把gap_s_coex_adv_data_set设置下来的数据发送到广播通道
			该函数相对于gap_s_adv_start的不同之处在于该函数是在连接状态中调用
**************************************************************************************************/
extern void gap_s_coex_adv_start(void);





/*************************************************************************************************
*函数名：gap_s_coex_adv_stop
*输入参数:无
*输出参数:无
*说明:调用该函数停止广播
			该函数相对于gap_s_adv_start的不同之处在于该函数是在连接状态中调用
**************************************************************************************************/
extern void gap_s_coex_adv_stop(void);





/*************************************************************************************************
*函数名：gap_s_coex_scan_req_data_set
*输入参数:uint8_t *buf		连接状态下扫描请求的数据指针
*输出参数:无
*说明:该函数设置连接状态下的扫描请求数据，设置的是烧面请求中的广播地址
**************************************************************************************************/
extern void gap_s_coex_scan_req_data_set(uint8_t *buf);





/*************************************************************************************************
*函数名：gap_s_coex_scan_start
*输入参数:无
*输出参数:无
*说明:调用该函数开始扫描
			该函数相对于gap_s_scan_start的不同之处在于该函数是在连接状态中调用
**************************************************************************************************/
extern void gap_s_coex_scan_start(void);





/*************************************************************************************************
*函数名：gap_s_coex_scan_stop
*输入参数:无
*输出参数:无
*说明:调用该函数停止扫描
			该函数相对于gap_s_scan_stop的不同之处在于该函数是在连接状态中调用
**************************************************************************************************/
extern void gap_s_coex_scan_stop(void);





/*************************************************************************************************
*函数名：gap_s_gatt_profiles_set
*输入参数:struct gap_profile_struct *p_gatt_profile profile元素(如:服务、特性、值以及report_handle)
*输出参数:无
*说明:指定profile各个元素的位置，底层协议栈会去p_gatt_profile结构体指定的位置去获取profile
**************************************************************************************************/
extern void gap_s_gatt_profiles_set(struct gap_profile_struct *p_gatt_profile);





/*************************************************************************************************
*函数名：gap_s_security_parameters_set
*输入参数:struct gap_pairing_req *p_sec_params 蓝牙安全参数
*输出参数:无
*说明:设置蓝牙安全参数，这些参数影响到绑定配对的过程
**************************************************************************************************/
extern void gap_s_security_parameters_set(struct gap_pairing_req *p_sec_params);





/*************************************************************************************************
*函数名：gap_s_security_parameters_get
*输入参数:struct gap_pairing_req *p_sec_params 蓝牙安全参数
*输出参数:无
*说明:获取蓝牙安全参数
**************************************************************************************************/
extern void gap_s_security_parameters_get(struct gap_pairing_req *p_sec_params);





/*************************************************************************************************
*函数名：gap_s_security_req
*输入参数:uint8_t flag -->是否绑定
						0	-->	配对后不绑定
						1	-->	配对后绑定
					uint8_t mitm	-->中间人保护
						0	-->	没有中间人保护
						1	-->	需要中间人保护（输入密码等）
*输出参数:无
*说明:其中中间人保护除了输入密码还有ODB这种方式
**************************************************************************************************/
extern void gap_s_security_req(uint8_t flag, uint8_t mitm);





/*************************************************************************************************
*函数名：gap_s_connection_param_set
*输入参数:struct gap_connection_param_rsp_pdu *p_connection_params	设置连接请求响应参数结构体指针
*输出参数:无
*说明:设置连接请求响应参数，这个是BLE4.1以上的版本支持的接口
			该函数只是把连接参数的设置保存到协议栈底层缓冲区，并没有真正起效，真正起效是在主机发起
			LL_CONNECTION_PARAM_REQ的时候
**************************************************************************************************/
extern void gap_s_connection_param_set(struct gap_connection_param_rsp_pdu *p_connection_params);





/*************************************************************************************************
*函数名：gap_s_connection_param_get
*输入参数:struct gap_connection_param_rsp_pdu *p_connection_params	返回连接参数结构体指针
*输出参数:无
*说明:获取连接请求响应参数，这个是BLE4.1以上的版本支持的接口
**************************************************************************************************/
extern void gap_s_connection_param_get(struct gap_connection_param_rsp_pdu *p_connection_params);





/*************************************************************************************************
*函数名：gap_s_connection_update
*输入参数:struct gap_update_params *p_update_params	返回连接参数请求结构体指针
*输出参数:无
*说明:设置蓝牙连接参数，具体请看gap_update_params结构体
			注意：只有在从机连接状态下才能够设置连接参数
			该函数只是发送连接参数更新请求，并没有设立相应的机制来判断这些连接参数更新是否真的能够起效，
虽然该函数，
			虽然连接参数包含了latency这个参数，但是SYD8821的协议栈并没有对latency进行管理，何时使能何时
失能并没有在gap_s_connection_update函数的管理范围之内
			SYD8821对于连接参数的管理专门成立了专门的连接参数管理机制：smart_update_latency
**************************************************************************************************/
extern void gap_s_connection_update(struct gap_update_params *p_update_params);





/*************************************************************************************************
*函数名：gap_s_link_parameters_get
*输入参数:struct gap_link_params* p_link	返回链路参数结构体指针
*输出参数:无
*说明:获取蓝牙本地链路参数，具体请看gap_link_params结构体
			链路参数是双方对连接参数商量得到的结果，是物理层上正在起作用的连接参数
**************************************************************************************************/
extern void gap_s_link_parameters_get(struct gap_link_params* p_link);





/*************************************************************************************************
*函数名：gap_s_passkey_set
*输入参数:uint32_t passkey	配对过程中的密码，该密码为十进制数据，比如这里传入passkey=123456，
														那么在主机端应输入字符“123456”
*输出参数:无
*说明:含有中间人保护的配对过程在开始配对的时候会上报GAP_EVT_SHOW_PASSKEY_REQ事件，app代码要在该事
			件中调用gap_s_passkey_set接口设置密码
**************************************************************************************************/
extern void gap_s_passkey_set(uint32_t passkey);





/*************************************************************************************************
*函数名：gap_s_evt_handler_set
*输入参数:struct gap_evt_callback* p_callback	设置协议栈上报结构体
*输出参数:无
*说明:该函数为app代码设置协议栈（GAP）上报蓝牙事件的接口，也是GAP层上和APP交互的接口
			gap_evt_callback中有两个成员，其中evt_mask成员能够屏蔽掉不需要的事件，设置相应的bitmask能够
			阻挡GAP的上报，
			gap_evt_callback的p_callback成员设置了GAP上报事件给APP的接口指针，该接口的参数是一个
			gap_ble_evt结构体，当协议栈发现蓝牙状态有变化时，
			GAP层会调用该函数同时返回一个gap_ble_evt结构体，APP的p_callback函数中根据gap_ble_evt结构体处
			理各类蓝牙事件，
**************************************************************************************************/
extern void gap_s_evt_handler_set(struct gap_evt_callback* p_callback);





/*************************************************************************************************
*函数名：gap_s_att_c_evt_handler_set
*输入参数:void* p_callback	设置Gatt层Client事件上报的函数指针
*输出参数:无
*说明:该函数为app代码设置Gatt的Client上报attc事件的接口，因为从机一般情况下是server端，所以GATT的事件并
不归类到GAP的事件中，而是单独成立一个接口
			p_callback指针参数是一个attc_ble_evt结构体，当协议栈发现GATT的Client有变化时（比如主机端对于某条
ATT指令作出了响应）会调用该函数，同时返回一个attc_ble_evt结构体，APP的p_callback函数中根据个attc_ble_evt
结构体处理各类蓝牙事件，
**************************************************************************************************/
extern void gap_s_att_c_evt_handler_set(void* p_callback);





/*************************************************************************************************
*函数名：gap_s_gatt_report_handle_get
*输入参数:struct gap_att_report_handle** p_hdl	返回profile中report_handle的二级指针
*输出参数:无
*说明:notify或者indicate的行为都需要往下传gap_att_report结构体，该结构体指定了发送的uuid和hdl等，而这些
信息都存储在_gatt_database_report_handle结构体中，
			当然调用该函数和直接读取_gatt_database_report_handle效果是一样的
**************************************************************************************************/
extern void gap_s_gatt_report_handle_get(struct gap_att_report_handle** p_hdl);





/*************************************************************************************************
*函数名：gap_s_gatt_read_rsp_set
*输入参数:uint8_t len	指定读响应的长度
					uint8_t *p_data 读响应的具体数据
*输出参数:无
*说明:该函数设置了读响应的返回数据，也就是主机端读取相应特性得到的数据
			调用该函数的时候协议栈可能并不能够立即将数据发送出去，只是把数据放到底层发送队列中，如果底层缓冲区
还有数据待发送，这里会因为要等待之前的数据发送完成
**************************************************************************************************/
extern void gap_s_gatt_read_rsp_set(uint8_t len,uint8_t *p_data);





/*************************************************************************************************
*函数名：gap_s_gatt_write_err_rsp_set
*输入参数:uint16_t hdl	 att中Error Response响应的Attribute Handle In Error参数
					uint8_t err att中Error Response响应的Error Code参数
*输出参数:无
*说明:该函数主要的目的是让APP拥有让协议栈底层回复错误的能力，当app数据有错时可以调用该接口
**************************************************************************************************/
extern void gap_s_gatt_write_err_rsp_set(uint16_t hdl, uint8_t err);





/*************************************************************************************************
*函数名：gap_s_check_fifo_sz
*输入参数:无
*输出参数:uint16_t	指示底层的fifo的剩余大小
*说明:对于SYD8821，发送数据都将存放在底层的fifo中，该函数能够获取底层fifo中剩余的大小
**************************************************************************************************/
extern uint16_t gap_s_check_fifo_sz(void);





/*************************************************************************************************
*函数名：gap_s_gatt_data_send
*输入参数:uint8_t type 数据发送的类型，为BLE_SEND_TYPE枚举成员，可以是notify形式（BLE_GATT_NOTIFICATION）
											，也可以是指示形式（BLE_GATT_INDICATION）
					struct gap_att_report* p_report	指定要发送的通道（具体的某个特性）的发送结构体，此结构体为
																					_gatt_database_report_handle的某个成员
					uint8_t len	要发送的数据的长度
					uint8_t *p_data	要发送的具体数据
*输出参数:uint8_t	返回的发送结果，当底层的fifo已经填满，或者过多的indicate还没有等到返回的时候，该值为0，
否则为1代表发送成功
*说明:notify是不需要返回的发送模式，该模式可能会存在主机的上层处理不及时而丢掉数据的情况，而indicate是
需要返回确认的发送形式，
			所以这里完成后需要等待协议栈上报GAP_EVT_ATT_HANDLE_CONFIRMATION事件
**************************************************************************************************/
extern uint8_t gap_s_gatt_data_send(uint8_t type, struct gap_att_report* p_report, uint8_t len, uint8_t *p_data);







/*************************************************************************************************
*函数名：gap_s_gatt_data_send_report_confirmation_handle
*输入参数:uint8_t type 数据发送的类型，为BLE_SEND_TYPE枚举成员，可以是notify形式（BLE_GATT_NOTIFICATION）
											，也可以是指示形式（BLE_GATT_INDICATION）
					struct gap_att_report* p_report	指定要发送的通道（具体的某个特性）的发送结构体，此结构体为
																					_gatt_database_report_handle的某个成员
					uint8_t len	要发送的数据的长度
					uint8_t *p_data	要发送的具体数据
*输出参数:gap_s_gatt_data_send_report_confirmation_handle和gap_s_gatt_data_send用法一致，唯一的区别是
进行indicate调用gap_s_gatt_data_send_report_confirmation_handle后协议栈上报GAP_EVT_ATT_HANDLE_CONFIRMATION事件
的时候会把发送数据的特性的handle等同时上报上来！
uint8_t	返回的发送结果，当底层的fifo已经填满，或者过多的indicate还没有等到返回的时候，该值为0，
否则为1代表发送成功
*说明:notify是不需要返回的发送模式，该模式可能会存在主机的上层处理不及时而丢掉数据的情况，而indicate是
需要返回确认的发送形式，
			所以这里完成后需要等待协议栈上报GAP_EVT_ATT_HANDLE_CONFIRMATION事件
**************************************************************************************************/
extern uint8_t gap_s_gatt_data_send_report_confirmation_handle(uint8_t type, struct gap_att_report* p_report, uint8_t len, uint8_t *p_data);





/*************************************************************************************************
*函数名：gap_s_connection_latency_mode
*输入参数:uint8_t en	-->是否使能latency
										0-->关闭latency
										1-->打开latency
*输出参数:无
*说明:蓝牙进入连接后主机会以连接参数中的interval作为间隔与从机进行一次交互，而规范中规定从机能够忽略掉一定
数量的interval而达到节省功耗的目的，
			能够忽略interval的个数就是latency，而该函数指示协议栈底层时候使能latency
**************************************************************************************************/
extern void gap_s_connection_latency_mode(uint8_t en);





/*************************************************************************************************
*函数名：gap_s_profile_data_read
*输入参数:uint16_t addr	偏移地址，有效范围是0x0-0xfff
					uint16_t len	读取数据的长度，有效范围是0x0-0xfff
					uint8_t *p_buf	返回数据的指针，SYD8821的flash要求该指针指向的内存必须是4字节对其的
*输出参数:uint8_t	指示读取数据的结果
									0	-->失败
									1	-->成功
*说明:SYD8821内部的flash除了存储代码和配置文件外还单独留出4K的区域给app存储重要数据，因为这些数据是存储在
flash中的，所以掉电后也不会丢失
			当传入的参数非法时，该函数返回错误，这里重点注意数据指针必须是四字节对其，否则会出错
**************************************************************************************************/
extern uint8_t gap_s_profile_data_read(uint16_t addr , uint16_t len, uint8_t *p_buf);





/*************************************************************************************************
*函数名：gap_s_profile_data_write
*输入参数:uint16_t addr	偏移地址，有效范围是0x0-0xfff
					uint16_t len	写数据的长度，有效范围是0x0-0xfff
					uint8_t *p_buf	写入数据的指针，SYD8821的flash要求该指针指向的内存必须是4字节对其的
*输出参数:uint8_t	指示写入数据的结果
									0	-->失败
									1	-->成功
*说明:SYD8821内部的flash除了存储代码和配置文件外还单独留出4K的区域给app存储重要数据，因为这些数据是存储在
flash中的，所以掉电后也不会丢失
			当传入的参数非法时，该函数返回错误,这里重点注意数据指针必须是四字节对其，否则会出错
			因为该函数在写flash之前还有擦除flash扇区，所以建议不要频繁调用该函数，以免造成flash出错
			该函数只是把数据放到底层缓冲区中，正在执行flash的操作的是在ble_sched_execute函数中
**************************************************************************************************/
extern uint8_t gap_s_profile_data_write(uint16_t addr , uint16_t len, uint8_t *p_buf);





/*************************************************************************************************
*函数名：att_c_mtureq
*输入参数:uuint16_t mtu	请求设置的mtu大小
*输出参数:uint8_t	指示 Exchange MTU Request指令是否发送成功
*说明:该指令只有在当前的GATT是处在client模式下才可使用，SYD8821完全支持规范中l2cap层的功能，这里支持app
请求改变mtu，在需要传输大量数据的时候可以把MTU调大
			当对方收到Exchange MTU Request后会相应发送Exchange MTU Response数据包，当SYD8821收到该数据包的
时候协议栈的GATT层会向
			gap_s_att_c_evt_handler_set函数指定的接口上报该事件
**************************************************************************************************/
extern uint8_t att_c_mtureq(uint16_t mtu);





/*************************************************************************************************
*函数名：att_c_findinforeq
*输入参数:uint16_t start_hdl 发送ATT的Find Information Request指令的Starting Handle参数
					uint16_t end_hdl	发送ATT的Find Information Request指令的Ending Handle参数
*输出参数:uint8_t	指示 Find Information Request指令是否发送成功
									0	-->失败
									1	-->成功
*说明:该指令只有在当前的GATT是处在client模式下才可使用，调用该接口SYD8821将会发送 
Find Information Request命令，关于该命令的具体内容可看规范中ATT的相关章节
			当对方收到att_c_findinforeq后会相应发送Find Information Response数据包，当SYD8821收到该数据包的
时候协议栈的GATT层会向
			gap_s_att_c_evt_handler_set函数指定的接口上报该事件
**************************************************************************************************/
extern uint8_t att_c_findinforeq(uint16_t start_hdl, uint16_t end_hdl);





/*************************************************************************************************
*函数名：att_c_findbytypevaluereq
*输入参数:uint16_t start_hdl 发送ATT的Find By Type Value Request指令的Starting Handle参数
					uint16_t end_hdl	发送ATT的Find By Type Value Request指令的Ending Handle参数
					uint16_t type	发送ATT的Find By Type Value Request指令的Attribute Type参数
					uint8_t val_sz	发送ATT的Find By Type Value Request指令的Attribute Value参数的长度
					uint8_t *p_val	发送ATT的Find By Type Value Request指令的Attribute Value参数
*输出参数:uint8_t	指示 Find By Type Value Request指令是否发送成功
									0	-->失败
									1	-->成功
*说明:该指令只有在当前的GATT是处在client模式下才可使用，调用该接口SYD8821将会发送 
Find By Type Value Request命令，关于该命令的具体内容可看规范中ATT的相关章节
			当对方收到Find By Type Value Request后会相应发送Find By Type Value Response数据包，当SYD8821收到
该数据包的时候协议栈的GATT层会向
			gap_s_att_c_evt_handler_set函数指定的接口上报该事件
**************************************************************************************************/
extern uint8_t att_c_findbytypevaluereq(uint16_t start_hdl, uint16_t end_hdl, uint16_t type, uint8_t val_sz, uint8_t *p_val);





/*************************************************************************************************
*函数名：att_c_readbytypereq
*输入参数:uint16_t start_hdl 发送ATT的Read By Type Request指令的Starting Handle参数
					uint16_t end_hdl	发送ATT的Read By Type Request指令的Ending Handle参数
					uint16_t type_sz	发送ATT的Read By Type Request指令的Attribute Type参数的长度，根据规范这里
														只能够填充0或者16
					uint8_t *p_type	发送ATT的Read By Type Request指令的Attribute Type参数
*输出参数:uint8_t	指示 Read By Type Request指令是否发送成功
									0	-->失败
									1	-->成功
*说明:该指令只有在当前的GATT是处在client模式下才可使用，调用该接口SYD8821将会发送 Read By Type Request
命令，关于该命令的具体内容可看规范中ATT的相关章节
			当对方收到Read By Type Request后会相应发送Read By Type Response数据包，当SYD8821收到该数据包的
时候协议栈的GATT层会向
			gap_s_att_c_evt_handler_set函数指定的接口上报该事件
**************************************************************************************************/
extern uint8_t att_c_readbytypereq(uint16_t start_hdl, uint16_t end_hdl, uint16_t type_sz, uint8_t *p_type);





/*************************************************************************************************
*函数名：att_c_readreq
*输入参数:uint16_t hdl 发送ATT的Read Request指令的Attribute Handle参数
*输出参数:uint8_t	指示 Read Request指令是否发送成功
									0	-->失败
									1	-->成功
*说明:该指令只有在当前的GATT是处在client模式下才可使用，调用该接口SYD8821将会发送Read Request命令，关于
该命令的具体内容可看规范中ATT的相关章节
			当对方收到Read Request后会相应发送Read Response数据包，当SYD8821收到该数据包的时候协议栈的GATT层
会向gap_s_att_c_evt_handler_set函数指定的接口上报该事件
**************************************************************************************************/
extern uint8_t att_c_readreq(uint16_t hdl);





/*************************************************************************************************
*函数名：att_c_readblobreq
*输入参数:uint16_t hdl 发送ATT的Read Blob Request指令的Attribute Handle参数
					uint16_t offset 发送ATT的Read Blob Request指令的Value Offset参数
*输出参数:uint8_t	指示  Read Blob Request指令是否发送成功
									0	-->失败
									1	-->成功
*说明:该指令只有在当前的GATT是处在client模式下才可使用，调用该接口SYD8821将会发送 Read Blob Request命令，
关于该命令的具体内容可看规范中ATT的相关章节
			当对方收到 Read Blob Request后会相应发送Read Blob Response数据包，当SYD8821收到该数据包的时候协议
栈的GATT层会向
			gap_s_att_c_evt_handler_set函数指定的接口上报该事件
**************************************************************************************************/
extern uint8_t att_c_readblobreq(uint16_t hdl, uint16_t offset);





/*************************************************************************************************
*函数名：att_c_readmultiplereq
*输入参数:uint8_t hdl_sz 发送ATT的Read Multiple Request指令的Set Of Handles参数的长度，
												依据规范该参数不得小于4
					uint8_t *p_hdl 发送ATT的Read Multiple Request指令的Set Of Handles参数
*输出参数:uint8_t	指示  Read Multiple Request指令是否发送成功
									0	-->失败
									1	-->成功
*说明:该指令只有在当前的GATT是处在client模式下才可使用，调用该接口SYD8821将会发送 Read Multiple Request
命令，关于该命令的具体内容可看规范中ATT的相关章节
			当对方收到 腞ead Multiple Request后会相应发送Read Multiple Response数据包，当SYD8821收到该数据包
的时候协议栈的GATT层会向
			gap_s_att_c_evt_handler_set函数指定的接口上报该事件
**************************************************************************************************/
extern uint8_t att_c_readmultiplereq(uint8_t hdl_sz, uint8_t *p_hdl);





/*************************************************************************************************
*函数名：att_c_readbygrouptypereq
*输入参数:uint16_t start_hdl 发送ATT的Read by Group Type Request指令的Starting Handle参数
					uint16_t end_hdl 发送ATT的Read by Group Type Request指令的Ending Handle参数
					uint16_t type_sz 发送ATT的Read by Group Type Request指令的Attribute Group Type参数的大小，
													依据规范该大小必须为2或者16
					uint8_t *p_type	发送ATT的Read by Group Type Request指令的Attribute Group Type参数
*输出参数:uint8_t	指示  Read by Group Type Reques指令是否发送成功
									0	-->失败
									1	-->成功
*说明:该指令只有在当前的GATT是处在client模式下才可使用，调用该接口SYD8821将会发送 
Read by Group Type Request命令，关于该命令的具体内容可看规范中ATT的相关章节
			当对方收到 Read by Group Type Request后会相应发送Read by Group Type Response数据包，当SYD8821
收到该数据包的时候协议栈的GATT层会向
			gap_s_att_c_evt_handler_set函数指定的接口上报该事件
**************************************************************************************************/
extern uint8_t att_c_readbygrouptypereq(uint16_t start_hdl, uint16_t end_hdl, uint16_t type_sz, uint8_t *p_type);





/*************************************************************************************************
*函数名：att_c_writereq
*输入参数:uint16_t hdl 发送ATT的Write Request指令的Attribute Handle参数
					uint16_t sz 发送ATT的Write Request指令的Attribute Value参数的大小
					uint8_t *p_buf	发送ATT的Write Request指令的Attribute Value参数
*输出参数:uint8_t	指示  Read Blob Request指令是否发送成功
									0	-->失败
									1	-->成功
*说明:该指令只有在当前的GATT是处在client模式下才可使用，调用该接口SYD8821将会发送 Write Request命令，关
于该命令的具体内容可看规范中ATT的相关章节
			当对方收到 Write Request后会相应发送Write Response数据包，当SYD8821收到该数据包的时候协议栈的
GATT层会向
			gap_s_att_c_evt_handler_set函数指定的接口上报该事件
**************************************************************************************************/
extern uint8_t att_c_writereq(uint16_t hdl, uint16_t sz, uint8_t *p_buf);





/*************************************************************************************************
*函数名：att_c_writecmdreq
*输入参数:uint16_t hdl 发送ATT的Write Command指令的Attribute Handle参数
					uint16_t sz 发送ATT的Write Command指令的Attribute Value参数的大小
					uint8_t *p_buf	发送ATT的Write Command指令的Attribute Value参数
*输出参数:uint8_t	指示  Write Command指令是否发送成功
									0	-->失败
									1	-->成功
*说明:该指令只有在当前的GATT是处在client模式下才可使用，调用该接口SYD8821将会发送 Write Command命令，关于
该命令的具体内容可看规范中ATT的相关章节
			因为Write Command命令并没有相应数据包，所以这里不会上报任何事件
**************************************************************************************************/
extern uint8_t att_c_writecmdreq(uint16_t hdl, uint16_t sz, uint8_t *p_buf);





/*************************************************************************************************
*函数名：att_c_preparewritereq
*输入参数:uint16_t hdl 发送ATT的Prepare Write Request指令的Attribute Handle参数
					uint16_t offset 发送ATT的Prepare Write Request指令的Value Offset参数
					uint16_t sz 发送ATT的Prepare Write Request指令的Part Attribute Value参数的大小
					uint8_t *p_buf	发送ATT的Write Command指令的Part Attribute Value参数
*输出参数:uint8_t	指示  att_c_preparewritereq指令是否发送成功
									0	-->失败
									1	-->成功
*说明:该指令只有在当前的GATT是处在client模式下才可使用，调用该接口SYD8821将会发送 Prepare Write Request
命令，关于该命令的具体内容可看规范中ATT的相关章节
			当对方收到 Prepare Write Request后会相应发送WPrepare Write Response数据包，当SYD8821收到该数据包
的时候协议栈的GATT层会向
			gap_s_att_c_evt_handler_set函数指定的接口上报该事件
**************************************************************************************************/
extern uint8_t att_c_preparewritereq(uint16_t hdl, uint16_t offset, uint16_t sz, uint8_t *p_buf);





/*************************************************************************************************
*函数名：att_c_executewritereq
*输入参数:uint8_t flags 发送ATT的Execute Write Request指令的Flags参数 
										0x00 -->Cancel all prepared writes
										0x01 --> Immediately write all pending prepared values
*输出参数:uint8_t	指示  att_c_preparewritereq指令是否发送成功
									0	-->失败
									1	-->成功
*说明:该指令只有在当前的GATT是处在client模式下才可使用，调用该接口SYD8821将会发送 
Execute Write Request命令，关于该命令的具体内容可看规范中ATT的相关章节
			当对方收到 Execute Write Request后会相应发送Execute Write Response数据包，当SYD8821收到该数据包
的时候协议栈的GATT层会向
			gap_s_att_c_evt_handler_set函数指定的接口上报该事件
**************************************************************************************************/
extern uint8_t att_c_executewritereq(uint8_t flags);





/*************************************************************************************************
*函数名：att_c_confirmation
*输入参数:无
*输出参数:uint8_t	指示  att_c_preparewritereq指令是否发送成功 
									0	-->失败
									1	-->成功
*说明:该指令只有在当前的GATT是处在client模式下才可使用，调用该接口SYD8821将会发送 
			Handle Value Confirmation命令，关于该命令的具体内容可看规范中ATT的相关章节
			因为Write Command命令并没有相应数据包，所以这里不会上报任何事件
**************************************************************************************************/
extern uint8_t att_c_confirmation(void);





/*************************************************************************************************
*函数名：bm_s_bond_manager_idx_set
*输入参数:uint8_t idx	设置绑定信息存储位置引索
*输出参数:无
*说明:SYD8821留有五个存储绑定信息的位置，该函数设置使用哪个位置的绑定信息，通过该函数可以实现绑定设备
的切换
			因为目前SYD8821作为从机只能够连接一个主机，所以同一时刻只能够有一个绑定信息
**************************************************************************************************/
extern void bm_s_bond_manager_idx_set(uint8_t idx);





/*************************************************************************************************
*函数名：bm_s_bond_manager_idx_get
*输入参数:uint8_t idx	获取绑定信息存储位置引索
*输出参数:无
*说明:SYD8821留有五个存储绑定信息的位置，该函数获取当前正在使用哪个位置的绑定信息，
			因为目前SYD8821作为从机只能够连接一个主机，所以同一时刻只能够有一个绑定信息
**************************************************************************************************/
extern void bm_s_bond_manager_idx_get(uint8_t *p_idx);





/*************************************************************************************************
*函数名：bm_s_bond_info_get
*输入参数:struct gap_bond_dev *p_device	绑定信息结构体
*输出参数:uint8_t 指示绑定信息是否获取成功 
										0	-->失败
										1	-->成功
*说明:SYD8821留有五个存储绑定信息的位置，该函数获取bm_s_bond_manager_idx_set指定的引索位置的具体绑定
信息内容
			绑定信息的内容请参考gap_bond_dev结构体
**************************************************************************************************/
extern uint8_t bm_s_bond_info_get(struct gap_bond_dev *p_device);





/*************************************************************************************************
*函数名：bm_s_bond_info_add
*输入参数:struct gap_bond_dev *p_device	绑定信息结构体
*输出参数:uint8_t 指示添加绑定信息是否成功
					0	-->失败
					1	-->成功
*说明:SYD8821留有五个存储绑定信息的位置，该函数增加一个绑定设备，因为增加绑定的操作都是在配对完成后由
协议栈增加，所以该函数非特殊情况不建议app使用
			绑定信息的内容请参考gap_bond_dev结构体
**************************************************************************************************/
extern uint8_t bm_s_bond_info_add(struct gap_bond_dev *p_device);





/*************************************************************************************************
*函数名：bm_s_bond_info_delete_all
*输入参数:无
*输出参数:uint8_t 指示删除绑定信息是否成功
					0	-->失败
					1	-->成功
*说明:SYD8821留有五个存储绑定信息的位置，该函数将删除所有的绑定信息，之后的设备都将重新绑定
**************************************************************************************************/
extern uint8_t bm_s_bond_info_delete_all(void);





/*************************************************************************************************
*函数名：bm_s_bond_info_delete
*输入参数:无
*输出参数:uint8_t 指示删除绑定信息是否成功
					0	-->失败
					1	-->成功
*说明:SYD8821留有五个存储绑定信息的位置，该函数将删除bm_s_bond_manager_idx_set指定的引索位置的绑定信息
**************************************************************************************************/
extern uint8_t bm_s_bond_info_delete(void);





/*************************************************************************************************
*函数名：ecdh_public_key_get
*输入参数:无
*输出参数:
*说明:
**************************************************************************************************/
extern void ecdh_public_key_get(uint8_t *p_x, uint8_t *p_y);





/*************************************************************************************************
*函数名：sys_mcu_clock_get
*输入参数:uint8_t *p_sel  返回当前时钟源的指针，其属于枚举_MCU_CLOCK_SEL_的成员
*输出参数:无
*说明:该函数获取当前mcu时钟源，
**************************************************************************************************/
extern void sys_mcu_clock_get(uint8_t *p_sel);





/*************************************************************************************************
*函数名：sys_mcu_clock_set
*输入参数:uint8_t  sel  设置MCU时钟源，其属于枚举_MCU_CLOCK_SEL_的成员
*输出参数:无
*说明:该函数设置mcu时钟源，因为MCU的时钟必须要经过校准才能够使用，所以调用了该函数后必须紧接着调用
sys_mcu_rc_calibration函数
**************************************************************************************************/
extern void sys_mcu_clock_set(uint8_t  sel);





/*************************************************************************************************
*函数名：sys_mcu_clock_div_set
*输入参数:
*输出参数:无
*说明:
**************************************************************************************************/
extern void sys_mcu_clock_div_set(uint8_t  div);
extern void sys_mcu_clock_div_get(uint8_t *p_div);





/*************************************************************************************************
*函数名：sys_32k_clock_get
*输入参数:uint8_t *p_sel  返回当前时钟源的指针，其属于枚32K_CLOCK_SEL_成员
*输出参数:无
*说明:该函数获取当前32.768KHz低频时钟源，因为高频时钟运行会带来很高的功耗，所以SYD8821的系统既有高频的
时钟源给MCU使用，也有低频的时钟源给timer使用
**************************************************************************************************/
extern void sys_32k_clock_get(uint8_t *p_sel);





/*************************************************************************************************
*函数名：sys_32k_clock_set
*输入参数:uint8_t sel  返回当前时钟源的指针，其属于枚32K_CLOCK_SEL_成员
*输出参数:无
*说明:该函数设置32.768KHz低频时钟源，因为高频时钟运行会带来很高的功耗，所以SYD8821的系统既有高频的时钟源
给MCU使用，也有低频的时钟源给timer使用
**************************************************************************************************/
extern void sys_32k_clock_set(uint8_t sel);





/*************************************************************************************************
*函数名：sys_32k_lpo_calibration
*输入参数:无
*输出参数:uint8_t
					0	-->失败
					1	-->成功
*说明:该函数用于校准芯片内部低频时钟源(也称为LPO，为内部32.768KHz RC振荡器)，因为RC振荡器有受温度影响的
特性，
			所以APP如果使用内部RC振荡器的话要做到以一定的间隔去校准内部RC晶振，这里推荐的间隔是3-10分钟，建议
使用3分钟
			不校准的内部RC振荡器会存在很大的偏差，BLE连接会出现断线以及连接不上的问题
**************************************************************************************************/
extern uint8_t sys_32k_lpo_calibration(void);





/*************************************************************************************************
*函数名：sys_mcu_rc_calibration
*输入参数:无
*输出参数:无
*说明:该函数用于校准芯片内高频时钟源，因为高速RC振荡器在上电的时候存在偏差比较大的情况，
			所以APP如果使用内部RC振荡器的话要做到在开机的选择完MCU时钟之后校准一次高频时钟源
**************************************************************************************************/
extern void sys_mcu_rc_calibration(void);





/*************************************************************************************************
*函数名：pmu_wakeup_config
*输入参数:struct gap_wakeup_config *p_cfg	唤醒源配置结构体
*输出参数:无
*说明:为了节省功耗，BLE蓝牙并不是时时刻刻工作的，应该说很长的一段时间都是睡眠状态，那自然就会产生关于睡眠
的设置，目前SYD8821能够唤醒MCU的有三个：定时器、GPIO、蓝牙中断
			关于GPIO的相关配置要谨慎，其有两个选项gpi_wakeup_en和gpi_wakeup_cfg很关键，前者代表是否使能GPIO唤
醒MCU，后者代表哪些管脚唤醒MCU，gpi_wakeup_cfg是bit_mask
			表示的，比如gpi_wakeup_cfg=0x00000900代表GPIO11和GPIO8能够唤醒
**************************************************************************************************/
extern void pmu_wakeup_config(struct gap_wakeup_config *p_cfg);





/*************************************************************************************************
*函数名：pmu_mcu_off
*输入参数:无
*输出参数:无
*说明:该函数将关闭MCU，内存和外设的状态还在
**************************************************************************************************/
extern void pmu_mcu_off(void);





/*************************************************************************************************
*函数名：pmu_system_off
*输入参数:无
*输出参数:无
*说明:该函数将关闭芯片所有模块，内存和外设等都会被关闭
**************************************************************************************************/
extern void pmu_system_off(void);





/*************************************************************************************************
*函数名：pmu_reset
*输入参数:uint32_t reset_type	复位类型,其属于枚举PMU_RESET_FLAG_TYPE的成员
*输出参数:无
*说明:这里在MCU_RESET模式下只是复位MCU和一些数字电路，而GPIO,TIMER,RTC，pem_led，watchdog这些外设是没有
被复位掉的,SYSTEM_RESET会把所有模块都复位掉
**************************************************************************************************/
extern void pmu_reset(uint32_t reset_type);





/*************************************************************************************************
*函数名：pmu_system_reset
*输入参数:无
*输出参数:无
*说明:系统复位，相当于：pmu_reset(SYSTEM_RESET);
**************************************************************************************************/
extern void pmu_system_reset(void);





/*************************************************************************************************
*函数名：pmu_mcu_reset
*输入参数:无
*输出参数:无
*说明:MCU复位，相当于：pmu_reset(MCU_RESET);
**************************************************************************************************/
extern void pmu_mcu_reset(void);





/*************************************************************************************************
*函数名：ble_flash_erase
*输入参数:uint32_t address  擦除的地址，这是物理地址
					uint8_t num	要擦除的扇区数
*输出参数:uint8_t	指示是否擦除成功
									0 --> 失败
									1	-->	成功
*说明:ble_flash_erase、ble_flash_read以及ble_flash_write三个函数构成了芯片内部flash的三个操作函数，
app可以使用这三个函数对芯片内部flash进行操作
			在调用ble_flash_write之前必须调用ble_flash_erase进行擦除，并且做好备份，因为ble_flash_erase擦除
的单位是扇区，也就是4096个byte，
			所以当ble_flash_write的地址范围是没有写过，那就不需要再次调用ble_flash_erase函数，比如擦除了第五
个扇区（0x4000)，第一次写0x4000-0x4100,
			第二次写0x4100-0x4200就不需要再次擦除了，因为他们的地址并没有重叠而且还在同一个扇区。
			SYD8821对于擦除flash空间的次数是有限制的（一般为10万次）,所以不建议频繁的擦除
**************************************************************************************************/
extern uint8_t ble_flash_erase(uint32_t address,uint8_t num);





/*************************************************************************************************
*函数名：ble_flash_read
*输入参数:uint32_t address  要读取的地址，这是物理地址
					uint16_t len	要读取的长度
					uint8_t *pbuf	返回数据的指针
*输出参数:uint8_t	指示是否操作成功
									0 --> 失败
									1	-->	成功
*说明:该函数负责从内部flash中读取数据，
			为了防止读取到代码的区域，所以这里要求代码区的前面48Kbyte空间不可读
**************************************************************************************************/
extern uint8_t ble_flash_read(uint32_t address,uint16_t len, uint8_t *pbuf);





/*************************************************************************************************
*函数名：ble_flash_write
*输入参数:uint32_t address  要写入的地址，这是物理地址
					uint16_t len	要写入的长度
					uint8_t *pbuf	写入数据的指针
*输出参数:uint8_t	指示是否操作成功
									0 --> 失败
									1	-->	成功
*说明:该函数负责写入数据到内部falsh空间中，SYD8821内部的flash空间也是通用的flash，所以遵循flash操作的
规范，在调用写函数之前必须调用擦除函数
			因为调用了擦除函数之后，flash内部空间的数据就回复成默认值了，所以在擦除之前一定要调用读取函数先把
整个扇区读取到内存空间的数组中，
			然后修改内存的数组的内容，随之调用擦除函数，再把内存中的数组写入flash中
**************************************************************************************************/
extern uint8_t ble_flash_write(uint32_t address,uint16_t len, uint8_t *pbuf);





/*************************************************************************************************
*函数名：ble_flash_write_burst
*输入参数:uint32_t address  要写入的地址，这是物理地址
					uint16_t len	要写入的长度
					uint8_t *pbuf	写入数据的指针
					uint8_t flush	传入0
*输出参数:uint8_t	指示是否操作成功
									0 --> 失败
									1	-->	成功
*说明:该函数负责写入数据到内部falsh空间中，SYD8821内部的flash空间也是通用的flash，所以遵循flash操作的
规范，在调用写函数之前必须调用擦除函数
			因为调用了擦除函数之后，flash内部空间的数据就回复成默认值了，所以在擦除之前一定要调用读取函数先把
整个扇区读取到内存空间的数组中，
			然后修改内存的数组的内容，随之调用擦除函数，再把内存中的数组写入flash中
**************************************************************************************************/
extern uint8_t ble_flash_write_burst(uint32_t address,uint16_t len, uint8_t *pbuf,uint8_t flush);





/*************************************************************************************************
*函数名：ota_code_erase
*输入参数:无
*输出参数:uint8_t	指示是否擦除成功
									0 --> 失败
									1	-->	成功
*说明:SYD8821的flash中划分了两个代码区，一个作为当前代码的运行区，一个作为ota过程中存储代码的备份去，这样
即使在OTA过程中出错依旧能够继续运行代码区的代码，
			不至于死机，如果ota的数据是正确的，就把备份区作为运行区，而原来的运行区就自然变成了备份区，OTA分为三
个阶段，擦除，写以及检验，
			调用ota_code_erase函数将会擦除flash内部的备份代码区的数据
			该函数只是作用于内部有两个代码区的形式，如果把整个flash空间都作为一个代码区，那该函数将不再适用
**************************************************************************************************/
extern uint8_t ota_code_erase(void);





/*************************************************************************************************
*函数名：ota_code_write
*输入参数:uint32_t offset	当前数据包中的flash数据相对于ota开始数据的偏移
					uint16_t len	本数据包中的有效flash数据的长度
					uint8_t *p_buf	此次数据包中flash数据的指针
*输出参数:uint8_t	指示是否写数据成功
									0 --> 失败
									1	-->	成功
*说明:SYD8821的flash中划分了两个代码区，一个作为当前代码的运行区，一个作为ota过程中存储代码的备份去，这样
即使在OTA过程中出错依旧能够继续运行代码区的代码，不至于死机，如果ota的数据是正确的，就把备份区作为运行区，
而原来的运行区就自然变成了备份区，OTA分为三个阶段，擦除，写以及检验，
			调用ota_code_erase函数将会擦除flash内部的备份代码区的数据
			该函数只是作用于内部有两个代码区的形式，如果把整个flash空间都作为一个代码区，那该函数将不再适用
			SYD8801的ota机制是把ota文件分成很多个数据包，然后逐条下发的形式
**************************************************************************************************/
extern uint8_t ota_code_write(uint32_t offset , uint16_t len, uint8_t *p_buf);





/*************************************************************************************************
*函数名：ota_code_update
*输入参数:uint8_t *p_desc	ota代码的描述
					uint8_t *p_ver	ota代码的版本
					uint32_t sz	ota代码的大小
					uint16_t checksum	ota代码的检验和
*输出参数:uint8_t	指示是否更新（校验）成功
									0 --> 失败
									1	-->	成功
*说明:SYD8821的flash中划分了两个代码区，一个作为当前代码的运行区，一个作为ota过程中存储代码的备份去，
这样即使在OTA过程中出错依旧能够继续运行代码区的代码，不至于死机，如果ota的数据是正确的，就把备份区作为运行区，
而原来的运行区就自然变成了备份区，OTA分为三个阶段，擦除，写以及检验，
			该函数负责计算flash中的数据的检验值是否等于参数checksum，如果不等于则返回失败，这时复位运行的程序是
原来未OTA的程序，p_desc和p_ver不会起作用，
			如果检验值相等，该函数将更新配置文件中的p_desc和p_ver，而后返回成功！
			注意：ota_code_update函数中并不会发起复位操作，app在调用ota_code_update后可以发起复位的操作，如果
检验成功，则OTA成功，否则OTA失败
**************************************************************************************************/
extern uint8_t ota_code_update(uint8_t *p_desc, uint8_t *p_ver, uint32_t sz, uint16_t checksum);





/*************************************************************************************************
*函数名：ble_sched_execute
*输入参数:无
*输出参数:无
*说明:该函数为ble调度函数，所谓的调度函数就是说该函数会去判断蓝牙的状态而执行一些行为，因为flash的操作比
较超时，如果在蓝牙协议栈需要进行大量而且重要的工作的时候
		 （比如配对成功后后保存绑定信息的操作）操作flash势必会阻塞蓝牙协议栈，所以这里会把一些flash的操作放
到该函数来运行，同时gap_s_profile_data_write也是在该函数中其作用，但是ble_flash_erase，ble_flash_read，
ble_flash_write，ota_code_erase，ota_code_write，ota_code_update函数都是立即起效的
			必须要在app的while（1）主循环中调用该函数，
**************************************************************************************************/
extern void ble_sched_execute(void);





/*************************************************************************************************
*函数名：BBRFWrite
*输入参数:uint8_t addr	BBR寄存器的地址偏移
					uint8_t data	要写入的数据
*输出参数:无
*说明:BBR寄存器是SYD8821里面比较特殊的一些寄存器，该函数负责写BBR寄存器
			BBR寄存器涉及到整个芯片的行为，不正确的写法有可能造成芯片错乱，所以这些寄存器的操作必须要在官方的正
确说明指导下进行
**************************************************************************************************/
extern void BBRFWrite(uint8_t addr, uint8_t data);





/*************************************************************************************************
*函数名：BBRFRead
*输入参数:uint8_t addr	BBR寄存器的地址偏移
					uint8_t data	要读出的数据指针
*输出参数:无
*说明:BBR寄存器是SYD8821里面比较特殊的一些寄存器，该函数负责读出BBR寄存器的内容
			BBR寄存器涉及到整个芯片的行为，不正确的写法有可能造成芯片错乱，所以这些寄存器的操作必须要在官方的
正确说明指导下进行
**************************************************************************************************/
extern void BBRFRead(uint8_t addr, uint8_t* data);





/*************************************************************************************************
*函数名：ota_code_update_496kb
*输入参数:uint8_t *p_desc	ota代码的描述
					uint8_t *p_ver	ota代码的版本
					uint16_t sz	ota代码的大小
					uint16_t checksum	ota代码的检验和
*输出参数:无
*说明:SYD8821的flash除了正常的两个248KB代码区的模式外，依旧支持一个大的496的代码区的形式，但是在496kB
的模式下OTA必须要在芯片外部增加一个flash芯片，ota过程中把代码放到该芯片中，在下载代码进外部flash完成后
，二次BootLoader代码（二次BootLoader是在SYD8821的基础上再建立一个独立于ROM区域的代码，该代码可以放在
代码中，在需要的时候拷贝入内存运行，也可以直接放在芯片内部falsh中，但是要区分好区域）负责把外部flash的
代码拷贝到内部flash中
			该函数负责把各个参数指定的内容写入到芯片内部的配置区，之后复位运行才能够运行到正确的496KB代码
			注意：ota_code_update函数中并不会发起复位操作，app在调用ota_code_update后可以发起复位的操作，如果
检验成功，则OTA成功，否则OTA失败
**************************************************************************************************/
extern uint8_t ota_code_update_496kb(uint8_t *p_desc, uint8_t *p_ver, uint32_t sz, uint16_t checksum);




/*************************************************************************************************
*函数名：_checksum_cache_496kb
*输入参数:uint32_t adr 496kb代码存在内部flash的位置，此为物理位置
					uint16_t sz	ota代码的大小
					uint16_t checksum	ota代码的检验和
*输出参数:无
*说明:SYD8821的flash除了正常的两个248KB代码区的模式外，依旧支持一个大的496的代码区的形式，但是在496kB
的模式下OTA必须要在芯片外部增加一个flash芯片，ota过程中把代码放到该芯片中，在下载代码进外部flash完成后
，二次BootLoader代码（二次BootLoader是在SYD8821的基础上再建立一个独立于ROM区域的代码，该代码可以放在
代码中，在需要的时候拷贝入内存运行，也可以直接放在芯片内部falsh中，但是要区分好区域）负责把外部flash的
代码拷贝到内部flash中
			该函数负责计算flash中的数据的检验值是否等于参数checksum，如果不等于则返回失败
			相对于ota_code_update函数，该函数拥有检验整个496kb代码的能力
**************************************************************************************************/
extern uint8_t _checksum_cache_496kb(uint32_t adr,uint32_t sz, uint16_t checksum);



/*************************************************************************************************
*函数名：gap_s_smart_update_latency
*输入参数:struct gap_smart_update_params *p_smart_params  智能连接参数结构体
*输出参数:无
*说明:因为规范中并没有关于latency的管理的相关说明，所以这里SYD8821增加一个智能管理连接参数和latency
的机制，叫做smart_update_latency，该函数负责管理latency的开始，和连接参数的更新操作，在连接参数请求
被主机拒绝的时候，该函数负责动态的调整连接参数再次请求更新，当有数据进行交互的时候该函数动态的开启
或者关闭latency以实现功耗的匹配。
			当然，该函数是独立于原来的协议栈机制的，也就是说不调用该函数，对原来的协议栈的行为是没有任何
影响的
			因为latency和连接参数等更新涉及到蓝牙的稳定性和兼容性等，所以这里建议使用智能管理机制
			smart_update_latency和协议栈本身是独立的，所以如果不调用该函数对于协议栈的功能是没有任何的影响
**************************************************************************************************/
extern uint8_t gap_s_smart_update_latency(struct gap_smart_update_params *p_smart_params);




/*************************************************************************************************
*函数名：SystemSleep
*输入参数:POWER_SAVING_TYPE mode	电源省电类型，为POWER_SAVING_TYPE枚举成员
					MODULE_CONTROL_TYPE c 模块控制类型，为MODULE_CONTROL_TYPE枚举类型
					uint32_t ldo_delay	MCU唤醒后“nop"执行执行的次数
					PMU_WAKEUP_CONFIG_TYPE	唤醒源使能控制
*输出参数:无
*说明:该函数使SYD8821进入浅度睡眠模式，低功耗的程度由mode和c两个变量控制，对于该函数不可传入带“RESET”
的变量，
			建议调用方式：SystemSleep(POWER_SAVING_RC_OFF, FLASH_LDO_MODULE, 11000 , 
												(PMU_WAKEUP_CONFIG_TYPE)(FSM_SLEEP_EN|PIN_WAKE_EN|TIMER_WAKE_EN|RTC_WAKE_EN));
**************************************************************************************************/
extern uint8_t SystemSleep(POWER_SAVING_TYPE mode, MODULE_CONTROL_TYPE c,uint32_t ldo_delay,PMU_WAKEUP_CONFIG_TYPE w);







/*************************************************************************************************
*gap_s_att_mtu_get
*输入参数:无
*输出参数:uint16_t 当前att层的MTU
*说明:该函数获取到当前att层的mtu，该mtu为主机和从机商量得到的结果，应用程序发送的最大的数据包不能够
大于该函数返回的mtu大小
			建议调用方式：mtu_now=gap_s_att_mtu_get;
**************************************************************************************************/
extern uint16_t gap_s_att_mtu_get(void);



/*************************************************************************************************
*ble_SetTxPower
*输入参数:BLE_TX_POWER value RF发射功率值，为BLE_TX_POWER举成员
*输出参数:无
*说明:该函数设置RF发射功率，上电后ble_init之后设置有效，RF发射功率默认为0dbm
**************************************************************************************************/
extern void ble_SetTxPower(BLE_TX_POWER value);

/*************************************************************************************************
*gap_s_att_mtu_get
*输入参数:无
*输出参数:uint8_t 当前硬件TX buffer状态
*说明:该函数获取到当前TX硬件buffer的状态是否为空
			返回1：无数据；返回0：有数据
**************************************************************************************************/			
extern uint8_t BBCheckTXFIFOEmpty(void);

/*************************************************************************************************
*ble_SetTxPower
*输入参数:uint8_t timer 连接事件中最后一个数据包到下一个连接事件到来的时间，单位是30.25us
*输出参数:无
*说明:该函数的参数（简称TH_LAST）决定了一个连接事件中最后一个moredata数据包到下一个连接事件的时间，
单位为30.5us，也就是说该函数的参数也到，那么一个连接事件内的数据包数目越小，
**************************************************************************************************/
extern void ll_set_replying_packet_timer(uint8_t timer);

/*************************************************************************************************
*amic_set_bias
*输入参数:AMIC_BIAS value amic模块的bias值，为AMIC_BIAS举成员
*输出参数:无
*说明:该函数设置amic模块的bias值，上电后ble_init之后设置有效，amic模块的bias值默认为1.4V
**************************************************************************************************/
extern void amic_set_bias(AMIC_BIAS value);


extern void gap_s_adv_start_powersaving(void);
extern void gap_s_adv_stop_powersaving(void);

extern uint8_t gap_s_verdor_datas_get(uint8_t *datas);
extern uint8_t gap_s_verdor_datas_set(uint8_t *datas);
extern void systemtick_delay(uint16_t n);
extern void sys_mcu_rc_calibrationHF(void);
extern uint8_t Setting4kUpdate(uint8_t *data,  uint32_t checksum,  uint8_t Xor);
extern void SetDevAddr_toflash(struct gap_ble_addr* p_dev);
extern uint8_t flash_peotect_key_confirm(uint32_t key);
extern uint8_t bm_s_bond_info_isvalid(void);
#endif
