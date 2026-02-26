/*
 * uc-Microlab — DS3231 RTC Driver (header)
 * File: ds3231.h / ds3231.c
 *
 * Project: uc-MicroLab
 * Component: DS3231 Real-Time Clock Driver
 * Hardware: uc-MicroLab board — version r1
 *
 * Description:
 *   Driver for the DS3231 high-accuracy I2C real-time clock (RTC).
 *   Provides routines to configure and operate the DS3231, including
 *   timekeeping, dual alarms, square-wave output, 32 kHz output,
 *   temperature reading, aging offset adjustment, and oscillator control.
 *   Built on top of the uc-MicroLab I2C HAL (i2c-hal.h).
 *
 * Public API:
 *   void DS3231_Init(void);
 *     Initialize the DS3231. Configures the I2C bus at 400 kHz and
 *     clears the EOSC bit in the control register to ensure the
 *     oscillator is running.
 *
 *   void DS3231_SetTime(DS3231_Datetime_t *time);
 *     Write the date and time to the DS3231 timekeeping registers.
 *     The century bit (bit 7 of the month register) is set automatically
 *     when year >= 2100.
 *
 *   void DS3231_GetTime(DS3231_Datetime_t *time);
 *     Read the current date and time from the DS3231 timekeeping registers
 *     and store the decoded values in the provided structure.
 *
 *   void DS3231_SetAlarm1(DS3231_Datetime_t *time, uint8_t mode);
 *     Configure and enable Alarm 1. The mode parameter selects one of the
 *     six alarm match conditions defined by DS3231_ALM1_* constants.
 *
 *   void DS3231_DisableAlarm1(void);
 *     Disable Alarm 1 interrupt by clearing the A1IE bit in the control
 *     register.
 *
 *   void DS3231_EnableAlarm1(void);
 *     Enable Alarm 1 interrupt by setting the A1IE bit in the control
 *     register.
 *
 *   void DS3231_SetAlarm2(DS3231_Datetime_t *time, uint8_t mode);
 *     Configure and enable Alarm 2. The mode parameter selects one of the
 *     five alarm match conditions defined by DS3231_ALM2_* constants.
 *
 *   void DS3231_DisableAlarm2(void);
 *     Disable Alarm 2 interrupt by clearing the A2IE bit in the control
 *     register.
 *
 *   void DS3231_EnableAlarm2(void);
 *     Enable Alarm 2 interrupt by setting the A2IE bit in the control
 *     register.
 *
 *   float DS3231_GetTemp(void);
 *     Read the internal temperature sensor. Returns the temperature in
 *     degrees Celsius with 0.25 °C resolution.
 *
 *   void DS3231_SetSQWFreq(uint8_t freq);
 *     Set the square-wave output frequency using one of the DS3231_SQW_*
 *     constants (1 Hz, 1.024 kHz, 4.096 kHz, or 8.192 kHz).
 *
 *   void DS3231_EnableSQW(void);
 *     Enable the square-wave output on the INT/SQW pin by setting the
 *     INTCN bit in the control register.
 *
 *   void DS3231_DisableSQW(void);
 *     Disable the square-wave output on the INT/SQW pin by clearing the
 *     INTCN bit in the control register.
 *
 *   void DS3231_Enable32khz(void);
 *     Enable the 32 kHz output pin by setting the EN32KHZ bit in the
 *     status register.
 *
 *   void DS3231_Disable32khz(void);
 *     Disable the 32 kHz output pin by clearing the EN32KHZ bit in the
 *     status register.
 *
 *   void DS3231_EnableOSC(void);
 *     Enable the oscillator by clearing the EOSC bit in the control
 *     register (active-low logic).
 *
 *   uint8_t DS3231_IsOSCStopped(void);
 *     Check the Oscillator Stop Flag (OSF) in the status register.
 *     Returns 1 if the oscillator has stopped or was stopped since the
 *     last power cycle; returns 0 otherwise.
 *
 *   void DS3231_SetAging(int8_t offset);
 *     Write a signed offset value to the aging register to fine-tune
 *     the oscillator frequency.
 *
 *   int8_t DS3231_GetAging(void);
 *     Read and return the current aging offset register value as a
 *     signed 8-bit integer.
 *
 * Public types:
 *   DS3231_Datetime_t
 *     Structure holding a complete date and time:
 *       uint8_t  sec   — seconds       (0–59)
 *       uint8_t  min   — minutes       (0–59)
 *       uint8_t  hour  — hours         (0–23)
 *       uint8_t  day   — day of week   (1–7)
 *       uint8_t  date  — day of month  (1–31)
 *       uint8_t  month — month         (1–12)
 *       uint16_t year  — full year     (2000–2199)
 *
 * Public constants:
 *   Device address:
 *     DS3231_ADDR             - 7-bit I2C address (0x68)
 *
 *   Register map:
 *     DS3231_REG_SECONDS      - Seconds register         (0x00)
 *     DS3231_REG_MINUTES      - Minutes register         (0x01)
 *     DS3231_REG_HOURS        - Hours register           (0x02)
 *     DS3231_REG_DAY          - Day-of-week register     (0x03)
 *     DS3231_REG_DATE         - Date register            (0x04)
 *     DS3231_REG_MONTH        - Month/century register   (0x05)
 *     DS3231_REG_YEAR         - Year register            (0x06)
 *     DS3231_REG_ALM1_SEC     - Alarm 1 seconds          (0x07)
 *     DS3231_REG_ALM1_MIN     - Alarm 1 minutes          (0x08)
 *     DS3231_REG_ALM1_HOUR   - Alarm 1 hours            (0x09)
 *     DS3231_REG_ALM1_DAY     - Alarm 1 day/date         (0x0A)
 *     DS3231_REG_ALM2_MIN     - Alarm 2 minutes          (0x0B)
 *     DS3231_REG_ALM2_HOUR   - Alarm 2 hours            (0x0C)
 *     DS3231_REG_ALM2_DAY     - Alarm 2 day/date         (0x0D)
 *     DS3231_REG_CONTROL      - Control register         (0x0E)
 *     DS3231_REG_STATUS       - Status register          (0x0F)
 *     DS3231_REG_AGING        - Aging offset register    (0x10)
 *     DS3231_REG_TEMP_MSB     - Temperature MSB register (0x11)
 *     DS3231_REG_TEMP_LSB     - Temperature LSB register (0x12)
 *
 *   Control register bits (DS3231_REG_CONTROL):
 *     DS3231_CTRL_A1IE        - Alarm 1 interrupt enable (bit 0)
 *     DS3231_CTRL_A2IE        - Alarm 2 interrupt enable (bit 1)
 *     DS3231_CTRL_INTCN       - Interrupt/SQW control    (bit 2)
 *     DS3231_CTRL_RS1         - Rate select 1            (bit 3)
 *     DS3231_CTRL_RS2         - Rate select 2            (bit 4)
 *     DS3231_CTRL_CONV        - Temperature conversion   (bit 5)
 *     DS3231_CTRL_BBSQW       - Battery-backed SQW       (bit 6)
 *     DS3231_CTRL_EOSC        - Enable oscillator (active-low) (bit 7)
 *
 *   Status register bits (DS3231_REG_STATUS):
 *     DS3231_STAT_A1F         - Alarm 1 flag             (bit 0)
 *     DS3231_STAT_A2F         - Alarm 2 flag             (bit 1)
 *     DS3231_STAT_BSY         - Device busy flag         (bit 2)
 *     DS3231_STAT_EN32KHZ     - 32 kHz output enable     (bit 3)
 *     DS3231_STAT_OSF         - Oscillator stop flag     (bit 7)
 *
 *   Square-wave frequencies (RS2:RS1 bits):
 *     DS3231_SQW_1HZ          - 1 Hz     (RS2=0, RS1=0)
 *     DS3231_SQW_1KHZ         - 1.024 kHz (RS2=0, RS1=1)
 *     DS3231_SQW_4KHZ         - 4.096 kHz (RS2=1, RS1=0)
 *     DS3231_SQW_8KHZ         - 8.192 kHz (RS2=1, RS1=1)
 *
 *   Alarm 1 match modes:
 *     DS3231_ALM1_PER_SEC              - Once per second
 *     DS3231_ALM1_MTC_SECS             - When seconds match
 *     DS3231_ALM1_MTC_MIN_SEC          - When minutes and seconds match
 *     DS3231_ALM1_MTC_HR_MIN_SEC       - When hours, minutes, and seconds match
 *     DS3231_ALM1_MTC_DT_HR_MIN_SEC    - When date, hours, minutes, seconds match
 *     DS3231_ALM1_MTC_DY_DT_HR_MIN_SEC - When day, hours, minutes, seconds match
 *
 *   Alarm 2 match modes:
 *     DS3231_ALM2_PER_MIN              - Once per minute (at 00 seconds)
 *     DS3231_ALM2_MTC_MIN              - When minutes match
 *     DS3231_ALM2_MTC_HR_MIN           - When hours and minutes match
 *     DS3231_ALM2_MTC_DT_HR_MIN        - When date, hours, and minutes match
 *     DS3231_ALM2_MTC_DY_DT_HR_MIN     - When day, hours, and minutes match
 *
 * Usage:
 *   - Include this header where DS3231 access is required:
 *       #include "ds3231.h"
 *
 *   - Initialize the DS3231:
 *       DS3231_Init();
 *
 *   - Set the current time:
 *       DS3231_Datetime_t now = {0, 30, 14, 4, 26, 2, 2026};
 *       DS3231_SetTime(&now);
 *
 *   - Read the current time:
 *       DS3231_Datetime_t now;
 *       DS3231_GetTime(&now);
 *
 *   - Set Alarm 1 to fire every day at 07:00:00:
 *       DS3231_Datetime_t alarm = {0, 0, 7, 0, 0, 0, 0};
 *       DS3231_SetAlarm1(&alarm, DS3231_ALM1_MTC_HR_MIN_SEC);
 *
 *   - Read the temperature:
 *       float temp = DS3231_GetTemp();
 *
 *   - Enable a 1 Hz square wave on the INT/SQW pin:
 *       DS3231_SetSQWFreq(DS3231_SQW_1HZ);
 *       DS3231_EnableSQW();
 *
 * Notes:
 *   - All I2C addresses used internally are 7-bit. The R/W bit is managed
 *     by the I2C HAL.
 *   - The DS3231 uses BCD encoding for all time and date registers.
 *     Conversion between BCD and binary is handled internally by this driver.
 *   - The century bit (bit 7 of the month register) is set automatically
 *     when the year value is >= 2100, supporting dates up to year 2199.
 *   - The EOSC bit is active-low: clearing it enables the oscillator.
 *   - Temperature resolution is 0.25 °C (10-bit signed value, two MSBs of
 *     the LSB register).
 *   - All operations are blocking. The underlying I2C HAL uses polling.
 *   - Alarm flag bits (A1F, A2F) in the status register must be cleared
 *     by the application after each alarm event.
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
 *   2026-02-26  v0.1  Initial header for DS3231 RTC driver
 *
 */

