
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
    
//    M74HC595Init();
    TLC5917Init();
    
    for( unsigned char i = 0; i < NUM_OF_7SEG_COMMON + NUM_OF_LEDS_COMMON; i++ ){
        blink_pos[ i ] = FALSE;
        display_datas[ i ] = 0x00;//0xff;
    }
    display_datas[6] = 0xff;
    //Display Counter init
    display_cnt = 0; blink_cnt = 0; blink_enable_flg = 0;
    BLINK_TIME = DISP_BLINK_TIME;
}

void ScanDisplay(){
    
    unsigned char temp; 
    SEG1 = SEG2 = SEG3 = SEG4 = SEG5 = SEG6 = COLLON = LED = HIGH; 
    display_cnt ++;
    temp = display_datas[ display_cnt - 1 ];
//    if( blink_pos[ display_cnt - 1 ] && blink_cnt < BLINK_TIME / 2 && blink_enable_flg ) return;
    if( blink_pos[ display_cnt - 1 ] && blink_cnt < BLINK_TIME / 2 ) temp = 0x00;
    TLC5917Write(temp);
    switch( display_cnt ){
        case 1: SEG1   = LOW; break;
        case 2: SEG2   = LOW; break;
        case 3: SEG3   = LOW; break;
        case 4: SEG4   = LOW; break;
        case 5: SEG5   = LOW; break;
        case 6: SEG6   = LOW; break;
        case 7: COLLON = LOW; break;
        case 8: LED    = LOW; display_cnt = 0; blink_cnt ++; break;
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
        case 0: return 0x3f; case 1: return 0x06; case 2: return 0x5b; case 3: return 0x4f; case 4: return 0x66;
        case 5: return 0x6d; case 6: return 0x7d; case 7: return 0x07; case 8: return 0x7f; case 9: return 0x6f;
        case 'A': return 0x77; case 'a': return 0x5f; case 'B': return 0x7f; case 'b': return 0x7c;
        case 'C': return 0x39; case 'c': return 0x58; case 'D': return 0x3f; case 'd': return 0x5e;
        case 'E': return 0x79; case 'e': return 0x7b; case 'F': return 0x71; case 'G': return 0x7d; case 'g': return 0x6f;
        case 'H': return 0x76; case 'h': return 0x74; case 'I': return 0x06; case 'J': return 0x0e; case 'L': return 0x38;
        case 'n': return 0x54; case 'O': return 0x3f; case 'o': return 0x5c; case 'P': return 0x73; case 'q': return 0x67;
        case 'r': return 0x50; case 'S': return 0x6d; case 't': return 0x78; case 'U': return 0x3e; case 'y': return 0x6e;
        case 0xff: return 0x00;
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