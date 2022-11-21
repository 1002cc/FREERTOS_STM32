#include "main.h"
/*
 *	���������������������޳�û������������������
 *       ��������������+ +
 *�����������ߩ��������������ߩ����� + +
 *���������������������� ��
 *�������������������� �� ++ + + +
 *����  ��  ������������������ ��+
 *���������������������� +
 *����  ���������ߡ��� ��
 *���������������������� + +
 *��������������������  ������
 *������������������������������������������
 *�������������������� + + + +
 *����������������������������Codes are far away from bugs with the animal protecting������
 *�������������������� + �����������ޱ���,������bug����
 *��������������������
 *������������������������+��+ + + +����������������
 *�������������� �� �������������� + +
 *������������ ��������������  	 �ǩ�����
 *������������ �������������� 	����
 *���������������� �������� �������� + + + +
 *���������������ϩϡ����ϩ�
 *���������������ߩ������ߩ�+ + + +
 ****************************************************
 *
 */
							/*ϵͳ�߼�*/
 /*
								��ʾ�¶Ⱥ�ʪ��
									^
									|
									|						----->WIFI----->MQTT------->΢��С����
	OLED(��ҳ��)----->��������----->��ʾʱ��--------------------->����
									|[��ʾ�л�]--->�༶�˵�
									|[����]�����͵����������л�(��)
									|		
							�û��޲�����������ģʽ
*/
 
//������ģ����������
/*
********************************************************************************************
*		PA9  PA10 	ʹ�ô���1	XCOM�������
*------------------------------------------------------------------------------------
*		PB10 PB11 	ʹ�ô���2   wifiģ��esp8266	΢��С������� �Լ�������ͼ�β�������С����
*		WIFI���Ʒ�����
*					
*------------------------------------------------------------------------------------
*		PA2  PA3  	ʹ�ô���3   HC_05����ģ��   �ֻ�������ƣ�	�������Ա�
* 		�������Ʒ���:
*					
*------------------------------------------------------------------------------------
*		OLED:PB8 PB9
*		OLED��ʾ�� 	1.��ǰʱ��
*					2.�û�������
*					3.��������
*					4.��Ϣ��ʾ
*------------------------------------------------------------------------------------
*		����������:������ʾ����
*------------------------------------------------------------------------------------
*		������ģ��:PC6 ���ģʽ  PC7����ģʽ
*		�����PE5 ��ʱ��9  1ͨ��
*		��������⣺������ģ�����Ŀ��,���180���Զ�ת��,
*					������ģ���鵽Ŀ�꣬���ͣ��,������LED�ƺͷ�������ʾ�;���
*------------------------------------------------------------------------------------
*		���󰴼�(��������)��
*					��Ϣ����---->���ݼ�ֵ
*					OUT: 	C1(PE11) C2(   PE12) C3(PE15) C4(PD9)
*					IN: 	R4(PE9) R3(PE10) R2(PD0) R1(PD14)

	//OUT: C1(PE11) C2(PE12) C3(PE15) C4(PD9) 
	//IN:  R4(PE9) R3(PE10) R2(PD0) R1(PD14)
*------------------------------------------------------------------------------------
*		�������裺PE7
*					ADC3 ͨ��5
*					DMA�������� ������0 ͨ��3
*------------------------------------------------------------------------------------
*		ģ��LED��ʹ�ó���
*		LED1 (PF9)  ¥�����ȵ� �ɹ���ǿ���Զ�����  Ҳ�����ֻ��Ͱ���(1)����
*		LED2 (PF10) �����������ʾ�����	���˿����ƻ�����ʱ��Խ����Խ�� û�������
*		LED3 (PE13) ��ͥʹ�õ� ���ֻ��Ͱ�������
*		LED4 (PE14) 1.��������ʧ��ָʾ��,
*					2.��������ʧ�ܵ���
*------------------------------------------------------------------------------------
*		������ʹ��
*		BEEP (PF8)  1.����������,���˳�ʱ��ͣ�����������
*					2.������������
*					3.�����RIE�������δ�����
*					4.�ֻ����ƾ���,��ʾ����
*		�������� A0(ADC�ɼ�)PB0 ADC12 IN8  DO(PB1)�ж�3 4
*		���洫���� 
*------------------------------------------------------------------------------------
*		��������:
				����
*				KEY1:�����л�
*				KEY2:�����л�
*				KEY3:�ر�
*				KEY4:����
********************************************************************************************
*/

									/*����ʹ��ͼ*/
/*
 * 
 * �����������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ���������������
 * �� GPIO  ��   0   ��   1   ��   2   ��   3   ��   4   ��   5   ��   6   ��   7   ��   8   ��   9   ��  10   ��   11  ��   12  ��   13  ��   14  ��   15  ��
 * �����������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة���������������
 * �����������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ���������������
 * ��  PA   ��  key1 ��       ��USART3 �� USART3��       ��       ��       ��       ��       �� USART1�� USART1��       ��       ��       ��       ��       ��
 * �����������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة���������������
 * �����������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ���������������
 * ��  PB   �� ����  ��       ��       ��       ��       ��       ��       ��       �� OLED  ��  OLED ��       ��       ��       ��       ��       ��       ��
 * �����������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة���������������
 * �����������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ���������������
 * ��  PC   ��       ��       ��       ��       ��       ��       ��       ��       ��       ��       ��       ��       ��       ��       ��       ��       ��
 * �����������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة���������������
 * �����������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ���������������
 * ��  PD   ��   R2  ��       ��       ��       ��       ��       ��       ��       ��       ��   C4  ��       ��       ��       ��       ��   R1  ��       ��
 * �����������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة���������������
 * �����������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ���������������
 * ��  PE   ��       ��       ��  key2 ��  key3 ��  key4 ��  ��� ��       ��   	   ��   R4  ��  R3   ��       ��   C1  ��   C2  �� LED3  �� LED4  ��   C3  ��
 * �����������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة���������������
 * �����������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ���������������
 * ��  PF   ��       ��       ��       ��       ��       ��       ��       �� ����  ��  BEEP ��   LED1��  LED2 ��       ��       ��       ��       ��       ��
 * �����������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة���������������
 * �����������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ��������������Щ���������������
 * ��  PG   ��       ��       ��       ��       ��       ��       ��       ��       ��       �� DHT11 ��       ��       ��       ��       ��       ��       ��
 * �����������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة��������������ة���������������
 */
