
#include "W5500.h"
#include "Config.h"

void OpenW5500(){
    
    W5500_RST_TRIS  = DIGITAL_OUTPUT;
    W5500_INT_TRIS  = DIGITAL_INPUT;  
    W5500_SCS_TRIS  = DIGITAL_OUTPUT;
    W5500_SCK_TRIS  = DIGITAL_OUTPUT;
    W5500_MOSI_TRIS = DIGITAL_OUTPUT;
    W5500_MISO_TRIS = DIGITAL_INPUT;  
    
    unsigned char mac_addr[] = {0xf4,0x6b,0x8c,0xce,0x67,0x4b};
    unsigned char ip_addr[] =  {10,10,15,223};
    unsigned char sub_mask[] = {255,255,255,0};
    unsigned char gtw_addr[] = {10,10,15,1};
    
    WriteNByteW5500( W5500_MR, RST, 1 ); __delay_ms(1); 
    WriteNByteW5500( W5500_GAR, gtw_addr, 4 ); 
    WriteNByteW5500( W5500_SUBR, sub_mask, 4 );
    WriteNByteW5500( W5500_SHAR, mac_addr, 6 );
    WriteNByteW5500( W5500_SIPR, ip_addr, 4 );

    for( uint8_t i = 0; i < 8; i++ ){
        WriteByteW5500Socket( i, W5500_Sn_RXBUF_SIZE, 0x02 );
        WriteByteW5500Socket( i, W5500_Sn_TXBUF_SIZE, 0x02 );
    }
    
    WriteNByteW5500( W5500_RTR, 0x07d0, 2 );
    WriteNByteW5500( W5500_RCR, 8, 1 );
}

uint8_t ReadW5500( uint16_t addr ){
    unsigned char receive_data;
    W5500_SCS = 0; NOP(); NOP();
    WriteShortSPI( addr );                                          //Write 16-bit register address through SPI
    WriteByteSPI( W5500_VDM | W5500_RWB_READ | W5500_COMMON_R );    //Write control byte through SPI, N bytes data length, write data, select general register
    receive_data = ReadByteSPI(); NOP(); NOP();
    WriteByteSPI( 0x00 );
    receive_data = ReadByteSPI(); NOP(); NOP();
    W5500_SCS = 1;
    return receive_data;
}

uint8_t ReadByteW5500Socket( uint8_t socket_number, uint16_t addr ){
    unsigned char receive_data;
    W5500_SCS = 0; NOP(); NOP();
    WriteShortSPI( addr );                                                          //Write 16-bit register address through SPI
    WriteByteSPI( W5500_VDM | W5500_RWB_READ | ( socket_number * 0x20 + 0x08 ) );   //Write control byte through SPI, N bytes data length, write data, select general register
    receive_data = ReadByteSPI(); NOP(); NOP();
    WriteByteSPI( 0x00 );
    receive_data = ReadByteSPI(); NOP(); NOP();
    W5500_SCS = 1;
    return receive_data;
}

uint16_t ReadShortW5500Socket( uint8_t socket_number, uint16_t addr ){
    uint16_t receive_data;
    W5500_SCS = 0; NOP(); NOP();
    WriteShortSPI( addr );                                                          //Write 16-bit register address through SPI
    WriteByteSPI( W5500_VDM | W5500_RWB_READ | ( socket_number * 0x20 + 0x08 ) );   //Write control byte through SPI, N bytes data length, write data, select general register
    receive_data = ReadByteSPI(); NOP(); NOP();
    WriteByteSPI( 0x00 );
    receive_data = ReadByteSPI(); NOP(); NOP();
    WriteByteSPI( 0x00 );
    receive_data = receive_data * 256 + ReadByteSPI(); NOP(); NOP();
    W5500_SCS = 1;
    return receive_data;
}

void WriteNByteW5500( uint16_t addr, uint8_t *write_datas, uint8_t data_size ){
    W5500_SCS = 0; NOP(); NOP();
    
    WriteShortSPI( addr );                                          //Write 16-bit register address through SPI
    WriteByteSPI( W5500_VDM | W5500_RWB_WRITE | W5500_COMMON_R );   //Write control byte through SPI, N bytes data length, write data, select general register
    for( uint8_t i = 0; i < data_size; i++ )						//Write the size bytes of data in the buffer to W5500 in a loop
	{
		WriteByteSPI( *write_datas ++ );                            //Write a byte of data
	}
    
    W5500_SCS = 1; 
}

