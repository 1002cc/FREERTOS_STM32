#include "led.h"

void led_init ( void )
{
    //ʹ�ܶ˿�F��ʱ��
    //����һ���ṹ��,������
    GPIO_InitTypeDef GPIO_InitStructture;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOE, ENABLE);
    //���ýṹ����    ����һ�������� PE8
    GPIO_InitStructture.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10; //��������
    GPIO_InitStructture.GPIO_Mode = GPIO_Mode_OUT;				//��Ӧģʽ
    GPIO_InitStructture.GPIO_OType = GPIO_OType_PP;				//����
    GPIO_InitStructture.GPIO_Speed = GPIO_Speed_50MHz; 			//������Ӧ
    GPIO_InitStructture.GPIO_PuPd = GPIO_PuPd_NOPULL; 			//û��ʹ������/��������

    //��ʼ��GPIOF
    GPIO_Init ( GPIOF, &GPIO_InitStructture );
    //���PE13��14��
    GPIO_InitStructture.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_Init ( GPIOE, &GPIO_InitStructture );

    //�����е�Ϩ��
    GPIO_WriteBit(GPIOF, GPIO_Pin_9 | GPIO_Pin_10, Bit_SET);
    GPIO_WriteBit(GPIOE, GPIO_Pin_13 | GPIO_Pin_14, Bit_SET);
}



