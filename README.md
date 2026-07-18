# Smart Home Access Control & Automation System (ATmega32)

Bare-metal C firmware for an **ATmega32**-based smart home controller: PIN-code access control (Admin/Guest roles) via a matrix keypad and character LCD, temperature-driven climate control, potentiometer-driven appliance dimming, a servo-actuated door lock, and remote control over UART (e.g. from a Bluetooth-connected mobile app).

**Every peripheral driver is written from scratch, register by register, from the ATmega32 datasheet** — GPIO, HD44780 LCD, matrix keypad, ADC, internal EEPROM, PWM/servo, timers, and USART. No vendor HAL, no Atmel Software Framework (ASF), no Arduino, no RTOS, and no external libraries beyond `avr-libc`.

## Features

- **PIN-code authentication** with two roles (Admin / Guest), 4-digit PINs persisted in internal EEPROM, first-boot provisioning flow, and a 3-strike lockout with a timed cooldown.
- **Session timeout** — a Timer0-driven ~10 ms tick counter tracks how long a session has been active.
- **Temperature-based climate control** — reads a sensor on the ADC and switches an AC/relay output on/off past configurable thresholds.
- **Analog appliance dimming** — a potentiometer reading drives a Timer2 PWM output (fan speed / dimmer).
- **Servo-actuated door lock** — Timer1 hardware PWM drives a hobby servo between two fixed angles (locked/unlocked), restricted to the Admin role.
- **Remote control over UART** — single-byte commands (e.g. from a Bluetooth serial module) toggle lamps and the door lock, with different command sets available to Admin vs. Guest sessions.
- **From-scratch peripheral drivers** for GPIO, character LCD, matrix keypad, ADC, EEPROM, PWM/servo, timers, and USART.

## Firmware architecture

`main.c` was originally one ~460-line file containing the entire application. It has been split into small, single-purpose modules that `main()` now orchestrates:

- `bsp.c` / `bsp.h` — `BSP_Init()`, a single peripheral bring-up call.
- `auth.c` / `auth.h` — first-boot provisioning, PIN entry, and the login/lockout state machine.
- `session.c` / `session.h` — the session-timeout clock (Timer0 tick counter).
- `climate.c` / `climate.h` — temperature → AC relay, potentiometer → fan PWM.
- `remote.c` / `remote.h` — UART remote-command dispatch.

Underneath sits the original driver layer: a channel-based DIO abstraction (`DIO.c`, `DIO_GFC.c`) that higher-level drivers (`Keypad.c`, `LCD.c`) are built on, plus direct-register drivers (`ADC.c`, `EEPROM.c`, `PWM_Servo.c`, `timer_driver.c`, `UART.c`).

## Building & flashing

This is an Atmel Studio / Microchip Studio 7 project — there is no standalone Makefile.

1. Open `Project.atsln` in Atmel Studio 7 or Microchip Studio.
2. Select the `Debug` or `Release` configuration and build (**F7**). Output (`.elf`, `.hex`, `.eep`, `.lss`, `.srec`) is written to `Debug/` or `Release/`.
3. Flash the resulting `.hex` to an ATmega32 with your programmer of choice (AVRISP, USBasp, etc.), and configure fuses for an external 16 MHz crystal to match `F_CPU`.

## Known issues

This refactor moved code into modules but did not change behavior — every issue below (found while restructuring) is still present and will be addressed in follow-up changes:

- `UART_Send`/`UART_Recieve` (`UART.c`) wait on the wrong status bits (`RXC`/`UDRE` swapped).
- Pin direction is configured in two places: the `DIO_GFC.c` `PinCFG[]` table, and ad-hoc `DDRx` writes in `bsp.c` and `PWM_Servo.c`.
- Session auto-logout doesn't work: the idle-timeout check is stranded inside `EEPROM.c`'s `u8GetKeyPressed()`, which nothing calls.
- Two keypad-scan implementations coexist in `Keypad.c` (`keypad_checkpress` and `PrintKey`), duplicating the row/column scan with different read primitives.
- First-boot provisioning stamps the wrong EEPROM "pass set" flag for the Admin PIN, so the device never persists that it has been provisioned.

## Author

Amgad Bassam
