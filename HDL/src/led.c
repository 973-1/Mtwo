#include "led.h"

void led_init(void)
{
//    for(uint8_t i = 0U; i<4; i++)
//    {
//        LED_OFF(i);
//    }
LED_OFF(1);
LED_OFF(2);
}

void led_water(void)
{
//    for(uint8_t i = 0U; i < 2; i++)
//    {
//        LED_ON(i);
//        HAL_Delay(250U);
//        LED_OFF(i);
//    }
LED_ON(1);
HAL_Delay(250);
LED_OFF(1);
HAL_Delay(250);
LED_ON(2);
HAL_Delay(250);
LED_OFF(2);
HAL_Delay(250);
}


