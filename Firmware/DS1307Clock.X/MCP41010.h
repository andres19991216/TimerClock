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
    #define MCP41010_WRITE       0x00010000
    #define MCP41010_SHOTDOWN    0x00100000

    #define MCP41010_SEL_NONE    0x00000000  
    #define MCP41010_SEL_POS0    0x00000001   
    #define MCP41010_SEL_POS1    0x00000010
    #define MCP41010_SEL_ALL     0x00000011
    
    //MCP41010 pin define for SPI
    #define MCP41010_SPI_DATA       PORTAbits.RA0
    #define MCP41010_SPI_CLOCK      PORTAbits.RA1
    #define MCP41010_POS_CS         PORTAbits.RA2
    #define MCP41010_SPI_DATA_TRIS  TRISAbits.TRISA0
    #define MCP41010_SPI_CLOCK_TRIS TRISAbits.TRISA1
    #define MCP41010_POS_CS_TRIS    TRISAbits.TRISA2

    #define NUM_MCP41010_OTPUTS 8

    void MCP41010Init();
    void MCP41010Write(unsigned int cmd, unsigned int data);
    void MCP41010Clear();
    
#ifdef	__cplusplus
}
#endif

#endif	/* MCP41010 */


