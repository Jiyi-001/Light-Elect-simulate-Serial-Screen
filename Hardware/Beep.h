#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f10x.h"
#include <stdint.h>

void Beep_Init(void);
void Beep_On(void);
void Beep_Off(void);
void Beep_Toggle(void);

// Generate a blocking square-wave tone on PB1
// frequencyHz: target frequency (e.g., 1000 for 1kHz)
// durationMs: tone duration in milliseconds
void Beep_ToneBlocking(uint16_t frequencyHz, uint16_t durationMs);

#endif