void WriteByteW5500Socket( uint8_t socket_number, uint16_t addr, uint8_t write_data ){
    W5500_SCS = 0; NOP(); NOP();
    
    WriteShortSPI( addr );                                                          //Write 16-bit register address through SPI
    WriteByteSPI( W5500_VDM | W5500_RWB_WRITE | ( socket_number * 0x20 + 0x08 ) );  //Write control byte through SPI, N bytes data length, write data, select general register
	WriteByteSPI( write_data );                                                     //Write a byte of data
    
    W5500_SCS = 1; 
}

void WriteShortW5500Socket( uint8_t socket_number, uint16_t addr, uint16_t write_data ){
    W5500_SCS = 0; NOP(); NOP();
    
    WriteShortSPI( addr );                                                          //Write 16-bit register address through SPI
    WriteByteSPI( W5500_VDM | W5500_RWB_WRITE | ( socket_number * 0x20 + 0x08 ) );  //Write control byte through SPI, N bytes data length, write data, select general register
	WriteShortSPI( write_data );                                                    //Write a byte of data
    
    W5500_SCS = 1; 
}

void WriteDwordW5500Socket( uint8_t socket_number, uint16_t addr, uint8_t *write_datas ){
    W5500_SCS = 0; NOP(); NOP();
    
    WriteShortSPI( addr );                                                          //Write 16-bit register address through SPI
    WriteByteSPI( W5500_VDM | W5500_RWB_WRITE | ( socket_number * 0x20 + 0x08 ) );  //Write control byte through SPI, N bytes data length, write data, select general register
	WriteShortSPI( *write_datas ++ );
    WriteShortSPI( *write_datas ++ );
    WriteShortSPI( *write_datas ++ );
    WriteShortSPI( *write_datas ++ );//Write a byte of data
    
    W5500_SCS = 1; 
}

void WriteByteSPI( uint8_t data ){
    uint8_t temp = data;
    for( unsigned char i = 0; i < 8; i ++ )
    {
        W5500_MOSI = ( temp & 0x80 ) ? 1 : 0;
        W5500_SCK = 1; NOP(); NOP(); NOP(); W5500_SCK = 1;
        temp = temp << 1;
    }
}

void WriteShortSPI( uint16_t data ){
    uint16_t temp = data;
    for( uint8_t i = 0; i < 16; i ++ )
    {
        W5500_MOSI = ( temp & 0x80 ) ? 1 : 0;
        W5500_SCK = 1; NOP(); NOP(); NOP(); W5500_SCK = 1;
        temp = temp << 1;
    }
}

uint8_t ReadByteSPI(){
    uint8_t temp = 0;
    for( uint8_t i = 0; i < 8; i ++ )
    {
        W5500_SCK = 1; NOP(); NOP(); NOP(); W5500_SCK = 1;
        temp = temp + W5500_MOSI;
        temp = temp << 1;
    }
    return temp;
}

uint16_t ReadSocketBuffer( uint8_t socket_number, uint8_t *datas )
{
	uint16_t rx_size;
	uint16_t offset, offset_1;
	
    rx_size = ReadShortW5500Socket( socket_number, W5500_Sn_RX_RSR );
	if( rx_size == 0 ) return 0;						//Return if no data is received
	if( rx_size > 1460 ) rx_size = 1460;

	offset = ReadShortW5500Socket( socket_number, W5500_Sn_RX_RD);
	offset_1 = offset;
	offset &= ( W5500_S_RX_SIZE - 1 );							//Calculate the actual physical address

	W5500_SCS = 0; NOP(); NOP();

	WriteShortSPI( offset );						//Write 16-bit address
	WriteByteSPI( W5500_VDM | W5500_RWB_READ | ( socket_number * 0x20 + 0x18 ) );		//Write control byte, N bytes data length, read data, select the register of port s
	
    uint8_t temp = ReadByteSPI();
	
	if( ( offset + rx_size ) < W5500_S_RX_SIZE )					//If the maximum address does not exceed the maximum address of the W5500 receive buffer register
	{
		for( uint16_t i = 0; i < rx_size; i++ )						//loop to read rx_size bytes of data
		{
			WriteByteSPI( 0x00 );					//Send a dummy data
			temp = ReadByteSPI();					//Read 1 byte of data
			*datas = temp;								//Save the read data to the data storage buffer
			datas ++;								//The address of the data storage buffer pointer is incremented by 1
		}
	}
	else											//If the maximum address exceeds the maximum address of the W5500 receive buffer register
	{
		offset = W5500_S_RX_SIZE - offset;
		for( uint16_t i = 0;i < offset; i++ )						//loop to read out the first offset bytes of data
		{
			WriteByteSPI( 0x00 );					//Send a dummy data
			WriteByteSPI( 0x00 );					//Send a dummy data
			temp = ReadByteSPI();					//Read 1 byte of data
			*datas = temp;							//Save the read data to the data storage buffer
			datas ++;								//The address of the data storage buffer pointer is incremented by 1
		}
		W5500_SCS = 1; NOP(); NOP(); 	//Set SCS of W5500 to high level

		W5500_SCS = 0; NOP(); NOP();	//Set the SCS of W5500 to low level

		WriteByteSPI( 0x00 );						//Write 16-bit address
		WriteByteSPI( W5500_VDM | W5500_RWB_READ | ( socket_number * 0x20 + 0x18 ) );
        
        temp = ReadByteSPI();

		for( uint16_t i = 0; i < rx_size; i++ )						//loop to read rx_size bytes of data
		{
			WriteByteSPI( 0x00 );					//Send a dummy data
			temp = ReadByteSPI();					//Read 1 byte of data
			*datas = temp;								//Save the read data to the data storage buffer
			datas ++;								//The address of the data storage buffer pointer is incremented by 1
		}
	}
	W5500_SCS = 1; NOP(); NOP(); 	 	//Set SCS of W5500 to high level

	offset_1 += rx_size;								//Update the actual physical address, that is, the starting address of the received data next time read
	WriteShortW5500Socket( socket_number, W5500_Sn_RX_RD, offset_1 );
	WriteByteW5500Socket(socket_number, W5500_Sn_CR, RECV );			//Send start receiving command
	return rx_size;	
    //Return the length of the received data
}

