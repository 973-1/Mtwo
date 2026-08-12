#include "beep.h"

uint8_t BEEP_Trigger = 0;

void Beep_Init(void)
{
    BEEP_OFF();
}

void Beep_Alarm(uint8_t times)
{
    for(uint8_t i = 0; i < times ; i++)
    {
        BEEP_ON();
        HAL_Delay(200);
        BEEP_OFF();
    }
}