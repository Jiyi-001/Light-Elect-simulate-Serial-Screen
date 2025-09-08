#ifndef  __KEY_H
#define  __KEY_H

void Key_Init(void);
uint8_t Key_GetNum(void);
uint8_t Key_Enter_Get(void);	//确认键
uint8_t Key_Back_Get(void);	//返回键
uint8_t Key_Up_Get(void);
uint8_t Key_Down_Get(void);
void Key_Reset_All(void);	//清除所有按键标志位

#endif 
