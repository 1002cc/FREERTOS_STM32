#include "iwdg.h"

void iwdg_init(void)
{
	//运行访问独立看门狗
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	//配置看门狗分配值为256   独立看门狗的硬件时钟=32khz/256=125hz
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	
	/*
	设置独立看门狗计算值为124
	计数范围：0-124，为1秒钟
	*/
	IWDG_SetReload(125-1);
	
	/*重载计数值*/
	IWDG_ReloadCounter();//喂狗
	
	IWDG_Enable();

}
