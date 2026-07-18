/*
 * LCD.c
 *
 * Created: 12/19/2021 12:12:27 AM
 *  Author: youssef rashad
 */ 
#include "LCD.h"
#include "DIO.h"
void LCD_Init()
{
	
	_delay_ms(35);
	DIO_Write(RW,STD_Low);
	DIO_Write(RS,STD_Low);
	LCD_Cmd(0x33);
	_delay_us(100);
	LCD_Cmd(0X32);//lcd init
	LCD_Cmd(0X28);//function set
	LCD_Cmd(0x06);//ENTTRY MODE SET
	LCD_Cmd(0x0C);//display on
	LCD_Cmd(0X01); //CLEAR DISPLAY
	_delay_ms(3);
}
void LCD_Cmd(Uint8 cmd)
{
	LCD_DataPort=(LCD_DataPort&0x0F)|(cmd&0xF0);
	DIO_Write(RS,STD_Low);
	DIO_Write(E,STD_Low);
	_delay_us(50);
	DIO_Write(E,STD_High);
	_delay_ms(20);
	
	LCD_DataPort=(LCD_DataPort&0x0F)|(cmd<<4);
	DIO_Write(RS,STD_Low);
	DIO_Write(E,STD_Low);
	_delay_us(50);
	DIO_Write(E,STD_High);
	_delay_ms(5);
}
void LCD_Char(Uint8 data)
{
	LCD_DataPort=(LCD_DataPort&0x0F)|(data&0xF0);
	DIO_Write(RS,STD_High);
	DIO_Write(E,STD_Low);
	_delay_us(50);
	DIO_Write(E,STD_High);
	_delay_ms(5);
	
	LCD_DataPort=(LCD_DataPort&0x0F)|(data<<4);
	DIO_Write(RS,STD_High);
	DIO_Write(E,STD_Low);
	_delay_us(50);
	DIO_Write(E,STD_High);
	_delay_ms(5);
}
void LCD_String(int8 * string)
{
	Uint8 i=0;
	while(string[i]!= '\0')
	{
		LCD_Char(string[i]);
		i++;
	}
}
void LCD_movecursor(uint8 row,uint8 coloumn)
{
	uint8 data ; // the command which will set the position of the cursor
	/* in case of wrong input set the cursor to the first position of the LCD*/
	if(row>2||row<1||coloumn>16||coloumn<1)
	{
		data=0x80;
	}
	/********************************************/
	/*set the postion of cursor depend on the selected row and coloumn*/
	if(row==1)
	{
		data=0x80+coloumn-1 ;
	}
	else if (row==2)
	{
		data=0xc0+coloumn-1;
	}
	/**************************************/
	
	LCD_Cmd(data); //set the position  of the cursor
}