/*�������ȼ�����*/
/*
	FREERTOS �ж����ȼ�Ҫ��5~32��Χ��
	���ܵ���5
*/
/*�ٽ������*/
/*
	��������һ��ִ��
	taskENTER_CRITICAL(); 	//�����ٽ���
	taskEXIT_CRITICAL();	//�˳��ٽ���
	����vTaskDelay(1000);delay_us �������������л� �ø������Ϊ����̬,Ȼ��ȥִ����������
	����Ϊ���ȼ�����,�����ӳٵ��ˣ���һ�������ִ�иú������룬��ʱ�ú�����Ϊ����̬���ȴ���ִ��
	����һЩ����ʱ��Ҫ����豸����룬Ϊ�˱�֤��˳��ִ�У�������ǰ������ٽ�δ��뱣��
										
*/
/*�жϴ���������������*/
/*���񴴽�*/
/*
	static void ������(void *pvParameters)
	{
		while(1)��for(;;)
		{
			//һ��Ҫ���ӳٺ���,���������л�
			//��Ȼ����ѭ��
			//ϵͳ��ס,�������񲻻�ִ��
			vTaskDelay(5000);
		}
	}
*/

const unsigned char wifi_m[] U8X8_PROGMEM = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf0,0x0f,0x00,0x00,0x00,0x80,0xff,0xff,0x01,0x00,0x00,0xe0,0xff,0xff,0x07,0x00,0x00,0xf8,0xff,0xff,0x1f,0x00,0x00,0xfe,0xff,0xff,0x7f,0x00,0x80,0xff,0xff,0xff,0xff,0x01,0xc0,0xff,0x03,0xc0,0xff,0x03,0xe0,0x7f,0x00,0x00,0xfe,0x07,0xf0,0x1f,0x00,0x00,0xf8,0x0f,0xf8,0x07,0x00,0x00,0xe0,0x1f,0xfc,0x03,0x00,0x00,0xc0,0x3f,0xfe,0x00,0xc0,0x03,0x00,0x7f,0x7f,0x00,0xfc,0x3f,0x00,0xfe,0x7f,0x80,0xff,0xff,0x01,0xfe,0x3f,0xe0,0xff,0xff,0x07,0xfc,0x1f,0xf0,0xff,0xff,0x0f,0xf8,0x04,0xf8,0xff,0xff,0x1f,0x60,0x00,0xfc,0x07,0xe0,0x3f,0x00,0x00,0xfe,0x01,0x80,0x7f,0x00,0x00,0x7f,0x00,0x00,0xfe,0x00,0x80,0x3f,0x00,0x00,0xfc,0x01,0x80,0x1f,0x00,0x00,0xf8,0x01,0xc0,0x0f,0x00,0x00,0xf0,0x03,0x80,0x07,0xf8,0x1f,0xe0,0x03,0x80,0x03,0xfe,0x7f,0xc0,0x01,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,0x01,0x00,0x00,0x80,0xff,0xff,0x03,0x00,0x00,0xc0,0x1f,0xf8,0x03,0x00,0x00,0xe0,0x0f,0xf0,0x07,0x00,0x00,0xe0,0x07,0xe0,0x07,0x00,0x00,0xe0,0x03,0xc0,0x07,0x00,0x00,0xc0,0x03,0xc0,0x03,0x00,0x00,0x80,0x01,0x80,0x01,0x00,0x00,0x00,0xc0,0x07,0x00,0x00,0x00,0x00,0xf0,0x0f,0x00,0x00,0x00,0x00,0xf8,0x0f,0x00,0x00,0x00,0x00,0xf8,0x1f,0x00,0x00,0x00,0x00,0xf8,0x1f,0x00,0x00,0x00,0x00,0xf8,0x1f,0x00,0x00,0x00,0x00,0xf8,0x1f,0x00,0x00,0x00,0x00,0xf0,0x0f,0x00,0x00,0x00,0x00,0xf0,0x0f,0x00,0x00,0x00,0x00,0xc0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };

const unsigned char wifi_s[] U8X8_PROGMEM = { 0x00,0x00,0xe0,0x07,0x78,0x1e,0x0c,0x30,0x06,0x60,0xe3,0xc7,0x30,0x0c,0x08,0x10,0x8c,0x30,0xe0,0x03,0x20,0x04,0x20,0x04,0x80,0x01,0x80,0x01,0x80,0x01,0x00,0x00 };
	
/*---------------------------------------------------------------*/
/*��������int main()                                             */
/*��  �ܣ�������							                     */
/*		  1.��ʼ��������ģ��  				     				 */
/*		  2.������ʼ�����ڿ�ʼ�����ﴴ��������������         */
/*		  3.�����������				       			 		 */
/*��  ������                          			   				 */
/*����ֵ����                                       			     */
/*---------------------------------------------------------------*/
int main()
{
	/* ����ϵͳ�ж����ȼ�����4 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	/*��ʼ��led��*/												
	led_init();
	/*��ʼ��delay*/
	delay_init(168);
	/*����1��ʼ��*/
	usartl_init(115200);
	/*����2����ģ��*/
	bluetooth_usart2_init(9600);
	/*wifi��ʼ��*/
	esp8266_init();
	/*������ģ���ʼ��*/
	hc_sr04_init();
	/*�����ʼ��*/
	pwmsg_init(2000 - 1, 840 - 1);
	/*��ʱ��3��ʼ��*/
	TIM3_Int_Init((10000 / 100) - 1,8400 - 1);
	/*������ʼ��*/
	key_init();
	/*���󰴼���ʼ��*/
	gpio4x4_init();
	/*��ʼ��ADC3*/
	adc_init();
	/*����ADC3��ת��*/
	ADC_SoftwareStartConv(ADC3);
	/*����������ʼ��*/
	mq_2_init();
	/*���洫����*/
	flame_transducer_init();
	/*����ADC1��ת��*/
	ADC_SoftwareStartConv(ADC1);
	/*����ADC2��ת��*/
	ADC_SoftwareStartConv(ADC2);
	/*��ʼ��RTC*/
	My_RTC_Init();
	rtc_alarm_init();
	/*�����ж�*/
	RTC_Set_WakeUp(RTC_WakeUpClock_CK_SPRE_16bits,0);
	/*RFID��ʼ��*/
	MFRC522_Initializtion();
	/*MQTT���ӳ�ʼ��*/
	esp8266_mqtt_init();
	xTaskCreate((TaskFunction_t)software_control_task,
				(char *)"start_task",
				(uint16_t)SOFTWARE_CONTROL_STK_SIZE,
				(void *)NULL,
				(UBaseType_t)SOFTWARE_CONTROL_TASK_PRIO,
				(TaskHandle_t *)&SOFTWARE_CONTROL_Handler);
	vTaskStartScheduler(); //�������������
	while (1);
}