void Write_SOCK_Data_Buffer( uint8_t socket_number, uint8_t *datas, uint8_t size)
{
	uint16_t offset, offset_1;

	offset = ReadShortW5500Socket( socket_number, W5500_Sn_TX_WR );
	offset_1 = offset;
	offset &= ( W5500_S_TX_SIZE - 1 );//计算实际的物理地址

	W5500_SCS = 0; NOP(); NOP();			 	//Set SCS of W5500 to low level

    WriteShortSPI( offset );						//Write 16-bit address
	WriteByteSPI( W5500_VDM | W5500_RWB_WRITE | ( socket_number * 0x20 + 0x10 ) );
	
	if( ( offset + size ) < W5500_S_TX_SIZE )//如果最大地址未超过W5500发送缓冲区寄存器的最大地址
	{
		for( uint16_t i = 0; i < size; i++ )//循环写入size个字节数据
		{
			WriteByteSPI( *datas++ );//写入一个字节的数据
		}
	}
	else//如果最大地址超过W5500发送缓冲区寄存器的最大地址
	{
		offset = W5500_S_TX_SIZE - offset;
		for( uint16_t i = 0; i < offset; i++ )//循环写入前offset个字节数据
		{
			WriteByteSPI( *datas++ );//写入一个字节的数据
		}
		W5500_SCS = 1; NOP(); NOP();			 //Set SCS of W5500 to high level

		W5500_SCS = 0; NOP(); NOP();		 	//Set SCS of W5500 to low level

		WriteByteSPI( 0x00 );//写16位地址
		WriteByteSPI( W5500_VDM | W5500_RWB_WRITE | ( socket_number * 0x20 + 0x10 ) );//写控制字节,N个字节数据长度,写数据,选择端口s的寄存器

		for( uint16_t i = 0; i < size; i++ )//循环写入size-offset个字节数据
		{
			WriteByteSPI( *datas++ );//写入一个字节的数据
		}
	}
	W5500_SCS = 1; NOP(); NOP(); 			 	//Set SCS of W5500 to high level

	offset_1 += size;//更新实际物理地址,即下次写待发送数据到发送数据缓冲区的起始地址
	WriteShortW5500Socket( socket_number, W5500_Sn_TX_WR, offset_1 );
	WriteByteW5500Socket( socket_number, W5500_Sn_CR, SEND );//发送启动发送命令
}

void W5500_Hardware_Reset(void)
{
	W5500_SCS = 0; NOP(); NOP();		//Reset pin pulled low
	__delay_us(50);
	W5500_SCS = 1; NOP(); NOP();			//Reset pin pulled high
	__delay_us(200);
	while( ( ReadW5500( W5500_PHYCFGR ) & LINK ) == 0 );									//Wait for the Ethernet connection to complete
}

