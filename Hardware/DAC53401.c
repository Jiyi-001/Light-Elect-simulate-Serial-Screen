#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "DAC53401_Reg.h"
#include "Timer.h"
#include "Delay.h"

#define DAC53401_ADDRESS		0x90 //A0����AGND

uint16_t I_Step_Delay = 0;
extern uint8_t Efficiency;
uint8_t Efficiency_High;
uint8_t Efficiency_Low;
extern uint8_t Light_Fre;

void DAC53401_WriteReg_Channel1(u8 RegAddress, u8 MSDB,u8 LSDB)
{
	MyI2C_Start_2();
	MyI2C_SendByte_2(DAC53401_ADDRESS);
	MyI2C_ReceiveAck_2();
	MyI2C_SendByte_2(RegAddress);
	MyI2C_ReceiveAck_2();
	MyI2C_SendByte_2(MSDB);
	MyI2C_ReceiveAck_2();
	MyI2C_SendByte_2(LSDB);
	MyI2C_ReceiveAck_2();
	MyI2C_Stop_2();
}

uint16_t DAC53401_ReadReg_Channel1(uint8_t RegAddress)
{
	uint16_t Data;
	uint8_t Data_H,Data_L;
	
	MyI2C_Start_2();
	MyI2C_SendByte_2(DAC53401_ADDRESS);
	MyI2C_ReceiveAck_2();
	MyI2C_SendByte_2(RegAddress);
	MyI2C_ReceiveAck_2();
	
	MyI2C_Start_2();
	MyI2C_SendByte_2(DAC53401_ADDRESS | 0x01);
	MyI2C_ReceiveAck_2();
	Data_H = MyI2C_ReceiveByte_2();
	MyI2C_SendAck_2(0);
	Data_L = MyI2C_ReceiveByte_2();
	MyI2C_SendAck_2(0);
	MyI2C_Stop_2();
	
	Data = (Data_H << 0x08) | Data_L ;
	return Data;
}

void DAC53401_WriteReg_Channel2(uint8_t RegAddress, uint8_t MSDB,uint8_t LSDB)
{
	MyI2C_Start_3();
	MyI2C_SendByte_3(DAC53401_ADDRESS);
	MyI2C_ReceiveAck_3();
	MyI2C_SendByte_3(RegAddress);
	MyI2C_ReceiveAck_3();
	MyI2C_SendByte_3(MSDB);
	MyI2C_ReceiveAck_3();
	MyI2C_SendByte_3(LSDB);
	MyI2C_ReceiveAck_3();
	MyI2C_Stop_3();
}

uint16_t DAC53401_ReadReg_Channel2(uint8_t RegAddress)
{
	uint16_t Data;
	uint8_t Data_H,Data_L;
	
	MyI2C_Start_3();
	MyI2C_SendByte_3(DAC53401_ADDRESS);
	MyI2C_ReceiveAck_3();
	MyI2C_SendByte_3(RegAddress);
	MyI2C_ReceiveAck_3();
	
	MyI2C_Start_3();
	MyI2C_SendByte_3(DAC53401_ADDRESS | 0x01);
	MyI2C_ReceiveAck_3();
	Data_H = MyI2C_ReceiveByte_3();
	MyI2C_SendAck_3(0);
	Data_L = MyI2C_ReceiveByte_3();
	MyI2C_SendAck_3(0);
	MyI2C_Stop_3();
	
	Data = (Data_H << 0x08) | Data_L ;
	return Data;
}

void DAC53401_Init(void)
{
	  MyI2C_Init();
}

void Light1_CurrentSet(float Light_Current)
{
    uint16_t Dac_data;
	  Dac_data = Light_Current * 11284;
    DAC53401_WriteReg_Channel1(DAC_DATA,Dac_data >> 8,Dac_data);
}

void Light2_CurrentSet(float Light_Current)
{
    uint16_t Dac_data;
	  Dac_data = Light_Current * 11284;
    DAC53401_WriteReg_Channel2(DAC_DATA,Dac_data >> 8,Dac_data);
}

void Light1_Level(uint8_t Efficiency)
{
    float Light_CurrentData;
	  //810波长
    Light_CurrentData = (Efficiency - 80) / 10 * 0.003 + 0.0245;
	  Light1_CurrentSet(Light_CurrentData);
}

void Light2_Level(uint8_t Efficiency)
{
    float Light_CurrentData;
	  //1064波长
	  Light_CurrentData = (Efficiency - 80) / 10 * 0.003 + 0.0245;
	  Light2_CurrentSet(Light_CurrentData);
}

void Light_Efficiency(uint8_t Efficiency)
{
    float Light_CurrentData;
	Light_CurrentData = Efficiency * 0.000312;
	//功率上升1mW --- 电流上升0.0003A
	Light1_CurrentSet(Light_CurrentData);
	Light2_CurrentSet(Light_CurrentData);
}

void Light_Wave_40Hz_80mW(void)
{
	Light_Efficiency(80);
	Delay_ms(13);
	Light_Efficiency(0);
	Delay_ms(12);

}

void Light1_Wave(uint8_t Light_Fre,uint8_t Efficiency_High,uint8_t Efficiency_Low)
{
	  uint8_t i;
	  float I_Step,I_Buf; 
	  I_Step = 0.00003 * (Efficiency_High - Efficiency_Low);     // 0.0015*[Efficiency_High - Efficiency_Low]/10(取Level)/10(步数)
    for(i=0;i<10;i++)
	  {
			  I_Buf = I_Step * i;
	      Light1_CurrentSet(I_Buf);
		  	Delay_ms(50 / Light_Fre);
    }
		for(i=0;i<8;i++)
		{
		    I_Buf = I_Step * (10 - i - 1);
	      Light1_CurrentSet(I_Buf);
			  Delay_ms(50 / Light_Fre);
		}
}
	
void Light2_Wave(uint8_t Light_Fre,uint8_t Efficiency_High,uint8_t Efficiency_Low)
{
	  uint8_t i;
	  float I_Step,I_Buf; 
	  I_Step = 0.00003 * (Efficiency_High - Efficiency_Low);     // 0.0032*[Efficiency_High - Efficiency_Low]/10(取Level)/10(步数)
  	for(i=0;i<10;i++)                            //Period共20步，其中上升沿11步
	  {
			  I_Buf = I_Step * i;
	      Light2_CurrentSet(I_Buf);
			  Delay_us(50000 / Light_Fre);
    }                                          //Period共20步，其中上升沿9步
		for(i=0;i<8;i++)
		{
		    I_Buf = I_Step * (10 - i - 1);
	      Light2_CurrentSet(I_Buf);
			  Delay_us(50000 / Light_Fre);
		}
}
