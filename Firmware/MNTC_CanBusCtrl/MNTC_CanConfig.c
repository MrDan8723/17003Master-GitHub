/*********************** (C) COPYRIGHT  2016 MNTC  **************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : MNTC_CanConfig_V0.2.c
  * Version     : V0.2
										1����CAN���ߵ�����������Э�����ֿ�
										2������CAN2����������
  * Description : CANͨ�������ļ�
  * Date        : 2018��7��20��
  * Author      : XYD
  * Check       :  
*******************************************************************************/



/*********************File include*********************/
#include "MNTC_CanConfig.h" 
#include "stm32f10x_can.h"

/******************Macro definition********************/

/*************External variable declaration************/

/***************Global variable definition*************/

/**************************************************************************************
  * name        : void MNTC_CanConfig(CAN_TypeDef* CANx, int remap)
  * Function    : CAN��ʼ������
  * Parameters  : int remap-- �Ƿ�ʹ���ض�������
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void MNTC_CanConfig(CAN_TypeDef* CANx,int remap)
{
  MNTC_CanPinConfig(CANx,remap);
	MNTC_CanDeviceConfig(CANx);
}


/**************************************************************************************
  * name        : void MNTC_CanPinConfig(CAN_TypeDef* CANx,int remap)
  * Function    : CAN�������ų�ʼ��
  * Parameters  : CAN_TypeDef* CANx-- ָ��CAN���裬CAN1,CAN2
                  int remap-- �Ƿ�ʹ���ض�������,0--��ʹ���ض������ţ�1--ʹ���ض�������
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void MNTC_CanPinConfig(CAN_TypeDef* CANx, int remap)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  if(0 == remap)                                                                          //��ʹ���ض�������
  {
    if(CAN1 == CANx)                                                                      //ָ��CAN1
    {
      RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;                                         //PA11=CAN1RX
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;                                         //PA12=CAN1TX
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    else if(CAN2 == CANx)                                                                 //ָ��CAN2
    {
      RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;                                         //PB12=CAN2RX
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;                                          //PPB13=CAN2TX
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
  }//end if(0 == remap)
  else if(1 == remap)
  {
    if(CAN1 == CANx)                                                                      //ָ��CAN1
    {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
      GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);  												            //ʹ��CAN1��ӳ��
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                                           //PB8=CAN1RX
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                           //PB9=CAN1TX
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    else if(CAN2 == CANx)                                                                 //ָ��CAN2
    {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
      GPIO_PinRemapConfig(GPIO_Remap_CAN2, ENABLE);  												              //ʹ��CAN2��ӳ��
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;                                           //PB5=CAN2RX
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                                           //PB6=CAN2TX
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
  }//end if(0 == remap)
}

/**************************************************************************************
  * name        : void MNTC_CanDeviceConfig(CAN_TypeDef* CANx)
  * Function    : CAN��ʼ������
  * Parameters  : CAN_TypeDef* CANx-- ָ��CAN����
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void MNTC_CanDeviceConfig(CAN_TypeDef* CANx)
{
	CAN_InitTypeDef        CAN_InitStructure;                                                           // CAN���߽ṹ������
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;	  																									// CAN���߹������ṹ������
  NVIC_InitTypeDef  NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	if(CAN1 == CANx)																																										//ʹ��CAN1
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 , ENABLE); 													                  // ��������ʱ��
#if CHIP_TYPE == 103
		NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;																				//103оƬ
#elif CHIP_TYPE == 107
		NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;																								//107оƬ
#endif
	}
	else if(CAN2 == CANx)																																								//ʹ��CAN2
	{
#if CHIP_TYPE == 107
		NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2 , ENABLE); 													                  // ��������ʱ��
#endif
	}
	
  CAN_DeInit(CANx);
  CAN_StructInit(&CAN_InitStructure);
  	
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;																					//ͨ�ŵ����ȼ�����Ϊ���
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* CAN�ṹ�������ʼ�� */
  CAN_InitStructure.CAN_TTCM = DISABLE;																																// ʱ�����ͨ��ģʽ
  CAN_InitStructure.CAN_ABOM = DISABLE;																																// �Զ����߹���ģʽ
  CAN_InitStructure.CAN_AWUM = DISABLE;																																// �Զ�����ģʽ
  CAN_InitStructure.CAN_NART = DISABLE;																																// ���Զ��ش���ģʽ
  CAN_InitStructure.CAN_RFLM = DISABLE;																																// ����FIFO����ģʽ																									
  CAN_InitStructure.CAN_TXFP = DISABLE;																																// ����FIFO���ȼ�
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;																												// ��������ģʽ
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;																														// ͬ����Ծ����ʱ�䵥Ԫ
  CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;																														// ʱ���1Ϊ6��ʱ�䵥Ԫ
  CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;																														// ʱ���2Ϊ3��ʱ�䵥Ԫ
  CAN_InitStructure.CAN_Prescaler = 12;																																// ��Ƶֵ
  CAN_Init(CANx, &CAN_InitStructure);																																	// CAN1��ʼ��
  
  /* CAN1 �˲�����ʼ�� */
	if(CAN1 == CANx)																																										//ʹ��CAN1
		CAN_FilterInitStructure.CAN_FilterNumber = 0;																	                    // ָ��ʹ��0�Ź�����
	else if(CAN2 == CANx)																																								//ʹ��CAN2
		CAN_FilterInitStructure.CAN_FilterNumber = 14;																	                  // ָ��ʹ��14�Ź�����
	
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;								                      // ����λ��ʶ��ģʽ
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;							                      // ������λ��Ϊ32λ
  