#include "ds3231.h"

#define TO_BCD(val) ((uint8_t)((((val) / 10U) << 4U) | ((val) % 10U)))
#define TO_BIN(val) ((uint8_t)((((val) >> 4U) * 10U) + ((val) & 0x0FU)))

void DS3231_Init(void)
{
	HAL_I2C_InitController(400000U);
	
	HAL_I2C_ControllerSend(DS3231_ADDR, DS3231_REG_CONTROL);
	uint8_t reg_ctrl = HAL_I2C_ControllerRead(DS3231_ADDR);
	
	reg_ctrl &= ~DS3231_CTRL_EOSC;
	
	uint8_t buff[2] = {DS3231_REG_CONTROL, reg_ctrl};
	HAL_I2C_ControllerTransmit(DS3231_ADDR, buff, 2);
}

void DS3231_SetTime(DS3231_Datetime_t* time)
{
	uint8_t buff[8];
	
	if(time->year >= 2100U) time->month |= 0x80U;
	
	buff[0] = DS3231_REG_SECONDS;
	buff[1] = TO_BCD(time->sec);
	buff[2] = TO_BCD(time->min);
	buff[3] = TO_BCD(time->hour);
	buff[4] = TO_BCD(time->day);
	buff[5] = TO_BCD(time->date);
	buff[6] = TO_BCD(time->month);
	buff[7] = TO_BCD((uint8_t)time->year % 100U);
	
	HAL_I2C_ControllerTransmit(DS3231_ADDR, buff, 8);
}

