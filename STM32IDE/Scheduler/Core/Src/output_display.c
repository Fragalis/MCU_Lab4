/*
 * output_display.c
 *
 *  Created on: Nov 29, 2023
 *      Author: ACER
 */


#include "output_display.h"

void Led_Red_Blinky(void) {
	HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
}
void Led_Yellow_Blinky(void) {
	HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin);
}
void Led_Green_Blinky(void) {
	HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
}
void Left_7SEG_Blinky(void) {
	HAL_GPIO_TogglePin(LEFT_7SEG_GPIO_Port, LEFT_7SEG_Pin);
	HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin, GPIO_PIN_RESET);
}
void Right_7SEG_Blinky(void) {
	HAL_GPIO_TogglePin(RIGHT_7SEG_GPIO_Port, RIGHT_7SEG_Pin);
	HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin, GPIO_PIN_RESET);
}
