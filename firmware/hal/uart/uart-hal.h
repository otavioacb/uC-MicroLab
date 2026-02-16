/*
 * uc-Microlab — UART HAL
 * File: uart-hal.h / uart-hal.c
 *
 * Project: uc-Microlab
 * Component: UART Hardware Abstraction Layer (HAL)
 * Hardware: uc-Microlab board — version r1
 *
 * Description:
 *   Minimal, portable UART HAL for AVR (ATmega) and other platforms.
 *   Provides initialization and transmit/receive primitives and a small,
 *   well-documented API surface for embedding in examples and libraries.
 *
 * Public API (examples of functions that should be declared/implemented):
 *   void HAL_UART_Init(unsigned int baud_rate);
 *   void HAL_UART_Transmit(unsigned char data);
 *   void HAL_UART_Send(unsigned char* msg, size_t len_msg);
 *   unsigned char HAL_UART_Receive(void);
 *   unsigned char HAL_UART_Read(unsigned char* buf, size_t len);
 *   unsigned char HAL_UART_Available();
 *   void HAL_UART_Flush(void);
 *
 * Usage:
 *   - Include the header in your sources: #include "uart-hal.h"
 *   - Call HAL_UART_Init(F_CPU/baud) once during startup, then use transmit/receive.
 *   - The implementation file (uart-hal.c or platform-specific file) must be
 *     added to the project build so the linker can find the symbols.
 *
 * Notes:
 *   - This comment block is intended to live at the top of uart-hal.h and/or
 *     uart-hal.c to identify the file in the uc-Microlab repository.
 *   - For AVR implementations, ensure F_CPU is defined before including
 *     <util/delay.h> or set F_CPU in the project compiler symbols.
 *
 * Author: otavioacb
 * Created: 2025-10-19
 *
 * License:
 *   SPDX-License-Identifier: MIT
 *   This file is licensed under the MIT License — see the repository LICENSE file
 *   for full terms and copyright information.
 *
 * Change log:
 *   2025-10-19  v0.1  Initial header for uc-Microlab UART HAL
 *   2026-02-16  v0.2  Added receive functions: HAL_UART_Receive, HAL_UART_Read, HAL_UART_Available, HAL_UART_Flush
 *                     Added inline comments for better code documentation
 *
 */

#ifndef UART_HAL_H
#define UART_HAL_H

#include <stddef.h>

void HAL_UART_Init(unsigned int baud_rate);

void HAL_UART_Transmit(unsigned char data);
void HAL_UART_Send(unsigned char* msg, size_t len_msg);

unsigned char HAL_UART_Receive(void);
unsigned char HAL_UART_Read(unsigned char* buf, size_t len);

unsigned char HAL_UART_Available();

void HAL_UART_Flush(void);

#endif
