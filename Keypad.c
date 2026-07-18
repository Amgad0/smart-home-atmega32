/*
* Keypad.c
*
* Created: 12/21/2021 1:24:55 AM
*  Author: Amgad Bassam
*/
#include "Keypad.h"
#define F_CPU 16000000UL
#include <util/delay.h>
#include "DIO_GFC.h"

Uint8 keypad_row[4]={r0,r1,r2,r3};
Uint8 keypad_col[3]={c0,c1,c2};
Uint8 keypad[4][3]={{'1','2','3'},
                    {'4','5','6'},
                    {'7','8','9'},
                    {'*','0','#'}};
void keypad_init()
{
	for(Uint8 x=0;x<3;x++)
	{
		DIO_Write(keypad_row[x],STD_High);
		DIO_Write(keypad_col[x],STD_High);
	}
}

/*
 * Single keypad scan routine (replaces the former separate PrintKey()/
 * keypad_checkpress() implementations, which duplicated this row/column scan
 * with two different read primitives). Drives one row low at a time, reads
 * columns via the channel-based DIO_Read(), and blocks until the key is
 * released before returning, to avoid re-triggering on the next scan.
 */
Uint8 keypad_checkpress()
{
	Uint8 returnval=notpressed;
	Uint8 row;
	Uint8 col;
	for(row=0;row<4;row++)
	{
		DIO_Write(keypad_row[row],STD_Low);
		_delay_ms(20);
		for (col=0;col<3;col++)
		{
			if (DIO_Read(keypad_col[col])==STD_Low)
			{
				while(DIO_Read(keypad_col[col]) == STD_Low);
				returnval=keypad[row][col];
				break;
			}
		}
		DIO_Write(keypad_row[row],STD_High);

	}
	return returnval;
}