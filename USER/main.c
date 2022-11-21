#include "main.h"
/*
 *	━━━━━━━━━神兽出没━━━━━━━━━
 *       ┏┓　　　┏┓+ +
 *　　　┏┛┻━━━━━━┛┻━━┓ + +
 *　　　┃　　　　　　┃ 　
 *　　　┃　　　━　　 ┃ ++ + + +
 *　　  ┃  ████━████ ┃+
 *　　　┃　　　　　　┃ +
 *　　  ┃　　　┻　　 ┃
 *　　　┃　　　　　　┃ + +
 *　　　┗━━━┓　　  ┏━┛
 *　　　　　┃　　　┃　　　　　　　　　　　
 *　　　　　┃　　　┃ + + + +
 *　　　　　┃　　　┃　　　　Codes are far away from bugs with the animal protecting　　　
 *　　　　　┃　　　┃ + 　　　　神兽保佑,代码无bug　　
 *　　　　　┃　　　┃
 *　　　　　┃　　　┃　　+　+ + + +　　　　　　　　
 *　　　　　┃　 　 ┗━━━━━┓ + +
 *　　　　　┃ 　　　　　　　  	 ┣━━┓
 *　　　　　┃ 　　　　　　　 	┏┛
 *　　　　　┗━┓ ┏━━┓ ┏━━┛ + + + +
 *　　　　　　┃┫┫　┃┫┫
 *　　　　　　┗┻┛　┗┻┛+ + + +
 ****************************************************
 *
 */
							/*系统逻辑*/
 /*
								显示温度和湿度
									^
									|
									|						----->WIFI----->MQTT------->微信小程序
	OLED(主页面)----->开机动画----->显示时间--------------------->蓝牙
									|[显示切换]--->多级菜单
									|[控制]按键和电子陀螺仪切换(无)
									|		
							用户无操作进入屏保模式
*/
 
//传感器模块引脚配置
/*
********************************************************************************************
*		PA9  PA10 	使用串口1	XCOM串口软件
*------------------------------------------------------------------------------------
*		PB10 PB11 	使用串口2   wifi模块esp8266	微信小程序控制 自己开发的图形操作控制小程序
*		WIFI控制方案：
*					
*------------------------------------------------------------------------------------
*		PA2  PA3  	使用串口3   HC_05蓝牙模块   手机软件控制：	蓝牙调试宝
* 		蓝牙控制方案:
*					
*------------------------------------------------------------------------------------
*		OLED:PB8 PB9
*		OLED显示： 	1.当前时间
*					2.用户智能锁
*					3.按键控制
*					4.信息提示
*------------------------------------------------------------------------------------
*		电子陀螺仪:控制显示操作
*------------------------------------------------------------------------------------
*		超声波模块:PC6 输出模式  PC7输入模式
*		舵机：PE5 定时器9  1通道
*		超声波监测：超声波模块测量目标,舵机180度自动转动,
*					超声波模块检查到目标，舵机停下,并控制LED灯和蜂鸣器提示和警告
*------------------------------------------------------------------------------------
*		矩阵按键(输入密码)：
*					消息队列---->传递键值
*					OUT: 	C1(PE11) C2(   PE12) C3(PE15) C4(PD9)
*					IN: 	R4(PE9) R3(PE10) R2(PD0) R1(PD14)

	//OUT: C1(PE11) C2(PE12) C3(PE15) C4(PD9) 
	//IN:  R4(PE9) R3(PE10) R2(PD0) R1(PD14)
*------------------------------------------------------------------------------------
*		光敏电阻：PE7
*					ADC3 通道5
*					DMA传输数据 数据流0 通道3
*------------------------------------------------------------------------------------
*		模拟LED灯使用场景
*		LED1 (PF9)  楼道走廊灯 由关照强度自动开启  也可以手机和按键(1)控制
*		LED2 (PF10) 超声波监测提示警告灯	有人靠近灯会亮起，时间越长灯越亮 没有则灯亮
*		LED3 (PE13) 家庭使用灯 由手机和按键控制
*		LED4 (PE14) 1.程序运行失败指示灯,
*					2.密码输入失败灯亮
*------------------------------------------------------------------------------------
*		蜂鸣器使用
*		BEEP (PF8)  1.蜂鸣器警报,有人长时间停留则蜂鸣器响
*					2.烟雾报警器触发
*					3.密码和RIE输入三次错误响
*					4.手机控制警报,警示作用
*		烟雾报警器 A0(ADC采集)PB0 ADC12 IN8  DO(PB1)中断3 4
*		火焰传感器 
*------------------------------------------------------------------------------------
*		按键功能:
				单击
*				KEY1:向左切换
*				KEY2:向右切换
*				KEY3:关闭
*				KEY4:返回
********************************************************************************************
*/

									/*引脚使用图*/
/*
 * 
 * ┌───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┐
 * │ GPIO  │   0   │   1   │   2   │   3   │   4   │   5   │   6   │   7   │   8   │   9   │  10   │   11  │   12  │   13  │   14  │   15  │
 * └───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┘
 * ┌───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┐
 * │  PA   │  key1 │       │USART3 │ USART3│       │       │       │       │       │ USART1│ USART1│       │       │       │       │       │
 * └───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┘
 * ┌───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┐
 * │  PB   │ 烟雾  │       │       │       │       │       │       │       │ OLED  │  OLED │       │       │       │       │       │       │
 * └───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┘
 * ┌───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┐
 * │  PC   │       │       │       │       │       │       │       │       │       │       │       │       │       │       │       │       │
 * └───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┘
 * ┌───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┐
 * │  PD   │   R2  │       │       │       │       │       │       │       │       │   C4  │       │       │       │       │   R1  │       │
 * └───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┘
 * ┌───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┐
 * │  PE   │       │       │  key2 │  key3 │  key4 │  舵机 │       │   	   │   R4  │  R3   │       │   C1  │   C2  │ LED3  │ LED4  │   C3  │
 * └───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┘
 * ┌───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┐
 * │  PF   │       │       │       │       │       │       │       │ 光敏  │  BEEP │   LED1│  LED2 │       │       │       │       │       │
 * └───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┘
 * ┌───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┐
 * │  PG   │       │       │       │       │       │       │       │       │       │ DHT11 │       │       │       │       │       │       │
 * └───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┘
 */
