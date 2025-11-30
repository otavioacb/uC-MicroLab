/*
 * uc-Microlab — PWM HAL
 * File: pwm-hal.h
 *
 * Project: uc-MicroLab
 * Component: PWM Hardware Abstraction Layer (HAL)
 * Hardware: uc-Microlab board — version r1 (AVR/ATmega) and compatible platforms
 *
 * Description:
 *   Minimal, portable PWM HAL exposing channel identifiers, mode and prescaler
 *   constants, and a small API to configure timers and update PWM duty
 *   cycles. The header is intended to provide a consistent interface for
 *   timers 0, 1 and 2 (OC0A/OC0B, OC1A/OC1B, OC2A/OC2B) on classic AVR
 *   microcontrollers used on the uc-MicroLab board.
 *
 * Public API:
 *   - void HAL_PWM_Init(uint8_t ch, uint8_t mode, uint8_t clock);
 *       High-level initializer: select channel, mode (fast/phase) and clock
 *       prescaler. Delegates to channel-specific configuration functions.
 *
 *   - void HAL_PWM_SetDutyCycle(uint8_t ch, uint8_t duty);
 *       Update the duty cycle for the selected channel. Duty is interpreted
 *       as 0..255 for 8-bit timers. For Timer1 (16-bit) the 8-bit value is
 *       internally scaled to the 16-bit range.
 *
 *   - void HAL_PWM_ConfigCH0(uint8_t mode, uint8_t prescale);
 *       Configure Timer/Counter0 PWM mode and prescaler. Use HAL_PWM_CH0_*
 *       constants for mode and HAL_PWM_CH0_CK_* values for prescale.
 *
 *   - void HAL_PWM_ConfigCH1(uint8_t mode, uint8_t prescale);
 *       Configure Timer/Counter1 PWM mode and prescaler. Use HAL_PWM_CH1_*
 *       constants for mode and HAL_PWM_CH1_CK_* values for prescale.
 *
 *   - void HAL_PWM_ConfigCH2(uint8_t mode, uint8_t prescale);
 *       Configure Timer/Counter2 PWM mode and prescaler. Use HAL_PWM_CH2_*
 *       constants for mode and HAL_PWM_CH2_CK_* values for prescale.
 *
 * Public constants (examples):
 *   - Channel identifiers:
 *       HAL_PWM_CH0A, HAL_PWM_CH0B, HAL_PWM_CH1A, HAL_PWM_CH1B,
 *       HAL_PWM_CH2A, HAL_PWM_CH2B
 *
 *   - Mode selectors:
 *       HAL_PWM_CHx_FAST   - fast PWM mode (per-channel specific encoding)
 *       HAL_PWM_CHx_PHASE  - phase-correct PWM mode (per-channel specific)
 *
 *   - Prescaler encodings:
 *       HAL_PWM_CH0_CK_1, HAL_PWM_CH0_CK_8, HAL_PWM_CH0_CK_64, ...
 *       HAL_PWM_CH1_CK_1, HAL_PWM_CH1_CK_8, ...
 *       HAL_PWM_CH2_CK_1, HAL_PWM_CH2_CK_8, HAL_PWM_CH2_CK_32, ...
 *
 * Usage:
 *   - Include this header in modules that require PWM access:
 *       #include "pwm-hal.h"
 *
 *   - Typical initialization sequence:
 *       HAL_PWM_Init(HAL_PWM_CH0A, HAL_PWM_CH0_FAST, HAL_PWM_CH0_CK_64);
 *       HAL_PWM_SetDutyCycle(HAL_PWM_CH0A, 128); // 50% for 8-bit timer
 *
 * Notes:
 *   - This header uses <avr/io.h> register and bit definitions. On non-AVR
 *     platforms the implementation must provide equivalent register access
 *     or adapt the HAL.
 *   - The HAL focuses on timer configuration and writing to OCRx registers.
 *     Pin direction (DDR) configuration for OCnx pins is expected to be done
 *     by board/platform code when needed.
 *   - Timer1 is 16-bit on common AVR parts; to provide a consistent 0..255
 *     duty interface the implementation may scale the 8-bit duty to 16-bit.
 *   - Some mode/prescaler macros are channel-specific and map to the WGM/
 *     CS bits layout for each timer; consult the device datasheet for exact
 *     behavior and adjust if necessary for your hardware variant.
 *
 * Author: otavioacb
 * Created: 2025-11-30
 *
 * License:
 *   SPDX-License-Identifier: MIT
 *   This file is licensed under the MIT License — see the repository LICENSE file
 *   for full terms and copyright information.
 *
 * Change log:
 *   2025-11-30  v0.1  Initial header for PWM HAL
 *
 */

