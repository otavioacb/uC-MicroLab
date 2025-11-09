/*
 * uc-Microlab — PORT HAL 
 * File: port-hal.h
 *
 * Project: uc-MicroLab
 * Component: PORT Hardware Abstraction Layer (HAL)
 * Hardware: uc-MicroLab board — version r1 (AVR/ATmega) and other platforms
 *
 * Description:
 *   Minimal, portable digital port/pin HAL for small microcontroller projects.
 *   Provides routines to configure pin direction, enable/disable internal
 *   pull-ups, write output levels and read input levels. The API is intentionally
 *   small and hardware-agnostic so it can be used in examples, libraries and
 *   higher-level firmware components.
 *
 * Public API:
 *   void HAL_Port_SetMode(volatile unsigned char* port,
 *                         volatile unsigned char* ddr,
 *                         unsigned char pin,
 *                         unsigned char mode,
 *                         unsigned char pull_up);
 *     Configure a pin's direction (input/output) and the internal pull-up state.
 *
 *   void HAL_Port_Write(volatile unsigned char* port,
 *                       unsigned char pin,
 *                       unsigned char value);
 *     Set the logical level (HIGH/LOW) of an output pin.
 *
 *   unsigned char HAL_Port_Read(volatile unsigned char* pinx,
 *                               unsigned char pin);
 *     Read the logical level of a pin (returns 0 or 1).
 *
 * Public constants:
 *   HAL_PORT_INPUT        - configure pin as input
 *   HAL_PORT_OUTPUT       - configure pin as output
 *   HAL_PORT_DIS_PULLUP   - disable internal pull-up
 *   HAL_PORT_EN_PULLUP    - enable internal pull-up
 *   HAL_PORT_LEVEL_LOW    - logical low (0)
 *   HAL_PORT_LEVEL_HIGH   - logical high (1)
 *
 * Usage:
 *   - Include this header where GPIO access is required:
 *       #include "port-hal.h"
 *   - On AVR/ATmega platforms pass addresses of the PORT, DDR and PIN registers:
 *       HAL_Port_SetMode(&PORTB, &DDRB, PB0, HAL_PORT_OUTPUT, HAL_PORT_DIS_PULLUP);
 *       HAL_Port_Write(&PORTB, PB0, HAL_PORT_LEVEL_HIGH);
 *       unsigned char v = HAL_Port_Read(&PINB, PB0);
 *   - Implementations should ensure proper access to hardware registers and
 *     consider atomic operations (disable interrupts) when modifying shared
 *     port registers in interrupt-driven contexts.
 *
 * Notes:
 *   - Parameters use volatile pointers to allow direct mapping to hardware
 *     registers (e.g. &PORTx, &DDRx, &PINx on AVR).
 *   - This header intentionally avoids heavy dependencies to remain portable.
 *     Platform-specific implementations may provide more efficient inline
 *     versions or optimizations.
 *
 * Author: otavioacb
 * Created: 2025-11-02
 *
 * License:
 *   SPDX-License-Identifier: MIT
 *   This file is licensed under the MIT License — see the repository LICENSE file
 *   for full terms and copyright information.
 *
 * Change log:
 *   2025-11-02  v0.1  Initial header for PORT HAL
 *
 */

#include "port-hal.h"

void HAL_Port_SetMode(volatile unsigned char* port, volatile unsigned char* ddr, unsigned char pin, unsigned char mode, unsigned char pull_up)
{
	if(mode == HAL_PORT_INPUT)
	{
		*ddr  &= ~(1 << pin);
		*port =   (pull_up == HAL_PORT_EN_PULLUP) ? (*port | (1 << pin)) : (*port & ~(1 << pin));
		return;
	}
	
	if(mode == HAL_PORT_OUTPUT)
	{
		*port &= ~(1 << pin);
		*ddr  |=  (1 << pin);
		return;
	}
}

void HAL_Port_Write(volatile unsigned char* port, unsigned char pin, unsigned char value)
{
	*port = (value == HAL_PORT_LEVEL_HIGH) ? (*port | (1 << pin)) : (*port & ~(1 << pin));
}

unsigned char HAL_Port_Read(volatile unsigned char* pinx, unsigned char pin)
{
	return ((*pinx & (1 << pin)) ? HAL_PORT_LEVEL_HIGH : HAL_PORT_LEVEL_LOW);

}
