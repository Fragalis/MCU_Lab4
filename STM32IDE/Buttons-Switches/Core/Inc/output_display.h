/*
 * output_display.h
 *
 *  Created on: Nov 10, 2023
 *      Author: ACER
 */

#ifndef INC_OUTPUT_DISPLAY_H_
#define INC_OUTPUT_DISPLAY_H_

#include "main.h"
#include "global.h"
#include "timer.h"

extern int durationRED;
extern int durationYELLOW;
extern int durationGREEN;

void display_7SEG(unsigned char num);
void update_7SEG(void);
void update_buffer(unsigned char num1, unsigned char num2);

void reset_state(unsigned char signal);

void display_traffic_LEDS(void);
void blinking_LED_RED(void);
void blinking_LED_YELLOW(void);
void blinking_LED_GREEN(void);
void reset_LEDS(void);

#endif /* INC_OUTPUT_DISPLAY_H_ */