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


#define SHORT_KEY_DELAY     2		// �̰���ʱ
#define LONG_KEY_DELAY      90		// ������ʱ
#define DOUBLE_KEY_DELAY  	30		// ˫�����δ���֮��������ʱ
#define CNTINUS_KEY_DELAY  	20		// ����������ʱ

#define     HW_KEYS_NUM   5 // ֧�ְ�������(������������+��ϰ���)

#define     NOKEY_INPUT_VALUE   0x00 // û�а�������ʱget_key_input()�ķ���ֵ
#define     KEY1_INPUT_VALUE    0x01
#define     KEY2_INPUT_VALUE    0x02
#define     KEY3_INPUT_VALUE    0x04
#define     KEY4_INPUT_VALUE    0x05

#define     KEY1_2_INPUT_VALUE  (KEY1_INPUT_VALUE | KEY2_INPUT_VALUE)  // KEY1+KE2��ϼ�

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

// ����ɨ��ṹ��
typedef struct {
    uint8_t scan_flag;      // ����ɨ���־,ÿ10ms����ʱ�ж���1,��key_scan������ɨ�谴��֮������
    uint16_t cnt;           // �������¼�����,��λ:ɨ������(10ms);����ֵ����ʱ��1,��������ʱ����
    uint16_t last_input;    // �ϴ�����
    uint8_t id;             // ��ǰ��Ч������key_info_t��Ϣ���е�ID
    uint8_t pressed;        // ����������Ч��־λ,��0��ʾ��������ʱ���Ѿ��ﵽ�̰���ʱ
    uint8_t wait_double_flag;// ������һ�������ڵȴ�˫��ʱ�ñ�־=1,û�а����ڵȴ�˫��ʱ=0

    uint8_t wait_double[HW_KEYS_NUM];    // �ȴ�˫����־λ.��ĳ����֧��˫��,���һ�ε�����,���ñ�־��1.
    uint16_t double_timeout[HW_KEYS_NUM];// �ȴ�˫����ʱ,��ĳ����֧��˫��,���һ�ε�����,����ֵ��ΪDOUBLE_KEY_DELAY.
                            // ÿ��ɨ���Լ�1,����0��ʱ��Ϊ��ʱ,����ʱ��δ�ȵ��ڶ��ΰ���,�򷵻ض̰�ֵ.
                            // ���ض̰�ֵ��˫����ֵʱ����
}key_scan_t;

// ������Ϣ�ṹ��
typedef struct {
    uint16_t hw_input_val;      // ����������ʱ��get_key_input()�ķ���ֵ

    uint8_t short_key_val;      // �̰���Чʱ,���صļ�ֵ,��֧����дKEY_NONE
    uint8_t long_key_val;       // ������Чʱ,���صļ�ֵ,��֧����дKEY_NONE
    uint8_t cntinus_key_val;    // ��������Чʱ,���صļ�ֵ,��֧����дKEY_NONE
    uint8_t double_key_val;     // ˫����Чʱ,���صļ�ֵ,��֧����дKEY_NONE

    uint16_t long_cnt;          // ������Чʱ���ж�����,��λ:ɨ������(10ms)
}key_info_t;
extern key_scan_t key;

#define KEY_S1   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define KEY_S2   GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)
#define KEY_S3   GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)
#define KEY_S4   GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)

void key_init(void); //������ʼ��
u8 key4x4_scanf(void);//���󰴼�
char kbd_read(void);
//unsigned int key_scan(unsigned int mode); //�����������жϼ��Ű���
void gpio4x4_init(void);
void KEY_EXTIInit(void);  //�����ж�
uint8_t key_scan_multi_function(void);
unsigned int key_scan(unsigned int mode);
#endif
