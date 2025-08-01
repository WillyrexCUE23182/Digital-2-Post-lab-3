#include <avr/io.h>
#include "UART.h"

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VAL ((F_CPU/16/BAUD) - 1)

void UART_Init(void) {
	UBRR0H = (uint8_t)(UBRR_VAL >> 8);
	UBRR0L = (uint8_t) UBRR_VAL;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

void UART_TransmitString(const char* s) {
	while (*s) {
		while (!(UCSR0A & (1<<UDRE0)));
		UDR0 = *s++;
	}
}

uint8_t UART_ReceiveByte(void) {
	while (!(UCSR0A & (1<<RXC0)));
	return UDR0;
}