uint8_t Detect_Gateway(void)
{
	uint8_t ip_adde[4];
	ip_adde[0]=IP_Addr[0]+1;
	ip_adde[1]=IP_Addr[1]+1;
	ip_adde[2]=IP_Addr[2]+1;
	ip_adde[3]=IP_Addr[3]+1;

	//Check the gateway and get the physical address of the gateway
	WriteDwordW5500Socket( 0, W5500_Sn_DIPR, ip_adde );//Write an IP value different from the local IP to the destination address register
	WriteByteW5500Socket( 0, W5500_Sn_MR, MR_TCP );//Set socket to TCP mode
	WriteByteW5500Socket( 0, W5500_Sn_CR, OPEN );//Open Socket
	__delay_us(10);//delay 5ms

	if(ReadByteW5500Socket( 0, W5500_Sn_SR) != SOCK_INIT )//If socket open fails
	{
		WriteByteW5500Socket( 0, W5500_Sn_CR, CLOSE );//Open unsuccessful, close Socket
		return FALSE;//return false
	}

	WriteByteW5500Socket( 0, W5500_Sn_CR, CONNECT );//Set Socket to Connect mode

	do
	{
		uint16_t temp = 0;
		temp = ReadByteW5500Socket( 0, W5500_Sn_IR );//Read Socket0 interrupt flag register
		if( temp != 0 )
		WriteByteW5500Socket( 0, W5500_Sn_IR, temp );
		__delay_us(10);//delay 5ms
		if( ( temp & IR_TIMEOUT ) == IR_TIMEOUT )
		{
			return FALSE;
		}
		else if( ReadByteW5500Socket( 0, W5500_Sn_DHAR ) != 0xff)
		{
			WriteByteW5500Socket( 0, W5500_Sn_CR, CLOSE );//close socket
			return TRUE;
		}
	}while(1);
}

void SocketInit( uint8_t socket )
{
	//set specified port
	switch( socket )
	{
		case 0:
			//Set the fragment length, refer to the W5500 data sheet, this value can not be modified
			WriteShortW5500Socket( 0, W5500_Sn_MSSR, 1460 );//Maximum fragment bytes = 1460 (0x5b4)
			//Set the port number of port 0
			WriteShortW5500Socket( 0, W5500_Sn_PORT, S0_Port[ 0 ] * 256 + S0_Port[ 1 ] );
			//Set the destination (remote) port number of port 0
			WriteShortW5500Socket( 0, W5500_Sn_DPORTR, S0_DPort[ 0 ] * 256 + S0_DPort[ 1 ] );
			//Set port 0 destination (remote) IP address
			WriteDwordW5500Socket( 0, W5500_Sn_DIPR, S0_DIP );
			break;

		case 1:
			//Set the fragment length, refer to the W5500 data sheet, this value can not be modified
			WriteShortW5500Socket( 1, W5500_Sn_MSSR, 1460 );//Maximum fragment bytes = 1460 (0x5b4)
			//Set the port number of port 0
			WriteShortW5500Socket( 1, W5500_Sn_PORT, S1_Port[ 0 ] * 256 + S1_Port[ 1 ] );
			//Set the destination (remote) port number of port 0
			WriteShortW5500Socket( 1, W5500_Sn_DPORTR, S1_DPort[ 0 ] * 256 + S1_DPort[ 1 ] );
			//Set port 0 destination (remote) IP address
			WriteDwordW5500Socket( 1, W5500_Sn_DIPR, S1_DIP );
			break;

		case 2:
			//Set the fragment length, refer to the W5500 data sheet, this value can not be modified
			WriteShortW5500Socket( 2, W5500_Sn_MSSR, 1460 );//Maximum fragment bytes = 1460 (0x5b4)
			//Set the port number of port 0
			WriteShortW5500Socket( 2, W5500_Sn_PORT, S2_Port[ 0 ] * 256 + S2_Port[ 1 ] );
			//Set the destination (remote) port number of port 0
			WriteShortW5500Socket( 2, W5500_Sn_DPORTR, S2_DPort[ 0 ] * 256 + S2_DPort[ 1 ] );
			//Set port 0 destination (remote) IP address
			WriteDwordW5500Socket( 2, W5500_Sn_DIPR, S2_DIP );
			break;

		case 3:
			//Set the fragment length, refer to the W5500 data sheet, this value can not be modified
			WriteShortW5500Socket( 3, W5500_Sn_MSSR, 1460 );//Maximum fragment bytes = 1460 (0x5b4)
			//Set the port number of port 0
			WriteShortW5500Socket( 3, W5500_Sn_PORT, S3_Port[ 0 ] * 256 + S3_Port[ 1 ] );
			//Set the destination (remote) port number of port 0
			WriteShortW5500Socket( 3, W5500_Sn_DPORTR, S3_DPort[ 0 ] * 256 + S3_DPort[ 1 ] );
			//Set port 0 destination (remote) IP address
			WriteDwordW5500Socket( 3, W5500_Sn_DIPR, S3_DIP );
			break;
			
		case 4:
			//Set the fragment length, refer to the W5500 data sheet, this value can not be modified
			WriteShortW5500Socket( 4, W5500_Sn_MSSR, 1460 );//Maximum fragment bytes = 1460 (0x5b4)
			//Set the port number of port 0
			WriteShortW5500Socket( 4, W5500_Sn_PORT, S4_Port[ 0 ] * 256 + S4_Port[ 1 ] );
			//Set the destination (remote) port number of port 0
			WriteShortW5500Socket( 4, W5500_Sn_DPORTR, S4_DPort[ 0 ] * 256 + S4_DPort[ 1 ] );
			//Set port 0 destination (remote) IP address
			WriteDwordW5500Socket( 4, W5500_Sn_DIPR, S4_DIP );
			break;

        case 5:
			//Set the fragment length, refer to the W5500 data sheet, this value can not be modified
			WriteShortW5500Socket( 5, W5500_Sn_MSSR, 1460 );//Maximum fragment bytes = 1460 (0x5b4)
			//Set the port number of port 0
			WriteShortW5500Socket( 5, W5500_Sn_PORT, S5_Port[ 0 ] * 256 + S5_Port[ 1 ] );
			//Set the destination (remote) port number of port 0
			WriteShortW5500Socket( 5, W5500_Sn_DPORTR, S5_DPort[ 0 ] * 256 + S5_DPort[ 1 ] );
			//Set port 0 destination (remote) IP address
			WriteDwordW5500Socket( 5, W5500_Sn_DIPR, S5_DIP );
			break;

		case 6:
			//Set the fragment length, refer to the W5500 data sheet, this value can not be modified
			WriteShortW5500Socket( 6, W5500_Sn_MSSR, 1460 );//Maximum fragment bytes = 1460 (0x5b4)
			//Set the port number of port 0
			WriteShortW5500Socket( 6, W5500_Sn_PORT, S6_Port[ 0 ] * 256 + S6_Port[ 1 ] );
			//Set the destination (remote) port number of port 0
			WriteShortW5500Socket( 6, W5500_Sn_DPORTR, S6_DPort[ 0 ] * 256 + S6_DPort[ 1 ] );
			//Set port 0 destination (remote) IP address
			WriteDwordW5500Socket( 6, W5500_Sn_DIPR, S6_DIP );
			break;

		case 7:
			//Set the fragment length, refer to the W5500 data sheet, this value can not be modified
			WriteShortW5500Socket( 7, W5500_Sn_MSSR, 1460 );//Maximum fragment bytes = 1460 (0x5b4)
			//Set the port number of port 0
			WriteShortW5500Socket( 7, W5500_Sn_PORT, S7_Port[ 0 ] * 256 + S7_Port[ 1 ] );
			//Set the destination (remote) port number of port 0
			WriteShortW5500Socket( 7, W5500_Sn_DPORTR, S7_DPort[ 0 ] * 256 + S7_DPort[ 1 ] );
			//Set port 0 destination (remote) IP address
			WriteDwordW5500Socket( 7, W5500_Sn_DIPR, S7_DIP );
			break;

		default:
			break;
	}
}

