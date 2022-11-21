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
	RTC_DateStructure.RTC_Year = year;						//��
	RTC_DateStructure.RTC_Month = month;					//��
	RTC_DateStructure.RTC_Date = date;						//��
	RTC_DateStructure.RTC_WeekDay = weekday;				//����
	return RTC_SetDate(RTC_Format_BCD,&RTC_DateStructure);	//BCD��ʽ�����ڳ�ʼ��
}

ErrorStatus Rtc_Set_Time(u8 h12,u8 hour,u8 min,u8 sec)
{
	RTC_TimeStructure.RTC_H12 = h12;						//AM �� 24 Сʱ�� 
	RTC_TimeStructure.RTC_Hours = hour;						//Сʱ
	RTC_TimeStructure.RTC_Minutes = min;					//����
	RTC_TimeStructure.RTC_Seconds = sec;					//��
	return RTC_SetTime(RTC_Format_BCD,&RTC_TimeStructure);	//BCD��ʽ�����ڳ�ʼ�� 
}


u8 My_RTC_Init(void)
{
	u16 retry=0X1FFF; 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//ʹ��PWRʱ��
	PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ������� 
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x5050)		//�Ƿ��һ������?
	{
		RCC_LSEConfig(RCC_LSE_ON);//LSE ����    
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//���ָ����RCC��־λ�������,�ȴ����پ������
		{
			retry++;
			vTaskDelay(10);
		}
		if(retry==0)return 1;		//LSE ����ʧ��. 

		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��    
		RCC_RTCCLKCmd(ENABLE);	//ʹ��RTCʱ�� 

		RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//RTC�첽��Ƶϵ��(1~0X7F)
		RTC_InitStructure.RTC_SynchPrediv  = 0xFF;//RTCͬ����Ƶϵ��(0~7FFF)
		RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;//RTC����Ϊ,24Сʱ��ʽ
		RTC_Init(&RTC_InitStructure);

		Rtc_Set_Data(23,59,56,RTC_H12_AM);	//����ʱ��
		Rtc_Set_Time(22,11,3,4);		//��������

		//RTC_WriteBackupRegister(RTC_BKP_DR0,0x5050);	//����Ѿ���ʼ������
	} 
 
	return 0;
}

void RTC_Set_WakeUp(u32 wksel,u16 cnt)
{ 
	RTC_WakeUpCmd(DISABLE);//�ر�WAKE UP
	
	RTC_WakeUpClockConfig(wksel);//����ʱ��ѡ��
	
	RTC_SetWakeUpCounter(cnt);//����WAKE UP�Զ���װ�ؼĴ���
	
	
	RTC_ClearITPendingBit(RTC_IT_WUT); //���RTC WAKE UP�ı�־
	EXTI_ClearITPendingBit(EXTI_Line22);//���LINE22�ϵ��жϱ�־λ 
	 
	RTC_ITConfig(RTC_IT_WUT,ENABLE);//����WAKE UP ��ʱ���ж�
	RTC_WakeUpCmd( ENABLE);//����WAKE UP ��ʱ����
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;//LINE22
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE22
	EXTI_Init(&EXTI_InitStructure);//����
 
 
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����
}

void rtc_alarm_init(void)
{	
	
	RTC_ClearFlag(RTC_FLAG_ALRAF);

	/*ʹ���ⲿ�жϿ�����17���ж�*/
	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/*ʹ�����ӵ��ж� */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);

}

void rtc_alarm_set(RTC_AlarmTypeDef RTC_AlarmStructure)
{
	
	//��ʼ��RTC����
	
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
	/* �˳��ٽ�� */
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );			
}