/*函数优先级问题*/
/*
	FREERTOS 中断优先级要在5~32范围内
	不能低于5
*/
/*临界段问题*/
/*
	保护代码一定执行
	taskENTER_CRITICAL(); 	//进入临界区
	taskEXIT_CRITICAL();	//退出临界区
	对于vTaskDelay(1000);delay_us 都会引起任务切换 让该任务变为堵塞态,然后去执行其他任务
	又因为优先级问题,就算延迟到了，不一定会继续执行该函数代码，此时该函数变为就绪态，等待被执行
	对于一些对延时有要求的设备或代码，为了保证其顺利执行，可以在前后加上临界段代码保护
										
*/
/*中断处理函数讲究快进快出*/
/*任务创建*/
/*
	static void 函数名(void *pvParameters)
	{
		while(1)或for(;;)
		{
			//一定要加延迟函数,触发任务切换
			//不然会死循环
			//系统卡住,其他任务不会执行
			vTaskDelay(5000);
		}
	}
*/

const unsigned char wifi_m[] U8X8_PROGMEM = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf0,0x0f,0x00,0x00,0x00,0x80,0xff,0xff,0x01,0x00,0x00,0xe0,0xff,0xff,0x07,0x00,0x00,0xf8,0xff,0xff,0x1f,0x00,0x00,0xfe,0xff,0xff,0x7f,0x00,0x80,0xff,0xff,0xff,0xff,0x01,0xc0,0xff,0x03,0xc0,0xff,0x03,0xe0,0x7f,0x00,0x00,0xfe,0x07,0xf0,0x1f,0x00,0x00,0xf8,0x0f,0xf8,0x07,0x00,0x00,0xe0,0x1f,0xfc,0x03,0x00,0x00,0xc0,0x3f,0xfe,0x00,0xc0,0x03,0x00,0x7f,0x7f,0x00,0xfc,0x3f,0x00,0xfe,0x7f,0x80,0xff,0xff,0x01,0xfe,0x3f,0xe0,0xff,0xff,0x07,0xfc,0x1f,0xf0,0xff,0xff,0x0f,0xf8,0x04,0xf8,0xff,0xff,0x1f,0x60,0x00,0xfc,0x07,0xe0,0x3f,0x00,0x00,0xfe,0x01,0x80,0x7f,0x00,0x00,0x7f,0x00,0x00,0xfe,0x00,0x80,0x3f,0x00,0x00,0xfc,0x01,0x80,0x1f,0x00,0x00,0xf8,0x01,0xc0,0x0f,0x00,0x00,0xf0,0x03,0x80,0x07,0xf8,0x1f,0xe0,0x03,0x80,0x03,0xfe,0x7f,0xc0,0x01,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,0x01,0x00,0x00,0x80,0xff,0xff,0x03,0x00,0x00,0xc0,0x1f,0xf8,0x03,0x00,0x00,0xe0,0x0f,0xf0,0x07,0x00,0x00,0xe0,0x07,0xe0,0x07,0x00,0x00,0xe0,0x03,0xc0,0x07,0x00,0x00,0xc0,0x03,0xc0,0x03,0x00,0x00,0x80,0x01,0x80,0x01,0x00,0x00,0x00,0xc0,0x07,0x00,0x00,0x00,0x00,0xf0,0x0f,0x00,0x00,0x00,0x00,0xf8,0x0f,0x00,0x00,0x00,0x00,0xf8,0x1f,0x00,0x00,0x00,0x00,0xf8,0x1f,0x00,0x00,0x00,0x00,0xf8,0x1f,0x00,0x00,0x00,0x00,0xf8,0x1f,0x00,0x00,0x00,0x00,0xf0,0x0f,0x00,0x00,0x00,0x00,0xf0,0x0f,0x00,0x00,0x00,0x00,0xc0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };

const unsigned char wifi_s[] U8X8_PROGMEM = { 0x00,0x00,0xe0,0x07,0x78,0x1e,0x0c,0x30,0x06,0x60,0xe3,0xc7,0x30,0x0c,0x08,0x10,0x8c,0x30,0xe0,0x03,0x20,0x04,0x20,0x04,0x80,0x01,0x80,0x01,0x80,0x01,0x00,0x00 };
	
/*---------------------------------------------------------------*/
/*函数名：int main()                                             */
/*功  能：主函数							                     */
/*		  1.初始化各功能模块  				     				 */
/*		  2.创建开始任务（在开始任务里创建所有其他任务）         */
/*		  3.开启任务调度				       			 		 */
/*参  数：无                          			   				 */
/*返回值：无                                       			     */
/*---------------------------------------------------------------*/
int main()
{
	/* 设置系统中断优先级分组4 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	/*初始化led灯*/												
	led_init();
	/*初始化delay*/
	delay_init(168);
	/*串口1初始化*/
	usartl_init(115200);
	/*串口2蓝牙模块*/
	bluetooth_usart2_init(9600);
	/*wifi初始化*/
	esp8266_init();
	/*超声波模块初始化*/
	hc_sr04_init();
	/*舵机初始化*/
	pwmsg_init(2000 - 1, 840 - 1);
	/*定时器3初始化*/
	TIM3_Int_Init((10000 / 100) - 1,8400 - 1);
	/*按键初始化*/
	key_init();
	/*矩阵按键初始化*/
	gpio4x4_init();
	/*初始化ADC3*/
	adc_init();
	/*启动ADC3的转换*/
	ADC_SoftwareStartConv(ADC3);
	/*烟雾报警器初始化*/
	mq_2_init();
	/*火焰传感器*/
	flame_transducer_init();
	/*启动ADC1的转换*/
	ADC_SoftwareStartConv(ADC1);
	/*启动ADC2的转换*/
	ADC_SoftwareStartConv(ADC2);
	/*初始化RTC*/
	My_RTC_Init();
	rtc_alarm_init();
	/*配置中断*/
	RTC_Set_WakeUp(RTC_WakeUpClock_CK_SPRE_16bits,0);
	/*RFID初始化*/
	MFRC522_Initializtion();
	/*MQTT连接初始化*/
	esp8266_mqtt_init();
	xTaskCreate((TaskFunction_t)software_control_task,
				(char *)"start_task",
				(uint16_t)SOFTWARE_CONTROL_STK_SIZE,
				(void *)NULL,
				(UBaseType_t)SOFTWARE_CONTROL_TASK_PRIO,
				(TaskHandle_t *)&SOFTWARE_CONTROL_Handler);
	vTaskStartScheduler(); //开启任务调度器
	while (1);
}


