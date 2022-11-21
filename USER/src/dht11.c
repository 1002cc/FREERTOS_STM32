#include "dht11.h"

#define TEST printf("����ִ��%s������%d�д���\n", __FILE__, __LINE__)
float g_temp=0.0;
float g_humi=0.0;
float g_length;
u32   g_ligth;
u32   g_mq;
u32   g_tf;
static GPIO_InitTypeDef GPIO_InitStructure;
//��ʼ��PG9���_
void dht11_init(GPIOMode_TypeDef gpio_mode)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=gpio_mode;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOG,&GPIO_InitStructure);
}

//���������ź�
void dht11_start(void)
{
	dht11_init(GPIO_Mode_OUT);	
	GPIO_WriteBit(GPIOG,GPIO_Pin_9,Bit_SET);
	vTaskDelay(5);
	GPIO_WriteBit(GPIOG,GPIO_Pin_9,Bit_RESET);
	vTaskDelay(20);
	GPIO_WriteBit(GPIOG,GPIO_Pin_9,Bit_SET);
}

//�ȴ�DHT11��Ӧ
int dht11_avk(void)
{
	dht11_init(GPIO_Mode_IN);	
	int t=0;
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9));
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9)==0);
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9))
	{
		delay_us(1);
		t++;
		if(t>40000)
			return -1;
	}
	return 1;
}

//�źŽ���
void dht11_stop(void)
{	
	dht11_init(GPIO_Mode_OUT);
	delay_us(100); 
}

//��ȡһ��λ������
unsigned char DHT11_ReadByte(void)
{
	dht11_init(GPIO_Mode_IN);
	unsigned char RDate=0;
	for(int i=7;i>=0;i--)
	{
	
		while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9)==0);
		delay_us(35);
		if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9)==1)
		{
			RDate |=0x01<<i;   //��1
			while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9));	
		}
		else
		{
			RDate &= ~(0x01<<i); //��0
		}
		
	}
	return RDate;
}

int32_t dht11_read(uint8_t *pdht_data)
{
	uint8_t data;
	uint32_t check_sum=0;
	//���������ź�
	dht11_start();
	//�ȴ�DHT11��Ӧ
	dht11_avk();
	
	for(int j=0; j<5; j++)//��������5���ֽ�
	{
		data = 0;
		for(int i=7; i>=0; i--)//���8��bit���ݵĽ��գ���λ����
		{
			//�ȴ��͵�ƽ�������
			while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9)==0);	
			delay_us(40);
			if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9))
			{
				data|=1<<i; 
				//�ȴ�����1�ĸߵ�ƽʱ��������
				while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9));	
			}
		}	
		pdht_data[j] = data;
	}
	//ͨ�Ž���
	dht11_stop();
	//����У���
	check_sum = (pdht_data[0]+pdht_data[1]+pdht_data[2]+pdht_data[3]) & 0xFF;
	if(check_sum != pdht_data[4])
		return -2;
	return 0;
}

