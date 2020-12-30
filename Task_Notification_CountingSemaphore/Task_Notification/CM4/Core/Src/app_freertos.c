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
  printf("FreeRTOS任务通知代替计数信号量实验\n");
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
  /* Infinite loop */
  uint32_t take_num = pdTRUE;/* 定义一个创建信息返回值，默认为pdPASS */
  /* 任务都是一个无限循环，不能返回 */
  for(;;)
  {
	//如果KEY1被单击
	if( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON )
	{
		/* uint32_t ulTaskNotifyTake( BaseType_t xClearCountOnExit, TickType_t xTicksToWait );
		* xClearCountOnExit：pdTRUE 在退出函数的时候任务任务通知值清零，类似二值信号量
		* pdFALSE 在退出函数ulTaskNotifyTakeO的时候任务通知值减一，类似计数型信号量。
		*/
		//获取任务通知 ,没获取到则不等待
		take_num=ulTaskNotifyTake(pdFALSE,0);//
		if(take_num > 0)
		printf( "KEY1被按下，成功申请到停车位，当前车位为 %ld \n", take_num - 1);
			else
		printf( "KEY1被按下，车位已经没有了，请按KEY2释放车位\n" );
	}
	osDelay(20);     //每20ms扫描一次
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
  /* Infinite loop */
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	/* 任务都是一个无限循环，不能返回 */
	for(;;)
	{
	//如果KEY2被单击
		if( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON )
		{
		  /* 原型:BaseType_t xTaskNotifyGive( TaskHandle_t xTaskToNotify ); */
		  /* 释放一个任务通知 */
		  xTaskNotifyGive(TakeTaskHandle);//发送任务通知
		  /* 此函数只会返回pdPASS */
		  if ( pdPASS == xReturn )
		  {
			  printf( "KEY2被按下，释放1个停车位。\n" );
		  }
		}
		osDelay(20);     //每20ms扫描一次
	}
  /* USER CODE END Give_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
