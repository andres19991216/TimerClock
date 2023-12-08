
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
        display_datas[ i ] = 0xff;//0x00;
    }
    //Display Counter init
    display_cnt = 0; blink_cnt = 0; blink_enable_flg = 0;
    BLINK_TIME = DISP_BLINK_TIME;
}

void ScanDisplay(){
    
    SEG1 = SEG2 = SEG3 = SEG4 = SEG5 = SEG6 = COLLON = LED = HIGH; 
    display_cnt ++;
    if( blink_pos[ display_cnt - 1 ] && blink_cnt < DISP_BLINK_TIME / 2 && blink_enable_flg ) return;
    switch( display_cnt ){
        case 1: TLC5917Write(display_datas[0]); SEG1 = LOW; break;
        case 2: TLC5917Write(display_datas[1]); SEG2 = LOW; break;
        case 3: TLC5917Write(display_datas[2]); SEG3 = LOW; break;
        case 4: TLC5917Write(display_datas[3]); SEG4 = LOW; break;
        case 5: TLC5917Write(display_datas[4]); SEG5 = LOW; break;
        case 6: TLC5917Write(display_datas[5]); SEG6 = LOW; break;
        case 7: if( blink_pos[ 6 ] ) TLC5917Write( 0xff ); else TLC5917Write( 0x00 ); COLLON = LOW; break;
        case 8: TLC5917Write(display_datas[7]); LED  = LOW; display_cnt = 0; blink_cnt ++; break;
    }
    if( blink_cnt == DISP_BLINK_TIME ) blink_cnt = 0;
    
//    SEG1 = SEG2 = SEG3 = SEG4 = SEG5 = SEG6 = COLLON = LED = 0;
//    display_cnt ++;
//    if( blink_pos[ display_cnt - 1 ] && blink_cnt < BLINK_TIME / 2 ) return;
//    switch( display_cnt ){
//        case 1: M74HC595Write(display_datas[0]); SEG1 = 1; break;
//        case 2: M74HC595Write(display_datas[1]); SEG2 = 1; break;
//        case 3: M74HC595Write(display_datas[2]); SEG3 = 1; break;
//        case 4: M74HC595Write(display_datas[3]); SEG4 = 1; break;
//        case 5: M74HC595Write(display_datas[4]); SEG5 = 1; break;
//        case 6: M74HC595Write(display_datas[5]); SEG6 = 1; break;
//        case 7: if( blink_pos[ 6 ] ) M74HC595Write(0x00); else M74HC595Write(0xff); COLLON = 1; break;
//        case 8: M74HC595Write(display_datas[7]); LED  = 1; display_cnt = 0; blink_cnt ++; break;
//    }
    if( blink_cnt == BLINK_TIME ) { 
        blink_cnt = 0; 
        if( counter_down_comp_flg ) counter_down_comp_cnt ++;
        if( counter_down_comp_cnt > 5 && counter_down_comp_flg ) { 
            counter_down_comp_flg = 0; blink_enable_flg = 0; counter_down_comp_cnt = 5; 
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
    
    //In case common anode
//    switch( mark ){
//        case 0:   return 0xc0; case 1:   return 0xf9; case 2:   return 0xa4; case 3:   return 0xb0; case 4:   return 0x99;
//        case 5:   return 0x92; case 6:   return 0x82; case 7:   return 0xf8; case 8:   return 0x80; case 9:   return 0x90;
//        case 'A': return 0x88; case 'a': return 0xa0; case 'B': return 0x80; case 'b': return 0x83;
//        case 'C': return 0xc6; case 'c': return 0xa7; case 'D': return 0xc0; case 'd': return 0xa1;
//        case 'E': return 0x86; case 'e': return 0x84; case 'F': return 0x8e; case 'G': return 0x82; case 'g': return 0x90;
//        case 'H': return 0x89; case 'h': return 0x8b; case 'I': return 0xf9; case 'J': return 0xf1; case 'L': return 0xc7;
//        case 'n': return 0xab; case 'O': return 0xc0; case 'o': return 0xa3; case 'P': return 0x8c; case 'q': return 0x98;
//        case 'r': return 0xaf; case 'S': return 0x92; case 't': return 0x87; case 'U': return 0xc1; case 'y': return 0x91;
//    }
    
	//In case common catode 
    switch( mark ){
        case 0: return 0x3f; case 1: return 0x06; case 2: return 0x5b; case 3: return 0x4f; case 4: return 0x66;
        case 5: return 0x6d; case 6: return 0x7d; case 7: return 0x07; case 8: return 0x7f; case 9: return 0x6f;
        case 'A': return 0x77; case 'a': return 0x5f; case 'B': return 0x7f; case 'b': return 0x7c;
        case 'C': return 0x39; case 'c': return 0x58; case 'D': return 0x3f; case 'd': return 0x5e;
        case 'E': return 0x79; case 'e': return 0x7b; case 'F': return 0x71; case 'G': return 0x7d; case 'g': return 0x6f;
        case 'H': return 0x76; case 'h': return 0x74; case 'I': return 0x06; case 'J': return 0x0e; case 'L': return 0x38;
        case 'n': return 0x54; case 'O': return 0x3f; case 'o': return 0x5c; case 'P': return 0x73; case 'q': return 0x67;
        case 'r': return 0x50; case 'S': return 0x6d; case 't': return 0x78; case 'U': return 0x3e; case 'y': return 0x6e;
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