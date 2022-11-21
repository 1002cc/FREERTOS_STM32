#include "ir.h"

static GPIO_InitTypeDef GPIO_InitStructure;
//初始化PA8引腳
void ir_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
}

int read_ir_data(unsigned char *buff)
{
	uint8_t data=0;
	uint32_t check_sum=0;
	if(PAin(8))
		return -1;
	int t=0;
	while(PAin(8)==0)
	{
		t++;;
		delay_us(10);
		if(t>=600)
			return -2;
	}
	for(int j=0;j<4;j++)
	{
		for(int i=0;i<8;i++) //最低有效位接收
		{
		
			while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9)==0);
			delay_us(700);
			if(PAin(8)==1)
			{
				data |=1<<i;   //置1
				while(GPAin(8));	
			}
		}
		buff[j]=data;
	}
	if(buff[0]+buff[1]==255)
		if(buff[2]+buff[3]==255)
			return 0;
	return -3;
}
