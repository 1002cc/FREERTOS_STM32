#include "adc.h"

static	ADC_InitTypeDef ADC_InitStructure;
static 	ADC_CommonInitTypeDef ADC_CommonInitStructure;
static 	GPIO_InitTypeDef GPIO_InitStructture;
static 	DMA_InitTypeDef DMA_InitStructure;
static 	EXTI_InitTypeDef EXTI_InitStruct;
static   NVIC_InitTypeDef NVIC_InitStruct;
//DMA�ⲿ����


//�����������Ŷ��� PE7
//=====================================================================================
void lsens_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    GPIO_InitStructture.GPIO_Pin = GPIO_Pin_7; //��������
    GPIO_InitStructture.GPIO_Mode = GPIO_Mode_AN;			
    GPIO_InitStructture.GPIO_OType = GPIO_OType_PP;				//����
    GPIO_InitStructture.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructture.GPIO_PuPd = GPIO_PuPd_NOPULL; 			//û��ʹ������/��������
    //��ʼ��GPIOF
    GPIO_Init ( GPIOF, &GPIO_InitStructture );
}

#define ADC_CDR_ADDRESS ADC3->DR //ȡ��ַADC3
u32 ADC_VAL;
#define DMA_STREAMx DMA2_Stream0
void adc_init(void)
{
	//ʹ��ADC3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	//��ʼ������ PE7
	lsens_init();
	
	//DMA���� //������0 ͨ��3
	DMA_InitStructure.DMA_Channel = DMA_Channel_2;   //ȷ��ͨ�� 0-7
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC_CDR_ADDRESS;  //�������ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_VAL; //�ڴ�Ļ���ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; //DMA�������ݷ��� ����-->�ڴ�  �ڴ�-->����  �ڴ�--->�ڴ�
	DMA_InitStructure.DMA_BufferSize = 1;
	//����Ҫ��ַ����
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	//���ݿ��ָ��
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	//ѭ��ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //���ȼ�
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; //�ر�FILO����
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;  
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	DMA_Init(DMA_STREAMx, &DMA_InitStructure);
	DMA_Cmd(DMA_STREAMx, ENABLE);
	
	//ADC�����ʼ��
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;    				//��������ģʽ ֻʹ��һ��ADC
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;					//ADCӲ��ʱ��=84/2=42MHz
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;		//����Ҫӳ������,ֱ�Ӷ�ȡ���Ĵ���
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//5*Tadc
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	
	// ADC3 Init
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; 						//12Ϊ����
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;								//��ͨ��ģʽ Ĭ�϶��ͨ��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;							//���ϵĽ���ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //����Ҫ�ڲ����崥��adc����
	//ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;			//�Ҷ���洢
	ADC_InitStructure.ADC_NbrOfConversion = 1;						//ֻ�е���ͨ��
	ADC_Init(ADC3, &ADC_InitStructure);
	//ͨ��5��ADC��������,���ȼ�Ϊ1���������ʱ��Ϊ5*Tadc
	ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);

	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);//����ADC��������
	ADC_DMACmd(ADC3, ENABLE);
	
	ADC_Cmd(ADC3, ENABLE);
}
//=====================================================================================

//�������� A0(ADC�ɼ�)PB0 ADC1 IN8  DO(PB1)�ж�3 4  ��2 ͨ��1
//=====================================================================================
void gpio_mq_init(void)
{	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStructture.GPIO_Pin = GPIO_Pin_0; //��������
    GPIO_InitStructture.GPIO_Mode = GPIO_Mode_AN;			
    GPIO_InitStructture.GPIO_OType = GPIO_OType_PP;				//����
    GPIO_InitStructture.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructture.GPIO_PuPd = GPIO_PuPd_NOPULL; 			//û��ʹ������/��������

    GPIO_Init ( GPIOB, &GPIO_InitStructture );	
	
	GPIO_InitStructture.GPIO_Pin = GPIO_Pin_1; 
	GPIO_InitStructture.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init ( GPIOB, &GPIO_InitStructture );
}

