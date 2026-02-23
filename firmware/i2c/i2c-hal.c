/*
 * uc-Microlab — I2C HAL (header comment)
 * File: i2c-hal.h / i2c-hal.c
 *
 * Project: uc-MicroLab
 * Component: I2C Hardware Abstraction Layer (HAL)
 * Hardware: uc-MicroLab board — version r1
 *
 * Description:
 *   Minimal, portable I2C HAL for small microcontroller projects.
 *   Provides routines to configure and operate the I2C (TWI) peripheral in
 *   both controller (master) and peripheral (slave) modes. The API is
 *   intentionally small and hardware-agnostic so it can be used in examples,
 *   libraries and higher-level firmware components.
 *
 * Public API:
 *   void HAL_I2C_InitController(uint32_t freq);
 *     Initialize the I2C peripheral in controller (master) mode with the
 *     specified SCL clock frequency in Hz.
 *
 *   void HAL_I2C_InitPeripheral(unsigned char addr);
 *     Initialize the I2C peripheral in peripheral (slave) mode, responding
 *     to the specified 7-bit address.
 *
 *   void HAL_I2C_SetFrequency(uint32_t freq);
 *     Change the SCL clock frequency. Only effective in controller mode.
 *
 *   void HAL_I2C_EndComm(void);
 *     Disable the I2C peripheral and release the bus.
 *
 *   void HAL_I2C_ControllerSend(unsigned char addr, unsigned char data);
 *     Transmit a single byte to the peripheral at the given 7-bit address.
 *     Performs a complete START → SLA+W → DATA → STOP sequence.
 *
 *   void HAL_I2C_ControllerTransmit(unsigned char addr,
 *                                    unsigned char *buf,
 *                                    size_t len);
 *     Transmit multiple bytes to the peripheral at the given 7-bit address.
 *     Performs a complete START → SLA+W → DATA[0..len-1] → STOP sequence.
 *
 *   unsigned char HAL_I2C_ControllerRead(unsigned char addr);
 *     Receive a single byte from the peripheral at the given 7-bit address.
 *     Performs a complete START → SLA+R → DATA (NACK) → STOP sequence.
 *
 *   void HAL_I2C_ControllerReceive(unsigned char addr,
 *                                   unsigned char *buf,
 *                                   size_t len);
 *     Receive multiple bytes from the peripheral at the given 7-bit address
 *     into buf. Sends ACK after each byte except the last, which receives
 *     NACK to signal the end of the transfer.
 *     Performs a complete START → SLA+R → DATA[0..len-1] → STOP sequence.
 *
 *   void HAL_I2C_PeripheralSend(unsigned char data);
 *     Transmit a single byte to the controller. Waits for the controller to
 *     address this device with SLA+R before sending.
 *
 *   void HAL_I2C_PeripheralTransmit(unsigned char *buf, size_t len);
 *     Transmit multiple bytes to the controller. Waits for the controller to
 *     address this device with SLA+R, then sends len bytes from buf.
 *
 *   unsigned char HAL_I2C_PeripheralRead(void);
 *     Receive a single byte from the controller. Waits for the controller to
 *     address this device with SLA+W before reading.
 *
 *   void HAL_I2C_PeripheralReceive(unsigned char *buf, size_t len);
 *     Receive multiple bytes from the controller into buf. Waits for the
 *     controller to address this device with SLA+W, then reads len bytes.
 *     Sends ACK after each byte except the last, which receives NACK to
 *     signal that the buffer is full.
 *
 * Public constants:
 *   Clock prescalers (TWPS bits in TWSR):
 *     HAL_I2C_PRE_1  - prescaler value 1   (TWPS = 0b00)
 *     HAL_I2C_PRE_4  - prescaler value 4   (TWPS = 0b01)
 *     HAL_I2C_PRE_16 - prescaler value 16  (TWPS = 0b10)
 *     HAL_I2C_PRE_64 - prescaler value 64  (TWPS = 0b11)
 *
 * Usage:
 *   - Include this header where I2C access is required:
 *       #include "i2c-hal.h"
 *
 *   - Initialize I2C in controller mode at 100 kHz:
 *       HAL_I2C_InitController(100000UL);
 *
 *   - Transmit a single byte to address 0x48:
 *       HAL_I2C_ControllerSend(0x48, 0xAA);
 *
 *   - Transmit multiple bytes to address 0x48:
 *       unsigned char buf[] = {0x01, 0x02, 0x03};
 *       HAL_I2C_ControllerTransmit(0x48, buf, 3);
 *
 *   - Receive a single byte from address 0x48:
 *       unsigned char data = HAL_I2C_ControllerRead(0x48);
 *
 *   - Receive multiple bytes from address 0x48:
 *       unsigned char buf[4];
 *       HAL_I2C_ControllerReceive(0x48, buf, 4);
 *
 *   - Initialize I2C in peripheral mode at address 0x32:
 *       HAL_I2C_InitPeripheral(0x32);
 *
 *   - Wait for and read a single byte sent by the controller:
 *       unsigned char data = HAL_I2C_PeripheralRead();
 *
 *   - Wait for and send a single byte to the controller:
 *       HAL_I2C_PeripheralSend(0xAA);
 *
 * Notes:
 *   - All addresses passed to the API are 7-bit. The R/W bit is managed
 *     internally by the HAL.
 *   - SCL frequency is derived from the CPU clock using the formula defined
 *     in the ATmega328P datasheet (section 26.5.2):
 *       TWBR = (F_CPU / freq - 16) / (2 * prescaler)
 *     HAL_I2C_SetFrequency assumes F_CPU = 16 MHz and prescaler = 1.
 *   - All operations are blocking (polling). Consider interrupt-driven
 *     transfers for performance-critical applications.
 *   - This HAL does not support multi-controller (multi-master) arbitration.
 *   - This HAL does not support 10-bit addressing.
 *
 * Author: otavioacb
 * Created: 2026-02-23
 *
 * License:
 *   SPDX-License-Identifier: MIT
 *   This file is licensed under the MIT License — see the repository LICENSE
 *   file for full terms and copyright information.
 *
 * Change log:
 *   2026-02-23  v0.1  Initial header for I2C HAL
 *
 */

