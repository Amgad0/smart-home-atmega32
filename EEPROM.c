#include "EEPROM.h"
#include <avr/io.h>
extern volatile uint16 session_counter ;//indicate session time
extern uint8 timeout_flag ;//stores if the session is still valid or outdated

uint8 EEPROM_ui8ReadByteFromAddress(uint16 u16Address)
{
	/* Wait for completion of previous write */
	while (((EECR & (1 << EEWE))>>EEWE)==1)
	;
	/* Set up address register */
	EEAR = u16Address;
	/* Start EEPROM read by writing EERE */
	EECR |= (1 << EERE);
	/* Return data from data register */
	return EEDR;
}

void EEPROM_vReadBlockFromAddress(uint16 uiAddress, uint8* dist,uint16 size)
{
	/* Wait for completion of previous write */
	while (((EECR & (1 << EEWE))>>EEWE)==1)
	;

	uint16 counter = 0;//the counter of bytes that are read

	while (counter < size) //loop until move all bytes to the given array
	{
		/* Set up address register */
		EEAR = uiAddress + counter;
		/* Start EEPROM read by writing EERE */
		EECR |= (1 << EERE);
		/* move data from data register to the array */
		*(dist + counter) = EEDR;

		counter++;//increase the bytes counter
	}
}

void EEPROM_vWriteByteToAddress(uint16 uiAddress, uint8 uiData)
{

	/* Wait for completion of previous write process*/
	while ( ( (EECR & (1 << EEWE) ) >>EEWE ) == 1)
	;
	/* Set up address register */
	EEAR = uiAddress;

	/* Read the byte in the address of EEAR */
	EECR |= (1 << EERE);
	if (EEDR != uiData)//compare the value read to the value to be written
	{//if they are not equal then write the data
		EEDR = uiData;//move the data to EEDR

		/* Write logical one to EEMWE */
		EECR |= (1 << EEMWE);
		/* Start EEPROM write by setting EEWE */
		EECR |= (1 << EEWE);
	}
	else
	{

	}

}

void EEPROM_vWriteBlockToAddress(uint16 uiAddress, uint8* uiData,uint16 size)
{
	uint16 counter = 0;//Bytes write counter
	while (counter < size)
	{
		/* Wait for completion of previous write process*/
		while (( (EECR & (1 << EEWE) ) >>EEWE ) == 1)
		;
		/* Set up address register */
		EEAR = uiAddress + counter;

		/* Read the byte in the address of EEAR */
		EECR |= (1 << EERE);
		if (EEDR != (*(uiData + counter)))//compare the value read to the value to be written
		{//if they are not equal then write the data
			EEDR = *(uiData + counter);//move the data to EEDR

			/* Write logical one to EEMWE */
			EECR |= (1 << EEMWE);
			/* Start EEPROM write by setting EEWE */
			EECR |= (1 << EEWE);
		}
		else
		{

		}
		counter++;//increase bytes counter
	}
}
/* ui8ComparePass() moved to the auth module as Auth_ComparePin(). */

uint8 u8GetKeyPressed(uint8 u8LoginMode)
{
	uint8 key_pressed = notpressed;
	while (key_pressed == notpressed)//repeat till the user press any key
	{
		if ( (session_counter>= ADMIN_TIMEOUT && u8LoginMode == ADMIN) || ( session_counter >= GUEST_TIMEOUT && u8LoginMode == GUEST ))//check for timeout
		{
			timeout_flag = TRUE;//set timeout flag to true
			break;//break the loop that wait for input from the user
		}

		key_pressed = keypad_checkpress();//if the user pressed any button in keypad save the value in key_pressed
	}
	return key_pressed;
}