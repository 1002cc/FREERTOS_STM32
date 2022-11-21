#include "adc.h"

static	ADC_InitTypeDef ADC_InitStructure;
static 	ADC_CommonInitTypeDef ADC_CommonInitStructure;
static 	GPIO_InitTypeDef GPIO_InitStructture;
static 	DMA_InitTypeDef DMA_InitStructure;
static 	EXTI_InitTypeDef EXTI_InitStruct;
static   NVIC_InitTypeDef NVIC_InitStruct;
//DMA外部传输


//光敏电阻引脚定义 PE7
//=====================================================================================
void lsens_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    GPIO_InitStructture.GPIO_Pin = GPIO_Pin_7; //设置引脚
    GPIO_InitStructture.GPIO_Mode = GPIO_Mode_AN;			
    GPIO_InitStructture.GPIO_OType = GPIO_OType_PP;				//推挽
    GPIO_InitStructture.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructture.GPIO_PuPd = GPIO_PuPd_NOPULL; 			//没有使能上拉/下拉电阻
    //初始化GPIOF
    GPIO_Init ( GPIOF, &GPIO_InitStructture );
}

#define ADC_CDR_ADDRESS ADC3->DR //取地址ADC3
u32 ADC_VAL;
#define DMA_STREAMx DMA2_Stream0
void adc_init(void)
{
	//使能ADC3时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	//初始化引脚 PE7
	lsens_init();
	
	//DMA配置 //数据流0 通道3
	DMA_InitStructure.DMA_Channel = DMA_Channel_2;   //确定通道 0-7
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC_CDR_ADDRESS;  //外设基地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_VAL; //内存的基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; //DMA传输数据方向 外设-->内存  内存-->外设  内存--->内存
	DMA_InitStructure.DMA_BufferSize = 1;
	//不需要地址递增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	//数据宽度指定
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	//循环模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; //关闭FILO缓存
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;  
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	DMA_Init(DMA_STREAMx, &DMA_InitStructure);
	DMA_Cmd(DMA_STREAMx, ENABLE);
	
	//ADC常规初始化
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;    				//独立工作模式 只使用一个ADC
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;					//ADC硬件时钟=84/2=42MHz
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;		//不需要映射数据,直接读取到寄存器
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//5*Tadc
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	
	// ADC3 Init
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; 						//12为精度
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;								//单通道模式 默认多个通道
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;							//不断的进行转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //不需要内部脉冲触发adc工作
	//ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;			//右对齐存储
	ADC_InitStructure.ADC_NbrOfConversion = 1;						//只有单个通道
	ADC_Init(ADC3, &ADC_InitStructure);
	//通道5和ADC关联起来,优先级为1，采样点的时间为5*Tadc
	ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);

	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);//设置ADC动作触发
	ADC_DMACmd(ADC3, ENABLE);
	
	ADC_Cmd(ADC3, ENABLE);
}
//=====================================================================================

//烟雾报警器 A0(ADC采集)PB0 ADC1 IN8  DO(PB1)中断3 4  流2 通道1
//=====================================================================================
void gpio_mq_init(void)
{	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStructture.GPIO_Pin = GPIO_Pin_0; //设置引脚
    GPIO_InitStructture.GPIO_Mode = GPIO_Mode_AN;			
    GPIO_InitStructture.GPIO_OType = GPIO_OType_PP;				//推挽
    GPIO_InitStructture.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructture.GPIO_PuPd = GPIO_PuPd_NOPULL; 			//没有使能上拉/下拉电阻

    GPIO_Init ( GPIOB, &GPIO_InitStructture );	
	
	GPIO_InitStructture.GPIO_Pin = GPIO_Pin_1; 
	GPIO_InitStructture.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init ( GPIOB, &GPIO_InitStructture );
}

