#ifndef BEEP_H
#define BEEP_H

#include "main.h"
#include "gpio.h"
//#include "stm32fixx_hal.h"

#define BEEP_ON() HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_SET)
#define BEEP_OFF() HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_RESET)

void Beep_Init(void);
void Beep_Alarm(uint8_t times);

extern uint8_t BEEP_Trigger;

#endif