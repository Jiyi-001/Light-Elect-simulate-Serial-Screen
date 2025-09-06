#include "TPL0501.h"
#include "DAC53401.h"
#include "OLED.h"
#include "delay.h"
#include "Timer.h"
#include "Power_probe.h"
#include "stm32f10x.h"                  // Device header
#include "Key.h"
#include "stdio.h"

extern 	float    Current;
extern  uint8_t  Light_LevelData;
extern	uint8_t  Mode_Flag;
extern  uint8_t  Wave_Level;
extern  uint8_t  Wave_Frequency;
extern  uint8_t  fanhui_flag;
extern  uint8_t  Powerdown_time;
extern  uint8_t  Efficiency;
extern  uint8_t  Efficiency_High;
extern  uint8_t  Efficiency_Low;
extern  uint8_t  Light_Fre;
extern  uint8_t  TERMINAL_FLAG;
uint8_t  Power_level=3;
uint16_t  Timer_1s = 1;
static char TimeString[6]; // "MM:SS" + '\0'
uint16_t Empty_19minute=0;
//extern  uint8_t  Current_Count;
volatile u8 Timer3_Count = 0;
volatile u8 Light_state = 0;
void version_menu(void)
{
    Mode_Flag=6;
    OLED_Clear();
	OLED_ShowChinese(0,0,"可调控光电刺激");
	OLED_ShowChar(0,16,'V',OLED_8X16);
	OLED_ShowNum(8,16,1,1,OLED_8X16); 
	OLED_ShowChar(16,16,'.',OLED_8X16);
	OLED_ShowNum(24,16,0,1,OLED_8X16); 
  	OLED_ShowString(0,32,"Editor: LYQ",OLED_8X16);
	OLED_Update();
	fanhui_flag=0;
    while(1)
	{
	    if(Power_Num()>=71)
		{
			Power_level=3;
		}
		else if(Power_Num()>=41)
		{
			Power_level=2;
		}
		else if(Power_Num()>=11)
		{
			Power_level=1;
		}
		else
		{
		    Power_level=0;
		}
        if(fanhui_flag==1)
		{return;}	
	}
}

void  Mode1_menu(void)
{
	Mode_Flag=1;
	Write_Channel1_CurrentValue(0.0003);
	Write_Channel2_CurrentValue(0.0003);
	Efficiency = 80;
	DAC53401_WriteReg_Channel1(0xD1,0X01,0XE0);
	DAC53401_WriteReg_Channel2(0xD1,0X01,0XE0);
	TIM3_Init();
    TIM4_Init();
	printf("page 4\xff\xff\xff");
}

void  Mode2_menu(void)
{
	Mode_Flag = 2;
	Light_Efficiency(0);
    TIM4_Init();
	Write_Channel1_CurrentValue(0.0014);
	Write_Channel2_CurrentValue(0.0014);
	printf("page 4\xff\xff\xff");
}

void  Mode3_menu(void)
{
    Mode_Flag=3;
	Efficiency = 80;
	DAC53401_WriteReg_Channel1(0xD1,0X01,0XE0);
	DAC53401_WriteReg_Channel2(0xD1,0X01,0XE0);
	Write_Channel1_CurrentValue(0.0014);
	Write_Channel2_CurrentValue(0.0014);
	TIM3_Init();
    TIM4_Init();
	printf("page 4\xff\xff\xff");
}

void  Mode4_menu(void)
{
	
}

void fanhui(void)
{
    return;
}

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		Timer3_Count++;
		if (Timer3_Count == 20) 
		{
			Light_state = !Light_state;
			if(Light_state)
			{
				Light_Efficiency(80);
			}
			else if(!Light_state)
			{
				Light_Efficiency(0);
			}
			Timer3_Count = 0;
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		Timer_1s++;
		uint16_t minutes = 19 - (Timer_1s / 60);
		uint16_t seconds = 59 - (Timer_1s % 60);
		snprintf(TimeString, sizeof(TimeString), "%02u:%02u", minutes, seconds);
		printf("t0.txt=\"%s\"\xff\xff\xff", TimeString);
		if(Timer_1s > 1199)
		{
			Timer_1s = 0;
			TERMINAL_FLAG = 1;
			printf("page 5\xff\xff\xff");
		}
	}
}