/*---------------------------------------------------------------*/
/*函数名：void software_control_task(void *pvParameters)         */
/*功  能：开始任务（配置）										 */
/*		  1.初始化OLED											 */
/*		  2.播放开机动画										 */
/*		  3.创建信号量，消息队列等任务通信方式   				 */
/*		  4.创建所有任务       			 						 */
/*		  5.删除本身       			 		    				 */
/*参  数：无                          			   				 */
/*返回值：无                                       			     */
/*---------------------------------------------------------------*/
void software_control_task(void *pvParameters)
{
	/*初始化OLED*/
	OLED_Init();	
	/*开机动画*/
	system_animation();
	/*初始化OLED*/
	IIC_U8G2_Init();
	/*初始化u8g2*/
	u8g2Init(&u8g2);
	/*加载进度条*/
	testDrawProcess(&u8g2);
	u8g2_ClearBuffer(&u8g2);  //清除缓冲区
	u8g2_DrawXBMP(&u8g2,40,10,48,48,wifi_m);
	u8g2_SendBuffer(&u8g2);	 //发送	
	vTaskDelay(1000); 
	/*进入临界区*/
	taskENTER_CRITICAL(); 
	/*创建信息队列*/
	/****************************************************************/
	KEY_ONE_Queue =  xQueueCreate(KEY_ONE_Length, sizeof(u32));
	KEY_NUM_Queue =  xQueueCreate(KEY_NUM_Length, sizeof(u8));
	USART1_Queue  =  xQueueCreate(QUEUE_USART_LEN,sizeof(u32));
	USART2_Queue  =  xQueueCreate(QUEUE_USART_LEN,sizeof(u32));
	USART3_Queue  =  xQueueCreate(QUEUE_USART_LEN,sizeof(u32));
	MQ_Queue	  =  xQueueCreate(KEY_NUM_Length, sizeof(u8));
	MQ1_Queue	  =  xQueueCreate(KEY_NUM_Length, sizeof(u8));
	/****************************************************************/
	/*创建二值信号量*/
	/****************************************************************/
	BinarySemaphore = xSemaphoreCreateBinary();
	/****************************************************************/
	printf("开始创建任务\r\n");
	int i = 0;
	while (task_su[i].pxTaskCode)
	{
		xTaskCreate(task_su[i].pxTaskCode,		/* 任务入口函数		*/
					task_su[i].pcName,			/* 任务名字 		*/
					task_su[i].usStackDepth,	/* 任务栈大小 		*/
					task_su[i].pvParameters,	/* 任务入口函数参数 */
					task_su[i].uxPriority,		/* 任务的优先级 	*/
					task_su[i].pxCreatedTask); 	/* 任务控制块指针	*/
		i++;
	}
	//初始化看门狗
	//iwdg_init();
	/*挂起任务*/
	/****************************************************************/
	vTaskSuspend(OLED_Handler);		   		//开始传感器数据显示
	vTaskSuspend(RTC_Handler);		   		//开始挂起时间显示
	vTaskSuspend(SG_HC_MONITOR_Handler);	//开始挂起超声波监测
	vTaskSuspend(DHT11_DATA_Handler);		//挂起传感器数据
	vTaskSuspend(KEY4x4_Handler);			//按键		
	vTaskSuspend(KEY4x4_PRO_Handler);		//按键	
	vTaskSuspend(LIGTH_DATA_Handler);		//按键
	vTaskSuspend(SUB_PUB_Handler);		//按键		
	/****************************************************************/		
	printf("创建任务完成\r\n");	
	vTaskDelete(SOFTWARE_CONTROL_Handler); 	//开始任务删除
	printf("开始任务删除\r\n");	
	taskEXIT_CRITICAL();
}

/*---------------------------------------------------------------*/
/*函数名：void key_switch_task(void *pvParameters)             	 */
/*功  能：多功能按键							 	 	    	 */
/*		  1.单击    				   					 		 */
/*		  2.双击 				   								 */
/*		  3.长按				   						 		 */
/*		  3.组合键	key1+key2			   						 */
/*参  数：无                          			   				 */
/*返回值：无                                       			     */
/*其  他：按键1	key_val=1 	单击  2:长按   3:组合键  4:双击	 	 */
/*	 	  按键2	key_val=5 	单击  6:长按   7:组合键  8:双击	 	 */
/*	 	  按键3	key_val=9 	单击  10:长按  11:组合键  12:双击	 */
/*	      按键4 key_val=13 	单击  14:长按  15:组合键  16:双击	 */
/*	      key_val=17 key1+key2									 */
/*---------------------------------------------------------------*/
//多功能按键有点问题
void key_switch_task(void* pvParameters)
{
	unsigned int key_val,fa;
	u8g2_SetFontMode(&u8g2, 1);
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
	while(1)
	{
		xQueueReceive(MQ1_Queue, &fa, 500);
		xQueueReceive(KEY_ONE_Queue, &key_val, portMAX_DELAY);
		if(s==0)
		{
			vTaskSuspend(SCREENSAVER_Handler);
			s++;
			key_val=0;
			vTaskResume(RTC_Handler);
		}
		if (0 != key_val) 
		{
			if(eTaskGetState(RTC_Handler)!=eSuspended)
			{
				vTaskSuspend(RTC_Handler);
			}
			if(eTaskGetState(DHT11_DATA_Handler)!=eSuspended)
			{
				vTaskSuspend(DHT11_DATA_Handler);
			}
			if(eTaskGetState(SG_HC_MONITOR_Handler)!=eSuspended)
			{
				vTaskSuspend(SG_HC_MONITOR_Handler);
			}
			if(eTaskGetState(KEY4x4_Handler)!=eSuspended)
			{
				vTaskSuspend(KEY4x4_Handler);
			}
			if(eTaskGetState(KEY4x4_PRO_Handler)!=eSuspended)
			{
				vTaskSuspend(KEY4x4_PRO_Handler);
			}
			switch(key_val)
			{
				case KEY_NONE:
					printf("键值无效\r\n");
				break;
				case KEY1_SHORT:
					printf("这是key1单击\r\n");
					func_index = table[func_index].prev;    //向上翻
				break;
				case KEY2_SHORT:
					printf("这是key2单击\r\n");
					func_index = table[func_index].next;    //向下翻
				break;
				case KEY3_SHORT:
					printf("这是key3单击\r\n");
					func_index = table[func_index].enter;    //确认
				break;
				case KEY4_SHORT:
					func_index=table[func_index].back;
					printf("这是key4单击\r\n");
				break;
				case KEY1_2_SHORT:
					printf("这是组合键\r\n") ;
					func_index=11;
				break;
			}
			u8g2_SetFontMode(&u8g2, 1);
			u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
			if (func_index != last_index)
			{
				current_operation_index = table[func_index].current_operation;
				(*current_operation_index)();//执行当前操作函数
				last_index = func_index;
			}
			
		}
		vTaskDelay(1000); 
	}
	//vTaskDelete(xTaskGetCurrentTaskHandle());
}

