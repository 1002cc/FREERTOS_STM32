#include "my_oled.h"

void oled_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructture;
	GPIO_InitStructture.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; 
	GPIO_InitStructture.GPIO_Mode = GPIO_Mode_OUT;				//响应模式
    GPIO_InitStructture.GPIO_OType = GPIO_OType_PP;				//推挽
    GPIO_InitStructture.GPIO_Speed = GPIO_Speed_100MHz; 			//高速响应
    GPIO_InitStructture.GPIO_PuPd = GPIO_PuPd_NOPULL; 			//没有使能上拉/下拉电阻
	
	GPIO_Init ( GPIOB, &GPIO_InitStructture );
}
