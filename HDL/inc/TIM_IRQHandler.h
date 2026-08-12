#ifndef TIM_IRQHANDLER_H
#define TIM_IRQHANDLER_H

#include "main.h"
#include "tim.h"

void TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif