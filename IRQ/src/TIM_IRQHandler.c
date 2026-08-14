#include "TIM_IRQHandler.h"
#include "EXTI_IRQHandler.h"

uint16_t tim_count = 0;
static uint8_t dir = 1;
uint16_t time = 0;
uint8_t led_bre = 0;

void TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{   __HAL_TIM_SET_AUTORELOAD(&htim3, led_T);
    HAL_TIM_GenerateEvent(&htim3, TIM_EVENTSOURCE_UPDATE);
    if(htim->Instance == TIM3)
    {   if(led_bre){
            if(dir == 1)
                {
                    tim_count++;
                }
            else{
                    tim_count--;
                }
            if(tim_count >=led_T)
            {
                dir = 0;
            }
            else if(tim_count == 0)
            {
                dir = 1;
            }
            __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,tim_count);
            __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,tim_count);
        }
    }
}