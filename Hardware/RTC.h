#ifndef __RTC_H
#define __RTC_H

#include "stm32f10x.h"

typedef struct 
{
    uint16_t year;   // 年
    uint8_t month;   // 月
    uint8_t day;     // 日
    uint8_t hour;    // 时
    uint8_t minute;  // 分
    uint8_t second;  // 秒
    uint8_t week;    // 星期
} RTC_TimeTypeDef;

uint8_t RTC_Init(void);
void RTC_Set_Time(uint16_t year, uint8_t mon, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec);
void RTC_Get_Time(RTC_TimeTypeDef* rtc_time);
uint8_t Is_Leap_Year(uint16_t year);

#endif
