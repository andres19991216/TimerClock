#include "S5500.h"
#include "Config.h"
#include <string.h>
#include "Uart.h"
#include "SPI.h"

void OpenS5500(){
    
    S5500_RST_TRIS  = DIGITAL_OUTPUT;
    S5500_INT_TRIS  = DIGITAL_INPUT;  
    S5500_SCS_TRIS  = DIGITAL_OUTPUT;
//    S5500_SCK_TRIS  = DIGITAL_OUTPUT;
//    S5500_MOSI_TRIS = DIGITAL_OUTPUT;
//    S5500_MISO_TRIS = DIGITAL_INPUT;  
    S5500_SCS = 1; S5500_RST = 1;
    
    OpenSPI( SPI_FOSC_4, MODE_00, SMPMID );
    
    unsigned char mac_addr[] = {0xf4,0x6b,0x8c,0xce,0x67,0x4b};
    unsigned char ip_addr[] = {10,10,15,223};
//    unsigned char ip_addr[] = {192,168,2,10};
    unsigned char sub_mask[] = {255,255,255,0};
    unsigned char gtw_addr[] = {10,10,15,1};
//    unsigned char gtw_addr[] = {192,168,2,1};
    char temp = "";
    
    WriteNByteS5500( S5500_MR, MR_RST, 1 ); __delay_ms(1); 
    WriteNByteS5500( S5500_GAR, gtw_addr, 4 ); __delay_ms(1); 
    
    uint8_t temp1 = 0;
    temp1 = ReadByteS5500( S5500_GAR );
    sprintf( temp, "W5500->%d\n" , temp1 );
    puts1USART( temp );
    
    WriteNByteS5500( S5500_SUBR, sub_mask, 4 ); __delay_ms(1); 
    WriteNByteS5500( S5500_SHAR, mac_addr, 6 ); __delay_ms(1); 
    WriteNByteS5500( S5500_SIPR, ip_addr, 4 ); __delay_ms(1); 

    for( uint8_t i = 0; i < 8; i++ ){
        WriteByteS5500Socket( i, S5500_Sn_RXBUF_SIZE, 0x02 ); __delay_ms(1); 
        WriteByteS5500Socket( i, S5500_Sn_TXBUF_SIZE, 0x02 ); __delay_ms(1); 
    }
    
    WriteNByteS5500( S5500_RTR, 0x07d0, 2 ); __delay_ms(1); //The unit of timeout period is 100us and the default of RTR is ?0x07D0? or ?2000?
    WriteNByteS5500( S5500_RCR, 8, 1 );      __delay_ms(1); //When retransmission occurs as many as ?RCR+1?, Timeout interrupt is issued (Sn_IR[TIMEOUT] = ?1?)
    puts1USART( "W5500 Init Complete!\n" );
}

uint8_t ReadByteS5500( uint16_t addr ){
    uint8_t receive_data = 0;
    S5500_SCS = 0; __delay_us(2);
    WriteShortSPI1( addr );                                          //Write 16-bit register address through SPI
    WriteByteSPI1( S5500_VDM | S5500_RWB_READ | S5500_COMMON_R );    //Write control byte through SPI, N bytes data length, write data, select general register
//    receive_data = ReadByteSPI1(); __delay_us(1);
//    WriteByteSPI1( 0x00 );
//    receive_data = ReadByteSPI1(); __delay_us(2);
    receive_data = ReadSPI(); __delay_us(2);
    S5500_SCS = 1;
    return receive_data;
}

uint8_t ReadByteS5500Socket( uint8_t socket_number, uint16_t addr ){
    unsigned char receive_data;
    S5500_SCS = 0; NOP(); NOP();
    WriteShortSPI1( addr );                                                          //Write 16-bit register address through SPI
    WriteByteSPI1( S5500_VDM | S5500_RWB_READ | ( socket_number * 0x20 + 0x08 ) );   //Write control byte through SPI, N bytes data length, write data, select general register
    receive_data = ReadByteSPI1(); NOP(); NOP();
    WriteByteSPI1( 0x00 );
    receive_data = ReadByteSPI1(); NOP(); NOP();
    S5500_SCS = 1;
    return receive_data;
}

uint16_t ReadShortS5500Socket( uint8_t socket_number, uint16_t addr ){
    uint16_t receive_data;
    S5500_SCS = 0; NOP(); NOP();
    WriteShortSPI1( addr );                                                          //Write 16-bit register address through SPI
    WriteByteSPI1( S5500_VDM | S5500_RWB_READ | ( socket_number * 0x20 + 0x08 ) );   //Write control byte through SPI, N bytes data length, write data, select general register
    receive_data = ReadByteSPI1(); NOP(); NOP();
    WriteByteSPI1( 0x00 );
    receive_data = ReadByteSPI1(); NOP(); NOP();
    WriteByteSPI1( 0x00 );
    receive_data = receive_data * 256 + ReadByteSPI1(); NOP(); NOP();
    S5500_SCS = 1;
    return receive_data;
}

