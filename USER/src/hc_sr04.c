#include "hc_sr04.h"

static GPIO_InitTypeDef GPIOInitStruct;
static TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

void hc_sr04_init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_T_E, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);//
	//����PE5��ݔ��ģʽ
	GPIOInitStruct.GPIO_Pin    = TRIG_P;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_OUT;//���ģʽ
	GPIOInitStruct.GPIO_OType  = GPIO_OType_PP;//�������
	GPIOInitStruct.GPIO_Speed  = GPIO_Speed_50MHz;//����
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//��������
	GPIO_Init(TRIG_G,&GPIOInitStruct);

	GPIOInitStruct.GPIO_Pin    = ECHO_P;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_IN;//����ģʽ
	GPIO_Init(ECHO_G,&GPIOInitStruct);
	
	TIM_TimeBaseStructure.TIM_Period = 65535; //����ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1; //Ԥ��Ƶ

	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM6,ENABLE);
	//��ʼ��
	GPIO_WriteBit(TRIG_G,TRIG_P,Bit_RESET);	 
}


//��ʱ������
float hc_src_tim_get_data(void)
{
	
	GPIO_WriteBit(TRIG_G,TRIG_P,Bit_SET);	
	delay_us(20);	
	GPIO_WriteBit(TRIG_G,TRIG_P,Bit_RESET);
	
	//�ȴ�ECHO������
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
		if(t>=10000)//���r�e�`
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

