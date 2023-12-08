/*
 * File:   I2C.c
  *
 * Created: 30/08/2023 
 * Author: Thao Tran
 */
#include "I2C.h"
#include <xc.h>

void I2cInit(void) {
    
    SSPSTATbits.SMP = 1; // 100KHz
    SSPSTATbits.CKE = 0; // Disable SMBus specific inputs
    
    SSPCON1bits.SSPEN = 1; // Enable SDA & SCL pins as Serial Ports
    SSPCON1bits.SSPM = 0b1000;// Mode Master
    
    SSPADD = 99; // 100KHz from 40MHz
}

void I2cStart(void) {
    SSPCON2bits.SEN = 1;    // Start Enable. Automatically cleared by hardware.
    while(SSPCON2bits.SEN == 1);
}

void I2cStop(void) {
    SSPCON2bits.PEN = 1;    // Stop Enable. Automatically cleared by hardware.
    while(SSPCON2bits.PEN == 1);
}

void I2cRestart(void) {
    SSPCON2bits.RSEN = 1;   // Repeated Start Enable. Automatically cleared by hardware.
    while(SSPCON2bits.RSEN == 1);
}

void I2cWait(void) {
	while(SSPSTATbits.R_NOT_W == 1)
        continue;
	if(SSPCON2bits.ACKSTAT == 1) { // If Acknowledge was not received from slave
		I2cStop();
	}
}

void I2cWrite(unsigned char data) {
    SSPBUF = data;
    while(SSPSTATbits.BF == 1);  // wait till buffer is empty 
    I2cWait(); // wait till Acknowledge received from slave 
}

unsigned char I2cRead(unsigned char ack) {
    unsigned char receive = 0;
    SSPCON2bits.RCEN = 1;       // Enables Receive mode for I2C
    while(SSPSTATbits.BF == 0);  // wait till buffer is full 
    receive = SSPBUF;
    I2cWait();
    SSPCON2bits.ACKDT = ack;   // ACK(0) ou NOT_ACK(1)
    SSPCON2bits.ACKEN=1;      // Transmit ACKDT data bit. Automatically cleared by hardware.
    while(SSPCON2bits.ACKEN == 1);     
    return receive;
}
