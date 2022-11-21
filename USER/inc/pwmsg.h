#ifndef __PWMSG_H
#define __PWMSG_H

#include "stm32f4xx.h"
#include "delay.h"

#define GPIO_Pin_X GPIO_Pin_5
#define GPIOX GPIOE
#define RCC_AHB1Periph_GPIOX RCC_AHB1Periph_GPIOE
#define RCC_APB2Periph_TIMX RCC_APB2Periph_TIM9
#define TIMX TIM9
//Õ®µ¿¥˙¬Î≈‰÷√

void pwmsg_init(u16 arr ,u16 psc);
void tim1_tim14_init(void);
void Breathing_lamp(void);
void TIM_GPIOF_Config(void);
void tim13_init(void);
#endif
