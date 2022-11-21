#include "pwmsg.h"

static TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; //��ʱ���ṹ��
static GPIO_InitTypeDef GPIO_InitStructure;				//�������Ž����
static TIM_OCInitTypeDef  TIM_OCInitStructure;			//PWM���ýṹ��

//���Ÿ���ģʽPWM������
void TIM_GPIO_Config(void) //��ʼ������
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //����ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 ;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14); //����TIM14��ʱ��
	
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);
}

//��ʼ����ʱ��1 (ch1) 14(ch3 ch4)
void tim1_tim14_init(void)
{

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    TIM_GPIO_Config();

    TIM_TimeBaseStructure.TIM_Period = (10000 / 100) - 1; //����ֵ  10ms
    TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1; //Ԥ��Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���

    TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 100;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    //ͨ��1
    TIM_OC1Init(TIM14, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);
	
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 50;
	
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
    //TIM_ARRPreloadConfig(TIM14, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_Cmd(TIM1, ENABLE);
    TIM_Cmd(TIM14, ENABLE);
}

//pwm������
//void Breathing_lamp(void)
//{
//	for(u8 pwm_conu=0;pwm_conu<=100;pwm_conu++)
//	{
//	
//		TIM_SetCompare3(TIM1,pwm_conu);
//		TIM_SetCompare4(TIM1,pwm_conu);
//		TIM_SetCompare1(TIM14,pwm_conu);
//		delay_ms(10);
//	}
//	for(u8 pwm_conu=100;pwm_conu>=0;pwm_conu--)
//	{
//		TIM_SetCompare3(TIM1,pwm_conu);
//		TIM_SetCompare4(TIM1,pwm_conu);
//		TIM_SetCompare1(TIM14,pwm_conu);
//		delay_ms(10);
//	}
//}
//=====================================================================================

//������ PF8
//=====================================================================================
void TIM_GPIOF_Config(void) //��ʼ������
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //����ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_TIM13); //����TIM14��ʱ��
}
//��ʼ����ʱ��13
void tim13_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
    TIM_GPIOF_Config();

    TIM_TimeBaseStructure.TIM_Period = (10000 / 100) - 1; //����ֵ  
    TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1; //Ԥ��Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���

    TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStructure);
	
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 100;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    //ͨ��1
    TIM_OC1Init(TIM13, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM13, TIM_OCPreload_Enable);
	
    TIM_ARRPreloadConfig(TIM13, ENABLE);
    TIM_Cmd(TIM13, ENABLE);
}
//=====================================================================================


//��ʼ�����Ÿ��ù���   PE5 ��ʱ��9  1ͨ��
void GPIO_PWM_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOX,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_X;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOX,&GPIO_InitStructure);
}

void pwmsg_init(u16 arr ,u16 psc)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIMX,ENABLE);
	
	GPIO_PWM_Init();
	
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_TIM9); //���Ÿ���ӳ�䵽TIM9
	
	TIM_TimeBaseStructure.TIM_Period = arr; //����ֵ 
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //Ԥ��Ƶ
	TIM_TimeBaseInit(TIMX, &TIM_TimeBaseStructure);	
		
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	
	TIM_OC1Init(TIMX, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIMX, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIMX,ENABLE);
	TIM_Cmd(TIMX, ENABLE);
}

//�ӳٿ��ƶ�����ų�ʼ��
void PWMSG_Init(void)
{
	GPIO_InitTypeDef GPIOInitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_6;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_OUT;
	GPIOInitStruct.GPIO_OType  = GPIO_OType_PP;
	GPIOInitStruct.GPIO_Speed  = GPIO_Low_Speed;
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&GPIOInitStruct);

}






