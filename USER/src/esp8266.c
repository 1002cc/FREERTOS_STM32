#include "esp8266.h"
#include "FreeRTOS.h"
#include "task.h" 
#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

uint8_t  g_esp8266_tx_buf[512];
volatile uint8_t  g_esp8266_rx_buf[512];
volatile uint32_t g_esp8266_rx_cnt=0;
volatile uint32_t g_esp8266_rx_end=0;

volatile uint32_t g_esp8266_transparent_transmission_sta=0;

void esp8266_init(void)
{
	esp8266_usart3_init(115200);
}


void esp8266_send_at(char *str)
{
	memset((void *)g_esp8266_rx_buf,0, sizeof g_esp8266_rx_buf);
	
	g_esp8266_rx_cnt = 0;	
	
	usart3_send_str(str);
}

void esp8266_send_bytes(uint8_t *buf,uint32_t len)
{
	usart3_send_bytes(buf,len);

}

void esp8266_send_str(char *buf)
{
	usart3_send_str(buf);

}

/* ���ҽ������ݰ��е��ַ��� */
int32_t esp8266_find_str_in_rx_packet(char *str,uint32_t timeout)
{
	char *dest = str;
	char *src  = (char *)&g_esp8266_rx_buf;
	
	//�ȴ����ڽ�����ϻ�ʱ�˳�
	while((strstr(src,dest)==NULL) && timeout)
	{		
		delay_ms(1);
		timeout--;
	}

	if(timeout) 
		return 0; 
		                    
	return -1; 
}

int32_t esp8266_find(char *str,uint32_t timeout)
{
	char *dest = str;
	char *src  = (char *)&g_esp8266_rx_buf;
	
	//�ȴ����ڽ�����ϻ�ʱ�˳�
	while((strstr(src,dest)==NULL) && timeout)
	{		
		delay_ms(1);
		timeout--;
	}

	if(timeout) 
		return 0; 
		                    
	return -1; 
}


/* �Լ���� */
int32_t  esp8266_self_test(void)
{
	esp8266_send_at("AT\r\n");
	
	return esp8266_find_str_in_rx_packet("OK",1000);
}

/*���ܣ������ȵ�*/
int32_t esp8266_connect_ap(char* ssid,char* pswd)
{
    //����ΪSTATIONģʽ	
	esp8266_send_at("AT+CWMODE_CUR=1\r\n"); 
	
	if(esp8266_find_str_in_rx_packet("OK",2000))
		return -1;

	//����Ŀ��AP
	printf("%s:%s\r\n",ssid,pswd);
	esp8266_send_at("AT+CWJAP_CUR="); 
	esp8266_send_at("\"");esp8266_send_at(ssid);esp8266_send_at("\"");	
	esp8266_send_at(",");	
	esp8266_send_at("\"");esp8266_send_at(pswd);esp8266_send_at("\"");	
	esp8266_send_at("\r\n");
	if(esp8266_find_str_in_rx_packet("OK",5000))
		if(esp8266_find_str_in_rx_packet("CONNECT",5000))
			return -2;

	return 0;
}

/* �˳�͸��ģʽ */
int32_t esp8266_exit_transparent_transmission (void)
{

	esp8266_send_at ("+++");
	
	//�˳�͸��ģʽ��������һ��ATָ��Ҫ���1��
	delay_ms ( 1000 ); 
	
	//��¼��ǰesp8266�����ڷ�͸��ģʽ
	g_esp8266_transparent_transmission_sta = 0;

	return 0;
}

/* ����͸��ģʽ */
int32_t  esp8266_entry_transparent_transmission(void)
{
	//����͸��ģʽ
	//esp8266_send_at("\r\n");
	delay_ms(2000);
	esp8266_send_at("AT+CIPMODE=1\r\n");  
	if(esp8266_find_str_in_rx_packet("OK",7000))
		return -1;
	
	delay_ms(2000);
	//��������״̬
	esp8266_send_at("AT+CIPSEND\r\n");
	if(esp8266_find_str_in_rx_packet("OK",7000))
		return -2;

	//��¼��ǰesp8266������͸��ģʽ
	g_esp8266_transparent_transmission_sta = 1;
	return 0;
}


/*���ܣ�ʹ��ָ��Э��(TCP/UDP)���ӵ�������*/
int32_t esp8266_connect_server(char* mode,char* ip,uint16_t port)
{	
	char buf[16]={0};
	esp8266_send_at("AT+CIPSTART=");
	esp8266_send_at("\"");	esp8266_send_at(mode);	esp8266_send_at("\"");
	esp8266_send_at(",");
	esp8266_send_at("\"");	esp8266_send_at(ip);	esp8266_send_at("\"");	
	esp8266_send_at(",");
	sprintf(buf,"%d",port);
	esp8266_send_at(buf);	
	esp8266_send_at("\r\n");
	
	if(esp8266_find_str_in_rx_packet("CONNECT",5000))
		if(esp8266_find_str_in_rx_packet("OK",5000))
			return -1;
	return 0;
}

/* �Ͽ������� */
int32_t esp8266_disconnect_server(void)
{
	esp8266_send_at("AT+CIPCLOSE\r\n");
		
	if(esp8266_find_str_in_rx_packet("CLOSED",5000))
		if(esp8266_find_str_in_rx_packet("OK",5000))
			return -1;
	
	return 0;	
}


/* ʹ�ܶ����� */
int32_t esp8266_enable_multiple_id(uint32_t b)
{

	char buf[32]={0};
	
	sprintf(buf,"AT+CIPMUX=%d\r\n", b);
	esp8266_send_at(buf);
	
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -1;
	
	return 0;
}

/* ���������� */
int32_t esp8266_create_server(uint16_t port)
{
	char buf[32]={0};
	
	sprintf(buf,"AT+CIPSERVER=1,%d\r\n", port);
	esp8266_send_at(buf);
	
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -1;
	
	return 0;
}

/* �رշ����� */
int32_t esp8266_close_server(uint16_t port)
{
	char buf[32]={0};
	
	sprintf(buf,"AT+CIPSERVER=0,%d\r\n", port);
	esp8266_send_at(buf);
	
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -1;
	
	return 0;
}

/* ���Դ򿪻�ر� */
int32_t esp8266_enable_echo(uint32_t b)
{
	if(b)
		esp8266_send_at("ATE1\r\n"); 
	else
		esp8266_send_at("ATE0\r\n"); 
	
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -1;

	return 0;
}

/* ��λ */
int32_t esp8266_reset(void)
{
	esp8266_send_at("AT+RST\r\n");
	
	if(esp8266_find_str_in_rx_packet("OK",10000))
		return -1;

	return 0;
}


