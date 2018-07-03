#include "I2C_MPU6050.h"

void I2C_ByteWrite(I2C_TypeDef* I2Cx, u8 REG_Address, u8 REG_data){
	I2C_GenerateSTART(I2Cx,ENABLE);
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2Cx,SlaveAddress,I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C2,REG_Address);
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(I2C2,REG_data);
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTOP(I2Cx,ENABLE);
}

u8 I2C_ByteRead(I2C_TypeDef* I2Cx, u8 REG_Address){
	u8 REG_data;
	while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2Cx,ENABLE);//起始信号
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2Cx,SlaveAddress,I2C_Direction_Transmitter);//发送设备地址+写信号
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//
	I2C_Cmd(I2Cx,ENABLE);	
	I2C_SendData(I2Cx,REG_Address);//发送存储单元地址，从0开始
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTART(I2Cx,ENABLE);//起始信号
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2Cx,SlaveAddress,I2C_Direction_Receiver);//发送设备地址+读信号
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	I2C_AcknowledgeConfig(I2Cx,DISABLE);
	I2C_GenerateSTOP(I2Cx,ENABLE);
	while(!(I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_RECEIVED)));
	REG_data=I2C_ReceiveData(I2Cx);//读出寄存器数据
	return REG_data;
}

void MPU6050_Init(I2C_TypeDef* I2Cx){
	I2C_ByteWrite(I2Cx,PWR_MGMT_1,0x00);//解除休眠状态
	I2C_ByteWrite(I2Cx,SMPLRT_DIV,0x07);
	I2C_ByteWrite(I2Cx,CONFIG,0x06);
	I2C_ByteWrite(I2Cx,GYRO_CONFIG,0x18);
	I2C_ByteWrite(I2Cx,ACCEL_CONFIG,0x01);
}

int MPU6050_GetData(I2C_TypeDef* I2Cx, unsigned char REG_Address){
	char H,L;
	H=I2C_ByteRead(I2Cx,REG_Address);
	L=I2C_ByteRead(I2Cx,REG_Address+1);
	return (H<<8)+L;   //合成数据
}
