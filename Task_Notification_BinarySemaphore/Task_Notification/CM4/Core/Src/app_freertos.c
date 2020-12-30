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
  printf("FreeRTOS任务通知代替二值信号量实验\n");
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
	for(;;)
	{
		/* uint32_t ulTaskNotifyTake( BaseType_t xClearCountOnExit, TickType_t xTicksToWait );
		 * xClearCountOnExit：pdTRUE 在退出函数的时候任务任务通知值清零，类似二值信号量
		 * pdFALSE 在退出函数ulTaskNotifyTakeO的时候任务通知值减一，类似计数型信号量。
		 */
		//获取任务通知 ,没获取到则一直等待
		ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
		printf("Receive1_Task 任务通知获取成功!\n");

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

	for(;;)
	{
	    /* uint32_t ulTaskNotifyTake( BaseType_t xClearCountOnExit, TickType_t xTicksToWait );
	     * xClearCountOnExit：pdTRUE 在退出函数的时候任务任务通知值清零，类似二值信号量
	     * pdFALSE 在退出函数ulTaskNotifyTakeO的时候任务通知值减一，类似计数型信号量。
	     */
	    //获取任务通知 ,没获取到则一直等待
	    ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
	    printf("Receive2_Task 任务通知获取成功!\n");

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
	for(;;)
	{
		/* KEY1 被按下 */
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON )
		{
		  /* 原型:BaseType_t xTaskNotifyGive( TaskHandle_t xTaskToNotify ); */
		  xTaskNotifyGive(Receive1TaskHandle);

		}

		/* KEY2 被按下 */
		if( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON )
		{
		  xTaskNotifyGive(Receive2TaskHandle);
		  /* 此函数只会返回pdPASS */
		}
		osDelay(20);
	}
  /* USER CODE END Send_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
