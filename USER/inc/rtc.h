#ifndef __RTC_H
#define __RTC_H

#include "stm32f4xx.h"
//extern u32 get_rtc_wakeup_event;
ErrorStatus Rtc_Set_Data(u8 year,u8 month,u8 date,u8 weekday);
ErrorStatus Rtc_Set_Time(u8 h12,u8 hour,u8 min,u8 sec);
void rtc_init(void);
u8 My_RTC_Init(void);
void RTC_Set_WakeUp(u32 wksel,u16 cnt);
void rtc_alarm_init(void);
void rtc_alarm_set(RTC_AlarmTypeDef RTC_AlarmStructure);
#endif

