/*
 * File:   M74HC595.h
  *
 * Created: 30/08/2023 
 * Author: Boris
 */

#ifndef M74HC595_H
#define	M74HC595_H

#ifdef	__cplusplus
extern "C" {
#endif

    //74HC595 pin define for SPI
    #define M74HC595_SPI_DATA       PORTAbits.RA0
    #define M74HC595_SPI_CLOCK      PORTAbits.RA1
    #define M74HC595_SPI_LATCH      PORTAbits.RA3
    #define M74HC595_SPI_OE         PORTAbits.RA5
    #define M74HC595_SPI_DATA_TRIS  TRISAbits.TRISA0
    #define M74HC595_SPI_CLOCK_TRIS TRISAbits.TRISA1
    #define M74HC595_SPI_LATCH_TRIS TRISAbits.TRISA3
    #define M74HC595_SPI_OE_TRIS    TRISAbits.TRISA5

    #define NUM_M74HC595_OTPUTS 8

    void M74HC595Init();
    void M74HC595Write(unsigned char data);
    void M74HC595Clear();


#ifdef	__cplusplus
}
#endif

#endif	/* M74HC595_H */

