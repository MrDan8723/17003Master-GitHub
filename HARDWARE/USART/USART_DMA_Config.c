/*********************** (C) COPYRIGHT  2017 MNTC  **************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : USART_DMA_Config.c
  * Version     : V0.1
  * Description : 串口DMA驱动动程序
  * Date        : 2018年6月15日
  * Author      : XYD
  * Check       :  
*******************************************************************************/



/*********************File include*********************/
#include "USART_DMA_Config.h"                 

/******************Macro definition********************/

/*************External variable declaration************/

/***************Global variable definition*************/


/**************************************************************************************
  * name        : void USART1_DMA_Config(int baud,int remap,unsigned char *rx_buf,int length)
  * Function    : 使用DMA功能的串口1的初始化
  * Parameters  : Null
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void USART1_DMA_Config(int baud,int remap,unsigned char *rx_buf,int length)  
{
  USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO, ENABLE);  
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 															//使能DMA时钟
	
	USART_DeInit(USART1);  																													//复位串口1
	
	if(remap == 0)																																	//不使用重定向引脚
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 																		//USART1_TX   PA.9
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 															//复用推挽输出
		GPIO_Init(GPIOA, &GPIO_InitStructure); 																				//初始化PA9
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;																		//USART1_RX  A.10
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;													//浮空输入
		GPIO_Init(GPIOA, &GPIO_InitStructure);  																			//初始化PA10
	}
	else																																						//使用重定向引脚
	{
		GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);  															// 串口重映射		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 																		//USART1_TX   PB6
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 															//复用推挽输出
		GPIO_Init(GPIOB, &GPIO_InitStructure); 																				//初始化PB6
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;																			//USART1_RX  PB7
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;													//浮空输入
		GPIO_Init(GPIOB, &GPIO_InitStructure);  																			//初始化PB7
	}
	

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1;												//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 															//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 																//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); 																								//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置
  USART_InitStructure.USART_BaudRate = baud;																			//波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//字长为8位数据格式
  USART_InitStructure.USART_StopBits = USART_StopBits_1;													//一个停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;															//无奇偶校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 								//收发模式

	USART_Init(USART1, &USART_InitStructure); 																			//初始化串口
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);																	//开启空闲中断
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);   																	//使能串口1 DMA接收
	USART_Cmd(USART1, ENABLE);                    															  	//使能串口 
 
    //相应的DMA配置
  DMA_DeInit(DMA1_Channel5);   																										//将DMA的通道5寄存器重设为缺省值  串口1对应的是DMA通道5
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;  									//DMA外设USART1数据寄存器基地址
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)rx_buf;  														//DMA内存基地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  														//数据传输方向，从外设读取发送到内存
  DMA_InitStructure.DMA_BufferSize = length;  																		//DMA通道的DMA缓存的大小
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  							//外设地址寄存器不变
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  												//内存地址寄存器递增
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  				//数据宽度为8位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 								//数据宽度为8位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  																	//工作在正常缓存模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 													//DMA通道 x拥有中优先级 
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  																	//DMA通道x没有设置为内存到内存传输
  DMA_Init(DMA1_Channel5, &DMA_InitStructure);  																	//根据DMA_InitStruct中指定的参数初始化DMA的通道

  DMA_Cmd(DMA1_Channel5, ENABLE); 																						    //正式驱动DMA传输
	
}

/**************************************************************************************
  * name        : void USART2_DMA_Config(int baud,int remap,unsigned char *rx_buf,int length)
  * Function    : 使用DMA功能的串口2的初始化
  * Parameters  : Null
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void USART2_DMA_Config(int baud,int remap,unsigned char *rx_buf,int length)  
{
  USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 															//使能DMA时钟
	
	USART_DeInit(USART2);  																													//复位串口2
	
	if(remap == 0)																																	//不使用重定向引脚
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); 
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 																		//USART2_TX   PA2
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 															//复用推挽输出
		GPIO_Init(GPIOA, &GPIO_InitStructure); 																				//初始化PA2
	 
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;																			//USART2_RX  PA3
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;													//浮空输入
		GPIO_Init(GPIOA, &GPIO_InitStructure);  																			//初始化PA3
	}
	else 
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO , ENABLE); 
		GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);  															// 串口重映射
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 																		//USART2_TX   PD5
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 															//复用推挽输出
		GPIO_Init(GPIOD, &GPIO_InitStructure); 																				//初始化PD5
	 
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;																			//USART2_RX  PD6
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;													//浮空输入
		GPIO_Init(GPIOD, &GPIO_InitStructure);  																			//初始化PD6
	}
	

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1;												//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 															//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 																//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); 																								//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置
  USART_InitStructure.USART_BaudRate = baud;																			//波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//字长为8位数据格式
  USART_InitStructure.USART_StopBits = USART_StopBits_1;													//一个停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;															//无奇偶校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 								//收发模式

	USART_Init(USART2, &USART_InitStructure); 																			//初始化串口
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);																	//开启空闲中断
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);   																	//使能串口2 DMA接收
	USART_Cmd(USART2, ENABLE);                    															  	//使能串口 
 
    //相应的DMA配置
  DMA_DeInit(DMA1_Channel6);   																										//将DMA的通道5寄存器重设为缺省值  串口1对应的是DMA通道5
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;  									//DMA外设USART2数据寄存器基地址
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)rx_buf;  														//DMA内存基地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  														//数据传输方向，从外设读取发送到内存
  DMA_InitStructure.DMA_BufferSize = length;  																		//DMA通道的DMA缓存的大小
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  							//外设地址寄存器不变
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  												//内存地址寄存器递增
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  				//数据宽度为8位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 								//数据宽度为8位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  																//工作在正常缓存模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 													//DMA通道 x拥有中优先级 
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  																	//DMA通道x没有设置为内存到内存传输
  DMA_Init(DMA1_Channel6, &DMA_InitStructure);  																	//根据DMA_InitStruct中指定的参数初始化DMA的通道

  DMA_Cmd(DMA1_Channel6, ENABLE); 																						    //正式驱动DMA传输
	
}

/**************** (C) COPYRIGHT 2017 MNTC *****END OF FILE*******************/