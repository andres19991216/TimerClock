/* 
 * File:   W5500.h
 * Author: Boris
 *
 * Created on September 7, 2023, 6:22 PM
 */

#ifndef W5500_H
#define	W5500_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include <stdbool.h>
    
    #define W5500_RST  PORTDbits.RD0
    #define W5500_INT  PORTDbits.RD1
    #define W5500_SCS  PORTDbits.RD2
    #define W5500_SCK  PORTDbits.RD3
    #define W5500_MOSI PORTDbits.RD4
    #define W5500_MISO PORTDbits.RD5
    #define W5500_RST_TRIS  TRISDbits.TRISD0
    #define W5500_INT_TRIS  TRISDbits.TRISD1
    #define W5500_SCS_TRIS  TRISDbits.TRISD2
    #define W5500_SCK_TRIS  TRISDbits.TRISD3
    #define W5500_MOSI_TRIS TRISDbits.TRISD4
    #define W5500_MISO_TRIS TRISDbits.TRISD5

    /***************** Common Register *****************/
    #define W5500_MR   0x0000   // Mode Register
        #define RST		0x80
        #define WOL		0x20
        #define PB		0x10
        #define PPP		0x08
        #define FARP	0x02   
    
    #define W5500_GAR   0x0001   // Gateway Address: 0x0001 to 0x0004
    #define W5500_SUBR  0x0005   // Subnet mask Address: 0x0005 to 0x0008
    #define W5500_SHAR  0x0009   // Source Hardware Address (MAC): 0x0009 to 0x000E
    #define W5500_SIPR  0x000F   // Source IP Address: 0x000F to 0x0012

    #define W5500_INTLEVEL	0x0013
    #define W5500_IR		0x0015
        #define CONFLICT	0x80
        #define UNREACH		0x40
        #define PPPOE		0x20
        #define MP			0x10

    #define W5500_IMR		0x0016
        #define IM_IR7		0x80
        #define IM_IR6		0x40
        #define IM_IR5		0x20
        #define IM_IR4		0x10

    #define W5500_SIR		0x0017
        #define S7_INT		0x80
        #define S6_INT		0x40
        #define S5_INT		0x20
        #define S4_INT		0x10
        #define S3_INT		0x08
        #define S2_INT		0x04
        #define S1_INT		0x02
        #define S0_INT		0x01

    #define W5500_SIMR	0x0018
        #define S7_IMR		0x80
        #define S6_IMR		0x40
        #define S5_IMR		0x20
        #define S4_IMR		0x10
        #define S3_IMR		0x08
        #define S2_IMR		0x04
        #define S1_IMR		0x02
        #define S0_IMR		0x01

    #define W5500_RTR		0x0019
    #define W5500_RCR		0x001b

    #define W5500_PTIMER	0x001c
    #define W5500_PMAGIC	0x001d
    #define W5500_PHA		0x001e
    #define W5500_PSID	0x0024
    #define W5500_PMRU	0x0026

    #define W5500_UIPR	0x0028
    #define W5500_UPORT	0x002c

    #define W5500_PHYCFGR	0x002e
        #define RST_PHY		0x80
        #define OPMODE		0x40
        #define DPX			0x04
        #define SPD			0x02
        #define LINK		0x01

    #define W5500_VERR	0x0039

    /********************* Socket Register *******************/
    #define W5500_Sn_MR		0x0000
        #define MULTI_MFEN		0x80
        #define BCASTB			0x40
        #define	ND_MC_MMB		0x20
        #define UCASTB_MIP6B	0x10
        #define MR_CLOSE		0x00
        #define MR_TCP          0x01
        #define MR_UDP          0x02
        #define MR_MACRAW		0x04

    #define W5500_Sn_CR		0x0001
        #define OPEN		0x01
        #define LISTEN		0x02
        #define CONNECT		0x04
        #define DISCON		0x08
        #define CLOSE		0x10
        #define SEND		0x20
        #define SEND_MAC	0x21
        #define SEND_KEEP	0x22
        #define RECV		0x40

    #define W5500_Sn_IR		0x0002
        #define IR_SEND_OK		0x10
        #define IR_TIMEOUT		0x08
        #define IR_RECV			0x04
        #define IR_DISCON		0x02
        #define IR_CON			0x01

    #define W5500_Sn_SR		0x0003
        #define SOCK_CLOSED		0x00
        #define SOCK_INIT		0x13
        #define SOCK_LISTEN		0x14
        #define SOCK_ESTABLISHED	0x17
        #define SOCK_CLOSE_WAIT		0x1c
        #define SOCK_UDP		0x22
        #define SOCK_MACRAW		0x42

        #define SOCK_SYNSEND	0x15
        #define SOCK_SYNRECV	0x16
        #define SOCK_FIN_WAI	0x18
        #define SOCK_CLOSING	0x1a
        #define SOCK_TIME_WAIT	0x1b
        #define SOCK_LAST_ACK	0x1d

    #define W5500_Sn_PORT		0x0004
    #define W5500_Sn_DHAR	   	0x0006
    #define W5500_Sn_DIPR		0x000c
    #define W5500_Sn_DPORTR     0x0010

    #define W5500_Sn_MSSR		0x0012
    #define W5500_Sn_TOS		0x0015
    #define W5500_Sn_TTL		0x0016

    #define W5500_Sn_RXBUF_SIZE	0x001e
    #define W5500_Sn_TXBUF_SIZE	0x001f
    #define W5500_Sn_TX_FSR     0x0020
    #define W5500_Sn_TX_RD	0x0022
    #define W5500_Sn_TX_WR	0x0024
    #define W5500_Sn_RX_RSR	0x0026
    #define W5500_Sn_RX_RD	0x0028
    #define W5500_Sn_RX_WR	0x002a

    #define W5500_Sn_IMR		0x002c
        #define IMR_SENDOK	0x10
        #define IMR_TIMEOUT	0x08
        #define IMR_RECV	0x04
        #define IMR_DISCON	0x02
        #define IMR_CON		0x01

    #define W5500_Sn_FRAG		0x002d
    #define W5500_Sn_KPALVTR	0x002f

    /************************ SPI Control Byte *************************/
    /*******************************************************************/
    /* Operation mode bits */
    #define W5500_VDM		0x00
    #define W5500_FDM1      0x01
    #define	W5500_FDM2      0x02
    #define W5500_FDM4      0x03

    /* Read_Write control bit */
    #define W5500_RWB_READ      0x00
    #define W5500_RWB_WRITE     0x04

    /* Block select bits */
    #define W5500_COMMON_R      0x00

    /* Socket 0 */
    #define W5500_S0_REG		0x08
    #define W5500_S0_TX_BUF     0x10
    #define W5500_S0_RX_BUF     0x18

    /* Socket 1 */
    #define W5500_S1_REG		0x28
    #define W5500_S1_TX_BUF     0x30
    #define W5500_S1_RX_BUF     0x38

    /* Socket 2 */
    #define W5500_S2_REG		0x48
    #define W5500_S2_TX_BUF     0x50
    #define W5500_S2_RX_BUF     0x58

    /* Socket 3 */
    #define W5500_S3_REG		0x68
    #define W5500_S3_TX_BUF     0x70
    #define W5500_S3_RX_BUF     0x78

    /* Socket 4 */
    #define W5500_S4_REG		0x88
    #define W5500_S4_TX_BUF     0x90
    #define W5500_S4_RX_BUF     0x98

    /* Socket 5 */
    #define W5500_S5_REG		0xa8
    #define W5500_S5_TX_BUF     0xb0
    #define W5500_5_RX_BUF      0xb8

    /* Socket 6 */
    #define W5500_S6_REG		0xc8
    #define W5500_S6_TX_BUF     0xd0
    #define W5500_S6_RX_BUF     0xd8

    /* Socket 7 */
    #define W5500_S7_REG		0xe8
    #define W5500_S7_TX_BUF     0xf0
    #define W5500_S7_RX_BUF     0xf8

    #define W5500_S_RX_SIZE     2048	
    #define W5500_S_TX_SIZE     2048  	

    #define W5500_S_INIT        0x01	//The port is initialized
    #define W5500_S_CONN        0x02	//The port is connected and data can be transmitted normally

    #define W5500_S_RECEIVE     0x01	//端口接收到一个数据包
    #define W5500_S_TRANSMITOK	0x02	//端口发送一个数据包完成

    #define TCP_SERVER		0x00	//TCP服务器模式
    #define TCP_CLIENT		0x01	//TCP客户端模式
    #define UDP_MODE		0x02	//UDP(广播)模式
    
    #include <xc.h>

    unsigned char Gateway_IP[4];//Gateway IP address
    unsigned char Sub_Mask[4];	///Subnet mask
    unsigned char Phy_Addr[6];	//physical address (MAC)
    unsigned char IP_Addr[4];	//Local IP address

    unsigned char S0_Port[2];	//Port number of port 0 (502)
    unsigned char S1_Port[2];	//Port number of port 1 (512)
    unsigned char S2_Port[2];	//Port number of port 2 (522)
    unsigned char S3_Port[2];	//Port number of port 3 (532)
    unsigned char S4_Port[2];	//Port number of port 4 (542)
    unsigned char S5_Port[2];	//Port number of port 5 (552)
    unsigned char S6_Port[2];	//Port number of port 6 (562)
    unsigned char S7_Port[2];	//Port number of port 7 (572)

    unsigned char S0_DIP[4];	//port 0 destination IP address
    unsigned char S1_DIP[4];	//port 1 destination IP address
    unsigned char S2_DIP[4];	//port 2 destination IP address
    unsigned char S3_DIP[4];	//port 3 destination IP address
    unsigned char S4_DIP[4];	//port 4 destination IP address
    unsigned char S5_DIP[4];	//port 5 destination IP address
    unsigned char S6_DIP[4];	//port 6 destination IP address
    unsigned char S7_DIP[4];	//port 7 destination IP address

    unsigned char S0_DPort[2];	//Port 0 destination port number (6000)
    unsigned char S1_DPort[2];	//Port 1 destination port number (6000)
    unsigned char S2_DPort[2];	//Port 2 destination port number (6000)
    unsigned char S3_DPort[2];	//Port 3 destination port number (6000)
    unsigned char S4_DPort[2];	//Port 4 destination port number (6000)
    unsigned char S5_DPort[2];	//Port 5 destination port number (6000)
    unsigned char S6_DPort[2];	//Port 6 destination port number (6000)
    unsigned char S7_DPort[2];	//Port 7 destination port number (6000)

    unsigned char UDP_DIPR[4];	//UDP (broadcast) mode, destination host IP address
    unsigned char UDP_DPORT[2];	//UDP (broadcast) mode, destination host port number

    /***************----- 端口的运行模式 -----***************/
    unsigned char S0_Mode =3;	//Operation mode of port 0, 0: TCP server mode, 1: TCP client mode, 2: UDP (broadcast) mode
    unsigned char S1_Mode =3;	//Operation mode of port 1, 0: TCP server mode, 1: TCP client mode, 2: UDP (broadcast) mode
    unsigned char S2_Mode =3;	//Operation mode of port 2, 0: TCP server mode, 1: TCP client mode, 2: UDP (broadcast) mode
    unsigned char S3_Mode =3;	//Operation mode of port 3, 0: TCP server mode, 1: TCP client mode, 2: UDP (broadcast) mode
    unsigned char S4_Mode =3;	//Operation mode of port 4, 0: TCP server mode, 1: TCP client mode, 2: UDP (broadcast) mode
    unsigned char S5_Mode =3;	//Operation mode of port 5, 0: TCP server mode, 1: TCP client mode, 2: UDP (broadcast) mode
    unsigned char S6_Mode =3;	//Operation mode of port 6, 0: TCP server mode, 1: TCP client mode, 2: UDP (broadcast) mode
    unsigned char S7_Mode =3;	//Operation mode of port 7, 0: TCP server mode, 1: TCP client mode, 2: UDP (broadcast) mode
   
    unsigned char S0_State =0;	//Port 0 status record, 1: port completes initialization, 2 port completes connection (data can be transmitted normally)
    unsigned char S1_State =0;	//Port 1 status record, 1: port completes initialization, 2 port completes connection (data can be transmitted normally)
    unsigned char S2_State =0;	//Port 2 status record, 1: port completes initialization, 2 port completes connection (data can be transmitted normally)
    unsigned char S3_State =0;	//Port 3 status record, 1: port completes initialization, 2 port completes connection (data can be transmitted normally)
    unsigned char S4_State =0;	//Port 4 status record, 1: port completes initialization, 2 port completes connection (data can be transmitted normally)
    unsigned char S5_State =0;	//Port 5 status record, 1: port completes initialization, 2 port completes connection (data can be transmitted normally)
    unsigned char S6_State =0;	//Port 6 status record, 1: port completes initialization, 2 port completes connection (data can be transmitted normally)
    unsigned char S7_State =0;	//Port 7 status record, 1: port completes initialization, 2 port completes connection (data can be transmitted normally)
    
    unsigned char S0_Data;		//Status of port 0 receiving and sending data, 1: port receiving data, 2: port sending data completed
    unsigned char S1_Data;		//Status of port 1 receiving and sending data, 1: port receiving data, 2: port sending data completed
    unsigned char S2_Data;		//Status of port 2 receiving and sending data, 1: port receiving data, 2: port sending data completed
    unsigned char S3_Data;		//Status of port 3 receiving and sending data, 1: port receiving data, 2: port sending data completed
    unsigned char S4_Data;		//Status of port 4 receiving and sending data, 1: port receiving data, 2: port sending data completed
    unsigned char S5_Data;		//Status of port 5 receiving and sending data, 1: port receiving data, 2: port sending data completed
    unsigned char S6_Data;		//Status of port 6 receiving and sending data, 1: port receiving data, 2: port sending data completed
    unsigned char S7_Data;		//Status of port 7 receiving and sending data, 1: port receiving data, 2: port sending data completed

    unsigned char W5500_Interrupt;
    
    void OpenW5500();
    void WriteNByteW5500( uint16_t addr, uint8_t *write_datas, uint8_t data_size );
    void WriteByteW5500Socket( uint8_t socket_number, uint16_t addr, uint8_t write_data );
    void WriteShortW5500Socket( uint8_t socket_number, uint16_t addr, uint16_t write_data );
    void WriteDwordW5500Socket( uint8_t socket_number, uint16_t addr, uint8_t *write_datas );
    uint8_t ReadW5500( uint16_t addr );
    uint8_t ReadByteW5500Socket( uint8_t socket_number, uint16_t addr );
    uint16_t ReadShortW5500Socket( uint8_t socket_number, uint16_t addr );
    void WriteByteSPI( uint8_t data );
    void WriteShortSPI( uint16_t data );
    uint8_t ReadByteSPI();
    uint16_t ReadSocketBuffer( uint8_t socket_number, uint8_t *datas );
    void Write_SOCK_Data_Buffer( uint8_t socket_number, uint8_t *datas, uint8_t size);
    void W5500_Hardware_Reset();
    uint8_t Detect_Gateway();
    void SocketInit( uint8_t socket );
    uint8_t SocketConnect( uint8_t socket );
    uint8_t SocketListen( uint8_t socket );
    uint8_t SocketUDP( uint8_t socket );
    void InterruptW5500();
    void CloseW5500();

#ifdef	__cplusplus
}
#endif

#endif	/* W5500_H */