/*---------------------------------------------------------------*/
/*函数名：void key_pro_task(void *pvParameters)                  */
/*功  能：矩阵按键							 	 	         	 */
/*		  1.获取密码    				   						 */
/*参  数：无                          			   				 */
/*返回值：无                                       			     */
/*其  他：  												     */
/*---------------------------------------------------------------*/
static void key_one_task(void *pvParameters)
{
	unsigned int key_val;
	BaseType_t err;
	while (1)
	{
		key_val = key_scan(1);
		if (KEY_NONE != key_val) 
		{
			err = xQueueOverwrite(KEY_ONE_Queue, &key_val); //覆写发送
			printf("key_val:%d\r\n",key_val);
			if (err != pdTRUE)
			{
				printf("发送失败\r\n");
			}
		}
		
		vTaskDelay(100);
	}
}

static void key_pro_task(void *pvParameters)
{
	unsigned int key_val;
	BaseType_t err;
	while (1)
	{
		key_val = key4x4_scanf();
		printf("KEY VALUE = %d.\r\n", key_val);
		err = xQueueOverwrite(KEY_NUM_Queue, &key_val); //覆写发送
		if (err != pdTRUE)
		{
			printf("发送失败\r\n");
		}
		BEEP(1);
		vTaskDelay(100);
		BEEP(0);
		vTaskDelay(100);
	}
}
/*---------------------------------------------------------------*/
/*函数名：void bluetooth_data_task(void *pvParameters)           */
/*功  能：蓝牙串接收处理							 	 	     */
/*		  1.    				   							 	 */
/*参  数：无                          			   				 */
/*返回值：无                                       			     */
/*其  他：  												     */
/*---------------------------------------------------------------*/
void bluetooth_data_task(void *pvParameters)
{
	u32 CommandValue=COMMANDERR;
	u32 flag_usart=0;
	BaseType_t xReturn=pdFALSE;	
	uint32_t i=0;
	char *p = NULL;
	RTC_TimeTypeDef  	RTC_TimeStructure;
	RTC_DateTypeDef  	RTC_DateStructure;	
	RTC_AlarmTypeDef 	RTC_AlarmStructure;	
	while(1)
	{
		if(USART2_Queue!=NULL)
        {
			xReturn = xQueueReceive( USART2_Queue,/* 消息队列的句柄 */
									&flag_usart, /* 得到的消息内容 */
									portMAX_DELAY);/* 等待时间一直等 */
			if(xReturn != pdPASS)
			{
				continue;
			}
			bluetooth_flag=1;			
			if(flag_usart==1)
			{
				printf("USART2收到数据:%s\r\n",USART2_RX_BUF);
				USART2_RX_BUF[USART2_LEN-1]='\0';
				printf("处理过的数据:%s\r\n",USART2_RX_BUF);
				if(strstr((char *)USART2_RX_BUF,"TIME SET"))
				{
					strtok((char *)USART2_RX_BUF,"-");
				
					p=strtok(NULL,"-");
					i = atoi(p);
					
					if(i<12)
						RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
					else
						RTC_TimeStructure.RTC_H12     = RTC_H12_PM;
						
					i= (i/10)*16+i%10;
					RTC_TimeStructure.RTC_Hours   = i;
					
					p=strtok(NULL,"-");
					i = atoi(p);	
					
					i= (i/10)*16+i%10;	
					RTC_TimeStructure.RTC_Minutes = i;
					
					p=strtok(NULL,"-");
					i = atoi(p);	
					
					i= (i/10)*16+i%10;					
					RTC_TimeStructure.RTC_Seconds = i; 					
					
					RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);
					BEEP(1);
					vTaskDelay(500);
					BEEP(0);
					printf("时间修改完成\r\n");				
				}
				else if(strstr((char *)USART2_RX_BUF,"DATA SET"))
				{	
					p = strtok((char*)USART2_RX_BUF,"-");
					p = strtok(NULL,"-");
					i = atoi(p);
					i =(i/10)*16+i%10;
					RTC_DateStructure.RTC_Year 	= i;
					
					p = strtok(NULL,"-");
					i = atoi(p);
					i= (i/10)*16+i%10;
					RTC_DateStructure.RTC_Month = i;
					
					p = strtok(NULL,"-");
					i = atoi(p);
					i= (i/10)*16+i%10;
					RTC_DateStructure.RTC_Date	= i;
					
					p = strtok(NULL,"-");
					i = atoi(p);
					i= (i/10)*16+i%10;
					RTC_DateStructure.RTC_WeekDay = i;
					RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
					BEEP(1);
					vTaskDelay(500);
					BEEP(0);
					printf("日期完成\r\n");	
				}else if (strstr((char *)USART2_RX_BUF,"ALARM SET"))
				{
					RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
					vTaskDelay(10);
					strtok((char *)USART2_RX_BUF,"-");
					p=strtok(NULL,"-");
					i = atoi(p);	
					if(i<12)
						RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;
					else
						RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_PM;
					i= (i/10)*16+i%10;
					RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = i;
					p=strtok(NULL,"-");
					i = atoi(p);						
					i= (i/10)*16+i%10;	
					RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = i;
					p=strtok(NULL,"-");
					i = atoi(p);						
					i= (i/10)*16+i%10;					
					RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = i; 	
					vTaskDelay(10);
					alarm_flag=1;
					RTC_AlarmStructure.RTC_AlarmDateWeekDay=0X13;
					RTC_AlarmStructure.RTC_AlarmDateWeekDaySel=RTC_AlarmDateWeekDaySel_Date;
					RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;  
					RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);
					RTC_ITConfig(RTC_IT_ALRA, ENABLE);
					RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
					BEEP(1);
					vTaskDelay(500);
					BEEP(0);
				}
				else
				{
					LowerToCap(USART2_RX_BUF,USART2_LEN-1);						//将字符串转换为大写		
					printf("CommandStr:%s\r\n",USART2_RX_BUF);
					CommandValue=CommandProcess(USART2_RX_BUF);		//命令解析
					if(CommandValue!=COMMANDERR)
					{
						printf("命令为:%s\r\n",USART2_RX_BUF);
						printf("CommandValue:%d\r\n",CommandValue);
						switch(CommandValue)						//处理命令
						{
							case LAMP1ON: 
								GPIO_WriteBit(GPIOF,GPIO_Pin_9,Bit_RESET);
								break;
							case LAMP1OFF:
								GPIO_WriteBit(GPIOF,GPIO_Pin_9,Bit_SET);
								break;
							case LAMP2ON:
								GPIO_WriteBit(GPIOE,GPIO_Pin_13,Bit_RESET);
								break;
							case LAMP2OFF:
								GPIO_WriteBit(GPIOE,GPIO_Pin_13,Bit_SET);
								break;
							case BEEPON:
								BEEP(1);
								break;
							case BEEPOFF:
								BEEP(0);
								break;
							case DOOROPEN:
								BEEP(1);
								vTaskDelay(1000);
								BEEP(0);
								TIM_SetCompare1(TIM9, 100);
								break;
							case MONTIORON:
								if(eTaskGetState(RTC_Handler)!=eSuspended)
								{
									vTaskSuspend(RTC_Handler);
								}
								if(eTaskGetState(SCREENSAVER_Handler)!=eSuspended)
								{
									vTaskSuspend(SCREENSAVER_Handler);
								}
								if(eTaskGetState(SG_HC_MONITOR_Handler)==eSuspended)
								{
									vTaskResume(SG_HC_MONITOR_Handler);
								}
								func_index=11;
								break;
							case MONTIOROFF:
								if(eTaskGetState(SG_HC_MONITOR_Handler)!=eSuspended)
								{
									vTaskSuspend(SG_HC_MONITOR_Handler);
								}
								func_index=0;
								break;
							case SCREENSAVER:
								if(eTaskGetState(SCREENSAVER_Handler)==eSuspended)
								{
									vTaskResume(SCREENSAVER_Handler);
								}							
								s=0;
								func_index = 0;
								break;
						}
					}
					else
					{	
						printf("无效的命令，请重新输入!!\r\n");
					}
				}
				memset(USART2_RX_BUF,0,USART_REC_LEN);
				USART2_RX_STA=0;
				USART2_LEN=0;
				flag_usart=0;
			}
			vTaskDelay(2000);  
			printf("现在运行bluetooth_data_task\r\n");
		}
	}
}

