#include "stm32f10x.h"

void delay(int t);

int main(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;

	//使能PB端口时钟    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  
      
    //PB.14初始化  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
    GPIO_Init(GPIOB, &GPIO_InitStructure);    
      
    //点亮小灯(PB.14)  
    GPIO_SetBits(GPIOB, GPIO_Pin_14); 
	while(1);
}

void delay(int t){
	int x,temp=10000;
	while(temp--){
		x=t;
		while(x--);
	}
}
