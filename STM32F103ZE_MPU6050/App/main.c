#include "stm32f10x.h"
#include "I2C_MPU6050.h"
#include "kalman.h"
#include "quaternion.h"
#include <stdio.h>

float X_angle = 0.0f, Y_angle = 0.0f;      // 姿态角
float X_angle0 = 0.0f, Y_angle0 = 0.0f;    // 姿态角初值
u8 mode=1;                                 // mode=0代表四元数法，1代表卡尔曼滤波
int AX,AY,AZ,GX,GY,GZ;

void delay(int t);
void RCC_Configuration(void); 
void GPIO_Configuration(void);
void I2C_Configuration(void);
void USART_Configuration(void);
void TIM_Configuration(void);
void NVIC_Configuration(void);
int fputc(int ch, FILE *f);

int main(void){
	delay(500);
	RCC_Configuration();
	GPIO_Configuration();
	I2C_Configuration();
	TIM_Configuration();
	NVIC_Configuration();
	USART_Configuration();
	MPU6050_Init(I2C2);
	delay(500);
	while(1){
		/*
		show(ACCEL_XOUT_H);
		show(ACCEL_YOUT_H);
		show(ACCEL_ZOUT_H);
		show(GYRO_XOUT_H);
		show(GYRO_YOUT_H);
		show(GYRO_ZOUT_H);
		*/
		delay(200);
		printf("X: %2.2f ------- Y: %2.2f\n", X_angle, Y_angle);
	}
}
/*
void show(u8 addr){
	unsigned int dat;
	switch(addr){
		case ACCEL_XOUT_H:{printf("AX:");break;}
		case ACCEL_YOUT_H:{printf("AY:");break;}	 
		case ACCEL_ZOUT_H:{printf("AZ:");break;}
		case GYRO_XOUT_H:{printf("GX:");break;}
		case GYRO_YOUT_H:{printf("GY:");break;}
		case GYRO_ZOUT_H:{printf("GZ:");break;}
	}
	dat=MPU6050_GetData(I2C2, addr);
	if(dat & 0x8000){
		printf("-%d\r\n",65536-dat);
	}else{
		printf("%d\r\n",dat);
	}
}
*/
int fputc(int ch, FILE *f){
	USART_SendData(USART1, (unsigned int) (ch & 0x1FF));
	while (!(USART1->SR & USART_FLAG_TXE));
	return (ch);
}

void delay(int t){
	int x;
	while(t--){
		x=10000;
		while(x--);
	}
}

void RCC_Configuration(void){
	SystemInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //打开PA端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   //打开PB端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);  //打开USART1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);    //打开I2C2(MPU6050)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);    //打开TIM2(定时器中断采样)
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

	//I2C2 (MPU6050) (PB.10--SCL, PB.11--SDA)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void I2C_Configuration(void){
	I2C_InitTypeDef I2C_InitStructure;

	//I2C配置
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; 
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; 
	I2C_InitStructure.I2C_ClockSpeed = 50000;
	I2C_Init(I2C2, &I2C_InitStructure);
	I2C_Cmd(I2C2, ENABLE);
	I2C_AcknowledgeConfig(I2C2, ENABLE);
}

void TIM_Configuration(void){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;      

	//TIM2(采样频率100Hz)
	TIM_TimeBaseStructure.TIM_Period = 1999; 
	TIM_TimeBaseStructure.TIM_Prescaler = 719; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;  
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE );  
	TIM_Cmd(TIM2,ENABLE);
}

void NVIC_Configuration(void){
	NVIC_InitTypeDef NVIC_InitStructure;

	//TIM2中断配置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
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
	USART_Cmd(USART1,ENABLE); 
	USART_ClearFlag(USART1,USART_FLAG_TC);
}

void TIM2_IRQHandler(void){
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	AX=MPU6050_GetData(I2C2, ACCEL_XOUT_H);
	AY=MPU6050_GetData(I2C2, ACCEL_YOUT_H);
	AZ=MPU6050_GetData(I2C2, ACCEL_ZOUT_H);
	GX=MPU6050_GetData(I2C2, GYRO_XOUT_H);
	GY=MPU6050_GetData(I2C2, GYRO_YOUT_H);
	GZ=MPU6050_GetData(I2C2, GYRO_ZOUT_H);
	switch(mode){
		case 0:{
			CalAngle_Quaternion(GX,GY,GZ,AX,AY,AZ);
			X_angle=Angle_X_Quaternion-X_angle0;
			Y_angle=Angle_Y_Quaternion-Y_angle0;
			break;
		}
		case 1:{
			CalAngle_Kalman(GX,GY,GZ,AX,AY,AZ);
			X_angle=Angle_X_Kalman-X_angle0;
			Y_angle=Angle_Y_Kalman-Y_angle0;
			break;
		}
	}
}
