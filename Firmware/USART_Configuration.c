/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : USART_Configuration.c
  * Version     : V4.0
  * Description : ��¼��Ŀ���ذ崮�����ó���Ӳ��ƽ̨PCB V0.5
  * Date        : 2016-08-11
  * Author      : XYD
  * Check       :  
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "USART_Configuration.h"
#include "stm32f10x_usart.h"


/*******************************************************************************
  * name        : USART_SendBuf(USART_TypeDef *com, char *buf, int buf_len)
  * Function    : ��ָ���������е�ָ�����ֽ����ݷ�����ָ���Ĵ���
  * Parameters  : USART_TypeDef *com-- ָ��Ŀ�괮�ڣ��궨��ֵ��stm32f10x.h����  USART1--����1 USART2--����2  USART3--����3 UART4--����4 UART5--����5
  * Parameters  : char *buf-- ָ���ķ��ͻ�������ַ
	* Parameters  : int buf_len-- ָ���ķ��͵��ֽڳ���
	* Returns     : Null
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void USART_SendBuf(USART_TypeDef *com, uint16_t *buf, int buf_len)
{
	int i=0;																										//ѭ�����Ʊ���
	for(i=0; i < buf_len; i++)
	{
		USART_SendData(com, buf[i]);															//����һ���ֽ�
		while(USART_GetFlagStatus(com,USART_FLAG_TXE)==RESET);		//�ȴ��������
	}
}

/*******************************************************************************
  * name        : Usart1_Configuration(void)
  * Function    : usart1 Configuration?
  * Parameters  : int baud--������
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
   
  /******����1������******/
  GPIOA->CRH&=0xFFFFF00F;  																												// PA9��PA10��������
  GPIOA->CRH|=0x000004B0;  																												// PA9Ϊ�����������50MHZ(TXD),PA10Ϊ��������(RXD)
  
  USART_InitStructure.USART_BaudRate = baud;																			//������
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//����λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;													//ֹͣλ
  USART_InitStructure.USART_Parity = USART_Parity_No;															//У��λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;									//����ģʽ 
  USART_Init(USART1,&USART_InitStructure);																				//���ô���
	
  USART_ITConfig(USART1, USART_IT_TXE, DISABLE);																	//ʧ�ܷ����ж�
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);																	//ʹ�ܽ����ж�
  USART_Cmd(USART1, ENABLE);																											//ʹ�ܴ���
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
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x08000000);             		          // �����ж����������ַ0x08000000 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                               // 2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;                             // ��������1���ж�����

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;                     // ��ռ���ȼ�Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                            // ��Ӧ���ȼ�Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                               // ʹ���ж�ͨ��

  NVIC_Init(&NVIC_InitStructure);                                               // ����1�жϳ�ʼ��  
}

/*******************************************************************************
  * name        : Usart2_Configuration(void)
  * Function    : usart2 Configuration
  * Parameters  : int baud--������
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
  GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);  																// ������ӳ��
   
  /******����2������******/
  GPIOD->CRL&=0xF00FFFFF;  																												// PD5��PD6��������
  GPIOD->CRL|=0x04B00000;  																												// PD5Ϊ�����������50MHZ(TXD),PD6Ϊ��������(RXD)
//  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;																//��������
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	/* Configure USART2 Rx (PA3) as input Up */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;																	//��������
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

  
  USART_InitStructure.USART_BaudRate = baud;																			//������
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//����λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;													//ֹͣλ
  USART_InitStructure.USART_Parity = USART_Parity_No;															//У��λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;									//����ģʽ 
  USART_Init(USART2,&USART_InitStructure);																				//���ô���
	
  USART_ITConfig(USART2, USART_IT_TXE, DISABLE);																	//ʧ�ܷ����ж�
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);																	//ʹ�ܽ����ж�
  USART_Cmd(USART2, ENABLE);																											//ʹ�ܴ���
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
  /******����2���ж�����******/
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x08000000);                  // �����ж����������ַ0x08000000  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                       // 2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;                     // ��������2���ж�����

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;             // ��ռ���ȼ�Ϊ1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                    // ��Ӧ���ȼ�Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                       // ʹ���ж�ͨ��

  NVIC_Init(&NVIC_InitStructure);                                       // ����2�жϳ�ʼ�� 
}

