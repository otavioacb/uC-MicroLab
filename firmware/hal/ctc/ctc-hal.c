/*
 * uc-Microlab – CTC HAL
 * File: ctc-hal.h / ctc-hal.c
 *
 * Project: uc-MicroLab
 * Component: CTC (Clear Timer on Compare) Hardware Abstraction Layer (HAL)
 * Hardware: uc-MicroLab board – version r1 (AVR/ATmega) and compatible platforms
 *
 * Description:
 *   Minimal, portable CTC HAL providing configuration and control for the timers 
 *   and output compare peripherals of AVR microcontrollers in CTC mode 
 *   (Clear Timer on Compare Match). The library is designed to abstract hardware 
 *   details and simplify the setup of timing and compare-match events while 
 *   maximizing portability and maintainability of codebases.
 *
 * Public API:
 *   - void HAL_CTC_Init(uint8_t src, uint8_t ch, uint8_t mode, uint8_t clk);
 *       Initialize the timer for CTC mode with the specified timer source, 
 *       channel, compare mode, and clock selection prescaler.
 *
 *   - void HAL_CTC_SetValue(uint8_t src, uint8_t ch, uint16_t cmp);
 *       Sets the compare value for the given timer and channel.
 *
 *   - void HAL_CTC_ConfigCH0(uint8_t ch, uint8_t mode, uint8_t clk);
 *       Configures Timer/Counter 0 with the specified channel, compare mode, 
 *       and clock prescaler.
 *
 *   - void HAL_CTC_ConfigCH1(uint8_t ch, uint8_t mode, uint8_t clk);
 *       Configures Timer/Counter 1 with the specified channel, compare mode, 
 *       and clock prescaler.
 *
 *   - void HAL_CTC_ConfigCH2(uint8_t ch, uint8_t mode, uint8_t clk);
 *       Configures Timer/Counter 2 with the specified channel, compare mode, 
 *       and clock prescaler.
 *
 * Notes:
 *   - This header uses <avr/io.h> types and register definitions. Platform-
 *     specific implementations may be required for non-AVR targets.
 *   - This library sets up timers in CTC mode and assumes the application only 
 *     uses the specified channels for compare-match functionality.
 *   - Users must ensure the timer does not conflict with other routines or 
 *     interrupts used in the application.
 *   - All setup and configuration functions must be called while the timer is 
 *     stopped, and values must be set appropriately to prevent undefined behavior.
 *
 * Example usage:
 *   - Include this header in application code:
 *       #include "ctc-hal.h"
 *   - Initialize a timer and its channel:
 *       HAL_CTC_Init(HAL_CTC_SRC_1, HAL_CTC_CH_A, HAL_CTCA_COMP_MODE_TOGGLE, HAL_CTC_CH1_CK_64);
 *       HAL_CTC_SetValue(HAL_CTC_SRC_1, HAL_CTC_CH_A, 25000);
 *
 * Author: otavioacb
 * Created: 2025-12-21
 *
 * License:
 *   SPDX-License-Identifier: MIT
 *   This file is licensed under the MIT License – see the repository LICENSE file
 *   for full terms and copyright information.
 *
 * Change log:
 *   2025-12-21  v0.1  Initial header for uc-Microlab CTC HAL
 *
 */
#include "ctc-hal.h"

void HAL_CTC_Init(uint8_t src, uint8_t ch, uint8_t mode, uint8_t clk)
{
	switch(src)
	{
		case HAL_CTC_SRC_0:
			HAL_CTC_ConfigCH0(ch, mode, clk);
			break;
		case HAL_CTC_SRC_1:
			HAL_CTC_ConfigCH1(ch, mode, clk);
			break;
		case HAL_CTC_SRC_2:
			HAL_CTC_ConfigCH2(ch, mode, clk);
			break;
		default:
			break;
		
	}
	
}

void HAL_CTC_SetValue(uint8_t src, uint8_t ch, uint16_t cmp)
{
	switch(src)
	{
		case HAL_CTC_SRC_0:
			if(ch == HAL_CTC_CH_A) OCR0A = (uint8_t) (0xFF & cmp);
			if(ch == HAL_CTC_CH_B) OCR0B = (uint8_t) (0xFF & cmp);
			break;
		case HAL_CTC_SRC_1:
			if(ch == HAL_CTC_CH_A) OCR1A = cmp;
			if(ch == HAL_CTC_CH_B) OCR1B = cmp;
			break;
		case HAL_CTC_SRC_2:
			if(ch == HAL_CTC_CH_A) OCR2A = (uint8_t) (0xFF & cmp);
			if(ch == HAL_CTC_CH_B) OCR2B = (uint8_t) (0xFF & cmp);
			break;
		default:
			break;
	}
}

void HAL_CTC_ConfigCH0(uint8_t ch, uint8_t mode, uint8_t clk)
{
	TCCR0A = 0;
	TCCR0B = 0;
	
	TCCR0A |= (1 << WGM01);
	TCCR0A |= mode;
	TCCR0B |= clk;	
	
	if(ch == HAL_CTC_CH_A) DDRD   |= (1 << PD6);
	if(ch == HAL_CTC_CH_B) DDRD   |= (1 << PD5);
}

void HAL_CTC_ConfigCH1(uint8_t ch, uint8_t mode, uint8_t clk)
{
	TCCR1A = 0;
	TCCR1B = 0;
	
	TCCR1A |= mode;
	TCCR1B |= (1 << WGM12);
	TCCR1B |= clk;
	
	if(ch == HAL_CTC_CH_A) DDRB   |= (1 << PB1);
	if(ch == HAL_CTC_CH_B) DDRB   |= (1 << PB2);
}

void HAL_CTC_ConfigCH2(uint8_t ch, uint8_t mode, uint8_t clk)
{
	TCCR2A = 0;
	TCCR2B = 0;
	
	TCCR2A |= (1 << WGM21);
	TCCR2A |= mode;
	TCCR2B |= clk;
	
	if(ch == HAL_CTC_CH_A) DDRB   |= (1 << PB3);	
	if(ch == HAL_CTC_CH_B) DDRD   |= (1 << PD3);
}

