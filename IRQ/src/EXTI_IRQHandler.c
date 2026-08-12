#include "EXTI_IRQHandler.h"

 uint8_t type_now = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == INPUT_1_Pin)
    {

    }
}

