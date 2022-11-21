#include "hc_sr04.h"

static GPIO_InitTypeDef GPIOInitStruct;
static TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

void hc_sr04_init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_T_E, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);//
	//配置PE5為輸出模式
	GPIOInitStruct.GPIO_Pin    = TRIG_P;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_OUT;//输出模式
	GPIOInitStruct.GPIO_OType  = GPIO_OType_PP;//推挽输出
	GPIOInitStruct.GPIO_Speed  = GPIO_Speed_50MHz;//低速
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//上拉输入
	GPIO_Init(TRIG_G,&GPIOInitStruct);

	GPIOInitStruct.GPIO_Pin    = ECHO_P;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_IN;//输入模式
	GPIO_Init(ECHO_G,&GPIOInitStruct);
	
	TIM_TimeBaseStructure.TIM_Period = 65535; //计数值
    TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1; //预分频

	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM6,ENABLE);
	//初始化
	GPIO_WriteBit(TRIG_G,TRIG_P,Bit_RESET);	 
}


//定时器处理
float hc_src_tim_get_data(void)
{
	
	GPIO_WriteBit(TRIG_G,TRIG_P,Bit_SET);	
	delay_us(20);	
	GPIO_WriteBit(TRIG_G,TRIG_P,Bit_RESET);
	
	//等待ECHO被拉高
	while(GPIO_ReadInputDataBit(ECHO_G,ECHO_P)==0);
	TIM6->CNT =0;
	TIM6->CR1 |= 0x01; 
	while(GPIO_ReadInputDataBit(ECHO_G,ECHO_P)==1);
	TIM6->CR1 &=~0X01; 
	return 0.017*TIM6->CNT;
}
//
float hc_sr04_get_data(void)
{
	uint32_t t=0;
	GPIO_WriteBit(TRIG_G,TRIG_P,Bit_SET);	
	delay_us(20);	
	GPIO_WriteBit(TRIG_G,TRIG_P,Bit_RESET);	//
	
	while(GPIO_ReadInputDataBit(ECHO_G,ECHO_P)==0)
	{
		t++;
		delay_us(1);
		if(t>=10000)//超時錯誤
			return -1;
	}
	t=0;
	while(GPIO_ReadInputDataBit(ECHO_G,ECHO_P))
	{
		t++;
		delay_us(9);
		
		if(t>=100000)
			return -2;
	}
	return 0.17*t;
}

