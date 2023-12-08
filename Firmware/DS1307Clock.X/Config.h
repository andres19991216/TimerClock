/*
 * Config.h
 *
 * Created: 30/08/2023 
 * Author: Boris
 */ 

#ifndef CONFIG_H
#define	CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    #define _XTAL_FREQ 32000000UL

    #define DIGITAL_INPUT  1
    #define DIGITAL_OUTPUT 0

    #define HIGH  1
    #define LOW   0

    #define TRUE    1
    #define FALSE   0

    #include <xc.h>

    void OscillatorInit();
    
#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */

