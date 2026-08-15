#include "TIM_IRQHandler_1.h"
#include "EXTI_IRQHandler.h"


void TIM_PeriodElapsedCallback_1(TIM_HandleTypeDef *htim)
{   
    float value ;

    static float phase = 0.0f;
    const float step =  PI/ 200.0f; 
    value = sin(phase);
   
    phase += step;
    if (phase > 2.0f * PI) 
    {
        phase -= 2.0f * PI;
    }  

    CAN_TxHeaderTypeDef TxHeader_3;
    uint32_t TxMailbox_3;
    uint8_t TxData_3[4] ;
    memcpy(TxData_3, &value, 4);

    TxHeader_3.StdId = 0;
    TxHeader_3.ExtId = 0x02020202;
    TxHeader_3.IDE = CAN_ID_EXT;
    TxHeader_3.DLC =4 ;
    TxHeader_3.TransmitGlobalTime = DISABLE;

    HAL_CAN_AddTxMessage(&hcan1, &TxHeader_3, TxData_3, &TxMailbox_3);

    CAN_TxHeaderTypeDef TxHeader;
    uint32_t TxMailbox;
    uint8_t TxData[3];
    TxData[0] = led_bre;
    TxData[1] = (uint8_t)(led_T & 0xFF);
    TxData[2] = (uint8_t)((led_T >> 8) & 0xFF);

    TxHeader.StdId = 0;
    TxHeader.ExtId = 0x02010101;
    TxHeader.IDE = CAN_ID_EXT;
    TxHeader.DLC = 8;
    TxHeader.TransmitGlobalTime = DISABLE;

    HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
}