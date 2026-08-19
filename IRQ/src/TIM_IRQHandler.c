#include "TIM_IRQHandler.h"
#include "EXTI_IRQHandler.h"

uint16_t tim_count = 0;
static uint8_t dir = 1;
uint16_t time = 0;
uint8_t led_bre = 0;
uint8_t now = 1;

void TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2)
    {   
        DJmotor_Func();
    }
}