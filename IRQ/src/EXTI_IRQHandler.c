#include "EXTI_IRQHandler.h"

uint32_t time_now = 0;
uint32_t time_period = 0;
uint8_t num = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == INPUT_1_Pin)
    {
        if(num == 1)
        {
            time_period = HAL_GetTick() - time_now;  // 第二次触发：用上次记录的时间算间隔
            num = 0;
        }
        else
        {
            time_now = HAL_GetTick();                // 第一次触发：记录起始时间
            num = 1;
        }
    }
}
