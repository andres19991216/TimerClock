/*
 * File:   Main.c
  *
 * Created: 30/08/2023 
 * Author: Boris
 */

#include "Config.h"
#include "I2C.h"
#include "DS1307.h"
#include "M74HC595.h"
#include "TLC5917.h"
#include "Timers.h"
#include "Interrupts.h"
#include "Main.h"
#include "Counter.h"
#include "Display.h"
#include "Buttons.h"
#include "Uart.h"
#include "Eeprom.h"
#include "MCP41010.h"
#include <stdio.h>

void main(void) {
    
//    printf("Hello");
     
    DeviceInit();
    
    while(1)
    {
        VerifyButtons(); 
        
        if( counter_down_comp_flg ) {
            BLINK_TIME = DISP_BLINK_TIME1; blink_enable_flg = 5; counter_skip_flg = 0; 
        }
        else { 
            BLINK_TIME = DISP_BLINK_TIME; 
            if( SYSTEM_STATE_FLG == COUNTER_COMP_STATE ) blink_enable_flg = 0; 
        }
   
        if( !SYSTEM_ACTION_FLG.DISP_UPDATE_ENABLE ) continue;
        SYSTEM_ACTION_FLG.DISP_UPDATE_ENABLE = 0;
        
        LEDProcess();
        
        switch( blink_enable_flg ) {
            case 0: BlinkDisable(); break;
            case 1: BlinkDisable(); blink_pos[ 6 ] = TRUE; break;
            case 2: BlinkDisable(); blink_pos[ 0 ] = TRUE; blink_pos[ 1 ] = TRUE; break;
            case 3: BlinkDisable(); blink_pos[ 2 ] = TRUE; blink_pos[ 3 ] = TRUE; break;
            case 4: BlinkDisable(); blink_pos[ 4 ] = TRUE; blink_pos[ 5 ] = TRUE; break;
            case 5: BlinkDisable(); blink_pos[ 0 ] = TRUE; blink_pos[ 1 ] = TRUE; blink_pos[ 2 ] = TRUE; blink_pos[ 3 ] = TRUE; blink_pos[ 4 ] = TRUE; blink_pos[ 5 ] = TRUE; blink_pos[ 6 ] = TRUE; break;
        }
        
        //-------------- RTC clock display process --------------
        switch( SYSTEM_STATE_FLG ) {
            case RTC_CLOCK_PLAY_STATE: UpdateDisplay( Ds1307HMSRead(), 1, 6 ); blink_enable_flg = 0; break;
        }
        
        //-------------- TIMER and COUNTER display process --------------
        if( counter_skip_flg ) continue;
        switch( SYSTEM_STATE_FLG ) {
            case COUNTER_PLAY_STATE: 
            case COUNTER_STOP_STATE: 
            case COUNTER_SET_STATE:  
            case COUNTER_COMP_STATE: UpdateDisplay( ReadCounter(), 1, 6 ); break;
        }
    }
}

void DeviceInit(){
    
    OscillatorInit();
    
    ADCON0 = 0x00; ADCON1 = 0x0F; //all analog pins use the digital in & out pins
    EnableAllInterrupts();        //Enable all interrupts 
    
    DetectStatus();
//    SYSTEM_STATE_FLG  = COUNTER_STOP_STATE; 
    
    OpenTimer0( TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_8 ); //Timer0 init.
    WriteTimer0( 65285 ); StartTimer0();                               //Set the timer0 overflow interrupt for displays and update the values
    
    OpenCounter();   //Init the Timer and Counter
    OpenDisplay();   //Init display ( for driver TLC5917 )
    OpenButtons();   //Init button
    Ds1307Init();    //Init Rtc clock ( DS1307 )
        
//    MCP41010Init();
//    MCP41010Write( MCP41010_WRITE | MCP41010_SEL_POS0, 50 );
    
    InitValue();
    
    Open1USART( USART_TX_INT_OFF &
                USART_RX_INT_OFF &
                USART_ASYNCH_MODE &
                USART_EIGHT_BIT &
                USART_CONT_RX &
                USART_BRGH_HIGH, 103);
    Puts1USART( "This is Ethernet protocol test firmware!\r\n" );

    set_mode_release_cnt = 0;
}

void LEDProcess() {
    display_datas[ 7 ] &= ALL_LED_OFF;
    switch( SYSTEM_STATE_FLG ){
        case RTC_CLOCK_PLAY_STATE:
        case RTC_CLOCK_SET_STATE:
            if( rtc_clock_disp_type == RTC_HOUR_TYPE_24 ) display_datas[ 7 ] |= TIMER_LED_ON;
            else                                          display_datas[ 7 ] |= CNTDN_LED_ON;
            return;
        case COUNTER_PLAY_STATE:
            if(counter_down_comp_flg) { display_datas[ 7 ] |= START_STOP_RED_LED_ON; }
            else                        display_datas[ 7 ] |= START_STOP_GREEN_LED_ON;
            if( counter_skip_flg ) {
                led_blink_cnt ++;
                if( led_blink_cnt < LED_BLINK_TIME / 2 ) display_datas[ 7 ] |= LAP_RESET_RED_LED_ON;
                if( led_blink_cnt > LED_BLINK_TIME )     led_blink_cnt = 0;
            }
            break;
        case COUNTER_STOP_STATE:
            display_datas[ 7 ] |= START_STOP_RED_LED_ON;
            break;
        case COUNTER_SET_STATE:
            display_datas[ 7 ] |= START_STOP_RED_LED_ON;
            break;
        case COUNTER_COMP_STATE:
            display_datas[ 7 ] |= START_STOP_RED_LED_ON;
            break;
    }
    if( counter_direction_flg == C_INC )  { display_datas[ 7 ] |= TIMER_LED_ON; } 
    else                                  { display_datas[ 7 ] |= CNTDN_LED_ON; } 
}
    
void DetectStatus(){
    TRISAbits.TRISA0 = DIGITAL_INPUT;
    TRISAbits.TRISA1 = DIGITAL_INPUT;
    TRISAbits.TRISA2 = DIGITAL_INPUT;
    TRISAbits.TRISA5 = DIGITAL_INPUT;
    if( PORTAbits.RA0 == HIGH && PORTAbits.RA1 == HIGH && PORTAbits.RA2 == HIGH && PORTAbits.RA5 == HIGH ) {
        SYSTEM_STATE_FLG  = COUNTER_STOP_STATE; blink_enable_flg = 0;
    }
    else if( PORTAbits.RA0 == LOW && PORTAbits.RA1 == HIGH && PORTAbits.RA2 == HIGH && PORTAbits.RA5 == HIGH ) {
        SYSTEM_STATE_FLG  = RTC_CLOCK_PLAY_STATE; blink_enable_flg = 0;
    }
}

void InitValue(){
    if( SYSTEM_STATE_FLG == RTC_CLOCK_PLAY_STATE ){
        blink_enable_flg = 1; 
        UpdateDisplay( Ds1307HMSRead(), 1, 6 );
        if( real_rtc_data[ HOUR ] == 0 && real_rtc_data[ MINUTE ] == 0 && real_rtc_data[ SECOND ] == 0 ) Ds1307Write( &rtc_default_values );
    }
    else if( SYSTEM_STATE_FLG == COUNTER_STOP_STATE ){
        counter_enable_flg = 0; counter_skip_flg = 0; //Ds1307Write( &rtc_default_values );
    }
}