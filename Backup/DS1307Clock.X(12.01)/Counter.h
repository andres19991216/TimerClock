/* 
 * File:   Counter.h
 * Author: Boris
 *
 * Created on September 3, 2023, 8:39 AM
 */

#ifndef COUNTER_H
#define	COUNTER_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    #include <stdbool.h>
    
    #define COUNTER_CLOCK      PORTBbits.RB2
    #define COUNTER_CLOCK_TRIS TRISBbits.TRISB2
    
    #define NUM_OF_COUNTER_DATA 6 
    
    #define C_HOUR   0
    #define C_MINUTE 2
    #define C_SECOND 4

    #define C_INC 1
    #define C_DEC 0

    signed char counter_down_hour;
    signed char counter_down_minute;
    signed char counter_down_second;
    
    unsigned char counter_down_set_hour;
    unsigned char counter_down_set_minute;
    unsigned char counter_down_set_second;
    
    unsigned char counter_up_hour;
    unsigned char counter_up_minute;
    unsigned char counter_up_second;
    
    bool counter_enable_flg;
    bool counter_direction_flg;
    bool counter_skip_flg;
    bool counter_seting_flg;
    bool counter_down_comp_flg;
    
    unsigned char counter_down_comp_cnt;

    void OpenCounter();
    unsigned char* ReadCounter();
    void SetCounter( unsigned char hour, unsigned char minute, unsigned char second );
    void StartCounter();
    void ScanCounter();
    void StopCounter();
    void CloseCounter();

#ifdef	__cplusplus
}
#endif

#endif	/* COUNTER_H */

