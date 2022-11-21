#ifndef __KEY_H
#define __KEY_H

#include "stm32f4xx.h"                  // Device header
#include "delay.h"
#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tim.h"
#include "stdio.h"

#define KEY_ON  1
#define KEY_OFF 0

#define KEY1_PRES 	1
#define KEY2_PRES	2
#define KEY3_PRES	3
#define KEY4_PRES   4


#define SHORT_KEY_DELAY     2		// 短按延时
#define LONG_KEY_DELAY      90		// 长按延时
#define DOUBLE_KEY_DELAY  	30		// 双击两次触发之间的最大延时
#define CNTINUS_KEY_DELAY  	20		// 连续触发延时

#define     HW_KEYS_NUM   5 // 支持按键数量(包括独立按键+组合按键)

#define     NOKEY_INPUT_VALUE   0x00 // 没有按键按下时get_key_input()的返回值
#define     KEY1_INPUT_VALUE    0x01
#define     KEY2_INPUT_VALUE    0x02
#define     KEY3_INPUT_VALUE    0x04
#define     KEY4_INPUT_VALUE    0x05

#define     KEY1_2_INPUT_VALUE  (KEY1_INPUT_VALUE | KEY2_INPUT_VALUE)  // KEY1+KE2组合键

typedef enum {
    KEY_NONE = 0,
	
    KEY1_SHORT,
    KEY1_LONG,
    KEY1_CNTINUS,
    KEY1_DOUBLE,
	
    KEY2_SHORT,
    KEY2_LONG,
    KEY2_CNTINUS,
    KEY2_DOUBLE,
	
    KEY3_SHORT,
    KEY3_LONG,
    KEY3_CNTINUS,
    KEY3_DOUBLE,
	
	KEY4_SHORT,
    KEY4_LONG,
    KEY4_CNTINUS,
    KEY4_DOUBLE,
	
    KEY1_2_SHORT,
    KEY1_2_LONG,
    KEY1_2_CNTINUS,
    KEY1_2_DOUBLE,
}key_val_t;

// 按键扫描结构体
typedef struct {
    uint8_t scan_flag;      // 按键扫描标志,每10ms被定时中断置1,在key_scan函数中扫描按键之后清零
    uint16_t cnt;           // 按键按下计数器,单位:扫描周期(10ms);当键值按下时加1,按键弹起时清零
    uint16_t last_input;    // 上次输入
    uint8_t id;             // 当前有效按键在key_info_t信息表中的ID
    uint8_t pressed;        // 按键按下生效标志位,非0表示持续按下时长已经达到短按延时
    uint8_t wait_double_flag;// 有任意一个按键在等待双击时该标志=1,没有按键在等待双击时=0

    uint8_t wait_double[HW_KEYS_NUM];    // 等待双击标志位.若某按键支持双击,则第一次单击后,将该标志置1.
    uint16_t double_timeout[HW_KEYS_NUM];// 等待双击超时,若某按键支持双击,则第一次单击后,将该值设为DOUBLE_KEY_DELAY.
                            // 每次扫描自减1,减到0的时刻为超时,若此时仍未等到第二次按下,则返回短按值.
                            // 返回短按值或双击键值时清零
}key_scan_t;

// 按键信息结构体
typedef struct {
    uint16_t hw_input_val;      // 当按键按下时，get_key_input()的返回值

    uint8_t short_key_val;      // 短按有效时,返回的键值,不支持则写KEY_NONE
    uint8_t long_key_val;       // 长按有效时,返回的键值,不支持则写KEY_NONE
    uint8_t cntinus_key_val;    // 连续按有效时,返回的键值,不支持则写KEY_NONE
    uint8_t double_key_val;     // 双击有效时,返回的键值,不支持则写KEY_NONE

    uint16_t long_cnt;          // 长按有效时间判定长度,单位:扫描周期(10ms)
}key_info_t;
extern key_scan_t key;

#define KEY_S1   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define KEY_S2   GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)
#define KEY_S3   GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)
#define KEY_S4   GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)

void key_init(void); //按键初始化
u8 key4x4_scanf(void);//矩阵按键
char kbd_read(void);
//unsigned int key_scan(unsigned int mode); //按键消抖加判断几号按键
void gpio4x4_init(void);
void KEY_EXTIInit(void);  //按键中断
uint8_t key_scan_multi_function(void);
unsigned int key_scan(unsigned int mode);
#endif
