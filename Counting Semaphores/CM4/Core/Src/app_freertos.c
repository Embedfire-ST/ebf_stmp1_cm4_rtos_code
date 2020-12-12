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
/* Definitions for TakeTask */
osThreadId_t TakeTaskHandle;
const osThreadAttr_t TakeTask_attributes = {
  .name = "TakeTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for GiveTask */
osThreadId_t GiveTaskHandle;
const osThreadAttr_t GiveTask_attributes = {
  .name = "GiveTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for CountingSem */
osSemaphoreId_t CountingSemHandle;
const osSemaphoreAttr_t CountingSem_attributes = {
  .name = "CountingSem"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Take_Task(void *argument);
void Give_Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of CountingSem */
  CountingSemHandle = osSemaphoreNew(5, 5, &CountingSem_attributes);

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
  /* creation of TakeTask */
  TakeTaskHandle = osThreadNew(Take_Task, NULL, &TakeTask_attributes);

  /* creation of GiveTask */
  GiveTaskHandle = osThreadNew(Give_Task, NULL, &GiveTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_Take_Task */
/**
  * @brief  Function implementing the TakeTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Take_Task */
void Take_Task(void *argument)
{
	/* USER CODE BEGIN Take_Task */
	osStatus_t xReturn = osOK ;
	/* Infinite loop */
	for(;;)
	{
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON )
		{
			xReturn = osSemaphoreAcquire(CountingSemHandle,/* 信号量句柄 */
									0); /* 等待时间 */
			if(osOK == xReturn)
			{
				printf("Conuting_Handle二值信号量获取成功!\n");
			}
			else
			{
				printf("Conuting_Handle二值信号量获取失败!\n");
			}

		}
		osDelay(20);
	}
	/* USER CODE END Take_Task */
}

/* USER CODE BEGIN Header_Give_Task */
/**
* @brief Function implementing the GiveTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Give_Task */
void Give_Task(void *argument)
{
	/* USER CODE BEGIN Give_Task */
	osStatus_t xReturn = osOK ;
	/* Infinite loop */
	for(;;)
	{
		/* K2 被按下 */
		if( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON )
		{
			xReturn = osSemaphoreRelease( CountingSemHandle );//给出信号量
			if( xReturn == osOK )
			{
				printf("Conuting_Handle二值信号量释放成功!\r\n");
			}
			else
			{
				printf("Conuting_Handle二值信号量释放失败!\r\n");
			}

		}
		osDelay(20);
	}
	/* USER CODE END Give_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
