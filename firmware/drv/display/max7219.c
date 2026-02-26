/*
 * uc-Microlab — MAX7219 LED Display Driver (header)
 * File: max7219.h / max7219.c
 *
 * Project: uc-MicroLab
 * Component: MAX7219 Serially Interfaced 8-Digit LED Display Driver
 * Hardware: uc-MicroLab board — version r1
 *
 * Description:
 *   Driver for the MAX7219 serially interfaced, 8-digit, 7-segment LED
 *   display driver. Provides routines to initialize and control the MAX7219
 *   over SPI, including digit decoding, scan-limit configuration, intensity
 *   control, display test, shutdown mode, and raw digit/data writes.
 *   Built on top of the uc-MicroLab SPI HAL (spi-hal.h).
 *
 * Public API:
 *   void MAX_Init(void);
 *     Initialize the MAX7219. Configures the SPI peripheral with mode 1,
 *     MSB-first, SPI format 0, and clock divider 2.
 *
 *   void MAX_ShutdownMode(void);
 *     Place the MAX7219 in shutdown (low-power) mode. The display is blanked
 *     and the device draws minimal current. Register contents are preserved.
 *
 *   void MAX_NormalOperation(void);
 *     Return the MAX7219 to normal operation from shutdown mode.
 *
 *   void MAX_DecodeMode(unsigned char digits);
 *     Configure the BCD decode mode for each digit position. Each bit in
 *     the digits byte corresponds to one digit (bit 0 = digit 0, etc.).
 *     Use the MAX_DEC_* constants for convenience.
 *     Note: In decode mode, each digit register expects a left-shifted
 *     value (BCD code B). In no-decode mode, the register value controls
 *     individual segments directly.
 *
 *   void MAX_ScanDigits(unsigned char digits);
 *     Set the scan-limit register to control how many digits are displayed
 *     (0x00 = digit 0 only, 0x07 = digits 0 through 7).
 *     Warning: this register sets the number of scanned digits and affects
 *     per-digit current. It must NOT be used to blank portions of the
 *     display.
 *
 *   void MAX_StartTest(void);
 *     Enable display test mode. All segments of all digits are turned on
 *     at maximum intensity regardless of other register settings.
 *
 *   void MAX_StopTest(void);
 *     Disable display test mode and return to normal operation.
 *
 *   void MAX_SetIntensity(unsigned char intensity);
 *     Set the display intensity (brightness) using the internal pulse-width
 *     modulator. Valid range is 0x00 (minimum) to 0x0F (maximum). The PWM
 *     duty cycle steps are: 1/32, 3/32, 5/32, ... 31/32.
 *
 *   void MAX_WriteDigit(unsigned char digit, unsigned char value);
 *     Write a value directly to a single digit register. The digit parameter
 *     must be in the range 0x01 (digit 0) to 0x08 (digit 7). The value
 *     meaning depends on the decode mode configured for that digit.
 *
 *   void MAX_SendData(unsigned char *values);
 *     Write values to all 8 digit registers in a single call. The values
 *     array must hold at least 9 elements; index 0 is ignored and indices
 *     1 through 8 map to digits 0 through 7.
 *
 * Internal API (use with caution):
 *   void _MAX_WriteCMD(unsigned char *buf);
 *     Transmit a 2-byte command frame (register address + data) over SPI.
 *     This is an internal function used by all public API routines. It may
 *     be called directly only if the full command frame is known.
 *
 * Public constants:
 *   Command register addresses:
 *     MAX_CMD_NOOP - No-op register          (0x00)
 *     MAX_CMD_DECM - Decode mode register    (0x09)
 *     MAX_CMD_INTE - Intensity register      (0x0A)
 *     MAX_CMD_SCAN - Scan-limit register     (0x0B)
 *     MAX_CMD_LOWP - Shutdown register       (0x0C)
 *     MAX_CMD_TEST - Display test register   (0x0F)
 *
 *   Command frame length:
 *     MAX_CMD_LEN  - SPI frame length in bytes (0x02)
 *
 *   Decode mode masks (for MAX_DecodeMode):
 *     MAX_DEC_NOD  - No decode for all digits         (0x00)
 *     MAX_DEC_D00  - Decode digit 0 only              (0x01)
 *     MAX_DEC_D01  - Decode digits 1 to 0             (0x03)
 *     MAX_DEC_D02  - Decode digits 2 to 0             (0x07)
 *     MAX_DEC_D03  - Decode digits 3 to 0             (0x0F)
 *     MAX_DEC_D04  - Decode digits 4 to 0             (0x1F)
 *     MAX_DEC_D05  - Decode digits 5 to 0             (0x3F)
 *     MAX_DEC_D06  - Decode digits 6 to 0             (0x7F)
 *     MAX_DEC_D07  - Decode digits 7 to 0 (all)       (0xFF)
 *
 * Usage:
 *   - Include this header where MAX7219 access is required:
 *       #include "max7219.h"
 *
 *   - Initialize the MAX7219:
 *       MAX_Init();
 *
 *   - Wake up and configure the display:
 *       MAX_NormalOperation();
 *       MAX_DecodeMode(MAX_DEC_D07);
 *       MAX_ScanDigits(0x07);
 *       MAX_SetIntensity(0x08);
 *
 *   - Write the value 3 to digit 0:
 *       MAX_WriteDigit(0x01, 0x03);
 *
 *   - Write values to all digits at once:
 *       unsigned char data[9] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
 *       MAX_SendData(data);
 *
 *   - Run a display self-test:
 *       MAX_StartTest();
 *       // ... delay ...
 *       MAX_StopTest();
 *
 *   - Enter low-power mode:
 *       MAX_ShutdownMode();
 *
 * Notes:
 *   - The MAX7219 uses a 16-bit SPI frame: the upper byte is the register
 *     address and the lower byte is the data. Only bits [11:8] of the
 *     address are decoded; bits [15:12] are ignored.
 *   - The SPI clock polarity and phase must match MAX7219 requirements
 *     (CPOL=0, CPHA=0 — SPI Mode 0). Verify that HAL_SPI_FORMAT0 maps
 *     to this configuration in spi-hal.h.
 *   - Daisy-chaining multiple MAX7219 devices is not supported by this
 *     driver in its current form. Each device requires its own CS line and
 *     separate initialization.
 *   - The scan-limit register affects per-digit peak current. Consult the
 *     MAX7219 datasheet (Table 8) when fewer than 8 digits are enabled.
 *   - Intensity steps follow odd PWM duty cycles: 1/32, 3/32, ... 31/32.
 *     Setting 0x00 does NOT blank the display; use MAX_ShutdownMode() for
 *     that purpose.
 *   - All SPI transfers are blocking (the underlying HAL uses polling).
 *
 * Author: otavioacb
 * Created: 2026-02-26
 *
 * License:
 *   SPDX-License-Identifier: MIT
 *   This file is licensed under the MIT License — see the repository LICENSE
 *   file for full terms and copyright information.
 *
 * Change log:
 *   2026-02-26  v0.1  Initial header for MAX7219 LED display driver
 *
 */

