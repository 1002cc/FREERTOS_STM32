#include "iwdg.h"

void iwdg_init(void)
{
	//���з��ʶ������Ź�
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	//���ÿ��Ź�����ֵΪ256   �������Ź���Ӳ��ʱ��=32khz/256=125hz
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	
	/*
	���ö������Ź�����ֵΪ124
	������Χ��0-124��Ϊ1����
	*/
	IWDG_SetReload(125-1);
	
	/*���ؼ���ֵ*/
	IWDG_ReloadCounter();//ι��
	
	IWDG_Enable();

}
