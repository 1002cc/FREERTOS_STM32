#include "stm32f4xx_it.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

//中断句柄
//===================================================================
extern QueueHandle_t KEY_ONE_Queue;
//按键中断
//================================================================
//按键1
//void EXTI0_IRQHandler(void)
//{
//	unsigned int key_val=1;
//	taskENTER_CRITICAL(); 
//	BaseType_t xHigherPriorityTaskWoken;
//	vTaskDelay(40);
//	xQueueSendFromISR(KEY_ONE_Queue,&key_val, &xHigherPriorityTaskWoken);
//    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//	
//	EXTI_ClearITPendingBit(EXTI_Line0);
//}

////按键2
//void EXTI2_IRQHandler(void)
//{
//    vTaskDelay(40);

//    EXTI_ClearITPendingBit(EXTI_Line2);
//}

////按键3
//void EXTI3_IRQHandler(void)
//{
//    vTaskDelay(40);

//    EXTI_ClearITPendingBit(EXTI_Line3);

//}
////按键4
//void EXTI4_IRQHandler(void)
//{
//    vTaskDelay(40);
//		
//	
//    EXTI_ClearITPendingBit(EXTI_Line4);
//}
//================================================================




