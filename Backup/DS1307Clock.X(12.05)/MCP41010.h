/* 
 * File:   TLC5917.h
 *  
 *
 * Created: 30/08/2023 
 * Author: Boris
 */

#ifndef MCP41010_H
#define	MCP41010_H

#ifdef	__cplusplus
extern "C" {
#endif

    //MCP41010 Command Define
    #define MCP41010_NO_PIN 255

    #define MCP41010_SHUTDOWN_BIT  0b00100000
    #define MCP41010_WRITE_BIT     0b00010000
    #define MCP41010_POT0_BIT      0b00000001
    #define MCP41010_POT1_BIT      0b00000010

    #define MCP41010_SHUTDOWN      MCP41010_SHUTDOWN_BIT  | MCP41010_POT0_BIT | MCP41010_POT0_BIT
    #define MCP41010_WRITE0        MCP41010_WRITE_BIT     | MCP41010_POT0_BIT 
    #define MCP41010_WRITE1        MCP41010_WRITE_BIT     | MCP41010_POT1_BIT 

    #define MAX_LEVEL                   255
    #define MAX_RESISTOR_LEVEL        10000
    
    //MCP41010 pin define for SPI
    #define MCP41010_SPI_DATA       PORTAbits.RA0
    #define MCP41010_SPI_CLOCK      PORTAbits.RA1
    #define MCP41010_POS_CS         PORTAbits.RA2
    #define MCP41010_SPI_DATA_TRIS  TRISAbits.TRISA0
    #define MCP41010_SPI_CLOCK_TRIS TRISAbits.TRISA1
    #define MCP41010_POS_CS_TRIS    TRISAbits.TRISA2

    #define NUM_MCP41010_OTPUTS 8

    void MCP41010Init();
    void MCP41010Write(unsigned int data);
    void MCP41010Reset();
    
#ifdef	__cplusplus
}
#endif

#endif	/* MCP41010 */


