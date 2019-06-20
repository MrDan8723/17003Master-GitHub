/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : USART_Configuration.c
  * Version     : V4.0
  * Description : 康录项目主控板串口配置程序，硬件平台PCB V0.5
  * Date        : 2016-08-11
  * Author      : XYD
  * Check       :  
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "USART_Configuration.h"
#include "stm32f10x_usart.h"


/*******************************************************************************
  * name        : USART_SendBuf(USART_TypeDef *com, char *buf, int buf_len)
  * Function    : 将指定缓冲区中的指定个字节数据发送至指定的串口
  * Parameters  : USART_TypeDef *com-- 指向目标串口，宏定义值由stm32f10x.h定义  USART1--串口1 USART2--串口2  USART3--串口3 UART4--串口4 UART5--串口5
  * Parameters  : char *buf-- 指定的发送缓冲区地址
	* Parameters  : int buf_len-- 指定的发送的字节长度
	* Returns     : Null
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void USART_SendBuf(USART_TypeDef *com, uint16_t *buf, int buf_len)
{
	int i=0;																										//循环控制变量
	for(i=0; i < buf_len; i++)
	{
		USART_SendData(com, buf[i]);															//发送一个字节
		while(USART_GetFlagStatus(com,USART_FLAG_TXE)==RESET);		//等待发送完成
	}
}

/*******************************************************************************
  * name        : Usart1_Configuration(void)
  * Function    : usart1 Configuration?
  * Parameters  : int baud--波特率
  * Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: PA9= TX PA10=RX
*******************************************************************************/
void Usart1_Configuration(int baud)
{
  USART_InitTypeDef USART_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO, ENABLE);  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  
   
  /******串口1的配置******/
  GPIOA->CRH&=0xFFFFF00F;  																												// PA9、PA10引脚清零
  GPIOA->CRH|=0x000004B0;  																												// PA9为复用推挽输出50MHZ(TXD),PA10为浮空输入(RXD)
  
  USART_InitStructure.USART_BaudRate = baud;																			//波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//数据位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;													//停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;															//校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;									//串口模式 
  USART_Init(USART1,&USART_InitStructure);																				//配置串口
	
  USART_ITConfig(USART1, USART_IT_TXE, DISABLE);																	//失能发送中断
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);																	//使能接收中断
  USART_Cmd(USART1, ENABLE);																											//使能串口
}

/*******************************************************************************
  * name        : Usart1_Nvic_Configuration(void)
  * Function    : Usart1 Nvic Configuration?
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void Usart1_Nvic_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;                                           
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x08000000);             		          // 设置中断向量表基地址0x08000000 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                               // 2位抢占优先级，2位响应优先级
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;                             // 开启串口1的中断向量

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;                     // 抢占优先级为0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                            // 响应优先级为0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                               // 使能中断通道

  NVIC_Init(&NVIC_InitStructure);                                               // 串口1中断初始化  
}

/*******************************************************************************
  * name        : Usart2_Configuration(void)
  * Function    : usart2 Configuration
  * Parameters  : int baud--波特率
  * Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: PD5= TX  PD6= RX
*******************************************************************************/
void Usart2_Configuration(int baud)
{
  USART_InitTypeDef USART_InitStructure;
//  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO,ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);  																// 串口重映射
   
  /******串口2的配置******/
  GPIOD->CRL&=0xF00FFFFF;  																												// PD5、PD6引脚清零
  GPIOD->CRL|=0x04B00000;  																												// PD5为复用推挽输出50MHZ(TXD),PD6为浮空输入(RXD)
//  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;																//复用推挽
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	/* Configure USART2 Rx (PA3) as input Up */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;																	//上拉输入
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

  
  USART_InitStructure.USART_BaudRate = baud;																			//波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//数据位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;													//停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;															//校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;									//串口模式 
  USART_Init(USART2,&USART_InitStructure);																				//配置串口
	
  USART_ITConfig(USART2, USART_IT_TXE, DISABLE);																	//失能发送中断
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);																	//使能接收中断
  USART_Cmd(USART2, ENABLE);																											//使能串口
}

/*******************************************************************************
  * name        : Usart2_Nvic_Configuration(void)
  * Function    : Usart2 Nvic Configuration
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void Usart2_Nvic_Configuration(void)
{
  /******串口2的中断配置******/
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x08000000);                  // 设置中断向量表基地址0x08000000  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                       // 2位抢占优先级，2位响应优先级
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;                     // 开启串口2的中断向量

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;             // 抢占优先级为1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                    // 响应优先级为0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                       // 使能中断通道

  NVIC_Init(&NVIC_InitStructure);                                       // 串口2中断初始化 
}

