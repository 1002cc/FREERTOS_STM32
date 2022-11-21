#ifndef __TIM_H
#define __TIM_H

#include "stm32f4xx.h"                  // Device header
#include "stdint.h"
#include "led.h"
#include "delay.h"
void tim2_init(void);
void tim4_init(unsigned short int arr, unsigned short int psc);
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM9_Int_Init(u16 arr,u16 psc);
#endif
