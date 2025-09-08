#ifndef  __TPL0501_H
#define  __TPL0501_H

#include "stdint.h"

void TPL0501_Init(void);
void Write_Channel1_ResistanceCode(uint8_t ResistanceCode);
void Write_Channel2_ResistanceCode(uint8_t ResistanceCode);
void Write_Channel1_CurrentValue(float CurrentValue);
void Write_Channel2_CurrentValue(float CurrentValue);
void striangle_Channel1_Wave(uint8_t Fre);
void striangle_Channel2_Wave(uint8_t Fre);
void striangle_Channel_Wave_30s(void);

#endif
