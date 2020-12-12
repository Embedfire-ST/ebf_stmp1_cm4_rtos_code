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
/* Definitions for ReceiveTask */
osThreadId_t ReceiveTaskHandle;
const osThreadAttr_t ReceiveTask_attributes = {
  .name = "ReceiveTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for SendTask */
osThreadId_t SendTaskHandle;
const osThreadAttr_t SendTask_attributes = {
  .name = "SendTask",
  .priority = (osPriority_t) osPriorityLow,
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

void Receive_Task(void *argument);
void Send_Task(void *argument);

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
  /* creation of ReceiveTask */
  ReceiveTaskHandle = osThreadNew(Receive_Task, NULL, &ReceiveTask_attributes);

  /* creation of SendTask */
  SendTaskHandle = osThreadNew(Send_Task, NULL, &SendTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_Receive_Task */
/**
  * @brief  Function implementing the ReceiveTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Receive_Task */
void Receive_Task(void *argument)
{
  /* USER CODE BEGIN Receive_Task */
	osStatus_t xReturn = osOK ;
  /* Infinite loop */
	for(;;)
	{
		//获取二值信号量 xSemaphore,没获取到则一直等待
		xReturn = osSemaphoreAcquire(BinarySem01Handle,/* 二值信号量句柄 */
								osWaitForever); /* 等待时间 */
		if(osOK == xReturn)
		  printf("BinarySem_Handle二值信号量获取成功!\n");

	}
  /* USER CODE END Receive_Task */
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
	osStatus_t xReturn = osOK ;
	/* Infinite loop */
	for(;;)
	{
	    /* K1 被按下 */
	    if( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON )
	    {
	      xReturn = osSemaphoreRelease( BinarySem01Handle );//给出二值信号量
	      if( xReturn == osOK )
	        printf("BinarySem_Handle二值信号量释放成功!\r\n\n");
	      else
	        printf("BinarySem_Handle二值信号量释放失败!\r\n\n");
	    }
	    /* K2 被按下 */
	    if( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON )
	    {
	      xReturn = osSemaphoreRelease( BinarySem01Handle );//给出二值信号量
	      if( xReturn == osOK )
	        printf("BinarySem_Handle二值信号量释放成功!\r\n\n");
	      else
	        printf("BinarySem_Handle二值信号量释放失败!\r\n\n");
	    }
	    osDelay(20);
	  }

	/* USER CODE END Send_Task */
	}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
