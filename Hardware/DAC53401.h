#ifndef __DAC53401_H
#define __DAC53401_H

#include "stdint.h"

void DAC53401_Init(void);
void DAC53401_WriteReg_Channel1(uint8_t RegAddress, uint8_t MSDB,uint8_t LSDB);
uint16_t DAC53401_ReadReg_Channel1(uint8_t RegAddress);
void DAC53401_WriteReg_Channel2(uint8_t RegAddress, uint8_t MSDB,uint8_t LSDB);
uint16_t DAC53401_ReadReg_Channel2(uint8_t RegAddress);
void Light1_CurrentSet(float Light_Current);
void Light2_CurrentSet(float Light_Current);
void Light1_Level(uint8_t Efficiency);
void Light2_Level(uint8_t Efficiency);
void Light1_Wave(uint8_t Light_Fre,uint8_t Efficiency_High,uint8_t Efficiency_Low);
void Light2_Wave(uint8_t Light_Fre,uint8_t Efficiency_High,uint8_t Efficiency_Low);
void Light_Efficiency(uint8_t Efficiency);
void Light_Wave_40Hz_80mW(void);

#endif
