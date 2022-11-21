#include "pwmsg.h"

static TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; //定时器结构体
static GPIO_InitTypeDef GPIO_InitStructure;				//定义引脚结果体
static TIM_OCInitTypeDef  TIM_OCInitStructure;			//PWM设置结构体

//引脚复用模式PWM呼吸灯
void TIM_GPIO_Config(void) //初始化引脚
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 ;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14); //连接TIM14定时器
	
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);
}

//初始化定时器1 (ch1) 14(ch3 ch4)
void tim1_tim14_init(void)
{

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    TIM_GPIO_Config();

    TIM_TimeBaseStructure.TIM_Period = (10000 / 100) - 1; //计数值  10ms
    TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1; //预分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数

    TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 100;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    //通道1
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

//pwm呼吸灯
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

//蜂鸣器 PF8
//=====================================================================================
void TIM_GPIOF_Config(void) //初始化引脚
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_TIM13); //连接TIM14定时器
}
//初始化定时器13
void tim13_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
    TIM_GPIOF_Config();

    TIM_TimeBaseStructure.TIM_Period = (10000 / 100) - 1; //计数值  
    TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1; //预分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数

    TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStructure);
	
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 100;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    //通道1
    TIM_OC1Init(TIM13, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM13, TIM_OCPreload_Enable);
	
    TIM_ARRPreloadConfig(TIM13, ENABLE);
    TIM_Cmd(TIM13, ENABLE);
}
//=====================================================================================


//初始化引脚复用功能   PE5 定时器9  1通道
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
	
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_TIM9); //引脚复用映射到TIM9
	
	TIM_TimeBaseStructure.TIM_Period = arr; //计数值 
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //预分频
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

//延迟控制舵机引脚初始化
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