void mq_2_init(void)
{	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);
	gpio_mq_init();
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;    				//��������ģʽ ֻʹ��һ��ADC
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;					//ADCӲ��ʱ��=84/2=42MHz
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;		//����Ҫӳ������,ֱ�Ӷ�ȡ���Ĵ���
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//5*Tadc
	ADC_CommonInit(&ADC_CommonInitStructure);

	// ADC2Init
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; 						//12Ϊ����
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;								//��ͨ��ģʽ Ĭ�϶��ͨ��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;							//���ϵĽ���ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //����Ҫ�ڲ����崥��adc����
	//ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;			//�Ҷ���洢
	ADC_InitStructure.ADC_NbrOfConversion = 1;						//ֻ�е���ͨ��
	ADC_Init(ADC2, &ADC_InitStructure);
	//ͨ��5��ADC��������,���ȼ�Ϊ1���������ʱ��Ϊ5*Tadc
	ADC_RegularChannelConfig(ADC2, ADC_Channel_8, 1, ADC_SampleTime_3Cycles);

	ADC_Cmd(ADC2, ENABLE);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource4);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line4; //ѡ��ָ�����ж��� 0~22
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt; //ģʽ �¼�ģʽ/�ж�ģʽ Ĭ���ж�ģʽ
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; //������ʽ �½��ش��� �����ش��� ���ش���
    EXTI_InitStruct.EXTI_LineCmd = ENABLE; //ʹ��  ENABLE/DISENABLE
    EXTI_Init(&EXTI_InitStruct);

    //�������ȼ�
    NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);	
}

//���洫���� 	PA6 
//=====================================================================================
void flame_transducer_init(void)
{
		/*���洫����*/
	
	//�˿�A��Ӳ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  
	
	//ADC1��Ӳ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	//����PA2����ΪAN��ģ���źţ�ģʽ
	GPIO_InitStructture.GPIO_Pin   = GPIO_Pin_6; 	//2������
	GPIO_InitStructture.GPIO_Mode  = GPIO_Mode_AN;//ģ��ģʽ
	GPIO_InitStructture.GPIO_PuPd  = GPIO_PuPd_NOPULL;//��ʹ������������
	GPIO_Init(GPIOA,&GPIO_InitStructture);	


	//����ADC1��ز���
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;	//����ADC����
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;	//ADC1��Ӳ��ʱ��Ƶ��=84MHz/2=42MHz
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//DMA�ر�
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//5*(1/42MHz)
	ADC_CommonInit(&ADC_CommonInitStructure);	
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;		//ת�����Ϊ12λ������
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;				//��ͨ��ɨ��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;			//��������
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//����Ҫ���崥��ADC����
	//ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;		//ת���Ľ���Ҷ���洢
	ADC_InitStructure.ADC_NbrOfConversion = 1;					//����ADCҪɨ���ͨ����Ŀ����ǰ��1��
	ADC_Init(ADC1, &ADC_InitStructure);
	
	
	//PA2�������ӵ�ADC1��ͨ��6���Ϊ1���Բ�����Ĳ���ʱ��Ϊ3��ADCʱ�����ڣ�ʱ��Խ��Խ׼ȷ
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_3Cycles);
	
	//ʹ��ADC1����
	ADC_Cmd(ADC1, ENABLE);

}

 void exit9_init(void)
{
	
	//�򿪶˿�B��Ӳ��ʱ�ӣ����ǹ���
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	//��SYSCFGӲ��ʱ�ӣ����ǹ���
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	GPIO_InitStructture.GPIO_Pin   = GPIO_Pin_6;	//9������
	GPIO_InitStructture.GPIO_Mode  = GPIO_Mode_IN;  //����ģʽ
	GPIO_InitStructture.GPIO_Speed = GPIO_High_Speed;//���٣��ٶ�Խ�ߣ���ӦԽ�죬���ǹ��Ļ����
	GPIO_InitStructture.GPIO_PuPd  = GPIO_PuPd_NOPULL;//��ʹ������������(��Ϊ�ⲿ�Ѿ�����������)
	GPIO_Init(GPIOE,&GPIO_InitStructture);	
	
	//��PB9���ӵ�EXTI3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource9);

	EXTI_InitStruct.EXTI_Line = EXTI_Line9;	//�ⲿ�ж�9
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt; //�жϴ���
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  //���������ڼ�����Ĵ��ڡ��½������ڼ��������ʧ��
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;			//ʹ�ܹ���
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;			//�ⲿ�ж�9-5�жϺ�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 5;//�ж���ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x05;		//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;				//���ⲿ�ж�0����ͨ��
	NVIC_Init(&NVIC_InitStruct);

}