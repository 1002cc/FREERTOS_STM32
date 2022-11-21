#ifndef __OLED_U8G2_H
#define __OLED_U8G2_H
#include "stdio.h"
#include "sys.h"
#include "stdlib.h"	
#include "delay.h"
#include "u8g2.h"


#define SEND_BUFFER_DISPLAY_MS(u8g2, ms)\
do {\
u8g2_SendBuffer(u8g2); \
delay_ms(ms);\
}while(0);
  



#define SCL_Pin GPIO_Pin_8
#define SDA_Pin GPIO_Pin_9
#define IIC_GPIO_Port GPIOB
	
void IIC_U8G2_Init(void);	
void u8g2Init(u8g2_t *u8g2);
void draw(u8g2_t *u8g2);
uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

void u8g2DrawTest(u8g2_t *u8g2);
void testShowFont(u8g2_t *u8g2);
void testDrawFrame(u8g2_t *u8g2);
void testDrawRBox(u8g2_t *u8g2);
void testDrawCircle(u8g2_t *u8g2);
void testDrawFilledEllipse(u8g2_t *u8g2);
void testDrawMulti(u8g2_t *u8g2);
void testDrawProcess(u8g2_t *u8g2);
void testDrawXBM(u8g2_t *u8g2);
void u8g2DRAW_UI(u8g2_t *u8g2);
#endif  
	 



