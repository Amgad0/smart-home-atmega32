/*
 * DIO.h
 *
 * Created: 10/8/2021 9:49:20 PM
 *  Author: Amgad Bassam
 */


#ifndef DIO_H_
#define DIO_H_

#include "STD_Types.h"
#include "BitMath.h"
#include "DIO_HW.h"
#include "DIO_Types.h"
#include "DIO_GFC.h"

/*
	Function Name        : DIO_Write
	Function Returns     : void
	Function Arguments   : DIO_ChannelTypes ChannelID, STD_LevelTypes Level
	Function Description : Drive the given channel's pin high or low.
*/
void DIO_Write(DIO_ChannelTypes ChannelID,STD_LevelTypes Level);

/*
	Function Name        : DIO_Read
	Function Returns     : STD_LevelTypes
	Function Arguments   : DIO_ChannelTypes ChannelID
	Function Description : Read the current level of the given channel's pin.
*/
STD_LevelTypes DIO_Read(DIO_ChannelTypes ChannelID);

/*
	Function Name        : DIO_Toggle
	Function Returns     : void
	Function Arguments   : DIO_ChannelTypes ChannelID
	Function Description : Invert the current output level of the given channel's pin.
*/
void DIO_Toggle(DIO_ChannelTypes ChannelID);

#endif /* DIO_H_ */