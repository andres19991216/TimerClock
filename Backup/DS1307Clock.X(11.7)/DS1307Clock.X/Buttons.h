/* 
 * File:   Buttons.h
 * Author: Boris
 *
 * Created on September 5, 2023, 2:52 AM
 */

#ifndef BUTTONS_H
#define	BUTTONS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    #include <stdbool.h>

    #define HOUR_BUTTON     PORTAbits.RA7
    #define MINUTE_BUTTON   PORTAbits.RA4
    #define SECOND_BUTTON   PORTAbits.RA6
    #define UP_BUTTON       PORTAbits.RA4
    #define DOWN_BUTTON     PORTAbits.RA6

    #define HOUR_BUTTON_TRIS    TRISAbits.TRISA7
    #define MINUTE_BUTTON_TRIS  TRISAbits.TRISA4
    #define SECOND_BUTTON_TRIS  TRISAbits.TRISA6
    
    #define SELECT_BUTTON   PORTBbits.RB4
    #define START_BUTTON    PORTBbits.RB4
    #define STOP_BUTTON     PORTBbits.RB4
    #define RESET_BUTTON    PORTDbits.RD6
    #define LAP_BUTTON      PORTDbits.RD6

    #define SELECT_BUTTON_TRIS  TRISBbits.TRISB4
    #define RESET_BUTTON_TRIS   TRISDbits.TRISD6

    #define BUTTON_PRESSED  0
    #define BUTTON_RELEASED 1

    #define BUTTON_EMPTY_STATE    0
    #define HOUR_BUTTON_PRESSED   11
    #define UP_BUTTON_PRESSED     12
    #define DOWN_BUTTON_PRESSED   13
    #define START_BUTTON_PRESSED  14
    #define RESET_BUTTON_PRESSED  15
    #define HOUR_BUTTON_LONG_PRESSED   16
    #define UP_BUTTON_LONG_PRESSED     17
    #define DOWN_BUTTON_LONG_PRESSED   18
    #define SELECT_BUTTON_LONG_PRESSED 19
    #define RESET_BUTTON_LONG_PRESSED  20

    #define BUTTON_PRESSED_TIME       50
    #define BUTTON_LONG_PRESSED_TIME  3500
    #define BUTTON_RELEASED_TIME      5

    unsigned int hour_btn_pressed_cnt, up_btn_pressed_cnt, down_btn_pressed_cnt, sel_btn_pressed_cnt, reset_btn_pressed_cnt;
    unsigned int hour_btn_released_cnt, up_btn_released_cnt, down_btn_released_cnt, sel_btn_released_cnt, reset_btn_released_cnt;
    
    unsigned char BTN_CLICK_STATE, BTN_CLICK_EVENT;
    
    bool btn_press_verify_flg, btn_long_press_verify_flg, timer_reset_flg;

    void OpenButtons();
    void ScanButtons();
    void VerifyButtons();
    void ActionButtons();

#ifdef	__cplusplus
}
#endif

#endif	/* BUTTONS_H */

