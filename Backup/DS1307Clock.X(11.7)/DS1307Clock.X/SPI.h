/* 
 * File:   SPI.h
 * Author: Boris
 *
 * Created on September 11, 2023, 1:44 AM
 */

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include <xc.h>
    
    /* SSPSTAT REGISTER */
    // Master SPI mode only
    #define   SMPEND        0b10000000           // Input data sample at end of data out             
    #define   SMPMID        0b00000000           // Input data sample at middle of data out

    #define   MODE_00       0b00000000              // Setting for SPI bus Mode 0,0
    //CKE           0x40                   // SSPSTAT register 
    //CKP           0x00                   // SSPCON1 register 
    #define   MODE_01       0b00000001              // Setting for SPI bus Mode 0,1
    //CKE           0x00                   // SSPSTAT register 
    //CKP           0x00                   // SSPCON1 register
    #define   MODE_10       0b00000010              // Setting for SPI bus Mode 1,0
    //CKE           0x40                   // SSPSTAT register
    //CKP           0x10                   // SSPCON1 register
    #define   MODE_11       0b00000011              // Setting for SPI bus Mode 1,1
    //CKE           0x00                   // SSPSTAT register
    //CKP           0x10                   // SSPCON1 register

    /* SSPCON1 REGISTER */
    #define   SSPENB        0b00100000           // Enable serial port and configures SCK, SDO, SDI

    #define   SPI_FOSC_4    0b00000000              // SPI Master mode, clock = Fosc/4
    #define   SPI_FOSC_16   0b00000001              // SPI Master mode, clock = Fosc/16
    #define   SPI_FOSC_64   0b00000010              // SPI Master mode, clock = Fosc/64
    #define   SPI_FOSC_TMR2 0b00000011              // SPI Master mode, clock = TMR2 output/2
    #define   SLV_SSON      0b00000100              // SPI Slave mode, /SS pin control enabled
    #define   SLV_SSOFF     0b00000101              // SPI Slave mode, /SS pin control disabled
    //************************************************************************************************//
    
    #define EnableIntSPI  (PIE1bits.SSP1IE = 1)
    #define DisableIntSPI (PIE1bits.SSP1IE = 0)
    #define SetPriorityIntSPI( priority ) ( IPR1bits.SSP1IP = priority )
    #define SPI_Clear_Intr_Status_Bit     ( PIR1bits.SSP1IF = 0 )
    #define SPI_Intr_Status               ( PIR1bits.SSP1IF )
    #define DataRdySPI()  (SSP1STATbits.BF)

    void OpenSPI( uint8_t sync_mode, uint8_t bus_mode, uint8_t smp_phase );
    signed char WriteSPI(  uint8_t data_out );
    void GetsSPI(  uint8_t *rdptr,  uint8_t length );
    void PutsSPI(  uint8_t *wrptr );
    uint8_t ReadSPI( void );
    
    #define  CloseSPI()    (SSP1CON1 &=0xDF)

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