#include "i2c-hal.h"

static void i2c_controller_start(void);
static void i2c_controller_stop(void);
static void i2c_controller_slaw(unsigned char addr);
static void i2c_controller_slar(unsigned char addr);
static void i2c_peripheral_wait_addr(void);

void HAL_I2C_InitController(uint32_t freq)
{
	HAL_I2C_SetFrequency(freq);
	
	TWCR = (1 << TWEN);
}

void HAL_I2C_InitPeripheral(unsigned char addr)
{
	TWAR = (addr << 1);
	
	TWCR = (1 << TWEN) | (1 << TWEA);
}

void HAL_I2C_SetFrequency(uint32_t freq)
{
	TWSR = 0x00;
	
	TWBR = (uint8_t)(( 16000000U/ freq - 16U) / 2U);
}

void HAL_I2C_ControllerSend(unsigned char addr, unsigned char data)
{
	i2c_controller_start();
	
	i2c_controller_slaw(addr);
	
	TWDR = data;
	TWCR = (1 << TWEN) | (1 << TWINT);
	WAIT_TWINT();
	
	i2c_controller_stop();
}

void HAL_I2C_ControllerTransmit(unsigned char addr, unsigned char* buf, size_t len)
{
	i2c_controller_start();
	
	i2c_controller_slaw(addr);
	
	for(int i = 0; i < len; ++i)
	{
		TWDR = buf[i];
		TWCR = (1 << TWEN) | (1 << TWINT);
		WAIT_TWINT();
	}
	
	i2c_controller_stop();
}


unsigned char HAL_I2C_ControllerRead(unsigned char addr)
{
	unsigned char recv = 0x00;
	
	i2c_controller_start();
	
	i2c_controller_slar(addr);
	
	TWCR = (1 << TWEN) | (1 << TWINT);
	WAIT_TWINT();
	recv = TWDR;
	
	i2c_controller_stop();
	
	return recv;
}

void HAL_I2C_ControllerReceive(unsigned char addr, unsigned char* buf, size_t len)
{
	i2c_controller_start();
	
	i2c_controller_slar(addr);
	
	for(int i = 0; i < len - 1; ++i)
	{
		TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
		WAIT_TWINT();
		buf[i] = TWDR;
	}
	
	TWCR = (1 << TWEN) | (1 << TWINT);
	WAIT_TWINT();
	buf[len - 1] = TWDR;
	
	i2c_controller_stop();
}


void HAL_I2C_PeripheralSend(unsigned char data)
{
	i2c_peripheral_wait_addr();
	
	TWDR = data;
	
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
	WAIT_TWINT();	
}

void HAL_I2C_PeripheralTransmit(unsigned char* buf, size_t len)
{
	i2c_peripheral_wait_addr();
	
	for(int i = 0; i < len; ++i)
	{
		TWDR = buf[i];
		TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
		WAIT_TWINT();
	}
}

unsigned char HAL_I2C_PeripheralRead(void)
{
	i2c_peripheral_wait_addr();
	
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
	WAIT_TWINT();
	
	return TWDR;
}

void HAL_I2C_PeripheralReceive(unsigned char* buf, size_t len)
{
	i2c_peripheral_wait_addr();
	
	for(int i = 0; i < len - 1; ++i)
	{
		TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
		WAIT_TWINT();
		buf[i] = TWDR;
	}
	TWCR = (1 << TWEN) | (1 << TWINT);
	WAIT_TWINT();
	buf[len - 1] = TWDR;
}

void HAL_I2C_EndComm(void)
{
	TWCR &= ~(1 << TWEN);
}

static void i2c_controller_start(void)
{
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA);
	WAIT_TWINT();	
}

static void i2c_controller_stop(void)
{
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO);
	WAIT_STOP();
}

static void i2c_controller_slaw(unsigned char addr)
{
	TWDR = (addr << 1);
	TWCR = (1 << TWEN) | (1 << TWINT);
	WAIT_TWINT();
}

static void i2c_controller_slar(unsigned char addr)
{
	TWDR = (addr << 1) | 0x01;
	TWCR = (1 << TWEN) | (1 << TWINT);
	WAIT_TWINT();
}

static void i2c_peripheral_wait_addr(void)
{
	TWCR = (1 << TWEN) | (1 << TWEA);
	WAIT_TWINT();
}
