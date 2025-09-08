#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void MySPI_W_CS1(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_4, (BitAction)BitValue);
}


void MySPI_W_SCK1(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_5, (BitAction)BitValue);
}
void MySPI_W_DIN1(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_7, (BitAction)BitValue);
}

void MySPI_W_CS2(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOC,GPIO_Pin_9, (BitAction)BitValue);
}


void MySPI_W_SCK2(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_12, (BitAction)BitValue);
}
void MySPI_W_DIN2(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOC,GPIO_Pin_8, (BitAction)BitValue);
}

void MySPI_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_12; // 增加PA12作SCK2
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9; // PC8 DIN2, PC9 CS2
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //初始化默认的片选和时钟电平
    MySPI_W_CS1(1);
    MySPI_W_SCK1(0);
    MySPI_W_CS2(1);
    MySPI_W_SCK2(0);
}

void MySPI_Start_Channel1(void)
{
    MySPI_W_CS1(0);
}

void MySPI_Stop_Channel1(void)
{
    MySPI_W_CS1(1);
}

void MySPI_SendByte_Channel1(uint8_t ByteSend)
{
   uint8_t i = 0x00;

   for(i=0 ;i < 8; i++)
   {
       MySPI_W_DIN1(ByteSend & (0x80 >> i));
	   MySPI_W_SCK1(1);
	   Delay_us(2);
	   MySPI_W_SCK1(0);
	   Delay_us(2);
   }	
}

void MySPI_Start_Channel2(void)
{
    MySPI_W_CS2(0);
}

void MySPI_Stop_Channel2(void)
{
    MySPI_W_CS2(1);
}

void MySPI_SendByte_Channel2(uint8_t ByteSend)
{
   uint8_t i = 0x00;

   for(i=0 ;i < 8; i++)
   {
       MySPI_W_DIN2(ByteSend & (0x80 >> i));
	   MySPI_W_SCK2(1);
	   Delay_us(2);
	   MySPI_W_SCK2(0);
	   Delay_us(2);
   }	
}

/*
void MySPI_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_128;
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;
	SPI_InitStructure.SPI_Direction=SPI_Direction_1Line_Tx;
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;
	SPI_Init(SPI1,&SPI_InitStructure);
	
	SPI_Cmd(SPI1,ENABLE);
	
	MySPI_W_CS(1);
}

void MySPI_Start(void)
{
    MySPI_W_CS(0);
}

void MySPI_Stop(void)
{
    MySPI_W_CS(1);
}

void MySPI_SendByte(uint8_t ByteSend)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	
	SPI_I2S_SendData(SPI1, ByteSend);
	
}
*/
