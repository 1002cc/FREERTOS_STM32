#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "esp8266.h"

static GPIO_InitTypeDef GPIO_InitStructrue;
static USART_InitTypeDef USART_InitStructrue;
static 	NVIC_InitTypeDef NVICInitStructrue;

//printf重定向
int fputc(int ch, FILE*p)
{
    USART_SendData(USART1, (u8)ch);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    return ch;
}

//发送字符串
void usart_senddata(USART_TypeDef* USARTx,char *pstr)
{
	char* p=pstr;
	while(*p!='\0')
	{
		USART_SendData(USARTx,*p);
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);
		USART_ClearFlag(USARTx,USART_FLAG_TXE);
		p++;
	}		
}

void usart3_send_str(char *str)
{
	char *p = str;
	
	while(*p!='\0')
	{
		USART_SendData(USART3,*p);
		//printf("正在发送:%c\r\n",*p);
		p++;
		//等待数据发送成功
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);
		USART_ClearFlag(USART3,USART_FLAG_TXE);
	}
}


void usart3_send_bytes(uint8_t *buf,uint32_t len)
{
	uint8_t *p = buf;
	
	while(len--)
	{
		USART_SendData(USART3,*p);
		
		p++;
		
		//等待数据发送成功
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);
		USART_ClearFlag(USART3,USART_FLAG_TXE);
	}
}


//将字符串中的小写字母转换为大写
//str:要转换的字符串
//len：字符串长度
void LowerToCap(u8 *str,u8 len)
{
	u8 i;
	for(i=0;i<len;i++)
	{
		if((96<str[i])&&(str[i]<123))	//小写字母
		str[i]=str[i]-32;				//转换为大写
	}
}

//解析数据
void mqtt_cjson_parse(char *pbuf)
{
	cJSON *json , *json_params, *json_led, *json_method;
	
	char *p = pbuf;
		
	//解析数据包
	json = cJSON_Parse(p);			
	if (!json)  
	{  
		cJSON_Delete(json);
		json=NULL;		
		return;
	} 		
	//{"user":"chen","params":{"led":1,"beep":1},"version":"1.1.0"}
	//根据method键获取值
	json_method = cJSON_GetObjectItem(json ,"user"); 
	if(json_method->type == cJSON_String)
	{
		printf("user:%s\r\n", json_method->valuestring);  
	}	
	//根据params键获取值
	json_params = cJSON_GetObjectItem(json , "params"); 
	if(json_params)
	{
		json_led=cJSON_GetObjectItem(json_params , "led"); 
		if(json_led->type == cJSON_Number)
		{
			printf("led:%d\r\n", json_led->valueint); 	
			if(json_led->valueint==0)
			{
				GPIO_WriteBit(GPIOF,GPIO_Pin_9,Bit_SET);
				GPIO_WriteBit(GPIOF,GPIO_Pin_10,Bit_SET);
				GPIO_WriteBit(GPIOE,GPIO_Pin_13,Bit_SET);
				GPIO_WriteBit(GPIOE,GPIO_Pin_14,Bit_SET);
			}
			if(json_led->valueint==1)
			{
				GPIO_WriteBit(GPIOF,GPIO_Pin_9,Bit_RESET);
				GPIO_WriteBit(GPIOF,GPIO_Pin_10,Bit_RESET);
				GPIO_WriteBit(GPIOE,GPIO_Pin_13,Bit_RESET);
				GPIO_WriteBit(GPIOE,GPIO_Pin_14,Bit_RESET);
			}
			if(json_led->valueint==2)
			{
				GPIO_WriteBit(GPIOF,GPIO_Pin_9,Bit_RESET);
			}
			if(json_led->valueint==3)
			{
				GPIO_WriteBit(GPIOF,GPIO_Pin_10,Bit_RESET);
			}
			if(json_led->valueint==4)
			{
				GPIO_WriteBit(GPIOE,GPIO_Pin_13,Bit_RESET);
			}
			if(json_led->valueint==5)
			{
				GPIO_WriteBit(GPIOE,GPIO_Pin_14,Bit_RESET);
			}
			if(json_led->valueint==6)
			{
				GPIO_WriteBit(GPIOF,GPIO_Pin_9,Bit_SET);
			}
			if(json_led->valueint==7)
			{
				GPIO_WriteBit(GPIOE,GPIO_Pin_10,Bit_SET);
			}
			if(json_led->valueint==8)
			{
				GPIO_WriteBit(GPIOE,GPIO_Pin_13,Bit_SET);
			}
			if(json_led->valueint==9)
			{
				GPIO_WriteBit(GPIOE,GPIO_Pin_14,Bit_SET);
			}
		}
		json_led=cJSON_GetObjectItem(json_params , "beep"); 
		if(json_led->type == cJSON_Number)
		{
			PFout(8) = json_led->valueint; 
			printf("beep:%d\r\n", json_led->valueint);  
		}				
	}
	cJSON_Delete(json);
	json=NULL;	
}


