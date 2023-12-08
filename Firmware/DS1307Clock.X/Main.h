/*
 * main.h
 *
 * Created: 30/08/2023 
 * Author: Boris
 */ 

#ifndef MAIN_H_
#define MAIN_H_

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
#define RTC_CLOCK_YEAR_SET_STATE    101
#define RTC_CLOCK_MONTH_SET_STATE   102
#define RTC_CLOCK_DAY_SET_STATE     103
#define RTC_CLOCK_WDAY_SET_STATE    104
#define RTC_CLOCK_HOUR_SET_STATE    105
#define RTC_CLOCK_MINUTE_SET_STATE  106
#define RTC_CLOCK_SECOND_SET_STATE  107
#define RTC_CLOCK_AMPM_SET_STATE    108

#define RTC_CLCOK_YE_MO_DA_SET_MOD   0
#define RTC_CLCOK_HO_MI_SE_SET_MOD   1
#define RTC_CLCOK_WDAY_AMPM_SET_MOD  2

// Counter define block
#define COUNTER_PLAY_STATE   200
#define COUNTER_STOP_STATE   201
#define COUNTER_HOUER_SET_STATE    202
#define COUNTER_MINUTE_SET_STATE   203
#define COUNTER_SECOND_SET_STATE   204

unsigned char SYSTEM_STATE_FLG;

// LED define block
#define TIMER_LED_ON      0xfe
#define TIMER_LED_OFF     0x01
#define CNTDN_LED_ON      0xfd
#define CNTDN_LED_OFF     0x02
#define LAP_LED_ON        0xfb
#define LAP_LED_OFF       0x04
#define RESET_LED_ON      0xbf
#define RESET_LED_OFF     0x40
#define SELECT_LED_ON     0xef
#define SELECT_LED_OFF    0x10
#define START_LED_ON      0xdf
#define START_LED_OFF     0x20

#define ALL_LED_OFF       0xff
#define ALL_LED_ON        0x00

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
//#define COUNTER_SECOND_ADDRESS  0x20
//#define COUNTER_MINUTE_ADDRESS  0x21
//#define COUNTER_HOUR_ADDRESS    0x22

unsigned char counter_hour_set_value, counter_min_set_value, counter_sec_set_value;
unsigned char temp_system_state;

unsigned char rtc_clock_disp_mode;
unsigned char rtc_clock_set_mode;
unsigned char rtc_default_values[10] = {0x10,0x12,0x09,0x05,0x03,0x11,0x23,'\0'};  // for test

void DeviceInit();
void ClockSetting( unsigned char set_mod );
void CounterSetting();
void LEDProcess();
void DetectStatus();
#endif /* MAIN_H_ */
