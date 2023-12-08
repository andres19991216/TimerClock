#include <xc.h>
#include "Counter.h"
#include "Display.h"
#include "Main.h"

void OpenCounter(){

    counter_down_hour   = 0;
    counter_down_minute = 0;
    counter_down_second = 0;
    
    counter_down_set_hour   = 0;
    counter_down_set_minute = 0;
    counter_down_set_second = 0;
    
    counter_up_hour   = 0;
    counter_up_minute = 0;
    counter_up_second = 0;
    
    counter_enable_flg    = 0;
    counter_skip_flg      = 0;
    counter_direction_flg = C_INC;
    counter_seting_flg    = 0;
    counter_down_comp_flg = 0;
    
    counter_down_comp_cnt = 0;
    
    TRISBbits.TRISB2    = 1;
    INTCON3bits.INT2IP  = 0;
    INTCON3bits.INT2IF  = 0;
    INTCON3bits.INT2IE  = 1;
    INTCON2bits.INTEDG2 = 1;
    
}

unsigned char* ReadCounter(){
    
    unsigned char counter_datas[ NUM_OF_COUNTER_DATA ];
    
    if( counter_direction_flg == C_INC ) {
        counter_datas[ C_HOUR + 1 ]   = counter_up_hour   % 10;
        counter_datas[ C_HOUR ]       = counter_up_hour   / 10;
        counter_datas[ C_MINUTE + 1 ] = counter_up_minute % 10;
        counter_datas[ C_MINUTE ]     = counter_up_minute / 10;
        counter_datas[ C_SECOND + 1 ] = counter_up_second % 10;
        counter_datas[ C_SECOND ]     = counter_up_second / 10;
    }
    
    else if( counter_direction_flg == C_DEC ){
        counter_datas[ C_HOUR + 1 ]   = counter_down_hour   % 10;
        counter_datas[ C_HOUR ]       = counter_down_hour   / 10;
        counter_datas[ C_MINUTE + 1 ] = counter_down_minute % 10;
        counter_datas[ C_MINUTE ]     = counter_down_minute / 10;
        counter_datas[ C_SECOND + 1 ] = counter_down_second % 10;
        counter_datas[ C_SECOND ]     = counter_down_second / 10;
    }
    
    return counter_datas;
}

void SetCounter( unsigned char hour, unsigned char minute, unsigned char second ){
    
    if( counter_direction_flg == C_INC ) return;
    else if( counter_direction_flg == C_DEC ){
        counter_down_hour   = hour;
        counter_down_minute = minute;
        counter_down_second = second;
    }
}

void StartCounter(){
    counter_enable_flg = 1;
}

void StopCounter(){
    counter_enable_flg = 0;
}

void ScanCounter(){
    if(!counter_enable_flg) return;
    if( counter_direction_flg == C_INC ){
        counter_up_second ++;
        if( counter_up_second == 60 ){
            counter_up_second = 0;
            counter_up_minute ++;
            if(counter_up_minute == 60){
                counter_up_minute = 0;
                counter_up_hour ++;
            }
        }
    }
    else if( counter_direction_flg == C_DEC ){
        if( counter_down_hour == 0 && counter_down_minute == 0 && counter_down_second == 0 ) {
            counter_enable_flg = 0; 
            counter_down_comp_flg = 1; counter_down_comp_cnt = 0;
            return;
        }
        counter_down_comp_flg = 0;
        counter_down_second --;
        if( counter_down_second < 0 ) {
            counter_down_second = 59;
            counter_down_minute --; 
            if( counter_down_minute < 0 ){
                counter_down_minute = 59;
                counter_down_hour --; 
            }
        }
    }
}

void CloseCounter(){
    INTCON3bits.INT2IF = 0;
    INTCON3bits.INT2IE = 0;
}



