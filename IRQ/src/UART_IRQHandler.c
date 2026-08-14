#include "UART_IRQHandler.h"

uint8_t rx_buffer[5] = {0};

void UART_Start_Receive(void)
{
    //HAL_UART_Receive_IT(&huart1 ,rx_buffer ,5);
    //HAL_UARTEx_ReceiveToIdle_IT(&huart1,rx_buffer,sizeof(rx_buffer));
    //HAL_UART_Receive_DMA(&huart1 ,rx_buffer ,5);
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1,rx_buffer,sizeof(rx_buffer));
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
 if(huart->Instance == USART1)
 {
    if (rx_buffer[5] == 0XFF)
    {
        for(uint8_t i = 0; i < 5; i++)
        {
            if(rx_buffer[i] == 1)
            {

            }
        }
    }
    HAL_UART_Receive_IT(&huart1 ,rx_buffer ,5);
 }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef*huart, uint16_t Size)
{
    if (huart->Instance == USART1)
    {
        if(rx_buffer[0] == 0xAA)
        {
            for(uint8_t i = 1;i < Size; i++)
            {
                if (rx_buffer[i] == 1)
                {
                    BEEP_Trigger++;
                }
            }

        }
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_buffer,sizeof(rx_buffer));
    }
}