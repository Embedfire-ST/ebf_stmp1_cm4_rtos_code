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
#include "./led/bsp_led.h"
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

#define KEY1_EVENT  (0x01 << 0)//设置事件掩码的位0
#define KEY2_EVENT  (0x01 << 1)//设置事件掩码的位1

/* USER CODE END Variables */
/* Definitions for LEDTask */
osThreadId_t LEDTaskHandle;
const osThreadAttr_t LEDTask_attributes = {
  .name = "LEDTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for KEYTask */
osThreadId_t KEYTaskHandle;
const osThreadAttr_t KEYTask_attributes = {
  .name = "KEYTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for Event01 */
osEventFlagsId_t Event01Handle;
const osEventFlagsAttr_t Event01_attributes = {
  .name = "Event01"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void LED_Task(void *argument);
void KEY_Task(void *argument);

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
  /* creation of LEDTask */
  LEDTaskHandle = osThreadNew(LED_Task, NULL, &LEDTask_attributes);

  /* creation of KEYTask */
  KEYTaskHandle = osThreadNew(KEY_Task, NULL, &KEYTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of Event01 */
  Event01Handle = osEventFlagsNew(&Event01_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_LED_Task */
/**
  * @brief  Function implementing the LEDTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_LED_Task */
void LED_Task(void *argument)
{
	/* USER CODE BEGIN LED_Task */
	/* Infinite loop */

	//uint32_t EventFlag;

	for(;;)
	{
		// 永远等待按键1和按键2事件均发生过
		osEventFlagsWait(Event01Handle,KEY1_EVENT|KEY2_EVENT,osFlagsWaitAll,osWaitForever);
		printf ( "KEY1与KEY2都按下\n");
		LED1_TOGGLE;       //LED1反转

		osDelay(200);
	}
	/* USER CODE END LED_Task */
}

/* USER CODE BEGIN Header_KEY_Task */
/**
* @brief Function implementing the KEYTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_KEY_Task */
void KEY_Task(void *argument)
{
	/* USER CODE BEGIN KEY_Task */
	/* Infinite loop */
	for(;;)
	{

	    if( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON )       //如果KEY2被单击
		{
			printf ( "KEY1被按下\n" );
			/* 触发一个事件1 */
			osEventFlagsSet(Event01Handle,KEY1_EVENT);
		}

		if( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON )       //如果KEY2被单击
		{
			printf ( "KEY2被按下\n" );
			/* 触发一个事件2 */
			osEventFlagsSet(Event01Handle,KEY2_EVENT);
		}

		osDelay(20);     //每20ms扫描一次	(20);

	}
	/* USER CODE END KEY_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
