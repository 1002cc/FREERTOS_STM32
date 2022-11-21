#include "key.h"

key_scan_t key = {0};

key_info_t keys_info[HW_KEYS_NUM] = {
   {KEY1_INPUT_VALUE,  KEY1_SHORT,  KEY1_LONG,  KEY1_CNTINUS,  KEY1_DOUBLE,  200},
   {KEY2_INPUT_VALUE,  KEY2_SHORT,  KEY2_LONG,  KEY_NONE,      KEY2_DOUBLE,  250},
   {KEY3_INPUT_VALUE,  KEY3_SHORT,  KEY3_LONG,  KEY_NONE,  	   KEY_NONE,     300},
   {KEY4_INPUT_VALUE,  KEY4_SHORT,  KEY4_LONG,  KEY_NONE, 	   KEY_NONE,     300},
   {KEY1_2_INPUT_VALUE,KEY1_2_SHORT,KEY_NONE,	KEY_NONE,      KEY_NONE,     300},
};


//������ʼ��
void key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructrue;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);
    //���尴�� PA0
    GPIO_InitStructrue.GPIO_Pin = GPIO_Pin_0; 			//��������
    GPIO_InitStructrue.GPIO_Mode = GPIO_Mode_IN;		//��Ӧģʽ
    GPIO_InitStructrue.GPIO_Speed=GPIO_Low_Speed; 
	GPIO_InitStructrue.GPIO_PuPd = GPIO_PuPd_NOPULL; 	//û��ʹ������/��������
    GPIO_Init( GPIOA, &GPIO_InitStructrue );
    //������������PE2 PE3 PE4
    GPIO_InitStructrue.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_Init( GPIOE, &GPIO_InitStructrue );				 
}

static uint16_t get_key_input(void)
{
    uint16_t key_val = NOKEY_INPUT_VALUE;
    //uint16_t input;

    //input = GPIOE->IDR; // ��ȡԭʼ����
    if(RESET==KEY_S1) 
	{ 
        key_val |= KEY1_INPUT_VALUE;
    }
    if(RESET==KEY_S2) 
	{ 
        key_val |= KEY2_INPUT_VALUE;
    }
    if(RESET==KEY_S3) 
	{ 
        key_val |= KEY3_INPUT_VALUE;
    }
	if(RESET==KEY_S4) 
	{ 
        key_val |= KEY4_INPUT_VALUE;
    }
    
    return key_val; 
}

static uint8_t get_key_id(uint16_t new_input)
{
    uint8_t key_id = HW_KEYS_NUM;
	uint8_t i = 0;
	
    if (0 == key.cnt)
	{	// ��һ���ж�
        key.last_input = new_input;
    } else if (key.last_input != new_input) 
	{ 
        // Ϊ��֧����ϼ�:�ǵ�һ���ж�ʱ,������뷢���仯,�����㲢��last_input���¸�ֵ.
        key.last_input = new_input;
        key.cnt = 0;
    }
    if (key.cnt < SHORT_KEY_DELAY) 
	{ // �������¼�ʱ����++	
        key.cnt++;
    }
    if (SHORT_KEY_DELAY == key.cnt)
	{
        key.pressed = 1;	// �̰���Ч��־λ
        for (i=0; i<HW_KEYS_NUM; i++) 
		{ // ͨ���Ա�key_info_t��Ϣ��,��ȡ��ǰ����ID
            if (keys_info[i].hw_input_val == new_input) 
			{
                key_id = i;
				// XXX ������: ������Чʱ���,���ڹ۲�̰�,����,��������ʱ�䳤��.
				//printf("KEY Press.\r\n"); 
                break;
            }
        }
    }

    return key_id;
}

static uint8_t key_pressed_handle(uint16_t new_input)
{
    uint8_t res = KEY_NONE;

    if (key.id < HW_KEYS_NUM) 
	{
        key.cnt++;
        if (new_input == key.last_input) 
		{
            if (SHORT_KEY_DELAY == key.cnt) 
			{
                key.pressed = 1; // ��ǰ���������Ч
            } 
			else if (keys_info[key.id].long_cnt == key.cnt) 
			{ // �����ﵽ2����
                res = keys_info[key.id].long_key_val; // ������ֵ
            } 
			else if ((keys_info[key.id].long_cnt + CNTINUS_KEY_DELAY) == key.cnt) 
			{ 
                // ����2��֮��,ÿ����0.2�뷵��һ�μ�ֵ
                key.cnt = keys_info[key.id].long_cnt;
                res = keys_info[key.id].cntinus_key_val; // һֱ��,���������ļ�ֵ
            }
        }
    }
    return res;
}

