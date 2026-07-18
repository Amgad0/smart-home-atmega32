/*
 * remote.h
 *
 * UART remote-control command handling for a paired serial/Bluetooth companion
 * app. Owns the last-received byte (latched by the USART RX-complete ISR) and
 * dispatches it to the door servo / lamp relays, gated by the active role.
 */


#ifndef REMOTE_H_
#define REMOTE_H_

#include "STD_Types.h"

/*
	Function Name        : Remote_Dispatch
	Function Returns     : void
	Function Arguments   : uint8 login_mode
	Function Description : Act on the most recently received UART command byte for the given login mode (ADMIN / GUEST / NO_MODE). Does nothing when no session is active or when the byte is not a command for that role.
*/
void Remote_Dispatch(uint8 login_mode);

#endif /* REMOTE_H_ */