uint8_t SocketConnect( uint8_t socket )
{
	WriteByteW5500Socket( socket, W5500_Sn_MR, MR_TCP );			//Set socket to TCP mode
	WriteByteW5500Socket( socket, W5500_Sn_CR, OPEN );			//Open Socket
	__delay_us( 5 );									//Delay 5ms
	if( ReadByteW5500Socket( socket, W5500_Sn_SR ) != SOCK_INIT )	//If the socket fails to open
	{
		WriteByteW5500Socket( socket, W5500_Sn_CR, CLOSE );		//Unsuccessful opening, close Socket
		return FALSE;								//Return FALSE(0x00)
	}
	WriteByteW5500Socket( socket, W5500_Sn_CR, CONNECT );		//Set Socket to Connect mode
	return TRUE;									//Return TRUE, the setting is successful
}

uint8_t SocketListen( uint8_t socket )
{
	WriteByteW5500Socket( socket, W5500_Sn_MR, MR_TCP );				//Set socket to TCP mode
	WriteByteW5500Socket( socket, W5500_Sn_CR,OPEN );				//Open Socket
	__delay_us( 5 );
	if( ReadByteW5500Socket( socket, W5500_Sn_SR ) != SOCK_INIT )		//If the socket fails to open
	{
		WriteByteW5500Socket( socket, W5500_Sn_CR, CLOSE );			//Unsuccessful opening, close Socket
		return FALSE;
	}	
	WriteByteW5500Socket( socket, W5500_Sn_CR, LISTEN );				//Set Socket to listening mode
	__delay_us( 5 );
	if( ReadByteW5500Socket( socket, W5500_Sn_SR ) != SOCK_LISTEN )		//If socket setting fails
	{
		WriteByteW5500Socket( socket, W5500_Sn_CR, CLOSE );			//The setting is unsuccessful, close the Socket
		return FALSE;
	}
	return TRUE;
}

