#include "key.h"

key_scan_t key = {0};

key_info_t keys_info[HW_KEYS_NUM] = {
   {KEY1_INPUT_VALUE,  KEY1_SHORT,  KEY1_LONG,  KEY1_CNTINUS,  KEY1_DOUBLE,  200},
   {KEY2_INPUT_VALUE,  KEY2_SHORT,  KEY2_LONG,  KEY_NONE,      KEY2_DOUBLE,  250},
   {KEY3_INPUT_VALUE,  KEY3_SHORT,  KEY3_LONG,  KEY_NONE,  	   KEY_NONE,     300},
   {KEY4_INPUT_VALUE,  KEY4_SHORT,  KEY4_LONG,  KEY_NONE, 	   KEY_NONE,     300},
   {KEY1_2_INPUT_VALUE,KEY1_2_SHORT,KEY_NONE,	KEY_NONE,      KEY_NONE,     300},
};


//按键初始化
void key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructrue;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);
    //定义按键 PA0
    GPIO_InitStructrue.GPIO_Pin = GPIO_Pin_0; 			//设置引脚
    GPIO_InitStructrue.GPIO_Mode = GPIO_Mode_IN;		//响应模式
    GPIO_InitStructrue.GPIO_Speed=GPIO_Low_Speed; 
	GPIO_InitStructrue.GPIO_PuPd = GPIO_PuPd_NOPULL; 	//没有使能上拉/下拉电阻
    GPIO_Init( GPIOA, &GPIO_InitStructrue );
    //定义其他按键PE2 PE3 PE4
    GPIO_InitStructrue.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_Init( GPIOE, &GPIO_InitStructrue );				 
}

static uint16_t get_key_input(void)
{
    uint16_t key_val = NOKEY_INPUT_VALUE;
    //uint16_t input;

    //input = GPIOE->IDR; // 读取原始输入
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
	{	// 第一次判断
        key.last_input = new_input;
    } else if (key.last_input != new_input) 
	{ 
        // 为了支持组合键:非第一次判断时,如果输入发生变化,则清零并对last_input重新赋值.
        key.last_input = new_input;
        key.cnt = 0;
    }
    if (key.cnt < SHORT_KEY_DELAY) 
	{ // 按键按下计时变量++	
        key.cnt++;
    }
    if (SHORT_KEY_DELAY == key.cnt)
	{
        key.pressed = 1;	// 短按有效标志位
        for (i=0; i<HW_KEYS_NUM; i++) 
		{ // 通过对比key_info_t信息表,获取当前按键ID
            if (keys_info[i].hw_input_val == new_input) 
			{
                key_id = i;
				// XXX 调试用: 按键有效时输出,用于观察短按,长按,超长按的时间长度.
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
                key.pressed = 1; // 标记按键按下生效
            } 
			else if (keys_info[key.id].long_cnt == key.cnt) 
			{ // 长按达到2秒钟
                res = keys_info[key.id].long_key_val; // 长按键值
            } 
			else if ((keys_info[key.id].long_cnt + CNTINUS_KEY_DELAY) == key.cnt) 
			{ 
                // 长按2秒之后,每持续0.2秒返回一次键值
                key.cnt = keys_info[key.id].long_cnt;
                res = keys_info[key.id].cntinus_key_val; // 一直按,连续触发的键值
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
			{ // 按下的时长，小于长按判定时间
                res = keys_info[key.id].short_key_val; // 短按键值.
                // 如果当前按键支持双击
                if (KEY_NONE != keys_info[key.id].double_key_val) 
				{ 
                    if (key.wait_double[key.id]) 
					{
                        key.wait_double[key.id] = 0; // 清除等待双击标志
                        key.double_timeout[key.id] = 0;
                        res = keys_info[key.id].double_key_val; // 双击键值
                    } 
					else 
					{
                        key.wait_double[key.id] = 1; // 设置等待双击标志
                        key.double_timeout[key.id] = DOUBLE_KEY_DELAY; // 设置超时时间
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
		{	// 如果按键正在等待双击
            key.double_timeout[i]--;
            key.wait_double_flag = 1;
            if (0 == key.double_timeout[i]) 
			{ // 减到0的时刻,表示等待超时了
                key.wait_double[i] = 0; // 清除等待双击标志
                return (keys_info[i].short_key_val); // 返回该键的短按值
            }
        }
    }
    return res;
}

// 按键扫描函数,需每10ms调用一次
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
            // 上次状态为"非按下",则获取当前按键ID
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
	{ // 前面的处理没有产生有效键值,再处理双击超时
        if (key.wait_double_flag) 
		{
            res = key_wait_double_timeout_handle();
        } 
    }
    return res;
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	static uint32_t t = 0;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
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
    static unsigned int key_up = 1; //按键按松开标志
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







//矩阵按键初始化
void gpio4x4_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructrue;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOD, ENABLE);
	//OUT: C1(PE11) C2(PE12) C3(PE15) C4(PD9) 
	//IN:  R4(PE9) R3(PE10) R2(PD0) R1(PD14)
	GPIO_InitStructrue.GPIO_Mode=GPIO_Mode_OUT;        //输出模式
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

//矩阵按键
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

    //引脚初始化 PA0 PE2 PE3 PE4
    key_init();
	gpio4x4_init();
    //分组优先级
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    //中断检测线路连接
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);


    //初始化中断 PA0
    EXTI_InitStruct.EXTI_Line = EXTI_Line0 | EXTI_Line2 | EXTI_Line3 | EXTI_Line4; //选择指定的中断线 0~22
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt; //模式 事件模式/中断模式 默认中断模式
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; //触发方式 下降沿触发 上升沿触发 边沿触发
    EXTI_InitStruct.EXTI_LineCmd = ENABLE; //使能  ENABLE/DISENABLE
    EXTI_Init(&EXTI_InitStruct);

    //设置优先级
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