void mq_2_init(void)
{	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);
	gpio_mq_init();
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;    				//独立工作模式 只使用一个ADC
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;					//ADC硬件时钟=84/2=42MHz
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;		//不需要映射数据,直接读取到寄存器
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//5*Tadc
	ADC_CommonInit(&ADC_CommonInitStructure);

	// ADC2Init
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; 						//12为精度
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;								//单通道模式 默认多个通道
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;							//不断的进行转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //不需要内部脉冲触发adc工作
	//ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;			//右对齐存储
	ADC_InitStructure.ADC_NbrOfConversion = 1;						//只有单个通道
	ADC_Init(ADC2, &ADC_InitStructure);
	//通道5和ADC关联起来,优先级为1，采样点的时间为5*Tadc
	ADC_RegularChannelConfig(ADC2, ADC_Channel_8, 1, ADC_SampleTime_3Cycles);

	ADC_Cmd(ADC2, ENABLE);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource4);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line4; //选择指定的中断线 0~22
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt; //模式 事件模式/中断模式 默认中断模式
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; //触发方式 下降沿触发 上升沿触发 边沿触发
    EXTI_InitStruct.EXTI_LineCmd = ENABLE; //使能  ENABLE/DISENABLE
    EXTI_Init(&EXTI_InitStruct);

    //设置优先级
    NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);	
}

//火焰传感器 	PA6 
//=====================================================================================
void flame_transducer_init(void)
{
		/*火焰传感器*/
	
	//端口A的硬件时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  
	
	//ADC1的硬件时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	//配置PA2引脚为AN（模拟信号）模式
	GPIO_InitStructture.GPIO_Pin   = GPIO_Pin_6; 	//2号引脚
	GPIO_InitStructture.GPIO_Mode  = GPIO_Mode_AN;//模拟模式
	GPIO_InitStructture.GPIO_PuPd  = GPIO_PuPd_NOPULL;//不使能上下拉电阻
	GPIO_Init(GPIOA,&GPIO_InitStructture);	


	//配置ADC1相关参数
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;	//单个ADC工作
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;	//ADC1的硬件时钟频率=84MHz/2=42MHz
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//DMA关闭
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//5*(1/42MHz)
	ADC_CommonInit(&ADC_CommonInitStructure);	
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;		//转换结果为12位的数据
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;				//单通道扫描
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;			//连续工作
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//不需要脉冲触发ADC工作
	//ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;		//转换的结果右对齐存储
	ADC_InitStructure.ADC_NbrOfConversion = 1;					//告诉ADC要扫描的通道数目，当前是1个
	ADC_Init(ADC1, &ADC_InitStructure);
	
	
	//PA2引脚连接到ADC1，通道6序号为1，对采样点的采样时间为3个ADC时钟周期，时间越长越准确
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_3Cycles);
	
	//使能ADC1工作
	ADC_Cmd(ADC1, ENABLE);

}

 void exit9_init(void)
{
	
	//打开端口B的硬件时钟，就是供电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	//打开SYSCFG硬件时钟，就是供电
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	GPIO_InitStructture.GPIO_Pin   = GPIO_Pin_6;	//9号引脚
	GPIO_InitStructture.GPIO_Mode  = GPIO_Mode_IN;  //输入模式
	GPIO_InitStructture.GPIO_Speed = GPIO_High_Speed;//高速，速度越高，响应越快，但是功耗会更高
	GPIO_InitStructture.GPIO_PuPd  = GPIO_PuPd_NOPULL;//不使能上下拉电阻(因为外部已经有上拉电阻)
	GPIO_Init(GPIOE,&GPIO_InitStructture);	
	
	//将PB9连接到EXTI3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource9);

	EXTI_InitStruct.EXTI_Line = EXTI_Line9;	//外部中断9
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt; //中断触发
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  //上升沿用于检测火焰的存在。下降沿用于检测火焰的消失。
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;			//使能工作
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;			//外部中断9-5中断号
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 5;//中断抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x05;		//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;				//打开外部中断0请求通道
	NVIC_Init(&NVIC_InitStruct);

}