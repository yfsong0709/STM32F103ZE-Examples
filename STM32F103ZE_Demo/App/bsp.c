/**
  ********************************  STM32F10x  *********************************
  * @文件名     ： bsp.c
  * @作者       ： strongerHuang
  * @库版本     ： V3.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2016年10月18日
  * @摘要       ： BSP板级支持包源文件
  ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
  2016-10-18 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
/* 包含的头文件 --------------------------------------------------------------*/
#include "bsp.h"


/************************************************
函数名称 ： RCC_Configuration
功    能 ： 时钟配置
参    数 ： 无
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void RCC_Configuration(void)
{
  /* 使能APB2时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB  | 
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD  | 
                         RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF  | 
                         RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO , ENABLE);

#if 0
  /* 使能APB1时钟 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* 使能AHB时钟 */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
#endif
}

/************************************************
函数名称 ： GPIO_Basic_Configuration
功    能 ： 基本输入输出引脚配置
参    数 ： 无
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void GPIO_Basic_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin =  PIN_LED;                            //引脚
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                  //频率(50M)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                   //输出类型(推挽式输出)
  GPIO_Init(PORT_LED, &GPIO_InitStructure);

#if 0
  /* 时钟输出 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  RCC_MCOConfig(RCC_MCO_SYSCLK);
#endif
}

/************************************************
函数名称 ： BSP_Initializes
功    能 ： 底层初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void BSP_Initializes(void)
{
  RCC_Configuration();                                               //时钟配置
  GPIO_Basic_Configuration();                                        //IO配置
}


/**** Copyright (C)2016 strongerHuang. All Rights Reserved **** END OF FILE ****/
