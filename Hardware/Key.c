#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "TPL0501.h"
#include "DAC53401.h"
#include "Timer.h"
#include "stdio.h"
#include "Function.h"
#define  Delay_time  200

uint16_t TIMER = 0;
float    Current = 0.0005;
uint8_t  Light_LevelData = 0;
uint8_t  Mode_Flag=0;
uint8_t  Wave_Frequency=5;
//uint8_t  Current_Count = 3;
uint8_t fanhui_flag = 0;
uint8_t Powerdown_time;
uint8_t Efficiency = 80;
extern uint8_t Efficiency_High;
extern uint8_t Efficiency_Low;
uint8_t Light_Fre = 0;
extern uint8_t RX_DFLAG;
extern uint8_t TERMINAL_FLAG;
extern uint16_t  Timer_1s;
void Key_Init(void)
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		
		GPIO_InitTypeDef  GPIO_InitStructure; 
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOB,&GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOA,&GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOC,&GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOC,&GPIO_InitStructure);
		
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource15);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource10);
		
		EXTI_InitTypeDef EXTI_InitStructure;
		EXTI_InitStructure.EXTI_Line=EXTI_Line12|EXTI_Line13|EXTI_Line14|EXTI_Line15|EXTI_Line8;
		EXTI_InitStructure.EXTI_LineCmd=ENABLE;
		EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
		EXTI_Init(&EXTI_InitStructure);
		
		EXTI_InitStructure.EXTI_Line=EXTI_Line10;
		EXTI_InitStructure.EXTI_LineCmd=ENABLE;
		EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
		EXTI_Init(&EXTI_InitStructure);
		
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		NVIC_Init(&NVIC_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		NVIC_Init(&NVIC_InitStructure);
}


void EXTI15_10_IRQHandler(void)
{
		if(EXTI_GetITStatus(EXTI_Line12)==SET)
		{
				TIM2_Init();
				while(TIMER <= Delay_time);
				TIMER = 0;
				printf("click b0,1\xff\xff\xff");
				TIM2_DISABLE();
				EXTI_ClearITPendingBit(EXTI_Line12);
	  }
	
		if(EXTI_GetITStatus(EXTI_Line13)==SET)
		{
				TIM2_Init();
				while(TIMER <= Delay_time){};
			  	TIMER = 0;
				printf("click b1,1\xff\xff\xff");
				TIM2_DISABLE();
				EXTI_ClearITPendingBit(EXTI_Line13);
    }
		if(EXTI_GetITStatus(EXTI_Line14)==SET)
		{
				TIM2_Init();
				while(TIMER <= Delay_time);
				TIMER = 0;
				printf("click b1,1\xff\xff\xff");
				TIM2_DISABLE();
				EXTI_ClearITPendingBit(EXTI_Line14);
		}
		if(EXTI_GetITStatus(EXTI_Line15)==SET)
	  	{
			if(Mode_Flag != 0)
			{
				Key_Pressed = 1;
			}
		    TIM2_Init();
	    	while(TIMER <= Delay_time){};
			switch(RX_DFLAG){
				case 1 : Mode1_menu(); break;
				case 2 : Mode2_menu(); break;
				case 3 : Mode3_menu(); break;
			}
			TIMER = 0;
        	TIM2_DISABLE();
			EXTI_ClearITPendingBit(EXTI_Line15);         
		}
		// PC10 电源键下降沿中断（由原来的 PC6 迁移而来）
		if(EXTI_GetITStatus(EXTI_Line10)==SET)
	  	{
			TIM2_Init();
	    	while(TIMER <= 50){};
	      	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)==0)
	      	{
	      		GPIO_ResetBits(GPIOC,GPIO_Pin_11); // 关机：由PC7迁移到PC11
	      	}
	    	TIMER = 0;
	    	TIM2_DISABLE();
	  		Delay_ms(100);
			EXTI_ClearITPendingBit(EXTI_Line10);
	  }
}

void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line8)==SET)
	{
		TIM2_Init();
		while(TIMER <= Delay_time);
		TERMINAL_FLAG = 1;
		Timer_1s = 1;
		printf("page 1\xff\xff\xff");
        TIMER = 0;
		TIM2_DISABLE();				
		EXTI_ClearITPendingBit(EXTI_Line8);	
	}
}

void TIM2_IRQHandler(void)
{
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
		{
			  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
				TIMER++;
		}
}
