
#include "Uart.h"

void Open1USART ( unsigned char config,  unsigned int spbrg){
    TXSTA1 = 0;          // Reset USART registers to POR state
    RCSTA1 = 0;

    if(config&0x01)      // Sync or async operation
        TXSTA1bits.SYNC = 1;

    if(config&0x02)      // 8- or 9-bit mode
    {
        TXSTA1bits.TX9 = 1;	
        RCSTA1bits.RX9 = 1;
    }

    if(config&0x04)      // Master or Slave (sync only)
        TXSTA1bits.CSRC = 1;

    if(config&0x08)      // Continuous or single reception
        RCSTA1bits.CREN = 1;
    else
        RCSTA1bits.SREN = 1;

    if(config&0x10)      // Baud rate select (asychronous mode only)
        TXSTA1bits.BRGH = 1;
    else
        TXSTA1bits.BRGH = 0;

    if(config&0x40)      // Interrupt on receipt
        PIE1bits.RC1IE = 1;
    else
        PIE1bits.RC1IE = 0;

    if(config&0x80)      // Interrupt on transmission
        PIE1bits.TX1IE = 1;
    else
        PIE1bits.TX1IE = 0;

    SPBRG1 = spbrg;       // Write baudrate to SPBRG

    TXSTA1bits.TXEN = 1;  // Enable transmitter
    RCSTA1bits.SPEN = 1;  // Enable receiver
    TRISCbits.TRISC6 = 0; TRISCbits.TRISC7 = 1; 
    if(TXSTA1bits.SYNC && !TXSTA1bits.CSRC)	//synchronous  slave mode
        TRISCbits.TRISC6 = 1;
}

char Busy1USART(void)
{
    if(!TXSTA1bits.TRMT) // Is the transmit shift register empty
        return 1;          // No, return FALSE
    return 0;            // Return TRUE
}

char Read1USART (void){
    char data;   // Holds received data

    USART1_Status.val &= 0xf2;          // Clear previous status flags

    if(RCSTA1bits.RX9)                  // If 9-bit mode
    {
        USART1_Status.RX_NINE = 0;        // Clear the recieve bit 9 for USART1
        if(RCSTA1bits.RX9D)               // according to the RX9D bit
            USART1_Status.RX_NINE = 1;
    }

    if(RCSTA1bits.FERR)                 // If a framing error occured
        USART1_Status.FRAME_ERROR = 1;    // Set the status bit

    if(RCSTA1bits.OERR)                 // If an overrun error occured
        USART1_Status.OVERRUN_ERROR = 1;  // Set the status bit

    data = RCREG1;                      // Read data

    return (data);                     // Return the received data
}

void Write1USART ( char data){
    if(TXSTA1bits.TX9)  // 9-bit mode?
    {
        TXSTA1bits.TX9D = 0;       // Set the TX9D bit according to the
        if(USART1_Status.TX_NINE)  // USART1 Tx 9th bit in status reg
            TXSTA1bits.TX9D = 1;
    }

    TXREG1 = data;      // Write the data byte to the USART2
}

void Gets1USART ( char *buffer,  unsigned char len){
    char i;    // Length counter
    unsigned char data;

    for(i=0;i<len;i++)  // Only retrieve len characters
    {
        while(!DataRdy1USART());// Wait for data to be received

        data = Getc1USART();    // Get a character from the USART
                               // and save in the string
        *buffer = data;
        buffer++;              // Increment the string pointer
    }
}

void Puts1USART ( char *data ){
    do
    {  // Transmit a byte
        while(Busy1USART());
            Putc1USART(*data);
    } while( *data++ );
}

void Close1USART(void)
{
    RCSTA1 &= 0b01001111;  // Disable the receiver
    TXSTA1bits.TXEN = 0;   // and transmitter

    PIE1 &= 0b11001111;   // Disable both interrupts
}

