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

#include "string.h"
#include "./led/bsp_led.h"
#include "./basic_tim/bsp_basic_tim.h"
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
/* Definitions for LED1Task */
osThreadId_t LED1TaskHandle;
const osThreadAttr_t LED1Task_attributes = {
  .name = "LED1Task",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for LED2Task */
osThreadId_t LED2TaskHandle;
const osThreadAttr_t LED2Task_attributes = {
  .name = "LED2Task",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for CPUTask */
osThreadId_t CPUTaskHandle;
const osThreadAttr_t CPUTask_attributes = {
  .name = "CPUTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 256 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void LED1_Task(void *argument);
void LED2_Task(void *argument);
void CPU_Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{

}

__weak unsigned long getRunTimeCounterValue(void)
{
return 0;
}
/* USER CODE END 1 */

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
  /* creation of LED1Task */
  LED1TaskHandle = osThreadNew(LED1_Task, NULL, &LED1Task_attributes);

  /* creation of LED2Task */
  LED2TaskHandle = osThreadNew(LED2_Task, NULL, &LED2Task_attributes);

  /* creation of CPUTask */
  CPUTaskHandle = osThreadNew(CPU_Task, NULL, &CPUTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_LED1_Task */
/**
  * @brief  Function implementing the LED1Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_LED1_Task */
void LED1_Task(void *argument)
{
  /* USER CODE BEGIN LED1_Task */
  /* Infinite loop */
  for(;;)
  {

	LED1_ON;
	osDelay(500);   /* 延时500个tick */
	printf("LED1_Task Running,LED1_ON\r\n");
	LED1_OFF;
	osDelay(500);   /* 延时500个tick */
	printf("LED1_Task Running,LED1_OFF\r\n");

  }
  /* USER CODE END LED1_Task */
}

/* USER CODE BEGIN Header_LED2_Task */
/**
* @brief Function implementing the LED2Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LED2_Task */
void LED2_Task(void *argument)
{
  /* USER CODE BEGIN LED2_Task */
  /* Infinite loop */
  for(;;)
  {

	LED2_ON;
	osDelay(300);   /* 延时500个tick */
	printf("LED2_Task Running,LED1_ON\r\n");

	LED2_OFF;
	osDelay(300);   /* 延时500个tick */
	printf("LED2_Task Running,LED1_OFF\r\n");

  }
  /* USER CODE END LED2_Task */
}

/* USER CODE BEGIN Header_CPU_Task */
/**
* @brief Function implementing the CPUTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CPU_Task */

uint8_t CPU_RunInfo[400];		//保存任务运行时间信息

void CPU_Task(void *argument)
{
  /* USER CODE BEGIN CPU_Task */
	HAL_TIM_Base_Start_IT(&htim6);

  /* Infinite loop */
  for(;;)
  {

    memset(CPU_RunInfo,0,400);				//信息缓冲区清零

    vTaskList((char *)&CPU_RunInfo);  //获取任务运行时间信息

    printf("---------------------------------------------\r\n");
    printf("任务名	   任务状态	优先级	剩余栈	任务序号\r\n");
    printf("%s", CPU_RunInfo);
    printf("---------------------------------------------\r\n");

    memset(CPU_RunInfo,0,400);				//信息缓冲区清零

    vTaskGetRunTimeStats((char *)&CPU_RunInfo);

    printf("任务名		运行计数	使用率\r\n");
    printf("%s", CPU_RunInfo);
    printf("---------------------------------------------\r\n\n");
	osDelay(1000);   /* 延时500个tick */

  }
  /* USER CODE END CPU_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
