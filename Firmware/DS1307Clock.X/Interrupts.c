
/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#include <xc.h>         /* XC8 General Include File */
#include "Timers.h"
#include "Main.h"
#include "Interrupts.h"
#include "Counter.h"
#include "Display.h"
#include "Buttons.h"

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

/* High-priority service */

void __interrupt(high_priority) high_isr(void){

    if( INTCONbits.TMR0IE && INTCONbits.TMR0IF ) { //TMR0 overflow interrupt
        INTCONbits.TMR0IF = 0; 
        WriteTimer0( 65285 ); 
        ScanDisplay(); 
        ScanButtons(); 
        return;
    }
    if( INTCON3bits.INT2IE && INTCON3bits.INT2IF ) { //INT2 interrupt
        INTCON3bits.INT2IF = 0;
        ScanCounter(); 
        SYSTEM_ACTION_FLG.DISP_UPDATE_ENABLE = 1;
        return;
    }
}

/* Low-priority interrupt routine */

void __interrupt(low_priority) low_isr(void)
{
//    if( INTCON3bits.INT2IE && INTCON3bits.INT2IF ) { //INT2 interrupt
//        INTCON3bits.INT2IF = 0; 
//        ScanCounter(); 
//        return;
//    }
}

void EnableAllInterrupts(){
    ei();
}

void DisableAllInterrupts(){
    di();
}

void EnablePeripheralInterrupts(){
    INTCONbits.PEIE = 1;
}

void DisablePeripheralInterrupts(){
    INTCONbits.PEIE = 0;
}

void EnablePriorityInterrupts(){
    RCONbits.IPEN = 1;
}

void DisablePriorityInterrupts(){
    RCONbits.IPEN = 0;
}