/*---------------------------------------------------------------*/
/*函数名：void read_data_task(void *pvParameters)           	 */
/*功  能：读取传感器温湿度数据,并显示				 	 	     */
/*		  1. 温湿度数据   				   						 */
/*		  1. 光照强度   				   						 */
/*		  1. 烟雾报警器数据   				   					 */
/*参  数：无                          			   				 */
/*返回值：无                                       			     */
/*其  他：两秒读取一次 		    							     */
/*---------------------------------------------------------------*/
void dht11_data_task(void* pvParameters)
{
	uint8_t buf1[5]; //存放温湿度数据
	char strTemp[32]="";
	char strHumi[32]="";	
	while(1)
	{
		/*读取温湿度数据,并存入结构体*/
		dht11_read(buf1);
		sprintf(strTemp, "Temp:%d.%d C",buf1[2],buf1[3]);
		sprintf(strHumi, "Humi:%d.%d %%",buf1[0],buf1[1]);
		u8g2_ClearBuffer(&u8g2);  //清除缓冲区
		u8g2_DrawStr(&u8g2, 10, 30, strTemp);
		u8g2_DrawStr(&u8g2, 10, 45, strHumi);
		u8g2_SendBuffer(&u8g2);	 //发送	
		vTaskDelay(1000); 			
	}
}

void adc_data_task(void* pvParameters)
{
	uint16_t mq2_val;
	uint16_t adc_fire_val;
	char strMq_2_val[16]="";
	char strFT_val[16]="";
	int mq_falg=0;
	int ft_flag=0;
	while(1)
	{
		/*读取烟雾报警器数据*/
		while(ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC)==RESET);//等待ADC3转换完成
		ADC_ClearFlag(ADC2,ADC_FLAG_EOC);
		mq2_val=ADC_GetConversionValue(ADC2);
		mq2_vol=3300*mq2_val/4095;
		/*火焰报警器*/
		while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);//等待ADC3转换完成
		adc_fire_val= ADC_GetConversionValue(ADC1);
		adc_fire_vol = 3300*adc_fire_val/4095;
		sprintf(strMq_2_val, "MQ_2: %d mv", mq2_vol);
		sprintf(strFT_val, 	 "FT: %d",adc_fire_vol);
		if(mq2_val>800 || mq2_vol> 800)
		{	
			BEEP(1);
			vTaskDelay(500);
			mq_falg=1;
			BEEP(0);
			LED(14);
		}
		if(adc_fire_val<550 || adc_fire_vol< 550)
		{
			BEEP(1);
			vTaskDelay(500);
			ft_flag=1;
			BEEP(0);
			LED(14);
		}
		if(func_index==9)
		{
			u8g2_ClearBuffer(&u8g2);  //清除缓冲区
			u8g2_DrawStr(&u8g2, 10,15, strFT_val);
			u8g2_DrawStr(&u8g2, 10,30, strMq_2_val);
			if(mq_falg==0)
			{
				u8g2_DrawStr(&u8g2, 10,45, "MQ:Normal");
			}else if(mq_falg==1)
			{
				u8g2_DrawStr(&u8g2, 10,45, "MQ:Abnormal");
			}
			if(ft_flag==0)
			{
				u8g2_DrawStr(&u8g2, 10,60, "FT:Normal");
			}else if(ft_flag==1)
			{
				u8g2_DrawStr(&u8g2, 10,60, "FT:Abnormal");
			}
			u8g2_SendBuffer(&u8g2);	 //发送	
		}
		vTaskDelay(1000); 	
	}
}

void ligth_data_task(void* pvParameters)
{
	u32 ligth_data;
	u32 ligth_val;
	char strLigth_data[32]="";
	while(1)
	{
		/*读取光照强度*/
		ligth_val = ADC_VAL * 3300 / 4095;
		if(ligth_val==0)
			ligth_val=680;
		ligth_data=ligth_val-600;
		if(ligth_data>=600)
		{
			GPIO_WriteBit(GPIOE,GPIO_Pin_13,Bit_RESET);
		}else
		{
			GPIO_WriteBit(GPIOE,GPIO_Pin_13,Bit_SET);
		}
		printf("光照强度:%d\r\n", ligth_data);
		if(func_index==10)
		{
			sprintf(strLigth_data, "Ligth: %d Lx", ligth_data);
			u8g2_ClearBuffer(&u8g2);  //清除缓冲区
			u8g2_DrawStr(&u8g2, 10, 35, strLigth_data);
			u8g2_SendBuffer(&u8g2);	 //发送	
		}
		vTaskDelay(1000); 	
	}
}

