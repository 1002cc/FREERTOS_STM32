#ifndef __USART_H
#define __USART_H

#include "stm32f4xx.h"                  // Device header
#include "led.h"
#include "stdio.h"
#include <string.h>
#include "stdarg.h"	
#include "cjson.h"
#define LAMP1ON	    1
#define LAMP1OFF	2
#define LAMP2ON 	3
#define LAMP2OFF 	4
#define BEEPON		5
#define BEEPOFF		6
#define DOOROPEN	7
#define MONTIORON 	8
#define MONTIOROFF 	9
#define SCREENSAVER 10
#define COMMANDERR	0XFF

#define USART_REC_LEN  			60  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
//USART1 USB串口
extern u8  USART1_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART1_RX_STA;         		//接收状态标记	
//USART2 蓝牙模块
extern u8  USART2_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART2_RX_STA;         		//接收状态标记
extern int USART2_LEN;
//USART3 WIFI模块
extern volatile u8  USART3_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART3_RX_STA;         		//接收状态标记
extern int USART3_LEN;

void mqtt_cjson_parse(char *pbuf);
void u3_printf(char* fmt, ...);
void u3_TxData(unsigned char *data);
void usart3_send_str(char *str);
void usart3_send_bytes(uint8_t *buf,uint32_t len);
void usartl_init(uint32_t baud);
void bluetooth_usart2_init(uint32_t baud);
void esp8266_usart3_init(uint32_t baud);
void usart_senddata(USART_TypeDef* USARTx,char *pstr);
void LowerToCap(u8 *str,u8 len);
u8 CommandProcess(u8 *str);
#endif
