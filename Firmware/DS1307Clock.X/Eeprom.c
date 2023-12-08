/*
 * File:   Eeprom.c
  *
 * Created: 30/08/2023 
 * Author: Boris
 */
#include "Config.h"
#include "Eeprom.h"

/********************************************************************
*     Function Name:    BusyEeprom                                    *
*     Return Value:     None										*
*     Parameters:       None										*
*     Description:      Checks & waits the status of ER bit in 		*
						EECON1 register     						*
********************************************************************/
void BusyEeprom ( void )
{
	while(EECON1bits.WR);
}

/********************************************************************
*     Function Name:    ReadEeprom                                 *
*     Return Value:     contents of EEDATA register                *
*     Parameters:       unsigned int add                           *
*     Description:      Read single byte from Internal EEP         *
********************************************************************/
unsigned char ReadEeprom( unsigned int address )
{
	EEADR = (address & 0x0ff);
	EECON1bits.CFGS = 0;
	EECON1bits.EEPGD = 0;
	EECON1bits.RD = 1;
	Nop();							//Nop may be required for latency at high frequencies
	Nop();							//Nop may be required for latency at high frequencies
	return ( EEDATA );              // return with read byte 
}

/********************************************************************
*     Function Name:    Write_b_eep                                 *
*     Return Value:     None										*
*     Parameters:       unsigned int add, unsigned char data        *
*     Description:      Write single byte to Internal EEP           *
********************************************************************/
void WriteEeprom( unsigned int address, unsigned char data )
{
	char GIE_BIT_VAL = 0;
	EEADR = (address & 0x0ff);
	EEDATA = data;
  	EECON1bits.EEPGD = 0;
	EECON1bits.CFGS = 0;
	EECON1bits.WREN = 1;
	GIE_BIT_VAL = INTCONbits.GIE;
	INTCONbits.GIE = 0;
	EECON2 = 0x55;
	EECON2 = 0xAA;
	EECON1bits.WR = 1;
	while(EECON1bits.WR);				//Wait till the write completion
	INTCONbits.GIE = GIE_BIT_VAL;
	EECON1bits.WREN = 0;
}