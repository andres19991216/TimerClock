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

void main(void) {
     
    DeviceInit();
    
    while(1)
    {
        VerifyButtons(); 
        
        if( SYSTEM_STATE_FLG == RTC_CLOCK_SET_STATE && set_mode_release_cnt > SET_TIME_LIMIT ) {
            Ds1307Write( &real_rtc_data ); set_mode_release_cnt = 0;
            SYSTEM_STATE_FLG = RTC_CLOCK_PLAY_STATE;
        }
        
        if( counter_down_comp_flg ) {
            BLINK_TIME = DISP_BLINK_TIME1; blink_enable_flg = 5; 
        }
        else BLINK_TIME = DISP_BLINK_TIME;
        
        if( !SYSTEM_ACTION_FLG.DISP_UPDATE_ENABLE ) continue;
        SYSTEM_ACTION_FLG.DISP_UPDATE_ENABLE = 0;
        
        Puts1USART( "This is Ethernet protocol test firmware!\r\n" );
        
        switch( blink_enable_flg ) {
            case 0: BlinkDisable(); break;
            case 1: BlinkDisable(); blink_pos[ 6 ] = TRUE; break;
            case 2: BlinkDisable(); blink_pos[ 0 ] = TRUE; blink_pos[ 1 ] = TRUE; break;
            case 3: BlinkDisable(); blink_pos[ 2 ] = TRUE; blink_pos[ 3 ] = TRUE; break;
            case 4: BlinkDisable(); blink_pos[ 4 ] = TRUE; blink_pos[ 5 ] = TRUE; break;
            case 5: BlinkDisable(); blink_pos[ 0 ] = TRUE; blink_pos[ 1 ] = TRUE; blink_pos[ 2 ] = TRUE; blink_pos[ 3 ] = TRUE; blink_pos[ 4 ] = TRUE; blink_pos[ 5 ] = TRUE; break;
        }
        
        LEDProcess();
        
        //-------------- RTC clock display process --------------
        switch( SYSTEM_STATE_FLG ) {
            case RTC_CLOCK_PLAY_STATE: UpdateDisplay( Ds1307Read( rtc_clock_disp_mode ), 1, 6 ); break;
            case RTC_CLOCK_SET_STATE:  ClockSetting(); break; 
        }
                
        //-------------- TIMER and COUNTER display process --------------
        if( counter_skip_flg ) continue;
        switch( SYSTEM_STATE_FLG ) {
            case COUNTER_PLAY_STATE: UpdateDisplay( ReadCounter(), 1, 6 ); break;
            case COUNTER_STOP_STATE: UpdateDisplay( ReadCounter(), 1, 6 ); break;
            case COUNTER_SET_STATE:  UpdateDisplay( ReadCounter(), 1, 6 ); break;
            case COUNTER_COMP_STATE: UpdateDisplay( ReadCounter(), 1, 6 ); break;
        }
        
        
    }
}

void DeviceInit(){
    
    OscillatorInit();
    
    ADCON0 = 0x00; ADCON1 = 0x0F; //all analog pins use the digital in & out pins
    EnableAllInterrupts();        //Enable all interrupts 
    
    DetectStatus();
    
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
    //Puts1USART( "This is Ethernet protocol test firmware!\r\n" );

    blink_enable_flg = 1; 
    set_mode_release_cnt = 0; led_blink_cnt = 0;
//    SYSTEM_STATE_FLG = RTC_CLOCK_PLAY_STATE;
}

void CounterSetting() {
//    display_datas[0] = MarkDisplay( set_counter_hour   / 10 );
//    display_datas[1] = MarkDisplay( set_counter_hour   % 10 );
//    display_datas[2] = MarkDisplay( set_counter_minute / 10 );
//    display_datas[3] = MarkDisplay( set_counter_minute % 10 );
//    display_datas[4] = MarkDisplay( set_counter_second / 10 );
//    display_datas[5] = MarkDisplay( set_counter_second % 10 );
}

