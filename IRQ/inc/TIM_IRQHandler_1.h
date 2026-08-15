#ifndef TIM_IRQHANDLER_1_H
#define TIM_IRQHANDLER_1_H

#define PI 3.14159265359f

#include "main.h"
#include "tim.h"
#include "EXTI_IRQHandler.h"
#include "can.h"
#include "UART_IRQHandler.h"
#include <math.h>
#include "led.h"

void TIM_PeriodElapsedCallback_1(TIM_HandleTypeDef *htim);

#endif