/*---------------------------------------------------------------*/
/*��������void software_control_task(void *pvParameters)         */
/*��  �ܣ���ʼ�������ã�										 */
/*		  1.��ʼ��OLED											 */
/*		  2.���ſ�������										 */
/*		  3.�����ź�������Ϣ���е�����ͨ�ŷ�ʽ   				 */
/*		  4.������������       			 						 */
/*		  5.ɾ������       			 		    				 */
/*��  ������                          			   				 */
/*����ֵ����                                       			     */
/*---------------------------------------------------------------*/
void software_control_task(void *pvParameters)
{
	/*��ʼ��OLED*/
	OLED_Init();	
	/*��������*/
	system_animation();
	/*��ʼ��OLED*/
	IIC_U8G2_Init();
	/*��ʼ��u8g2*/
	u8g2Init(&u8g2);
	/*���ؽ�����*/
	testDrawProcess(&u8g2);
	u8g2_ClearBuffer(&u8g2);  //���������
	u8g2_DrawXBMP(&u8g2,40,10,48,48,wifi_m);
	u8g2_SendBuffer(&u8g2);	 //����	
	vTaskDelay(1000); 
	/*�����ٽ���*/
	taskENTER_CRITICAL(); 
	/*������Ϣ����*/
	/****************************************************************/
	KEY_ONE_Queue =  xQueueCreate(KEY_ONE_Length, sizeof(u32));
	KEY_NUM_Queue =  xQueueCreate(KEY_NUM_Length, sizeof(u8));
	USART1_Queue  =  xQueueCreate(QUEUE_USART_LEN,sizeof(u32));
	USART2_Queue  =  xQueueCreate(QUEUE_USART_LEN,sizeof(u32));
	USART3_Queue  =  xQueueCreate(QUEUE_USART_LEN,sizeof(u32));
	MQ_Queue	  =  xQueueCreate(KEY_NUM_Length, sizeof(u8));
	MQ1_Queue	  =  xQueueCreate(KEY_NUM_Length, sizeof(u8));
	/****************************************************************/
	/*������ֵ�ź���*/
	/****************************************************************/
	BinarySemaphore = xSemaphoreCreateBinary();
	/****************************************************************/
	printf("��ʼ��������\r\n");
	int i = 0;
	while (task_su[i].pxTaskCode)
	{
		xTaskCreate(task_su[i].pxTaskCode,		/* ������ں���		*/
					task_su[i].pcName,			/* �������� 		*/
					task_su[i].usStackDepth,	/* ����ջ��С 		*/
					task_su[i].pvParameters,	/* ������ں������� */
					task_su[i].uxPriority,		/* ��������ȼ� 	*/
					task_su[i].pxCreatedTask); 	/* ������ƿ�ָ��	*/
		i++;
	}
	//��ʼ�����Ź�
	//iwdg_init();
	/*��������*/
	/****************************************************************/
	vTaskSuspend(OLED_Handler);		   		//��ʼ������������ʾ
	vTaskSuspend(RTC_Handler);		   		//��ʼ����ʱ����ʾ
	vTaskSuspend(SG_HC_MONITOR_Handler);	//��ʼ�����������
	vTaskSuspend(DHT11_DATA_Handler);		//���𴫸�������
	vTaskSuspend(KEY4x4_Handler);			//����		
	vTaskSuspend(KEY4x4_PRO_Handler);		//����	
	vTaskSuspend(LIGTH_DATA_Handler);		//����
	vTaskSuspend(SUB_PUB_Handler);		//����		
	/****************************************************************/		
	printf("�����������\r\n");	
	vTaskDelete(SOFTWARE_CONTROL_Handler); 	//��ʼ����ɾ��
	printf("��ʼ����ɾ��\r\n");	
	taskEXIT_CRITICAL();
}

/*---------------------------------------------------------------*/
/*��������void key_switch_task(void *pvParameters)             	 */
/*��  �ܣ��๦�ܰ���							 	 	    	 */
/*		  1.����    				   					 		 */
/*		  2.˫�� 				   								 */
/*		  3.����				   						 		 */
/*		  3.��ϼ�	key1+key2			   						 */
/*��  ������                          			   				 */
/*����ֵ����                                       			     */
/*��  ��������1	key_val=1 	����  2:����   3:��ϼ�  4:˫��	 	 */
/*	 	  ����2	key_val=5 	����  6:����   7:��ϼ�  8:˫��	 	 */
/*	 	  ����3	key_val=9 	����  10:����  11:��ϼ�  12:˫��	 */
/*	      ����4 key_val=13 	����  14:����  15:��ϼ�  16:˫��	 */
/*	      key_val=17 key1+key2									 */
/*---------------------------------------------------------------*/
//�๦�ܰ����е�����
void key_switch_task(void* pvParameters)
{
	unsigned int key_val,fa;
	u8g2_SetFontMode(&u8g2, 1);
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
	while(1)
	{
		xQueueReceive(MQ1_Queue, &fa, 500);
		xQueueReceive(KEY_ONE_Queue, &key_val, portMAX_DELAY);
		if(s==0)
		{
			vTaskSuspend(SCREENSAVER_Handler);
			s++;
			key_val=0;
			vTaskResume(RTC_Handler);
		}
		if (0 != key_val) 
		{
			if(eTaskGetState(RTC_Handler)!=eSuspended)
			{
				vTaskSuspend(RTC_Handler);
			}
			if(eTaskGetState(DHT11_DATA_Handler)!=eSuspended)
			{
				vTaskSuspend(DHT11_DATA_Handler);
			}
			if(eTaskGetState(SG_HC_MONITOR_Handler)!=eSuspended)
			{
				vTaskSuspend(SG_HC_MONITOR_Handler);
			}
			if(eTaskGetState(KEY4x4_Handler)!=eSuspended)
			{
				vTaskSuspend(KEY4x4_Handler);
			}
			if(eTaskGetState(KEY4x4_PRO_Handler)!=eSuspended)
			{
				vTaskSuspend(KEY4x4_PRO_Handler);
			}
			switch(key_val)
			{
				case KEY_NONE:
					printf("��ֵ��Ч\r\n");
				break;
				case KEY1_SHORT:
					printf("����key1����\r\n");
					func_index = table[func_index].prev;    //���Ϸ�
				break;
				case KEY2_SHORT:
					printf("����key2����\r\n");
					func_index = table[func_index].next;    //���·�
				break;
				case KEY3_SHORT:
					printf("����key3����\r\n");
					func_index = table[func_index].enter;    //ȷ��
				break;
				case KEY4_SHORT:
					func_index=table[func_index].back;
					printf("����key4����\r\n");
				break;
				case KEY1_2_SHORT:
					printf("������ϼ�\r\n") ;
					func_index=11;
				break;
			}
			u8g2_SetFontMode(&u8g2, 1);
			u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
			if (func_index != last_index)
			{
				current_operation_index = table[func_index].current_operation;
				(*current_operation_index)();//ִ�е�ǰ��������
				last_index = func_index;
			}
			
		}
		vTaskDelay(1000); 
	}
	//vTaskDelete(xTaskGetCurrentTaskHandle());
}

