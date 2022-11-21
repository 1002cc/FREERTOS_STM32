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
	
	while(buflen--)//�ַ����Ƿ������
	{
		USART3->DR = *buf;//����һ���ַ�
		while(!(USART3->SR & (1<<7)));//�ȴ��������
		buf++;
	}
}


int transport_sendPacketBuffer( unsigned char* buf, int buflen)
{
	USART3_RX_FLAG=0;
	USART3_RX_CNT=0;
	memset((char *)USART3_RX_BUFF,0,USART_RX_MAXLEN);//��ս��ջ�����
	USART3_Send_Buff(buf,buflen);//��������
	return buflen;
}


int transport_getdata(unsigned char* buf, int count)
{
	u16 rx_cnt=0;
	static u16 read_count=0;
	while(!USART3_RX_FLAG && rx_cnt<=100)//�ȴ�������Ӧ���ݱ�
	{
		delay_ms(10);
		rx_cnt++;
	}
	if(rx_cnt>100) return 1;//��ʱ,δ���յ�����
	rx_cnt=0;
	memcpy(buf,USART3_RX_BUFF+read_count,count);
	read_count+=count;//USART3_BUFF����ƫ��
	if(read_count>=USART3_RX_CNT)//�Ѿ�����USART3_BUFF����
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