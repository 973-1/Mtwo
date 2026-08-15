#include "myostask.h"

void LED_Task(void *argument)
{
  for(;;)
  {
    LED_ON(1);
    osDelay(250);
    LED_OFF(1);
    osDelay(250);
    LED_ON(2);
    osDelay(250);
    LED_OFF(2);
    osDelay(250);
  }
}

void BEEP_Task(void *argument)
{
  for(;;)
  {
    if (BEEP_Trigger != 0)
    {
      for(uint8_t i = 0; i < BEEP_Trigger ; i++)
    {
        BEEP_ON();
        osDelay(200);
        BEEP_OFF();
        osDelay(200);
    }
      BEEP_Trigger = 0;
    }
  }
}

void LOG_Task(void *argument)
{
  for(;;)
  {
    
  }
}