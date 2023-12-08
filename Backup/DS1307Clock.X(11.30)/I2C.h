/*
 * i2c_driver.h
 *
 * Created: 30/08/2023 
 * Author: Boris
 */ 


#ifndef I2C_DRIVER_H_
#define I2C_DRIVER_H_

#ifdef	__cplusplus
extern "C" {
#endif

    void I2cInit(void);
    void I2cWait(void);
    void I2cStart(void);
    void I2cStop(void);
    void I2cRestart(void);
    void I2cWrite(unsigned char data);
    unsigned char I2cRead(unsigned char ack);

#ifdef	__cplusplus
}
#endif

#endif /* I2C_DRIVER_H_ */