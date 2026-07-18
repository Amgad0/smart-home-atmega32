/*
 * EEPROM.h
 *
 *  Created on: Apr 12, 2018
 *      Author: Amgad Bassam
 */

#ifndef HEADERS_EEPROM_H_
#define HEADERS_EEPROM_H_

#include "STD_Types.h"
#include "main_config.h"
#define notpressed 0xFF

/*
	Function Name        : EEPROM_ui8ReadByteFromAddress
	Function Returns     : uint8
	Function Arguments   : const uint16 uiAddress
	Function Description :  Read one byte from the given  address.
*/
uint8 EEPROM_ui8ReadByteFromAddress(uint16 u16Address);

/*
	Function Name        : EEPROM_vReadBlockFromAddress
	Function Returns     : void
	Function Arguments   : const uint16 uiAddress, uint8* dist,const uint16 size
	Function Description : Read block of bytes of the given size (size) and save the result to given array(dist array).
*/
void EEPROM_vReadBlockFromAddress( uint16 u16Address, uint8* dist,uint16 size);

/*
	Function Name        : EEPROM_vWriteByteToAddress
	Function Returns     : void
	Function Arguments   : const uint16 uiAddress, const uint8 uiData
	Function Description : Write one byte to given address.
*/
void EEPROM_vWriteByteToAddress( uint16 u16Address,  uint8 u8Data);

/*
	Function Name        : EEPROM_vWriteBlockToAddress
	Function Returns     : void
	Function Arguments   : const uint16 uiAddress, const uint8* uiData,const uint16 size
	Function Description : Write block of bytes  of the given size (size) to EEPROM at the given address(u16Address) from the given array (u8Data).
*/
void EEPROM_vWriteBlockToAddress( uint16 u16Address,uint8* u8Data, uint16 size);
/* PIN comparison moved to the auth module (Auth_ComparePin); the timeout-aware
 * keypad wait (u8GetKeyPressed) was removed in favor of session.c's
 * Session_CheckExpiry(). */
#endif /* HEADERS_EEPROM_H_ */
