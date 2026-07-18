/*
 * UART.h
 *
 * Created: 12/15/2021 3:53:44 PM
 *  Author: Amgad Bassam
 */


#ifndef UART_H_
#define UART_H_

#include "avr/io.h"
#define F_CPU 16000000UL

#define BR 9600

#ifndef DoubleSpeed
#define UBRR ((F_CPU/(BR*16UL))-1)
#else
#define UBRR ((F_CPU/(BR*8UL))-1)
#endif

/*
	Function Name        : UART_Init
	Function Returns     : void
	Function Arguments   : void
	Function Description : Configure the USART for 9600 baud, 8N1, with the RX-complete interrupt enabled.
*/
void UART_Init(void);

/*
	Function Name        : UART_Send
	Function Returns     : void
	Function Arguments   : unsigned char info
	Function Description : Wait for the transmit data register to be empty (UDRE), then send one byte.
*/
void UART_Send(unsigned char info);

/*
	Function Name        : UART_Recieve
	Function Returns     : unsigned char
	Function Arguments   : void
	Function Description : Wait for a byte to be received (RXC), then return it.
*/
unsigned char UART_Recieve(void);

/*
	Function Name        : UART_String
	Function Returns     : void
	Function Arguments   : char *string
	Function Description : Send a null-terminated string over UART one byte at a time.
*/
void UART_String(char *string);




#endif /* UART_H_ */