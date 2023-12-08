#include <xc.h>
#include "Counter.h"

void OpenCounter(){
    
    counter_enable_flg = 0;
    counter_skip_flg = 0;
    
    counter_hour = 0;
    counter_minute = 0;
    counter_second = 0;
    
    set_counter_hour = 0;
    set_counter_minute = 0;
    set_counter_second = 0;
    
    counter_direction_flg = C_INC;
    counter_seting_flg = 0;
    
    TRISBbits.TRISB2 = 1;
    INTCON3bits.INT2IP = 0;
    INTCON3bits.INT2IF = 0;
    INTCON3bits.INT2IE = 1;
    INTCON2bits.INTEDG2 = 1;
    
}

unsigned char* ReadCounter(){
    
    unsigned char counter_datas[ NUM_OF_COUNTER_DATA ];
    
    counter_datas[ C_HOUR + 1 ]   = counter_hour   % 10;
    counter_datas[ C_HOUR ]       = counter_hour   / 10;
    counter_datas[ C_MINUTE + 1 ] = counter_minute % 10;
    counter_datas[ C_MINUTE ]     = counter_minute / 10;
    counter_datas[ C_SECOND + 1 ] = counter_second % 10;
    counter_datas[ C_SECOND ]     = counter_second / 10;
    
    return counter_datas;
    
}

void SetCounter( unsigned char hour, unsigned char minute, unsigned char second ){
    
    
    if( counter_direction_flg == C_INC ) {
        set_counter_hour   = hour;   counter_hour   = 0;
        set_counter_minute = minute; counter_minute = 0;
        set_counter_second = second; counter_second = 0;
    }
    else{
        counter_hour   = hour;
        counter_minute = minute;
        counter_second = second;
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
        if( counter_hour >= set_counter_hour && counter_minute >= set_counter_minute && counter_second >= set_counter_second ) {
            counter_enable_flg = 0;
            return;
        }
        counter_second ++;
        if( counter_second == 60 ){
            counter_second = 0;
            counter_minute ++;
            if(counter_minute == 60){
                counter_minute = 0;
                counter_hour ++;
            }
        }
    }
    else if( counter_direction_flg == C_DEC ){
        if( counter_hour == 0 && counter_minute == 0 && counter_second == 0 ) {
            counter_enable_flg = 0;
            return;
        }
        counter_second --;
        if( counter_second < 0 ) {
            counter_second = 59;
            counter_minute --; 
            if( counter_minute < 0 ){
                counter_minute = 59;
                counter_hour --; 
            }
        }
    }
    
}

void CloseCounter(){
    INTCON3bits.INT2IF = 0;
    INTCON3bits.INT2IE = 0;
}



