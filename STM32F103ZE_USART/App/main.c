#include "stm32f10x.h"
#include <stdio.h>

void delay(int t);
void RCC_Configuration(void); 
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void USART_Configuration(void);
void USART_Char(USART_TypeDef* USARTx, u8 c);
void USART_String(USART_TypeDef* USARTx, u8 *c);

int main(void){     
	//初始化
	RCC_Configuration();
	GPIO_Configuration();
	NVIC_Configuration();
	USART_Configuration();
	while(1);
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);   //打开PC端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);   //打开PD端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);   //打开PE端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);  //打开USART1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);  //打开USART2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);  //打开USART3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);   //打开UART4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);   //打开UART5
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

	//USATR2_TX (PA.2)
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//USATR2_RX (PA.3)
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//USATR3_TX (PB.10)
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//USATR3_RX (PB.11)
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//UATR4_TX (PC.10)
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	//UATR4_RX (PC.11)
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	//UATR5_TX (PC.12)
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	//UATR5_RX (PD.2)
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOD,&GPIO_InitStructure);
}

void NVIC_Configuration(void){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	//USART1中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
	NVIC_Init(&NVIC_InitStructure);

	//USART2中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_Init(&NVIC_InitStructure);

	//USART3中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 
	NVIC_Init(&NVIC_InitStructure);

	//USART4中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4; 
	NVIC_Init(&NVIC_InitStructure);

	//UART5中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5; 
	NVIC_Init(&NVIC_InitStructure);
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

	//USART2配置
	USART_Init(USART2,&USART_InitStructure); 
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE); 
	USART_Cmd(USART2,ENABLE); 
	USART_ClearFlag(USART2,USART_FLAG_TC);

	//USART3配置
	USART_Init(USART3,&USART_InitStructure); 
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE); 
	USART_Cmd(USART3,ENABLE); 
	USART_ClearFlag(USART3,USART_FLAG_TC);

	//UART4配置
	USART_Init(UART4,&USART_InitStructure); 
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE); 
	USART_Cmd(UART4,ENABLE); 
	USART_ClearFlag(UART4,USART_FLAG_TC);

	//UART5配置
	USART_Init(UART5,&USART_InitStructure); 
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE); 
	USART_Cmd(UART5,ENABLE); 
	USART_ClearFlag(UART5,USART_FLAG_TC);
}

void USART_Char(USART_TypeDef* USARTx, u8 c){
	USART_SendData(USARTx,c);
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);
	USART_ClearFlag(USARTx,USART_FLAG_TC);  
}

void USART_String(USART_TypeDef* USARTx, u8 *c){
	while(*c != '\0'){
		USART_Char(USARTx,*c++);
	}
}

void USART1_IRQHandler(void){   
	u8 Data;  
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){    
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);       
		Data=USART_ReceiveData(USART1);

		if(Data=='1'){
			USART_String(USART1, "Correct!");
		}else{
			USART_String(USART1, "Wrong!");
		}
	}  
} 

void USART2_IRQHandler(void){   
	u8 Data;  
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){    
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);       
		Data=USART_ReceiveData(USART2);

		if(Data=='2'){
			USART_String(USART2, "Correct!");
		}else{
			USART_String(USART2, "Wrong!");
		}
	}  
} 

void USART3_IRQHandler(void){   
	u8 Data;  
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){    
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);       
		Data=USART_ReceiveData(USART3);

		if(Data=='3'){
			USART_String(USART3, "Correct!");
		}else{
			USART_String(USART3, "Wrong!");
		}
	}  
} 

void UART4_IRQHandler(void){   
	u8 Data;  
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET){    
		USART_ClearITPendingBit(UART4,USART_IT_RXNE);       
		Data=USART_ReceiveData(UART4);

		if(Data=='4'){
			USART_String(UART4, "Correct!");
		}else{
			USART_String(UART4, "Wrong!");
		}
	}  
} 

void UART5_IRQHandler(void){   
	u8 Data;  
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET){    
		USART_ClearITPendingBit(UART5,USART_IT_RXNE);       
		Data=USART_ReceiveData(UART5);

		if(Data=='5'){
			USART_String(UART5, "Correct!");
		}else{
			USART_String(UART5, "Wrong!");
		}
	}  
} 
