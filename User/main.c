#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "TPL0501.h"
#include "DAC53401.h"
#include "Delay.h"
#include "Key.h"
#include "Power_probe.h"
#include "stdint.h"
#include "Serial.h"
#include "stdio.h"
#include "Beep.h"
#include "RTC.h"

#define TIME_STRING_LEN 32
char TimeString[TIME_STRING_LEN];
extern 	float Current;
extern  uint8_t  Light_LevelData;
extern	uint8_t  Mode_Flag;
extern  uint8_t  Wave_Level;
extern  uint8_t  Wave_Frequency;
extern  uint16_t  Timer_1s;
extern  uint8_t  Power_level;
extern  uint8_t  Key_Pressed;
extern  uint8_t  Protect_Flag;
uint8_t Key_Released = 0;
//extern  uint8_t  Current_Count;
uint8_t Power_pecent;
uint8_t TERMINAL_FLAG = 0;
uint8_t Send_Flag = 0;
uint8_t Pressed_Time = 0;

int main(void)
{	
	SystemInit();
	RTC_Init();
	Serial_Init();
	Key_Init();
	Beep_Init();
    OLED_Init();
	Powerprobe_Init();
	DAC53401_Init();
	TPL0501_Init();
	Delay_s(1);
	GPIO_SetBits(GPIOC,GPIO_Pin_11);
	Delay_ms(1);
	TIM1_Init();

	RTC_TimeTypeDef rtc_time;
    while(1)
{
	if(TERMINAL_FLAG == 1)
	{
		TERMINAL_FLAG = 0;
		Mode_Flag=0;
		TIM3_DISABLE();
		TIM4_DISABLE();
		Light_Efficiency(0);
		Write_Channel1_CurrentValue(0.0003);
		Write_Channel2_CurrentValue(0.0003);
		for(uint8_t i = 0; i < 3; i++)
		{
			Beep_ToneBlocking(4000,500);
			Delay_ms(500);
		}
	}

	if(Send_Flag == 1)
	{
		Send_Flag = 0;
		RTC_Get_Time(&rtc_time);
		uint8_t Power_pecent=Power_Num();

		snprintf(TimeString, TIME_STRING_LEN, 
             "%04d/%02d/%02d %02d:%02d",
             rtc_time.year,
             rtc_time.month,
             rtc_time.day,
             rtc_time.hour,
             rtc_time.minute);

    // 发送时间字符串到串口屏
    	printf("va0.txt=\"%s\"\xff\xff\xff", TimeString);

		if(Power_pecent>=75)		printf("va1.val=6\xff\xff\xff");
		else if(Power_pecent>=50)	printf("va1.val=5\xff\xff\xff");
		else if(Power_pecent>=25)	printf("va1.val=4\xff\xff\xff");
		else if(Power_pecent>=10)	printf("va1.val=2\xff\xff\xff");
		else						printf("va1.val=3\xff\xff\xff");
	}
}
}


void TIM1_UP_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
    {
		static uint8_t Protect_count = 0;
        Send_Flag = 1;
		if(Protect_Flag == 1)
		{
			Protect_count ++;
			if(Protect_count >= 2)
			{
				Protect_Flag = 0;
			}
		}
		if(Key_Pressed == 1)
		{
			Pressed_Time ++;
			if(Pressed_Time >4)
			{
				Key_Released  = 1;
				TERMINAL_FLAG = 1;
				Pressed_Time  = 0;
				Timer_1s = 1;
				printf("page 1\xff\xff\xff");
			}
		}
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}
