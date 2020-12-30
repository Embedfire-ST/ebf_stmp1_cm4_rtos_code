/*
 * bsp_basic_tim.c
 *
 *  Created on: Nov 30, 2020
 *      Author: zhan
 */

#include "./basic_tim/bsp_basic_tim.h"
#include "./led/bsp_led.h"


volatile unsigned long CPU_RunTime = 0UL;

void BasicTim_Config(void)
{
	//启动TIM6
	HAL_TIM_Base_Start_IT(&htim6);

}

//回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM6 )
	{
		CPU_RunTime++;

	}

}

void configureTimerForRunTimeStats(void)
{
	 CPU_RunTime = 0UL;
}

unsigned long getRunTimeCounterValue(void)
{
	return CPU_RunTime;
}



















