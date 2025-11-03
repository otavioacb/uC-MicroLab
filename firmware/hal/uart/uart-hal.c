/*
 * uc-Microlab — UART HAL (header comment)
 * File: uart-hal.h / uart-hal.c
 *
 * Project: uc-Microlab
 * Component: UART Hardware Abstraction Layer (HAL)
 * Hardware: uc-Microlab board — version r1
 *
 * Description:
 *   Minimal, portable UART HAL for AVR (ATmega) and other platforms.
 *   Provides initialization and transmit/receive primitives and a small,
 *   well-documented API surface for embedding in examples and libraries.
 *
 * Public API (examples of functions that should be declared/implemented):
 *   void HAL_UART_Init(unsigned int baud_rate);
 *   void HAL_UART_Transmit(unsigned char data);
 *   void HAL_UART_Send(unsigned char* msg, size_t len_msg);
 *
 * Usage:
 *   - Include the header in your sources: #include "uart-hal.h"
 *   - Call HAL_UART_Init(F_CPU/baud) once during startup, then use transmit/receive.
 *   - The implementation file (uart-hal.c or platform-specific file) must be
 *     added to the project build so the linker can find the symbols.
 *
 * Notes:
 *   - This comment block is intended to live at the top of uart-hal.h and/or
 *     uart-hal.c to identify the file in the uc-Microlab repository.
 *   - For AVR implementations, ensure F_CPU is defined before including
 *     <util/delay.h> or set F_CPU in the project compiler symbols.
 *
 * Author: otavioacb
 * Created: 2025-10-19
 *
 * License:
 *   SPDX-License-Identifier: MIT
 *   This file is licensed under the MIT License — see the repository LICENSE file
 *   for full terms and copyright information.
 *
 * Change log:
 *   2025-10-19  v0.1  Initial header for uc-Microlab UART HAL
 *
 */
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

void HAL_UART_Send(unsigned char* msg, size_t len_msg)
{
	for(int i = 0; i < len_msg; ++i) HAL_UART_Transmit(msg[i]);
}

