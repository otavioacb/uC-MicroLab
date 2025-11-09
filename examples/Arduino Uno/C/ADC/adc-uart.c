/*
  uc-Microlab Example: ADC + UART
  Repository: uc-Microlab

  Description:
    Simple example showing how to use the ADC HAL (adc-hal.h / adc-hal.c)
    together with the UART HAL (uart-hal.h / uart-hal.c). The program
    initializes the UART at 9600 baud, configures the ADC to sample channel 0
    (10-bit ADC), and in the main loop starts a conversion, waits for it to
    complete, then sends a formatted reading over UART.

    Expected HAL calls shown:
      - HAL_UART_Init(unsigned int baud_rate);
      - HAL_UART_Send(unsigned char* msg, size_t len_msg);
      - HAL_ADC_SetChannel(unsigned char ch);
      - HAL_ADC_SetReference(unsigned char ref);
      - HAL_ADC_SetPrescaler(unsigned char pre);
      - HAL_ADC_Enable(void);
      - HAL_ADC_StartConversion(void);
      - HAL_ADC_Finished(void);
      - HAL_ADC_Read(void);

  Hardware: uc-Microlab — version r1
  Target MCU: ATmega328P (Arduino Uno compatible)

  Connections:
    - ADC CH0 (MCU PC0 / ADC0) -> analog signal to measure
    - UART TX (MCU PD1 / TXD0) -> serial adapter RX
    - UART RX (MCU PD0 / RXD0) -> serial adapter TX  (not used here)

  Build notes / usage:
    - Define F_CPU before including <util/delay.h> (this file defines it below).
    - Add adc-hal.c and uart-hal.c to the project source files so the linker
      finds the HAL symbols (Project -> Add -> Existing Item...).
    - Use snprintf to avoid buffer overruns and send only the actual bytes
      written (example uses strlen to compute length).
    - Serial monitor settings: 9600 baud, 8 data bits, no parity, 1 stop bit (8N1).
    - SPDX-License-Identifier: MIT — see repository LICENSE for full terms.

  Author: otavioacb
  Date: 2025-11-09
*/
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "adc-hal.h"
#include "uart-hal.h"

unsigned char msg[20];
uint16_t adc_val = 0x00;

int main(void)
{
	HAL_UART_Init(9600);
	
	HAL_ADC_SetChannel(HAL_ADC_CH00);
	HAL_ADC_SetReference(HAL_ADC_AVCC);
	HAL_ADC_SetPrescaler(HAL_ADC_DF128);
	              
	HAL_ADC_Enable();
	
    while(1)
    {
		HAL_ADC_StartConversion();
		while(!HAL_ADC_Finished());
		adc_val = HAL_ADC_Read();
		sprintf(msg, "O valor e: %d\n", adc_val);
		HAL_UART_Send(msg, sizeof(msg));
    }
}