void DS3231_GetTime(DS3231_Datetime_t* time)
{
	uint8_t reg_vals[7];

	HAL_I2C_ControllerSend(DS3231_ADDR, DS3231_REG_SECONDS);
	HAL_I2C_ControllerReceive(DS3231_ADDR, reg_vals, 7);

	time->sec   = TO_BIN(reg_vals[0]);
	time->min   = TO_BIN(reg_vals[1]);
	time->hour  = TO_BIN(reg_vals[2]);
	time->day   = reg_vals[3];
	time->date  = TO_BIN(reg_vals[4]);
	time->month = TO_BIN(reg_vals[5]);
	time->year  = TO_BIN(reg_vals[6]);
	
	if (reg_vals[5] & 0x80U) time->year += 2100U;
	else time->year += 2000U;
}

void DS3231_SetAlarm1(DS3231_Datetime_t* time, uint8_t mode)
{
	DS3231_EnableAlarm1();
	
	uint8_t buff[5] = {
		DS3231_REG_ALM1_SEC,
		TO_BCD(time->sec),
		TO_BCD(time->min),
		TO_BCD(time->hour),
		TO_BCD(time->day)
	};
	
	switch(mode)
	{
		case DS3231_ALM1_PER_SEC:
			buff[1] = 0x80;
			buff[2] = 0x80;
			buff[3] = 0x80;
			buff[4] = 0x80; 
			break;
		case DS3231_ALM1_MTC_SECS:
			buff[2] |= 0x80U;
			buff[3] |= 0x80U;
			buff[4] |= 0x80U;
			break;
		case DS3231_ALM1_MTC_MIN_SEC:
			buff[3] |= 0x80U;
			buff[4] |= 0x80U;
			break;
		case DS3231_ALM1_MTC_HR_MIN_SEC:
			buff[4] |= 0x80U;
			break;
		case DS3231_ALM1_MTC_DT_HR_MIN_SEC:
			break;
		case DS3231_ALM1_MTC_DY_DT_HR_MIN_SEC:
			buff[4] |= 0x40U;
			break;
		default:
			break;
	}
	
	HAL_I2C_ControllerTransmit(DS3231_ADDR, buff, 5);
}

