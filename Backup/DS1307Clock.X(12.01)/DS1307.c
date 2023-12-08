/*
 * File:   DS1307.c
  *
 * Created: 30/08/2023 
 * Author: Thao Tran
 */
#include "DS1307.h"
#include "I2C.h"
#include "Config.h"

void Ds1307Init(){
    
//    DS1307_I2C_DATA_TRIS  = DIGITAL_INPUT;
//    DS1307_I2C_CLOCK_TRIS = DIGITAL_INPUT;
    rtc_clock_disp_type = RTC_HOUR_TYPE_12;
    rtc_hour_ampm_flg = RTC_HOUR_TYPE_PM;
    I2cInit();

}

unsigned char* Ds1307Read( unsigned char disp_type ){
    
    I2cStart();
    I2cWrite( DS1307_WRITE_ADDR );
    I2cWrite( 0x00 );            
    I2cRestart();
    I2cWrite( DS1307_READ_ADDR );
    unsigned char ds1307_date_datas[ 6 ];
    for( unsigned char i = 0; i < 6; i++ ) real_rtc_data[ i ] = I2cRead( 0 ); real_rtc_data[ 6 ] = I2cRead( 1 );
    rtc_clock_disp_type = ( real_rtc_data[ HOUR ] & 0x40 ) ? 1 : 0; 
    if( rtc_clock_disp_type == RTC_HOUR_TYPE_24 ) real_rtc_data[ HOUR ] = real_rtc_data[ HOUR ] & 0x3f;
    else { rtc_hour_ampm_flg = ( real_rtc_data[ HOUR ] & 0x20 ) ? 1 : 0; real_rtc_data[ HOUR ] = real_rtc_data[ HOUR ] & 0x1f; }
    switch( disp_type ) {
        case RTC_HO_MI_SE_DISP_MODE: 
                ds1307_date_datas[ 0 ] = real_rtc_data[ HOUR ]   >> 4; ds1307_date_datas[ 1 ] = real_rtc_data[ HOUR ]   % 16; 
                ds1307_date_datas[ 2 ] = real_rtc_data[ MINUTE ] >> 4; ds1307_date_datas[ 3 ] = real_rtc_data[ MINUTE ] % 16;
                ds1307_date_datas[ 4 ] = real_rtc_data[ SECOND ] >> 4; ds1307_date_datas[ 5 ] = real_rtc_data[ SECOND ] % 16;
                break;
        case RTC_HO_MI_WD_DISP_MODE: 
                ds1307_date_datas[ 0 ] = real_rtc_data[ HOUR ]    >> 4; ds1307_date_datas[ 1 ] = real_rtc_data[ HOUR ]    % 16; 
                ds1307_date_datas[ 2 ] = real_rtc_data[ MINUTE ]  >> 4; ds1307_date_datas[ 3 ] = real_rtc_data[ MINUTE ]  % 16;
                ds1307_date_datas[ 4 ] = real_rtc_data[ WEEKDAY ] >> 4; ds1307_date_datas[ 5 ] = real_rtc_data[ WEEKDAY ] % 16;
                break;
        case RTC_YE_MO_DA_DISP_MODE: 
                ds1307_date_datas[ 0 ] = real_rtc_data[ YEAR ]  >> 4; ds1307_date_datas[ 1 ] = real_rtc_data[ YEAR ]  % 16; 
                ds1307_date_datas[ 2 ] = real_rtc_data[ MONTH ] >> 4; ds1307_date_datas[ 3 ] = real_rtc_data[ MONTH ] % 16;
                ds1307_date_datas[ 4 ] = real_rtc_data[ DAY ]   >> 4; ds1307_date_datas[ 5 ] = real_rtc_data[ DAY ]   % 16;
                break;
        default: break;
    }
    I2cStop();
    return ds1307_date_datas;
}

unsigned char* Ds1307HMSRead(){
    
    I2cStart();
    I2cWrite( DS1307_WRITE_ADDR );
    I2cWrite( 0x00 );            
    I2cRestart();
    I2cWrite( DS1307_READ_ADDR );
    unsigned char ds1307_date_datas[ 3 ];
    real_rtc_data[SECOND] = I2cRead( 0 );
    real_rtc_data[MINUTE] = I2cRead( 0 );
    real_rtc_data[HOUR]   = I2cRead( 1 );
    I2cStop();
    rtc_clock_disp_type = ( real_rtc_data[ HOUR ] & 0x40 ) ? 1 : 0; 
    if( rtc_clock_disp_type == RTC_HOUR_TYPE_24 ) real_rtc_data[ HOUR ] = real_rtc_data[ HOUR ] & 0x3f;
    else { rtc_hour_ampm_flg = ( real_rtc_data[ HOUR ] & 0x20 ) ? 1 : 0; real_rtc_data[ HOUR ] = real_rtc_data[ HOUR ] & 0x1f; }
    ds1307_date_datas[ 0 ] = real_rtc_data[ HOUR ]   >> 4; ds1307_date_datas[ 1 ] = real_rtc_data[ HOUR ]   % 16; 
    ds1307_date_datas[ 2 ] = real_rtc_data[ MINUTE ] >> 4; ds1307_date_datas[ 3 ] = real_rtc_data[ MINUTE ] % 16;
    ds1307_date_datas[ 4 ] = real_rtc_data[ SECOND ] >> 4; ds1307_date_datas[ 5 ] = real_rtc_data[ SECOND ] % 16;
    if( rtc_clock_disp_type == RTC_HOUR_TYPE_12 && ds1307_date_datas[ 0 ] == 0 ) ds1307_date_datas[ 0 ] = 0xff;
    return ds1307_date_datas;
}

