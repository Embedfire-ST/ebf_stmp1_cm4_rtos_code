/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "./led/bsp_led.h"
#include "./key/bsp_key.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for Receive1Task */
osThreadId_t Receive1TaskHandle;
const osThreadAttr_t Receive1Task_attributes = {
  .name = "Receive1Task",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for Receive2Task */
osThreadId_t Receive2TaskHandle;
const osThreadAttr_t Receive2Task_attributes = {
  .name = "Receive2Task",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for SendTask */
osThreadId_t SendTaskHandle;
const osThreadAttr_t SendTask_attributes = {
  .name = "SendTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Receive1_Task(void *argument);
void Receive2_Task(void *argument);
void Send_Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  printf("FreeRTOS任务通知代替消息队列实验\n");
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Receive1Task */
  Receive1TaskHandle = osThreadNew(Receive1_Task, NULL, &Receive1Task_attributes);

  /* creation of Receive2Task */
  Receive2TaskHandle = osThreadNew(Receive2_Task, NULL, &Receive2Task_attributes);

  /* creation of SendTask */
  SendTaskHandle = osThreadNew(Send_Task, NULL, &SendTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_Receive1_Task */
/**
  * @brief  Function implementing the Receive1Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Receive1_Task */
void Receive1_Task(void *argument)
{
  /* USER CODE BEGIN Receive1_Task */
  /* Infinite loop */
	BaseType_t xReturn = pdTRUE;/* 定义一个创建信息返回值，默认为pdPASS */

	uint32_t r_num;

	for(;;)
	{
	/* BaseType_t xTaskNotifyWait(uint32_t ulBitsToClearOnEntry,
								  uint32_t ulBitsToClearOnExit,
								  uint32_t *pulNotificationValue,
								  TickType_t xTicksToWait );
	 * ulBitsToClearOnEntry：当没有接收到任务通知的时候将任务通知值与此参数的取
	   反值进行按位与运算，当此参数为Oxffffffff时就会将任务通知值清零。
	 * ulBits ToClearOnExit：如果接收到了任务通知，在做完相应的处理退出函数之前将
	   任务通知值与此参数的取反值进行按位与运算，当此参数为0xffffffff或者ULONG MAX的时候
	   就会将任务通知值清零。
	 * pulNotification Value：此参数用来保存任务通知值。
	 * xTick ToWait：阻塞时间。
	 *
	 * 返回值：pdTRUE：获取到了任务通知。pdFALSE：任务通知获取失败。
	 */
	//获取任务通知 ,没获取到则一直等待
		xReturn=xTaskNotifyWait(0x0,			//进入函数的时候不清除任务bit
							0xffffffff,	  //退出函数的时候清除所有的bit
							&r_num,		  //保存任务通知值
							portMAX_DELAY);	//阻塞时间
		if( pdTRUE == xReturn )
		{
			printf("Receive1_Task任务通知消息为%ld \n",r_num);
		}

		LED1_TOGGLE;
	}
  /* USER CODE END Receive1_Task */
}

/* USER CODE BEGIN Header_Receive2_Task */
/**
* @brief Function implementing the Receive2Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Receive2_Task */
void Receive2_Task(void *argument)
{
  /* USER CODE BEGIN Receive2_Task */
	BaseType_t xReturn = pdTRUE;/* 定义一个创建信息返回值，默认为pdPASS */

	uint32_t r_num;

	for(;;)
	{
	/* BaseType_t xTaskNotifyWait(uint32_t ulBitsToClearOnEntry,
								  uint32_t ulBitsToClearOnExit,
								  uint32_t *pulNotificationValue,
								  TickType_t xTicksToWait );
	 * ulBitsToClearOnEntry：当没有接收到任务通知的时候将任务通知值与此参数的取
	   反值进行按位与运算，当此参数为Oxffffffff时就会将任务通知值清零。
	 * ulBits ToClearOnExit：如果接收到了任务通知，在做完相应的处理退出函数之前将
	   任务通知值与此参数的取反值进行按位与运算，当此参数为0xffffffff或者ULONG MAX的时候
	   就会将任务通知值清零。
	 * pulNotification Value：此参数用来保存任务通知值。
	 * xTick ToWait：阻塞时间。
	 *
	 * 返回值：pdTRUE：获取到了任务通知。pdFALSE：任务通知获取失败。
	 */
	//获取任务通知 ,没获取到则一直等待
		xReturn=xTaskNotifyWait(0x0,			//进入函数的时候不清除任务bit
							0xffffffff,	  //退出函数的时候清除所有的bit
							&r_num,		  //保存任务通知值
							portMAX_DELAY);	//阻塞时间
		if( pdTRUE == xReturn )
		{
			printf("Receive2_Task任务通知消息为%ld \n",r_num);
		}

		LED2_TOGGLE;
	}
  /* USER CODE END Receive2_Task */
}

/* USER CODE BEGIN Header_Send_Task */
/**
* @brief Function implementing the SendTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Send_Task */
void Send_Task(void *argument)
{
  /* USER CODE BEGIN Send_Task */

	  uint32_t send1 = 1;
	  uint32_t send2 = 2;

	  for(;;)
	  {
	    /* KEY1 被按下 */
	    if( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON )
	    {
	      /* 原型:BaseType_t xTaskNotify( TaskHandle_t xTaskToNotify,
	                                      uint32_t ulValue,
	                                      eNotifyAction eAction );
	       * eNoAction = 0，通知任务而不更新其通知值。
	       * eSetBits，     设置任务通知值中的位。
	       * eIncrement，   增加任务的通知值。
	       * eSetvaluewithoverwrite，覆盖当前通知
	       * eSetValueWithoutoverwrite 不覆盖当前通知
	       *
	       * pdFAIL：当参数eAction设置为eSetValueWithoutOverwrite的时候，
	                 * 如果任务通知值没有更新成功就返回pdFAIL。
	       * pdPASS: eAction 设置为其他选项的时候统一返回pdPASS。
	      */
	      xTaskNotify( Receive1TaskHandle, /*任务句柄*/
	                              send1, /* 发送的数据，最大为4字节 */
	                             eSetValueWithOverwrite );/*覆盖当前通知*/

	    }
	    /* KEY2 被按下 */
	    if( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON )
	    {
	      xTaskNotify( Receive2TaskHandle, /*任务句柄*/
	                             send2, /* 发送的数据，最大为4字节 */
	                             eSetValueWithOverwrite );/*覆盖当前通知*/

	    }
	    osDelay(20);
	  }
  /* USER CODE END Send_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