/*---------------------------------------------------------------*/
/*��������void key_pro_task(void *pvParameters)                  */
/*��  �ܣ����󰴼�							 	 	         	 */
/*		  1.��ȡ����    				   						 */
/*��  ������                          			   				 */
/*����ֵ����                                       			     */
/*��  ����  												     */
/*---------------------------------------------------------------*/
static void key_one_task(void *pvParameters)
{
	unsigned int key_val;
	BaseType_t err;
	while (1)
	{
		key_val = key_scan(1);
		if (KEY_NONE != key_val) 
		{
			err = xQueueOverwrite(KEY_ONE_Queue, &key_val); //��д����
			printf("key_val:%d\r\n",key_val);
			if (err != pdTRUE)
			{
				printf("����ʧ��\r\n");
			}
		}
		
		vTaskDelay(100);
	}
}

static void key_pro_task(void *pvParameters)
{
	unsigned int key_val;
	BaseType_t err;
	while (1)
	{
		key_val = key4x4_scanf();
		printf("KEY VALUE = %d.\r\n", key_val);
		err = xQueueOverwrite(KEY_NUM_Queue, &key_val); //��д����
		if (err != pdTRUE)
		{
			printf("����ʧ��\r\n");
		}
		BEEP(1);
		vTaskDelay(100);
		BEEP(0);
		vTaskDelay(100);
	}
}
/*---------------------------------------------------------------*/
/*��������void bluetooth_data_task(void *pvParameters)           */
/*��  �ܣ����������մ���							 	 	     */
/*		  1.    				   							 	 */
/*��  ������                          			   				 */
/*����ֵ����                                       			     */
/*��  ����  												     */
/*---------------------------------------------------------------*/
void bluetooth_data_task(void *pvParameters)
{
	u32 CommandValue=COMMANDERR;
	u32 flag_usart=0;
	BaseType_t xReturn=pdFALSE;	
	uint32_t i=0;
	char *p = NULL;
	RTC_TimeTypeDef  	RTC_TimeStructure;
	RTC_DateTypeDef  	RTC_DateStructure;	
	RTC_AlarmTypeDef 	RTC_AlarmStructure;	
	while(1)
	{
		if(USART2_Queue!=NULL)
        {
			xReturn = xQueueReceive( USART2_Queue,/* ��Ϣ���еľ�� */
									&flag_usart, /* �õ�����Ϣ���� */
									portMAX_DELAY);/* �ȴ�ʱ��һֱ�� */
			if(xReturn != pdPASS)
			{
				continue;
			}
			bluetooth_flag=1;			
			if(flag_usart==1)
			{
				printf("USART2�յ�����:%s\r\n",USART2_RX_BUF);
				USART2_RX_BUF[USART2_LEN-1]='\0';
				printf("�����������:%s\r\n",USART2_RX_BUF);
				if(strstr((char *)USART2_RX_BUF,"TIME SET"))
				{
					strtok((char *)USART2_RX_BUF,"-");
				
					p=strtok(NULL,"-");
					i = atoi(p);
					
					if(i<12)
						RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
					else
						RTC_TimeStructure.RTC_H12     = RTC_H12_PM;
						
					i= (i/10)*16+i%10;
					RTC_TimeStructure.RTC_Hours   = i;
					
					p=strtok(NULL,"-");
					i = atoi(p);	
					
					i= (i/10)*16+i%10;	
					RTC_TimeStructure.RTC_Minutes = i;
					
					p=strtok(NULL,"-");
					i = atoi(p);	
					
					i= (i/10)*16+i%10;					
					RTC_TimeStructure.RTC_Seconds = i; 					
					
					RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);
					BEEP(1);
					vTaskDelay(500);
					BEEP(0);
					printf("ʱ���޸����\r\n");				
				}
				else if(strstr((char *)USART2_RX_BUF,"DATA SET"))
				{	
					p = strtok((char*)USART2_RX_BUF,"-");
					p = strtok(NULL,"-");
					i = atoi(p);
					i =(i/10)*16+i%10;
					RTC_DateStructure.RTC_Year 	= i;
					
					p = strtok(NULL,"-");
					i = atoi(p);
					i= (i/10)*16+i%10;
					RTC_DateStructure.RTC_Month = i;
					
					p = strtok(NULL,"-");
					i = atoi(p);
					i= (i/10)*16+i%10;
					RTC_DateStructure.RTC_Date	= i;
					
					p = strtok(NULL,"-");
					i = atoi(p);
					i= (i/10)*16+i%10;
					RTC_DateStructure.RTC_WeekDay = i;
					RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
					BEEP(1);
					vTaskDelay(500);
					BEEP(0);
					printf("�������\r\n");	
				}else if (strstr((char *)USART2_RX_BUF,"ALARM SET"))
				{
					RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
					vTaskDelay(10);
					strtok((char *)USART2_RX_BUF,"-");
					p=strtok(NULL,"-");
					i = atoi(p);	
					if(i<12)
						RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;
					else
						RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_PM;
					i= (i/10)*16+i%10;
					RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = i;
					p=strtok(NULL,"-");
					i = atoi(p);						
					i= (i/10)*16+i%10;	
					RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = i;
					p=strtok(NULL,"-");
					i = atoi(p);						
					i= (i/10)*16+i%10;					
					RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = i; 	
					vTaskDelay(10);
					alarm_flag=1;
					RTC_AlarmStructure.RTC_AlarmDateWeekDay=0X13;
					RTC_AlarmStructure.RTC_AlarmDateWeekDaySel=RTC_AlarmDateWeekDaySel_Date;
					RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;  
					RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);
					RTC_ITConfig(RTC_IT_ALRA, ENABLE);
					RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
					BEEP(1);
					vTaskDelay(500);
					BEEP(0);
				}
				else
				{
					LowerToCap(USART2_RX_BUF,USART2_LEN-1);						//���ַ���ת��Ϊ��д		
					printf("CommandStr:%s\r\n",USART2_RX_BUF);
					CommandValue=CommandProcess(USART2_RX_BUF);		//�������
					if(CommandValue!=COMMANDERR)
					{
						printf("����Ϊ:%s\r\n",USART2_RX_BUF);
						printf("CommandValue:%d\r\n",CommandValue);
						switch(CommandValue)						//��������
						{
							case LAMP1ON: 
								GPIO_WriteBit(GPIOF,GPIO_Pin_9,Bit_RESET);
								break;
							case LAMP1OFF:
								GPIO_WriteBit(GPIOF,GPIO_Pin_9,Bit_SET);
								break;
							case LAMP2ON:
								GPIO_WriteBit(GPIOE,GPIO_Pin_13,Bit_RESET);
								break;
							case LAMP2OFF:
								GPIO_WriteBit(GPIOE,GPIO_Pin_13,Bit_SET);
								break;
							case BEEPON:
								BEEP(1);
								break;
							case BEEPOFF:
								BEEP(0);
								break;
							case DOOROPEN:
								BEEP(1);
								vTaskDelay(1000);
								BEEP(0);
								TIM_SetCompare1(TIM9, 100);
								break;
							case MONTIORON:
								if(eTaskGetState(RTC_Handler)!=eSuspended)
								{
									vTaskSuspend(RTC_Handler);
								}
								if(eTaskGetState(SCREENSAVER_Handler)!=eSuspended)
								{
									vTaskSuspend(SCREENSAVER_Handler);
								}
								if(eTaskGetState(SG_HC_MONITOR_Handler)==eSuspended)
								{
									vTaskResume(SG_HC_MONITOR_Handler);
								}
								func_index=11;
								break;
							case MONTIOROFF:
								if(eTaskGetState(SG_HC_MONITOR_Handler)!=eSuspended)
								{
									vTaskSuspend(SG_HC_MONITOR_Handler);
								}
								func_index=0;
								break;
							case SCREENSAVER:
								if(eTaskGetState(SCREENSAVER_Handler)==eSuspended)
								{
									vTaskResume(SCREENSAVER_Handler);
								}							
								s=0;
								func_index = 0;
								break;
						}
					}
					else
					{	
						printf("��Ч���������������!!\r\n");
					}
				}
				memset(USART2_RX_BUF,0,USART_REC_LEN);
				USART2_RX_STA=0;
				USART2_LEN=0;
				flag_usart=0;
			}
			vTaskDelay(2000);  
			printf("��������bluetooth_data_task\r\n");
		}
	}
}

