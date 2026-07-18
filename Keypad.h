/*
 * Keypad.h
 *
 * Created: 12/21/2021 1:24:27 AM
 *  Author: Amgad Bassam
 */


#ifndef KEYPAD_H_
#define KEYPAD_H_
#define notpressed 0xFF
#include "DIO.h"
#define r0 DIO_ChannelC0
#define r1 DIO_ChannelC1
#define r2 DIO_ChannelC2
#define r3 DIO_ChannelC3

#define c0 DIO_ChannelC4
#define c1 DIO_ChannelC5
#define c2 DIO_ChannelC6

/*
	Function Name        : keypad_init
	Function Returns     : void
	Function Arguments   : void
	Function Description : Set all keypad row and column lines high (idle) before scanning begins.
*/
void keypad_init();

/*
	Function Name        : keypad_checkpress
	Function Returns     : Uint8
	Function Arguments   : void
	Function Description : Scan the 4x3 matrix keypad and return the character of the pressed key, blocking until it is released; returns notpressed if no key is down.
*/
Uint8 keypad_checkpress();




#endif /* KEYPAD_H_ */