#include "oled_u8g2.h"
#include "FreeRTOS.h"
#include "task.h"

void IIC_U8G2_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOB, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = SCL_Pin|SDA_Pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_GPIO_Port, &GPIO_InitStructure);
}

uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
	taskENTER_CRITICAL(); //�����ٽ���
    switch (msg)
    {
    case U8X8_MSG_DELAY_100NANO: // delay arg_int * 100 nano seconds
        __NOP();
        break;
    case U8X8_MSG_DELAY_10MICRO: // delay arg_int * 10 micro seconds
        for (uint16_t n = 0; n < 320; n++)
        {
            __NOP();
        }
        break;
    case U8X8_MSG_DELAY_MILLI: // delay arg_int * 1 milli second
        delay_ms(1);
        break;
    case U8X8_MSG_DELAY_I2C: // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
        delay_us(5);
        break;                    // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
    case U8X8_MSG_GPIO_I2C_CLOCK: // arg_int=0: Output low at I2C clock pin
		if(arg_int == 1) 
		{
			GPIO_SetBits(IIC_GPIO_Port, SCL_Pin);
		}
		else if(arg_int == 0)
		{
			GPIO_ResetBits(IIC_GPIO_Port, SCL_Pin);  
		}  
        break;                    // arg_int=1: Input dir with pullup high for I2C clock pin
    case U8X8_MSG_GPIO_I2C_DATA:  // arg_int=0: Output low at I2C data pin
        if(arg_int == 1) 
		{
			GPIO_SetBits(IIC_GPIO_Port, SDA_Pin);
		}
		else if(arg_int == 0)
		{
			GPIO_ResetBits(IIC_GPIO_Port, SDA_Pin);  
		} 
        break;                    // arg_int=1: Input dir with pullup high for I2C data pin
    case U8X8_MSG_GPIO_MENU_SELECT:
        u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_NEXT:
        u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_PREV:
        u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_HOME:
        u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
        break;
    default:
        u8x8_SetGPIOResult(u8x8, 1); // default return value
        break;
    }
	taskEXIT_CRITICAL();				   	//�˳��ٽ���
    return 1;
}

//��������ʾ
void testDrawProcess(u8g2_t *u8g2)
{
	for(int i=10;i<=80;i=i+2)
	{
		u8g2_ClearBuffer(u8g2); 
			
		char buff[20];
		sprintf(buff,"%d%%",(int)(i/80.0*100));
		
		u8g2_SetFont(u8g2,u8g2_font_ncenB12_tf);
		u8g2_DrawStr(u8g2,16,32,"SYSTEM GO--");//�ַ���ʾ
		
		u8g2_SetFont(u8g2,u8g2_font_ncenB08_tf);
		u8g2_DrawStr(u8g2,100,49,buff);//��ǰ������ʾ
		
		u8g2_DrawRBox(u8g2,16,40,i,10,4);//Բ��������ο�
		u8g2_DrawRFrame(u8g2,16,40,80,10,4);//Բ�Ǿ���
		
		u8g2_SendBuffer(u8g2);
	}
	delay_ms(500);
}

void u8g2Init(u8g2_t *u8g2)
{
	u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay);  // ��ʼ�� u8g2 �ṹ��
	u8g2_InitDisplay(u8g2); // ������ѡ��оƬ���г�ʼ����������ʼ����ɺ���ʾ�����ڹر�״̬
	u8g2_SetPowerSave(u8g2, 0); // ����ʾ��
	u8g2_ClearBuffer(u8g2);
}

