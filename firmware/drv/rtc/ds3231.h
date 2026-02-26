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

#ifndef DS3231_H_
#define DS3231_H_

#include <stdint.h>
#include "i2c-hal.h"

#define DS3231_ADDR             0x68 

#define DS3231_REG_SECONDS      0x00
#define DS3231_REG_MINUTES      0x01
#define DS3231_REG_HOURS        0x02
#define DS3231_REG_DAY          0x03
#define DS3231_REG_DATE         0x04
#define DS3231_REG_MONTH        0x05
#define DS3231_REG_YEAR         0x06
#define DS3231_REG_ALM1_SEC     0x07
#define DS3231_REG_ALM1_MIN     0x08
#define DS3231_REG_ALM1_HOUR    0x09
#define DS3231_REG_ALM1_DAY     0x0A
#define DS3231_REG_ALM2_MIN     0x0B
#define DS3231_REG_ALM2_HOUR    0x0C
#define DS3231_REG_ALM2_DAY     0x0D
#define DS3231_REG_CONTROL      0x0E
#define DS3231_REG_STATUS       0x0F
#define DS3231_REG_AGING        0x10
#define DS3231_REG_TEMP_MSB     0x11
#define DS3231_REG_TEMP_LSB     0x12 

#define DS3231_CTRL_A1IE        0x01   /* bit 0 */
#define DS3231_CTRL_A2IE        0x02   /* bit 1 */
#define DS3231_CTRL_INTCN       0x04   /* bit 2 */
#define DS3231_CTRL_RS1         0x08   /* bit 3 */
#define DS3231_CTRL_RS2         0x10   /* bit 4 */
#define DS3231_CTRL_CONV        0x20   /* bit 5 */
#define DS3231_CTRL_BBSQW       0x40   /* bit 6 */
#define DS3231_CTRL_EOSC        0x80   /* bit 7 */

#define DS3231_STAT_A1F         0x01   /* bit 0 */
#define DS3231_STAT_A2F         0x02   /* bit 1 */
#define DS3231_STAT_BSY         0x04   /* bit 2 */
#define DS3231_STAT_EN32KHZ     0x08   /* bit 3 */
#define DS3231_STAT_OSF         0x80   /* bit 7 */

#define DS3231_SQW_1HZ          0x00   /* RS2=0, RS1=0 */
#define DS3231_SQW_1KHZ         0x08   /* RS2=0, RS1=1 */
#define DS3231_SQW_4KHZ         0x10   /* RS2=1, RS1=0 */
#define DS3231_SQW_8KHZ         0x18   /* RS2=1, RS1=1 */

#define DS3231_ALM1_PER_SEC               0x0F
#define DS3231_ALM1_MTC_SECS              0x0E
#define DS3231_ALM1_MTC_MIN_SEC           0x0C
#define DS3231_ALM1_MTC_HR_MIN_SEC        0x08 
#define DS3231_ALM1_MTC_DT_HR_MIN_SEC     0x00
#define DS3231_ALM1_MTC_DY_DT_HR_MIN_SEC  0x10

#define DS3231_ALM2_PER_MIN               0x07
#define DS3231_ALM2_MTC_MIN               0x06
#define DS3231_ALM2_MTC_HR_MIN            0x04
#define DS3231_ALM2_MTC_DT_HR_MIN         0x00
#define DS3231_ALM2_MTC_DY_DT_HR_MIN      0x08

typedef struct 
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint16_t year;
} DS3231_Datetime_t;

void DS3231_Init(void);

void DS3231_SetTime(DS3231_Datetime_t* time);
void DS3231_GetTime(DS3231_Datetime_t* time);

void DS3231_SetAlarm1(DS3231_Datetime_t* time, uint8_t mode);
void DS3231_DisableAlarm1(void);
void DS3231_EnableAlarm1(void);

void DS3231_SetAlarm2(DS3231_Datetime_t* time, uint8_t mode);
void DS3231_DisableAlarm2(void);
void DS3231_EnableAlarm2(void);

float DS3231_GetTemp();

void DS3231_SetSQWFreq(uint8_t freq);
void DS3231_EnableSQW(void);
void DS3231_DisableSQW(void);

void DS3231_Enable32khz(void);
void DS3231_Disable32khz(void);

void DS3231_EnableOSC(void);
uint8_t DS3231_IsOSCStopped(void);

void DS3231_SetAging(int8_t offset);
int8_t DS3231_GetAging();

#endif /* DS3231_H_ */
