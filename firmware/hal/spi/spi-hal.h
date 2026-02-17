/*
 * uc-Microlab — SPI HAL (header comment)
 * File: spi-hal.h / spi-hal.c
 *
 * Project: uc-MicroLab
 * Component: SPI Hardware Abstraction Layer (HAL)
 * Hardware: uc-MicroLab board — version r1 
 *
 * Description:
 *   Minimal, portable SPI HAL for small microcontroller projects.
 *   Provides routines to configure and operate the SPI peripheral in both
 *   master and slave modes. Supports configuration of clock rate, data order,
 *   and SPI formats (clock polarity and phase). The API is intentionally
 *   small and hardware-agnostic so it can be used in examples, libraries and
 *   higher-level firmware components.
 *
 * Public API:
 *   void HAL_SPI_Init(unsigned char mode,
 *                     unsigned char order,
 *                     unsigned char ck,
 *                     unsigned char format);
 *     Initialize the SPI peripheral with specified mode, data order, clock
 *     divider and format (CPOL/CPHA combination).
 *
 *   void HAL_SPI_SetClock(unsigned char ck);
 *     Change the SPI clock rate divider.
 *
 *   void HAL_SPI_SetOrder(unsigned char order);
 *     Set data transmission order (MSB-first or LSB-first).
 *
 *   void HAL_SPI_SetFormat(unsigned char format);
 *     Set SPI clock polarity and phase (format 0-3).
 *
 *   void HAL_SPI_Write(unsigned char data);
 *     Transmit a single byte over SPI.
 *
 *   void HAL_SPI_Send(unsigned char* data, size_t len);
 *     Transmit multiple bytes over SPI.
 *
 *   unsigned char HAL_SPI_Read();
 *     Receive a single byte from SPI.
 *
 *   void HAL_SPI_Receive(unsigned char* buf, size_t len);
 *     Receive multiple bytes from SPI into buffer.
 *
 * Public constants:
 *   SPI Formats (CPOL/CPHA combinations):
 *     HAL_SPI_FORMAT0 - CPOL=0, CPHA=0 (sample on rising edge, setup on falling edge)
 *     HAL_SPI_FORMAT1 - CPOL=0, CPHA=1 (setup on rising edge, sample on falling edge)
 *     HAL_SPI_FORMAT2 - CPOL=1, CPHA=0 (sample on falling edge, setup on rising edge)
 *     HAL_SPI_FORMAT3 - CPOL=1, CPHA=1 (setup on falling edge, sample on rising edge)
 *
 *   Data order:
 *     HAL_SPI_DT_MSB  - transmit most significant bit first
 *     HAL_SPI_DT_LSB  - transmit least significant bit first
 *
 *   Clock dividers (relative to system clock):
 *     HAL_SPI_CK_2    - divide by 2 (fastest)
 *     HAL_SPI_CK_4    - divide by 4
 *     HAL_SPI_CK_8    - divide by 8
 *     HAL_SPI_CK_16   - divide by 16
 *     HAL_SPI_CK_32   - divide by 32
 *     HAL_SPI_CK_64   - divide by 64
 *     HAL_SPI_CK_128  - divide by 128 (slowest)
 *
 *   Operating modes:
 *     HAL_SPI_MD0     - slave or peripheral mode
 *     HAL_SPI_MD1     - master or controller mode
 *
 * Usage:
 *   - Include this header where SPI access is required:
 *       #include "spi-hal.h"
 *   - Initialize SPI in master mode, MSB-first, clock/16, format 0:
 *       HAL_SPI_Init(HAL_SPI_MD1, HAL_SPI_DT_MSB, HAL_SPI_CK_16, HAL_SPI_FORMAT0);
 *   - Transmit a byte:
 *       HAL_SPI_Write(0xAA);
 *   - Transmit multiple bytes:
 *       unsigned char buf[] = {0x01, 0x02, 0x03};
 *       HAL_SPI_Send(buf, 3);
 *   - Receive a byte:
 *       unsigned char data = HAL_SPI_Read();
 *   - Implementations should ensure proper configuration of SPI hardware
 *     registers and handle chip select (SS/CS) management at a higher level.
 *
 * Notes:
 *   - This HAL does not manage chip select (CS/SS) pins — that responsibility
 *     belongs to the application or a higher-level driver.
 *   - Blocking operations are used; consider interrupt-driven or DMA transfers
 *     for performance-critical applications.
 *
 * Author: otavioacb
 * Created: 2026-02-17
 *
 * License:
 *   SPDX-License-Identifier: MIT
 *   This file is licensed under the MIT License — see the repository LICENSE file
 *   for full terms and copyright information.
 *
 * Change log:
 *   2026-02-17  v0.1  Initial header for SPI HAL
 *
 */


#ifndef SPI_HAL_H_
#define SPI_HAL_H_

#include <avr/io.h>
#include <stddef.h>

#define HAL_SPI_FORMAT0 0x00
#define HAL_SPI_FORMAT1 0x01
#define HAL_SPI_FORMAT2 0x02
#define HAL_SPI_FORMAT3 0x03

#define HAL_SPI_DT_MSB  0x00
#define HAL_SPI_DT_LSB  0x01

#define HAL_SPI_CK_4    0x00
#define HAL_SPI_CK_16   0x01
#define HAL_SPI_CK_64   0x02
#define HAL_SPI_CK_128  0x03
#define HAL_SPI_CK_2    0x04
#define HAL_SPI_CK_8    0x05
#define HAL_SPI_CK_32   0x06

#define HAL_SPI_MD0     0x00 // Slave or Peripheral Mode
#define HAL_SPI_MD1     0x01 // Master or Controller Mode

void HAL_SPI_Init(unsigned char mode, unsigned char order, unsigned char ck, unsigned char format);

void HAL_SPI_SetClock(unsigned char ck);
void HAL_SPI_SetOrder(unsigned char order);
void HAL_SPI_SetFormat(unsigned char format);

void HAL_SPI_Write(unsigned char data);
void HAL_SPI_Send(unsigned char* data, size_t len);

unsigned char HAL_SPI_Read();
void HAL_SPI_Receive(unsigned char* buf, size_t len);

#endif /* SPI-HAL_H_ */