/*---------------------------------------------------------------*/
/*函数名：void sg_hc_monitor_task(void *pvParameters)            */
/*功  能：超声波监测							 	 	         */
/*参  数：无                          			   				 */
/*返回值：无                                       			     */
/*其  他：配合舵机转动 目标距离有目标则会锁定目标 并亮灯提示 	 */
/*		  如果长时间停留则会警报 	 							 */
/*---------------------------------------------------------------*/
static void sg_hc_monitor_task(void *pvParameters)
{
	float hc_length;
	int dir = 0, pwm_conu = 50;
	char  g_mqtt[64];
	char  g_mqtt1[64]={"alert Discovery target"};
	char strlength[32]="";
	int length_flag=0;
	u32 num=0;
	while (1)
	{
		taskENTER_CRITICAL(); 
		hc_length = hc_src_tim_get_data();
		printf("当前距离:%.2f\r\n", hc_length);
		if (hc_length < 10)
		{
			LED(14);
			length_flag=1;
			num++;
			if(num>=10)
			{
				BEEP(1);
				vTaskDelay(1000); 		
				BEEP(0);
				sprintf(g_mqtt,"{\"getmessage\": \"%s\"}",g_mqtt1);

				mqtt_publish_data(MQTT_PUBLISH_TOPIC,g_mqtt,0);
			}
		}
		else
		{
			if (dir)
				pwm_conu += 20;
			else
				pwm_conu -= 20;
			if (pwm_conu >= 499)
				dir = 0;
			if (pwm_conu <= 70)
				dir = 1;
			GPIO_WriteBit(GPIOE,GPIO_Pin_14,Bit_SET);
			length_flag=0;
			num=0;
			TIM_SetCompare1(TIM9, pwm_conu);
			vTaskDelay(50);
		}
		if(func_index==11)
		{
			sprintf(strlength, "%.2f m",hc_length);
			u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
			u8g2_ClearBuffer(&u8g2);  //清除缓冲区
			u8g2_DrawStr(&u8g2, 32, 25, "Distance");
			u8g2_DrawStr(&u8g2, 35, 40, strlength);
			if(length_flag==1)
			{
				u8g2_DrawStr(&u8g2, 30, 50, "Targeted");
			}
			u8g2_SendBuffer(&u8g2);	 //发送	
		}
		printf("现在运行sg_hc_monitor_task\r\n");
		vTaskDelay(500);
	}
}

/*---------------------------------------------------------------*/
/*函数名：void get_time_task(void *pvParameters)             	 */
/*功  能：RTC时钟									 	 	     */
/*		  1.计时				   							 	 */
/*参  数：无                          			   				 */
/*返回值：无                                       			     */
/*其  他：  												     */
/*---------------------------------------------------------------*/
const unsigned char alarm_s[] U8X8_PROGMEM = { 0x00,0x00,0x9c,0x39,0xe6,0x6f,0x1a,0x58,0x0c,0x30,0x84,0x21,0x84,0x21,0x86,0x61,0x06,0x63,0x04,0x26,0x04,0x20,0x0c,0x30,0x18,0x1c,0xf8,0x1f,0x8c,0x11,0x00,0x00 };
	
const unsigned char lanya[] U8X8_PROGMEM = { 0x00,0x00,0x80,0x03,0x80,0x07,0x80,0x1d,0x8c,0x39,0xbc,0x3d,0xf0,0x0f,0xc0,0x07,0xe0,0x07,0xf0,0x0f,0xbc,0x3d,0x8c,0x39,0x80,0x1d,0x80,0x07,0x80,0x03,0x80,0x00 };

static void get_time_task(void *pvParameters)
{
	int delay_1ms_cnt=0;
	uint8_t buf[5];
	char str_rtc_date[32]={0},str_time[32]={0};
	RTC_DateTypeDef RTC_DateStruct;
	RTC_TimeTypeDef RTC_TimeStruct;
	while (1)
	{
		RTC_GetDate(RTC_Format_BCD,&RTC_DateStruct);;
		RTC_GetTime(RTC_Format_BCD,&RTC_TimeStruct);
		sprintf(str_rtc_date,"20%02x-%02x-%02x",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date);
		sprintf(str_time,"%02x-%02x-%02x",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds);
		u8g2_ClearBuffer(&u8g2);  //清除缓冲区
		if(bluetooth_flag==1)
		{
			u8g2_DrawXBMP(&u8g2,0,0,16,16,lanya);
		}
		if(wifi_flag==1)
		{
			u8g2_DrawXBMP(&u8g2,50,0,16,16,wifi_s);
		}
		if(alarm_flag==1)
		{
			u8g2_DrawXBMP(&u8g2,25,0,16,16,alarm_s);
		}
		else if(alarm_flag==2)
		{
			BEEP(1);
			vTaskDelay(1000); 		
			BEEP(0);
			alarm_flag=0;
		}
		u8g2_SetFont(&u8g2,u8g2_font_t0_22b_tr);
		u8g2_DrawStr(&u8g2, 10, 30, str_rtc_date);
		u8g2_SetFont(&u8g2,u8g2_font_profont29_tf);
		u8g2_DrawStr(&u8g2, 0, 60, str_time);
		u8g2_SendBuffer(&u8g2);	 //发送		
		vTaskDelay(500);
		delay_1ms_cnt++;
		if((delay_1ms_cnt % 4) ==0)
		{	
			if(0 == dht11_read(buf))
			{
				g_temp = (float)buf[2]+(float)buf[3]/10;
				g_humi = (float)buf[0]+(float)buf[1]/10;
				g_length = hc_src_tim_get_data();
				g_ligth=ADC_VAL * 3300 / 4095;
				g_mq=(int)mq2_vol;
				g_tf=(int)adc_fire_vol;
			}
			printf("上报设备状态\r\n");
			//上报设备状态
			mqtt_report_devices_status();	
		}
	}
	// vTaskDelete(xTaskGetCurrentTaskHandle());
}

/*---------------------------------------------------------------*/
/*函数名：void iwdg_task(void *pvParameters)             		 */
/*功  能：独立看门狗喂狗							 	 	     */
/*		  1.喂狗				   							 	 */
/*参  数：无                          			   				 */
/*返回值：无                                       			     */
/*其  他：  												     */
/*---------------------------------------------------------------*/
static void iwdg_task(void *pvParameters)
{
	while (1)
	{
		IWDG_ReloadCounter(); //喂狗
		vTaskDelay(50);
		printf("现在运行iwdg_task\r\n");
	}
}

/*---------------------------------------------------------------*/
/*函数名：void screensaver_task(void *pvParameters)              */
/*功  能：屏保								 	 	         	 */
/*		  1.屏保太空人动画    			 						 */
/*参  数：无                          			   				 */
/*返回值：无                                       			     */
/*其  他： 无操作一段时间播放屏保 							     */
/*---------------------------------------------------------------*/
static void screensaver_task(void *pvParameters)
{
	while (1)
	{
		oled_ui();
		vTaskDelay(100);
	}
}

