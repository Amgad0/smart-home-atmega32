/*
 * session.h
 *
 * Login session-timeout clock. Owns the Timer0-driven tick counter and the
 * timeout flag (both file-private to session.c) and wraps the timer_driver
 * start/stop calls. The auto-logout works like this: Session_Start() begins the
 * ~10 ms tick on a successful login; the caller polls Session_CheckExpiry() each
 * loop to raise the timeout flag once the role's budget is exceeded, then acts
 * on Session_TimedOut() to end the session.
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

/* Raise the timeout flag if the active session (login_mode ADMIN or GUEST) has
 * run past its allowed idle time. A no-op when login_mode is NO_MODE. Call once
 * per main-loop iteration while logged in. */
void Session_CheckExpiry(uint8 login_mode);

/* TRUE once the active session has exceeded its allowed idle time. */
uint8 Session_TimedOut(void);

/* Clear the timeout flag after it has been handled. */
void Session_ClearTimeout(void);

#endif /* SESSION_H_ */