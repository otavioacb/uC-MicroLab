/*
 * uc-Microlab — ADC HAL
 * File: adc-hal.h
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
 *       while (HAL_ADC_isRunning()) { wait  }
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

#ifndef ADC_HAL_H
#define ADC_HAL_H

#include <avr/io.h>

#define HAL_ADC_REF_MASK 0xC0
#define HAL_ADC_AREF  0x00
#define HAL_ADC_AVCC  0x40
#define HAL_ADC_INTE  0xC0

#define HAL_ADC_LEFT  0x20
#define HAL_ADC_RIGHT 0x00

#define HAL_ADC_CH_MASK 0x0F
#define HAL_ADC_CH00  0x00
#define HAL_ADC_CH01  0x01
#define HAL_ADC_CH02  0x02
#define HAL_ADC_CH03  0x03
#define HAL_ADC_CH04  0x04
#define HAL_ADC_CH05  0x05
#define HAL_ADC_CH06  0x06
#define HAL_ADC_CH07  0x07
#define HAL_ADC_TEMP  0x08

#define HAL_ADC_DF_MASK 0x07
#define HAL_ADC_DF2    0x01
#define HAL_ADC_DF4    0x02
#define HAL_ADC_DF8    0x03
#define HAL_ADC_DF16   0x04
#define HAL_ADC_DF32   0x05
#define HAL_ADC_DF64   0x06
#define HAL_ADC_DF128  0x07

#define HAL_ADC_TRSC_MASK 0x07
#define HAL_ADC_TRSC_FREE 0x00
#define HAL_ADC_TRSC_COMP 0x01
#define HAL_ADC_TRSC_EXIN 0x02
#define HAL_ADC_TRSC_T0CP 0x03
#define HAL_ADC_TRSC_T0OV 0x04
#define HAL_ADC_TRSC_T1CP 0x05
#define HAL_ADC_TRSC_T1OV 0x06
#define HAL_ADC_TRSC_T1EV 0x07

void HAL_ADC_SetReference(unsigned char ref);
void HAL_ADC_SetAdjustment(unsigned char adj);
void HAL_ADC_SetChannel(unsigned char ch);
void HAL_ADC_SetPrescaler(unsigned char pre);

void HAL_ADC_EnableInterrupt();
void HAL_ADC_StartConversion();
void HAL_ADC_Enable();
void HAL_ADC_StartAutoTrigger(unsigned char src);
void HAL_ADC_DisableChannel(unsigned char ch);
void HAL_ADC_EnableChannel(unsigned char ch);

unsigned char HAL_ADC_isRunning();
unsigned char HAL_ADC_Finished();
uint16_t HAL_ADC_Read();

#endif // ADC_HAL_H
