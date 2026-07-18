/*
 * LCD.h
 *
 * Created: 12/19/2021 12:12:09 AM
 *  Author: Amgad Bassam
 */


#ifndef LCD_H_
#define LCD_H_
#define F_CPU 16000000UL
#include <util/delay.h>

#include "DIO.h"
#define RS DIO_ChannelB1
#define RW DIO_ChannelB2
#define E DIO_ChannelB0
#define LCD_DataPort PORTA_Reg
/*
	Function Name        : LCD_Init
	Function Returns     : void
	Function Arguments   : void
	Function Description : Initialize the HD44780 LCD in 4-bit mode (function set, entry mode, display on, clear).
*/
void LCD_Init();

/*
	Function Name        : LCD_Cmd
	Function Returns     : void
	Function Arguments   : Uint8 cmd
	Function Description : Send a command byte to the LCD over the 4-bit data bus.
*/
void LCD_Cmd(Uint8 cmd);

/*
	Function Name        : LCD_Char
	Function Returns     : void
	Function Arguments   : Uint8 data
	Function Description : Write a single character to the LCD at the current cursor position.
*/
void LCD_Char(Uint8 data);

/*
	Function Name        : LCD_String
	Function Returns     : void
	Function Arguments   : int8 * string
	Function Description : Write a null-terminated string to the LCD one character at a time.
*/
void LCD_String(int8 * string);

/*
	Function Name        : LCD_movecursor
	Function Returns     : void
	Function Arguments   : uint8 row, uint8 coloumn
	Function Description : Move the LCD cursor to the given row (1 or 2) and column (1-16).
*/
void LCD_movecursor(uint8 row,uint8 coloumn);




#endif /* LCD_H_ */