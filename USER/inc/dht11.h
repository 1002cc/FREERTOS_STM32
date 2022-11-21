#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f4xx.h"
#include "stdio.h"
#include "delay.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sys.h"
extern float g_temp;
extern float g_humi;
extern float g_length;
extern u32   g_ligth;
extern u32   g_mq;
extern u32   g_tf;
void dht11_1init(void);
int32_t dht11_read(uint8_t *pdht_data);
#endif