/*---------------------------------------------------------------*/
/*函数名：void usart1_task(void *pvParameters)              	 */
/*功  能：USART1接收数据					 	 	         	 */
/*		  1.修改日期时间			    			 			 */
/*参  数：无                          			   				 */
/*返回值：无                                       			     */
/*其  他：DATA SET-22-11-9	修改日期指令					     */
/*		  TIME SET-20-4-0	修改时间指令					     */
/*---------------------------------------------------------------*/
static void usart1_task(void* pvParameters)
{
	uint32_t i=0;
	char *p = NULL;
	u32 flag_usart=0;
	RTC_TimeTypeDef  	RTC_TimeStructure;
	RTC_DateTypeDef  	RTC_DateStructure;	
	BaseType_t xReturn=pdFALSE;	
	vTaskDelay(50);
	while(1)
	{	
		/*接收到串口中断内容*/
		xReturn = xQueueReceive( USART1_Queue,/* 消息队列的句柄 */
								&flag_usart, /* 得到的消息内容 */
								portMAX_DELAY);/* 等待时间一直等 */
		printf("USART1收到数据:%s\r\n",USART1_RX_BUF);
		if(xReturn != pdPASS)
		{
			continue;
		}			
		if(flag_usart==1)
		{			
			/* 设置时间 */
			if(strstr((char *)USART1_RX_BUF,"TIME SET"))
			{
				/* 以等号分割字符串 */
				strtok((char *)USART1_RX_BUF,"-");
				
				/* 获取时 */
				p=strtok(NULL,"-");
				i = atoi(p);
				
				
				/* 通过时，判断是AM还是PM */
				if(i<12)
					RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
				else
					RTC_TimeStructure.RTC_H12     = RTC_H12_PM;
					
				/* 转换为BCD编码 */
				i= (i/10)*16+i%10;
				RTC_TimeStructure.RTC_Hours   = i;
				
				/* 获取分 */
				p=strtok(NULL,"-");
				i = atoi(p);	
				
				/* 转换为BCD编码 */
				i= (i/10)*16+i%10;	
				RTC_TimeStructure.RTC_Minutes = i;
				
				/* 获取秒 */
				p=strtok(NULL,"-");
				i = atoi(p);	
				
				/* 转换为BCD编码 */
				i= (i/10)*16+i%10;					
				RTC_TimeStructure.RTC_Seconds = i; 					
				
				/* 设置RTC时间 */
				RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);
				printf("时间修改完成\r\n");				
			}
			/* 设置日期 */
			else if(strstr((char *)USART1_RX_BUF,"DATA SET"))
			{	
				//设置年
				p = strtok((char*)USART1_RX_BUF,"-");
				p = strtok(NULL,"-");
				i = atoi(p);
				i =(i/10)*16+i%10;
				RTC_DateStructure.RTC_Year 	= i;
				
				//设置月
				p = strtok(NULL,"-");
				i = atoi(p);
				i= (i/10)*16+i%10;
				RTC_DateStructure.RTC_Month = i;
				
				//设置日
				p = strtok(NULL,"-");
				i = atoi(p);
				i= (i/10)*16+i%10;
				RTC_DateStructure.RTC_Date	= i;
				
				//设置星期几
				p = strtok(NULL,"-");
				i = atoi(p);
				i= (i/10)*16+i%10;
				RTC_DateStructure.RTC_WeekDay = i;
				
				RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
				printf("日期完成\r\n");	
			}
			memset(USART1_RX_BUF,0,USART_REC_LEN);
			USART1_RX_STA=0;
			flag_usart=0;
		}

	}
}

/*---------------------------------------------------------------*/
/*函数名：void rfid_task(void *pvParameters)             		 */
/*功  能：RFID解锁						 	     				 */
/*		  1.获取密码    				   						 */
/*参  数：无                          			   				 */
/*返回值：无                                       			     */
/*其  他：密码八位数,三次输入错误会警报，短时间不能输		     */
/*		  自动解除挂起	     									 */
/* 		  4*4矩阵键盘 获取密码 									 */
/*		 'C'进入RFID读卡			 						   	 */
/*		 '#'清除所有输入密码									 */
/*		 '*'清除一位输入密码									 */
/*---------------------------------------------------------------*/
const unsigned char co[] U8X8_PROGMEM = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0x3f,0x00,0x00,0x00,0x00,0xf0,0xff,0x00,0x00,0x00,0x00,0xf8,0xff,0x01,0x00,0x00,0x00,0xfc,0xff,0x03,0x00,0x00,0x00,0x7e,0xf0,0x07,0x00,0x00,0x00,0x1f,0xc0,0x07,0x00,0x00,0x00,0x1f,0x80,0x0f,0x00,0x00,0x00,0x0f,0x80,0x0f,0x00,0x00,0x80,0x0f,0x00,0x0f,0x00,0x00,0x80,0x0f,0x00,0x1f,0x00,0x00,0x80,0x0f,0x00,0x0f,0x00,0x00,0x80,0x0f,0x00,0x0f,0x00,0x00,0x80,0x0f,0x00,0x00,0x00,0x00,0x80,0x0f,0x00,0x00,0x00,0x8c,0x80,0x0f,0x00,0x00,0x80,0xff,0xff,0xff,0x03,0x00,0xc0,0xff,0xff,0xff,0x07,0x00,0xe0,0xff,0xff,0xff,0x0f,0x00,0xe0,0xff,0xff,0xff,0x0f,0x00,0xf0,0xff,0xff,0xff,0x0f,0x00,0xf0,0xff,0xc3,0xff,0x0f,0x00,0xf0,0xff,0x83,0xff,0x0f,0x00,0xf0,0xff,0x01,0xff,0x0f,0x00,0xf0,0xff,0x01,0xff,0x0f,0x00,0xf0,0xff,0x83,0xff,0x0f,0x00,0xf0,0xff,0xc7,0xff,0x0f,0x00,0xf0,0xff,0xc7,0xff,0x0f,0x00,0xf0,0xff,0xc7,0xff,0x0f,0x00,0xf0,0xff,0xc7,0xff,0x0f,0x00,0xf0,0xff,0xc7,0xff,0x0f,0x00,0xf0,0xff,0xc7,0xff,0x0f,0x00,0xf0,0xff,0xcf,0xff,0x0f,0x00,0xf0,0xff,0xff,0xff,0x0f,0x00,0xe0,0xff,0xff,0xff,0x0f,0x00,0xe0,0xff,0xff,0xff,0x0f,0x00,0xc0,0xff,0xff,0xff,0x07,0x00,0x80,0xff,0xff,0xff,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
//RFID固定卡号判断	
static void rfid_task(void* parameter)
{
	int rfid_flag=0;
	u8 status,card_size;
	char  g_mqtt[64];
	char  g_mqtt1[64]={"Opening the door"};
	xQueueReceive(MQ_Queue, &rfid_flag, portMAX_DELAY);
	if(rfid_flag==1)
	{
	while(1)
	{
			MFRC522_Initializtion();												//有BUG每次使用时都要初始化一次
			
			status=MFRC522_Request(0x52, card_pydebuf);			//寻卡						//寻卡
			
			if(status==0)		//如果读到卡													//如果读到卡
			{
				status=MFRC522_Anticoll(card_numberbuf);			//防撞处理			
				card_size=MFRC522_SelectTag(card_numberbuf);	//选卡
				status=MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);	//验卡
				status=MFRC522_Write(4, card_writebuf);				//写卡（写卡要小心，特别是各区的块3）
				status=MFRC522_Read(4, card_readbuf);					//读卡					//选卡
				
				printf("card_size %d Kbits\r\n",card_size);
				
				PFout(8)=1;
				delay_ms(200);
				PFout(8)=0;
				TIM_SetCompare1(TIM9, 100);
				printf("解锁成功\r\n");
				sprintf(g_mqtt,"{\"getmessage\": %s}",g_mqtt1);
				u8g2_ClearBuffer(&u8g2);  //清除缓冲区
				u8g2_DrawXBMP(&u8g2,40,10,48,48,co);
				u8g2_SendBuffer(&u8g2);	 //发送	
				xQueueOverwrite(MQ1_Queue, &rfid_flag); //覆写发送
			}	
			vTaskDelay(1000);
		}
	}
}