void WriteNByteS5500( uint16_t addr, uint8_t *write_datas, uint8_t data_size ){
    S5500_SCS = 0; __delay_us(2);
    
    WriteShortSPI1( addr );                                           //Write 16-bit register address through SPI
    WriteByteSPI1( S5500_VDM | S5500_RWB_WRITE | S5500_COMMON_R );   //Write control byte through SPI, N bytes data length, write data, select general register
    for( uint8_t i = 0; i < data_size; i++ )						//Write the size bytes of data in the buffer to W5500 in a loop
	{
//		WriteByteSPI1( *write_datas ++ );                            //Write a byte of data
        WriteSPI( *write_datas ++ );
	}
    __delay_us(2);
    S5500_SCS = 1; 
}

void WriteByteS5500Socket( uint8_t socket_number, uint16_t addr, uint8_t write_data ){
    S5500_SCS = 0; NOP(); NOP();
    
    WriteShortSPI1( addr );                                                          //Write 16-bit register address through SPI
    WriteByteSPI1( S5500_VDM | S5500_RWB_WRITE | ( socket_number * 0x20 + 0x08 ) );  //Write control byte through SPI, N bytes data length, write data, select general register
	WriteByteSPI1( write_data );                                                     //Write a byte of data
    
    S5500_SCS = 1; 
}

void WriteShortS5500Socket( uint8_t socket_number, uint16_t addr, uint16_t write_data ){
    S5500_SCS = 0; NOP(); NOP();
    
    WriteShortSPI1( addr );                                                          //Write 16-bit register address through SPI
    WriteByteSPI1( S5500_VDM | S5500_RWB_WRITE | ( socket_number * 0x20 + 0x08 ) );  //Write control byte through SPI, N bytes data length, write data, select general register
	WriteShortSPI1( write_data );                                                    //Write a byte of data
    
    S5500_SCS = 1; 
}

void WriteDwordS5500Socket( uint8_t socket_number, uint16_t addr, uint8_t *write_datas ){
    S5500_SCS = 0; NOP(); NOP();
    
    WriteShortSPI1( addr );                                                          //Write 16-bit register address through SPI
    WriteByteSPI1( S5500_VDM | S5500_RWB_WRITE | ( socket_number * 0x20 + 0x08 ) );  //Write control byte through SPI, N bytes data length, write data, select general register
	WriteShortSPI1( *write_datas ++ );
    WriteShortSPI1( *write_datas ++ );
    WriteShortSPI1( *write_datas ++ );
    WriteShortSPI1( *write_datas ++ );//Write a byte of data
    
    S5500_SCS = 1; 
}

void WriteByteSPI1( uint8_t data ){
//    uint8_t temp = data;
//    for( unsigned char i = 0; i < 8; i ++ )
//    {
//        S5500_MOSI = ( temp & 0x80 ) ? 1 : 0;
//        S5500_SCK = 1; __delay_us(1); S5500_SCK = 0; __delay_us(1);
//        temp = temp << 1;
//    }
    WriteSPI( data );
}

void WriteShortSPI1( uint16_t data ){
//    uint8_t temp = data;
//    for( uint8_t i = 0; i < 16; i ++ )
//    {
//        S5500_MOSI = ( temp & 0x80 ) ? 1 : 0;
//        S5500_SCK = 1; __delay_us(1); S5500_SCK = 0; __delay_us(1);
//        temp = temp << 1;
//    }
    uint8_t temp = ( data & 0xFF00 ) >> 8;
    WriteSPI( temp );
    temp = data & 0x00FF;
    WriteSPI( temp );
}

uint8_t ReadByteSPI1(){
    uint8_t temp = 0;
//    for( uint8_t i = 0; i < 8; i ++ )
//    {
//        S5500_SCK = 1; __delay_us(1); S5500_SCK = 0; __delay_us(1);
//        temp = temp + S5500_MISO;
//        temp = temp << 1;
//    }
    temp = ReadSPI();
    return temp;
}

void CloseSocket( uint8_t socket_number )
{
    // Send Close Command
    WriteByteS5500Socket( socket_number, S5500_Sn_CR, CR_CLOSE );
    // Waiting until the S0_CR is clear
    while( ReadByteS5500Socket( socket_number, S5500_Sn_CR ) );
}

void DisconnectSocket( uint8_t socket_number )
{
    // Send Disconnect Command
    WriteByteS5500Socket( socket_number, S5500_Sn_CR, CR_DISCON );
    // Wait for Disconecting Process
    while( ReadByteS5500Socket( socket_number, S5500_Sn_CR ) );
}

