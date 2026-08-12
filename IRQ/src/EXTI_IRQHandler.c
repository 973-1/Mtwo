#include "EXTI_IRQHandler.h"

volatile uint8_t BEEP_Trigger = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == INPUT_1_Pin)
    {

    }
}

