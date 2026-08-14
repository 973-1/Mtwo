#ifndef UART_IRQHandler_H
#define UART_IRQHandler_H

#include "main.h"
#include "usart.h"
#include "beep.h"

void UART_Start_Receive(void);
extern uint8_t led_bre;

#endif