void DS3231_DisableAlarm1(void)
{
	HAL_I2C_ControllerSend(DS3231_ADDR, DS3231_REG_CONTROL);
	uint8_t reg_ctrl = HAL_I2C_ControllerRead(DS3231_ADDR);
	
	reg_ctrl &= DS3231_CTRL_A1IE;
	uint8_t cmd[2] = {DS3231_REG_CONTROL, reg_ctrl};
	HAL_I2C_ControllerTransmit(DS3231_ADDR, cmd, 2);
}

void DS3231_EnableAlarm1(void)
{
	HAL_I2C_ControllerSend(DS3231_ADDR, DS3231_REG_CONTROL);
	uint8_t reg_ctrl = HAL_I2C_ControllerRead(DS3231_ADDR);
	
	reg_ctrl |= DS3231_CTRL_A1IE;
	uint8_t cmd[2] = {DS3231_REG_CONTROL, reg_ctrl};
	HAL_I2C_ControllerTransmit(DS3231_ADDR, cmd, 2);
}

void DS3231_SetAlarm2(DS3231_Datetime_t* time, uint8_t mode)
{
	DS3231_EnableAlarm2();
	
	uint8_t buff[4] = {
		DS3231_REG_ALM2_MIN,
		TO_BCD(time->min),
		TO_BCD(time->hour),
		TO_BCD(time->day)
	};
	
	switch(mode)
	{
		case DS3231_ALM2_PER_MIN:
			buff[1] = 0x80;
			buff[2] = 0x80;
			buff[3] = 0x80;
			break;
		case DS3231_ALM2_MTC_MIN:
			buff[2] |= 0x80U;
			buff[3] |= 0x80U;
			break;
		case DS3231_ALM2_MTC_HR_MIN:
			buff[3] |= 0x80U;
			break;
		case DS3231_ALM2_MTC_DT_HR_MIN:
			break;
		case DS3231_ALM2_MTC_DY_DT_HR_MIN:
			buff[3] |= 0x40U; 
			break;
		default:
			break;
	}
	
	HAL_I2C_ControllerTransmit(DS3231_ADDR, buff, 4);
}

void DS3231_DisableAlarm2(void)
{
	HAL_I2C_ControllerSend(DS3231_ADDR, DS3231_REG_CONTROL);
	uint8_t reg_ctrl = HAL_I2C_ControllerRead(DS3231_ADDR);
	
	reg_ctrl &= ~DS3231_CTRL_A2IE;
	uint8_t cmd[2] = {DS3231_REG_CONTROL, reg_ctrl};
	HAL_I2C_ControllerTransmit(DS3231_ADDR, cmd, 2);
}

void DS3231_EnableAlarm2(void)
{
	HAL_I2C_ControllerSend(DS3231_ADDR, DS3231_REG_CONTROL);
	uint8_t reg_ctrl = HAL_I2C_ControllerRead(DS3231_ADDR);
	
	reg_ctrl |= DS3231_CTRL_A2IE;
	uint8_t cmd[2] = {DS3231_REG_CONTROL, reg_ctrl};
	HAL_I2C_ControllerTransmit(DS3231_ADDR, cmd, 2);
}

