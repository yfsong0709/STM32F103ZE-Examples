#include "stm32f10x.h"

void delay(int t);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void TIM_Configuration(void);

int main(void){
	RCC_Configuration();
	GPIO_Configuration();
	TIM_Configuration();
	TIM_SetCompare1(TIM4, 200);
	TIM_SetCompare2(TIM4, 200);
	TIM_SetCompare3(TIM4, 200);
	TIM_SetCompare4(TIM4, 200);
	delay(5000);
	TIM_SetCompare1(TIM4, 100);
	TIM_SetCompare2(TIM4, 100);
	TIM_SetCompare3(TIM4, 100);
	TIM_SetCompare4(TIM4, 100);
	delay(2000);
	TIM_SetCompare1(TIM4, 110);
	TIM_SetCompare2(TIM4, 110);
	TIM_SetCompare3(TIM4, 110);
	TIM_SetCompare4(TIM4, 110);
	delay(1000);		   
	TIM_SetCompare1(TIM4, 130);
	TIM_SetCompare2(TIM4, 130);
	TIM_SetCompare3(TIM4, 130);
	TIM_SetCompare4(TIM4, 130);
	delay(1000);		   
	TIM_SetCompare1(TIM4, 150);
	TIM_SetCompare2(TIM4, 150);
	TIM_SetCompare3(TIM4, 150);
	TIM_SetCompare4(TIM4, 150);
	delay(1000);		   
	TIM_SetCompare1(TIM4, 170);
	TIM_SetCompare2(TIM4, 170);
	TIM_SetCompare3(TIM4, 170);
	TIM_SetCompare4(TIM4, 170);
	delay(1000);		   
	TIM_SetCompare1(TIM4, 190);
	TIM_SetCompare2(TIM4, 190);
	TIM_SetCompare3(TIM4, 190);
	TIM_SetCompare4(TIM4, 190);
	delay(3000);		   
	TIM_SetCompare1(TIM4, 150);
	TIM_SetCompare2(TIM4, 150);
	TIM_SetCompare3(TIM4, 150);
	TIM_SetCompare4(TIM4, 150);
	delay(1000);		   
	TIM_SetCompare1(TIM4, 100);
	TIM_SetCompare2(TIM4, 100);
	TIM_SetCompare3(TIM4, 100);
	TIM_SetCompare4(TIM4, 100);
	while(1);
}

void RCC_Configuration(void){
    SystemInit();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
}

void GPIO_Configuration(void){
    GPIO_InitTypeDef GPIO_InitStructure;	

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void TIM_Configuration(void){
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    TIM_TimeBaseStructure.TIM_Period=1999;
    TIM_TimeBaseStructure.TIM_Prescaler=719;
    TIM_TimeBaseStructure.TIM_ClockDivision=0;
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
  
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;

    TIM_OC1Init(TIM4,&TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);    
  
    TIM_OC2Init(TIM4,&TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  
   
    TIM_OC3Init(TIM4,&TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
  
    TIM_OC4Init(TIM4,&TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_Cmd(TIM4,ENABLE);   
}

void delay(int t){
	int x,temp=10000;
	while(temp--){
		x=t;
		while(x--);
	}
}


