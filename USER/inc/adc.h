#ifndef __ADC_H
#define __ADC_H

#include "stm32f4xx.h"
#include "stdio.h"
#include "delay.h"

void adc_init(void);
void lsens_read_data(uint16_t adc_val,uint16_t adc_vol);
void gpio_mq_init(void);
void mq_2_init(void);
void mq2_read_data(uint16_t mq2_val,uint16_t mq2_vol);
void flame_transducer_init(void);
extern u32 ADC_VAL;
extern u32 MQ_2_VAL;
#endif
