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
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for Test_Queue */
osMessageQueueId_t Test_QueueHandle;
const osMessageQueueAttr_t Test_Queue_attributes = {
  .name = "Test_Queue"
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

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of Test_Queue */
  Test_QueueHandle = osMessageQueueNew (4, sizeof(uint32_t), &Test_Queue_attributes);

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
	osStatus_t xReturn = osOK;
	uint32_t r_queue;	/* 接收消息的变量 */
	/* Infinite loop */
	for(;;)
	{

	  xReturn = osMessageQueueGet( Test_QueueHandle, /* 消息队列的句柄 */
									&r_queue,/* 发送的消息内容 */
									0,			/* 发送优先级*/
									osWaitForever);  /*永远等待 */
	    if(osOK == xReturn)
	      printf("本次接收到的数据为%d\n\n",r_queue);
	    else
	      printf("数据接收出错,错误代码0x%lx\n",xReturn);


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
	osStatus_t xReturn = osOK;
	uint32_t send_data1 = 1;
	uint32_t send_data2 = 2;

  /* Infinite loop */
	for(;;)
	{
	    if( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON )
	    {/* K1 被按下 */
	      printf("发送消息send_data1！\n");
	      xReturn = osMessageQueuePut( Test_QueueHandle, /* 消息队列的句柄*/
	                            &send_data1,/* 发送的消息内容 */
								0,			/* 发送优先级*/
	                            0 );        /* 等待时间 0 */
//	      if(xReturn == osOK)
//	        printf("消息send_data1发送成功!\n\n");
	    }
	    if( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON )
	    {/* K2 被按下*/
	      printf("发送消息send_data2！\n");
	      xReturn = osMessageQueuePut( Test_QueueHandle, /* 消息队列的句柄 */
	                            &send_data2,/* 发送的消息内容 */
								0,			/* 发送优先级*/
	                            0 );        /* 等待时间 0 */
//	      if(xReturn == osOK)
//	        printf("消息send_data2发送成功!\n\n");
	    }
	    osDelay(20);/* 延时20个tick */

	}
  /* USER CODE END Send_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
