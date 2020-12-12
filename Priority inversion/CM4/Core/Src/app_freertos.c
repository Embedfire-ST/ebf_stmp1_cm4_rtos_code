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
/* Definitions for LowPriorityTask */
osThreadId_t LowPriorityTaskHandle;
const osThreadAttr_t LowPriorityTask_attributes = {
  .name = "LowPriorityTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for MidPriorityTask */
osThreadId_t MidPriorityTaskHandle;
const osThreadAttr_t MidPriorityTask_attributes = {
  .name = "MidPriorityTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for HighPriority */
osThreadId_t HighPriorityHandle;
const osThreadAttr_t HighPriority_attributes = {
  .name = "HighPriority",
  .priority = (osPriority_t) osPriorityHigh,
  .stack_size = 128 * 4
};
/* Definitions for BinarySem01 */
osSemaphoreId_t BinarySem01Handle;
const osSemaphoreAttr_t BinarySem01_attributes = {
  .name = "BinarySem01"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void LowPriority_Task(void *argument);
void MidPriority_Task(void *argument);
void HighPriority_Task(void *argument);

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
  /* creation of BinarySem01 */
  BinarySem01Handle = osSemaphoreNew(1, 1, &BinarySem01_attributes);

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
  /* creation of LowPriorityTask */
  LowPriorityTaskHandle = osThreadNew(LowPriority_Task, NULL, &LowPriorityTask_attributes);

  /* creation of MidPriorityTask */
  MidPriorityTaskHandle = osThreadNew(MidPriority_Task, NULL, &MidPriorityTask_attributes);

  /* creation of HighPriority */
  HighPriorityHandle = osThreadNew(HighPriority_Task, NULL, &HighPriority_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_LowPriority_Task */
/**
  * @brief  Function implementing the LowPriorityTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_LowPriority_Task */
void LowPriority_Task(void *argument)
{
	/* USER CODE BEGIN LowPriority_Task */
	/* Infinite loop */
	static uint32_t i;
	osStatus_t xReturn;
	for(;;)
	{
		printf("LowPriority_Task 获取信号量\n");
		//获取二值信号量 xSemaphore,没获取到则一直等待
		xReturn = osSemaphoreAcquire (BinarySem01Handle, osWaitForever);
		if( xReturn == osOK )
			printf("LowPriority_Task Running\n\n");

		for(i=0;i<2000000;i++)//模拟低优先级任务占用信号量
		{
			osThreadYield();//发起任务调度
		}

		printf("LowPriority_Task 释放信号量!\r\n");
		xReturn = osSemaphoreRelease( BinarySem01Handle );//给出二值信号量
		osDelay(500);
	}
	/* USER CODE END LowPriority_Task */
}

/* USER CODE BEGIN Header_MidPriority_Task */
/**
* @brief Function implementing the MidPriorityTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MidPriority_Task */
void MidPriority_Task(void *argument)
{
	/* USER CODE BEGIN MidPriority_Task */
	/* Infinite loop */
	for(;;)
	{
		printf("MidPriority_Task Running\n");
		osDelay(500);
	}
	/* USER CODE END MidPriority_Task */
}

/* USER CODE BEGIN Header_HighPriority_Task */
/**
* @brief Function implementing the HighPriorityTas thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_HighPriority_Task */
void HighPriority_Task(void *argument)
{
	/* USER CODE BEGIN HighPriority_Task */
	/* Infinite loop */
	osStatus_t xReturn;
	for(;;)
	{
		printf("HighPriority_Task 获取信号量\n");
		//获取二值信号量 xSemaphore,没获取到则一直等待
		xReturn = osSemaphoreAcquire (BinarySem01Handle, osWaitForever);

		if(osOK == xReturn)
			printf("HighPriority_Task Running\n");

		xReturn = osSemaphoreRelease( BinarySem01Handle );//给出二值信号量
		if( osOK == xReturn )
			printf("HighPriority_Task 释放信号量!\r\n");

		osDelay(500);

	}
	/* USER CODE END HighPriority_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
