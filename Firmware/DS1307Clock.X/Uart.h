/* 
 * File:   Uart.h
 * Author: Boris
 *
 * Created on September 10, 2023, 2:49 PM
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include <xc.h>

    /* Change this to near if building small memory model versions of
     * the libraries. */
    #define MEM_MODEL far

    /* Configuration bit masks to be 'anded' together and passed as the 'config'
     * parameter to the 'open' routine. */

    #define USART_TX_INT_ON   0b11111111  // Transmit interrupt on
    #define USART_TX_INT_OFF  0b01111111  // Transmit interrupt off
    #define USART_RX_INT_ON   0b11111111  // Receive interrupt on
    #define USART_RX_INT_OFF  0b10111111  // Receive interrupt off
    #define USART_BRGH_HIGH   0b11111111  // High baud rate
    #define USART_BRGH_LOW    0b11101111  // Low baud rate
    #define USART_CONT_RX     0b11111111  // Continuous reception
    #define USART_SINGLE_RX   0b11110111  // Single reception
    #define USART_SYNC_MASTER 0b11111111  // Synchrounous master mode
    #define USART_SYNC_SLAVE  0b11111011  // Synchrounous slave mode
    #define USART_NINE_BIT    0b11111111  // 9-bit data
    #define USART_EIGHT_BIT   0b11111101  // 8-bit data
    #define USART_SYNCH_MODE  0b11111111  // Synchronous mode
    #define USART_ASYNCH_MODE 0b11111110  // Asynchronous mode
    #define USART_ADDEN_ON    0b11111111  // Enables address detection
    #define USART_ADDEN_OFF   0b11011111  // Disables address detection
   
    #define BAUD_IDLE_CLK_HIGH  0b11111111  // idle state for clock is a high level
    #define BAUD_IDLE_CLK_LOW   0b11101111  // idle state for clock is a low level
    #define BAUD_16_BIT_RATE    0b11111111  // 16-bit baud generation rate
    #define BAUD_8_BIT_RATE     0b11110111  // 8-bit baud generation rate
    #define BAUD_WAKEUP_ON      0b11111111  // RX pin monitored
    #define BAUD_WAKEUP_OFF     0b11111101  // RX pin not monitored
    #define BAUD_AUTO_ON        0b11111111  // auto baud rate measurement enabled
    #define BAUD_AUTO_OFF       0b11111110  // auto baud rate measurement disabled
    
    volatile union {
        unsigned char val;
        struct
        {
            unsigned RX_NINE:1;         // Receive Bit 8 if 9-bit mode is enabled
            unsigned TX_NINE:1;         // Transmit Bit 8 if 9-bit mode is enabled
            unsigned FRAME_ERROR:1;     // Framing Error for usart
            unsigned OVERRUN_ERROR:1;   // Overrun Error for usart
            unsigned fill:4;
        };
    }USART1_Status;

    void Open1USART ( unsigned char config,  unsigned int spbrg);
    #define DataRdy1USART( ) (PIR1bits.RCIF)
    char Read1USART (void);
    void Write1USART ( char data);
    void Gets1USART ( char *buffer,  unsigned char len);
    void Puts1USART ( char *data);
//    void putrs1USART ( const MEM_MODEL rom char *data);
    #define Getc1USART Read1USART
    #define Putc1USART Write1USART
    void Close1USART(void);
    char Busy1USART(void);
   
    union USART2
    {
      unsigned char val;
      struct
      {
        unsigned RX_NINE:1;         // Receive Bit 8 if 9-bit mode is enabled
        unsigned TX_NINE:1;         // Transmit Bit 8 if 9-bit mode is enabled
        unsigned FRAME_ERROR:1;     // Framing Error for usart
        unsigned OVERRUN_ERROR:1;   // Overrun Error for usart
        unsigned fill:4;
      };
    };
    extern union USART2 USART2_Status;
    void Open2USART ( unsigned char config,  unsigned int spbrg);
    #define DataRdy2USART( ) (PIR1bits.RCIF)
    char Read2USART (void);
    void Write2USART ( char data);
    void Gets2USART ( char *buffer,  unsigned char len);
    void Puts2USART ( char *data);
    #define Getc2USART Read2USART
    #define Putc2USART Write2USART
    #define Close2USART( ) RCSTA2&=0b01001111,TXSTA2bits.TXEN=0,PIE3&=0b11001111
    #define Busy2USART( ) (!TXSTA2bits.TRMT)

#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

