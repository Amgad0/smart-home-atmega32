/*
 * Keypad.h
 *
 * Created: 12/21/2021 1:24:27 AM
 *  Author: youssef rashad
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
void keypad_init();
Uint8 keypad_checkpress();




#endif /* KEYPAD_H_ */