/*---------------------------------------------------------------*/
/*��������void read_data_task(void *pvParameters)           	 */
/*��  �ܣ���ȡ��������ʪ������,����ʾ				 	 	     */
/*		  1. ��ʪ������   				   						 */
/*		  1. ����ǿ��   				   						 */
/*		  1. ������������   				   					 */
/*��  ������                          			   				 */
/*����ֵ����                                       			     */
/*��  ���������ȡһ�� 		    							     */
/*---------------------------------------------------------------*/
void dht11_data_task(void* pvParameters)
{
	uint8_t buf1[5]; //�����ʪ������
	char strTemp[32]="";
	char strHumi[32]="";	
	while(1)
	{
		/*��ȡ��ʪ������,������ṹ��*/
		dht11_read(buf1);
		sprintf(strTemp, "Temp:%d.%d C",buf1[2],buf1[3]);
		sprintf(strHumi, "Humi:%d.%d %%",buf1[0],buf1[1]);
		u8g2_ClearBuffer(&u8g2);  //���������
		u8g2_DrawStr(&u8g2, 10, 30, strTemp);
		u8g2_DrawStr(&u8g2, 10, 45, strHumi);
		u8g2_SendBuffer(&u8g2);	 //����	
		vTaskDelay(1000); 			
	}
}

void adc_data_task(void* pvParameters)
{
	uint16_t mq2_val;
	uint16_t adc_fire_val;
	char strMq_2_val[16]="";
	char strFT_val[16]="";
	int mq_falg=0;
	int ft_flag=0;
	while(1)
	{
		/*��ȡ������������*/
		while(ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC)==RESET);//�ȴ�ADC3ת�����
		ADC_ClearFlag(ADC2,ADC_FLAG_EOC);
		mq2_val=ADC_GetConversionValue(ADC2);
		mq2_vol=3300*mq2_val/4095;
		/*���汨����*/
		while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);//�ȴ�ADC3ת�����
		adc_fire_val= ADC_GetConversionValue(ADC1);
		adc_fire_vol = 3300*adc_fire_val/4095;
		sprintf(strMq_2_val, "MQ_2: %d mv", mq2_vol);
		sprintf(strFT_val, 	 "FT: %d",adc_fire_vol);
		if(mq2_val>800 || mq2_vol> 800)
		{	
			BEEP(1);
			vTaskDelay(500);
			mq_falg=1;
			BEEP(0);
			LED(14);
		}
		if(adc_fire_val<550 || adc_fire_vol< 550)
		{
			BEEP(1);
			vTaskDelay(500);
			ft_flag=1;
			BEEP(0);
			LED(14);
		}
		if(func_index==9)
		{
			u8g2_ClearBuffer(&u8g2);  //���������
			u8g2_DrawStr(&u8g2, 10,15, strFT_val);
			u8g2_DrawStr(&u8g2, 10,30, strMq_2_val);
			if(mq_falg==0)
			{
				u8g2_DrawStr(&u8g2, 10,45, "MQ:Normal");
			}else if(mq_falg==1)
			{
				u8g2_DrawStr(&u8g2, 10,45, "MQ:Abnormal");
			}
			if(ft_flag==0)
			{
				u8g2_DrawStr(&u8g2, 10,60, "FT:Normal");
			}else if(ft_flag==1)
			{
				u8g2_DrawStr(&u8g2, 10,60, "FT:Abnormal");
			}
			u8g2_SendBuffer(&u8g2);	 //����	
		}
		vTaskDelay(1000); 	
	}
}

void ligth_data_task(void* pvParameters)
{
	u32 ligth_data;
	u32 ligth_val;
	char strLigth_data[32]="";
	while(1)
	{
		/*��ȡ����ǿ��*/
		ligth_val = ADC_VAL * 3300 / 4095;
		if(ligth_val==0)
			ligth_val=680;
		ligth_data=ligth_val-600;
		if(ligth_data>=600)
		{
			GPIO_WriteBit(GPIOE,GPIO_Pin_13,Bit_RESET);
		}else
		{
			GPIO_WriteBit(GPIOE,GPIO_Pin_13,Bit_SET);
		}
		printf("����ǿ��:%d\r\n", ligth_data);
		if(func_index==10)
		{
			sprintf(strLigth_data, "Ligth: %d Lx", ligth_data);
			u8g2_ClearBuffer(&u8g2);  //���������
			u8g2_DrawStr(&u8g2, 10, 35, strLigth_data);
			u8g2_SendBuffer(&u8g2);	 //����	
		}
		vTaskDelay(1000); 	
	}
}

