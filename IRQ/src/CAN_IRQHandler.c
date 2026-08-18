#include "CAN_IRQHandler.h"

CAN_RxHeaderTypeDef RxHeader;
CAN_TxHeaderTypeDef tx_header;
uint8_t RxData[10];
uint8_t TxLed = 0;
uint8_t *Rx_data = RxData;

void HAL_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan ->Instance == CAN1)
    {
        if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0,&RxHeader, RxData) == HAL_OK)
        {
            if (RxHeader.StdId == 0x201)
            {   DJmotor_Receive(RxHeader, *Rx_data)
                //BEEP_Trigger ++;
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
            
        }  
        // else if (hcan->Instance == CAN2)
        // {
        //     if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0,&RxHeader, RxData) == HAL_OK)
        //     {

        //     }
        // }
           
    }
}