/* 
 * File:   Eeprom.h
 *  
 *
 * Created: 30/08/2023 
 * Author: Boris
 */

#ifndef EEPROM_H
#define	EEPROM_H

#ifdef	__cplusplus
extern "C" {
#endif

    void BusyEeprom ( void );
    unsigned char ReadEeprom( unsigned int address );
    void WriteEeprom( unsigned int address, unsigned char data );
    
#ifdef	__cplusplus
}
#endif

#endif	/* EEPROM_H */

