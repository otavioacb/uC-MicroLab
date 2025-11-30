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

#ifndef PWM_HAL_H
#define PWM_HAL_H

#include <avr/io.h>

#define HAL_PWM_CH0A  0
#define HAL_PWM_CH0B  1
#define HAL_PWM_CH1A  2
#define HAL_PWM_CH1B  3
#define HAL_PWM_CH2A  4
#define HAL_PWM_CH2B  5

#define HAL_PWM_CH0_FAST       0x07
#define HAL_PWM_CH0_PHASE      0x05

#define HAL_PWM_CH1_FAST       0x05
#define HAL_PWM_CH1_PHASE      0x01

#define HAL_PWM_CH2_FAST       0x03
#define HAL_PWM_CH2_PHASE      0x01

#define HAL_PWM_CH0_CK_1       0x01
#define HAL_PWM_CH0_CK_8       0x02
#define HAL_PWM_CH0_CK_64      0x03
#define HAL_PWM_CH0_CK_256     0x04
#define HAL_PWM_CH0_CK_1024    0x05
#define HAL_PWM_CH0_CK_FALL    0x06
#define HAL_PWM_CH0_CK_RISI    0x07

#define HAL_PWM_CH1_CK_1       0x01
#define HAL_PWM_CH1_CK_8       0x02
#define HAL_PWM_CH1_CK_64      0x03
#define HAL_PWM_CH1_CK_256     0x04
#define HAL_PWM_CH1_CK_1024    0x05
#define HAL_PWM_CH1_CK_FALL    0x06
#define HAL_PWM_CH1_CK_RISI    0x07

#define HAL_PWM_CH2_CK_1       0x01
#define HAL_PWM_CH2_CK_8       0x02
#define HAL_PWM_CH2_CK_32      0x03
#define HAL_PWM_CH2_CK_64      0x04
#define HAL_PWM_CH2_CK_128     0x05
#define HAL_PWM_CH2_CK_256     0x06 
#define HAL_PWM_CH2_CK_1024    0x07
 
void HAL_PWM_Init(uint8_t ch, uint8_t mode, uint8_t clock);
void HAL_PWM_SetDutyCycle(uint8_t ch, uint8_t duty);

void HAL_PWM_ConfigCH0(uint8_t mode, uint8_t prescale);
void HAL_PWM_ConfigCH1(uint8_t mode, uint8_t prescale);
void HAL_PWM_ConfigCH2(uint8_t mode, uint8_t prescale);

#endif // PWM_HAL_H
