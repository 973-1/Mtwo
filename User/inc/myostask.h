#ifndef MYOSTASK_H
#define MYOSTASK_H

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

void LED_Task(void *argument);
void BEEP_Task(void *argument);

#endif