float DS3231_GetTemp()
{
	uint8_t temp_bytes[2];
	int16_t raw_temp;
	
	HAL_I2C_ControllerSend(DS3231_ADDR, DS3231_REG_TEMP_MSB);
	HAL_I2C_ControllerReceive(DS3231_ADDR, temp_bytes, 2);
	
	raw_temp = (int16_t) (((uint16_t)temp_bytes[0] << 2U) | (temp_bytes[1] >> 6U));

	if(raw_temp & 0x0200) raw_temp |= (int16_t) 0xFC00;
	
	return (float) raw_temp * 0.25f;
}

void DS3231_SetSQWFreq(uint8_t freq)
{
	HAL_I2C_ControllerSend(DS3231_ADDR, DS3231_REG_CONTROL);
	uint8_t reg_ctrl = HAL_I2C_ControllerRead(DS3231_ADDR);
	
	reg_ctrl = (reg_ctrl & 0xE7) | freq;
	
	uint8_t buff[2] = {DS3231_REG_CONTROL, reg_ctrl};
	HAL_I2C_ControllerTransmit(DS3231_ADDR, buff, 2);
}

void DS3231_EnableSQW(void)
{
	HAL_I2C_ControllerSend(DS3231_ADDR, DS3231_REG_CONTROL);
	uint8_t reg_ctrl = HAL_I2C_ControllerRead(DS3231_ADDR);
	
	reg_ctrl |= DS3231_CTRL_INTCN;
	uint8_t buff[2] = {DS3231_REG_CONTROL, reg_ctrl};
	HAL_I2C_ControllerTransmit(DS3231_ADDR, buff, 2);
}

void DS3231_DisableSQW(void)
{
	HAL_I2C_ControllerSend(DS3231_ADDR, DS3231_REG_CONTROL);
	uint8_t reg_ctrl = HAL_I2C_ControllerRead(DS3231_ADDR);
	
	reg_ctrl &= ~DS3231_CTRL_INTCN;
	uint8_t buff[2] = {DS3231_REG_CONTROL, reg_ctrl};
	HAL_I2C_ControllerTransmit(DS3231_ADDR, buff, 2);
}

void DS3231_Enable32khz(void)
{
	HAL_I2C_ControllerSend(DS3231_ADDR, DS3231_REG_STATUS);
	uint8_t reg_status = HAL_I2C_ControllerRead(DS3231_ADDR);
	
	reg_status |= DS3231_STAT_EN32KHZ;
	uint8_t buff[2] = {DS3231_REG_STATUS, reg_status};
	HAL_I2C_ControllerTransmit(DS3231_ADDR, buff, 2);
}

void DS3231_Disable32khz(void)
{
	HAL_I2C_ControllerSend(DS3231_ADDR, DS3231_REG_STATUS);
	uint8_t reg_status = HAL_I2C_ControllerRead(DS3231_ADDR);
	
	reg_status &= ~DS3231_STAT_EN32KHZ;
	uint8_t buff[2] = {DS3231_REG_STATUS, reg_status};
	HAL_I2C_ControllerTransmit(DS3231_ADDR, buff, 2);
}

void DS3231_EnableOSC(void)
{
	HAL_I2C_ControllerSend(DS3231_ADDR, DS3231_REG_CONTROL);
	uint8_t reg_ctrl = HAL_I2C_ControllerRead(DS3231_ADDR);
	
	reg_ctrl &= ~DS3231_CTRL_EOSC;
	uint8_t buff[2] = {DS3231_REG_CONTROL, reg_ctrl};
	HAL_I2C_ControllerTransmit(DS3231_ADDR, buff, 2);
}

uint8_t DS3231_IsOSCStopped(void)
{
	HAL_I2C_ControllerSend(DS3231_ADDR, DS3231_REG_STATUS);
	uint8_t reg_status = HAL_I2C_ControllerRead(DS3231_ADDR);
	
	return (reg_status & DS3231_STAT_OSF) ? 1U : 0U;
}

void DS3231_SetAging(int8_t offset)
{
	uint8_t buff[2] = {DS3231_REG_AGING, (uint8_t) offset};
		
	HAL_I2C_ControllerTransmit(DS3231_ADDR, buff, 2);
}

int8_t DS3231_GetAging()
{
	HAL_I2C_ControllerSend(DS3231_ADDR, DS3231_REG_AGING);
	uint8_t raw_aging = HAL_I2C_ControllerRead(DS3231_ADDR);
	
	return (int8_t) raw_aging;
}
