/*
 * uc-Microlab — EEPROM HAL
 * File: eeprom-hal.h / eeprom-hal.c
 *
 * Project: uc-MicroLab
 * Component: EEPROM Hardware Abstraction Layer (HAL)
 * Hardware: uc-MicroLab board — version r1 (AVR/ATmega) and compatible platforms
 *
 * Description:
 *   Minimal, portable EEPROM HAL providing read and write access to non-volatile
 *   memory on AVR microcontrollers. The header exposes a simple API for single-byte
 *   and multi-byte operations, enabling application code and higher-level drivers
 *   to store and retrieve persistent configuration data, calibration values, and
 *   user settings across power cycles.
 *
 * Public API:
 *   - void HAL_EEPROM_Write(unsigned int addr, unsigned char data);
 *       Write a single byte to the specified EEPROM address.
 *
 *   - unsigned int HAL_EEPROM_Save(unsigned int addr, unsigned char* data, size_t len);
 *       Write multiple bytes (buffer) to EEPROM starting at the specified address.
 *       Returns the next available address after the written data.
 *
 *   - unsigned char HAL_EEPROM_Read(unsigned int addr);
 *       Read a single byte from the specified EEPROM address.
 *
 *   - void HAL_EEPROM_Get(unsigned int addr, unsigned char* data, size_t len);
 *       Read multiple bytes from EEPROM starting at the specified address into
 *       the provided buffer.
 *
 * Usage:
 *   - Include this header in modules that require EEPROM access:
 *       #include "eeprom-hal.h"
 *   - Write a single byte:
 *       HAL_EEPROM_Write(0x0010, 0xAB);
 *   - Write a buffer:
 *       unsigned char buffer[4] = {0x01, 0x02, 0x03, 0x04};
 *       unsigned int next_addr = HAL_EEPROM_Save(0x0020, buffer, 4);
 *   - Read a single byte:
 *       unsigned char value = HAL_EEPROM_Read(0x0010);
 *   - Read a buffer:
 *       unsigned char buffer[4];
 *       HAL_EEPROM_Get(0x0020, buffer, 4);
 *
 * Notes:
 *   - EEPROM writes are typically slow (several milliseconds per byte on AVR).
 *     Ensure proper timing or use interrupt-driven approaches for time-critical
 *     applications.
 *   - EEPROM has a limited write endurance (typically ~100,000 cycles). Avoid
 *     excessive writes to the same address.
 *   - Address range is device-specific. Consult the target MCU datasheet for
 *     EEPROM size and valid address ranges.
 *   - Functions may block or wait for EEPROM ready status. Consider disabling
 *     interrupts or using appropriate synchronization in multi-threaded or
 *     interrupt-driven contexts.
 *   - This header uses <avr/io.h> types and register definitions. Platform-
 *     specific implementations may be required for non-AVR targets.
 *
 * Author: otavioacb
 * Created: 2026-02-12
 *
 * License:
 *   SPDX-License-Identifier: MIT
 *   This file is licensed under the MIT License — see the repository LICENSE file
 *   for full terms and copyright information.
 *
 * Change log:
 *   2026-02-12  v0.1  Initial header for EEPROM HAL
 *
 */

#include "eeprom-hal.h"

void HAL_EEPROM_Write(unsigned int addr, unsigned char data)
{
	while(EECR & (1 << EEPE));
	EEAR = addr;
	
	EEDR = data;
	EECR |= (1 << EEMPE);
	EECR |= (1 << EEPE);	
}

unsigned int HAL_EEPROM_Save(unsigned int addr, unsigned char* data, size_t len)
{
	for(int i = 0; i < len; ++i) HAL_EEPROM_Write(addr++, data[i]);
	
	return addr - 1;
}

unsigned char HAL_EEPROM_Read(unsigned int addr)
{
	while(EECR & (1 << EEPE));
	
	EEAR = addr;
	EECR |= (1 << EERE);
	
	return EEDR;
}


void HAL_EEPROM_Get(unsigned int addr, unsigned char* data, size_t len)
{
	for(int i = 0; i < len; i++) data[i] = HAL_EEPROM_Read(addr++);
}
