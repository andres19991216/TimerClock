/* 
 * File:   Display.h
 * Author: Boris
 *
 * Created on September 4, 2023, 1:41 AM
 */

#ifndef DISPLAY_H
#define	DISPLAY_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include <stdbool.h>
    
    #define SEG1   LATCbits.LATC2
    #define SEG2   LATCbits.LATC1
    #define SEG3   LATCbits.LATC0
    #define SEG4   LATCbits.LATC5
    #define SEG5   LATBbits.LATB1
    #define SEG6   LATBbits.LATB5
    #define COLLON LATDbits.LATD7
    #define SEG1_TRIS   TRISCbits.TRISC2
    #define SEG2_TRIS   TRISCbits.TRISC1
    #define SEG3_TRIS   TRISCbits.TRISC0
    #define SEG4_TRIS   TRISCbits.TRISC5
    #define SEG5_TRIS   TRISBbits.TRISB1
    #define SEG6_TRIS   TRISBbits.TRISB5
    #define COLLON_TRIS TRISDbits.TRISD7

    #define LED       LATBbits.LATB0
    #define LED_TRIS  TRISBbits.TRISB0

    #define NUM_OF_7SEG_COMMON 7
    #define NUM_OF_LEDS_COMMON 1

    #define DISP_BLINK_TIME  300 //300
    #define DISP_BLINK_TIME1 80 //80

    #define FOUR_SEG_TIME_VALUE 65285//65485
    #define TWO_SEG_TIME_VALUE  65285//65485
    #define LEDS_TIME_VALUE     64085

    unsigned char display_datas[ NUM_OF_7SEG_COMMON + NUM_OF_LEDS_COMMON ];
    bool blink_pos[ NUM_OF_7SEG_COMMON + NUM_OF_LEDS_COMMON ];
    unsigned char blink_enable_flg;
    unsigned char display_cnt, blink_cnt;
    unsigned int BLINK_TIME;
    
    void OpenDisplay();
    void ScanDisplay();
    void UpdateDisplay( unsigned char* update_datas, unsigned char start_position, unsigned char end_position );
    void BlinkDisplay( unsigned char start_pos, unsigned char end_pos );
    void BlinkDisable();
    unsigned char MarkDisplay( unsigned char mark );
    void DecimalTo7SegmentDecode( unsigned long int decimal_data, unsigned char start_pos, unsigned char end_pos, bool zero_mode );

#ifdef	__cplusplus
}
#endif

#endif	/* DISPLAY_H */