void Ds1307Write(unsigned char* write_datas){
    
    unsigned char hour_temp = write_datas[ 2 ];
    if( rtc_clock_disp_type == RTC_HOUR_TYPE_12 ) { 
        if( hour_temp > 0x12 ) hour_temp = hour_temp - 0x12;
        if( hour_temp % 16 >= 10 ) hour_temp -= 6;
        hour_temp = hour_temp | 0x40;
        if( rtc_hour_ampm_flg == RTC_HOUR_TYPE_PM ) hour_temp = hour_temp | 0x20;
    } 
    else {
        if( rtc_hour_ampm_flg == RTC_HOUR_TYPE_PM && hour_temp < 0x12 ) hour_temp = hour_temp + 0x12;
        if( hour_temp % 16 >= 10 ) hour_temp += 6;
    }
    write_datas[ 2 ] = hour_temp;
    I2cStart();
    I2cWrite( DS1307_WRITE_ADDR );
    I2cWrite( 0x00 );
    I2cWrite( write_datas[ 0 ] );  //__delay_us(1);
    I2cWrite( write_datas[ 1 ] );  //__delay_us(1);
    I2cWrite( write_datas[ 2 ] );  //__delay_us(1);
    I2cWrite( write_datas[ 3 ] );  //__delay_us(1);
    I2cWrite( write_datas[ 4 ] );  //__delay_us(1);
    I2cWrite( write_datas[ 5 ] );  //__delay_us(1);
    I2cWrite( write_datas[ 6 ] );  //__delay_us(1);
    I2cWrite( 0x90 );              //__delay_us(1);
    I2cStop();
}

void Ds1307HourSet(unsigned char hour_data){
    unsigned char hour_temp = hour_data;
    if( rtc_clock_disp_type == RTC_HOUR_TYPE_12 ) { 
        if( hour_temp > 0x12 ) hour_temp = hour_temp - 0x12;
        if( hour_temp % 16 >= 10 ) hour_temp -= 6;
        hour_temp = hour_temp | 0x40;
        if( rtc_hour_ampm_flg == RTC_HOUR_TYPE_PM ) hour_temp = hour_temp | 0x20;
    } 
    else {
        if( rtc_hour_ampm_flg == RTC_HOUR_TYPE_PM && hour_temp < 0x12 ) {
            hour_temp = hour_temp + 0x12;
            if( hour_temp % 16 >= 10 ) hour_temp += 6;
        }
    }
    I2cStart();
    I2cWrite( DS1307_WRITE_ADDR );
    I2cWrite( 2 );
    I2cWrite( hour_temp );
    I2cStop();
}

void Ds1307MinuteSet(unsigned char minute_data){
    I2cStart();
    I2cWrite( DS1307_WRITE_ADDR );
    I2cWrite( 1 );
    I2cWrite( minute_data );
    I2cStop();
}

void Ds1307SecondSet(unsigned char second_data){
    I2cStart();
    I2cWrite( DS1307_WRITE_ADDR );
    I2cWrite( 0 );
    I2cWrite( second_data );
    I2cStop();
}

void GetAsciiValue(unsigned char time_bcd_data) {
    
    unsigned char temp;
    // Convert BCD to ASCII since LCD supports ASCII
    // 0x30 is '0' in ASCII so we always add 0x30
    temp = time_bcd_data;                 
    temp = (temp >> 4) | 0x30;    // Add 0x30 to unpacked BCD 
//    lcd_data(temp);             // write converted BCD digit
    
    temp = time_bcd_data;                 
    temp = temp & 0x0F;         // unpack lower byte
    temp = temp | 0x30;         // Add 0x30 to unpacked BCD 
//    lcd_data(temp);             // write converted BCD digit
}
 
unsigned char GetDaysSymbol(unsigned char time_bcd_data) {
    
    unsigned char temp;      
    temp = time_bcd_data;         
    temp = temp & 0x0F;
    temp = temp | 0x30;        
    
    switch(temp){
        case(0x31): return SONDAY;  
        case(0x32): return MONDAY;
        case(0x33): return TUESDAY;
        case(0x34): return WENSDAY;
        case(0x35): return THURSDAY;
        case(0x36): return FRUDAY;
        case(0x37): return SATERDAY;
        default:    return ERROR;
    }
    
}

 unsigned char GetMonthsSymbol(unsigned char time_bcd_data) {
    
    unsigned char Ltemp;
    unsigned char Htemp;
    
    Htemp = time_bcd_data;                 
    Htemp = (Htemp >> 4) | 0x30;                
    
    Ltemp = time_bcd_data;                 
    Ltemp = Ltemp & 0x0F;         
    Ltemp = Ltemp | 0x30;    
    
    switch(Ltemp)
    {
        case (0x30): if(Htemp == (0x31)) return OCTOBER;  else return ERROR;
        case (0x31): if(Htemp == (0x31)) return NOVEMBER; else return JANUARY;
        case (0x32): if(Htemp == (0x31)) return DECEMBER; else return FEBRUARY;
        case (0x33): return MARCH;
        case (0x34): return APRIL; 
        case (0x35): return MAY;
        case (0x36): return JUNE;
        case (0x37): return JULY;
        case (0x38): return AUGUST;
        case (0x39): return SEPTEMBER;
        default:     return ERROR;
    }
    
}