uint8_t SocketUDP( uint8_t socket )
{
	WriteByteW5500Socket( socket, W5500_Sn_MR,MR_UDP );				//Set Socket to UDP mode
	WriteByteW5500Socket( socket, W5500_Sn_CR, OPEN );				//Open Socket
	__delay_us( 5 );
	if(ReadByteW5500Socket( socket, W5500_Sn_SR ) != SOCK_UDP )		//If the socket fails to open
	{
		WriteByteW5500Socket( socket, W5500_Sn_CR, CLOSE );			//Unsuccessful opening, close Socket
		return FALSE;
	}
	else
		return TRUE;
}

void InterruptW5500(void)
{
	unsigned char i,j;

	IntDispose:

	i = ReadW5500( W5500_SIR );//Read the port interrupt flag register
	__delay_us( 5 );

	if( ( i & S0_INT ) == S0_INT )//Socket0事件处理
	{
		j = ReadByteW5500Socket( 0, W5500_Sn_IR );//读取Socket0中断标志寄存器
		WriteByteW5500Socket( 0, W5500_Sn_IR, j );
		if( j & IR_CON )//在TCP模式下,Socket0成功连接
		{
			S0_State |= W5500_S_CONN;//网络连接状态0x02,端口完成连接，可以正常传输数据
		}
		if( j & IR_DISCON )//在TCP模式下Socket断开连接处理
		{
			WriteByteW5500Socket( 0, W5500_Sn_CR, CLOSE );//关闭端口,等待重新打开连接
			SocketInit(0);		//指定Socket(0~7)初始化,初始化端口0
			S0_State=0;//网络连接状态0x00,端口连接失败
		}
		if( j & IR_SEND_OK )//Socket0数据发送完成,可以再次启动S_tx_process()函数发送数据
		{
			S0_Data |= W5500_S_TRANSMITOK;//端口发送一个数据包完成
		}
		if( j & IR_RECV )//Socket接收到数据,可以启动S_rx_process()函数
		{
			S0_Data |= W5500_S_RECEIVE;//端口接收到一个数据包
		}
		if( j & IR_TIMEOUT )//Socket连接或数据传输超时处理
		{
			WriteByteW5500Socket( 0, W5500_Sn_CR, CLOSE );// 关闭端口,等待重新打开连接
			S0_State=0;//网络连接状态0x00,端口连接失败
		}
	}

	if( ( i & S1_INT ) == S1_INT )//Socket0事件处理
	{
		j = ReadByteW5500Socket( 1, W5500_Sn_IR );//读取Socket0中断标志寄存器
		WriteByteW5500Socket( 1, W5500_Sn_IR, j );
		if( j & IR_CON )//在TCP模式下,Socket0成功连接
		{
			S1_State |= W5500_S_CONN;//网络连接状态0x02,端口完成连接，可以正常传输数据
		}
		if( j & IR_DISCON )//在TCP模式下Socket断开连接处理
		{
			WriteByteW5500Socket( 1, W5500_Sn_CR, CLOSE );//关闭端口,等待重新打开连接
			SocketInit(1);		//指定Socket(0~7)初始化,初始化端口0
			S1_State=0;//网络连接状态0x00,端口连接失败
		}
		if( j & IR_SEND_OK )//Socket0数据发送完成,可以再次启动S_tx_process()函数发送数据
		{
			S1_Data |= W5500_S_TRANSMITOK;//端口发送一个数据包完成
		}
		if( j & IR_RECV )//Socket接收到数据,可以启动S_rx_process()函数
		{
			S1_Data |= W5500_S_RECEIVE;//端口接收到一个数据包
		}
		if( j & IR_TIMEOUT )//Socket连接或数据传输超时处理
		{
			WriteByteW5500Socket( 1, W5500_Sn_CR, CLOSE );// 关闭端口,等待重新打开连接
			S1_State=0;//网络连接状态0x00,端口连接失败
		}
	}
    
    if( ( i & S2_INT ) == S2_INT )//Socket0事件处理
	{
		j = ReadByteW5500Socket( 2, W5500_Sn_IR );//读取Socket0中断标志寄存器
		WriteByteW5500Socket( 2, W5500_Sn_IR, j );
		if( j & IR_CON )//在TCP模式下,Socket0成功连接
		{
			S2_State |= W5500_S_CONN;//网络连接状态0x02,端口完成连接，可以正常传输数据
		}
		if( j & IR_DISCON )//在TCP模式下Socket断开连接处理
		{
			WriteByteW5500Socket( 2, W5500_Sn_CR, CLOSE );//关闭端口,等待重新打开连接
			SocketInit(2);		//指定Socket(0~7)初始化,初始化端口0
			S2_State=0;//网络连接状态0x00,端口连接失败
		}
		if( j & IR_SEND_OK )//Socket0数据发送完成,可以再次启动S_tx_process()函数发送数据
		{
			S2_Data |= W5500_S_TRANSMITOK;//端口发送一个数据包完成
		}
		if( j & IR_RECV )//Socket接收到数据,可以启动S_rx_process()函数
		{
			S2_Data |= W5500_S_RECEIVE;//端口接收到一个数据包
		}
		if( j & IR_TIMEOUT )//Socket连接或数据传输超时处理
		{
			WriteByteW5500Socket( 2, W5500_Sn_CR, CLOSE );// 关闭端口,等待重新打开连接
			S2_State=0;//网络连接状态0x00,端口连接失败
		}
    }
    
    if( ( i & S3_INT ) == S3_INT )//Socket0事件处理
	{
		j = ReadByteW5500Socket( 3, W5500_Sn_IR );//读取Socket0中断标志寄存器
		WriteByteW5500Socket( 3, W5500_Sn_IR, j );
		if( j & IR_CON )//在TCP模式下,Socket0成功连接
		{
			S3_State |= W5500_S_CONN;//网络连接状态0x02,端口完成连接，可以正常传输数据
		}
		if( j & IR_DISCON )//在TCP模式下Socket断开连接处理
		{
			WriteByteW5500Socket( 3, W5500_Sn_CR, CLOSE );//关闭端口,等待重新打开连接
			SocketInit(3);		//指定Socket(0~7)初始化,初始化端口0
			S3_State=0;//网络连接状态0x00,端口连接失败
		}
		if( j & IR_SEND_OK )//Socket0数据发送完成,可以再次启动S_tx_process()函数发送数据
		{
			S3_Data |= W5500_S_TRANSMITOK;//端口发送一个数据包完成
		}
		if( j & IR_RECV )//Socket接收到数据,可以启动S_rx_process()函数
		{
			S3_Data |= W5500_S_RECEIVE;//端口接收到一个数据包
		}
		if( j & IR_TIMEOUT )//Socket连接或数据传输超时处理
		{
			WriteByteW5500Socket( 3, W5500_Sn_CR, CLOSE );// 关闭端口,等待重新打开连接
			S3_State=0;//网络连接状态0x00,端口连接失败
		}
	}
    
    if( ( i & S4_INT ) == S4_INT )//Socket0事件处理
	{
		j = ReadByteW5500Socket( 4, W5500_Sn_IR );//读取Socket0中断标志寄存器
		WriteByteW5500Socket( 4, W5500_Sn_IR, j );
		if( j & IR_CON )//在TCP模式下,Socket0成功连接
		{
			S4_State |= W5500_S_CONN;//网络连接状态0x02,端口完成连接，可以正常传输数据
		}
		if( j & IR_DISCON )//在TCP模式下Socket断开连接处理
		{
			WriteByteW5500Socket( 4, W5500_Sn_CR, CLOSE );//关闭端口,等待重新打开连接
			SocketInit(4);		//指定Socket(0~7)初始化,初始化端口0
			S4_State=0;//网络连接状态0x00,端口连接失败
		}
		if( j & IR_SEND_OK )//Socket0数据发送完成,可以再次启动S_tx_process()函数发送数据
		{
			S4_Data |= W5500_S_TRANSMITOK;//端口发送一个数据包完成
		}
		if( j & IR_RECV )//Socket接收到数据,可以启动S_rx_process()函数
		{
			S4_Data |= W5500_S_RECEIVE;//端口接收到一个数据包
		}
		if( j & IR_TIMEOUT )//Socket连接或数据传输超时处理
		{
			WriteByteW5500Socket( 4, W5500_Sn_CR, CLOSE );// 关闭端口,等待重新打开连接
			S4_State=0;//网络连接状态0x00,端口连接失败
		}
	}
    
    if( ( i & S5_INT ) == S5_INT )//Socket0事件处理
	{
		j = ReadByteW5500Socket( 5, W5500_Sn_IR );//读取Socket0中断标志寄存器
		WriteByteW5500Socket( 5, W5500_Sn_IR, j );
		if( j & IR_CON )//在TCP模式下,Socket0成功连接
		{
			S5_State |= W5500_S_CONN;//网络连接状态0x02,端口完成连接，可以正常传输数据
		}
		if( j & IR_DISCON )//在TCP模式下Socket断开连接处理
		{
			WriteByteW5500Socket( 5, W5500_Sn_CR, CLOSE );//关闭端口,等待重新打开连接
			SocketInit(5);		//指定Socket(0~7)初始化,初始化端口0
			S5_State=0;//网络连接状态0x00,端口连接失败
		}
		if( j & IR_SEND_OK )//Socket0数据发送完成,可以再次启动S_tx_process()函数发送数据
		{
			S5_Data |= W5500_S_TRANSMITOK;//端口发送一个数据包完成
		}
		if( j & IR_RECV )//Socket接收到数据,可以启动S_rx_process()函数
		{
			S5_Data |= W5500_S_RECEIVE;//端口接收到一个数据包
		}
		if( j & IR_TIMEOUT )//Socket连接或数据传输超时处理
		{
			WriteByteW5500Socket( 5, W5500_Sn_CR, CLOSE );// 关闭端口,等待重新打开连接
			S5_State=0;//网络连接状态0x00,端口连接失败
		}
	}
    
    if( ( i & S6_INT ) == S6_INT )//Socket0事件处理
	{
		j = ReadByteW5500Socket( 6, W5500_Sn_IR );//读取Socket0中断标志寄存器
		WriteByteW5500Socket( 6, W5500_Sn_IR, j );
		if( j & IR_CON )//在TCP模式下,Socket0成功连接
		{
			S6_State |= W5500_S_CONN;//网络连接状态0x02,端口完成连接，可以正常传输数据
		}
		if( j & IR_DISCON )//在TCP模式下Socket断开连接处理
		{
			WriteByteW5500Socket( 6, W5500_Sn_CR, CLOSE );//关闭端口,等待重新打开连接
			SocketInit(6);		//指定Socket(0~7)初始化,初始化端口0
			S6_State=0;//网络连接状态0x00,端口连接失败
		}
		if( j & IR_SEND_OK )//Socket0数据发送完成,可以再次启动S_tx_process()函数发送数据
		{
			S6_Data |= W5500_S_TRANSMITOK;//端口发送一个数据包完成
		}
		if( j & IR_RECV )//Socket接收到数据,可以启动S_rx_process()函数
		{
			S6_Data |= W5500_S_RECEIVE;//端口接收到一个数据包
		}
		if( j & IR_TIMEOUT )//Socket连接或数据传输超时处理
		{
			WriteByteW5500Socket( 6, W5500_Sn_CR, CLOSE );// 关闭端口,等待重新打开连接
			S6_State=0;//网络连接状态0x00,端口连接失败
		}
	}
    
    if( ( i & S7_INT ) == S7_INT )//Socket0事件处理
	{
		j = ReadByteW5500Socket( 7, W5500_Sn_IR );//读取Socket0中断标志寄存器
		WriteByteW5500Socket( 7, W5500_Sn_IR, j );
		if( j & IR_CON )//在TCP模式下,Socket0成功连接
		{
			S7_State |= W5500_S_CONN;//网络连接状态0x02,端口完成连接，可以正常传输数据
		}
		if( j & IR_DISCON )//在TCP模式下Socket断开连接处理
		{
			WriteByteW5500Socket( 7, W5500_Sn_CR, CLOSE );//关闭端口,等待重新打开连接
			SocketInit(7);		//指定Socket(0~7)初始化,初始化端口0
			S7_State=0;//网络连接状态0x00,端口连接失败
		}
		if( j & IR_SEND_OK )//Socket0数据发送完成,可以再次启动S_tx_process()函数发送数据
		{
			S7_Data |= W5500_S_TRANSMITOK;//端口发送一个数据包完成
		}
		if( j & IR_RECV )//Socket接收到数据,可以启动S_rx_process()函数
		{
			S7_Data |= W5500_S_RECEIVE;//端口接收到一个数据包
		}
		if( j & IR_TIMEOUT )//Socket连接或数据传输超时处理
		{
			WriteByteW5500Socket( 7, W5500_Sn_CR, CLOSE );// 关闭端口,等待重新打开连接
			S7_State=0;//网络连接状态0x00,端口连接失败
		}
	}

	if( ReadW5500( W5500_SIR ) != 0 ) 
		goto IntDispose;
}


void CloseW5500(){
    
    W5500_RST_TRIS  = DIGITAL_OUTPUT;
    W5500_INT_TRIS  = DIGITAL_OUTPUT;  
    W5500_SCS_TRIS  = DIGITAL_OUTPUT;
    W5500_SCK_TRIS  = DIGITAL_OUTPUT;
    W5500_MOSI_TRIS = DIGITAL_OUTPUT;
    W5500_MISO_TRIS = DIGITAL_OUTPUT;
    W5500_RST = 0;
    W5500_INT = 0;
    W5500_SCS = 0;
    W5500_SCK = 0;
    W5500_MOSI = 0;
    W5500_MISO = 0;

}