//命令处理函数，将字符串命令转换成命令值
//str：命令
//返回值: 0XFF，命令错误；其他值，命令值
u8 CommandProcess(u8 *str)
{
	u8 CommandValue=COMMANDERR;
	if(strcmp((char*)str,"LED1_ON")==0) 		CommandValue=1;
	else if(strcmp((char*)str,"LED1_OFF")==0) 	CommandValue=2;
	else if(strcmp((char*)str,"LED2_ON")==0) 	CommandValue=3;
	else if(strcmp((char*)str,"LED2_OFF")==0) 	CommandValue=4;
	else if(strcmp((char*)str,"BEEP_ON")==0) 	CommandValue=5;
	else if(strcmp((char*)str,"BEEP_OFF")==0) 	CommandValue=6;
	else if(strcmp((char*)str,"DOOR")==0) 		CommandValue=7;
	else if(strcmp((char*)str,"MONITOR_ON")==0) CommandValue=8;
	else if(strcmp((char*)str,"MONITOR_OFF")==0) CommandValue=9;
	else if(strcmp((char*)str,"SCREENSAVER")==0) CommandValue=10;
	return CommandValue;
}


//PA9 PA10 初始化串口1
void usartl_init(uint32_t baud)
{
    //打开PA硬件时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    //配置引脚 PA9 PA10
    GPIO_InitStructrue.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructrue.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructrue.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructrue.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructrue);
    //配置引脚复用为USART1功能
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
    //初始化配置USART
    USART_InitStructrue.USART_BaudRate	= baud; //配置波特率
    USART_InitStructrue.USART_WordLength = USART_WordLength_8b; //字长位八位数据格式
    USART_InitStructrue.USART_StopBits	= USART_StopBits_1; //一个停止位
    USART_InitStructrue.USART_Parity	= USART_Parity_No; //无奇偶校验位
    USART_InitStructrue.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
    USART_InitStructrue.USART_Mode		= USART_Mode_Rx | USART_Mode_Tx; //配置收发模式
    USART_Init(USART1, &USART_InitStructrue);
    //配置中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    //设置优先级
    NVICInitStructrue.NVIC_IRQChannel = USART1_IRQn;
    NVICInitStructrue.NVIC_IRQChannelPreemptionPriority = 8;
    NVICInitStructrue.NVIC_IRQChannelSubPriority =8;
    NVICInitStructrue.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVICInitStructrue);
    //使能串口
    USART_Cmd(USART1, ENABLE);
}

//HC_05蓝牙模块初始化 PA2 PA3 初始化串口2
void bluetooth_usart2_init(uint32_t baud)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    
    GPIO_InitStructrue.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructrue.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructrue.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructrue.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructrue.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructrue);
    //配置引脚复用为USART1功能
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART3);
    //初始化配置USART
    USART_InitStructrue.USART_BaudRate	= baud; //配置波特率
    USART_InitStructrue.USART_WordLength = USART_WordLength_8b; //字长位八位数据格式
    USART_InitStructrue.USART_StopBits	= USART_StopBits_1; //一个停止位
    USART_InitStructrue.USART_Parity	= USART_Parity_No; //无奇偶校验位
    USART_InitStructrue.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
    USART_InitStructrue.USART_Mode		= USART_Mode_Rx | USART_Mode_Tx; //配置收发模式
    USART_Init(USART2, &USART_InitStructrue);
    //配置中断
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    //设置优先级
    NVICInitStructrue.NVIC_IRQChannel = USART2_IRQn;
    NVICInitStructrue.NVIC_IRQChannelPreemptionPriority =8;
    NVICInitStructrue.NVIC_IRQChannelSubPriority = 8;
    NVICInitStructrue.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVICInitStructrue);
    //使能串口
    USART_Cmd(USART2, ENABLE);
}

//  PB10 PB11  使用串口3通信
void esp8266_usart3_init(uint32_t baud)
{
	 //打开硬件时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    //配置引脚 PB10 PB11
    GPIO_InitStructrue.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructrue.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructrue.GPIO_Speed=GPIO_High_Speed;
    GPIO_InitStructrue.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructrue.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructrue);
    //配置引脚复用为USART1功能
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
    //初始化配置USART
    USART_InitStructrue.USART_BaudRate	= baud; //配置波特率
    USART_InitStructrue.USART_WordLength = USART_WordLength_8b; //字长位八位数据格式
    USART_InitStructrue.USART_StopBits	= USART_StopBits_1; //一个停止位
    USART_InitStructrue.USART_Parity	= USART_Parity_No; //无奇偶校验位
    USART_InitStructrue.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
    USART_InitStructrue.USART_Mode		= USART_Mode_Rx | USART_Mode_Tx; //配置收发模式
    USART_Init(USART3, &USART_InitStructrue);
    //配置中断
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    //设置优先级
    NVICInitStructrue.NVIC_IRQChannel = USART3_IRQn;
    NVICInitStructrue.NVIC_IRQChannelPreemptionPriority = 8;
    NVICInitStructrue.NVIC_IRQChannelSubPriority = 8;
    NVICInitStructrue.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVICInitStructrue);
    //使能串口
    USART_Cmd(USART3, ENABLE);
}


