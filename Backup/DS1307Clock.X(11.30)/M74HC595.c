/*
 * File:   M74HC595.c
  *
 * Created: 30/08/2023 
 * Author: Thao Tran
 */
#include "M74HC595.h"
#include "Config.h"
#include "SPI.h"

void M74HC595Init() {
  
//---------------  Use GPIO pins (any I/O pins) --------------------//  
    M74HC595_SPI_DATA_TRIS  = DIGITAL_OUTPUT;
    M74HC595_SPI_CLOCK_TRIS = DIGITAL_OUTPUT;
    M74HC595_SPI_LATCH_TRIS = DIGITAL_OUTPUT;
    M74HC595_SPI_OE_TRIS    = DIGITAL_OUTPUT;
    
    M74HC595_SPI_DATA  = LOW;
    M74HC595_SPI_CLOCK = LOW;
    M74HC595_SPI_LATCH = LOW;
    M74HC595_SPI_OE    = LOW;
    
//---------------  Use SPI pins (RC3, RC4, RC5) --------------------//    
//    OpenSPI( SPI_FOSC_4, MODE_00, SMPMID );
//    M74HC595_SPI_LATCH_TRIS = DIGITAL_OUTPUT;
//    M74HC595_SPI_LATCH = LOW;
    
}

void M74HC595Write(unsigned char data) {

//---------------  Use GPIO pins (any I/O pins) --------------------//     
    unsigned char temp = data;
    M74HC595_SPI_DATA = 0;
    for( unsigned char i = 0; i < NUM_M74HC595_OTPUTS; i++ ){
        M74HC595_SPI_DATA = (temp & 0x80) ? 1 : 0; __delay_us(1);
        M74HC595_SPI_CLOCK = HIGH; NOP(); M74HC595_SPI_CLOCK = LOW; NOP();
        temp = temp << 1;
    }
    M74HC595_SPI_LATCH = HIGH; NOP(); M74HC595_SPI_LATCH = LOW; NOP();//__delay_us(2);
    
//---------------  Use SPI pins (RC3, RC4, RC5) --------------------// 
//    WriteSPI( data );              
//    M74HC595_SPI_LATCH = HIGH; __delay_us(10); M74HC595_SPI_LATCH = LOW; __delay_us(5);
    
}

void M74HC595Clear() {
    
} 