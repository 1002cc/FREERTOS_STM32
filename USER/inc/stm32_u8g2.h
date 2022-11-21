#ifndef __STM32_U8G2_H
#define __STM32_U8G2_H
 
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "u8g2.h"
#include "stdio.h"
void IIC_Init(void);
uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
void u8g2Init(u8g2_t *u8g2);
void draw(u8g2_t *u8g2);
#endif
