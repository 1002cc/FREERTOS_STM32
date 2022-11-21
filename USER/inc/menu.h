#ifndef __MENU_H
#define __MENU_H

#define uchar unsigned char
	
#include "u8g2.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stdio.h"
#include "rtc.h"


typedef struct
{
	uchar current; 						//当前索引号
	uchar prev;							//向上翻索引号
	uchar next;							//向下翻索引号
	uchar enter;						//确认索引号
	uchar back; 							//退出
	void (*current_operation)(void); 	//当前状态应该执行的操作
} Menu_table;


extern Menu_table table[30];

extern u8g2_t u8g2;

extern void home(void);
extern void Temperature(void);
extern void Palygame(void);
extern void Ligthset(void);
extern void Monitor( void);
extern void Unlock( void);
extern void Esp_Mqtt(void);
extern void Info(void);

extern void TestTemperature(void);
extern void Set(void);
extern void ConrtolGame(void);
extern void Ledset(void);
extern void UnlockSet(void);
extern void MQTTSet(void);
extern void Information(void);
extern void LED1(void);
  

#endif
