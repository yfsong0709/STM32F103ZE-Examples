#include "stm32f10x.h"
#include "NRF905.h"
#include <stdio.h>

void delay(int t);
void RCC_Configuration(void); 
void GPIO_Configuration(void);
void USART_Configuration(void);
int fputc(int ch, FILE *f);

int main(void){
	RCC_Configuration();
	GPIO_Configuration();
	USART_Configuration();
	NRF905_Init();
	while(1){
		NRF905_data[0]=0x00;
		NRF905_Rx();
		printf("The key is %d\n",(int)NRF905_data[0]);
		delay(200);
	}
}

int fputc(int ch, FILE *f){
  USART_SendData(USART1, (unsigned char) ch);
  while (!(USART1->SR & USART_FLAG_TXE));
  return (ch);
}

void delay(int t){
	int x,temp=10000;
	while(temp--){
		x=t;
		while(x--);
	}
}

void RCC_Configuration(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //打开PA端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   //打开PB端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);  //打开USART1
}

void GPIO_Configuration(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;

	//USATR1_TX (PA.9)
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//USATR1_RX (PA.10)
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//NRF905
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void USART_Configuration(void){
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=9600; 
	USART_InitStructure.USART_WordLength=USART_WordLength_8b; 
	USART_InitStructure.USART_StopBits=USART_StopBits_1; 
	USART_InitStructure.USART_Parity=USART_Parity_No; 
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None; 
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;

	//USART1配置
	USART_Init(USART1,&USART_InitStructure); 
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE); 
	USART_Cmd(USART1,ENABLE); 
	USART_ClearFlag(USART1,USART_FLAG_TC);
}