void draw(u8g2_t *u8g2)
{
	u8g2_ClearBuffer(u8g2); 
	
    u8g2_SetFontMode(u8g2, 1); /*����ģʽѡ��*/
    u8g2_SetFontDirection(u8g2, 0); /*���巽��ѡ��*/
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf); /*�ֿ�ѡ��*/
    u8g2_DrawStr(u8g2, 0, 20, "U");
    
    u8g2_SetFontDirection(u8g2, 1);
    u8g2_SetFont(u8g2, u8g2_font_inb30_mn);
    u8g2_DrawStr(u8g2, 21,8,"8");
        
    u8g2_SetFontDirection(u8g2, 0);
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf);
    u8g2_DrawStr(u8g2, 51,30,"g");
    u8g2_DrawStr(u8g2, 67,30,"\xb2");
    
    u8g2_DrawHLine(u8g2, 2, 35, 47);
    u8g2_DrawHLine(u8g2, 3, 36, 47);
    u8g2_DrawVLine(u8g2, 45, 32, 12);
    u8g2_DrawVLine(u8g2, 46, 33, 12);
  
    u8g2_SetFont(u8g2, u8g2_font_4x6_tr);
    u8g2_DrawStr(u8g2, 1,54,"github.com/olikraus/u8g2");
	
	u8g2_SendBuffer(u8g2);
	delay_ms(1000);
}




//������� ����Ӣ�Ŀ�ѡ�� u8g2_font_ncenB..(��) ϵ������
//u8g2_font_unifont_t_symbols/u8g2_font_unifont_h_symbols(ϸ Բ��)
void testShowFont(u8g2_t *u8g2)
{
	int t = 1000;
	char testStr[14] = "STM32F103C8T6";
	
	u8g2_ClearBuffer(u8g2);
	
	u8g2_SetFont(u8g2,u8g2_font_u8glib_4_tf);
	u8g2_DrawStr(u8g2,0,5,testStr);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	
	u8g2_SetFont(u8g2,u8g2_font_ncenB08_tf);
	u8g2_DrawStr(u8g2,0,30,testStr);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	
    u8g2_SetFont(u8g2,u8g2_font_ncenB10_tr);
	u8g2_DrawStr(u8g2,0,60,testStr);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
}
//�����ľ���
void testDrawFrame(u8g2_t *u8g2)
{
	int t = 1000;
	int x = 16;
	int y = 32;
	int w = 50;
	int h = 20;
	u8g2_ClearBuffer(u8g2);
	u8g2_DrawStr(u8g2,0, 15, "DrawFrame");

	u8g2_DrawFrame(u8g2, x, y, w, h);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawFrame(u8g2, x+w+5, y-10, w-20, h+20);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
}

//��ʵ��Բ�Ǿ���
void testDrawRBox(u8g2_t *u8g2)
{
	int t = 1000;
	int x = 16;
	int y = 32;
	int w = 50;
	int h = 20;
	int r = 3;
	u8g2_ClearBuffer(u8g2);
	u8g2_DrawStr(u8g2,0, 15, "DrawRBox");

	u8g2_DrawRBox(u8g2, x, y, w, h, r);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawRBox(u8g2, x+w+5, y-10, w-20, h+20, r);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
}

//������Բ
void testDrawCircle(u8g2_t *u8g2)
{
	int t = 600;
	int stx = 0;  //��ͼ��ʼx
	int sty = 16; //��ͼ��ʼy
	int with = 16;//һ��ͼ��ļ��
	int r = 15;   //Բ�İ뾶
	u8g2_ClearBuffer(u8g2);
	u8g2_DrawStr(u8g2, 0, 15, "DrawCircle");

	u8g2_DrawCircle(u8g2, stx, sty - 1 + with, r, U8G2_DRAW_UPPER_RIGHT); //����
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawCircle(u8g2, stx + with, sty, r, U8G2_DRAW_LOWER_RIGHT); //����
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawCircle(u8g2, stx - 1 + with * 3, sty - 1 + with, r, U8G2_DRAW_UPPER_LEFT); //����
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawCircle(u8g2, stx - 1 + with * 4, sty, r, U8G2_DRAW_LOWER_LEFT); //����
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawCircle(u8g2, stx - 1 + with * 2, sty - 1 + with * 2, r, U8G2_DRAW_ALL);//����Բ
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	
    u8g2_DrawCircle(u8g2, 32*3, 32, 31, U8G2_DRAW_ALL);//�Ҳ�����Բ
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
}

