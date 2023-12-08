/* 
 * File:   DS1307.h
 *
 * Created: 30/08/2023 
 * Author: Boris
 */
 
#ifndef DS1307_H
#define	DS1307_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include <stdbool.h>

    //Address of DS1307 for I2C
    #define DS1307_READ_ADDR   (0xD1)    // 1101001 R(1) 
    #define DS1307_WRITE_ADDR  (0xD0)    // 1101000 W(0) 
    
    //DS1307 pin define for I2C
    #define DS1307_I2C_DATA       PORTCbits.RC3
    #define DS1307_I2C_CLOCK      PORTCbits.RC4
    #define DS1307_I2C_DATA_TRIS  TRISCbits.TRISC3
    #define DS1307_I2C_CLOCK_TRIS TRISCbits.TRISC4

    //DS1307 data type define
    #define SECOND  0
    #define MINUTE  1
    #define HOUR    2
    #define WEEKDAY 3
    #define DAY     4
    #define MONTH   5
    #define YEAR    6
    
    //days 
    #define SONDAY   1 
    #define MONDAY   2
    #define TUESDAY  3
    #define WENSDAY  4
    #define THURSDAY 5
    #define FRUDAY   6
    #define SATERDAY 7

    //Months
    #define JANUARY   1
    #define FEBRUARY  2
    #define MARCH     3
    #define APRIL     4
    #define MAY       5
    #define JUNE      6
    #define JULY      7
    #define AUGUST    8
    #define SEPTEMBER 9
    #define OCTOBER   10
    #define NOVEMBER  11
    #define DECEMBER  12

    #define ERROR     200

    #define NUM_DS1307_DATA_BYTS 14

    #define RTC_HOUR_TYPE_24   0
    #define RTC_HOUR_TYPE_12   1

    #define RTC_HOUR_TYPE_PM   1
    #define RTC_HOUR_TYPE_AM   0

    #define RTC_DISP_MODE_NUM       3    
    #define RTC_HO_MI_SE_DISP_MODE  0   // hour, minute and second  diplay mode
    #define RTC_HO_MI_WD_DISP_MODE  1   // hour, minute and weekday diplay mode
    #define RTC_YE_MO_DA_DISP_MODE  2   // year, month  and day     diplay mode

//    unsigned char ds1307_date_datas[NUM_DS1307_DATA_BYTS];
    
    bool rtc_clock_disp_type;
    bool rtc_hour_ampm_flg;
    
    unsigned char real_rtc_data[ 7 ];
    
    void Ds1307Init();
    unsigned char* Ds1307Read( unsigned char disp_type );
    void Ds1307Write(unsigned char* write_datas);
    void GetAsciiValue (unsigned char time_bcd_data);
    unsigned char GetDaysSymbol(unsigned char time_bcd_data);
    unsigned char GetMonthsSymbol(unsigned char time_bcd_data);

#ifdef	__cplusplus
}
#endif

#endif	/* DS1307_H */

