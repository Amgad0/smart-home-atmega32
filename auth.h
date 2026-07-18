/*
 * auth.h
 *
 * Access control: first-boot PIN provisioning, masked PIN entry, and the
 * login / 3-strike-lockout state machine. Owns the lockout flag and drives the
 * keypad + LCD + EEPROM to authenticate an Admin or Guest session.
 *
 * The constant-time-unsafe PIN comparison (Auth_ComparePin) previously lived in
 * the EEPROM driver; it belongs here with the rest of the authentication logic.
 */


#ifndef AUTH_H_
#define AUTH_H_

#include "STD_Types.h"

/*
 * Boot-time entry point: on first boot (PINs not yet stored) walk the installer
 * through provisioning; otherwise restore the persisted lockout state.
 */
void Auth_Begin(void);

/* TRUE if both Admin and Guest PINs have already been stored in EEPROM. */
uint8 Auth_IsProvisioned(void);

/* First-boot flow: prompt for and store the Admin and Guest PINs. */
void Auth_Provision(void);

/*
 * Blocking masked PIN entry: reads PASS_SIZE keypad digits into out_pin,
 * echoing each digit briefly before overwriting it with the password symbol.
 */
void Auth_ReadPin(uint8* out_pin);

/* Byte-wise PIN compare; returns TRUE when the two PINs match. */
uint8 Auth_ComparePin(uint8* pin1, uint8* pin2, uint8 size);

/*
 * Run the login state machine until a session is granted. Handles mode
 * selection, PIN checking, and lockout. Returns ADMIN or GUEST and starts the
 * session-timeout clock on success.
 */
uint8 Auth_Login(void);

#endif /* AUTH_H_ */