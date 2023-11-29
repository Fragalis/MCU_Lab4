/*
 * timer.c
 *
 *  Created on: Nov 10, 2023
 *      Author: ACER
 */

#include "timer.h"

int timerLED_counter = 0;
int timerLED_flag = 0;

int timer7SEG_counter = 0;
int timer7SEG_flag = 0;

void set_timerLED(int duration) {
	timerLED_counter = duration / TIMER_CYCLE;
	timerLED_flag = 0;
}

void set_timer7SEG(int duration) {
	timer7SEG_counter = duration / TIMER_CYCLE;
	timer7SEG_flag = 0;
}

void timer_run() {
	if(timerLED_counter > 0) timerLED_counter--;
	timerLED_flag = (timerLED_counter <= 0);

	if(timer7SEG_counter > 0) timer7SEG_counter--;
	timer7SEG_flag = (timer7SEG_counter <= 0);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if(htim->Instance == TIM2) {
		button_reading();
		timer_run();
	}
}