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
#ifndef CTC_HAL_H_
#define CTC_HAL_H_

#include <avr/io.h>

#define HAL_CTC_CH_A              0x00
#define HAL_CTC_CH_B              0x01

#define HAL_CTC_SRC_0             0x00
#define HAL_CTC_SRC_1             0x01
#define HAL_CTC_SRC_2             0x02

#define HAL_CTCA_COMP_MODE_TOGGLE 0x40
#define HAL_CTCA_COMP_MODE_CLEAR  0x80
#define HAL_CTCA_COMP_MODE_SET    0xC0

#define HAL_CTCB_COMP_MODE_TOGGLE 0x10
#define HAL_CTCB_COMP_MODE_CLEAR  0x20
#define HAL_CTCB_COMP_MODE_SET    0x30

#define HAL_CTC_CH0_CK_1          0x01
#define HAL_CTC_CH0_CK_8          0x02
#define HAL_CTC_CH0_CK_64         0x03
#define HAL_CTC_CH0_CK_256        0x04
#define HAL_CTC_CH0_CK_1024       0x05
#define HAL_CTC_CH0_CK_FALL       0x06
#define HAL_CTC_CH0_CK_RISI       0x07

#define HAL_CTC_CH1_CK_1          0x01
#define HAL_CTC_CH1_CK_8          0x02
#define HAL_CTC_CH1_CK_64         0x03
#define HAL_CTC_CH1_CK_256        0x04
#define HAL_CTC_CH1_CK_1024       0x05
#define HAL_CTC_CH1_CK_FALL       0x06
#define HAL_CTC_CH1_CK_RISI       0x07

#define HAL_CTC_CH2_CK_1          0x01
#define HAL_CTC_CH2_CK_8          0x02
#define HAL_CTC_CH2_CK_32         0x03
#define HAL_CTC_CH2_CK_64         0x04
#define HAL_CTC_CH2_CK_128        0x05
#define HAL_CTC_CH2_CK_256        0x06
#define HAL_CTC_CH2_CK_1024       0x07

void HAL_CTC_Init(uint8_t src, uint8_t ch, uint8_t mode, uint8_t clk);

void HAL_CTC_SetValue(uint8_t src, uint8_t ch, uint16_t cmp);

void HAL_CTC_ConfigCH0(uint8_t ch, uint8_t mode, uint8_t clk);
void HAL_CTC_ConfigCH1(uint8_t ch, uint8_t mode, uint8_t clk);
void HAL_CTC_ConfigCH2(uint8_t ch, uint8_t mode, uint8_t clk);

#endif /* CTC_HAL_H_ */
