/*
  uc-Microlab Example: UART HAL usage (main.c)
  Repository: uc-Microlab

  Description:
    Simple example showing how to use the UART HAL (uart-hal.h / uart-hal.c).
    The program initializes the UART at 9600 baud and sends a single byte (0x0F)
    every second. This file demonstrates the expected calls:
      - HAL_UART_Init(unsigned long baud);
      - HAL_UART_Transmit(unsigned char data);

  Hardware: uc-Microlab — version r1
  Target MCU: ATmega328P (Arduino Uno compatible)

  Connections:
    - UART TX (MCU PD1 / TXD0) -> serial adapter RX
    - UART RX (MCU PD0 / RXD0) -> serial adapter TX  (not used in this example)

  Build notes / usage:
    - Define F_CPU before including <util/delay.h> (this file defines it below).
    - Add uart-hal.c to the project source files so the linker finds the HAL symbols.
    - In Microchip Studio: Project -> Add -> Existing Item... -> select uart-hal.c
    - Serial monitor settings: 9600 baud, 8 data bits, no parity, 1 stop bit (8N1).
    - SPDX-License-Identifier: MIT — see repository LICENSE for full terms.

  Author: otavioacb
  Date: 2025-10-19
*/


#define F_CPU 16000000UL

#include <util/delay.h>
#include "uart-hal.h"


int main(void)
{
	HAL_UART_Init(9600);	

    while(1)
    {
		HAL_UART_Transmit(0x0F);
		_delay_ms(1000);
    }

}
