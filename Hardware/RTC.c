#include "RTC.h"

// 月份数据表
const uint8_t table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; 
const uint8_t mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

// RTC时钟初始化
uint8_t RTC_Init(void)
{
    // 使能PWR和BKP时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    
    // 使能后备寄存器访问
    PWR_BackupAccessCmd(ENABLE);
    
    // 检查是否第一次配置RTC
    if(BKP_ReadBackupRegister(BKP_DR1) != 0x5050)
    {
        // 复位备份区域
        BKP_DeInit();
        
        // 使能LSE
        RCC_LSEConfig(RCC_LSE_ON);
        // 等待LSE就绪
        while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
        
        // 选择LSE作为RTC时钟源
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
        
        // 使能RTC时钟
        RCC_RTCCLKCmd(ENABLE);
        
        // 等待RTC寄存器同步
        RTC_WaitForSynchro();
        
        // 等待上一次写操作完成
        RTC_WaitForLastTask();
        
        // 设置RTC预分频值，32768Hz/32768 = 1Hz
        RTC_SetPrescaler(32767);
        
        // 等待设置完成
        RTC_WaitForLastTask();
        
        // 初始化时间为2025年8月9日17:20:00
        RTC_Set_Time(2025,9,8,10,20,00);
        
        // 标记已经初始化过
        BKP_WriteBackupRegister(BKP_DR1, 0x5050);
        return 0;
    }
    return 1;
}

// 设置时间
void RTC_Set_Time(uint16_t year,uint8_t mon,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec)
{
    uint32_t seccount=0;
    uint16_t t;
    uint32_t daycnt=0;
    
    // 年份统计
    for(t=1970;t<year;t++)
    {
        if(Is_Leap_Year(t))daycnt+=366;
        else daycnt+=365;
    }
    
    // 月份统计
    for(t=0;t<mon-1;t++)
    {
        daycnt+=mon_table[t];
        if(Is_Leap_Year(year)&&t==1)daycnt++;
    }
    
    // 天数统计
    daycnt+=day-1;
    
    // 转换为秒计数
    seccount=daycnt*86400+hour*3600+min*60+sec;
    
    // 设置时间
    RTC_WaitForLastTask();
    RTC_SetCounter(seccount);
    RTC_WaitForLastTask();
}

// 获取时间
void RTC_Get_Time(RTC_TimeTypeDef* rtc_time)
{
    static uint16_t daycnt=0;
    uint32_t timecount=0;
    uint32_t temp=0;
    uint16_t temp1=0;
    timecount=RTC_GetCounter();
    temp=timecount/86400;   // 得到天数
    if(daycnt!=temp)        // 超过一天了
    {
        daycnt=temp;
        temp1=1970;         // 从1970年开始
        while(temp>=365)
        {
            if(Is_Leap_Year(temp1))
            {
                if(temp>=366)temp-=366;
                else break;
            }
            else temp-=365;
            temp1++;
        }
        rtc_time->year=temp1;// 得到年份
        temp1=0;
        while(temp>=28)      // 超过了一个月
        {
            if(Is_Leap_Year(rtc_time->year)&&temp1==1)
            {
                if(temp>=29)temp-=29;
                else break;
            }
            else
            {
                if(temp>=mon_table[temp1])temp-=mon_table[temp1];
                else break;
            }
            temp1++;
        }
        rtc_time->month=temp1+1;  // 得到月份
        rtc_time->day=temp+1;     // 得到日期
    }
    temp=timecount%86400;         // 得到秒钟数   
    rtc_time->hour=temp/3600;     // 小时
    rtc_time->minute=(temp%3600)/60; // 分钟    
    rtc_time->second=(temp%3600)%60;  // 秒钟
    
    // 获取星期
    temp=rtc_time->year-1970;
    rtc_time->week=temp+1;
}

// 判断是否是闰年
uint8_t Is_Leap_Year(uint16_t year)
{
    if(year%4==0)
    {
        if(year%100==0)
        {
            if(year%400==0)return 1;
            else return 0;
        }else return 1;
    }else return 0;
}
