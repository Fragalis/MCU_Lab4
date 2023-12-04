/*
 * scheduler.c
 *
 *  Created on: Nov 29, 2023
 *      Author: ACER
 */

#include "scheduler.h"

typedef struct {
	// Pointer to the task (must be a ’void(void)’ function )
	void (* pTask)(void);
	// Delay (tick) until the function will (next) be run
	uint32_t Delay;
	// Interval (tick) between subsequent runs
	uint32_t Period;
	// Incremented (by scheduler) when task is due to execute
	uint8_t RunMe;
	uint32_t TaskID;
} sTask;

sTask SCH_Tasks_G[SCH_MAX_TASKS];
static uint32_t newTaskID = 0;
static uint32_t SCH_Update_Count = 0;
unsigned char SCH_Error_G;

static uint32_t Get_New_Task_ID(void){
	newTaskID++;
	if(newTaskID == NO_TASK_ID){
		newTaskID++;
	}
	return newTaskID;
}

void SCH_Init(void)
{
	unsigned char i;
	while(i < SCH_MAX_TASKS) {
		SCH_Delete_Task(i++);
	}
	SCH_Error_G = 0;
}

void SCH_Update(void)
{
	++ SCH_Update_Count;
	if (SCH_Tasks_G[0].pTask && SCH_Tasks_G[0].RunMe == 0) {
		if(SCH_Tasks_G[0].Delay > 0){
			SCH_Tasks_G[0].Delay = SCH_Tasks_G[0].Delay - 1;
		}
		if (SCH_Tasks_G[0].Delay == 0) {
			SCH_Tasks_G[0].RunMe = 1;
		}
	}
}

void SCH_Dispatch_Tasks(void)
{
	if(SCH_Tasks_G[0].RunMe > 0) {
		(*SCH_Tasks_G[0].pTask)();
		SCH_Tasks_G[0].RunMe = 0;
		sTask temptask = SCH_Tasks_G[0];
		SCH_Delete_Task(temptask.TaskID);
		if (temptask.Period != 0) {
			SCH_Add_Task(temptask.pTask, temptask.Period, temptask.Period);
		}
	}
}

uint32_t SCH_Add_Task(void (* pFunction)(), uint32_t DELAY, uint32_t PERIOD)
{
	DELAY = DELAY / TIMER_CYCLE;
	uint8_t newTaskIndex = 0;
	uint32_t sumDelay = 0;
	uint32_t newDelay = 0;

	for(newTaskIndex = 0; newTaskIndex < SCH_MAX_TASKS; newTaskIndex ++){
		sumDelay = sumDelay + SCH_Tasks_G[newTaskIndex].Delay;
		if(sumDelay > DELAY){
			newDelay = DELAY - (sumDelay - SCH_Tasks_G[newTaskIndex].Delay);
			SCH_Tasks_G[newTaskIndex].Delay = sumDelay - DELAY;
			for(uint8_t i = SCH_MAX_TASKS - 1; i > newTaskIndex; i --){
				if(SCH_Tasks_G[i - 1].pTask != 0)
				{
					SCH_Tasks_G[i].pTask = SCH_Tasks_G[i - 1].pTask;
					SCH_Tasks_G[i].Period = SCH_Tasks_G[i - 1].Period;
					SCH_Tasks_G[i].Delay = SCH_Tasks_G[i - 1].Delay;
					SCH_Tasks_G[i].RunMe = SCH_Tasks_G[i - 1].RunMe;
					SCH_Tasks_G[i].TaskID = SCH_Tasks_G[i - 1].TaskID;
				}
			}
			SCH_Tasks_G[newTaskIndex].pTask = pFunction;
			SCH_Tasks_G[newTaskIndex].Delay = newDelay;
			SCH_Tasks_G[newTaskIndex].Period = PERIOD;
			SCH_Tasks_G[newTaskIndex].RunMe = 0;
			if(SCH_Tasks_G[newTaskIndex].Delay == 0){
				SCH_Tasks_G[newTaskIndex].RunMe = 1;
			}
			SCH_Tasks_G[newTaskIndex].TaskID = Get_New_Task_ID();
			return SCH_Tasks_G[newTaskIndex].TaskID;
		} else {
			if(SCH_Tasks_G[newTaskIndex].pTask == 0x0000){
				SCH_Tasks_G[newTaskIndex].pTask = pFunction;
				SCH_Tasks_G[newTaskIndex].Delay = DELAY - sumDelay;
				SCH_Tasks_G[newTaskIndex].Period = PERIOD;
				SCH_Tasks_G[newTaskIndex].RunMe = 0;
				if(SCH_Tasks_G[newTaskIndex].Delay == 0){
					SCH_Tasks_G[newTaskIndex].RunMe = 1;
				}
				SCH_Tasks_G[newTaskIndex].TaskID = Get_New_Task_ID();
				return SCH_Tasks_G[newTaskIndex].TaskID;
			}
		}
	}
	return SCH_Tasks_G[newTaskIndex].TaskID;
}

uint8_t SCH_Delete_Task(uint32_t taskID){
	uint8_t Return_code = RETURN_ERROR;
	if(taskID != NO_TASK_ID){
		for(uint8_t taskIndex = 0; taskIndex < SCH_MAX_TASKS; taskIndex ++){
			if(SCH_Tasks_G[taskIndex].TaskID == taskID){
				Return_code = RETURN_NORMAL;
				if((taskIndex != 0 && taskIndex < SCH_MAX_TASKS - 1) && (SCH_Tasks_G[taskIndex+1].pTask != 0x0000)){
						SCH_Tasks_G[taskIndex+1].Delay += SCH_Tasks_G[taskIndex].Delay;
				}
				uint8_t j;
				for(j = taskIndex; j < SCH_MAX_TASKS - 1; j ++){
					SCH_Tasks_G[j].pTask = SCH_Tasks_G[j+1].pTask;
					SCH_Tasks_G[j].Period = SCH_Tasks_G[j+1].Period;
					SCH_Tasks_G[j].Delay = SCH_Tasks_G[j+1].Delay;
					SCH_Tasks_G[j].RunMe = SCH_Tasks_G[j+1].RunMe;
					SCH_Tasks_G[j].TaskID = SCH_Tasks_G[j+1].TaskID;
				}
				SCH_Tasks_G[j].pTask = 0x0000;
				SCH_Tasks_G[j].Period = 0;
				SCH_Tasks_G[j].Delay = 0;
				SCH_Tasks_G[j].RunMe = 0;
				SCH_Tasks_G[j].TaskID = 0;
				return Return_code;
			}
		}
	}
	return Return_code; // return status
}
