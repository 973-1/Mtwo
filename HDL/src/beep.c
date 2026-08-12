#include "beep.h"

void Beep_Init(void)
{
    BEEP_ON();
    HAL_Delay(200);
    BEEP_OFF();
}

void Beep_Alarm(uint8_t times)
{
    BEEP_ON();
    HAL_Delay(times);
    BEEP_OFF();

}