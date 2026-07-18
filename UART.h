/*
 * UART.h
 *
 * Created: 12/15/2021 3:53:44 PM
 *  Author: Wafeek
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

void UART_Init(void);
void UART_Send(unsigned char info);
unsigned char UART_Recieve(void);
void UART_String(char *string);




#endif /* UART_H_ */