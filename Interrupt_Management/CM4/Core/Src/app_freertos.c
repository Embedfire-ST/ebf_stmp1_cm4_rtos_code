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
#include "./usart/bsp_debug_usart.h"


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

//
static uint32_t send_data1 = 1;
static uint32_t send_data2 = 2;

/* USER CODE END Variables */
/* Definitions for LEDTask */
osThreadId_t LEDTaskHandle;
const osThreadAttr_t LEDTask_attributes = {
  .name = "LEDTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for ReceiveTask */
osThreadId_t ReceiveTaskHandle;
const osThreadAttr_t ReceiveTask_attributes = {
  .name = "ReceiveTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for Queue */
osMessageQueueId_t QueueHandle;
const osMessageQueueAttr_t Queue_attributes = {
  .name = "Queue"
};
/* Definitions for BinarySem */
osSemaphoreId_t BinarySemHandle;
const osSemaphoreAttr_t BinarySem_attributes = {
  .name = "BinarySem"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void USART_IDLE_IRQHandler(UART_HandleTypeDef *huart); //编写串口IDLE中断处理函数
/* USER CODE END FunctionPrototypes */

void LED_Task(void *argument);
void Receive_Task(void *argument);

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
  /* creation of BinarySem */  //修改初始二值信号量为0
  BinarySemHandle = osSemaphoreNew(1, 0, &BinarySem_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  printf("FreeRTOS中断管理实验\n");
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of Queue */
  QueueHandle = osMessageQueueNew (4, sizeof(uint32_t), &Queue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of LEDTask */
  LEDTaskHandle = osThreadNew(LED_Task, NULL, &LEDTask_attributes);

  /* creation of ReceiveTask */
  ReceiveTaskHandle = osThreadNew(Receive_Task, NULL, &ReceiveTask_attributes);

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
  uint32_t r_queue;
  osStatus_t restat;
  /* Infinite loop */
  for(;;)
  {
	 /* 队列读取（接收），等待时间为一直等待 */
	 restat = osMessageQueueGet (QueueHandle, &r_queue, 0, osWaitForever);
	 if(osOK == restat )
	 {
		printf("触发中断的是 KEY%ld !\n",r_queue);
	 }
	 else
	 {
		printf("数据接收出错\n");
	 }
	 LED1_TOGGLE;
  }
  /* USER CODE END LED_Task */
}

/* USER CODE BEGIN Header_Receive_Task */
/**
* @brief Function implementing the ReceiveTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Receive_Task */
void Receive_Task(void *argument)
{
  /* USER CODE BEGIN Receive_Task */
  osStatus_t Ret = osOK;

  //先清除IDLE中断标志，防止开启IDLE中断后进入IDLE中断
  __HAL_UART_CLEAR_IDLEFLAG(&huart3);

  //开启IDLE中断标志
  __HAL_UART_ENABLE_IT(&huart3,UART_IT_IDLE);

  /* Infinite loop */
  for(;;)
  {
	  //开启串口数据接收
	  HAL_UART_Receive_DMA(&huart3, rx_buf, RX_MAX);
	  //获取二值信号量 xSemaphore,没获取到则一直等待
	  Ret = osSemaphoreAcquire(BinarySemHandle,osWaitForever);
	  if( Ret == osOK)
	  {
		  LED2_TOGGLE;
		  printf("接收的数据为：%s\n",rx_buf);
		  memset(rx_buf,0,RX_MAX);	//清空字符串
	  }
  }
  /* USER CODE END Receive_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
//EXIT 上升沿触发回调函数
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{


    uint32_t ulReturn;
    /* 进入临界段，临界段可以嵌套 */
  	ulReturn = taskENTER_CRITICAL_FROM_ISR();

  	//判断产生外部中断的引脚
	if(KEY1_Pin == GPIO_Pin)
	{
		osMessageQueuePut(QueueHandle, &send_data1,0,0);
	}
	//判断产生外部中断的引脚
	if(KEY2_Pin == GPIO_Pin)
	{
		osMessageQueuePut(QueueHandle, &send_data2,0,0);
	}
	/* 退出临界段 */
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );

}


//编写IDLE中断处理函数
void USART_IDLE_IRQHandler(UART_HandleTypeDef *huart)
{
	uint32_t ulReturn;

	/* 进入临界段，临界段可以嵌套 */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();

	if(__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE))
	{
		//停止DMA接收
		HAL_UART_DMAStop(&huart3);

		//释放二值信号量 ，发送接收到新数据标志，供前台程序查询
		osSemaphoreRelease(BinarySemHandle);

		//清除IDLE中断标志
		__HAL_UART_CLEAR_IDLEFLAG(huart);

	}

	/* 退出临界段 */
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}


/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
