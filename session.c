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
 * Kept as plain (non-static) globals with these exact names so EEPROM.c's
 * u8GetKeyPressed(), which declares them `extern`, still resolves at link time.
 * session_counter is incremented from the Timer0 compare-match ISR below.
 */
volatile uint16 session_counter = 0;
uint8 timeout_flag = FALSE;

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