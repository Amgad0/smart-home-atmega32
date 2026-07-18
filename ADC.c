/*
 * ADC.c
 *
 * Created: 10/22/2021 9:23:43 PM
 *  Author: amgad
 */ 

#include "ADC.h"
#include <avr/io.h>

void ADC_Init()
{
	ADMUX = (1<<REFS0) | (1<<REFS1); //select use internal Vref
	ADCSRA = (1<<ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2); //enable ADC and divide MC clock by 128
}
unsigned short ADC_Read(unsigned char channel)
{
	unsigned short data = 0;
	ADMUX = (ADMUX & 0xE0) | (channel & 0x1f); //1st AND put zeros in the first 5 bits and leave the last 3 unchanged
	//2nd AND take the first 5 bits only
	ADCSRA |= (1<<ADSC); //start conversion
	while(!(ADCSRA>>ADIF)&1); //read interrupt flag and stay at this line while it's 0 
	ADCSRA |= (1<<ADIF); //clear ADIF bit
	data = ADCL;
	data |= (ADCH<<8);
	return data;
}