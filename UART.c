/*
 * UART.c
 *
 * Created: 12/15/2021 3:53:10 PM
 *  Author: Wafeek
 */
#include "UART.h"


void UART_Init(void){
	UCSRB |= (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
	UCSRC |= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
	UBRRL=UBRR;
	UBRRH=UBRR>>8;
}
void UART_Send(unsigned char info){
	while(!(UCSRA&(1<<UDRE)));   /* wait until the transmit data register is empty */
	UDR=info;
}
unsigned char UART_Recieve(void){
	while(!(UCSRA&(1<<RXC)));    /* wait until a byte has been received */
	return UDR;
}
void UART_String(char *string){
	unsigned char i=0;
	while(string[i] != '\0'){
		UART_Send(string[i]);
		i++;
	}
}