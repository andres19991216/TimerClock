
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
unsigned int cnt = 0;
/* High-priority service */

void __interrupt(high_priority) high_isr(void){

    if( INTCONbits.TMR0IE && INTCONbits.TMR0IF ) { //TMR0 overflow interrupt
        INTCONbits.TMR0IF = 0; 
//        WriteTimer0( 65285 );
        switch( display_cnt ){
            case 0: WriteTimer0( FOUR_SEG_TIME_VALUE ); break;  
            case 1: WriteTimer0( FOUR_SEG_TIME_VALUE ); break;
            case 2: WriteTimer0( FOUR_SEG_TIME_VALUE ); break;
            case 3: WriteTimer0( FOUR_SEG_TIME_VALUE ); break;
            case 4: WriteTimer0( TWO_SEG_TIME_VALUE );  break;
            case 5: WriteTimer0( TWO_SEG_TIME_VALUE );  break;
            case 6: WriteTimer0( FOUR_SEG_TIME_VALUE ); break;
            case 7: WriteTimer0( LEDS_TIME_VALUE );     break;
        }
        ScanDisplay();
        ScanButtons(); 
        cnt ++;
        if( cnt == 100 ){
            SYSTEM_ACTION_FLG.DISP_UPDATE_ENABLE = 1; cnt = 0;
        }
        return;
    }
    if( INTCON3bits.INT2IE && INTCON3bits.INT2IF ) { //INT2 interrupt
        INTCON3bits.INT2IF = 0;
        ScanCounter(); 
        return;
    }
}

/* Low-priority interrupt routine */

void __interrupt(low_priority) low_isr(void)
{
    if( INTCON3bits.INT2IE && INTCON3bits.INT2IF ) { //INT2 interrupt
        INTCON3bits.INT2IF = 0;
//        SYSTEM_ACTION_FLG.DISP_UPDATE_ENABLE = 1;
        ScanCounter(); 
        return;
    }
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