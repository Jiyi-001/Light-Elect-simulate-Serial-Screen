#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "Delay.h"
#include "Timer.h"
#include "OLED.h"

extern  uint16_t TIMER;
extern  uint8_t  fanhui_flag;

void TPL0501_Init(void)
{
    MySPI_Init();
}

void Write_Channel1_ResistanceCode(uint8_t ResistanceCode)
{
    MySPI_Start_Channel1();
	MySPI_SendByte_Channel1(0x80);
	MySPI_SendByte_Channel1(ResistanceCode);
	MySPI_Stop_Channel1();
}

void Write_Channel2_ResistanceCode(uint8_t ResistanceCode)
{
    MySPI_Start_Channel2();
	MySPI_SendByte_Channel2(0x80);
	MySPI_SendByte_Channel2(ResistanceCode);
	MySPI_Stop_Channel2();
}

void Write_Channel1_CurrentValue(float CurrentValue)
{
	uint32_t Resistance = 24.96/CurrentValue;
	uint8_t ResistanceCode = (Resistance << 8) / 100000;
    MySPI_Start_Channel1();
	Write_Channel1_ResistanceCode(ResistanceCode);
    MySPI_Stop_Channel1();
}

void Write_Channel2_CurrentValue(float CurrentValue)
{
	uint32_t Resistance = 24.96/CurrentValue;
	uint8_t ResistanceCode = (Resistance << 8) / 100000;
    MySPI_Start_Channel2();
	Write_Channel2_ResistanceCode(ResistanceCode);
    MySPI_Stop_Channel2();
}

void striangle_Channel_Wave_30s(void)
{
	uint8_t i=0;
	float Current = 0.0005;
	for(i=0;i<18;i++)
	{	    
		Current += 0.00005;
		Write_Channel1_CurrentValue(Current);
		Write_Channel2_CurrentValue(Current);
		Delay_ms(750);
		if(fanhui_flag==1)
	    {return;}
	}
	for(i=0;i<18;i++)
	{	    
		Current -= 0.00005;
		Write_Channel1_CurrentValue(Current);
		Write_Channel2_CurrentValue(Current);
		Delay_ms(750);
		if(fanhui_flag==1)
	    {return;}
	}
}
