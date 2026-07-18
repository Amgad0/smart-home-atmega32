/*
 * session.c
 *
 * Login session-timeout clock. See session.h.
 */

#include "session.h"
#include "timer_driver.h"
#include "main_config.h"
#include <avr/interrupt.h>

/*
 * File-private session state. session_counter is incremented from the Timer0
 * compare-match ISR below (hence volatile); timeout_flag is set/cleared only
 * from normal code via the Session_* functions.
 */
static volatile uint16 session_counter = 0;
static uint8 timeout_flag = FALSE;

void Session_Start(void)
{
	timer0_initializeCTC();
}

void Session_Stop(void)
{
	timer0_stop();
}

void Session_Reset(void)
{
	session_counter = 0;
}

void Session_CheckExpiry(uint8 login_mode)
{
	if ((login_mode == ADMIN && session_counter >= ADMIN_TIMEOUT)
		|| (login_mode == GUEST && session_counter >= GUEST_TIMEOUT))
	{
		timeout_flag = TRUE;
	}
}

uint8 Session_TimedOut(void)
{
	return timeout_flag;
}

void Session_ClearTimeout(void)
{
	timeout_flag = FALSE;
}

ISR(TIMER0_COMP_vect)
{
	session_counter++;
}