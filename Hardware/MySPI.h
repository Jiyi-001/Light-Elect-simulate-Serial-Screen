#ifndef __MYSPI_H
#define __MYSPI_H

void MySPI_Init(void);
void MySPI_W_CS1(uint8_t BitValue);
void MySPI_W_SCK1(uint8_t BitValue);
void MySPI_W_DIN1(uint8_t BitValue);
void MySPI_W_CS2(uint8_t BitValue);
void MySPI_W_SCK2(uint8_t BitValue);
void MySPI_W_DIN2(uint8_t BitValue);
void MySPI_Start_Channel1(void);
void MySPI_Stop_Channel1(void);
void MySPI_SendByte_Channel1(uint8_t ByteSend);
void MySPI_Start_Channel2(void);
void MySPI_Stop_Channel2(void);
void MySPI_SendByte_Channel2(uint8_t ByteSend);


#endif
