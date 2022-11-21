#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"                  // Device header

void led_init ( void );

#define LED(a) if(a==9)\
	GPIO_ToggleBits(GPIOF,GPIO_Pin_9);\
	else if(a==10)\
	GPIO_ToggleBits(GPIOF,GPIO_Pin_10);\
	else if(a==13)\
	GPIO_ToggleBits(GPIOE,GPIO_Pin_13);\
	else if(a==14)\
	GPIO_ToggleBits(GPIOE,GPIO_Pin_14);
	
#define BEEP(a) if(a==1)\
	GPIO_WriteBit(GPIOF,GPIO_Pin_8,Bit_SET);\
	else \
	GPIO_WriteBit(GPIOF,GPIO_Pin_8,Bit_RESET);	
	

#endif
