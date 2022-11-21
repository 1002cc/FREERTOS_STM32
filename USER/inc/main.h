#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"
#include <stdlib.h>
#include "led.h"
#include "key.h"
#include "usart.h"
#include "hc_sr04.h"
#include "dht11.h"
#include "pwmsg.h"
#include "sys.h"
#include "delay.h"
#include "adc.h"
#include "oled.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "rtc.h"
#include "iwdg.h"
#include "esp8266.h"
#include "tim.h"
#include "i2c.h"
#include "oled_u8g2.h"
#include "menu.h" 
#include "rfid.h"
#include "cjson.h"
#include "esp8266.h"
#include "mqtt_app.h"

//检查代码
#define TEST printf("正在执行%s函数第%d行代码\r\n", __FILE__, __LINE__);

//创建任务结构体
typedef struct __task_t
{
	TaskFunction_t pxTaskCode;
	const char * const pcName;		
	const uint16_t usStackDepth;
	void * const pvParameters;
	UBaseType_t uxPriority;
	TaskHandle_t * const pxCreatedTask;
}task_t;

//开始任务
//===================================================================
#define SOFTWARE_CONTROL_STK_SIZE 		128 		//任务堆栈大小
#define SOFTWARE_CONTROL_TASK_PRIO 		1  		 	//任务优先级
void software_control_task(void* pvParameters);		//任务函数
TaskHandle_t SOFTWARE_CONTROL_Handler;				//任务句柄
//运行任务
/*--------------------------------------------------------------------*/
/*多功能按键*/		
void key_switch_task(void* pvParameters);
/*按键*/
void key_one_task(void* pvParameters);
/*蓝牙串口接收数据处理*/
void bluetooth_data_task(void* pvParameters);
/*读取温湿度数据数据*/	
void dht11_data_task(void* pvParameters);
/*读取ADC烟雾报警器数据*/
void adc_data_task(void* pvParameters);
/*读取亮度数据数据*/
void ligth_data_task(void* pvParameters);
/*超声波监测*/
void sg_hc_monitor_task(void* pvParameters);
/*OLED显示*/
void oled_task(void* pvParameters);
/*RTC时间*/
void get_time_task(void* pvParameters);
/*屏保*/
static void screensaver_task(void* pvParameters);
/*USART1接收数据处理*/
static void usart1_task(void* pvParameters);
/*RFID解锁*/
static void  rfid_task(void* pvParameters);
/*矩阵解锁显示*/
static void  key_pro_task(void* pvParameters);
/*矩阵解锁显示*/
static void  key4x4_task(void* pvParameters);
/*MQTT*/
static void  esp8266_mqtt_task(void* pvParameters);
/*mqtt数据处理*/
static void sub_pub_task(void*pvParameters);
/*看门狗*/
void iwdg_task(void* pvParameters);

/*任务句柄*/
TaskHandle_t KEY_SWITCH_Handler;
TaskHandle_t KEY_ONE_Handler;
TaskHandle_t BLUETOOTH_DATA_Handler;
TaskHandle_t DHT11_DATA_Handler;
TaskHandle_t ADC_DATA_Handler;
TaskHandle_t LIGTH_DATA_Handler;
TaskHandle_t SG_HC_MONITOR_Handler;
TaskHandle_t OLED_Handler;
TaskHandle_t RTC_Handler;
TaskHandle_t SCREENSAVER_Handler;
TaskHandle_t USART1_Handler;
TaskHandle_t RFID_Handler;
TaskHandle_t  KEY4x4_PRO_Handler;
TaskHandle_t  KEY4x4_Handler;
TaskHandle_t  MQTT_ESP_Handler;
TaskHandle_t SUB_PUB_Handler;
TaskHandle_t IWDG_Handler;

/*--------------------------------------------------------------------*/

static const task_t task_su[]={
	{key_switch_task, 	  	"key_switch_task", 		2048, 	NULL, 	18, &KEY_SWITCH_Handler		},
	{key_one_task, 			"key_one_task", 		64, 	NULL, 	18, &KEY_ONE_Handler		},
	{bluetooth_data_task, 	"bluetooth_data_task",	512, 	NULL, 	10, &BLUETOOTH_DATA_Handler	},
	{dht11_data_task, 		"dht11_data_task", 		128, 	NULL, 	12, &DHT11_DATA_Handler		},
	{adc_data_task, 		"adc_data_task", 		128, 	NULL, 	12, &ADC_DATA_Handler		},
	{ligth_data_task,		"ligth_data_task", 		128, 	NULL, 	12, &LIGTH_DATA_Handler		},
	{get_time_task, 		"get_time_task", 		128, 	NULL, 	17, &RTC_Handler			},
	{sg_hc_monitor_task,	"sg_hc_monitor_task", 	64, 	NULL, 	6,  &SG_HC_MONITOR_Handler	},
	{screensaver_task, 		"screensaver_task",		128,	NULL, 	20, &SCREENSAVER_Handler	},
	{usart1_task, 			"usart1_task", 			128, 	NULL, 	10, &USART1_Handler			},
	{rfid_task, 			"rfid_task", 			128, 	NULL, 	12, &RFID_Handler			},
	{key4x4_task, 			"key4x4_task", 			128, 	NULL, 	12, &KEY4x4_Handler			},
	{key_pro_task, 			"key_pro_task", 		128, 	NULL, 	12, &KEY4x4_PRO_Handler		},
	{esp8266_mqtt_task, 	"esp8266_mqtt_task", 	1024*4, NULL, 	12, &MQTT_ESP_Handler		},
	{iwdg_task, 			"iwdg_task", 			32, 	NULL, 	15, &IWDG_Handler			}
};


/*MFRC522数据区*/
u8  mfrc552pidbuf[18];
u8  card_pydebuf[2];
u8  card_numberbuf[5];
u8  card_password[5]={0x39,0x59,0x0A,0x4D,0x27}; //已固定好的卡号    
u8  card_key0Abuf[6]={0xff,0xff,0xff,0xff,0xff,0xff};						//密钥一,初始状态位6个0xff
u8  card_writebuf[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
u8  card_readbuf[18];


//消息队列句柄
/*--------------------------------------------------------------------*/
//矩阵按键数值
QueueHandle_t KEY_ONE_Queue;
QueueHandle_t KEY_NUM_Queue;
//传感器信息结构体
QueueHandle_t MESSAGE_Queue;
//传感器
QueueHandle_t USART1_Queue;
QueueHandle_t USART2_Queue;
QueueHandle_t USART3_Queue;
QueueHandle_t OLED_Queue;
QueueHandle_t MQ_Queue;
QueueHandle_t MQ1_Queue;

#define QUEUE_USART_LEN  1  
#define MESSAGE_Length   1
#define KEY_NUM_Length   1
#define KEY_ONE_Length   1
/*--------------------------------------------------------------------*/
//二值信号量句柄
/*--------------------------------------------------------------------*/
SemaphoreHandle_t BinarySemaphore=NULL;  //   0/1 创建是为空
/*--------------------------------------------------------------------*/

//多级菜单
void (*current_operation_index)(void);
u8 func_index = 0; //初始显示欢迎界面
u8 last_index = 127; //last初始为无效值

//u8g2图形库结构体
u8g2_t u8g2;

uint16_t mq2_vol;
uint16_t adc_fire_vol;

u32 alarm_flag=0;
u32 bluetooth_flag=0;
u32 wifi_flag=0;
u32 s=0;

/*--------------------------------------------------------------------*/
#endif
