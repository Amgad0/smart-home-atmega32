/*
 * ADC.h
 *
 * Created: 10/22/2021 9:23:55 PM
 *  Author: Amgad Bassam
 */


#ifndef ADC_H_
#define ADC_H_

#include "LCD.h"

/*
	Function Name        : ADC_Init
	Function Returns     : void
	Function Arguments   : void
	Function Description : Configure the ADC for single-conversion mode using the internal reference and a /128 clock prescaler.
*/
void ADC_Init();

/*
	Function Name        : ADC_Read
	Function Returns     : unsigned short
	Function Arguments   : unsigned char channel
	Function Description : Select the given ADC channel, run a single conversion, and return the 10-bit result.
*/
unsigned short ADC_Read(unsigned char channel);



#endif /* ADC_H_ */