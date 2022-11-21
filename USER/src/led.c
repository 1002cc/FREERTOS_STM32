#include "led.h"

void led_init ( void )
{
    //使能端口F打开时钟
    //定义一个结构体,并配置
    GPIO_InitTypeDef GPIO_InitStructture;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOE, ENABLE);
    //设置结构参数    定义一个蜂鸣器 PE8
    GPIO_InitStructture.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10; //设置引脚
    GPIO_InitStructture.GPIO_Mode = GPIO_Mode_OUT;				//响应模式
    GPIO_InitStructture.GPIO_OType = GPIO_OType_PP;				//推挽
    GPIO_InitStructture.GPIO_Speed = GPIO_Speed_50MHz; 			//高速响应
    GPIO_InitStructture.GPIO_PuPd = GPIO_PuPd_NOPULL; 			//没有使能上拉/下拉电阻

    //初始化GPIOF
    GPIO_Init ( GPIOF, &GPIO_InitStructture );
    //添加PE13、14灯
    GPIO_InitStructture.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_Init ( GPIOE, &GPIO_InitStructture );

    //让所有灯熄灭
    GPIO_WriteBit(GPIOF, GPIO_Pin_9 | GPIO_Pin_10, Bit_SET);
    GPIO_WriteBit(GPIOE, GPIO_Pin_13 | GPIO_Pin_14, Bit_SET);
}



