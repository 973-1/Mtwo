#ifndef EXTI_IRQHandler_H
#define EXTI_IRQHandler_H

#include "main.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

extern uint8_t time_period;

#endif