#include "beep.h"

void Beep_Init(void)
{
    BEEP_ON();
    HAL_Delay(200);
    BEEP_OFF();
}

void Beep_Alarm(uint8_t times)
{
    if(uint8_t i = 0; i < times ; i++)
    {
        BEEP_ON();
        HAL_Delay(times);
        BEEP_OFF();
    }
}