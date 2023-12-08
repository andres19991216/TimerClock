/* 
 * Resources used:
 * -SP1 module and three pins (SDO, SCK, CS)
 * 
 * Summary:
 * -MCP41010 is a 10K ohm digital potentiometer 
 * -8 bit wiper; 256 taps; ~39 ohms per step
 * -Wiper is 52 ohms nominal
 * -Wiper defaults to 0x80 at power up
 * -Max current of 1ma when wiper value set to 0x00
 * -Supports SPI modes 0 and 1; Using mode 0
 * -Supports SCK up to 10MHz
 * -16 bit shift register
 * -Software shutdown
 * 
 *        Command Byte
 *    ----------------------
 *    |X|X|C1|C0|X|X|P1*|P0|
 *    ----------------------
 * 
 *   C1|C0|Command        P1*|P0|Pot Selection
 *   -----------------------------------------
 *   0  0  Nop            0   0  Nop
 *   0  1  Write data     0   1  Command executed on pot 0
 *   1  0  Shutdown       1   0  Command executed on pot 1
 *   1  1  Nop            1   1  Command executed on both pots
 * 
 *   *P1 is a don't care bit on MCP41xxx's  
 * 
 * Calculate resistance:
 * Rwa = Rab(256-Dn)/256+Rw
 * Rwb = Rab(Dn)/256+Rw
 * 
 * Rab = Total pot resistance
 * Dn = Wiper position
 * Rw = Wiper resistance 
 * 
 * ****************************************************************************
 * 05/23   Adam Hout        -Original source
 * ****************************************************************************
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
    
    MCP41010Reset();
}

void MCP41010Write(unsigned int data) {
    
    MCP41010_SPI_DATA = 0; MCP41010_POS_CS = LOW; Nop(); Nop(); 
    unsigned int temp = MCP41010_WRITE0;
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

void MCP41010Reset() {
    MCP41010_SPI_DATA = 0; MCP41010_POS_CS = LOW; Nop(); Nop(); 
    unsigned int temp = MCP41010_SHUTDOWN;
    for( unsigned char i = 0; i < NUM_MCP41010_OTPUTS; i++ ){
        MCP41010_SPI_DATA = (temp & 0x80) ? 1 : 0; __delay_us(1);
        MCP41010_SPI_CLOCK = HIGH; Nop(); Nop(); Nop(); MCP41010_SPI_CLOCK = LOW; Nop(); Nop(); Nop();
        temp = temp << 1;
    }
    Nop(); Nop(); MCP41010_POS_CS = HIGH; 
}