#include "pwm-hal.h"

void HAL_PWM_Init(uint8_t ch, uint8_t mode, uint8_t prescale)
{
	switch(ch)
	{
		case HAL_PWM_CH0A:
			DDRD |= (1 << PD6);
			HAL_PWM_ConfigCH0(mode, prescale);
			break;
		case HAL_PWM_CH0B:
			DDRD |= (1 << PD5);
			HAL_PWM_ConfigCH0(mode, prescale);
			break;
		case HAL_PWM_CH1A:
			DDRB |= (1 << PB1);
			HAL_PWM_ConfigCH1(mode, prescale);
			break;
		case HAL_PWM_CH1B:
			DDRB |= (1 << PB2);
			HAL_PWM_ConfigCH1(mode, prescale);
			break;
		case HAL_PWM_CH2A:
			DDRB |= (1 << PB3);
			HAL_PWM_ConfigCH2(mode, prescale);
			break;
		case HAL_PWM_CH2B:
			DDRD |= (1 << PD3);
			HAL_PWM_ConfigCH2(mode, prescale);
			break;
		default:
			break;
	}
}

void HAL_PWM_SetDutyCycle(uint8_t ch, uint8_t duty)
{
	if (duty > 100) duty = 100;
	
	switch(ch)
	{
		case HAL_PWM_CH0A:
			OCR0A = (uint8_t)((uint16_t)duty * 255u / 100u);
			break;
		case HAL_PWM_CH0B:
			OCR0B = (uint8_t)((uint16_t)duty * 255u / 100u);
			break;
		case HAL_PWM_CH1A:
			OCR1A = (uint16_t)((uint32_t)duty * 65535u / 100u);
			break;
		case HAL_PWM_CH1B:
			OCR1B = (uint16_t)((uint32_t)duty * 65535u / 100u);
			break;
		case HAL_PWM_CH2A:
			OCR2A = (uint8_t)((uint16_t)duty * 255u / 100u);
			break;
		case HAL_PWM_CH2B:
			OCR2B = (uint8_t)((uint16_t)duty * 255u / 100u);
			break;
		default:
			break;
	}
}

void HAL_PWM_ConfigCH0(uint8_t mode, uint8_t prescale)
{
	TCCR0A |=  (mode & 0x03);
	
	if(mode & 0x04) TCCR0B |= (1 << WGM02);

	TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
	TCCR0B |= (prescale & 0x07);

	TCNT0 = 0;
}

void HAL_PWM_ConfigCH1(uint8_t mode, uint8_t prescale)
{
	TCCR1A |= (mode & 0x03);
	
	if(mode & 0x04) TCCR1B |= (1 << WGM12);
	
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1);
	TCCR1B |= (prescale & 0x07);
	
	TCNT1   = 0;
}

void HAL_PWM_ConfigCH2(uint8_t mode, uint8_t prescale)
{
	TCCR2A |=  (mode & 0x03);
	
	if(mode & 0x04) TCCR2B |= (1 << WGM22);

	TCCR2A |= (1 << COM2A1) | (1 << COM2B1);
	TCCR2B |= (prescale & 0x07);

	TCNT2 = 0;
}