static uint8_t key_release_handle(void)
{
    uint8_t res = KEY_NONE;

    if (key.pressed) 
	{
        if (key.id < HW_KEYS_NUM) 
		{ 
            if (key.cnt < keys_info[key.id].long_cnt) 
			{ // ���µ�ʱ����С�ڳ����ж�ʱ��
                res = keys_info[key.id].short_key_val; // �̰���ֵ.
                // �����ǰ����֧��˫��
                if (KEY_NONE != keys_info[key.id].double_key_val) 
				{ 
                    if (key.wait_double[key.id]) 
					{
                        key.wait_double[key.id] = 0; // ����ȴ�˫����־
                        key.double_timeout[key.id] = 0;
                        res = keys_info[key.id].double_key_val; // ˫����ֵ
                    } 
					else 
					{
                        key.wait_double[key.id] = 1; // ���õȴ�˫����־
                        key.double_timeout[key.id] = DOUBLE_KEY_DELAY; // ���ó�ʱʱ��
                        key.wait_double_flag = 1;
                        res = KEY_NONE;
                    }
                }
            }
        }
    }
    key.cnt = 0;
    key.pressed = 0;
    key.last_input = NOKEY_INPUT_VALUE;

    return res;
}

static uint8_t key_wait_double_timeout_handle(void)
{
    uint8_t res = KEY_NONE;
    uint8_t i;

    key.wait_double_flag = 0;
    for (i = 0; i < HW_KEYS_NUM; i++) 
	{
        if (key.double_timeout[i]) 
		{	// ����������ڵȴ�˫��
            key.double_timeout[i]--;
            key.wait_double_flag = 1;
            if (0 == key.double_timeout[i]) 
			{ // ����0��ʱ��,��ʾ�ȴ���ʱ��
                key.wait_double[i] = 0; // ����ȴ�˫����־
                return (keys_info[i].short_key_val); // ���ظü��Ķ̰�ֵ
            }
        }
    }
    return res;
}

// ����ɨ�躯��,��ÿ10ms����һ��
uint8_t key_scan_multi_function(void)
{
    uint8_t res = KEY_NONE;
    uint16_t key_input;

	key.scan_flag = 0;
    key_input = get_key_input();
    if (NOKEY_INPUT_VALUE != key_input) 
	{
        if (0 == key.pressed) 
		{
            // �ϴ�״̬Ϊ"�ǰ���",���ȡ��ǰ����ID
            key.id = get_key_id(key_input);
		} 
		else 
		{
            res = key_pressed_handle(key_input);
        }
    } 
	else 
	{
        res = key_release_handle();
    }
    if (KEY_NONE == res) 
	{ // ǰ��Ĵ���û�в�����Ч��ֵ,�ٴ���˫����ʱ
        if (key.wait_double_flag) 
		{
            res = key_wait_double_timeout_handle();
        } 
    }
    return res;
}

//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
	static uint32_t t = 0;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
		key.scan_flag = 1;
		t++;
		if (1000 == t) 
		{
			t = 0;
		}
	}
}


























unsigned int key_scan(unsigned int mode)
{
    static unsigned int key_up = 1; //�������ɿ���־
    if(mode)key_up = 1;
    if(key_up && (KEY_S1 == 0x00 || KEY_S2 == 0x00 || KEY_S3 == 0x00 || KEY_S4 == 0x00))
    {
        vTaskDelay(40);
        key_up = 0;
        if(KEY_S1 == 0x00)	   return KEY1_SHORT;
        else if(KEY_S2 == 0x00)return KEY2_SHORT;
        else if(KEY_S3 == 0x00)return KEY3_SHORT;
        else if(KEY_S4 == 0x00)return KEY4_SHORT;
    } else if(KEY_S1 == 0x00 || KEY_S2 == 0x00 || KEY_S3 == 0x00 || KEY_S4 == 0x00)key_up = 1;
    return 0;
}







