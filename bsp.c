/*
 * bsp.c
 *
 * Board support / peripheral bring-up. See bsp.h.
 *
 * The init order is preserved from the original main(): the servo/appliance PWM
 * timers and Servo(0) are configured before ICR1 (the Timer1 top) is set.
 */

#include "bsp.h"
#include "main_config.h"
#include "Bitmath.h"
#include "PWM_Servo.h"
#include "UART.h"
#include "DIO_GFC.h"
#include "LCD.h"
#include "Keypad.h"
#include "ADC.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void BSP_Init(void)
{
	sei();

	/* Pin directions first: DIO_Init() applies the DIO_GFC PinCFG table, which
	 * is the single source of truth for every pin's direction (servo, AC/fan
	 * relays, lamps, keypad, LCD, ADC inputs). Peripherals are configured after,
	 * so their output pins are already set up by the time they are driven. */
	DIO_Init();

	/* PWM timers + door servo (timer config only; pins owned by PinCFG above) */
	Timer1_ServoPWM();
	Timer2_PhasePWM();

	UART_Init();
	Servo(DOOR_CLOSED_ANGLE);

	/* Display / keypad */
	LCD_Init();
	keypad_init();

	ADC_Init();

	ICR1 = SERVO_ICR1_TOP;              /* Timer1 Fast-PWM top for the servo */
}