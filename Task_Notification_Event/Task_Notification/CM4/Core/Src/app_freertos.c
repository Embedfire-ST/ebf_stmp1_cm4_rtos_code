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
#define KEY1_EVENT  (0x01 << 0)//设置事件掩码的位0
#define KEY2_EVENT  (0x01 << 1)//设置事件掩码的位1
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
/* Definitions for KEYTask */
osThreadId_t KEYTaskHandle;
const osThreadAttr_t KEYTask_attributes = {
  .name = "KEYTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for LEDTask */
osThreadId_t LEDTaskHandle;
const osThreadAttr_t LEDTask_attributes = {
  .name = "LEDTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void KEY_Task(void *argument);
void LED_Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  printf("FreeRTOS任务通知代替事件组实验\n");
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
  /* creation of KEYTask */
  KEYTaskHandle = osThreadNew(KEY_Task, NULL, &KEYTask_attributes);

  /* creation of LEDTask */
  LEDTaskHandle = osThreadNew(LED_Task, NULL, &LEDTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_KEY_Task */
/**
  * @brief  Function implementing the KEYTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_KEY_Task */
void KEY_Task(void *argument)
{
  /* USER CODE BEGIN KEY_Task */
  /* Infinite loop */
    /* 任务都是一个无限循环，不能返回 */
	for(;;)
	{
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON )
		{
		  printf ( "KEY1被按下\n" );
		  /* 原型:BaseType_t xTaskNotify( TaskHandle_t xTaskToNotify,
										  uint32_t ulValue,
										  eNotifyAction eAction );
		   * eNoAction = 0，通知任务而不更新其通知值。
		   * eSetBits，     设置任务通知值中的位。
		   * eIncrement，   增加任务的通知值。
		   * eSetvaluewithoverwrite，覆盖当前通知
		   * eSetValueWithoutoverwrite 不覆盖当前通知
		   *
		   * pdFAIL：当参数eAction设置为eSetValueWithoutOverwrite的时候，
		   * 如果任务通知值没有更新成功就返回pdFAIL。
		   * pdPASS: eAction 设置为其他选项的时候统一返回pdPASS。
		  */
				/* 触发一个事件1 */
			xTaskNotify((TaskHandle_t	)LEDTaskHandle,//接收任务通知的任务句柄
					   (uint32_t		)KEY1_EVENT,			//要触发的事件
					   (eNotifyAction)eSetBits);			//设置任务通知值中的位
		}

		if( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON )
		{
			printf ( "KEY2被按下\n" );
			/* 触发一个事件2 */
			xTaskNotify((TaskHandle_t	)LEDTaskHandle,//接收任务通知的任务句柄
						 (uint32_t		)KEY2_EVENT,			//要触发的事件
						  (eNotifyAction)eSetBits);			//设置任务通知值中的位
		}
		osDelay(20);     //每20ms扫描一次
	}
  /* USER CODE END KEY_Task */
}

/* USER CODE BEGIN Header_LED_Task */
/**
* @brief Function implementing the LEDTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LED_Task */
void LED_Task(void *argument)
{
  /* USER CODE BEGIN LED_Task */
  /* Infinite loop */
  uint32_t r_event = 0;  /* 定义一个事件接收变量 */
  uint32_t last_event = 0;/* 定义一个保存事件的变量 */
  BaseType_t xReturn = pdTRUE;/* 定义一个创建信息返回值，默认为pdPASS */
  /* 任务都是一个无限循环，不能返回 */
  for(;;)
  {
	/* BaseType_t xTaskNotifyWait(uint32_t ulBitsToClearOnEntry,
								  uint32_t ulBitsToClearOnExit,
								  uint32_t *pulNotificationValue,
								  TickType_t xTicksToWait );
	 * ulBitsToClearOnEntry：当没有接收到任务通知的时候将任务通知值与此参数的取
	   反值进行按位与运算，当此参数为Oxfffff或者ULONG_MAX的时候就会将任务通知值清零。
	 * ulBits ToClearOnExit：如果接收到了任务通知，在做完相应的处理退出函数之前将
	   任务通知值与此参数的取反值进行按位与运算，当此参数为0xfffff或者ULONG MAX的时候
	   就会将任务通知值清零。
	 * pulNotification Value：此参数用来保存任务通知值。
	 * xTick ToWait：阻塞时间。
	 *
	 * 返回值：pdTRUE：获取到了任务通知。pdFALSE：任务通知获取失败。
	 */
	//获取任务通知 ,没获取到则一直等待
	xReturn = xTaskNotifyWait(0x0,			//进入函数的时候不清除任务bit
							  0xffffffff,	  //退出函数的时候清除所有的bitR
							  &r_event,		  //保存任务通知值
							  portMAX_DELAY);	//阻塞时间
	if( pdTRUE == xReturn )
	{
	  last_event |= r_event;
	  /* 如果接收完成并且正确 */
	  if(last_event == (KEY1_EVENT|KEY2_EVENT))
	  {
		last_event = 0;     /* 上一次的事件清零 */
		printf ( "Key1与Key2都按下\n");
		LED1_TOGGLE;       //LED1	反转
	  }
	  else  /* 否则就更新事件 */
		last_event = r_event;   /* 更新上一次触发的事件 */
	}

  }
  /* USER CODE END LED_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
