
#include "Config.h"
#include "Buttons.h"
#include "Main.h"
#include "Counter.h"
#include "Display.h"
#include "DS1307.h"
#include "Eeprom.h"

void OpenButtons(){
    
    HOUR_BUTTON_TRIS   = DIGITAL_INPUT;
    MINUTE_BUTTON_TRIS = DIGITAL_INPUT;
    SECOND_BUTTON_TRIS = DIGITAL_INPUT;
    SELECT_BUTTON_TRIS = DIGITAL_INPUT;
    RESET_BUTTON_TRIS  = DIGITAL_INPUT;
    
    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; BTN_CLICK_EVENT = 0;
    
    hour_btn_pressed_cnt  = 0; up_btn_pressed_cnt  = 0; down_btn_pressed_cnt  = 0; sel_btn_pressed_cnt  = 0; reset_btn_pressed_cnt = 0;
    hour_btn_released_cnt = 0; up_btn_released_cnt = 0; down_btn_released_cnt = 0; sel_btn_released_cnt = 0; reset_btn_released_cnt = 0;
    continue_pressed_cnt = 0;
    
    btn_press_verify_flg = FALSE; btn_long_press_verify_flg = FALSE;
    
}

void ScanButtons(){
    
    if( HOUR_BUTTON   == BUTTON_PRESSED ) { hour_btn_released_cnt = 0; hour_btn_pressed_cnt ++; BTN_CLICK_EVENT = 1; }
    if( HOUR_BUTTON   == BUTTON_RELEASED && BTN_CLICK_EVENT == 1 ){ hour_btn_pressed_cnt = 0; hour_btn_released_cnt ++; }
    
    if( UP_BUTTON     == BUTTON_PRESSED ) { up_btn_released_cnt = 0; up_btn_pressed_cnt ++; BTN_CLICK_EVENT = 2; }
    if( UP_BUTTON     == BUTTON_RELEASED && BTN_CLICK_EVENT == 2 ){ up_btn_pressed_cnt = 0; up_btn_released_cnt ++; }
    
    if( DOWN_BUTTON   == BUTTON_PRESSED ) { down_btn_released_cnt = 0; down_btn_pressed_cnt ++; BTN_CLICK_EVENT = 3; }
    if( DOWN_BUTTON   == BUTTON_RELEASED && BTN_CLICK_EVENT == 3 ){ down_btn_pressed_cnt = 0; down_btn_released_cnt ++; }
    
    if( SELECT_BUTTON == BUTTON_PRESSED ) { sel_btn_released_cnt = 0; sel_btn_pressed_cnt ++; BTN_CLICK_EVENT = 4; }
    if( SELECT_BUTTON == BUTTON_RELEASED && BTN_CLICK_EVENT == 4 ){ sel_btn_pressed_cnt = 0; sel_btn_released_cnt ++; }
    
    if( RESET_BUTTON  == BUTTON_PRESSED ) { reset_btn_released_cnt = 0; reset_btn_pressed_cnt ++; BTN_CLICK_EVENT = 5; }
    if( RESET_BUTTON  == BUTTON_RELEASED && BTN_CLICK_EVENT == 5 ){ reset_btn_pressed_cnt = 0; reset_btn_released_cnt ++; }
    
}