/*---------------------------------------------------------------*/
/*��������void sg_hc_monitor_task(void *pvParameters)            */
/*��  �ܣ����������							 	 	         */
/*��  ������                          			   				 */
/*����ֵ����                                       			     */
/*��  ������϶��ת�� Ŀ�������Ŀ���������Ŀ�� ��������ʾ 	 */
/*		  �����ʱ��ͣ����ᾯ�� 	 							 */
/*---------------------------------------------------------------*/
static void sg_hc_monitor_task(void *pvParameters)
{
	float hc_length;
	int dir = 0, pwm_conu = 50;
	char  g_mqtt[64];
	char  g_mqtt1[64]={"alert Discovery target"};
	char strlength[32]="";
	int length_flag=0;
	u32 num=0;
	while (1)
	{
		taskENTER_CRITICAL(); 
		hc_length = hc_src_tim_get_data();
		printf("��ǰ����:%.2f\r\n", hc_length);
		if (hc_length < 10)
		{
			LED(14);
			length_flag=1;
			num++;
			if(num>=10)
			{
				BEEP(1);
				vTaskDelay(1000); 		
				BEEP(0);
				sprintf(g_mqtt,"{\"getmessage\": \"%s\"}",g_mqtt1);

				mqtt_publish_data(MQTT_PUBLISH_TOPIC,g_mqtt,0);
			}
		}
		else
		{
			if (dir)
				pwm_conu += 20;
			else
				pwm_conu -= 20;
			if (pwm_conu >= 499)
				dir = 0;
			if (pwm_conu <= 70)
				dir = 1;
			GPIO_WriteBit(GPIOE,GPIO_Pin_14,Bit_SET);
			length_flag=0;
			num=0;
			TIM_SetCompare1(TIM9, pwm_conu);
			vTaskDelay(50);
		}
		if(func_index==11)
		{
			sprintf(strlength, "%.2f m",hc_length);
			u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
			u8g2_ClearBuffer(&u8g2);  //���������
			u8g2_DrawStr(&u8g2, 32, 25, "Distance");
			u8g2_DrawStr(&u8g2, 35, 40, strlength);
			if(length_flag==1)
			{
				u8g2_DrawStr(&u8g2, 30, 50, "Targeted");
			}
			u8g2_SendBuffer(&u8g2);	 //����	
		}
		printf("��������sg_hc_monitor_task\r\n");
		vTaskDelay(500);
	}
}

/*---------------------------------------------------------------*/
/*��������void get_time_task(void *pvParameters)             	 */
/*��  �ܣ�RTCʱ��									 	 	     */
/*		  1.��ʱ				   							 	 */
/*��  ������                          			   				 */
/*����ֵ����                                       			     */
/*��  ����  												     */
/*---------------------------------------------------------------*/
const unsigned char alarm_s[] U8X8_PROGMEM = { 0x00,0x00,0x9c,0x39,0xe6,0x6f,0x1a,0x58,0x0c,0x30,0x84,0x21,0x84,0x21,0x86,0x61,0x06,0x63,0x04,0x26,0x04,0x20,0x0c,0x30,0x18,0x1c,0xf8,0x1f,0x8c,0x11,0x00,0x00 };
	
const unsigned char lanya[] U8X8_PROGMEM = { 0x00,0x00,0x80,0x03,0x80,0x07,0x80,0x1d,0x8c,0x39,0xbc,0x3d,0xf0,0x0f,0xc0,0x07,0xe0,0x07,0xf0,0x0f,0xbc,0x3d,0x8c,0x39,0x80,0x1d,0x80,0x07,0x80,0x03,0x80,0x00 };

static void get_time_task(void *pvParameters)
{
	int delay_1ms_cnt=0;
	uint8_t buf[5];
	char str_rtc_date[32]={0},str_time[32]={0};
	RTC_DateTypeDef RTC_DateStruct;
	RTC_TimeTypeDef RTC_TimeStruct;
	while (1)
	{
		RTC_GetDate(RTC_Format_BCD,&RTC_DateStruct);;
		RTC_GetTime(RTC_Format_BCD,&RTC_TimeStruct);
		sprintf(str_rtc_date,"20%02x-%02x-%02x",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date);
		sprintf(str_time,"%02x-%02x-%02x",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds);
		u8g2_ClearBuffer(&u8g2);  //���������
		if(bluetooth_flag==1)
		{
			u8g2_DrawXBMP(&u8g2,0,0,16,16,lanya);
		}
		if(wifi_flag==1)
		{
			u8g2_DrawXBMP(&u8g2,50,0,16,16,wifi_s);
		}
		if(alarm_flag==1)
		{
			u8g2_DrawXBMP(&u8g2,25,0,16,16,alarm_s);
		}
		else if(alarm_flag==2)
		{
			BEEP(1);
			vTaskDelay(1000); 		
			BEEP(0);
			alarm_flag=0;
		}
		u8g2_SetFont(&u8g2,u8g2_font_t0_22b_tr);
		u8g2_DrawStr(&u8g2, 10, 30, str_rtc_date);
		u8g2_SetFont(&u8g2,u8g2_font_profont29_tf);
		u8g2_DrawStr(&u8g2, 0, 60, str_time);
		u8g2_SendBuffer(&u8g2);	 //����		
		vTaskDelay(500);
		delay_1ms_cnt++;
		if((delay_1ms_cnt % 4) ==0)
		{	
			if(0 == dht11_read(buf))
			{
				g_temp = (float)buf[2]+(float)buf[3]/10;
				g_humi = (float)buf[0]+(float)buf[1]/10;
				g_length = hc_src_tim_get_data();
				g_ligth=ADC_VAL * 3300 / 4095;
				g_mq=(int)mq2_vol;
				g_tf=(int)adc_fire_vol;
			}
			printf("�ϱ��豸״̬\r\n");
			//�ϱ��豸״̬
			mqtt_report_devices_status();	
		}
	}
	// vTaskDelete(xTaskGetCurrentTaskHandle());
}

/*---------------------------------------------------------------*/
/*��������void iwdg_task(void *pvParameters)             		 */
/*��  �ܣ��������Ź�ι��							 	 	     */
/*		  1.ι��				   							 	 */
/*��  ������                          			   				 */
/*����ֵ����                                       			     */
/*��  ����  												     */
/*---------------------------------------------------------------*/
static void iwdg_task(void *pvParameters)
{
	while (1)
	{
		IWDG_ReloadCounter(); //ι��
		vTaskDelay(50);
		printf("��������iwdg_task\r\n");
	}
}

/*---------------------------------------------------------------*/
/*��������void screensaver_task(void *pvParameters)              */
/*��  �ܣ�����								 	 	         	 */
/*		  1.����̫���˶���    			 						 */
/*��  ������                          			   				 */
/*����ֵ����                                       			     */
/*��  ���� �޲���һ��ʱ�䲥������ 							     */
/*---------------------------------------------------------------*/
static void screensaver_task(void *pvParameters)
{
	while (1)
	{
		oled_ui();
		vTaskDelay(100);
	}
}

