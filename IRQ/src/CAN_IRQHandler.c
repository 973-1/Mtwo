#include "CAN_IRQHandler.h"

CAN_RxHeaderTypeDef RxHeader;
uint8_t RxData[1];
uint8_t TxLed = 0;

void HAL_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan ->Instance == CAN1)
    {
        if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0,&RxHeader, RxData) == HAL_OK)
        {
            if (RxHeader.StdId == 0x201)
            {   //BEEP_Trigger ++;
            //     CAN_TxHeaderTypeDef TxHeader;
            //     uint32_t TxMailbox;
            //     uint8_t TxData[8] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};

            //     TxHeader.StdId = 0x102;
            //     TxHeader.ExtId = 0;
            //     TxHeader.IDE = CAN_ID_STD;//CAN_ID_EXT
            //     TxHeader.DLC = 8;
            //     TxHeader.TransmitGlobalTime = DISABLE;

            //     HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
            }
            else if(RxHeader.ExtId == 0x01020101)
            {   
                BEEP_Trigger = RxData[0];
                
            }
            else if(RxHeader.ExtId == 0x01020201)
            {
                TxLed = RxData[0];
                CAN_TxHeaderTypeDef TxHeader_2;
                uint32_t TxMailbox_2;
                uint8_t TxData_2[3] = {'O','K',RxData[0]};

                TxHeader_2.StdId = 0;
                TxHeader_2.ExtId = 0x02010201;
                TxHeader_2.IDE = CAN_ID_EXT;
                TxHeader_2.DLC = 3;
                TxHeader_2.TransmitGlobalTime = DISABLE;

                HAL_CAN_AddTxMessage(&hcan1, &TxHeader_2, TxData_2, &TxMailbox_2);
            }
        }  
        else if (hcan->Instance == CAN2)
        {
            if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0,&RxHeader, RxData) == HAL_OK)
            {

            }
        }
           
    }
}