u8 	USART1_RX_BUF[USART_REC_LEN];    
u16 USART1_RX_STA=0; 

u8  USART2_RX_BUF[USART_REC_LEN]; 
u16 USART2_RX_STA=0; 
int USART2_LEN=0;

volatile u8  USART3_RX_BUF[60]; 
u16 USART3_RX_STA=0;
int USART3_LEN=0;
extern QueueHandle_t USART1_Queue;
extern QueueHandle_t USART2_Queue;
extern QueueHandle_t USART3_Queue;
/*---------------------------------------------------------------*/
/*函数名：void USART1_IRQHandler(void *pvParameters)             */
/*功  能：USART1中断  USB串口中断								 */
/*参  数：无                          			   				 */
/*返回值：无                                       			     */
/*其  他： 									   					 */
/*---------------------------------------------------------------*/
void USART1_IRQHandler(void)                
{
	u8 Res;
	BaseType_t err;
	uint32_t ulReturn;
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
	{
		Res =USART_ReceiveData(USART1);
		
		if((USART1_RX_STA&0x8000)==0)
		{
		if(USART1_RX_STA&0x4000)
			{
			if(Res!=0x0a)USART1_RX_STA=0;
			else USART1_RX_STA|=0x8000;	
			}
			
		else 
			{	
			if(Res==0x0d)USART1_RX_STA|=0x4000;
			else
				{
				USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
				USART1_RX_STA++;
				if(USART1_RX_STA>(USART_REC_LEN-1))USART1_RX_STA=0;	  
				}		 
			}
		}  
		if((USART1_RX_STA&0x8000)&&(USART1_Queue!=NULL))
		{
			printf("USART1接收完成\r\n");
			u32 data=1;
			err = xQueueSendFromISR(USART1_Queue, (void *)&data,NULL); //覆写发送
			if (err != pdTRUE)
			{
				printf("发送失败\r\n");
			}
		
		}
	}
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );
} 

/*---------------------------------------------------------------*/
/*函数名：void USART2_IRQHandler(void *pvParameters)             */
/*功  能：USART2中断  蓝牙模块串口中断							 */
/*参  数：无                          			   				 */
/*返回值：无                                       			     */
/*其  他： 									   					 */
/*---------------------------------------------------------------*/
void USART2_IRQHandler(void)
{
	u8 Res;
	BaseType_t err;
	uint32_t ulReturn;
	ulReturn = taskENTER_CRITICAL_FROM_ISR(); 
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  
	{
		Res=USART_ReceiveData(USART2);
		USART2_RX_BUF[USART2_LEN++]=Res;
		if(Res=='#' || USART2_LEN>=(sizeof USART2_RX_BUF))
		{
			u32 data=1;
			err=xQueueSendFromISR(USART2_Queue,(void *)&data,NULL);
			if (err != pdTRUE)
			{
				printf("发送失败\r\n");
			}
		}			
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	} 
	taskEXIT_CRITICAL_FROM_ISR(ulReturn);
}

/*---------------------------------------------------------------*/
/*函数名：void USART3_IRQHandler(void *pvParameters)             */
/*功  能：USART3中断  WIFI模块串口中断							 */
/*参  数：无                          			   				 */
/*返回值：无                                       			     */
/*其  他： 									   					 */
/*---------------------------------------------------------------*/
extern u32 wifi_flag;
void USART3_IRQHandler(void)   
{   
	uint8_t d=0;
	//检测是否接收到数据
	uint32_t ulReturn;
	ulReturn = taskENTER_CRITICAL_FROM_ISR(); 
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		d=USART_ReceiveData(USART3);
		g_esp8266_rx_buf[g_esp8266_rx_cnt++]=d;
			
		if(g_esp8266_rx_cnt >= sizeof g_esp8266_rx_buf)
		{
			g_esp8266_rx_end=1;
		}
		//将接收到的数据返发给PC
		USART_SendData(USART1,d);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);	
		//清空标志位，可以响应新的中断请求
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
	taskEXIT_CRITICAL_FROM_ISR(ulReturn);
}












