#include "dht11.h"

#define TEST printf("正在执行%s函数第%d行代码\n", __FILE__, __LINE__)
float g_temp=0.0;
float g_humi=0.0;
float g_length;
u32   g_ligth;
u32   g_mq;
u32   g_tf;
static GPIO_InitTypeDef GPIO_InitStructure;
//初始化PG9引腳
void dht11_init(GPIOMode_TypeDef gpio_mode)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=gpio_mode;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOG,&GPIO_InitStructure);
}

//发送启动信号
void dht11_start(void)
{
	dht11_init(GPIO_Mode_OUT);	
	GPIO_WriteBit(GPIOG,GPIO_Pin_9,Bit_SET);
	vTaskDelay(5);
	GPIO_WriteBit(GPIOG,GPIO_Pin_9,Bit_RESET);
	vTaskDelay(20);
	GPIO_WriteBit(GPIOG,GPIO_Pin_9,Bit_SET);
}

//等待DHT11响应
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

//信号结束
void dht11_stop(void)
{	
	dht11_init(GPIO_Mode_OUT);
	delay_us(100); 
}

//获取一个位的数据
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
			RDate |=0x01<<i;   //置1
			while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9));	
		}
		else
		{
			RDate &= ~(0x01<<i); //置0
		}
		
	}
	return RDate;
}

int32_t dht11_read(uint8_t *pdht_data)
{
	uint8_t data;
	uint32_t check_sum=0;
	//发送启动信号
	dht11_start();
	//等待DHT11响应
	dht11_avk();
	
	for(int j=0; j<5; j++)//连续接收5个字节
	{
		data = 0;
		for(int i=7; i>=0; i--)//完成8个bit数据的接收，高位优先
		{
			//等待低电平持续完毕
			while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9)==0);	
			delay_us(40);
			if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9))
			{
				data|=1<<i; 
				//等待数据1的高电平时间持续完毕
				while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9));	
			}
		}	
		pdht_data[j] = data;
	}
	//通信结束
	dht11_stop();
	//计算校验和
	check_sum = (pdht_data[0]+pdht_data[1]+pdht_data[2]+pdht_data[3]) & 0xFF;
	if(check_sum != pdht_data[4])
		return -2;
	return 0;
}

