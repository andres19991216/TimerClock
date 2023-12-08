
#include "Config.h"
#include "Buttons.h"
#include "Main.h"
#include "Counter.h"
#include "Display.h"
#include "DS1307.h"

void OpenButtons(){
    
    HOUR_BUTTON_TRIS   = DIGITAL_INPUT;
    MINUTE_BUTTON_TRIS = DIGITAL_INPUT;
    SECOND_BUTTON_TRIS = DIGITAL_INPUT;
    SELECT_BUTTON_TRIS = DIGITAL_INPUT;
    RESET_BUTTON_TRIS  = DIGITAL_INPUT;
    
    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; BTN_CLICK_EVENT = 0;
    
    hour_btn_pressed_cnt  = 0; up_btn_pressed_cnt  = 0; down_btn_pressed_cnt  = 0; sel_btn_pressed_cnt  = 0; reset_btn_pressed_cnt = 0;
    hour_btn_released_cnt = 0; up_btn_released_cnt = 0; down_btn_released_cnt = 0; sel_btn_released_cnt = 0; reset_btn_released_cnt = 0;
    
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
        case 1: if( hour_btn_pressed_cnt > BUTTON_LONG_PRESSED_TIME ) { btn_long_press_verify_flg = TRUE; hour_btn_pressed_cnt = BUTTON_LONG_PRESSED_TIME; }
                else if( hour_btn_pressed_cnt > BUTTON_PRESSED_TIME ) btn_press_verify_flg = TRUE;
                if( hour_btn_released_cnt > BUTTON_RELEASED_TIME )     { 
                    hour_btn_released_cnt = 0; BTN_CLICK_EVENT = 0; 
                    if( btn_long_press_verify_flg ) { BTN_CLICK_STATE = HOUR_BUTTON_LONG_PRESSED; btn_long_press_verify_flg = FALSE; } 
                    else if( btn_press_verify_flg ) { BTN_CLICK_STATE = HOUR_BUTTON_PRESSED;      btn_press_verify_flg      = FALSE; }
                } 
                ActionButtons(); break;
        // Minute(Up) button process block        
        case 2: if( up_btn_pressed_cnt > BUTTON_LONG_PRESSED_TIME ) { btn_long_press_verify_flg = TRUE; up_btn_pressed_cnt = BUTTON_LONG_PRESSED_TIME; }
                else if( up_btn_pressed_cnt > BUTTON_PRESSED_TIME ) btn_press_verify_flg = TRUE;
                if( up_btn_released_cnt > BUTTON_RELEASED_TIME )     { 
                    up_btn_released_cnt = 0; BTN_CLICK_EVENT = 0; 
                    if( btn_long_press_verify_flg ) { BTN_CLICK_STATE = UP_BUTTON_LONG_PRESSED; btn_long_press_verify_flg = FALSE; } 
                    else if( btn_press_verify_flg ) { BTN_CLICK_STATE = UP_BUTTON_PRESSED;      btn_press_verify_flg      = FALSE; }
                } 
                ActionButtons(); break;
        // Second(Dn) button process block
        case 3: if( down_btn_pressed_cnt > BUTTON_LONG_PRESSED_TIME ) { btn_long_press_verify_flg = TRUE; down_btn_pressed_cnt = BUTTON_LONG_PRESSED_TIME; }
                else if( down_btn_pressed_cnt > BUTTON_PRESSED_TIME ) btn_press_verify_flg = TRUE;
                if( down_btn_released_cnt > BUTTON_RELEASED_TIME )     { 
                    down_btn_released_cnt = 0; BTN_CLICK_EVENT = 0; 
                    if( btn_long_press_verify_flg ) { BTN_CLICK_STATE = DOWN_BUTTON_LONG_PRESSED; btn_long_press_verify_flg = FALSE; } 
                    else if( btn_press_verify_flg ) { BTN_CLICK_STATE = DOWN_BUTTON_PRESSED;      btn_press_verify_flg      = FALSE; }
                } 
                ActionButtons(); break;
        // Select(Start/Stop) button process block
        case 4: if( sel_btn_pressed_cnt > BUTTON_LONG_PRESSED_TIME ) { btn_long_press_verify_flg = TRUE; sel_btn_pressed_cnt = BUTTON_LONG_PRESSED_TIME; }
                else if( sel_btn_pressed_cnt > BUTTON_PRESSED_TIME ) btn_press_verify_flg = TRUE;
                if( sel_btn_released_cnt > BUTTON_RELEASED_TIME )     { 
                    sel_btn_released_cnt = 0; BTN_CLICK_EVENT = 0; 
                    if( btn_long_press_verify_flg ) { BTN_CLICK_STATE = SELECT_BUTTON_LONG_PRESSED; btn_long_press_verify_flg = FALSE; } 
                    else if( btn_press_verify_flg ) { BTN_CLICK_STATE = START_BUTTON_PRESSED;       btn_press_verify_flg      = FALSE; }
                } 
                ActionButtons(); break;
        // Reset(Lap) button process block        
        case 5: if( reset_btn_pressed_cnt > BUTTON_LONG_PRESSED_TIME ) { btn_long_press_verify_flg = TRUE; reset_btn_pressed_cnt = BUTTON_LONG_PRESSED_TIME; }
                else if( reset_btn_pressed_cnt > BUTTON_PRESSED_TIME ) btn_press_verify_flg = TRUE;
                if( reset_btn_released_cnt > BUTTON_RELEASED_TIME )     { 
                    reset_btn_released_cnt = 0; BTN_CLICK_EVENT = 0; 
                    if( btn_long_press_verify_flg ) { BTN_CLICK_STATE = RESET_BUTTON_LONG_PRESSED; btn_long_press_verify_flg = FALSE; } 
                    else if( btn_press_verify_flg ) { BTN_CLICK_STATE = RESET_BUTTON_PRESSED;      btn_press_verify_flg      = FALSE; }
                } 
                ActionButtons(); break;
    }
}

