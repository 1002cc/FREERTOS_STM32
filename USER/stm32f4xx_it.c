#include "stm32f4xx_it.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

//�жϾ��
//===================================================================
extern QueueHandle_t KEY_ONE_Queue;
//�����ж�
//================================================================
//����1
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

////����2
//void EXTI2_IRQHandler(void)
//{
//    vTaskDelay(40);

//    EXTI_ClearITPendingBit(EXTI_Line2);
//}

////����3
//void EXTI3_IRQHandler(void)
//{
//    vTaskDelay(40);

//    EXTI_ClearITPendingBit(EXTI_Line3);

//}
////����4
//void EXTI4_IRQHandler(void)
//{
//    vTaskDelay(40);
//		
//	
//    EXTI_ClearITPendingBit(EXTI_Line4);
//}
//================================================================




