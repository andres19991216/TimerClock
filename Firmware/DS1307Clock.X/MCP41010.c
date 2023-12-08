/*
 * File:   TLC5917.c
  *
 * Created: 30/08/2023 
 * Author: Thao Tran
 */
#include "MCP41010.h"
#include "Config.h"

void MCP41010Init() {
    
    MCP41010_SPI_DATA_TRIS  = DIGITAL_OUTPUT;
    MCP41010_SPI_CLOCK_TRIS = DIGITAL_OUTPUT;
    MCP41010_POS_CS_TRIS    = DIGITAL_OUTPUT;
    
    MCP41010_POS_CS    = HIGH;
    MCP41010_SPI_CLOCK = LOW;
    MCP41010_SPI_DATA  = LOW;
    
}

void MCP41010Write(unsigned int cmd, unsigned int data) {
    
    MCP41010_SPI_DATA = 0; MCP41010_POS_CS = LOW; Nop(); Nop(); 
    unsigned int temp = cmd;
    for( unsigned char i = 0; i < NUM_MCP41010_OTPUTS; i++ ){
        MCP41010_SPI_DATA = (temp & 0x80) ? 1 : 0; __delay_us(1);
        MCP41010_SPI_CLOCK = HIGH; Nop(); Nop(); Nop(); MCP41010_SPI_CLOCK = LOW; Nop(); Nop(); Nop();
        temp = temp << 1;
    }
    temp = data;
    for( unsigned char i = 0; i < NUM_MCP41010_OTPUTS; i++ ){
        MCP41010_SPI_DATA = (temp & 0x80) ? 1 : 0; __delay_us(1);
        MCP41010_SPI_CLOCK = HIGH; Nop(); Nop(); Nop(); MCP41010_SPI_CLOCK = LOW; Nop(); Nop(); Nop();
        temp = temp << 1;
    }
    Nop(); Nop(); MCP41010_POS_CS = HIGH; 
}

void MCP41010Clear() {
    
} 