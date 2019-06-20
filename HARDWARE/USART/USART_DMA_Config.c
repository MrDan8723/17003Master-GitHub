/*********************** (C) COPYRIGHT  2017 MNTC  **************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : USART_DMA_Config.c
  * Version     : V0.1
  * Description : ����DMA����������
  * Date        : 2018��6��15��
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
  * Function    : ʹ��DMA���ܵĴ���1�ĳ�ʼ��
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
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 															//ʹ��DMAʱ��
	
	USART_DeInit(USART1);  																													//��λ����1
	
	if(remap == 0)																																	//��ʹ���ض�������
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 																		//USART1_TX   PA.9
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 															//�����������
		GPIO_Init(GPIOA, &GPIO_InitStructure); 																				//��ʼ��PA9
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;																		//USART1_RX  A.10
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;													//��������
		GPIO_Init(GPIOA, &GPIO_InitStructure);  																			//��ʼ��PA10
	}
	else																																						//ʹ���ض�������
	{
		GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);  															// ������ӳ��		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 																		//USART1_TX   PB6
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 															//�����������
		GPIO_Init(GPIOB, &GPIO_InitStructure); 																				//��ʼ��PB6
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;																			//USART1_RX  PB7
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;													//��������
		GPIO_Init(GPIOB, &GPIO_InitStructure);  																			//��ʼ��PB7
	}
	

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1;												//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 															//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 																//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); 																								//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������
  USART_InitStructure.USART_BaudRate = baud;																			//������
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//�ֳ�Ϊ8λ���ݸ�ʽ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;													//һ��ֹͣλ
  USART_InitStructure.USART_Parity = USART_Parity_No;															//����żУ��λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 								//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); 																			//��ʼ������
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);																	//���������ж�
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);   																	//ʹ�ܴ���1 DMA����
	USART_Cmd(USART1, ENABLE);                    															  	//ʹ�ܴ��� 
 
    //��Ӧ��DMA����
  DMA_DeInit(DMA1_Channel5);   																										//��DMA��ͨ��5�Ĵ�������Ϊȱʡֵ  ����1��Ӧ����DMAͨ��5
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;  									//DMA����USART1���ݼĴ�������ַ
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)rx_buf;  														//DMA�ڴ����ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  														//���ݴ��䷽�򣬴������ȡ���͵��ڴ�
  DMA_InitStructure.DMA_BufferSize = length;  																		//DMAͨ����DMA����Ĵ�С
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  							//�����ַ�Ĵ�������
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  												//�ڴ��ַ�Ĵ�������
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  				//���ݿ��Ϊ8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 								//���ݿ��Ϊ8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  																	//��������������ģʽ
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 													//DMAͨ�� xӵ�������ȼ� 
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  																	//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
  DMA_Init(DMA1_Channel5, &DMA_InitStructure);  																	//����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��

  DMA_Cmd(DMA1_Channel5, ENABLE); 																						    //��ʽ����DMA����
	
}

/**************************************************************************************
  * name        : void USART2_DMA_Config(int baud,int remap,unsigned char *rx_buf,int length)
  * Function    : ʹ��DMA���ܵĴ���2�ĳ�ʼ��
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
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 															//ʹ��DMAʱ��
	
	USART_DeInit(USART2);  																													//��λ����2
	
	if(remap == 0)																																	//��ʹ���ض�������
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); 
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 																		//USART2_TX   PA2
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 															//�����������
		GPIO_Init(GPIOA, &GPIO_InitStructure); 																				//��ʼ��PA2
	 
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;																			//USART2_RX  PA3
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;													//��������
		GPIO_Init(GPIOA, &GPIO_InitStructure);  																			//��ʼ��PA3
	}
	else 
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO , ENABLE); 
		GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);  															// ������ӳ��
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 																		//USART2_TX   PD5
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 															//�����������
		GPIO_Init(GPIOD, &GPIO_InitStructure); 																				//��ʼ��PD5
	 
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;																			//USART2_RX  PD6
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;													//��������
		GPIO_Init(GPIOD, &GPIO_InitStructure);  																			//��ʼ��PD6
	}
	

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1;												//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 															//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 																//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); 																								//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������
  USART_InitStructure.USART_BaudRate = baud;																			//������
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//�ֳ�Ϊ8λ���ݸ�ʽ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;													//һ��ֹͣλ
  USART_InitStructure.USART_Parity = USART_Parity_No;															//����żУ��λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 								//�շ�ģʽ

	USART_Init(USART2, &USART_InitStructure); 																			//��ʼ������
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);																	//���������ж�
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);   																	//ʹ�ܴ���2 DMA����
	USART_Cmd(USART2, ENABLE);                    															  	//ʹ�ܴ��� 
 
    //��Ӧ��DMA����
  DMA_DeInit(DMA1_Channel6);   																										//��DMA��ͨ��5�Ĵ�������Ϊȱʡֵ  ����1��Ӧ����DMAͨ��5
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;  									//DMA����USART2���ݼĴ�������ַ
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)rx_buf;  														//DMA�ڴ����ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  														//���ݴ��䷽�򣬴������ȡ���͵��ڴ�
  DMA_InitStructure.DMA_BufferSize = length;  																		//DMAͨ����DMA����Ĵ�С
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  							//�����ַ�Ĵ�������
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  												//�ڴ��ַ�Ĵ�������
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  				//���ݿ��Ϊ8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 								//���ݿ��Ϊ8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  																//��������������ģʽ
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 													//DMAͨ�� xӵ�������ȼ� 
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  																	//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
  DMA_Init(DMA1_Channel6, &DMA_InitStructure);  																	//����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��

  DMA_Cmd(DMA1_Channel6, ENABLE); 																						    //��ʽ����DMA����
	
}

/**************** (C) COPYRIGHT 2017 MNTC *****END OF FILE*******************/