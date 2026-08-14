#include "myostask.h"

void LED_Task(void *argument)
{
  for(;;)
  {
    osDelay(10);
  }
}

void BEEP_Task(void *argument)
{
  for(;;)
  {
    osDelay(10);
  }
}