//  CAN_FilterInitStructure.CAN_FilterIdHigh = 0X0102; 		                                              // ��������ʶ����Ҫ���˵�ID��λ   ����Դ��ַID.12-ID.8    (((u16)CAN_MASTER_ID )<<8) | 0x00
//  CAN_FilterInitStructure.CAN_FilterIdLow  = 0X0000 | CAN_ID_EXT |CAN_RTR_DATA;                       // ��������ʶ����Ҫ���˵�ID��λ           0x0000
//  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFFFF;																							// ����������λ����λ��Ҫƥ�� 0xFFF0;	
//  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;																								// ����������λ����λ����Ҫƥ��   0x0000
  
  
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0X0000; 		// ��������ʶ����Ҫ���˵�ID��λ   ����Դ��ַID.12-ID.8
  CAN_FilterInitStructure.CAN_FilterIdLow  = 0X0000; // ��������ʶ����Ҫ���˵�ID��λ
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0X0000;																							// ����������λ����λ��Ҫƥ��
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;																								// ����������λ����λ����Ҫƥ��
  
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;																// ����ָ���������FIFO0
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;												                      // ʹ�ܹ�����
  CAN_FilterInit(&CAN_FilterInitStructure);
  
  CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);																														  // CANx�����ж�ʹ��
}

/**************************************************************************************
  * name        : void MNTC_CanSetFilter(CAN_TypeDef* CANx,uint8_t source_id, uint8_t my_id)
  * Function    : CAN�˲�������
  * Parameters  : CAN_TypeDef* CANx-- ָ��CAN����
									uint8_t source_id-- ��������ָ���Դ��ַ
									uint8_t my_id-- ������ַ
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void MNTC_CanSetFilter(CAN_TypeDef* CANx,uint8_t source_id, uint8_t my_id)
{
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;	  																									// CAN���߹������ṹ������

	  /* CAN1 �˲�����ʼ�� */
	if(CAN1 == CANx)																																										//ʹ��CAN1
		CAN_FilterInitStructure.CAN_FilterNumber = 0;																	                    // ָ��ʹ��0�Ź�����
	else if(CAN2 == CANx)																																								//ʹ��CAN2
		CAN_FilterInitStructure.CAN_FilterNumber = 14;																	                  // ָ��ʹ��14�Ź�����
	
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;								                      // ����λ��ʶ��ģʽ
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;							                      // ������λ��Ϊ32λ
  
  CAN_FilterInitStructure.CAN_FilterIdHigh = (((u16)source_id )<<8) | my_id; 		                  		// ��������ʶ����Ҫ���˵�ID��λ   ����Դ��ַID.12-ID.8
  CAN_FilterInitStructure.CAN_FilterIdLow  = 0X0000 | CAN_ID_EXT |CAN_RTR_DATA;                       // ��������ʶ����Ҫ���˵�ID��λ           0x0000
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFFFF;																							// ����������λ����λ��Ҫƥ��
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;																								// ����������λ����λ����Ҫƥ��
  
  
//  CAN_FilterInitStructure.CAN_FilterIdHigh = 0X0000; 		// ��������ʶ����Ҫ���˵�ID��λ   ����Դ��ַID.12-ID.8
//  CAN_FilterInitStructure.CAN_FilterIdLow  = 0X0000; // ��������ʶ����Ҫ���˵�ID��λ
//  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0X0000;																							// ����������λ����λ��Ҫƥ��
//  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;																								// ����������λ����λ����Ҫƥ��
  
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;																// ����ָ���������FIFO0
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;												                      // ʹ�ܹ�����
  CAN_FilterInit(&CAN_FilterInitStructure);

}
/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/
