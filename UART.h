#ifndef UART_H
#define UART_H

#include <stdint.h>

void UART_Init(void);
void UART_TransmitString(const char* s);
uint8_t UART_ReceiveByte(void);

#endif
