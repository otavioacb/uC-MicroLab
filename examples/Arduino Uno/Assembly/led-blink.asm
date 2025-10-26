; -----------------------------------------------------------------------------
; uc-Microlab Example: GPIO Blink (assembly)
; Repository: uc-Microlab
;
; Description:
;   Simple AVR assembly example that toggles a GPIO pin in a loop using a
;   software delay routine. Intended as an example for the uc-Microlab board
;   (version r1) and the ATmega328P MCU.
;
; Hardware: uc-Microlab — version r1
; Target MCU: ATmega328P (Arduino Uno compatible)
;
; Connections:
;   - PB2 -> LED (through a suitable resistor) -> VCC (for active-low)
;     or PB2 -> resistor -> LED -> GND (for active-high). Adjust wiring
;     according to your LED orientation and desired polarity.
;
; Build notes / usage:
;   - This file is AVR assembly source. For Atmel/Microchip Studio use the
;     built-in assembler (AVRASM2). For AVR-GCC toolchain you can assemble
;     with avr-as / avr-gcc as appropriate.
;   - Add this file to your project (Source files) in Microchip Studio or
;     place it in the sketch folder for advanced Arduino builds (toolchain
;     may need manual assembly).
;   - The delay routine is a simple software loop; timing depends on CPU
;     clock frequency (F_CPU). The code does not reference F_CPU directly,
;     so delays are approximate — measure/adjust if precise timing is needed.
;
; Author: otavioacb
; Date: 2025-10-26
; License: MIT (SPDX-License-Identifier: MIT) — see repository LICENSE file
; -----------------------------------------------------------------------------

.include "m328pdef.inc"
.org 0x0000

rjmp main

; ---------------------------------------------------------------------------
; delay: simple three-byte nested loop delay
;   Uses registers r17, r18, r19 as counters. Timing is approximate.
; ---------------------------------------------------------------------------
delay:
    clr r17
    clr r18
    ldi r19, 10

delay_loop:
    dec r18
    brne delay_loop

    dec r17
    brne delay_loop

    dec r19
    brne delay_loop

    ret

; ---------------------------------------------------------------------------
; main: configure PB2 as output and blink it
; ---------------------------------------------------------------------------
main:
    ; Set PB2 as output: DDRB bit 2 = 1
    sbi DDRB, 2

loop:
    ; Clear PORTB bit 2 (drive pin low)
    cbi PORTB, 2
    rcall delay        ; call delay

    ; Set PORTB bit 2 (drive pin high)
    sbi PORTB, 2
    rcall delay        ; call delay

    rjmp loop          ; infinite loop
