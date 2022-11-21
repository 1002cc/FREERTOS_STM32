#ifndef __IR_H
#define __IR_H

#include "stm32f4xx.h"
void ir_init(void);
int read_ir_data(unsigned char *buff);
#endif