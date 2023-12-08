/*
 * File:   Config.c
  *
 * Created: 30/08/2023 
 * Author: Thao Tran
 */


#include <xc.h>

void OscillatorInit(){
    OSCCONbits.SCS0 = 1;   // dont care if SCS1_bit = 1 
    OSCCONbits.SCS1 = 1;   // setup for internal oscillator block 
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCTUNEbits.PLLEN = 1;
}
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.