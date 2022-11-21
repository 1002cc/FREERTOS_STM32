#include "my_oled.h"

void oled_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructture;
	GPIO_InitStructture.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; 
	GPIO_InitStructture.GPIO_Mode = GPIO_Mode_OUT;				//��Ӧģʽ
    GPIO_InitStructture.GPIO_OType = GPIO_OType_PP;				//����
    GPIO_InitStructture.GPIO_Speed = GPIO_Speed_100MHz; 			//������Ӧ
    GPIO_InitStructture.GPIO_PuPd = GPIO_PuPd_NOPULL; 			//û��ʹ������/��������
	
	GPIO_Init ( GPIOB, &GPIO_InitStructture );
}
