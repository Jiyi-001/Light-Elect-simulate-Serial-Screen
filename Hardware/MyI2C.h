#ifndef __MYI2C_H
#define __MYI2C_H

void MyI2C_Init(void);
void MyI2C_W_SCL2(uint8_t BitValue);
void MyI2C_W_SDA2(uint8_t BitValue);
uint8_t MyI2C_R_SDA2(void);
void MyI2C_W_SCL3(uint8_t BitValue);
void MyI2C_W_SDA3(uint8_t BitValue);
uint8_t MyI2C_R_SDA3(void);
void MyI2C_Start_2(void);
void MyI2C_Stop_2(void);
void MyI2C_SendByte_2(uint8_t Byte);
uint8_t MyI2C_ReceiveByte_2(void);
void MyI2C_SendAck_2(uint8_t AckBit);
uint8_t MyI2C_ReceiveAck_2(void);
void MyI2C_Start_3(void);
void MyI2C_Stop_3(void);
void MyI2C_SendByte_3(uint8_t Byte);
uint8_t MyI2C_ReceiveByte_3(void);
void MyI2C_SendAck_3(uint8_t AckBit);
uint8_t MyI2C_ReceiveAck_3(void);

#endif