void VerifyButtons(){
    
    switch( BTN_CLICK_EVENT ) {
        // Hour button process block
        case 1: if( hour_btn_pressed_cnt > BUTTON_LONG_PRESSED_TIME ) { if( !btn_long_press_verify_flg ) BTN_CLICK_STATE = HOUR_BUTTON_LONG_PRESSED; btn_long_press_verify_flg = TRUE; hour_btn_pressed_cnt = BUTTON_LONG_PRESSED_TIME; ActionButtons(); }
                else if( hour_btn_pressed_cnt > BUTTON_PRESSED_TIME ) { if( !btn_press_verify_flg )      BTN_CLICK_STATE = HOUR_BUTTON_PRESSED;      btn_press_verify_flg      = TRUE; ActionButtons(); }
                if( hour_btn_released_cnt > BUTTON_RELEASED_TIME )     { 
                    hour_btn_released_cnt = 0; BTN_CLICK_EVENT = 0; BTN_CLICK_EVENT = 0; btn_long_press_verify_flg = FALSE; btn_press_verify_flg      = FALSE;
                } 
                break;
        // Minute(Up) button process block        
        case 2: if( up_btn_pressed_cnt > BUTTON_LONG_PRESSED_TIME ) { if( !btn_long_press_verify_flg ) BTN_CLICK_STATE = UP_BUTTON_LONG_PRESSED; btn_long_press_verify_flg = TRUE; up_btn_pressed_cnt = BUTTON_LONG_PRESSED_TIME; ActionButtons(); }
                else if( up_btn_pressed_cnt > BUTTON_PRESSED_TIME ) { if( !btn_press_verify_flg )      BTN_CLICK_STATE = UP_BUTTON_PRESSED;      btn_press_verify_flg      = TRUE; ActionButtons(); }
                if( up_btn_released_cnt > BUTTON_RELEASED_TIME )     { 
                    up_btn_released_cnt = 0; BTN_CLICK_EVENT = 0; BTN_CLICK_EVENT = 0; btn_long_press_verify_flg = FALSE; btn_press_verify_flg      = FALSE; BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                } 
                break;
        // Second(Dn) button process block
        case 3: if( down_btn_pressed_cnt > BUTTON_LONG_PRESSED_TIME ) { if( !btn_long_press_verify_flg ) BTN_CLICK_STATE = DOWN_BUTTON_LONG_PRESSED; btn_long_press_verify_flg = TRUE; down_btn_pressed_cnt = BUTTON_LONG_PRESSED_TIME; ActionButtons(); }
                else if( down_btn_pressed_cnt > BUTTON_PRESSED_TIME ) { if( !btn_press_verify_flg )      BTN_CLICK_STATE = DOWN_BUTTON_PRESSED;      btn_press_verify_flg      = TRUE; ActionButtons(); }
                if( down_btn_released_cnt > BUTTON_RELEASED_TIME )     { 
                    down_btn_released_cnt = 0; BTN_CLICK_EVENT = 0; BTN_CLICK_EVENT = 0; btn_long_press_verify_flg = FALSE; btn_press_verify_flg    = FALSE; BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    if( btn_long_press_verify_flg ) { btn_long_press_verify_flg = FALSE; } 
                    else if( btn_press_verify_flg ) { btn_press_verify_flg      = FALSE; }
                } 
                break;
        // Select(Start/Stop) button process block
        case 4: if( sel_btn_pressed_cnt > BUTTON_LONG_PRESSED_TIME ) { if( !btn_long_press_verify_flg ) BTN_CLICK_STATE = SELECT_BUTTON_LONG_PRESSED; btn_long_press_verify_flg = TRUE; sel_btn_pressed_cnt = BUTTON_LONG_PRESSED_TIME; ActionButtons(); }
                else if( sel_btn_pressed_cnt > BUTTON_PRESSED_TIME ) { if( !btn_press_verify_flg )      BTN_CLICK_STATE = START_BUTTON_PRESSED;       btn_press_verify_flg      = TRUE; ActionButtons(); }
                if( sel_btn_released_cnt > BUTTON_RELEASED_TIME )     { 
                    sel_btn_released_cnt = 0; BTN_CLICK_EVENT = 0; BTN_CLICK_EVENT = 0; btn_long_press_verify_flg = FALSE; btn_press_verify_flg      = FALSE; 
                } 
                break;
        // Reset(Lap) button process block        
        case 5: if( reset_btn_pressed_cnt > BUTTON_LONG_PRESSED_TIME ) { if( !btn_long_press_verify_flg ) BTN_CLICK_STATE = RESET_BUTTON_LONG_PRESSED; btn_long_press_verify_flg = TRUE; reset_btn_pressed_cnt = BUTTON_LONG_PRESSED_TIME; ActionButtons(); }
                else if( reset_btn_pressed_cnt > BUTTON_PRESSED_TIME ) { if( !btn_press_verify_flg )      BTN_CLICK_STATE = RESET_BUTTON_PRESSED;      btn_press_verify_flg      = TRUE; ActionButtons(); }
                if( reset_btn_released_cnt > BUTTON_RELEASED_TIME )     { 
                    reset_btn_released_cnt = 0; BTN_CLICK_EVENT = 0; btn_long_press_verify_flg = FALSE; btn_press_verify_flg      = FALSE;
                } 
                break;
    }
}

