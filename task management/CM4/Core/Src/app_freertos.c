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
/* Definitions for LEDTask */
osThreadId_t LEDTaskHandle;			//定义线程ID
const osThreadAttr_t LEDTask_attributes = {
  .name = "LEDTask",							//LED线程名
  .priority = (osPriority_t) osPriorityNormal,  //线程优先级
  .stack_size = 128 * 4							//栈大小
};
/* Definitions for KEYTask */
osThreadId_t KEYTaskHandle;						//定义线程ID
const osThreadAttr_t KEYTask_attributes = {
  .name = "KEYTask",							//按键任务
  .priority = (osPriority_t) osPriorityNormal1,	//线程优先级
  .stack_size = 128 * 4							//栈大小
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
	printf("这是一个[野火]-STM32全系列开发板-CMSIS_RTOS任务管理实验！\n\n");
	printf("按下KEY1挂起任务，按下KEY2恢复任务\n");
	/* Infinite loop */
	for(;;)
	{
		LED1_TOGGLE;
		printf("LED_Task Running......\r\n");
		osDelay(500);   /* 延时500个tick */

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
	osStatus_t osStatus;

  /* USER CODE BEGIN KEY_Task */
  /* Infinite loop */
	for(;;)
	{
	    if( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON )
	    {/* K1 被按下 */
	    	osStatus = osThreadSuspend(LEDTaskHandle);/* 挂起LED任务 */
	    	if( osOK == osStatus )
	    	{
	    		printf("挂起LED任务成功！\n");
	    	}
	    }
	    if( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON )
	    {/* K2 被按下 */

	    	osStatus = osThreadResume(LEDTaskHandle);/* 恢复LED任务 */
	    	if( osOK == osStatus )
	    	{
	    		printf("恢复LED任务成功！\n");
	    	}
	    }

	    osDelay(20);
	}
  /* USER CODE END KEY_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
