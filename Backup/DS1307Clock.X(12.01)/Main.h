/*
 * main.h
 *
 * Created: 30/08/2023 
 * Author: Boris
 */ 

#ifndef MAIN_H_
#define MAIN_H_

// PIC18F4520 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = INTIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)

#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = OFF      // MCLR Pin Enable bit (RE3 input pin enabled; MCLR disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

volatile union {
    unsigned char SYSTEM_ACTION_FLG;
    struct {										
        unsigned DISP_UPDATE_ENABLE  :1;		
        unsigned COUNTER_READ_ENABLE :1;		
        unsigned DISPLAY_BLINK_ENABEL:1;		
        unsigned fill                :5;		
    };
}SYSTEM_ACTION_FLG;

#define START_INIT_STATE     0

// RTC clock define block
#define RTC_CLOCK_PLAY_STATE  100
#define RTC_CLOCK_SET_STATE   101

// Counter define block
#define COUNTER_PLAY_STATE   200
#define COUNTER_STOP_STATE   201
#define COUNTER_SET_STATE    202
#define COUNTER_COMP_STATE   203

unsigned char SYSTEM_STATE_FLG;

// LED define block
#define TIMER_LED_ON      0x01
#define TIMER_LED_OFF     0xfe
#define CNTDN_LED_ON      0x02
#define CNTDN_LED_OFF     0xfd
#define LAP_RESET_RED_LED_ON        0x04
#define LAP_RESET_RED_LED_OFF       0xfb
#define LAP_RESET_GREEN_LED_ON      0x40
#define LAP_RESET_GREEN_LED_OFF     0xbf
#define START_STOP_RED_LED_ON       0x10
#define START_STOP_RED_LED_OFF      0xef
#define START_STOP_GREEN_LED_ON     0x20
#define START_STOP_GREEN_LED_OFF    0xdf

#define ALL_LED_OFF       0x00
#define ALL_LED_ON        0xff

// ---EEPROM ADDRESS LIST---
// ---RTC CLOCK BLOCK---
#define RTC_SECOND_ADDRESS  0x00
#define RTC_MINUTE_ADDRESS  0x01
#define RTC_HOUR_ADDRESS    0x02
#define RTC_WEEKDAY_ADDRESS 0x03
#define RTC_DAY_ADDRESS     0x04
#define RTC_MONTH_ADDRESS   0x05
#define RTC_YEAR_ADDRESS    0x06
// ---TIMER/COUNTER BLOCK---
#define TIMER_SECOND_ADDRESS  0x10
#define TIMER_MINUTE_ADDRESS  0x11
#define TIMER_HOUR_ADDRESS    0x12

#define SET_TIME_LIMIT    400//1000
#define LED_BLINK_TIME    25

unsigned char counter_hour_set_value, counter_min_set_value, counter_sec_set_value;
unsigned char temp_system_state;

unsigned char rtc_clock_disp_mode;
unsigned char rtc_clock_set_mode;
unsigned char rtc_default_values[10] = {0x10,0x12,0x09,0x05,0x30,0x11,0x23,'\0'};  // for test
unsigned int set_mode_release_cnt;
unsigned int led_blink_cnt;

void DeviceInit();
void LEDProcess();
void DetectStatus();
void InitValue();
#endif /* MAIN_H_ */
