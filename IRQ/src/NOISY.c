#include "NOISY.h"

void Noisy(void){
    CAN_TxHeaderTypeDef TxHeader_4;
    uint32_t TxMailbox_4;
    uint8_t TxData_4[6] = {000000};
    

    TxHeader_4.StdId = 0;
    TxHeader_4.ExtId = 0x02010101;
    TxHeader_4.IDE = CAN_ID_EXT;
    TxHeader_4.DLC = 6;
    TxHeader_4.TransmitGlobalTime = DISABLE;

    HAL_CAN_AddTxMessage(&hcan1, &TxHeader_4, TxData_4, &TxMailbox_4);
}