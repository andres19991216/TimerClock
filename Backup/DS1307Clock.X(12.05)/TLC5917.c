/*
 * File:   TLC5917.c
  *
 * Created: 30/08/2023 
 * Author: Thao Tran
 */
#include "TLC5917.h"
#include "Config.h"

void TLC5917Init() {
    
    TLC5917_SPI_DATA_TRIS  = DIGITAL_OUTPUT;
    TLC5917_SPI_CLOCK_TRIS = DIGITAL_OUTPUT;
    TLC5917_LED_CS_TRIS    = DIGITAL_OUTPUT;
    TLC5917_LED_OE_TRIS    = DIGITAL_OUTPUT;
    
    TLC5917_LED_OE = HIGH;
    TLC5917_LED_CS = LOW;
    TLC5917_SPI_CLOCK = LOW;
    TLC5917_SPI_DATA  = LOW;
    
}

void TLC5917Write(unsigned int data) {
    
    unsigned int temp = data;
    TLC5917_SPI_DATA = 0; TLC5917_LED_CS = LOW; TLC5917_LED_OE = HIGH; 
    for( unsigned char i = 0; i < NUM_TLC5917_OTPUTS; i++ ){
        TLC5917_SPI_DATA = (temp & 0x80) ? 1 : 0; __delay_us(1);
        TLC5917_SPI_CLOCK = HIGH; __delay_us(1); TLC5917_SPI_CLOCK = LOW; __delay_us(1);
        temp = temp << 1;
    }
    TLC5917_LED_CS = HIGH; __delay_us(1); TLC5917_LED_CS = LOW; __delay_us(1);
    TLC5917_LED_OE = LOW; __delay_us(2);
}

void TLC5917Clear() {
    
} 