void ActionButtons(){
    
    if( counter_down_comp_flg ) {
        blink_enable_flg = 0; counter_down_comp_flg = 0; SYSTEM_STATE_FLG = COUNTER_COMP_STATE; BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
    }
    
    // Click select button with long time, in this case system entry RTC_TIME_PLAY_STATE or COUNTER_PLAY_STATE
    
    //-----------------------------------------------RTC clock Process---------------------------------------------------------------
    // RTC Time play state
    signed char temp;
    switch( SYSTEM_STATE_FLG ){
        
        case RTC_CLOCK_PLAY_STATE:      //RTC clock play state
            switch( BTN_CLICK_STATE ) {
                case START_BUTTON_PRESSED:  // Change the clock display (24/12)
                    if( rtc_clock_disp_type == RTC_HOUR_TYPE_24 ){
                        if( rtc_hour_ampm_flg == RTC_HOUR_TYPE_AM && real_rtc_data[ HOUR ] == 0 ) real_rtc_data[ HOUR ] = 0x12;
                        rtc_clock_disp_type = RTC_HOUR_TYPE_12;
                    }
                    else {
                        if( rtc_hour_ampm_flg == RTC_HOUR_TYPE_AM && real_rtc_data[ HOUR ] == 0x12 ) real_rtc_data[ HOUR ] = 0x00;
                        rtc_clock_disp_type = RTC_HOUR_TYPE_24;
                    }
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; Ds1307Write( &real_rtc_data ); 
                    return;
                case HOUR_BUTTON_PRESSED: 
                    SYSTEM_STATE_FLG = RTC_CLOCK_SET_STATE;
                    break;
                case UP_BUTTON_PRESSED: 
                    SYSTEM_STATE_FLG = RTC_CLOCK_SET_STATE;
                    break;
                case DOWN_BUTTON_PRESSED: 
                    SYSTEM_STATE_FLG = RTC_CLOCK_SET_STATE;
                    break;
            }
            break;
        case RTC_CLOCK_SET_STATE:      //RTC clock play state
            switch( BTN_CLICK_STATE ) {
                case HOUR_BUTTON_PRESSED: 
                    temp = real_rtc_data[ HOUR ]; temp ++;
                    if( temp % 16 >= 10 ) temp += 6; 
                    if( rtc_clock_disp_type == RTC_HOUR_TYPE_24 ){
                        if( temp > 0x11 ) { rtc_hour_ampm_flg = RTC_HOUR_TYPE_PM; }
                        if( temp > 0x23 ) { temp = 0; rtc_hour_ampm_flg = RTC_HOUR_TYPE_AM; } 
                    }
                    else if( rtc_clock_disp_type == RTC_HOUR_TYPE_12 ){
                        if( temp > 0x12 ) temp = 1;
                        if( temp > 0x11 ) {
                            if( rtc_hour_ampm_flg == RTC_HOUR_TYPE_PM ) rtc_hour_ampm_flg = RTC_HOUR_TYPE_AM;
                            else                                        rtc_hour_ampm_flg = RTC_HOUR_TYPE_PM;
                        }
                    }
                    real_rtc_data[ HOUR ] = temp;
                    BTN_CLICK_STATE  = BUTTON_EMPTY_STATE;
                    set_mode_release_cnt = 0;
                    return;
                case UP_BUTTON_PRESSED: 
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    temp = real_rtc_data[ MINUTE ]; temp ++;
                    if( temp % 16 >= 10 ) temp += 6; 
                    if( temp > 0x59 ) temp = 0; 
                    real_rtc_data[ MINUTE ] = temp;
                    set_mode_release_cnt = 0;
                    return;
                case DOWN_BUTTON_PRESSED: 
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    temp = real_rtc_data[ SECOND ]; temp ++;
                    if( temp % 16 >= 10 ) temp += 6; 
                    if( temp > 0x59 ) temp = 0; 
                    real_rtc_data[ SECOND ] = temp;
                    set_mode_release_cnt = 0;
                    return;
            }
            break;  
            
        //-----------------------------------------------Timer/Counter Process---------------------------------------------------------------    
        case COUNTER_PLAY_STATE:          //Counter play state
            switch( BTN_CLICK_STATE ) {
                case START_BUTTON_PRESSED:  //Counter start and stop
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; 
                    if( counter_skip_flg ) return;
                    if( !counter_enable_flg ) { SYSTEM_STATE_FLG = COUNTER_PLAY_STATE; StartCounter(); blink_enable_flg = 1; }
                    else                      { SYSTEM_STATE_FLG = COUNTER_STOP_STATE; StopCounter();  blink_enable_flg = 0; }
                    temp_system_state = SYSTEM_STATE_FLG; 
                    return;
                case RESET_BUTTON_PRESSED:      //Skip the counter display
                    if( !counter_enable_flg ) return;
                    counter_skip_flg = 1 - counter_skip_flg; 
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; 
                    return;
                case RESET_BUTTON_LONG_PRESSED: //Change the counter direction (up/down)
                    UpDownTransfer();
                    counter_skip_flg = 0;
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; 
                    return;
            }
            break;
        case COUNTER_STOP_STATE:         //Counter stop state
            counter_skip_flg = 0; blink_enable_flg = 0;
            switch( BTN_CLICK_STATE ) {
                case START_BUTTON_PRESSED:  
                    if( !counter_enable_flg ) { SYSTEM_STATE_FLG = COUNTER_PLAY_STATE; StartCounter(); blink_enable_flg = 1; } 
                    else                      { SYSTEM_STATE_FLG = COUNTER_STOP_STATE; StopCounter();  blink_enable_flg = 0; } 
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; temp_system_state = SYSTEM_STATE_FLG; 
                    return;
                case RESET_BUTTON_PRESSED:      //Skip the counter display
                    if( counter_direction_flg == C_INC ){
                        counter_up_hour   = 0; 
                        counter_up_minute = 0; 
                        counter_up_second = 0;
                    }
                    else if( counter_direction_flg == C_DEC ){
                        counter_down_hour   = counter_down_set_hour;
                        counter_down_minute = counter_down_set_minute; 
                        counter_down_second = counter_down_set_second;
                    }
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; 
                    return;
                case RESET_BUTTON_LONG_PRESSED: //Change the counter direction (up/down)
                    UpDownTransfer(); 
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; 
                    return;
                case UP_BUTTON_PRESSED: 
                    if( counter_direction_flg == C_INC ) { BTN_CLICK_STATE = BUTTON_EMPTY_STATE; return; }
                    SYSTEM_STATE_FLG = COUNTER_SET_STATE;
                    break;
                case DOWN_BUTTON_PRESSED: 
                    if( counter_direction_flg == C_INC ) { BTN_CLICK_STATE = BUTTON_EMPTY_STATE; return; }
                    SYSTEM_STATE_FLG = COUNTER_SET_STATE;
                    break;
            }
            break;
        case COUNTER_COMP_STATE: 
            counter_direction_flg = C_DEC;
            blink_enable_flg = 0;
            switch( BTN_CLICK_STATE ) {
                case UP_BUTTON_PRESSED:    
                    SYSTEM_STATE_FLG = COUNTER_SET_STATE; break;
                case DOWN_BUTTON_PRESSED:  
                    SYSTEM_STATE_FLG = COUNTER_SET_STATE; break;
                case START_BUTTON_PRESSED:      
                    SYSTEM_STATE_FLG = COUNTER_PLAY_STATE; StartCounter(); blink_enable_flg = 1; 
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; 
                    return;
                case RESET_BUTTON_PRESSED:      //Skip the counter display
                    counter_down_hour   = 0; 
                    counter_down_minute = 0; 
                    counter_down_second = 0; 
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; 
                    return;
                case RESET_BUTTON_LONG_PRESSED: //Change the counter direction (up/down)
                    counter_direction_flg = C_INC;
                    SYSTEM_STATE_FLG = COUNTER_STOP_STATE; StopCounter(); 
                    counter_up_hour   = counter_down_hour;
                    counter_up_minute = counter_down_minute;
                    counter_up_second = counter_down_second;
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; 
                    return;
            }
            break;
        case COUNTER_SET_STATE:   //Hour set state
            switch( BTN_CLICK_STATE ) {
                case UP_BUTTON_LONG_PRESSED:
                    continue_pressed_cnt ++;
                    if( continue_pressed_cnt > BUTTON_CONTINUE_PRESSED_STEP_TIME ) {
                        IncValProcess(); continue_pressed_cnt = 0;
                    }
                    return;
                case DOWN_BUTTON_LONG_PRESSED:
                    continue_pressed_cnt ++;
                    if( continue_pressed_cnt > BUTTON_CONTINUE_PRESSED_STEP_TIME ) {
                        DecValProcess(); continue_pressed_cnt = 0;
                    }
                    return;
                case UP_BUTTON_PRESSED: 
                    blink_enable_flg = 1; 
                    IncValProcess();
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; 
                    return;
                case DOWN_BUTTON_PRESSED:  
                    blink_enable_flg = 1; 
                    DecValProcess();
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; 
                    return;
                case START_BUTTON_PRESSED:      //Skip the counter display
                    if( counter_direction_flg == C_INC ){
                        BTN_CLICK_STATE = BUTTON_EMPTY_STATE; return;
                    }
                    SYSTEM_STATE_FLG = COUNTER_PLAY_STATE; StartCounter(); blink_enable_flg = 1; 
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; 
                    return;
                case RESET_BUTTON_PRESSED:      //Skip the counter display
                    if( counter_direction_flg == C_INC ){
                        BTN_CLICK_STATE = BUTTON_EMPTY_STATE; return;
                    }
                    else if( counter_direction_flg == C_DEC ){
                        counter_down_hour   = 0; 
                        counter_down_minute = 0; 
                        counter_down_second = 0; 
                    }
                    blink_enable_flg = 0;
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; 
                    return;
            }
            break; 
    }
}

