/*
 * LCD.h
 *
 * Created: 12/19/2021 12:12:09 AM
 *  Author: youssef rashad
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
void LCD_Init();
void LCD_Cmd(Uint8 cmd);
void LCD_Char(Uint8 data);
void LCD_String(int8 * string);
void LCD_movecursor(uint8 row,uint8 coloumn);




#endif /* LCD_H_ */