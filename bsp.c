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

	/* PWM timers + door servo */
	Timer1_ServoPWM();
	Timer2_PhasePWM();

	UART_Init();
	Servo(DOOR_CLOSED_ANGLE);

	/* DIO / display / keypad */
	DIO_Init();
	LCD_Init();
	keypad_init();

	/* Output direction bits (these mirror the DIO_GFC PinCFG table; kept here
	 * because the original firmware set them explicitly at this point). */
	SetBit(DDRD, MOTOR_BIT);            /* door servo output      */
	SetBit(DDRB, AC_FAN_BIT);           /* AC / fan relay outputs */
	SetBit(DDRB, AC_RELAY1_BIT);
	SetBit(DDRB, AC_RELAY2_BIT);

	ADC_Init();

	ICR1 = SERVO_ICR1_TOP;              /* Timer1 Fast-PWM top for the servo */

	SetBit(DDRD, LAMP1_BIT);            /* lamp relay outputs */
	SetBit(DDRD, LAMP2_BIT);

	SetBit(DDRA, 1);                    /* PA1 output (matches PinCFG) */
}