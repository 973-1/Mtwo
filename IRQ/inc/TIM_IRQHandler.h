#ifndef TIM_IRQHANDLER_H
#define TIM_IRQHANDLER_H

#include "main.h"
#include "tim.h"
#include "EXTI_IRQHandler.h"
#include "DJMotor.h"

void TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
extern uint8_t led_bre;

#endif