#include "stm32f10x.h"
#include "NRF905.h"
#include "I2C_MPU6050.h"
#include "kalman.h"

#define PID_P 2			//定义比例系数
#define PID_I 0.1f		//定义积分系数
#define PID_D 0			//定义微分系数

void delay(int t);
void RCC_Configuration(void); 
void GPIO_Configuration(void);
void TIM_Configuration(void);
void I2C_Configuration(void);
void NVIC_Configuration(void);
void Moto_Init(void);
void Moto_Set(u8 sp1,u8 sp2,u8 sp3,u8 sp4);
void Obj_Set_NRF905(void);
void Speed_Set_PID(void);

u8 obj1=100,obj2=100,obj3=100,obj4=100;               // PID稳定目标
u8 speed1=0,speed2=0,speed3=0,speed4=0;               // PID输出控制电机转速
float X_angle = 0.0f, Y_angle = 0.0f;                 // 姿态角
int AX,AY,AZ,GX,GY,GZ;
float e1_X=0,e1_Y=0,e2_X=0,e2_Y=0,e3_X=0,e3_Y=0;
float duk_X=0,duk_Y=0,uk_X=0,uk_Y=0;						 

int main(void){	
	RCC_Configuration();
	GPIO_Configuration();
	TIM_Configuration();
	I2C_Configuration();
	NVIC_Configuration();
	MPU6050_Init(I2C2);
	NRF905_Init();
	TIM_Cmd(TIM4,ENABLE);
	Moto_Init();
	TIM_Cmd(TIM2,ENABLE);	
	while(1);
}

void delay(int t){
	u16 x=0;
	while(t--){
		x=12000;
		while(x--);
	}
}

void Moto_Init(void){
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
}

void Moto_Set(u8 sp1,u8 sp2,u8 sp3,u8 sp4){
	TIM_SetCompare1(TIM4, sp1);
	TIM_SetCompare2(TIM4, sp2);
	TIM_SetCompare3(TIM4, sp3);
	TIM_SetCompare4(TIM4, sp4);
}

void Obj_Set_NRF905(void){
	NRF905_data[0]=0x00;
	NRF905_Rx();
	if(NRF905_data[0]==1){
		obj1+=2;
		obj2+=2;
		obj3+=2;
		obj4+=2;
		if(obj1>200)obj1=200;
		if(obj2>200)obj2=200;
		if(obj3>200)obj3=200;
		if(obj4>200)obj4=200;
	}
	if(NRF905_data[0]==2){
		obj1-=2;
		obj2-=2;
		obj3-=2;
		obj4-=2;
		if(obj1<100)obj1=100;
		if(obj2<100)obj2=100;
		if(obj3<100)obj3=100;
		if(obj4<100)obj4=100;
	}
	speed1=obj1;
	speed2=obj2;
	speed3=obj3;
	speed4=obj4;
}

void Speed_Set_PID(void){
	e1_X=0.0f-X_angle;
	e1_Y=0.0f-Y_angle;
	duk_X=PID_P*(e1_X-e2_X)+PID_I*e1_X+PID_D*(e1_X-e2_X*2+e3_X);
	duk_Y=PID_P*(e1_Y-e2_Y)+PID_I*e1_Y+PID_D*(e1_Y-e2_Y*2+e3_Y);
	uk_X+=duk_X;
	uk_Y+=duk_Y;
	if(uk_X>0){
		speed1=obj1+uk_X;
	}else{
		speed3=obj3-uk_X;
	}
	if(uk_Y>0){
		speed2=obj2+uk_Y;
	}else{
		speed4=obj4-uk_Y;
	}
	e3_X=e2_X;
	e3_Y=e2_Y;
	e2_X=e1_X;
	e2_Y=e1_Y;
	if(speed1<100)speed1=100;
	if(speed2<100)speed2=100;
	if(speed3<100)speed3=100;
	if(speed4<100)speed4=100;
	if(speed1>200)speed1=200;
	if(speed2>200)speed2=200;
	if(speed3>200)speed3=200;
	if(speed4>200)speed4=200;
}

void RCC_Configuration(void){
	delay(1000); 
	SystemInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);    //打开TIM4(PWM)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);    //打开I2C2(MPU6050)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);    //打开TIM2(定时器中断采样)
}

void GPIO_Configuration(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;

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

	//PWM(PB.6--X+, PB.7--X-, PB.8--Y+, PB.9--Y-)
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

	//MPU6050(PB.10--SCL, PB.11--SDA)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void I2C_Configuration(void){
	I2C_InitTypeDef I2C_InitStructure;

	//I2C2配置(MPU6050)
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
    TIM_OCInitTypeDef TIM_OCInitStructure;

	//TIM4配置(PWM)
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
	TIM_Cmd(TIM4,DISABLE);
	
	//TIM2配置(采样频率50Hz)
	TIM_TimeBaseStructure.TIM_Period = 1999; 
	TIM_TimeBaseStructure.TIM_Prescaler = 719; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;  
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  
	TIM_Cmd(TIM2,DISABLE);   
}

void NVIC_Configuration(void){
	NVIC_InitTypeDef NVIC_InitStructure;

	//TIM2中断配置(50Hz中断采样)
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM2_IRQHandler(void){
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	AX=MPU6050_GetData(I2C2, ACCEL_XOUT_H);
	AY=MPU6050_GetData(I2C2, ACCEL_YOUT_H);
	AZ=MPU6050_GetData(I2C2, ACCEL_ZOUT_H);
	GX=MPU6050_GetData(I2C2, GYRO_XOUT_H);
	GY=MPU6050_GetData(I2C2, GYRO_YOUT_H);
	GZ=MPU6050_GetData(I2C2, GYRO_ZOUT_H);

	CalAngle_Kalman(GX,GY,GZ,AX,AY,AZ);
	X_angle=Angle_X_Kalman;
	Y_angle=Angle_Y_Kalman;

	Obj_Set_NRF905();
	//Speed_Set_PID();
	Moto_Set(speed1,speed2,speed3,speed4);
}
