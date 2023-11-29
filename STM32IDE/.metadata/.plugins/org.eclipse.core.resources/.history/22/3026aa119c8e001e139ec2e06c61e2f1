/*
 * timer.c
 *
 *  Created on: Nov 29, 2023
 *      Author: ACER
 */

#include "timer.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if(htim->Instance == TIM2) {
		SCH_Update();
	}
}
