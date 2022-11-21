#ifndef __HC_SR04_H
#define __HC_SR04_H

#include "stm32f4xx.h"
#include "stdio.h"
#include "delay.h"

//PC6 输出模式  PC7输入模式
#define RCC_T_E RCC_AHB1Periph_GPIOC

#define TRIG_G GPIOC
#define TRIG_P GPIO_Pin_9

#define ECHO_G GPIOC
#define ECHO_P GPIO_Pin_7

void hc_sr04_init(void);
void HCSR04_Init(void);
float hc_sr04_get_data(void);
float hc_src_tim_get_data(void);

#endif
