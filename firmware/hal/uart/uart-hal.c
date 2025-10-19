#include "uart-hal.h"

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>

void HAL_UART_Init(unsigned int baud_rate)
{
	// Calculating clock frequency
	unsigned int ubrr = (F_CPU / (16UL * baud_rate)) - 1;

	// Upload register values for UBRRn (12-bits)
	UBRR0H = (unsigned char) (ubrr >> 8);
	UBRR0L = (unsigned char) ubrr;
	
	// Enabling receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	
	// Set Frame format: 8 data and 2 stop bits
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

void HAL_UART_Transmit(unsigned char data)
{
	// Waiting for empty transmit buffer
	while(!(UCSR0A & (1 << UDRE0)));
	
	// Copying the data to be transmitted
	UDR0 = data;
}