uint8_t InitSocket( uint8_t socket_number, uint8_t eth_protocol, uint16_t tcp_port )
{
    uint8_t retval = 0;

    // Make sure we close the socket first
    if ( ReadByteS5500Socket( socket_number, S5500_Sn_SR ) == SOCK_CLOSED ) {
        CloseSocket( socket_number );
    }
    // Assigned Socket 0 Mode Register
    WriteByteS5500Socket( socket_number, S5500_Sn_MR, eth_protocol );
    // Now open the Socket 0
    WriteShortS5500Socket( socket_number, S5500_Sn_PORT, tcp_port );
    WriteByteS5500Socket( socket_number, S5500_Sn_CR, CR_OPEN );

    // Wait for Opening Process
    while( ReadByteS5500Socket( socket_number, S5500_Sn_CR ) );
    // Check for Init Status
    if ( ReadByteS5500Socket( socket_number, S5500_Sn_SR ) == SOCK_INIT)
        retval = 1;
    else
        CloseSocket( socket_number );

    return retval;
}

uint8_t ListenSocket( uint8_t socket_number )
{
    uint8_t retval = 0;
   
    if ( ReadByteS5500Socket( socket_number, S5500_Sn_CR ) == SOCK_INIT ) {
        // Send the LISTEN Command
        WriteByteS5500Socket( socket_number, S5500_Sn_CR, CR_LISTEN );
        // Wait for Listening Process
        while( ReadByteS5500Socket( socket_number, S5500_Sn_CR ) );
        // Check for Listen Status
        if ( ReadByteS5500Socket( socket_number, S5500_Sn_SR ) == SOCK_LISTEN )
            retval = 1;
        else
            CloseSocket( socket_number );
    }
    return retval;
}

uint16_t SendSocket( uint8_t socket_number, const uint8_t *buf, uint16_t buflen )
{
    uint16_t ptr,offaddr,realaddr,txsize,timeout;   

    if ( buflen <= 0 ) return 0;

    // Make sure the TX Free Size Register is available
    txsize = ReadShortS5500Socket( socket_number, S5500_Sn_TX_FSR );

    timeout=0;
    while ( txsize < buflen ) {
        __delay_ms(1);
        txsize = ReadShortS5500Socket( socket_number, S5500_Sn_TX_FSR );
        // Timeout for approx 1000 ms
        if ( timeout++ > 1000 ) {
            // Disconnect the connection
            DisconnectSocket( socket_number );
            return 0;
        }
    }	

    // Read the Tx Write Pointer
    offaddr = ReadShortS5500Socket( socket_number, S5500_Sn_TX_WR );
    
    S5500_SCS = 0; NOP(); NOP();
    WriteShortSPI1( offaddr );
    WriteByteSPI1( S5500_VDM | S5500_RWB_WRITE | ( socket_number * 0x20 + 0x10 ) );
    while( buflen ) {
        buflen--;
        // Calculate the real W5100 physical Tx Buffer Address
        WriteByteSPI1( *buf++ );
        //buf++;
    }
    S5500_SCS = 1; NOP(); NOP();
    // Increase the S0_TX_WR value, so it point to the next transmit
    WriteShortS5500Socket( socket_number, S5500_Sn_TX_WR, offaddr );
    // Now Send the SEND command
    WriteByteS5500Socket( socket_number, S5500_Sn_CR, CR_SEND );

    // Wait for Sending Process
    while( ReadByteS5500Socket( socket_number, S5500_Sn_SR ) );	

    return 1;
}

uint16_t ReceivSocket( uint8_t socket_number, uint8_t *buf, uint16_t buflen )
{
    uint16_t ptr,offaddr,realaddr;   	

    if ( buflen <= 0 ) return 0;   

    // If the request size > MAX_BUF,just truncate it
    if ( buflen > 1460 ) buflen = 1460;
      
    // Read the Rx Read Pointer
    offaddr = ReadShortS5500Socket( socket_number, S5500_Sn_RX_RD );
    S5500_SCS = 0; NOP(); NOP();

	WriteShortSPI1( offaddr );						//Write 16-bit address
	WriteByteSPI1( S5500_VDM | S5500_RWB_READ | ( socket_number * 0x20 + 0x18 ) );
    
    while( buflen ) {
        buflen--;
        *buf = ReadByteSPI1();
        buf++;
    }
    *buf='\0';        // String terminated character
    S5500_SCS = 1; NOP(); NOP();
    // Increase the S0_RX_RD value, so it point to the next receive
    WriteShortS5500Socket( socket_number, S5500_Sn_TX_WR, offaddr );
    // Now Send the RECV command
    WriteByteS5500Socket( socket_number, S5500_Sn_CR, CR_RECV );
    __delay_us(5);    // Wait for Receive Process

    return 1;
}

uint16_t ReceivSocketSize( uint8_t socket_number )
{   
    return ReadByteS5500Socket( socket_number, S5500_Sn_RX_RSR );
}

void ResetS5500(){
    S5500_RST = 0;
    __delay_ms(10);
    S5500_RST = 1;
}

int StrIndex( char *s, char *t )
{
    int i,n;

    n = strlen(t);
    for(i=0;*(s+i); i++) {
        if ( strncmp( s+i, t, n ) == 0)
            return i;
    }
    return -1;
}