void IncValProcess(){
    counter_down_second ++;
    if( counter_down_second == 60 ){
        counter_down_second = 0;
        counter_down_minute ++;
        if(counter_down_minute == 60){
            counter_down_minute = 0;
            counter_down_hour ++;
        }
    }
    UpdateSetValue();
}

void DecValProcess(){
    counter_down_second --;
    if( counter_down_second < 0 && counter_down_hour + counter_down_minute) {
        counter_down_second = 59;
        counter_down_minute --; 
        if( counter_down_minute < 0 && counter_down_hour ){
            counter_down_minute = 59;
            counter_down_hour --; 
        }
        else if( counter_down_minute < 0 && !counter_down_hour ) counter_down_minute = 0;
    }
    if( counter_down_second < 0 && !(counter_down_hour + counter_down_minute) ) counter_down_second = 0;
    UpdateSetValue();
}

void UpDownTransfer(){
    if( counter_direction_flg == C_INC ) {
        counter_direction_flg = C_DEC;
        counter_down_hour   = counter_up_hour;
        counter_down_minute = counter_up_minute;
        counter_down_second = counter_up_second;
    }
    else{
        counter_direction_flg = C_INC;
        counter_up_hour   = counter_down_hour;
        counter_up_minute = counter_down_minute;
        counter_up_second = counter_down_second;
    }
    UpdateSetValue();
}

void UpdateSetValue(){
    counter_down_set_hour   = counter_down_hour;
    counter_down_set_minute = counter_down_minute;
    counter_down_set_second = counter_down_second;
}
