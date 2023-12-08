/* 
 * File:   S5500.h
 * Author: Boris
 *
 * Created on September 9, 2023, 6:25 PM
 */

#ifndef S5500_H
#define	S5500_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include <stdbool.h>
    #include <xc.h>

    #define S5500_RST  PORTDbits.RD0
    #define S5500_INT  PORTDbits.RD1
    #define S5500_SCS  PORTDbits.RD2
    #define S5500_SCK  PORTDbits.RD3
    #define S5500_MOSI PORTDbits.RD4
    #define S5500_MISO PORTDbits.RD5
    #define S5500_RST_TRIS  TRISDbits.TRISD0
    #define S5500_INT_TRIS  TRISDbits.TRISD1
    #define S5500_SCS_TRIS  TRISDbits.TRISD2
    #define S5500_SCK_TRIS  TRISDbits.TRISD3
    #define S5500_MOSI_TRIS TRISDbits.TRISD4
    #define S5500_MISO_TRIS TRISDbits.TRISD5  

    #define S5500_MR        0x0000   // Mode Register
        #define MR_RST		0x80
        #define MR_WOL		0x20
        #define MR_PB		0x10
        #define MR_PPP		0x08
        #define MR_FARP     0x02   
    
    #define S5500_GAR       0x0001   // Gateway Address: 0x0001 to 0x0004
    #define S5500_SUBR      0x0005   // Subnet mask Address: 0x0005 to 0x0008
    #define S5500_SHAR      0x0009   // Source Hardware Address (MAC): 0x0009 to 0x000E
    #define S5500_SIPR      0x000F   // Source IP Address: 0x000F to 0x0012
    
    #define S5500_RTR		0x0019
    #define S5500_RCR		0x001b
    
    /********************* Socket Register *******************/
    #define S5500_Sn_MR		0x0000
        #define MULTI_MFEN		0x80
        #define BCASTB			0x40
        #define	ND_MC_MMB		0x20
        #define UCASTB_MIP6B	0x10
        #define MR_CLOSE		0x00
        #define MR_TCP          0x01
        #define MR_UDP          0x02
        #define MR_MACRAW		0x04

    #define S5500_Sn_CR		0x0001
        #define CR_OPEN		0x01
        #define CR_LISTEN		0x02
        #define CR_CONNECT		0x04
        #define CR_DISCON		0x08
        #define CR_CLOSE		0x10
        #define CR_SEND		0x20
        #define CR_SEND_MAC	0x21
        #define CR_SEND_KEEP	0x22
        #define CR_RECV		0x40

    #define S5500_Sn_IR		0x0002
        #define IR_SEND_OK		0x10
        #define IR_TIMEOUT		0x08
        #define IR_RECV			0x04
        #define IR_DISCON		0x02
        #define IR_CON			0x01

    #define S5500_Sn_SR		0x0003
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

    #define S5500_Sn_PORT		0x0004
    #define S5500_Sn_DHAR	   	0x0006
    #define S5500_Sn_DIPR		0x000c
    #define S5500_Sn_DPORT      0x0010

    #define S5500_Sn_MSSR		0x0012
    #define S5500_Sn_TOS		0x0015
    #define S5500_Sn_TTL		0x0016

    #define S5500_Sn_RXBUF_SIZE	0x001e
    #define S5500_Sn_TXBUF_SIZE	0x001f
    #define S5500_Sn_TX_FSR     0x0020
    #define S5500_Sn_TX_RD	0x0022
    #define S5500_Sn_TX_WR	0x0024
    #define S5500_Sn_RX_RSR	0x0026
    #define S5500_Sn_RX_RD	0x0028
    #define S5500_Sn_RX_WR	0x002a

    #define S5500_Sn_IMR		0x002c
        #define IMR_SENDOK	0x10
        #define IMR_TIMEOUT	0x08
        #define IMR_RECV	0x04
        #define IMR_DISCON	0x02
        #define IMR_CON		0x01

    #define S5500_Sn_FRAG		0x002d
    #define S5500_Sn_KPALVTR	0x002f

    #define TX_BUF_MASK      0x07FF   // Tx 2K Buffer Mask:
    #define RX_BUF_MASK      0x07FF   // Rx 2K Buffer Mask:
    #define NET_MEMALLOC     0x05     // Use 2K of Tx/Rx Buffer

        /* Operation mode bits */
    #define S5500_VDM		0x00
    #define S5500_FDM1      0x01
    #define	S5500_FDM2      0x02
    #define S5500_FDM4      0x03

    /* Read_Write control bit */
    #define S5500_RWB_READ      0x00
    #define S5500_RWB_WRITE     0x04

    /* Block select bits */
    #define S5500_COMMON_R      0x00

    #define TCP_PORT    80  //TCP/IP Port

    void OpenS5500();
    uint8_t ReadByteS5500( uint16_t addr );
    uint8_t ReadByteS5500Socket( uint8_t socket_number, uint16_t addr );
    uint16_t ReadShortS5500Socket( uint8_t socket_number, uint16_t addr );
    void WriteNByteS5500( uint16_t addr, uint8_t *write_datas, uint8_t data_size );
    void WriteByteS5500Socket( uint8_t socket_number, uint16_t addr, uint8_t write_data );
    void WriteShortS5500Socket( uint8_t socket_number, uint16_t addr, uint16_t write_data );
    void WriteDwordS5500Socket( uint8_t socket_number, uint16_t addr, uint8_t *write_datas );
    void WriteByteSPI1( uint8_t data );
    void WriteShortSPI1( uint16_t data );
    void ResetS5500();
    uint8_t ReadByteSPI1();
    void CloseSocket( uint8_t socket_number );
    void DisconnectSocket( uint8_t socket_number );
    uint8_t InitSocket( uint8_t socket_number, uint8_t eth_protocol, uint16_t tcp_port );
    uint8_t ListenSocket( uint8_t socket_number );
    uint16_t SendSocket( uint8_t socket_number, const uint8_t *buf, uint16_t buflen );
    uint16_t ReceivSocket( uint8_t socket_number, uint8_t *buf, uint16_t buflen );
    uint16_t ReceivSocketSize( uint8_t socket_number );
    int StrIndex( char *s, char *t );
    
#ifdef	__cplusplus
}
#endif

#endif	/* S5500_H */