/*---------------------------------------------------------------*/
/*函数名：void receive_data_task(void *pvParameters)             */
/*功  能：获取矩阵按键的数值						 	     	 */
/*		  1.获取密码    				   						 */
/*参  数：无                          			   				 */
/*返回值：无                                       			     */
/*其  他：密码八位数,三次输入错误会警报，短时间不能输		     */
/*		  冻结模式,将receive_data_task任务定时挂起,时间到		 */
/*		  自动解除挂起	     									 */
/* 		  4*4矩阵键盘 获取密码 									 */
/*		 'C'进入RFID读卡			 						   	 */
/*		 '#'清除所有输入密码									 */
/*		 '*'清除一位输入密码									 */
/*---------------------------------------------------------------*/
static void key4x4_task(void* pvParameters)
{
	u8 	i=0;
	char password[9]={0};
	char key_val=0;
	char str='*';
	char strdispaly[9]=" "; //回显-
	int pass_num=0;
	int rfid_flag=1;
	char  g_mqtt[64];
	char  g_mqtt1[64]={"Opening the door"};
	while(1)
	{
		u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
		u8g2_ClearBuffer(&u8g2);  //清除缓冲区
		u8g2_DrawStr(&u8g2, 20, 20,"password");
		u8g2_DrawStr(&u8g2, 30, 30, strdispaly);
		u8g2_SendBuffer(&u8g2);	 //发送	
		xQueueReceive(KEY_NUM_Queue,&key_val,portMAX_DELAY);
		if(key_val!='*')
		{
			printf("key_num:%d\r\n",key_val);
			if(key_val=='B') //退格
			{
				i--;
				password[i]='\0';
				strdispaly[i]=' ';
			
			}if(key_val=='A') //清除
			{
				i=0;
				memset(password,0,9);
				memset(strdispaly,0,9);
			}
			else
			{
				password[i]=key_val;			
				strdispaly[i]=str;
				i++;
			}		
			printf("password:%s\r\n",password);
			printf("key_val:%c\r\n",key_val);
			vTaskDelay(10);
			if(i >= 8)
			{			
				if(strcmp(password,"12345678")==0)
				{
					printf("密码输入正确\r\n");
					sprintf(g_mqtt,"{\"getmessage\": %s}",g_mqtt1);
					u8g2_ClearBuffer(&u8g2);  //清除缓冲区
					u8g2_DrawStr(&u8g2, 10, 30, "Password sure");
					u8g2_SendBuffer(&u8g2);	 //发送	
					vTaskDelay(2000);  
					u8g2_ClearBuffer(&u8g2);  //清除缓冲区
					u8g2_DrawStr(&u8g2, 5, 30, "Opening the door");
					u8g2_SendBuffer(&u8g2);	 //发送	
					vTaskDelay(2000);
					u8g2_ClearBuffer(&u8g2);  //清除缓冲区
					u8g2_DrawStr(&u8g2, 10, 30, "Welcome home");
					u8g2_SendBuffer(&u8g2);	 //发送	
					vTaskDelay(2000);
					TIM_SetCompare1(TIM9, 100);
					u8g2_ClearBuffer(&u8g2);  //清除缓冲区
					u8g2_DrawXBMP(&u8g2,40,10,48,48,co);
					u8g2_SendBuffer(&u8g2);	 //发送	
					vTaskDelay(2000);
					memset(password,0,9);
					memset(strdispaly,0,9);					
					s=0;
					func_index = 0;
				}else
				{
					pass_num++;
					printf("密码输入错误\r\n");
					u8g2_ClearBuffer(&u8g2);  //清除缓冲区
					u8g2_DrawStr(&u8g2, 5, 30, "Password error");
					u8g2_SendBuffer(&u8g2);	 //发送	
					vTaskDelay(2000);
					if(pass_num>=3)
					{
						BEEP(1);
						vTaskDelay(1000);
						BEEP(0);
						printf("密码错误三次\r\n");
						printf("任务已挂起\r\n");
						vTaskSuspend(xTaskGetCurrentTaskHandle());
						pass_num=0;
					}
					i=0;
					memset(password,0,9);
					memset(strdispaly,0,9);
				}
				i=0;
			}
		}else
	  	{
			xQueueOverwrite(MQ_Queue, &rfid_flag); //覆写发送
		}
	}
}

static void esp8266_mqtt_task(void* pvParameters)
{
	uint32_t 	i=0;
	while(1)
	{
		if(g_esp8266_rx_end && g_esp8266_transparent_transmission_sta)
		{		
			for(i=0;i<g_esp8266_rx_cnt;i++)
			{	
				if(g_esp8266_rx_buf[i] == '{')
				{
					mqtt_cjson_parse((char *)&g_esp8266_rx_buf[i]);
					break;
				}
			}
			//清空接收缓冲区、接收计数值、接收结束标志位
			memset((void *)g_esp8266_rx_buf,0,sizeof g_esp8266_rx_buf);
			g_esp8266_rx_cnt=0;
			g_esp8266_rx_end=0;
		}
		vTaskDelay(1000);
	}
}