void ActionButtons(){
    
    // Click select button with long time, in this case system entry RTC_TIME_PLAY_STATE or COUNTER_PLAY_STATE
    
    //-----------------------------------------------RTC clock Process---------------------------------------------------------------
    // RTC Time play state
    signed char temp;
    switch( SYSTEM_STATE_FLG ){
        
        case RTC_CLOCK_PLAY_STATE:      //RTC clock play state
            switch( BTN_CLICK_STATE ) {
                case SELECT_BUTTON_LONG_PRESSED:  //Change from rtc clock to counter state
                    SYSTEM_STATE_FLG = COUNTER_STOP_STATE; BTN_CLICK_STATE = BUTTON_EMPTY_STATE; 
                    counter_direction_flg = C_INC; blink_enable_flg = 0; temp_system_state = SYSTEM_STATE_FLG;
                    SetCounter( counter_hour_set_value, counter_min_set_value, counter_sec_set_value ); StopCounter(); 
                    return;
                case START_BUTTON_PRESSED:  // Change the clock display (24/12)
                    if( rtc_clock_disp_type == RTC_HOUR_TYPE_24 ) rtc_clock_disp_type = RTC_HOUR_TYPE_12;
                    else                                          rtc_clock_disp_type = RTC_HOUR_TYPE_24;
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; Ds1307Write( &real_rtc_data ); 
                    return;
                case RESET_BUTTON_PRESSED: // Change the clock display mode
                    rtc_clock_disp_mode ++;
                    if( rtc_clock_disp_mode >= RTC_DISP_MODE_NUM ) rtc_clock_disp_mode = RTC_HO_MI_SE_DISP_MODE;
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; 
                    return;
                case HOUR_BUTTON_LONG_PRESSED: 
                    SYSTEM_STATE_FLG = RTC_CLOCK_YEAR_SET_STATE; rtc_clock_set_mode = RTC_CLCOK_YE_MO_DA_SET_MOD;
                    blink_enable_flg = 2; BTN_CLICK_STATE = BUTTON_EMPTY_STATE; 
                    return;
                case UP_BUTTON_LONG_PRESSED: 
                    SYSTEM_STATE_FLG = RTC_CLOCK_HOUR_SET_STATE; rtc_clock_set_mode = RTC_CLCOK_HO_MI_SE_SET_MOD;
                    blink_enable_flg = 2; BTN_CLICK_STATE = BUTTON_EMPTY_STATE; 
                    return;
                case DOWN_BUTTON_LONG_PRESSED:   
                    SYSTEM_STATE_FLG = RTC_CLOCK_WDAY_SET_STATE; rtc_clock_set_mode = RTC_CLCOK_WDAY_AMPM_SET_MOD;
                    blink_enable_flg = 2; BTN_CLICK_STATE = BUTTON_EMPTY_STATE; 
                    return;
            }
            break;
            
        case RTC_CLOCK_YEAR_SET_STATE: 
            switch( BTN_CLICK_STATE ) {
                case UP_BUTTON_PRESSED:  //Increase RTC year value
                    temp = real_rtc_data[ YEAR ]; temp ++;
                    if( temp % 16 >= 10 ) temp += 6; if( temp > 0x99 ) temp = 23; 
                    real_rtc_data[ YEAR ] = temp;
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    return;
                case DOWN_BUTTON_PRESSED:  //Decrease RTC year value
                    temp = real_rtc_data[ YEAR ]; temp --;
                    if( temp == 0x22 ) temp = 0x23; 
                    else if( temp % 16 >= 10 ) temp -= 6;
                    real_rtc_data[ YEAR ] = temp;
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    return;
                case HOUR_BUTTON_PRESSED: 
                    SYSTEM_STATE_FLG = RTC_CLOCK_MONTH_SET_STATE; blink_enable_flg = 3; BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    return;
                case RESET_BUTTON_LONG_PRESSED: 
                    SYSTEM_STATE_FLG = RTC_CLOCK_PLAY_STATE; blink_enable_flg = 1; BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    Ds1307Write( &real_rtc_data ); return;
            }
            break;
        case RTC_CLOCK_MONTH_SET_STATE:
            switch( BTN_CLICK_STATE ) {
                case UP_BUTTON_PRESSED:  //Increase RTC year value
                    temp = real_rtc_data[ MONTH ]; temp ++;
                    if( temp % 16 >= 10 ) temp += 6; if( temp > 0x12 ) temp = 1; 
                    real_rtc_data[ MONTH ] = temp;
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    return;
                case DOWN_BUTTON_PRESSED:  //Decrease RTC year value
                    temp = real_rtc_data[ MONTH ]; temp --;
                    if( temp == 0x00 ) temp = 0x12;
                    else if( temp % 16 >= 10 ) temp -= 6;
                    real_rtc_data[ MONTH ] = temp;
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    return;
                case HOUR_BUTTON_PRESSED: 
                    SYSTEM_STATE_FLG = RTC_CLOCK_DAY_SET_STATE; blink_enable_flg = 4; BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    return;
                case RESET_BUTTON_LONG_PRESSED: 
                    SYSTEM_STATE_FLG = RTC_CLOCK_PLAY_STATE; blink_enable_flg = 1; BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    Ds1307Write( &real_rtc_data ); return;
            }
        case RTC_CLOCK_DAY_SET_STATE: 
            switch( BTN_CLICK_STATE ) {
                case UP_BUTTON_PRESSED:  //Increase RTC year value
                    temp = real_rtc_data[ DAY ]; temp ++;
                    if( temp % 16 >= 10 ) temp += 6; if( temp > 0x31 ) temp = 1; 
                    real_rtc_data[ DAY ] = temp;
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    return;
                case DOWN_BUTTON_PRESSED:  //Decrease RTC year value
                    temp = real_rtc_data[ DAY ]; temp --;
                    if( temp == 0x00 ) temp = 0x31;
                    else if( temp % 16 >= 10 ) temp -= 6;
                    real_rtc_data[ DAY ] = temp;
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    return;
                case HOUR_BUTTON_PRESSED: 
                    SYSTEM_STATE_FLG = RTC_CLOCK_YEAR_SET_STATE; blink_enable_flg = 2; BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    return;
                case RESET_BUTTON_LONG_PRESSED: 
                    SYSTEM_STATE_FLG = RTC_CLOCK_PLAY_STATE; blink_enable_flg = 1; BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    Ds1307Write( &real_rtc_data ); return;
            }
            
        case RTC_CLOCK_HOUR_SET_STATE: 
            switch( BTN_CLICK_STATE ) {
                case UP_BUTTON_PRESSED:  //Increase RTC hour value
                    temp = real_rtc_data[ YEAR ]; temp ++;
                    if( temp % 16 >= 10 ) temp += 6; if( temp > 0x24 ) temp = 1; 
                    real_rtc_data[ YEAR ] = temp;
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    return;
                case DOWN_BUTTON_PRESSED:  //Decrease RTC hour value
                    temp = real_rtc_data[ YEAR ]; temp --;
                    if( temp == 0x00 ) temp = 0x24;
                    else if( temp % 16 >= 10 ) temp -= 6;
                    real_rtc_data[ YEAR ] = temp;
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    return;
                case HOUR_BUTTON_PRESSED: 
                    SYSTEM_STATE_FLG = RTC_CLOCK_MINUTE_SET_STATE; blink_enable_flg = 3; BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    return;
                case RESET_BUTTON_LONG_PRESSED: 
                    SYSTEM_STATE_FLG = RTC_CLOCK_PLAY_STATE; blink_enable_flg = 1; BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    Ds1307Write( &real_rtc_data ); return;
            }
            break;
        case RTC_CLOCK_MINUTE_SET_STATE:
            switch( BTN_CLICK_STATE ) {
                case UP_BUTTON_PRESSED:  //Increase RTC minute value
                    temp = real_rtc_data[ MONTH ]; temp ++;
                    if( temp % 16 >= 10 ) temp += 6; if( temp > 0x12 ) temp = 1; 
                    real_rtc_data[ MONTH ] = temp;
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    return;
                case DOWN_BUTTON_PRESSED:  //Decrease RTC minute value
                    temp = real_rtc_data[ MONTH ]; temp --;
                    if( temp == 0x00 ) temp = 0x12; 
                    else if( temp % 16 >= 10 ) temp -= 6;
                    real_rtc_data[ MONTH ] = temp;
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    return;
                case HOUR_BUTTON_PRESSED: 
                    SYSTEM_STATE_FLG = RTC_CLOCK_SECOND_SET_STATE; blink_enable_flg = 4; BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    return;
                case RESET_BUTTON_LONG_PRESSED: 
                    SYSTEM_STATE_FLG = RTC_CLOCK_PLAY_STATE; blink_enable_flg = 1; BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    Ds1307Write( &real_rtc_data ); return;
            }
        case RTC_CLOCK_SECOND_SET_STATE: 
            switch( BTN_CLICK_STATE ) {
                case UP_BUTTON_PRESSED:  //Increase RTC second value
                    temp = real_rtc_data[ SECOND ]; temp ++;
                    if( temp % 16 >= 10 ) temp += 6; if( temp > 0x59 ) temp = 0; 
                    real_rtc_data[ SECOND ] = temp;
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    return;
                case DOWN_BUTTON_PRESSED:  //Decrease RTC second value
                    temp = real_rtc_data[ SECOND ]; temp --;
                    if( temp < 0x00 ) temp = 0x59; 
                    else if( temp % 16 >= 10 ) temp -= 6;
                    real_rtc_data[ SECOND ] = temp;
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    return;
                case HOUR_BUTTON_PRESSED: 
                    SYSTEM_STATE_FLG = RTC_CLOCK_HOUR_SET_STATE; blink_enable_flg = 2; BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    return;
                case RESET_BUTTON_LONG_PRESSED: 
                    SYSTEM_STATE_FLG = RTC_CLOCK_PLAY_STATE; blink_enable_flg = 1; BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    Ds1307Write( &real_rtc_data ); return;
            }
            
        case RTC_CLOCK_WDAY_SET_STATE: 
            switch( BTN_CLICK_STATE ) {
                case UP_BUTTON_PRESSED:  //Increase RTC year value
                    temp = real_rtc_data[ WEEKDAY ]; temp ++;
                    if( temp > 0x07 ) temp = 1; 
                    real_rtc_data[ WEEKDAY ] = temp;
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    return;
                case DOWN_BUTTON_PRESSED:  //Decrease RTC year value
                    temp = real_rtc_data[ WEEKDAY ]; temp --;
                    if( temp == 0 ) temp = 7; 
                    real_rtc_data[ WEEKDAY ] = temp;
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    return;
                case HOUR_BUTTON_PRESSED: 
                    SYSTEM_STATE_FLG = RTC_CLOCK_AMPM_SET_STATE; blink_enable_flg = 3; BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    return;
                case RESET_BUTTON_LONG_PRESSED: 
                    SYSTEM_STATE_FLG = RTC_CLOCK_PLAY_STATE; blink_enable_flg = 1; BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    Ds1307Write( &real_rtc_data ); return;
            }
            break;
        case RTC_CLOCK_AMPM_SET_STATE:
            switch( BTN_CLICK_STATE ) {
                case UP_BUTTON_PRESSED:  //Increase RTC year value
                    rtc_hour_ampm_flg = RTC_HOUR_TYPE_AM;
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    return;
                case DOWN_BUTTON_PRESSED:  //Decrease RTC year value
                    rtc_hour_ampm_flg = RTC_HOUR_TYPE_PM;
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    return;
                case HOUR_BUTTON_PRESSED: 
                    SYSTEM_STATE_FLG = RTC_CLOCK_WDAY_SET_STATE; blink_enable_flg = 2; BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    return;
                case RESET_BUTTON_LONG_PRESSED: 
                    SYSTEM_STATE_FLG = RTC_CLOCK_PLAY_STATE; blink_enable_flg = 1; BTN_CLICK_STATE = BUTTON_EMPTY_STATE;
                    Ds1307Write( &real_rtc_data ); return;
            }
            
        case COUNTER_PLAY_STATE:          //Counter play state
            switch( BTN_CLICK_STATE ) {
                case SELECT_BUTTON_LONG_PRESSED: //Change from counter to rtc clock state
                    SYSTEM_STATE_FLG = RTC_CLOCK_PLAY_STATE; blink_enable_flg = 1; 
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; StopCounter(); 
                    return;
                case START_BUTTON_PRESSED:  //Counter start and stop
                    counter_skip_flg = 0;
                    if( !counter_enable_flg ) { SYSTEM_STATE_FLG = COUNTER_PLAY_STATE; StartCounter(); blink_enable_flg = 1; }
                    else                      { SYSTEM_STATE_FLG = COUNTER_STOP_STATE; StopCounter();  blink_enable_flg = 0; }
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; temp_system_state = SYSTEM_STATE_FLG; 
                    return;
                case RESET_BUTTON_LONG_PRESSED: //Change the counter direction (up/down)
                    counter_direction_flg = 1 - counter_direction_flg; 
                    SetCounter( counter_hour_set_value, counter_min_set_value, counter_sec_set_value ); StopCounter(); blink_enable_flg = 0;
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; 
                    return;
                case RESET_BUTTON_PRESSED:      //Skip the counter display
                    if( !counter_enable_flg ) return;
                    counter_skip_flg = 1 - counter_skip_flg; 
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; 
                    return;
            }
            break;
          
        case COUNTER_STOP_STATE:         //Counter stop state
            if( BTN_CLICK_STATE == START_BUTTON_PRESSED ) {
                counter_skip_flg = 0;
                if( !counter_enable_flg ) { SYSTEM_STATE_FLG = COUNTER_PLAY_STATE; StartCounter(); blink_enable_flg = 1; } // frome stop to start
                else                      { SYSTEM_STATE_FLG = COUNTER_STOP_STATE; StopCounter();  blink_enable_flg = 0; } // frome start to stop
                BTN_CLICK_STATE = BUTTON_EMPTY_STATE; temp_system_state = SYSTEM_STATE_FLG; 
                return;
            }
            
        case COUNTER_HOUER_SET_STATE:   //Hour set state
            switch( BTN_CLICK_STATE ) {
                case UP_BUTTON_PRESSED: // Increase value of hours
                    blink_enable_flg = 2; counter_hour_set_value ++;   
                    SetCounter( counter_hour_set_value, counter_min_set_value, counter_sec_set_value );
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; 
                    return;
                case DOWN_BUTTON_PRESSED:  // Decrease value of hours
                    blink_enable_flg = 2; if(counter_hour_set_value>0) counter_hour_set_value --;   
                    SetCounter( counter_hour_set_value, counter_min_set_value, counter_sec_set_value );
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; 
                    return;
            }
            break; 
            
            
        case COUNTER_MINUTE_SET_STATE:   //Minute set state
            switch( BTN_CLICK_STATE ) {
                case UP_BUTTON_PRESSED: // Increase value of hours
                    blink_enable_flg = 3; if(counter_min_set_value<59) counter_min_set_value ++;   
                    SetCounter( counter_hour_set_value, counter_min_set_value, counter_sec_set_value );
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; 
                    return;
                case DOWN_BUTTON_PRESSED:  // Decrease value of hours
                    blink_enable_flg = 3; if(counter_min_set_value>0) counter_min_set_value --;   
                    SetCounter( counter_hour_set_value, counter_min_set_value, counter_sec_set_value );
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; 
                    return;
            }
            break;
            
            
        case COUNTER_SECOND_SET_STATE:   //Second set state
            switch( BTN_CLICK_STATE ) {
                case UP_BUTTON_PRESSED: // Increase value of hours
                    blink_enable_flg = 4; if(counter_sec_set_value<59) counter_sec_set_value ++;   
                    SetCounter( counter_hour_set_value, counter_min_set_value, counter_sec_set_value );
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; 
                    return;
                case DOWN_BUTTON_PRESSED:  // Decrease value of hours
                    blink_enable_flg = 4; if(counter_sec_set_value>0) counter_sec_set_value --;   
                    SetCounter( counter_hour_set_value, counter_min_set_value, counter_sec_set_value );
                    BTN_CLICK_STATE = BUTTON_EMPTY_STATE; 
                    return;
            }
            break;
    }
    
    //---------------------------------COUNTER SETING BLOCK---------------------------------------
    // Set the hour value
    if( SYSTEM_STATE_FLG >= COUNTER_STOP_STATE && BTN_CLICK_STATE == HOUR_BUTTON_LONG_PRESSED ){
        blink_enable_flg = 2; SYSTEM_STATE_FLG = COUNTER_HOUER_SET_STATE; 
        //SetCounter( counter_hour_set_value, counter_min_set_value, counter_sec_set_value );
        BTN_CLICK_STATE = BUTTON_EMPTY_STATE; return;
    }
    
    // Set the minute value
    if( SYSTEM_STATE_FLG >= COUNTER_STOP_STATE && BTN_CLICK_STATE == UP_BUTTON_LONG_PRESSED ){
        blink_enable_flg = 3; SYSTEM_STATE_FLG = COUNTER_MINUTE_SET_STATE; 
        //SetCounter( counter_hour_set_value, counter_min_set_value, counter_sec_set_value );
        BTN_CLICK_STATE = BUTTON_EMPTY_STATE; return;
    }
    
    // Set the second value
    if( SYSTEM_STATE_FLG >= COUNTER_STOP_STATE && BTN_CLICK_STATE == DOWN_BUTTON_LONG_PRESSED ){
        blink_enable_flg = 4; SYSTEM_STATE_FLG = COUNTER_SECOND_SET_STATE;  
        //SetCounter( counter_hour_set_value, counter_min_set_value, counter_sec_set_value );
        BTN_CLICK_STATE = BUTTON_EMPTY_STATE; return;
    }
    
    // Exit the counter set state
    if( SYSTEM_STATE_FLG >= COUNTER_HOUER_SET_STATE && BTN_CLICK_STATE == RESET_BUTTON_LONG_PRESSED ){
        blink_enable_flg = 1; SYSTEM_STATE_FLG = temp_system_state;  
        //SetCounter( counter_hour_set_value, counter_min_set_value, counter_sec_set_value );
        BTN_CLICK_STATE = BUTTON_EMPTY_STATE; return;
    }
    //-------------------------------------------------------------------------------------------------
     
}