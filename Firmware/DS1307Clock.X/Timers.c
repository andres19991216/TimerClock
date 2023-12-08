/*
 * File:   Timers.c
  *
 * Created: 30/08/2023 
 * Author: Thao Tran
 */

#include "Timers.h"
#include "Config.h"

/* ***** TIMER0 ***** */

void OpenTimer0(unsigned char config)
{
    T0CON = (0x7f & config);  // Configure timer, but don't start it yet
    TMR0H = 0;                // Reset Timer0 to 0x0000
    TMR0L = 0;
    INTCONbits.TMR0IF = 0;      // Clear Timer0 overflow flag

    if(config&0x80)           // If interrupts enabled
        INTCONbits.TMR0IE = 1;    // Enable Timer0 overflow interrupt
    else
        INTCONbits.TMR0IE = 0;

}

unsigned int ReadTimer0(void)
{
    union Timers timer;

    timer.bt[0] = TMR0L;  // Copy Timer0 low byte into union
    timer.bt[1] = TMR0H;  // Copy Timer0 high byte into union

    return (timer.lt);    // Return the int
}

void WriteTimer0(unsigned int timer0)
{
    union Timers timer;

    timer.lt = timer0;    // Copy timer value into union

    TMR0H = timer.bt[1];  // Write high byte to Timer0
    TMR0L = timer.bt[0];  // Write low byte to Timer0
}

void StartTimer0 (void) {
    T0CONbits.TMR0ON = 1;  // Start Timer0
}

void StopTimer0 (void){
    T0CONbits.TMR0ON = 0;  // Stop Timer0
}

void CloseTimer0(void)
{
    T0CONbits.TMR0ON = 0;  // Disable Timer0
    INTCONbits.TMR0IE = 0;   // Disable Timer0 overflow interrupts
}
//-----------------------------------------------------------------------------------------------------------

/* ***** TIMER1 ***** */

void OpenTimer1(unsigned char config)
{
    T1CON = (0x7e & config); // Set everything except 8/16 mode, and
                              // don't start timer yet

    if( config & 0x40 )       // The 8/16 selection bit isn't in the
        T1CONbits.RD16 = 1;   // right place -- we have to move it
    else
        T1CONbits.RD16 = 0;

    TMR1H=0;             // Clear out timer registers
    TMR1L=0;
    PIR1bits.TMR1IF=0;

    if(config&0x80)      // Enable interrupts if selected
        PIE1bits.TMR1IE=1;
    else
        PIE1bits.TMR1IE=0;
  
}

unsigned int ReadTimer1(void)
{
    union Timers timer;

    timer.bt[0] = TMR1L;    // Read Lower byte
    timer.bt[1] = TMR1H;    // Read upper byte

    return (timer.lt);      // Return the 16-bit value
}

void WriteTimer1(unsigned int timer1)
{
    union Timers timer;

    timer.lt = timer1;    // Save the 16-bit value in local

    TMR1H = timer.bt[1];  // Write high byte to Timer1 High byte
    TMR1L = timer.bt[0];  // Write low byte to Timer1 Low byte
}

void StartTimer1 (void){
    T1CONbits.TMR1ON = 1;  // Start Timer1
}

void StopTimer1 (void){
    T1CONbits.TMR1ON = 0;  // Stop Timer1
}

void CloseTimer1(void)
{
    T1CONbits.TMR1ON = 0;  // Disable Timer1
    PIE1bits.TMR1IE = 0;   // Disable Timer1 overflow interrupts 
}
//-----------------------------------------------------------------------------------------------------------

/* ***** TIMER2 ***** */
void OpenTimer2(unsigned char config)
{
    T2CON = (0xfb & config);  // Set all configuration values, but
                              // don't start timer yet 								

    TMR2 = 0;                 // Clear Timer2
    PIR1bits.TMR2IF = 0;

    if(config & 0x80)         // Enable timer interrupts?
        PIE1bits.TMR2IE = 1;
    else
        PIE1bits.TMR2IE = 0;
}

unsigned char ReadTimer2(void)
{
    unsigned char timer;

    timer = TMR2;      // Read Timer2

    return (timer);    // Return 8-bits
}

void WriteTimer2(unsigned char timer2)
{
    TMR2 = timer2;    // Write byte to Timer2
}

void StartTimer2 (void){
    T2CONbits.TMR2ON = 1;  // Start Timer2
}

void StopTimer2 (void){
    T2CONbits.TMR2ON = 0;  // Stop Timer2
}

void CloseTimer2(void)
{
    T2CONbits.TMR2ON = 0;  // Disable Timer2
    PIE1bits.TMR2IE = 0;   // Disable Timer2 overflow interrupts
}
//-----------------------------------------------------------------------------------------------------------

/* ***** TIMER3 ***** */

void OpenTimer3(unsigned char config)
{
    T3CON = (0x7E & config);  // Set everything except 8/16 mode and Timer as CCP source
                              // don't start timer yet
    
    if(config & 0x01)   // Interrupt on/off
        T3CONbits.RD16 = 1;
    else
        T3CONbits.RD16 = 0;
    
    TMR3H=0;          //Clear timer3-related registers
    TMR3L=0;
    
    PIR2bits.TMR3IF = 0;

    if(config & 0x80)   // Interrupt on/off
        PIE2bits.TMR3IE = 1;
    else
        PIE2bits.TMR3IE = 0;

}

unsigned int ReadTimer3(void)
{
    union Timers timer;

    timer.bt[0] = TMR3L;   // Read low byte of Timer3
    timer.bt[1] = TMR3H;   // Read high byte of Timer3

    return (timer.lt);     // Return 16-bit value
}

void WriteTimer3(unsigned int timer3)
{
    union Timers timer;

    timer.lt = timer3;    // Save value in local variable
    TMR3H = timer.bt[1];  // Write high byte to Timer3
    TMR3L = timer.bt[0];  // Write low byte to Timer3
}

void StartTimer3 (void){
    T3CONbits.TMR3ON = 1;  // Start Timer3
}

void StopTimer3 (void){
    T3CONbits.TMR3ON = 0;  // Stop Timer3
}

void CloseTimer3(void)
{
  T3CONbits.TMR3ON = 0;  // Disable Timer3
  PIE2bits.TMR3IE = 0;   // Disable Timer3 overflow interrupts
}
//-----------------------------------------------------------------------------------------------------------