/*---------------------------------------------------------------*/
/*��������void usart1_task(void *pvParameters)              	 */
/*��  �ܣ�USART1��������					 	 	         	 */
/*		  1.�޸�����ʱ��			    			 			 */
/*��  ������                          			   				 */
/*����ֵ����                                       			     */
/*��  ����DATA SET-22-11-9	�޸�����ָ��					     */
/*		  TIME SET-20-4-0	�޸�ʱ��ָ��					     */
/*---------------------------------------------------------------*/
static void usart1_task(void* pvParameters)
{
	uint32_t i=0;
	char *p = NULL;
	u32 flag_usart=0;
	RTC_TimeTypeDef  	RTC_TimeStructure;
	RTC_DateTypeDef  	RTC_DateStructure;	
	BaseType_t xReturn=pdFALSE;	
	vTaskDelay(50);
	while(1)
	{	
		/*���յ������ж�����*/
		xReturn = xQueueReceive( USART1_Queue,/* ��Ϣ���еľ�� */
								&flag_usart, /* �õ�����Ϣ���� */
								portMAX_DELAY);/* �ȴ�ʱ��һֱ�� */
		printf("USART1�յ�����:%s\r\n",USART1_RX_BUF);
		if(xReturn != pdPASS)
		{
			continue;
		}			
		if(flag_usart==1)
		{			
			/* ����ʱ�� */
			if(strstr((char *)USART1_RX_BUF,"TIME SET"))
			{
				/* �ԵȺŷָ��ַ��� */
				strtok((char *)USART1_RX_BUF,"-");
				
				/* ��ȡʱ */
				p=strtok(NULL,"-");
				i = atoi(p);
				
				
				/* ͨ��ʱ���ж���AM����PM */
				if(i<12)
					RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
				else
					RTC_TimeStructure.RTC_H12     = RTC_H12_PM;
					
				/* ת��ΪBCD���� */
				i= (i/10)*16+i%10;
				RTC_TimeStructure.RTC_Hours   = i;
				
				/* ��ȡ�� */
				p=strtok(NULL,"-");
				i = atoi(p);	
				
				/* ת��ΪBCD���� */
				i= (i/10)*16+i%10;	
				RTC_TimeStructure.RTC_Minutes = i;
				
				/* ��ȡ�� */
				p=strtok(NULL,"-");
				i = atoi(p);	
				
				/* ת��ΪBCD���� */
				i= (i/10)*16+i%10;					
				RTC_TimeStructure.RTC_Seconds = i; 					
				
				/* ����RTCʱ�� */
				RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);
				printf("ʱ���޸����\r\n");				
			}
			/* �������� */
			else if(strstr((char *)USART1_RX_BUF,"DATA SET"))
			{	
				//������
				p = strtok((char*)USART1_RX_BUF,"-");
				p = strtok(NULL,"-");
				i = atoi(p);
				i =(i/10)*16+i%10;
				RTC_DateStructure.RTC_Year 	= i;
				
				//������
				p = strtok(NULL,"-");
				i = atoi(p);
				i= (i/10)*16+i%10;
				RTC_DateStructure.RTC_Month = i;
				
				//������
				p = strtok(NULL,"-");
				i = atoi(p);
				i= (i/10)*16+i%10;
				RTC_DateStructure.RTC_Date	= i;
				
				//�������ڼ�
				p = strtok(NULL,"-");
				i = atoi(p);
				i= (i/10)*16+i%10;
				RTC_DateStructure.RTC_WeekDay = i;
				
				RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
				printf("�������\r\n");	
			}
			memset(USART1_RX_BUF,0,USART_REC_LEN);
			USART1_RX_STA=0;
			flag_usart=0;
		}

	}
}

/*---------------------------------------------------------------*/
/*��������void rfid_task(void *pvParameters)             		 */
/*��  �ܣ�RFID����						 	     				 */
/*		  1.��ȡ����    				   						 */
/*��  ������                          			   				 */
/*����ֵ����                                       			     */
/*��  ���������λ��,�����������ᾯ������ʱ�䲻����		     */
/*		  �Զ��������	     									 */
/* 		  4*4������� ��ȡ���� 									 */
/*		 'C'����RFID����			 						   	 */
/*		 '#'���������������									 */
/*		 '*'���һλ��������									 */
/*---------------------------------------------------------------*/
const unsigned char co[] U8X8_PROGMEM = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0x3f,0x00,0x00,0x00,0x00,0xf0,0xff,0x00,0x00,0x00,0x00,0xf8,0xff,0x01,0x00,0x00,0x00,0xfc,0xff,0x03,0x00,0x00,0x00,0x7e,0xf0,0x07,0x00,0x00,0x00,0x1f,0xc0,0x07,0x00,0x00,0x00,0x1f,0x80,0x0f,0x00,0x00,0x00,0x0f,0x80,0x0f,0x00,0x00,0x80,0x0f,0x00,0x0f,0x00,0x00,0x80,0x0f,0x00,0x1f,0x00,0x00,0x80,0x0f,0x00,0x0f,0x00,0x00,0x80,0x0f,0x00,0x0f,0x00,0x00,0x80,0x0f,0x00,0x00,0x00,0x00,0x80,0x0f,0x00,0x00,0x00,0x8c,0x80,0x0f,0x00,0x00,0x80,0xff,0xff,0xff,0x03,0x00,0xc0,0xff,0xff,0xff,0x07,0x00,0xe0,0xff,0xff,0xff,0x0f,0x00,0xe0,0xff,0xff,0xff,0x0f,0x00,0xf0,0xff,0xff,0xff,0x0f,0x00,0xf0,0xff,0xc3,0xff,0x0f,0x00,0xf0,0xff,0x83,0xff,0x0f,0x00,0xf0,0xff,0x01,0xff,0x0f,0x00,0xf0,0xff,0x01,0xff,0x0f,0x00,0xf0,0xff,0x83,0xff,0x0f,0x00,0xf0,0xff,0xc7,0xff,0x0f,0x00,0xf0,0xff,0xc7,0xff,0x0f,0x00,0xf0,0xff,0xc7,0xff,0x0f,0x00,0xf0,0xff,0xc7,0xff,0x0f,0x00,0xf0,0xff,0xc7,0xff,0x0f,0x00,0xf0,0xff,0xc7,0xff,0x0f,0x00,0xf0,0xff,0xcf,0xff,0x0f,0x00,0xf0,0xff,0xff,0xff,0x0f,0x00,0xe0,0xff,0xff,0xff,0x0f,0x00,0xe0,0xff,0xff,0xff,0x0f,0x00,0xc0,0xff,0xff,0xff,0x07,0x00,0x80,0xff,0xff,0xff,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
//RFID�̶������ж�	
static void rfid_task(void* parameter)
{
	int rfid_flag=0;
	u8 status,card_size;
	char  g_mqtt[64];
	char  g_mqtt1[64]={"Opening the door"};
	xQueueReceive(MQ_Queue, &rfid_flag, portMAX_DELAY);
	if(rfid_flag==1)
	{
	while(1)
	{
			MFRC522_Initializtion();												//��BUGÿ��ʹ��ʱ��Ҫ��ʼ��һ��
			
			status=MFRC522_Request(0x52, card_pydebuf);			//Ѱ��						//Ѱ��
			
			if(status==0)		//���������													//���������
			{
				status=MFRC522_Anticoll(card_numberbuf);			//��ײ����			
				card_size=MFRC522_SelectTag(card_numberbuf);	//ѡ��
				status=MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);	//�鿨
				status=MFRC522_Write(4, card_writebuf);				//д����д��ҪС�ģ��ر��Ǹ����Ŀ�3��
				status=MFRC522_Read(4, card_readbuf);					//����					//ѡ��
				
				printf("card_size %d Kbits\r\n",card_size);
				
				PFout(8)=1;
				delay_ms(200);
				PFout(8)=0;
				TIM_SetCompare1(TIM9, 100);
				printf("�����ɹ�\r\n");
				sprintf(g_mqtt,"{\"getmessage\": %s}",g_mqtt1);
				u8g2_ClearBuffer(&u8g2);  //���������
				u8g2_DrawXBMP(&u8g2,40,10,48,48,co);
				u8g2_SendBuffer(&u8g2);	 //����	
				xQueueOverwrite(MQ1_Queue, &rfid_flag); //��д����
			}	
			vTaskDelay(1000);
		}
	}
}

