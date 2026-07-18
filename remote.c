/*
 * remote.c
 *
 * UART remote-control command handling. See remote.h.
 */

#define F_CPU 16000000UL
#include "remote.h"
#include "main_config.h"
#include "PWM_Servo.h"
#include "Bitmath.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* Latest byte received over UART, latched by the RX-complete ISR below. */
volatile unsigned char UartV = 0;

static void Remote_HandleAdmin(uint8 cmd)
{
	switch (cmd)
	{
		case CMD_ADMIN_DOOR_OPEN:
		Servo(DOOR_OPEN_ANGLE);
		break;
		case CMD_ADMIN_DOOR_CLOSE:
		Servo(DOOR_CLOSED_ANGLE);
		break;
		case CMD_ADMIN_LAMP1_TOGGLE:
		ToggleBit(PORTD, LAMP1_BIT);
		_delay_ms(LAMP_TOGGLE_DELAY_MS);
		break;
		case CMD_ADMIN_LAMP2_TOGGLE:
		ToggleBit(PORTD, LAMP2_BIT);
		_delay_ms(LAMP_TOGGLE_DELAY_MS);
		break;
		default:
		break;
	}
}

static void Remote_HandleGuest(uint8 cmd)
{
	switch (cmd)
	{
		case CMD_GUEST_LAMP1_TOGGLE:
		ToggleBit(PORTD, LAMP1_BIT);
		_delay_ms(LAMP_TOGGLE_DELAY_MS);
		break;
		case CMD_GUEST_LAMP2_TOGGLE:
		ToggleBit(PORTD, LAMP2_BIT);
		_delay_ms(LAMP_TOGGLE_DELAY_MS);
		break;
		default:
		break;
	}
}

void Remote_Dispatch(uint8 login_mode)
{
	uint8 cmd = UartV;

	if (login_mode == ADMIN)
	{
		Remote_HandleAdmin(cmd);
	}
	else if (login_mode == GUEST)
	{
		Remote_HandleGuest(cmd);
	}
}

ISR(USART_RXC_vect)
{
	UartV = UDR;
}