/*******************************************************************************
  * name        : Uart3_Configuration(void)
  * Function    : uart3 Configuration
  * Parameters  : int baud--波特率
  * Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: USART3_TX= PB10   USART3_RX= PB11  重映射后： USART3_TX= PD8   USART3_RX= PD9
*******************************************************************************/
void Usart3_Configuration(int baud)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 
  GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);  																// 串口重映射
	
	/* Configure USART3 Tx (PD.8) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;																			//复用推挽
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Configure USART3 Rx (PD.9) as input Up */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;																				//上拉输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = baud;  
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  
	USART_InitStructure.USART_Parity = USART_Parity_No;  
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  
	USART_Init(USART3, &USART_InitStructure);  
	
	USART_ITConfig(USART3, USART_IT_TXE, DISABLE);																	//失能发送中断
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);																	//使能接收中断
  USART_Cmd(USART3, ENABLE);

}

/*******************************************************************************
  * name        : Usart3_Nvic_Configuration(void)
  * Function    : Usart3 Nvic Configuration
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void Usart3_Nvic_Configuration(void)
{
  /******串口3的中断配置******/
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x08000000);                  // 设置中断向量表基地址0x08000000  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                       // 2位抢占优先级，2位响应优先级
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;                     // 开启串口3的中断向量

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;             // 抢占优先级为1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                    // 响应优先级为0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                       // 使能中断通道

  NVIC_Init(&NVIC_InitStructure);                                       // 串口2中断初始化 
}

/*******************************************************************************
  * name        : Uart4_Configuration(void)
  * Function    : uart4 Configuration
  * Parameters  : int baud--波特率
  * Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: PC10= Tx PC11= Rx
*******************************************************************************/
void Uart4_Configuration(int baud)
{
  USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE); 
   
  /******串口4的配置******/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;																	// PC10= Tx PC11= Rx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		 																	// PD5为复用推挽输出50MHZ(TXD),PD6为浮空输入(RXD)
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
  
  USART_InitStructure.USART_BaudRate = baud;																			//波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//数据位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;													//停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;															//校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;									//串口模式 
  USART_Init(UART4,&USART_InitStructure);																					//配置串口
	
  USART_ITConfig(UART4, USART_IT_TXE, DISABLE);																		//失能发送中断
  USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);																		//使能接收中断
  USART_Cmd(UART4, ENABLE);																												//使能串口
}

/*******************************************************************************
  * name        : Uart4_Nvic_Configuration(void)
  * Function    : Uart4 Nvic_Configuration
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void Uart4_Nvic_Configuration(void)
{
  /******串口4中断配置******/
  NVIC_InitTypeDef NVIC_InitStructure;                                           
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x08000000);                          // 设置中断向量表基地址0x08000000 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                               // 2位抢占优先级，2位响应优先级
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;                            	// 开启串口4的中断向量

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;                     // 抢占优先级为2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                            // 响应优先级为0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                               // 使能中断通道

  NVIC_Init(&NVIC_InitStructure);                                               // 串口1中断初始化  
}

/*******************************************************************************
  * name        : Uart5_Configuration(void)
  * Function    : uart5 Configuration
  * Parameters  : int baud--波特率
  * Returns     : Null
  * Author      : ZQChen
  * Check       : 
	* Note				: PC12= Tx PD2= Rx
*******************************************************************************/
void Uart5_Configuration(int baud)
{
  USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE); 
   
  /******串口4的配置******/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;																	// PC10= Tx PC11= Rx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;		 																	// PD5为复用推挽输出50MHZ(TXD),PD6为浮空输入(RXD)
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
  
  USART_InitStructure.USART_BaudRate = baud;																			//波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//数据位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;													//停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;															//校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;									//串口模式 
  USART_Init(UART5,&USART_InitStructure);																					//配置串口
	
  USART_ITConfig(UART5, USART_IT_TXE, DISABLE);																		//失能发送中断
  USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);																		//使能接收中断
  USART_Cmd(UART5, ENABLE);																												//使能串口
}

/*******************************************************************************
  * name        : Uart5_Nvic_Configuration(void)
  * Function    : Uart5 Nvic_Configuration
  * Parameters  : Null
  * Returns     : Null
  * Author      : ZQChen
  * Check       : 
*******************************************************************************/
void Uart5_Nvic_Configuration(void)
{
  /******串口4中断配置******/
  NVIC_InitTypeDef NVIC_InitStructure;                                           
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x08000000);                          // 设置中断向量表基地址0x08000000 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                               // 2位抢占优先级，2位响应优先级
  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;                            	// 开启串口4的中断向量

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;                     // 抢占优先级为2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                            // 响应优先级为0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                               // 使能中断通道

  NVIC_Init(&NVIC_InitStructure);                                               // 串口1中断初始化  
}



/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE****************/


