/*
 * session.h
 *
 * Login session-timeout clock. Owns the Timer0-driven tick counter and the
 * timeout flag, and wraps the timer_driver start/stop calls.
 *
 * The underlying globals (session_counter / timeout_flag) keep their original
 * names so the legacy EEPROM.c helper (u8GetKeyPressed) still links against
 * them via extern.
 */


#ifndef SESSION_H_
#define SESSION_H_

#include "STD_Types.h"

/* Start the ~10 ms Timer0 tick and begin counting the active session. */
void Session_Start(void);

/* Stop the Timer0 tick (no more ticks accumulate). */
void Session_Stop(void);

/* Reset the elapsed-tick counter back to zero. */
void Session_Reset(void);

/* TRUE once the active session has exceeded its allowed idle time. */
uint8 Session_TimedOut(void);

/* Clear the timeout flag after it has been handled. */
void Session_ClearTimeout(void);

#endif /* SESSION_H_ */