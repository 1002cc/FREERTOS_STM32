#include "rtc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

static NVIC_InitTypeDef  NVIC_InitStructure;
static EXTI_InitTypeDef  EXTI_InitStructure;
static RTC_TimeTypeDef  RTC_TimeStructure;
static RTC_InitTypeDef  RTC_InitStructure;
static RTC_DateTypeDef RTC_DateStructure;

ErrorStatus Rtc_Set_Data(u8 year,u8 month,u8 date,u8 weekday)
{
	RTC_DateStructure.RTC_Year = year;						//年
	RTC_DateStructure.RTC_Month = month;					//月
	RTC_DateStructure.RTC_Date = date;						//日
	RTC_DateStructure.RTC_WeekDay = weekday;				//星期
	return RTC_SetDate(RTC_Format_BCD,&RTC_DateStructure);	//BCD格式，日期初始化
}

ErrorStatus Rtc_Set_Time(u8 h12,u8 hour,u8 min,u8 sec)
{
	RTC_TimeStructure.RTC_H12 = h12;						//AM 或 24 小时制 
	RTC_TimeStructure.RTC_Hours = hour;						//小时
	RTC_TimeStructure.RTC_Minutes = min;					//分钟
	RTC_TimeStructure.RTC_Seconds = sec;					//秒
	return RTC_SetTime(RTC_Format_BCD,&RTC_TimeStructure);	//BCD格式，日期初始化 
}


u8 My_RTC_Init(void)
{
	u16 retry=0X1FFF; 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//使能PWR时钟
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问 
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x5050)		//是否第一次配置?
	{
		RCC_LSEConfig(RCC_LSE_ON);//LSE 开启    
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//检查指定的RCC标志位设置与否,等待低速晶振就绪
		{
			retry++;
			vTaskDelay(10);
		}
		if(retry==0)return 1;		//LSE 开启失败. 

		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
		RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟 

		RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//RTC异步分频系数(1~0X7F)
		RTC_InitStructure.RTC_SynchPrediv  = 0xFF;//RTC同步分频系数(0~7FFF)
		RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;//RTC设置为,24小时格式
		RTC_Init(&RTC_InitStructure);

		Rtc_Set_Data(23,59,56,RTC_H12_AM);	//设置时间
		Rtc_Set_Time(22,11,3,4);		//设置日期

		//RTC_WriteBackupRegister(RTC_BKP_DR0,0x5050);	//标记已经初始化过了
	} 
 
	return 0;
}

void RTC_Set_WakeUp(u32 wksel,u16 cnt)
{ 
	RTC_WakeUpCmd(DISABLE);//关闭WAKE UP
	
	RTC_WakeUpClockConfig(wksel);//唤醒时钟选择
	
	RTC_SetWakeUpCounter(cnt);//设置WAKE UP自动重装载寄存器
	
	
	RTC_ClearITPendingBit(RTC_IT_WUT); //清除RTC WAKE UP的标志
	EXTI_ClearITPendingBit(EXTI_Line22);//清除LINE22上的中断标志位 
	 
	RTC_ITConfig(RTC_IT_WUT,ENABLE);//开启WAKE UP 定时器中断
	RTC_WakeUpCmd( ENABLE);//开启WAKE UP 定时器　
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;//LINE22
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE22
	EXTI_Init(&EXTI_InitStructure);//配置
 
 
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
}

void rtc_alarm_init(void)
{	
	
	RTC_ClearFlag(RTC_FLAG_ALRAF);

	/*使能外部中断控制线17的中断*/
	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/*使能闹钟的中断 */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);

}

void rtc_alarm_set(RTC_AlarmTypeDef RTC_AlarmStructure)
{
	
	//初始化RTC闹钟
	
	RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
}

void RTC_WKUP_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_WUT)==SET)
	{
		RTC_ClearITPendingBit(RTC_IT_WUT);
	
	}	
	EXTI_ClearITPendingBit(EXTI_Line22);
}

extern u32 alarm_flag;
void RTC_Alarm_IRQHandler(void)
{
	uint32_t ulReturn;
	ulReturn = taskENTER_CRITICAL_FROM_ISR();	
	
	if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
	{
		
		GPIO_WriteBit(GPIOF,GPIO_Pin_9,Bit_RESET);
		printf("rtc alarm trig\r\n");
		alarm_flag=2;
		RTC_ClearITPendingBit(RTC_IT_ALRA);
	}
	EXTI_ClearITPendingBit(EXTI_Line17);
	/* 退出临界段 */
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );			
}