/*---------------------------------------------------------------*/
/*��������void receive_data_task(void *pvParameters)             */
/*��  �ܣ���ȡ���󰴼�����ֵ						 	     	 */
/*		  1.��ȡ����    				   						 */
/*��  ������                          			   				 */
/*����ֵ����                                       			     */
/*��  ���������λ��,�����������ᾯ������ʱ�䲻����		     */
/*		  ����ģʽ,��receive_data_task����ʱ����,ʱ�䵽		 */
/*		  �Զ��������	     									 */
/* 		  4*4������� ��ȡ���� 									 */
/*		 'C'����RFID����			 						   	 */
/*		 '#'���������������									 */
/*		 '*'���һλ��������									 */
/*---------------------------------------------------------------*/
static void key4x4_task(void* pvParameters)
{
	u8 	i=0;
	char password[9]={0};
	char key_val=0;
	char str='*';
	char strdispaly[9]=" "; //����-
	int pass_num=0;
	int rfid_flag=1;
	char  g_mqtt[64];
	char  g_mqtt1[64]={"Opening the door"};
	while(1)
	{
		u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
		u8g2_ClearBuffer(&u8g2);  //���������
		u8g2_DrawStr(&u8g2, 20, 20,"password");
		u8g2_DrawStr(&u8g2, 30, 30, strdispaly);
		u8g2_SendBuffer(&u8g2);	 //����	
		xQueueReceive(KEY_NUM_Queue,&key_val,portMAX_DELAY);
		if(key_val!='*')
		{
			printf("key_num:%d\r\n",key_val);
			if(key_val=='B') //�˸�
			{
				i--;
				password[i]='\0';
				strdispaly[i]=' ';
			
			}if(key_val=='A') //���
			{
				i=0;
				memset(password,0,9);
				memset(strdispaly,0,9);
			}
			else
			{
				password[i]=key_val;			
				strdispaly[i]=str;
				i++;
			}		
			printf("password:%s\r\n",password);
			printf("key_val:%c\r\n",key_val);
			vTaskDelay(10);
			if(i >= 8)
			{			
				if(strcmp(password,"12345678")==0)
				{
					printf("����������ȷ\r\n");
					sprintf(g_mqtt,"{\"getmessage\": %s}",g_mqtt1);
					u8g2_ClearBuffer(&u8g2);  //���������
					u8g2_DrawStr(&u8g2, 10, 30, "Password sure");
					u8g2_SendBuffer(&u8g2);	 //����	
					vTaskDelay(2000);  
					u8g2_ClearBuffer(&u8g2);  //���������
					u8g2_DrawStr(&u8g2, 5, 30, "Opening the door");
					u8g2_SendBuffer(&u8g2);	 //����	
					vTaskDelay(2000);
					u8g2_ClearBuffer(&u8g2);  //���������
					u8g2_DrawStr(&u8g2, 10, 30, "Welcome home");
					u8g2_SendBuffer(&u8g2);	 //����	
					vTaskDelay(2000);
					TIM_SetCompare1(TIM9, 100);
					u8g2_ClearBuffer(&u8g2);  //���������
					u8g2_DrawXBMP(&u8g2,40,10,48,48,co);
					u8g2_SendBuffer(&u8g2);	 //����	
					vTaskDelay(2000);
					memset(password,0,9);
					memset(strdispaly,0,9);					
					s=0;
					func_index = 0;
				}else
				{
					pass_num++;
					printf("�����������\r\n");
					u8g2_ClearBuffer(&u8g2);  //���������
					u8g2_DrawStr(&u8g2, 5, 30, "Password error");
					u8g2_SendBuffer(&u8g2);	 //����	
					vTaskDelay(2000);
					if(pass_num>=3)
					{
						BEEP(1);
						vTaskDelay(1000);
						BEEP(0);
						printf("�����������\r\n");
						printf("�����ѹ���\r\n");
						vTaskSuspend(xTaskGetCurrentTaskHandle());
						pass_num=0;
					}
					i=0;
					memset(password,0,9);
					memset(strdispaly,0,9);
				}
				i=0;
			}
		}else
	  	{
			xQueueOverwrite(MQ_Queue, &rfid_flag); //��д����
		}
	}
}

static void esp8266_mqtt_task(void* pvParameters)
{
	uint32_t 	i=0;
	while(1)
	{
		if(g_esp8266_rx_end && g_esp8266_transparent_transmission_sta)
		{		
			for(i=0;i<g_esp8266_rx_cnt;i++)
			{	
				if(g_esp8266_rx_buf[i] == '{')
				{
					mqtt_cjson_parse((char *)&g_esp8266_rx_buf[i]);
					break;
				}
			}
			//��ս��ջ����������ռ���ֵ�����ս�����־λ
			memset((void *)g_esp8266_rx_buf,0,sizeof g_esp8266_rx_buf);
			g_esp8266_rx_cnt=0;
			g_esp8266_rx_end=0;
		}
		vTaskDelay(1000);
	}
}
