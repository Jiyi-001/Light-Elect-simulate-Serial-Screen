#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void MyI2C_W_SCL2(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_1, (BitAction)BitValue);
	Delay_us(10);
}

void MyI2C_W_SDA2(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_2, (BitAction)BitValue);
	Delay_us(10);
}

uint8_t MyI2C_R_SDA2(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2);
	Delay_us(10);
	return BitValue;
}

void MyI2C_W_SCL3(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)BitValue);
	Delay_us(10);
}

void MyI2C_W_SDA3(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_7, (BitAction)BitValue);
	Delay_us(10);
}

uint8_t MyI2C_R_SDA3(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);
	Delay_us(10);
	return BitValue;
}

void MyI2C_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;  // I2C2: PA1(SCL), PA2(SDA)
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;  // I2C3: PB6(SCL), PB7(SDA)
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);
	GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7);
}

void MyI2C_Start_2(void)
{
	MyI2C_W_SDA2(1);
	MyI2C_W_SCL2(1);
	MyI2C_W_SDA2(0);
	MyI2C_W_SCL2(0);
}

void MyI2C_Stop_2(void)
{
	MyI2C_W_SDA2(0);
	MyI2C_W_SCL2(1);
	MyI2C_W_SDA2(1);
}

void MyI2C_SendByte_2(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SDA2(Byte & (0x80 >> i));
		MyI2C_W_SCL2(1);
		MyI2C_W_SCL2(0);
	}
}

uint8_t MyI2C_ReceiveByte_2(void)
{
	uint8_t i, Byte = 0x00;
	MyI2C_W_SDA2(1);
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SCL2(1);
		if (MyI2C_R_SDA2() == 1){Byte |= (0x80 >> i);}
		MyI2C_W_SCL2(0);
	}
	return Byte;
}

void MyI2C_SendAck_2(uint8_t AckBit)
{
	MyI2C_W_SDA2(AckBit);
	MyI2C_W_SCL2(1);
	MyI2C_W_SCL2(0);
}

uint8_t MyI2C_ReceiveAck_2(void)
{
	uint8_t AckBit;
	MyI2C_W_SDA2(1);
	MyI2C_W_SCL2(1);
	AckBit = MyI2C_R_SDA2();
	MyI2C_W_SCL2(0);
	return AckBit;
}

void MyI2C_Start_3(void)
{
	MyI2C_W_SDA3(1);
	MyI2C_W_SCL3(1);
	MyI2C_W_SDA3(0);
	MyI2C_W_SCL3(0);
}

void MyI2C_Stop_3(void)
{
	MyI2C_W_SDA3(0);
	MyI2C_W_SCL3(1);
	MyI2C_W_SDA3(1);
}

void MyI2C_SendByte_3(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SDA3(Byte & (0x80 >> i));
		MyI2C_W_SCL3(1);
		MyI2C_W_SCL3(0);
	}
}

uint8_t MyI2C_ReceiveByte_3(void)
{
	uint8_t i, Byte = 0x00;
	MyI2C_W_SDA3(1);
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SCL3(1);
		if (MyI2C_R_SDA3() == 1){Byte |= (0x80 >> i);}
		MyI2C_W_SCL3(0);
	}
	return Byte;
}

void MyI2C_SendAck_3(uint8_t AckBit)
{
	MyI2C_W_SDA3(AckBit);
	MyI2C_W_SCL3(1);
	MyI2C_W_SCL3(0);
}

uint8_t MyI2C_ReceiveAck_3(void)
{
	uint8_t AckBit;
	MyI2C_W_SDA3(1);
	MyI2C_W_SCL3(1);
	AckBit = MyI2C_R_SDA3();
	MyI2C_W_SCL3(0);
	return AckBit;
}