/*******************************************************************************
  * name        : Uart3_Configuration(void)
  * Function    : uart3 Configuration
  * Parameters  : int baud--������
  * Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: USART3_TX= PB10   USART3_RX= PB11  ��ӳ��� USART3_TX= PD8   USART3_RX= PD9
*******************************************************************************/
void Usart3_Configuration(int baud)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 
  GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);  																// ������ӳ��
	
	/* Configure USART3 Tx (PD.8) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;																			//��������
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Configure USART3 Rx (PD.9) as input Up */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;																				//��������
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = baud;  
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  
	USART_InitStructure.USART_Parity = USART_Parity_No;  
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  
	USART_Init(USART3, &USART_InitStructure);  
	
	USART_ITConfig(USART3, USART_IT_TXE, DISABLE);																	//ʧ�ܷ����ж�
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);																	//ʹ�ܽ����ж�
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
  /******����3���ж�����******/
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x08000000);                  // �����ж����������ַ0x08000000  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                       // 2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;                     // ��������3���ж�����

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;             // ��ռ���ȼ�Ϊ1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                    // ��Ӧ���ȼ�Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                       // ʹ���ж�ͨ��

  NVIC_Init(&NVIC_InitStructure);                                       // ����2�жϳ�ʼ�� 
}

/*******************************************************************************
  * name        : Uart4_Configuration(void)
  * Function    : uart4 Configuration
  * Parameters  : int baud--������
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
   
  /******����4������******/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;																	// PC10= Tx PC11= Rx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		 																	// PD5Ϊ�����������50MHZ(TXD),PD6Ϊ��������(RXD)
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
  
  USART_InitStructure.USART_BaudRate = baud;																			//������
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//����λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;													//ֹͣλ
  USART_InitStructure.USART_Parity = USART_Parity_No;															//У��λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;									//����ģʽ 
  USART_Init(UART4,&USART_InitStructure);																					//���ô���
	
  USART_ITConfig(UART4, USART_IT_TXE, DISABLE);																		//ʧ�ܷ����ж�
  USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);																		//ʹ�ܽ����ж�
  USART_Cmd(UART4, ENABLE);																												//ʹ�ܴ���
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
  /******����4�ж�����******/
  NVIC_InitTypeDef NVIC_InitStructure;                                           
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x08000000);                          // �����ж����������ַ0x08000000 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                               // 2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;                            	// ��������4���ж�����

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;                     // ��ռ���ȼ�Ϊ2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                            // ��Ӧ���ȼ�Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                               // ʹ���ж�ͨ��

  NVIC_Init(&NVIC_InitStructure);                                               // ����1�жϳ�ʼ��  
}

/*******************************************************************************
  * name        : Uart5_Configuration(void)
  * Function    : uart5 Configuration
  * Parameters  : int baud--������
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
   
  /******����4������******/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;																	// PC10= Tx PC11= Rx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;		 																	// PD5Ϊ�����������50MHZ(TXD),PD6Ϊ��������(RXD)
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
  
  USART_InitStructure.USART_BaudRate = baud;																			//������
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//����λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;													//ֹͣλ
  USART_InitStructure.USART_Parity = USART_Parity_No;															//У��λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;									//����ģʽ 
  USART_Init(UART5,&USART_InitStructure);																					//���ô���
	
  USART_ITConfig(UART5, USART_IT_TXE, DISABLE);																		//ʧ�ܷ����ж�
  USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);																		//ʹ�ܽ����ж�
  USART_Cmd(UART5, ENABLE);																												//ʹ�ܴ���
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
  /******����4�ж�����******/
  NVIC_InitTypeDef NVIC_InitStructure;                                           
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x08000000);                          // �����ж����������ַ0x08000000 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                               // 2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;                            	// ��������4���ж�����

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;                     // ��ռ���ȼ�Ϊ2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                            // ��Ӧ���ȼ�Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                               // ʹ���ж�ͨ��

  NVIC_Init(&NVIC_InitStructure);                                               // ����1�жϳ�ʼ��  
}



/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE****************/


