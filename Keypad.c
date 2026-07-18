/*
* Keypad.c
*
* Created: 12/21/2021 1:24:55 AM
*  Author: youssef rashad
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
Uint8 keyindic = 0;
void keypad_init()
{
	for(Uint8 x=0;x<3;x++)
	{
		DIO_Write(keypad_row[x],STD_High);
		DIO_Write(keypad_col[x],STD_High);
	}
}
Uint8 PrintKey(void)
{
	for (Uint8 i=0;i<4;i++)
	{
		DIO_Write(keypad_row[i],STD_Low);
		for (Uint8 j=0;j<3;j++)
		{
			if(DIO_Read(keypad_col[j])==STD_Low)
			{
				while(DIO_Read(keypad_col[j])==STD_Low);
				return keypad[i][j];
			}
		}
		DIO_Write(keypad_row[i],STD_High);



	}
	return 0;
}

Uint8 keypad_checkpress()
{
	Uint8 returnval=notpressed;
	Uint8 row;
	Uint8 col;
	for(row=0;row<4;row++)
	{

		/*DIO_Write(row,0);
		DIO_Write(DIO_ChannelC1,1);
		DIO_Write(DIO_ChannelC2,1);
		DIO_Write(DIO_ChannelC3,1);
		*/

		//_delay_ms(20);
		DIO_Write(keypad_row[row],STD_Low);
		_delay_ms(20);
		for (col=0;col<3;col++)
		{//'C'
			keyindic= DIO_u8read('C', (col+4));
			if (keyindic==0)
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