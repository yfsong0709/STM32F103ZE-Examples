//---------------------------------------NRF905 数据交换端口------------------------------------------------
// TXEN = PB.1
// TRX_CE = PA.7
// MISO = PA.3
// MOSI = PA.2														 	
// SCK = PA.1
// CSN = PA.0
// PWR = PB.2
// AM = PA.5
// DR = PA.4;
// CD = PA.6;
//----------------------------------------------------------------------------------------------------------
#define NRF905_BYTE_BIT0	0x01
#define NRF905_BYTE_BIT1	0x02
#define NRF905_BYTE_BIT2	0x04
#define NRF905_BYTE_BIT3	0x08
#define NRF905_BYTE_BIT4	0x10
#define NRF905_BYTE_BIT5	0x20
#define NRF905_BYTE_BIT6	0x40
#define NRF905_BYTE_BIT7	0x80
//----------------------------------------------------------------------------------------------------------
unsigned char NRF905_DATA_BUF;
#define NRF905_DATA7	((NRF905_DATA_BUF&NRF905_BYTE_BIT7) != 0)
#define NRF905_DATA0    ((NRF905_DATA_BUF&NRF905_BYTE_BIT0) != 0)
//-------------------------------------------nrf905控制指令-------------------------------------------------
#define NRF905_WC		0x00
#define NRF905_RC		0x10
#define NRF905_WTP		0x20
#define NRF905_RTP		0x21
#define NRF905_WTA		0x22
#define NRF905_RTA		0x23
#define NRF905_RRP		0x24
//------------------------------------------------NRF905寄存器配置------------------------------------------
unsigned char NRF905_RFConf[11]={
  0x00,                             //配置命令
  0x4c,                             //CH_NO,配置频段在430MHZ
  0x0c,                             //输出功率为10db,不重发，节电为正常模式
  0x44,                             //地址宽度设置，为4字节
  0x04,0x04,                        //接收发送有效数据长度为32字节
  0x11,0x22,0x33,0x44,              //接收地址
  0x58                              //CRC充许，8位CRC校验，外部时钟信号不使能，16M晶振
};
unsigned char NRF905_TxAddress[4]={0x11,0x22,0x33,0x44};
unsigned char NRF905_data[4]={0x00,0x00,0x00,0x00};
//------------------------------------------------延时-------------------------------------------------------
static void NRF905_Delay(unsigned char n)
{
	unsigned int i;
	while(n--)
	for(i=0;i<80;i++);
}
//-------------------------------------------------SPI写函数--------------------------------------------------
void NRF905_SpiWrite(unsigned char send)
{
	unsigned char i;
	NRF905_DATA_BUF=send;
	for (i=0;i<8;i++)
	{
		if (NRF905_DATA7)	
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);
		}
		else
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET);
		}
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);
		NRF905_DATA_BUF=NRF905_DATA_BUF<<1;
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);
	}
}
//---------------------------------------------------SPI读函数-----------------------------------------------------
unsigned char NRF905_SpiRead(void)
{
	unsigned char j;
	for (j=0;j<8;j++)
	{
    NRF905_DATA_BUF=NRF905_DATA_BUF<<1;
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))	
		{
			NRF905_DATA_BUF|=NRF905_BYTE_BIT0;
		}
		else
		{
			NRF905_DATA_BUF&=~NRF905_BYTE_BIT0;
		}
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);
	 }
	 return NRF905_DATA_BUF;
}
//------------------------------------------------------初始化nRF905-------------------------------------------
void NRF905_Init(void)
{
	unsigned char i;
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);						
	GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);						
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);						
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);						
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);						
	GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_SET);					
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_RESET);					
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_RESET);	
	NRF905_Delay(1);
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);									
	for (i=0;i<11;i++)	
	{
	  NRF905_SpiWrite(NRF905_RFConf[i]);
	}
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
}
//-----------------------------------------------------发送数据打包----------------------------------------------
void NRF905_Tx(unsigned char Buf0,unsigned char Buf1,unsigned char Buf2,unsigned char Buf3)
{
	unsigned char i;
	unsigned char NRF905_TxBuf[4]={0x08,0x08,0x08,0x08};
	NRF905_TxBuf[0]=Buf0;
	NRF905_TxBuf[1]=Buf1;
	NRF905_TxBuf[2]=Buf2;
	NRF905_TxBuf[3]=Buf3;
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_RESET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_SET);
	NRF905_Delay(1); 
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
	NRF905_SpiWrite(NRF905_WTP);
	for (i=0;i<4;i++)
	{
		NRF905_SpiWrite(NRF905_TxBuf[i]);		
	}
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
	NRF905_Delay(1);						
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);						
	NRF905_SpiWrite(NRF905_WTA);				
	for (i=0;i<4;i++)			
	{
		NRF905_SpiWrite(NRF905_TxAddress[i]);
	}
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);						
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_SET);					
	NRF905_Delay(1);					
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_RESET);					
}
//----------------------------------------------------读NRF905接收数据-------------------------------------------------
void NRF905_Rx(void)						
{
	unsigned char i;
	NRF905_Delay(1);
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_RESET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_SET);
	NRF905_Delay(100);
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_RESET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);						
	NRF905_Delay(1);
	NRF905_SpiWrite(NRF905_RRP);
	for (i = 0 ;i < 4 ;i++)
	{ 
		NRF905_data[i]=NRF905_SpiRead();		    
	}
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
	NRF905_Delay(10);
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_SET);							
}
