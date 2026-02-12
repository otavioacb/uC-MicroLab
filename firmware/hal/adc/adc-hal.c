/*
 * uc-Microlab — ADC HAL
 * File: adc-hal.h / adc-hal.c
 *
 * Project: uc-MicroLab
 * Component: ADC Hardware Abstraction Layer (HAL)
 * Hardware: uc-MicroLab board — version r1 (AVR/ATmega) and compatible platforms
 *
 * Description:
 *   Minimal, portable ADC HAL providing configuration and basic conversion
 *   control for AVR ADC peripherals. The header exposes constants for
 *   reference selection, data alignment, input channel selection, prescaler
 *   selection and auto-trigger sources, along with a small API to configure
 *   and use the ADC from application code or higher-level drivers.
 *
 * Public API:
 *   - void HAL_ADC_SetReference(unsigned char ref);
 *       Select ADC voltage reference (AREF, AVCC, Internal).
 *
 *   - void HAL_ADC_SetAdjustment(unsigned char adj);
 *       Configure data adjustment (left/right) for reading ADC result.
 *
 *   - void HAL_ADC_SetChannel(unsigned char ch);
 *       Select ADC input channel.
 *
 *   - void HAL_ADC_SetPrescaler(unsigned char pre);
 *       Set ADC clock prescaler to control ADC conversion speed.
 *
 *   - void HAL_ADC_EnableInterrupt();
 *       Enable ADC conversion complete interrupt.
 *
 *   - void HAL_ADC_StartConversion();
 *       Start a single ADC conversion.
 *
 *   - void HAL_ADC_Enable();
 *       Enable the ADC peripheral.
 *
 *   - void HAL_ADC_StartAutoTrigger(unsigned char src);
 *       Enable auto-triggering and select the trigger source.
 *
 *   - void HAL_ADC_DisableChannel(unsigned char ch);
 *       Disable a specific ADC channel (platform-specific behavior).
 *
 *   - void HAL_ADC_EnableChannel(unsigned char ch);
 *       Enable a specific ADC channel (platform-specific behavior).
 *
 *   - unsigned char HAL_ADC_isRunning();
 *       Return non-zero while a conversion is in progress.
 *
 *   - unsigned char HAL_ADC_Finished();
 *       Return non-zero when a conversion has finished.
 *
 *   - uint16_t HAL_ADC_Read();
 *       Read the last conversion result (10-bit value, adjustments applied).
 *
 * Public constants:
 *   HAL_ADC_REF_MASK   - mask for reference selection bits
 *   HAL_ADC_AREF       - external AREF
 *   HAL_ADC_AVCC       - AVCC as reference
 *   HAL_ADC_INTE       - internal reference (if available)
 *
 *   HAL_ADC_LEFT       - left adjust result
 *   HAL_ADC_RIGHT      - right adjust result
 *
 *   HAL_ADC_CH_XX      - channel selection constants (CH00..CH07, TEMP)
 *
 *   HAL_ADC_DF_*       - prescaler/divisor constants (DF2..DF128)
 *
 *   HAL_ADC_TRSC_*     - auto-trigger source constants (FREE, COMP, EXIN, T0CP, ...)
 *
 * Usage:
 *   - Include this header in modules that require ADC access:
 *       #include "adc-hal.h"
 *   - Typical sequence on AVR:
 *       HAL_ADC_SetReference(HAL_ADC_AVCC);
 *       HAL_ADC_SetAdjustment(HAL_ADC_RIGHT);
 *       HAL_ADC_SetPrescaler(HAL_ADC_DF128);
 *       HAL_ADC_Enable();
 *       HAL_ADC_SetChannel(HAL_ADC_CH00);
 *       HAL_ADC_StartConversion();
 *       while (HAL_ADC_isRunning()) {  }
 *       uint16_t v = HAL_ADC_Read();
 *
 * Notes:
 *   - This header uses <avr/io.h> types and register definitions. Platform-
 *     specific implementations may be required for non-AVR targets.
 *   - Some functions operate on hardware registers and may require atomic
 *     access or interrupt masking in interrupt-driven systems.
 *   - The enable/disable channel functions are provided for boards that
 *     require explicit channel gating; on many AVR devices channel selection
 *     is done only by writing the MUX bits.
 *
 * Author: otavioacb
 * Created: 2025-11-09
 *
 * License:
 *   SPDX-License-Identifier: MIT
 *   This file is licensed under the MIT License — see the repository LICENSE file
 *   for full terms and copyright information.
 *
 * Change log:
 *   2025-11-09  v0.1  Initial header for ADC HAL
 *
 */
#include "adc-hal.h"

void HAL_ADC_SetReference(unsigned char ref)
{
	while(HAL_ADC_isRunning());
	
	ADMUX = (ADMUX & ~HAL_ADC_REF_MASK) | ref;
}

void HAL_ADC_SetAdjustment(unsigned char adj)
{
	while(HAL_ADC_isRunning());

	ADMUX = (ADMUX & ~HAL_ADC_LEFT) | adj;
}

void HAL_ADC_SetChannel(unsigned char ch)
{
	while(HAL_ADC_isRunning());

	ADMUX = (ADMUX & ~HAL_ADC_CH_MASK) | ch;
}

void HAL_ADC_SetPrescaler(unsigned char pre)
{
	ADCSRA = (ADCSRA & ~HAL_ADC_DF_MASK) | pre;	
}

void HAL_ADC_EnableInterrupt()
{
	ADCSRA |= (1 << ADIE);
}


void HAL_ADC_StartConversion()
{
	ADCSRA |= (1 << ADSC);
}

void HAL_ADC_Enable()
{
	ADCSRA |= (1 << ADEN);
}

void HAL_ADC_StartAutoTrigger(unsigned char src)
{	
	ADCSRA |= (1 << ADATE);
	ADCSRB  = (ADCSRB & ~HAL_ADC_TRSC_MASK) | src;	
}

void HAL_ADC_DisableChannel(unsigned char ch)
{
	DIDR0 |= (1 << ch);
}

void HAL_ADC_EnableChannel(unsigned char ch)
{
	DIDR0 &= ~(1 << ch);
}

unsigned char HAL_ADC_isRunning()
{
	return (ADCSRA & (1 << ADSC));
}

unsigned char HAL_ADC_Finished()
{
	return (ADCSRA & (1 << ADIF));
}

uint16_t HAL_ADC_Read()
{
	uint8_t low  = ADCL;
	uint8_t high = ADCH;
	
	if (ADMUX & (1 << ADLAR)) return ((uint16_t)high << 2) | (low >> 6);
	else return ((uint16_t)high << 8) | low;
}
