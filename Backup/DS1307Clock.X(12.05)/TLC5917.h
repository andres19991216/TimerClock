/* 
 * File:   TLC5917.h
 *  
 *
 * Created: 30/08/2023 
 * Author: Boris
 */

#ifndef TLC5917_H
#define	TLC5917_H

#ifdef	__cplusplus
extern "C" {
#endif

    //TLC5917 pin define for SPI
    #define TLC5917_SPI_DATA       PORTAbits.RA0
    #define TLC5917_SPI_CLOCK      PORTAbits.RA1
    #define TLC5917_LED_CS         PORTAbits.RA5
    #define TLC5917_LED_OE         PORTAbits.RA3
    #define TLC5917_SPI_DATA_TRIS  TRISAbits.TRISA0
    #define TLC5917_SPI_CLOCK_TRIS TRISAbits.TRISA1
    #define TLC5917_LED_CS_TRIS    TRISAbits.TRISA5
    #define TLC5917_LED_OE_TRIS    TRISAbits.TRISA3

    #define NUM_TLC5917_OTPUTS 8

    void TLC5917Init();
    void TLC5917Write(unsigned int data);
    void TLC5917Clear();
    
#ifdef	__cplusplus
}
#endif

#endif	/* TLC5917_H */