#include "max7219.h"

void MAX_Init()
{
	HAL_SPI_Init(HAL_SPI_MD1, HAL_SPI_DT_MSB, HAL_SPI_FORMAT0, HAL_SPI_CK_2);
}

void MAX_ShutdownMode()
{
	unsigned char cmd[2] = {MAX_CMD_LOWP, 0x00};
	
	_MAX_WriteCMD(cmd);
}

void MAX_NormalOperation()
{
	unsigned char cmd[2] = {MAX_CMD_LOWP, 0x01};
		
	_MAX_WriteCMD(cmd);
}

/*
 * In Decode Mode, each digit need to be left shifted 
 * to a 8-bit data. So, the range of digits are:
 * No decode - 0x00
 * Decode Digit 0 - 0x01
 * Decode Digits 1 to 0 - 0x03
 * Decode Digits 2 to 0 - 0x07
 * Decode Digits 3 to 0 - 0x0F
 * Decode Digits 4 to 0 - 0x1F
 * Decode Digits 5 to 0 - 0x3F
 * Decode Digits 6 to 0 - 0x7F
 * Decode Digits 7 to 0 - 0xFF
 */
void MAX_DecodeMode(unsigned char digits)
{
	unsigned char cmd[2] = {MAX_CMD_DECM, digits};
	
	_MAX_WriteCMD(cmd);
}

/*
 * In Scan Mode, the range limits are 0x00 and 
 * 0x07. To enable all digits send 0x07. To disable
 * all digits send 0x00. 
 *
 * Caution: Scan-limit register should not be used
 * to blank portions of the display!!!
 */
void MAX_ScanDigits(unsigned char digits)
{
	unsigned char cmd[2] = {MAX_CMD_SCAN, digits};
	
	_MAX_WriteCMD(cmd);
}

void MAX_StartTest()
{
	unsigned char cmd[2] = {MAX_CMD_TEST, 0x01};
		
	_MAX_WriteCMD(cmd);
}

void MAX_StopTest()
{
	unsigned char cmd[2] = {MAX_CMD_TEST, 0x00};
	
	_MAX_WriteCMD(cmd);
}

/*
 * To MAX7219, the intesity is regulated by
 * a duty cycle of an internal modulator. 
 * The steps are prime numbers starting at 
 * 3 and going to 31 divided in a range of
 * 0x00 to 0x0F numbers.
 */
void MAX_SetIntesity(unsigned char intesity)
{
	unsigned char cmd[2] = {MAX_CMD_INTE, intesity};
		
	_MAX_WriteCMD(cmd);
}

/*
 * Send a byte that represent a value to the
 * specific digit.
 */
void MAX_WriteDigit(unsigned char digit, unsigned char value)
{
	unsigned char cmd[2] = {digit, value};
	
	_MAX_WriteCMD(cmd);
}

/*
 * To write different patterns you need
 * to send a buffer with 9 bytes to be send
 * for each byte a digit pattern to be write.
 */
void MAX_SendData(unsigned char* values)
{
	for(int i = 1; i < 9; ++i) MAX_WriteDigit((unsigned char) i, values[i]);
}

/*
 * Attention, this function is used as internal 
 * function of this library. Use it alone if you
 * know what you are doing...
 */
void _MAX_WriteCMD(unsigned char* buf)
{
	HAL_SPI_Send(buf, MAX_CMD_LEN);
}
