#include "Beep.h"
#include "Delay.h"

void Beep_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef gpio;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Pin = GPIO_Pin_1; // PB1
    GPIO_Init(GPIOB, &gpio);

    // Default OFF
    GPIO_SetBits(GPIOB, GPIO_Pin_1);
}

void Beep_On(void)
{
    // Active level depends on wiring; assuming low-active (PB1 low = sound)
    GPIO_ResetBits(GPIOB, GPIO_Pin_1);
}

void Beep_Off(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_1);
}

void Beep_Toggle(void)
{
    if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_1))
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_1);
    }
    else
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_1);
    }
}

void Beep_ToneBlocking(uint16_t frequencyHz, uint16_t durationMs)
{
    if (frequencyHz == 0 || durationMs == 0)
    {
        return;
    }

    uint32_t periodUs = 1000000UL / frequencyHz;
    uint32_t halfPeriodUs = periodUs / 2U;

    // Total cycles to run
    uint32_t totalCycles = (uint32_t)frequencyHz * durationMs / 1000UL;

    for (uint32_t i = 0; i < totalCycles; i++)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_1);
        Delay_us(halfPeriodUs);
        GPIO_SetBits(GPIOB, GPIO_Pin_1);
        Delay_us(halfPeriodUs);
    }
}

