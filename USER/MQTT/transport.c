#include "transport.h"
#include "usart.h"
#include "string.h"
#include "led.h"
#include "delay.h"

#define USART_RX_MAXLEN  60  
u8  USART3_RX_BUFF[USART_RX_MAXLEN]; 
u16 USART3_RX_FLAG=0; 
u16 USART3_RX_CNT;
void USART3_Send_Buff(unsigned char* buf, int buflen)
{
	
	while(buflen--)//字符串是否发送完成
	{
		USART3->DR = *buf;//发送一个字符
		while(!(USART3->SR & (1<<7)));//等待发送完成
		buf++;
	}
}


int transport_sendPacketBuffer( unsigned char* buf, int buflen)
{
	USART3_RX_FLAG=0;
	USART3_RX_CNT=0;
	memset((char *)USART3_RX_BUFF,0,USART_RX_MAXLEN);//清空接收缓冲区
	USART3_Send_Buff(buf,buflen);//发送数据
	return buflen;
}


int transport_getdata(unsigned char* buf, int count)
{
	u16 rx_cnt=0;
	static u16 read_count=0;
	while(!USART3_RX_FLAG && rx_cnt<=100)//等待接收响应数据报
	{
		delay_ms(10);
		rx_cnt++;
	}
	if(rx_cnt>100) return 1;//超时,未接收到数据
	rx_cnt=0;
	memcpy(buf,USART3_RX_BUFF+read_count,count);
	read_count+=count;//USART3_BUFF数据偏移
	if(read_count>=USART3_RX_CNT)//已经读完USART3_BUFF数据
	{
		USART3_RX_FLAG=0;
		USART3_RX_CNT=0;
		read_count=0;
	}	
	return count;
}

int transport_getdatanb(void *sck, unsigned char* buf, int count)
{
	return 0;
}

int transport_open(char* addr, int port)
{
	return 0;
}

int transport_close(int sock)
{
	return 0;
}