void ClockSetting() {
    display_datas[0] = MarkDisplay( real_rtc_data[ HOUR ]    >> 4 );
    display_datas[1] = MarkDisplay( real_rtc_data[ HOUR ]    % 16 );
    display_datas[2] = MarkDisplay( real_rtc_data[ MINUTE ]  >> 4 );
    display_datas[3] = MarkDisplay( real_rtc_data[ MINUTE ]  % 16 );
    display_datas[4] = MarkDisplay( real_rtc_data[ SECOND ]  >> 4 );
    display_datas[5] = MarkDisplay( real_rtc_data[ SECOND ]  % 16 );
}

void LEDProcess() {
    display_datas[ 7 ] &= ALL_LED_OFF;
 
    if( SYSTEM_STATE_FLG == RTC_CLOCK_PLAY_STATE || SYSTEM_STATE_FLG == RTC_CLOCK_SET_STATE ) {
        if( rtc_clock_disp_type == RTC_HOUR_TYPE_24 ) display_datas[ 7 ] |= TIMER_LED_ON;
        else                                          display_datas[ 7 ] |= CNTDN_LED_ON;
    }
    else if( SYSTEM_STATE_FLG == COUNTER_PLAY_STATE || SYSTEM_STATE_FLG == COUNTER_SET_STATE || SYSTEM_STATE_FLG == COUNTER_STOP_STATE || SYSTEM_STATE_FLG == COUNTER_COMP_STATE ) {
        if( counter_direction_flg == C_INC ) {
            display_datas[ 7 ] |= TIMER_LED_ON;
            if( SYSTEM_STATE_FLG == COUNTER_PLAY_STATE ) { display_datas[ 7 ] |= LAP_LED_ON; display_datas[ 7 ] |= STOP_LED_ON; }
            if( SYSTEM_STATE_FLG == COUNTER_STOP_STATE ) { display_datas[ 7 ] |= LAP_LED_ON; display_datas[ 7 ] |= START_LED_ON; }
            if( timer_reset_flg ) {
                led_blink_cnt ++;
                if( led_blink_cnt < LED_BLINK_TIME / 2 ) display_datas[ 7 ] |= LAP_LED_ON;
                if( led_blink_cnt > LED_BLINK_TIME ) led_blink_cnt = 0;
            }
        }
        else {
            display_datas[ 7 ] |= CNTDN_LED_ON;
            if( SYSTEM_STATE_FLG == COUNTER_PLAY_STATE ) { display_datas[ 7 ] |= LAP_LED_ON; display_datas[ 7 ] |= STOP_LED_ON; }
            if( SYSTEM_STATE_FLG == COUNTER_STOP_STATE ) { display_datas[ 7 ] |= LAP_LED_ON; display_datas[ 7 ] |= START_LED_ON; }
        }
    }
}

void DetectStatus(){
    TRISAbits.TRISA0 = DIGITAL_INPUT;
    TRISAbits.TRISA1 = DIGITAL_INPUT;
    TRISAbits.TRISA2 = DIGITAL_INPUT;
    TRISAbits.TRISA5 = DIGITAL_INPUT;
    if( PORTAbits.RA0 == HIGH && PORTAbits.RA1 == HIGH && PORTAbits.RA2 == HIGH && PORTAbits.RA5 == HIGH ) {
        SYSTEM_STATE_FLG  = COUNTER_STOP_STATE;
    }
    else if( PORTAbits.RA0 == LOW && PORTAbits.RA1 == LOW && PORTAbits.RA2 == LOW && PORTAbits.RA5 == LOW ) {
        SYSTEM_STATE_FLG  = RTC_CLOCK_PLAY_STATE;
    }
}

void InitValue(){
    if( SYSTEM_STATE_FLG == RTC_CLOCK_PLAY_STATE ){
    }
    else if( SYSTEM_STATE_FLG == COUNTER_PLAY_STATE ){
        counter_skip_flg = 0; 
    }
}