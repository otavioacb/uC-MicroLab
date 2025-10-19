# Firmwares

This directory contains firmware solutions for the uC MicroLab board: low-level hardware descriptions, Hardware Abstraction Layers (HAL), device drivers (DRV), middleware (MDW) and application examples (App). The goal is to collect re-usable and educational implementations that demonstrate how to access peripherals, build drivers, and compose higher-level features and example projects.

<p align="center">
  <img width="538" height="269" alt="firmware-structures" src="https://github.com/user-attachments/assets/a857ec5a-e821-499a-b333-c51b00d9ab5c" />
</p>

The architecture image included in the repository is a reference for the intended separation of responsibilities between layers (HW → HAL → DRV → MDW → App). Use it as a guide when structuring new modules.

Overview of layers
- HW: Hardware descriptions and documentation (GPIO, CPU, Flash, Timer, RAM, ROM, etc.). This folder documents the physical resources and board-specific details.
- HAL: Peripheral abstractions that provide a stable, minimal API to access hardware peripherals (USART, I2C, SPI, PWM, ADC, PORT, etc.). HAL modules should be portable and free of application logic.
- DRV: Device-level drivers that implement component-specific behavior (Display LCD, Buzzer, RGB LED, Push-buttons, 7-seg display, etc.), typically using HAL services.
- MDW: Middleware components such as RTOS integration (e.g., FreeRTOS), protocol stacks, or utility libraries.
- App: Example applications and demos that combine drivers and middleware to show real use cases (schedulers, UI menus, main routines).

Suggested directory layout
```text
firmwares/
├── board/               # Board Support Package (BSP): pinouts, clocks, board config
├── hw/                  # Hardware descriptions and schematics or mapping tables
├── hal/                 # HAL modules per peripheral (hal_usart/, hal_i2c/, ...)
├── drv/                 # Drivers per device (drv_lcd/, drv_buzzer/, drv_rgb/, ...)
├── mdw/                 # Middleware (FreeRTOS, protocol libs, common utilities)
├── apps/                # Example applications (app_blinky/, app_menu/, app_demo/)
├── tools/               # Flashing scripts, helper scripts, CI helpers
├── docs/                # API docs, design decisions, usage guides
├── build/               # Build artifacts (generated) — typically in .gitignore
└── README.md            # (this file)
```

Naming conventions and public API guidelines
- Filenames:
  - HAL: <peripheral>-hal.c / <peripheral>-hal.h (e.g., usart-hal.c / usart-hal.h)
  - Driver: <device>-drv.c / <device>-drv.h (e.g., buzzer-drv.c / buzzer-drv.h)
  - App: <name>-app.c (e.g., scheduler-app.c)
- Symbols:
  - Prefix public functions and types with the module prefix (e.g., HAL_USART_Init, DRV_LCD_Write).
- Headers:
  - Each public header should document the API, parameters, return values, preconditions, and thread-safety expectations.
- Dependencies:
  - Document any dependencies at the top of module READMEs (for example, lcd-drv depends on spi-hal).

How to add a new HAL or driver — quick workflow
1. Create the folder: hal/<peripheral>/ or drv/<device>/.
2. Implement .c and .h files using the naming conventions above.
3. Add a local README.md inside the module describing:
   - Purpose and scope
   - Public API with short examples
   - Dependencies (other HALs, RTOS services, etc.)
   - Expected behavior and error handling
4. Add a simple example in apps/ that exercises the new module.
5. Add build rules or a sample CMake/Makefile in examples/ so it is easy to compile and flash.

Build, test and flashing
- Provide at least one example project in examples/ with a ready-to-run build configuration (CMake, Make, or PlatformIO).
- Place board-specific compile flags and linker scripts in board/ or in the BSP to avoid duplication.
- Keep flashing scripts in tools/ (e.g., openocd, st-flash, avrdude) and document the flashing steps in the example README.
- Document required toolchain, target MCU, and recommended compiler flags in the main docs/ or the top-level CONTRIBUTING.md.

Testing and validation
- Supply small example tests or smoke-tests that validate each HAL and driver (e.g., toggle a pin, send a UART message).
- Prefer hardware-in-the-loop examples when possible; otherwise provide minimal host-side test programs.
- If using an RTOS, provide examples showing both bare-metal and RTOS-based usage.

Best practices
- Keep HALs free of application logic; HAL should only expose hardware operations.
- Drivers (DRV) can implement device-specific logic and simple state machines, but avoid embedding whole application flows.
- Document breaking API changes and provide migration notes in module READMEs.
- Favor small, well-documented modules over monolithic files.
- Use consistent code style and document it in CONTRIBUTING.md.

Contributing
- Open a Pull Request with:
  - A short description of the change
  - Example or test that demonstrates the new module
  - Build and flash instructions
- Follow the repository's coding style and naming conventions.
- If reusing external code, include licensing and attribution where required.

Visual reference
- The included architecture image is intended to guide placement of modules: low-level access belongs in HAL/HW, device logic in DRV, shared services in MDW, and user-facing examples in APP/EXAMPLES.

What I can generate for you next
- Module README templates for hal/, drv/, and apps/
- A base CMakeLists.txt or Makefile example for building examples
- Starter driver/HAL skeletons matching the naming and API guidelines

```