//��ʵ����Բ
void testDrawFilledEllipse(u8g2_t *u8g2)
{
	int t = 800;
	int with = 16;//һ��ͼ��ļ��
	int rx = 27;  //��Բx����İ뾶
	int ry = 22;  //��Բy����İ뾶
	u8g2_ClearBuffer(u8g2);
	u8g2_DrawStr(u8g2,0, 14, "DrawFilledEllipse");

	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawFilledEllipse(u8g2, 0, with, rx, ry, U8G2_DRAW_LOWER_RIGHT);//����
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawFilledEllipse(u8g2, with * 4 - 1, with, rx, ry, U8G2_DRAW_LOWER_LEFT); //����
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawFilledEllipse(u8g2, 0, with * 4 - 1, rx, ry, U8G2_DRAW_UPPER_RIGHT); //����
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawFilledEllipse(u8g2, with * 4 - 1, with * 4 - 1, rx, ry, U8G2_DRAW_UPPER_LEFT); //����
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawFilledEllipse(u8g2, with * 6, with * 2.5, rx, ry, U8G2_DRAW_ALL);//������Բ
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
}

//���β���
void testDrawMulti(u8g2_t *u8g2)
{
	u8g2_ClearBuffer(u8g2);
	for (int j = 0; j < 64; j+=16)
	{
		for (int i = 0; i < 128; i+=16)
		{
			u8g2_DrawPixel(u8g2, i, j);
			u8g2_SendBuffer(u8g2);
		}
	}
  
	//ʵ�ľ����𽥱��
    u8g2_ClearBuffer(u8g2);
	for(int i=30; i>0; i-=2)
	{
		u8g2_DrawBox(u8g2,i*2,i,128-i*4,64-2*i);
		u8g2_SendBuffer(u8g2);
	}
	//���ľ����𽥱�С
	u8g2_ClearBuffer(u8g2);
	for(int i=0; i<32; i+=2)
	{
		u8g2_DrawFrame(u8g2,i*2,i,128-i*4,64-2*i);
		u8g2_SendBuffer(u8g2);
	}
	
	//ʵ��Բ�Ǿ����𽥱��
	u8g2_ClearBuffer(u8g2);
	for(int i=30; i>0; i-=2)
	{
		u8g2_DrawRBox(u8g2,i*2,i,128-i*4,64-2*i,10-i/3);
		u8g2_SendBuffer(u8g2);
	}
    //����Բ�Ǿ����𽥱�С
	u8g2_ClearBuffer(u8g2);
	for(int i=0; i<32; i+=2)
	{
		u8g2_DrawRFrame(u8g2,i*2,i,128-i*4,64-2*i,10-i/3);
		u8g2_SendBuffer(u8g2);
	}
	
	//ʵ��Բ�𽥱��
	u8g2_ClearBuffer(u8g2);
	for(int i=2; i<64; i+=3)
	{
		u8g2_DrawDisc(u8g2,64,32,i, U8G2_DRAW_ALL);
		u8g2_SendBuffer(u8g2);
	}
	//����Բ�𽥱�С
	u8g2_ClearBuffer(u8g2);
	for(int i=64; i>0; i-=3)
	{
		u8g2_DrawCircle(u8g2,64,32,i, U8G2_DRAW_ALL);
		u8g2_SendBuffer(u8g2);
	}
	
	//ʵ����Բ�𽥱��
    u8g2_ClearBuffer(u8g2);
	for(int i=2; i<32; i+=3)
	{
		u8g2_DrawFilledEllipse(u8g2,64,32, i*2, i, U8G2_DRAW_ALL);
		u8g2_SendBuffer(u8g2);
	}
    //������Բ�𽥱�С
    u8g2_ClearBuffer(u8g2);
	for(int i=32; i>0; i-=3)
	{
		u8g2_DrawEllipse(u8g2,64,32, i*2, i, U8G2_DRAW_ALL);
		u8g2_SendBuffer(u8g2);
	}
}

void u8g2DRAW_UI(u8g2_t *u8g2)
{
	testShowFont(u8g2);
	vTaskDelay(1000);
}
