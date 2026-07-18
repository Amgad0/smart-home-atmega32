# Smart Home Access Control & Automation System (ATmega32)

Bare-metal C firmware for an **ATmega32**-based smart home controller: PIN-code access control (Admin/Guest roles) via a matrix keypad and character LCD, temperature-driven climate control, potentiometer-driven appliance dimming, a servo-actuated door lock, and remote control over UART (e.g. from a Bluetooth-connected mobile app).

**Every peripheral driver is written from scratch, register by register, from the ATmega32 datasheet** — GPIO, HD44780 LCD, matrix keypad, ADC, internal EEPROM, PWM/servo, timers, and USART. No vendor HAL, no Atmel Software Framework (ASF), no Arduino, no RTOS, and no external libraries beyond `avr-libc`. The register-level drivers are layered under a small application (access control, climate, remote) so the same primitives are reused across features.

## Table of contents

- [Features](#features)
- [System behavior](#system-behavior)
- [Hardware & wiring](#hardware--wiring)
- [Firmware architecture](#firmware-architecture)
- [Driver layer reference](#driver-layer-reference)
- [EEPROM memory map](#eeprom-memory-map)
- [Building & flashing](#building--flashing)
- [Design highlights](#design-highlights)
- [Author](#author)

## Features

- **PIN-code authentication** with two roles (Admin / Guest), 4-digit PINs persisted in internal EEPROM, first-boot provisioning flow, and a 3-strike lockout with a timed cooldown.
- **Session timeout** — an idle logged-in session is automatically ended by a Timer0-driven ~10 ms tick counter.
- **Temperature-based climate control** — reads a sensor on the ADC and switches an AC/relay output on/off past configurable thresholds.
- **Analog appliance dimming** — a potentiometer reading drives a Timer2 PWM output (fan speed / dimmer).
- **Servo-actuated door lock** — Timer1 hardware PWM drives a hobby servo between two fixed angles (locked/unlocked), restricted to the Admin role.
- **Remote control over UART** — single-byte commands (e.g. from a Bluetooth serial module) toggle lamps and the door lock, with different command sets available to Admin vs. Guest sessions.
- **From-scratch peripheral drivers** for GPIO, character LCD (HD44780, 4-bit mode), matrix keypad, ADC, EEPROM, PWM/servo, timers, and USART — see below.

## System behavior

1. **First boot / provisioning** — if no PINs are stored yet, the LCD walks the installer through setting a 4-digit Admin PIN and a 4-digit Guest PIN, both written to EEPROM.
2. **Login** — the user picks Admin or Guest on the keypad and enters a 4-digit PIN, masked on the LCD as it's typed. A correct PIN starts a timed session; 3 consecutive wrong PINs locks out login for a fixed cooldown period.
3. **Active session** — while logged in, the firmware continuously:
   - reads the temperature sensor and switches the AC relay past fixed thresholds,
   - reads the potentiometer and updates the PWM appliance/fan output,
   - watches for UART command bytes from a paired remote (mobile app / Bluetooth module) and drives the door servo or lamp relays — the accepted command set depends on whether the session is Admin or Guest,
   - echoes keypad presses to the LCD.
4. **Timeout** — once the session clock exceeds the role's configured timeout, the session ends and the firmware returns to the login prompt.

## Hardware & wiring

| Function | MCU pin(s) | Notes |
|---|---|---|
| LCD data (4-bit) | `PORTA` upper nibble | HD44780-compatible character LCD |
| LCD control (RS / RW / E) | `PB1` / `PB2` / `PB0` | |
| Keypad rows (4) | `PC0–PC3` | driven low one at a time to scan |
| Keypad columns (3) | `PC4–PC6` | 4×3 matrix keypad |
| Door lock servo | `PD5` (Timer1 `OC1A`) | hardware PWM, angle mapped to `OCR1A` |
| Appliance/fan PWM | `PD7` (Timer2 `OC2`) | duty cycle driven by potentiometer reading |
| AC relay output | `PORTB5–PORTB7` | switched by temperature thresholds |
| Lamp relays | `PD2`, `PD3` | toggled by UART remote commands |
| Temperature sensor | ADC channel 0 | polled, scaled in `main.c` |
| Potentiometer | ADC channel 2 | polled, feeds `PWM_duty_Timer2` |
| UART (remote link) | USART, 9600 baud | e.g. Bluetooth serial module for app control |

MCU: ATmega32, `F_CPU = 16 MHz` (external crystal — configure programmer fuses accordingly; fuse bits are not stored in this project).

## Firmware architecture

The driver layer is split in two styles:

- A **channel-based DIO abstraction** (`DIO_*` files) that lets higher-level drivers address any pin as a `DIO_ChannelTypes` enum value instead of raw port/bit pairs, with pin directions declared once in a static configuration table.
- **Direct-register drivers** (ADC, EEPROM, PWM/Timer, UART) that talk to AVR SFRs directly, since their timing/PWM/interrupt requirements don't map cleanly onto a generic pin abstraction.

Above that sits an **application layer** split into small, single-purpose modules — `main.c` is now just the orchestration loop that calls into them each pass, rather than one 460-line state machine:

```
main.c  (orchestration loop)
   |
   |-- bsp.c        -- one-shot peripheral bring-up (BSP_Init)
   |-- auth.c       -- provisioning, PIN entry, login/lockout state machine
   |-- session.c    -- session-timeout clock (owns TIMER0_COMP_vect)
   |-- climate.c     -- temperature -> AC relay, potentiometer -> fan PWM
   |-- remote.c     -- UART command dispatch (owns USART_RXC_vect)
   |
   |-- Keypad.c / LCD.c            -- built on the DIO abstraction
   |       |
   |       '-- DIO.c -- DIO_GFC.c -- DIO_TYPES.h -- DIO_HW.h
   |
   |-- ADC.c, EEPROM.c, PWM_Servo.c, timer_driver.c, UART.c
   |       -- direct AVR register access (avr/io.h)
   |
   '-- STD_Types.h, Bitmath.h, main_config.h  -- shared types / bit macros / app constants
```

Each application module owns any global state and ISR its concern needs (e.g. `session.c` owns the timeout counter and its Timer0 ISR, `remote.c` owns the last-received UART byte and its RX ISR), so `main.c` itself has no `extern` globals.

## Driver layer reference

| File | Role |
|---|---|
| [`STD_Types.h`](STD_Types.h) | Project-wide fixed-width typedefs (`uint8`, `uint16`, `sint8`, `float64`, `STD_LevelTypes`) used instead of `stdint.h` throughout. |
| [`Bitmath.h`](Bitmath.h) | `SetBit` / `ClearBit` / `ToggleBit` / `GetBit` register-manipulation macros used by every driver. |
| [`DIO_HW.h`](DIO_HW.h) | Memory-mapped addresses of `PORTx`/`DDRx`/`PINx` for ports A–D — the bottom of the DIO stack. |
| [`DIO_TYPES.h`](DIO_TYPES.h) | `DIO_ChannelTypes` (32 values spanning ports A–D), `DIO_PortTypes`, `DIO_DirTypes` enums used by the DIO API. |
| [`DIO.c`](DIO.c) / [`DIO.h`](DIO.h) | Channel-addressed GPIO driver: `DIO_Write`, `DIO_Read`, `DIO_Toggle` decode a `DIO_ChannelTypes` value into port + bit (`channel / 8`, `channel % 8`). |
| [`DIO_GFC.c`](DIO_GFC.c) / [`DIO_GFC.h`](DIO_GFC.h) | Declarative pin-configuration table (`PinCFG[32]`) listing every pin's direction/reset level; `DIO_Init()` programs all four `DDRx` registers from it at boot. |
| [`Keypad.c`](Keypad.c) / [`Keypad.h`](Keypad.h) | 4×3 matrix keypad driver (rows `C0–C3`, columns `C4–C6`) built on the DIO layer; scans rows and detects a pressed column, mapping to digits `0–9`, `*`, `#`. |
| [`LCD.c`](LCD.c) / [`LCD.h`](LCD.h) | HD44780 character LCD driver in 4-bit mode, built on the DIO layer; provides command/character writes and row/column cursor positioning. |
| [`ADC.c`](ADC.c) / [`ADC.h`](ADC.h) | Polled single-conversion ADC driver used for the temperature sensor and potentiometer inputs. |
| [`EEPROM.c`](EEPROM.c) / [`EEPROM.h`](EEPROM.h) | Byte/block read-write driver for the ATmega32's internal EEPROM (used to persist PINs and lockout state), plus a timeout-aware keypad-wait helper. |
| [`PWM_Servo.c`](PWM_Servo.c) / [`PWM_Servo.h`](PWM_Servo.h) | Timer1 hardware PWM for the door-lock servo (`Servo(angle)` maps an angle to a pulse width) and Timer2 phase-correct PWM for the appliance/fan duty cycle. |
| [`timer_driver.c`](timer_driver.c) / [`timer_driver.h`](timer_driver.h) | Timer0 CTC-mode driver producing the ~10 ms tick used for session-timeout tracking. |
| [`UART.c`](UART.c) / [`UART.h`](UART.h) | USART driver (9600 baud, RX-complete interrupt) for the remote-control link. |
| [`main_config.h`](main_config.h) | Application-level constants: EEPROM address map, PIN length/retry limits, keypad command codes, session timeout durations, climate/servo/lamp thresholds, and UART command codes. |
| [`bsp.c`](bsp.c) / [`bsp.h`](bsp.h) | `BSP_Init()` — the single peripheral bring-up call (interrupts, PWM timers, UART, ADC, DIO/LCD/keypad, and the output-pin direction bits). |
| [`auth.c`](auth.c) / [`auth.h`](auth.h) | First-boot provisioning, masked PIN entry, PIN comparison (`Auth_ComparePin`), and the login/lockout state machine (`Auth_Login`). |
| [`session.c`](session.c) / [`session.h`](session.h) | Session-timeout clock — owns the tick counter, the timeout flag, and the Timer0 compare-match ISR. |
| [`climate.c`](climate.c) / [`climate.h`](climate.h) | `Climate_Update()` (temperature → AC relay) and `Fan_Update()` (potentiometer → fan/dimmer PWM). |
| [`remote.c`](remote.c) / [`remote.h`](remote.h) | UART remote-command dispatch (`Remote_Dispatch`) — owns the latest received byte and the USART RX-complete ISR. |
| [`main.c`](main.c) | The orchestration loop: bring up the board, provision or restore login state, then repeatedly service climate/fan, session timeout, login, and remote commands. |

## EEPROM memory map

| Address | Contents |
|---|---|
| `0x100` | Admin PIN "is set" flag |
| `0x101` | Guest PIN "is set" flag |
| `0x102–0x105` | Admin PIN (4 bytes) |
| `0x106–0x109` | Guest PIN (4 bytes) |
| `0x10A` | Login-blocked flag (lockout state) |

## Building & flashing

This is an Atmel Studio / Microchip Studio 7 project — there is no standalone Makefile.

1. Open [`Project.atsln`](Project.atsln) in Atmel Studio 7 or Microchip Studio.
2. Select the `Debug` or `Release` configuration and build (**F7**). Output (`.elf`, `.hex`, `.eep`, `.lss`, `.srec`) is written to `Debug/` or `Release/`.
3. Flash the resulting `.hex` to an ATmega32 with your programmer of choice (AVRISP, USBasp, etc.), and configure fuses for an external 16 MHz crystal to match `F_CPU`.

**Headless build:** a bare `msbuild Project.cproj` does not work with this project type — Atmel Studio's makefile generator needs the full IDE host environment. Use the IDE executable's build switch instead:
```
"C:\Program Files (x86)\Atmel\Studio\7.0\AtmelStudio.exe" "Project.atsln" /build Debug /out build.log
```
Also keep the project path free of non-ASCII characters — the bundled `avr-size`/memory-usage reporting silently breaks (shows 0% used, even though the build and `.hex` are fine) on a path with characters outside the system code page.

## Design highlights

A few engineering decisions worth calling out beyond the feature list:

- **Layered, register-level HAL.** A channel-based DIO abstraction (`DIO_Read`/`DIO_Write`/`DIO_Toggle`, addressing any pin as a single `DIO_ChannelTypes` value) sits over hand-mapped register definitions, and the higher-level drivers (keypad, LCD) are built on top of it — so the same primitives are reused rather than re-poking registers everywhere.
- **Single source of truth for pin configuration.** Every pin's direction and reset level is declared once in a `PinCFG[]` table that `DIO_Init()` walks at boot; re-targeting a pin is a one-line table edit, with no scattered `DDRx` writes to keep in sync.
- **Single-responsibility application modules.** The app logic is split into small units — board bring-up (`bsp`), access control (`auth`), the session-timeout clock (`session`), climate/fan (`climate`), and UART remote control (`remote`) — each owning its own state and ISR, orchestrated by a compact `main()` super-loop. No dynamic memory and no RTOS: deterministic, bare-metal control flow.
- **Security-minded access control.** 4-digit PINs persisted in internal EEPROM with first-boot provisioning, masked PIN entry on the LCD, a 3-strike lockout with a timed cooldown that survives power cycles, and an idle **auto-logout** driven by a Timer0 tick that re-locks an unattended session.
- **Hardware-timer peripherals.** Timer1 Fast-PWM positions the door-lock servo, Timer2 phase-correct PWM drives the analog appliance/fan output from an ADC reading, and Timer0 CTC provides the session-timeout time base — three independent timers, each configured directly from the datasheet.

## Author

Amgad Bassam