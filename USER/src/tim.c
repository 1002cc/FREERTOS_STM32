#include "tim.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "delay.h"
#include "led.h"
#include "tim.h"
#include "usart.h"
#include "stdio.h"	
#include "malloc.h"	 
static TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; //定时器结构体

//定时器 
void tim2_init(void)
{
	NVIC_InitTypeDef NVICInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	//时基单元配置
//	TIM2->PSC = 8400-1;
//	TIM2->ARR = (2000) - 1; 
//	
//	TIM3->PSC = 8400-1;
//	TIM3->ARR = (5000) - 1; 
//	
//	TIM4->PSC = 8400-1;
//	TIM4->ARR = (8000) - 1; 
//	
//	TIM5->PSC = 8400-1;
//	TIM5->ARR = (12000) - 1; 
//	
//	TIM2->CNT = 0;
//	TIM3->CNT = 0;
//	TIM4->CNT = 0;
//	TIM5->CNT = 0;
//	TIM2->DIER |=0x01;
//	TIM3->DIER |=0x01;
//	TIM4->DIER |=0x01;
//	TIM5->DIER |=0x01;	
//	TIM2->CR1 |= 0x01;//开启定时器2
//	TIM3->CR1 |= 0x01;//开启定时器3
//	TIM4->CR1 |= 0x01;//开启定时器4
//	TIM5->CR1 |= 0x01;//开启定时器5

	TIM_TimeBaseStructure.TIM_Period = 1000 - 1; //计数值
    TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1; //预分频
    //TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//默认向上计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	
		
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	NVICInitStruct.NVIC_IRQChannel = TIM2_IRQn;//中断号选取
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 2;//优先级0~15
	NVICInitStruct.NVIC_IRQChannelSubPriority = 2;//优先级0~15
	NVICInitStruct.NVIC_IRQChannelCmd   =  ENABLE;
	NVIC_Init(&NVICInitStruct);
	
	TIM_Cmd(TIM2, ENABLE);
}


void TIM2_IRQHandler(void)
{

    if(TIM_GetITStatus(TIM2, TIM_IT_Update)==1)
	{

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=6; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=6; //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

