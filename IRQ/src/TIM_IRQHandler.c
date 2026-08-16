#include "TIM_IRQHandler.h"
#include "EXTI_IRQHandler.h"

uint16_t tim_count = 0;
static uint8_t dir = 1;
uint16_t time = 0;
uint8_t led_bre = 0;

void TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{   
    if(htim->Instance == TIM3)
    {   
        if(led_bre){
            static uint32_t acc = 0;
            uint16_t period = led_T;
            if (period < 4) period = 4;      /* 防除零/防闪烁 */

            acc += 1998;                     /* 每 1ms 应走的总格数 */
            while (acc >= period) {          /* 攒够一份走一格 */
                acc -= period;
                if(dir == 1) tim_count++;
                else          tim_count--;

                if(tim_count >= 999){ tim_count = 999; dir = 0; }
                if(tim_count <= 0)  { tim_count = 0;   dir = 1; }
            }
            __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,tim_count);
            __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,tim_count);
        }
        else {
            __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
            __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,0);
        }
    }
}