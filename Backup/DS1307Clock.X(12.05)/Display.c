
#include "Display.h"
#include "Config.h"
#include "M74HC595.h"
#include "TLC5917.h"
#include "Counter.h"
#include "Main.h"

void OpenDisplay(){
    
    //Anode Common bits init
    SEG1_TRIS   = DIGITAL_OUTPUT;
    SEG2_TRIS   = DIGITAL_OUTPUT;
    SEG3_TRIS   = DIGITAL_OUTPUT;
    SEG4_TRIS   = DIGITAL_OUTPUT;
    SEG5_TRIS   = DIGITAL_OUTPUT;
    SEG6_TRIS   = DIGITAL_OUTPUT;
    COLLON_TRIS = DIGITAL_OUTPUT;
    LED_TRIS    = DIGITAL_OUTPUT;
    
    SEG1 = SEG2 = SEG3 = SEG4 = SEG5 = SEG6 = COLLON = LED = HIGH;
    
    M74HC595Init();
    
    for( unsigned char i = 0; i < NUM_OF_7SEG_COMMON + NUM_OF_LEDS_COMMON; i++ ){
        blink_pos[ i ] = FALSE;
        display_datas[ i ] = 0xff;//0x00;
    }
    display_datas[6] = 0x00;
    //Display Counter init
    display_cnt = 0; blink_cnt = 0; blink_enable_flg = 0;
    BLINK_TIME = DISP_BLINK_TIME;
}

void ScanDisplay(){
    
    unsigned char temp; 
    SEG1 = SEG2 = SEG3 = SEG4 = SEG5 = SEG6 = COLLON = LED = LOW; 
    display_cnt ++;
    temp = display_datas[ display_cnt - 1 ];

    if( blink_pos[ display_cnt - 1 ] && blink_cnt < BLINK_TIME / 2 ) temp = 0xff;
    M74HC595Write(temp);
    switch( display_cnt ){
        case 1: SEG1   = HIGH; break;
        case 2: SEG2   = HIGH; break;
        case 3: SEG3   = HIGH; break;
        case 4: SEG4   = HIGH; break;
        case 5: SEG5   = HIGH; break;
        case 6: SEG6   = HIGH; break;
        case 7: COLLON = HIGH; break;
        case 8: LED    = HIGH; display_cnt = 0; blink_cnt ++; break;
    }
    
    if( blink_cnt == BLINK_TIME ) { 
        blink_cnt = 0; 
        if( counter_down_comp_flg ) counter_down_comp_cnt ++;
        if( counter_down_comp_cnt > 15 && counter_down_comp_flg ) { 
            counter_down_comp_flg = 0; blink_enable_flg = 0; counter_down_comp_cnt = 12; 
            SYSTEM_STATE_FLG = COUNTER_COMP_STATE;
        }
    }
}

void UpdateDisplay(  unsigned char* update_datas, unsigned char start_position, unsigned char end_position  ){
    
    for( unsigned char i = start_position - 1; i < end_position; i ++ ) {
        display_datas[ i ] = MarkDisplay( update_datas[ i ] ); 
    }
}

unsigned char MarkDisplay( unsigned char mark ){
    
    switch( mark ){
        case 0:   return 0xc0; case 1:   return 0xf9; case 2:   return 0xa4; case 3:   return 0xb0; case 4:   return 0x99;
        case 5:   return 0x92; case 6:   return 0x82; case 7:   return 0xf8; case 8:   return 0x80; case 9:   return 0x90;
        case 'A': return 0x88; case 'a': return 0xa0; case 'B': return 0x80; case 'b': return 0x83;
        case 'C': return 0xc6; case 'c': return 0xa7; case 'D': return 0xc0; case 'd': return 0xa1;
        case 'E': return 0x86; case 'e': return 0x84; case 'F': return 0x8e; case 'G': return 0x82; case 'g': return 0x90;
        case 'H': return 0x89; case 'h': return 0x8b; case 'I': return 0xf9; case 'J': return 0xf1; case 'L': return 0xc7;
        case 'n': return 0xab; case 'O': return 0xc0; case 'o': return 0xa3; case 'P': return 0x8c; case 'q': return 0x98;
        case 'r': return 0xaf; case 'S': return 0x92; case 't': return 0x87; case 'U': return 0xc1; case 'y': return 0x91;
    }
}

void BlinkDisplay( unsigned char start_pos, unsigned char end_pos ){
    
    for( unsigned char i = start_pos; i < end_pos + 1; i ++ ){
        blink_pos[ i ] = TRUE; 
    }
}

void BlinkDisable() {
    for( unsigned char i = 0; i < NUM_OF_7SEG_COMMON; i ++ ){
        blink_pos[ i ] = FALSE; 
    }
}