/*
* DIO_GFC.c
*
* Created: 10/8/2021 10:47:28 PM
*  Author: user
*/

#include "DIO_GFC.h"

/*
 * Single source of truth for every pin's direction and reset level.
 * DIO_Init() walks this table once at boot to program DDRA..DDRD. No other
 * module should set DDRx directly — output pins used elsewhere (servo PD5,
 * fan/OC2 PD7, AC/fan relays PB5-PB7, lamps PD2/PD3, PA1) are declared Output
 * here, so a pin remap is a one-line edit in this table.
 */
DIO_PinCFG PinCFG[] = {
	//PORTA
	{Input,STD_High},
	{Output,STD_High},
	{Output,STD_High},
	{Output,STD_High},
	{Output,STD_High},
	{Output,STD_High},
	{Output,STD_High},
	{Output,STD_High},
	
	//PORTB
	{Output,STD_High},
	{Output,STD_High},
	{Output,STD_High},
	{Output,STD_High},
	{Output,STD_High},
	{Output,STD_High},
	{Output,STD_High},
	{Output,STD_High},
	//PORTC
	{Output,STD_High},//r0
	{Output,STD_High},//r1
	{Output,STD_High},//r2
	{Output,STD_High},//r3
	{Input,STD_Low},//c0
	{Input,STD_Low},//c1
	{Input,STD_Low},//c2
	{Output,STD_High},
	//PORTD
	{Input,STD_High},
	{Output,STD_High},
	{Output,STD_High},
	{Output,STD_High},
	{Output,STD_High},
	{Output,STD_High},
	{Output,STD_High},
	{Output,STD_High}
};

void DIO_Init(){
	DIO_ChannelTypes count = 0;
	DIO_PortTypes Portx;
	DIO_ChannelTypes ChannelPos;
	for(count=DIO_ChannelA0;count<PINCOUNT;count++){
		Portx = count/8;
		ChannelPos = count%8;
		switch(Portx){
			case DIO_PortA:
			if(PinCFG[count].PinDir == Output){
				SetBit(DDRA_Reg,ChannelPos);
			}
			else
			{
				ClearBit(DDRA_Reg,ChannelPos);
			}
			break;
			case DIO_PortB:
			if(PinCFG[count].PinDir == Output){
				SetBit(DDRB_Reg,ChannelPos);
			}
			else
			{
				ClearBit(DDRB_Reg,ChannelPos);
			}
			break;
			case DIO_PortC:
			if(PinCFG[count].PinDir == Output){
				SetBit(DDRC_Reg,ChannelPos);
			}
			else
			{
				ClearBit(DDRC_Reg,ChannelPos);
			}
			break;
			case DIO_PortD:
			if(PinCFG[count].PinDir == Output){
				SetBit(DDRD_Reg,ChannelPos);
			}
			else
			{
				ClearBit(DDRD_Reg,ChannelPos);
			}
			break;
		}
	}

}