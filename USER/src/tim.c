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
static TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; //��ʱ���ṹ��

//��ʱ�� 
void tim2_init(void)
{
	NVIC_InitTypeDef NVICInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	//ʱ����Ԫ����
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
//	TIM2->CR1 |= 0x01;//������ʱ��2
//	TIM3->CR1 |= 0x01;//������ʱ��3
//	TIM4->CR1 |= 0x01;//������ʱ��4
//	TIM5->CR1 |= 0x01;//������ʱ��5

	TIM_TimeBaseStructure.TIM_Period = 1000 - 1; //����ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1; //Ԥ��Ƶ
    //TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//Ĭ�����ϼ���
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	
		
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	NVICInitStruct.NVIC_IRQChannel = TIM2_IRQn;//�жϺ�ѡȡ
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 2;//���ȼ�0~15
	NVICInitStruct.NVIC_IRQChannelSubPriority = 2;//���ȼ�0~15
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
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=6; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=6; //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

