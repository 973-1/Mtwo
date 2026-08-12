#include "EXTI_IRQHandler.h"

 uint8_t time_now = 0;
 uint8_t time_period = 0;
 uint8_t num = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == INPUT_1_Pin)
    {
        time_now = HAL_GetTick();
        if(num == 1)
        {
           time_period = (HAL_GetTick() - time_now);
            num = 0;
        }
        else
        {
            num++;
        }
    }
}

