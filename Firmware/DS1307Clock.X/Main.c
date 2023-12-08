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
#include "MCP41010.h"

void main(void) {
     
    DeviceInit();
    
    while(1)
    {
        
        VerifyButtons(); 
        
        if( !SYSTEM_ACTION_FLG.DISP_UPDATE_ENABLE ) continue;
        SYSTEM_ACTION_FLG.DISP_UPDATE_ENABLE = 0;
        
        switch( blink_enable_flg ) {
            case 1: BlinkDisable(); blink_pos[ 6 ] = TRUE; break;
            case 2: BlinkDisable(); blink_pos[ 0 ] = TRUE; blink_pos[ 1 ] = TRUE; break;
            case 3: BlinkDisable(); blink_pos[ 2 ] = TRUE; blink_pos[ 3 ] = TRUE; break;
            case 4: BlinkDisable(); blink_pos[ 4 ] = TRUE; blink_pos[ 5 ] = TRUE; break;
        }
        
        LEDProcess();
        
        //-------------- RTC clock display process --------------
        switch( SYSTEM_STATE_FLG ) {
            case RTC_CLOCK_PLAY_STATE: UpdateDisplay( Ds1307Read( rtc_clock_disp_mode ), 1, 6 ); break;
            case RTC_CLOCK_YEAR_SET_STATE:   
            case RTC_CLOCK_MONTH_SET_STATE:  
            case RTC_CLOCK_DAY_SET_STATE:    
            case RTC_CLOCK_WDAY_SET_STATE:   
            case RTC_CLOCK_HOUR_SET_STATE:   
            case RTC_CLOCK_MINUTE_SET_STATE: 
            case RTC_CLOCK_SECOND_SET_STATE: 
            case RTC_CLOCK_AMPM_SET_STATE:   ClockSetting( rtc_clock_set_mode ); break;
        }
                
        //-------------- TIMER and COUNTER display process --------------
        if( counter_skip_flg ) continue;
        switch( SYSTEM_STATE_FLG ) {
            case COUNTER_PLAY_STATE: UpdateDisplay( ReadCounter(), 1, 6 ); break;
            case COUNTER_STOP_STATE: UpdateDisplay( ReadCounter(), 1, 6 ); break;
            case COUNTER_HOUER_SET_STATE:  
            case COUNTER_MINUTE_SET_STATE: 
            case COUNTER_SECOND_SET_STATE: CounterSetting(); break;
        }
    }
}

void DeviceInit(){
    
    OscillatorInit();
    
    ADCON0 = 0x00; ADCON1 = 0x0F; //all analog pins use the digital in & out pins
    EnableAllInterrupts();        //Enable all interrupts 
    
    OpenTimer0( TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_8 ); //Timer0 init.
    WriteTimer0( 65000 ); StartTimer0();                               //Set the timer0 overflow interrupt for displays and update the values
    
    OpenCounter();   //Init the Timer and Counter
    OpenDisplay();   //Init display ( for driver TLC5917 )
    OpenButtons();   //Init button
    Ds1307Init();    //Init Rtc clock ( DS1307 )
    Ds1307Write( &rtc_default_values );
    
    MCP41010Init();
    MCP41010Write( MCP41010_WRITE | MCP41010_SEL_POS0, 200 );
    
//    Open1USART( USART_TX_INT_OFF &
//                USART_RX_INT_OFF &
//                USART_ASYNCH_MODE &
//                USART_EIGHT_BIT &
//                USART_CONT_RX &
//                USART_BRGH_HIGH, 103);
//    Puts1USART( "This is Ethernet protocol test firmware!\r\n" );
    
    counter_hour_set_value = 10; counter_min_set_value = 30; counter_sec_set_value = 50; counter_enable_flg = 1;

    SYSTEM_STATE_FLG  = RTC_CLOCK_PLAY_STATE; 
    rtc_clock_disp_mode = RTC_HO_MI_SE_DISP_MODE;
    rtc_clock_set_mode = RTC_CLCOK_YE_MO_DA_SET_MOD;
    blink_enable_flg = 1; 
}

void CounterSetting() {
    display_datas[0] = MarkDisplay( set_counter_hour   / 10 );
    display_datas[1] = MarkDisplay( set_counter_hour   % 10 );
    display_datas[2] = MarkDisplay( set_counter_minute / 10 );
    display_datas[3] = MarkDisplay( set_counter_minute % 10 );
    display_datas[4] = MarkDisplay( set_counter_second / 10 );
    display_datas[5] = MarkDisplay( set_counter_second % 10 );
}

void ClockSetting( unsigned char set_mod ) {
    switch( set_mod ) {
        case RTC_CLCOK_YE_MO_DA_SET_MOD:
            display_datas[0] = MarkDisplay( real_rtc_data[ YEAR ]   >> 4 );
            display_datas[1] = MarkDisplay( real_rtc_data[ YEAR ]   % 16 );
            display_datas[2] = MarkDisplay( real_rtc_data[ MONTH ]  >> 4 );
            display_datas[3] = MarkDisplay( real_rtc_data[ MONTH ]  % 16 );
            display_datas[4] = MarkDisplay( real_rtc_data[ DAY ]    >> 4 );
            display_datas[5] = MarkDisplay( real_rtc_data[ DAY ]    % 16 );
            break;
        case RTC_CLCOK_HO_MI_SE_SET_MOD:
            display_datas[0] = MarkDisplay( real_rtc_data[ HOUR ]    >> 4 );
            display_datas[1] = MarkDisplay( real_rtc_data[ HOUR ]    % 16 );
            display_datas[2] = MarkDisplay( real_rtc_data[ MINUTE ]  >> 4 );
            display_datas[3] = MarkDisplay( real_rtc_data[ MINUTE ]  % 16 );
            display_datas[4] = MarkDisplay( real_rtc_data[ SECOND ]  >> 4 );
            display_datas[5] = MarkDisplay( real_rtc_data[ SECOND ]  % 16 );
            break;
        case RTC_CLCOK_WDAY_AMPM_SET_MOD:
            display_datas[0] = MarkDisplay( real_rtc_data[ WEEKDAY ] >> 4 );
            display_datas[1] = MarkDisplay( real_rtc_data[ WEEKDAY ] % 16 );
            display_datas[2] = 0xff;//MarkDisplay( real_rtc_data[ MONTH ]  >> 4 );
            display_datas[3] = 0xff;//MarkDisplay( real_rtc_data[ MONTH ]  % 16 );
            display_datas[4] = 0xff;//MarkDisplay( real_rtc_data[ DAY ]    >> 4 );
            display_datas[5] = 0xff;//MarkDisplay( real_rtc_data[ DAY ]    % 16 );
            break;
    }
}

void LEDProcess() {
    display_datas[ 7 ] |= ALL_LED_OFF;
    if( SYSTEM_STATE_FLG == RTC_CLOCK_PLAY_STATE ) {
        if( rtc_clock_disp_type == RTC_HOUR_TYPE_24 ) display_datas[ 7 ] &= TIMER_LED_ON;
        else display_datas[ 7 ] &= CNTDN_LED_ON;
    }
}