//���󰴼���ʼ��
void gpio4x4_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructrue;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOD, ENABLE);
	//OUT: C1(PE11) C2(PE12) C3(PE15) C4(PD9) 
	//IN:  R4(PE9) R3(PE10) R2(PD0) R1(PD14)
	GPIO_InitStructrue.GPIO_Mode=GPIO_Mode_OUT;        //���ģʽ
	GPIO_InitStructrue.GPIO_OType=GPIO_OType_PP;       
	GPIO_InitStructrue.GPIO_Pin= GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15; 
	GPIO_InitStructrue.GPIO_Speed=GPIO_High_Speed;     
	GPIO_InitStructrue.GPIO_PuPd  = GPIO_PuPd_NOPULL;  
	GPIO_Init(GPIOE,&GPIO_InitStructrue);             
	
	GPIO_InitStructrue.GPIO_Pin=GPIO_Pin_9; 			
    GPIO_Init(GPIOD,&GPIO_InitStructrue);              
    
	
	GPIO_InitStructrue.GPIO_PuPd  = GPIO_PuPd_UP;	   
    GPIO_InitStructrue.GPIO_Mode=GPIO_Mode_IN;       
    GPIO_InitStructrue.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10;               
    GPIO_Init(GPIOE,&GPIO_InitStructrue);              				 

    GPIO_InitStructrue.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_14;              
    GPIO_Init(GPIOD,&GPIO_InitStructrue);  
}

//���󰴼�
u8 key4x4_scanf(void)
{
	while(1)
	{
		PDout(9) = 0;
		PEout(15) = 1;
		PEout(12) = 1;
		PEout(11) = 1;
		vTaskDelay(40);
		
		if( PEin(9) == 0 ) return 'A';
		else if( PEin(10) == 0 ) return 'B';
		else if( PDin(0) == 0 ) return 'C';
		else if( PDin(14) == 0 ) return 'D';
		
		PDout(9) = 1;
		PEout(15) = 0; 
		PEout(12) = 1; 
		PEout(11) = 1;
		vTaskDelay(40);


		if( PEin(9) == 0 ) return '3'; 
		else if( PEin(10) == 0 ) return '6';
		else if( PDin(0) == 0 ) return '9'; 
		else if( PDin(14) == 0 ) return '#';
		
		PDout(9) = 1;
		PEout(15) = 1; 
		PEout(12) = 0; 
		PEout(11) = 1;
		vTaskDelay(40);

		if( PEin(9) == 0 ) return '2'; 
		else if( PEin(10) == 0 ) return '5'; 
		else if( PDin(0) == 0 ) return '8'; 
		else if( PDin(14) == 0 ) return '0';
		
		PDout(9) = 1;
		PEout(15) = 1; 
		PEout(12) = 1; 
		PEout(11) = 0;
		vTaskDelay(40);

		if( PEin(9) == 0 ) return '1'; 
		else if( PEin(10) == 0 ) return '4'; 
		else if( PDin(0) == 0 ) return '7'; 
		else if( PDin(14) == 0 ) return '*';
		vTaskDelay(40);
	
	}
}



void KEY_EXTIInit(void)
{
    //PA0 PE2 PE3 PE4
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    //���ų�ʼ�� PA0 PE2 PE3 PE4
    key_init();
	gpio4x4_init();
    //�������ȼ�
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    //�жϼ����·����
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);


    //��ʼ���ж� PA0
    EXTI_InitStruct.EXTI_Line = EXTI_Line0 | EXTI_Line2 | EXTI_Line3 | EXTI_Line4; //ѡ��ָ�����ж��� 0~22
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt; //ģʽ �¼�ģʽ/�ж�ģʽ Ĭ���ж�ģʽ
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; //������ʽ �½��ش��� �����ش��� ���ش���
    EXTI_InitStruct.EXTI_LineCmd = ENABLE; //ʹ��  ENABLE/DISENABLE
    EXTI_Init(&EXTI_InitStruct);

    //�������ȼ�
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 4 ;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

