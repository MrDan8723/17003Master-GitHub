/*********************** (C) COPYRIGHT  2016 MNTC  **************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : MNTC_CanConfig_V0.2.c
  * Version     : V0.2
										1、将CAN总线的驱动程序与协议程序分开
										2、增加CAN2的驱动程序
  * Description : CAN通信驱动文件
  * Date        : 2018年7月20日
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
  * Function    : CAN初始化函数
  * Parameters  : int remap-- 是否使用重定向引脚
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
  * Function    : CAN外设引脚初始化
  * Parameters  : CAN_TypeDef* CANx-- 指定CAN外设，CAN1,CAN2
                  int remap-- 是否使用重定向引脚,0--不使用重定向引脚，1--使用重定向引脚
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void MNTC_CanPinConfig(CAN_TypeDef* CANx, int remap)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  if(0 == remap)                                                                          //不使用重定向引脚
  {
    if(CAN1 == CANx)                                                                      //指定CAN1
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
    else if(CAN2 == CANx)                                                                 //指定CAN2
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
    if(CAN1 == CANx)                                                                      //指定CAN1
    {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
      GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);  												            //使能CAN1重映射
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                                           //PB8=CAN1RX
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                           //PB9=CAN1TX
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    else if(CAN2 == CANx)                                                                 //指定CAN2
    {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
      GPIO_PinRemapConfig(GPIO_Remap_CAN2, ENABLE);  												              //使能CAN2重映射
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
  * Function    : CAN初始化函数
  * Parameters  : CAN_TypeDef* CANx-- 指定CAN外设
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void MNTC_CanDeviceConfig(CAN_TypeDef* CANx)
{
	CAN_InitTypeDef        CAN_InitStructure;                                                           // CAN总线结构体声明
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;	  																									// CAN总线过滤器结构体声明
  NVIC_InitTypeDef  NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	if(CAN1 == CANx)																																										//使用CAN1
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 , ENABLE); 													                  // 开启外设时钟
#if CHIP_TYPE == 103
		NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;																				//103芯片
#elif CHIP_TYPE == 107
		NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;																								//107芯片
#endif
	}
	else if(CAN2 == CANx)																																								//使用CAN2
	{
#if CHIP_TYPE == 107
		NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2 , ENABLE); 													                  // 开启外设时钟
#endif
	}
	
  CAN_DeInit(CANx);
  CAN_StructInit(&CAN_InitStructure);
  	
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;																					//通信的优先级设置为最高
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* CAN结构体参数初始化 */
  CAN_InitStructure.CAN_TTCM = DISABLE;																																// 时间出发通信模式
  CAN_InitStructure.CAN_ABOM = DISABLE;																																// 自动离线管理模式
  CAN_InitStructure.CAN_AWUM = DISABLE;																																// 自动唤醒模式
  CAN_InitStructure.CAN_NART = DISABLE;																																// 非自动重传输模式
  CAN_InitStructure.CAN_RFLM = DISABLE;																																// 接收FIFO锁定模式																									
  CAN_InitStructure.CAN_TXFP = DISABLE;																																// 发送FIFO优先级
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;																												// 正常工作模式
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;																														// 同步跳跃两个时间单元
  CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;																														// 时间段1为6个时间单元
  CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;																														// 时间段2为3个时间单元
  CAN_InitStructure.CAN_Prescaler = 12;																																// 分频值
  CAN_Init(CANx, &CAN_InitStructure);																																	// CAN1初始化
  
  /* CAN1 滤波器初始化 */
	if(CAN1 == CANx)																																										//使用CAN1
		CAN_FilterInitStructure.CAN_FilterNumber = 0;																	                    // 指定使用0号过滤器
	else if(CAN2 == CANx)																																								//使用CAN2
		CAN_FilterInitStructure.CAN_FilterNumber = 14;																	                  // 指定使用14号过滤器
	
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;								                      // 屏蔽位标识符模式
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;							                      // 过滤器位宽为32位
  
//  CAN_FilterInitStructure.CAN_FilterIdHigh = 0X0102; 		                                              // 过滤器标识符。要过滤的ID高位   发送源地址ID.12-ID.8    (((u16)CAN_MASTER_ID )<<8) | 0x00
//  CAN_FilterInitStructure.CAN_FilterIdLow  = 0X0000 | CAN_ID_EXT |CAN_RTR_DATA;                       // 过滤器标识符。要过滤的ID低位           0x0000
//  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFFFF;																							// 过滤器屏蔽位：高位需要匹配 0xFFF0;	
//  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;																								// 过滤器屏蔽位：低位不需要匹配   0x0000
  
  
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0X0000; 		// 过滤器标识符。要过滤的ID高位   发送源地址ID.12-ID.8
  CAN_FilterInitStructure.CAN_FilterIdLow  = 0X0000; // 过滤器标识符。要过滤的ID低位
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0X0000;																							// 过滤器屏蔽位：高位需要匹配
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;																								// 过滤器屏蔽位：低位不需要匹配
  
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;																// 设置指向过滤器的FIFO0
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;												                      // 使能过滤器
  CAN_FilterInit(&CAN_FilterInitStructure);
  
  CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);																														  // CANx接收中断使能
}

/**************************************************************************************
  * name        : void MNTC_CanSetFilter(CAN_TypeDef* CANx,uint8_t source_id, uint8_t my_id)
  * Function    : CAN滤波器设置
  * Parameters  : CAN_TypeDef* CANx-- 指定CAN外设
									uint8_t source_id-- 本机接收指令的源地址
									uint8_t my_id-- 本机地址
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void MNTC_CanSetFilter(CAN_TypeDef* CANx,uint8_t source_id, uint8_t my_id)
{
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;	  																									// CAN总线过滤器结构体声明

	  /* CAN1 滤波器初始化 */
	if(CAN1 == CANx)																																										//使用CAN1
		CAN_FilterInitStructure.CAN_FilterNumber = 0;																	                    // 指定使用0号过滤器
	else if(CAN2 == CANx)																																								//使用CAN2
		CAN_FilterInitStructure.CAN_FilterNumber = 14;																	                  // 指定使用14号过滤器
	
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;								                      // 屏蔽位标识符模式
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;							                      // 过滤器位宽为32位
  
  CAN_FilterInitStructure.CAN_FilterIdHigh = (((u16)source_id )<<8) | my_id; 		                  		// 过滤器标识符。要过滤的ID高位   发送源地址ID.12-ID.8
  CAN_FilterInitStructure.CAN_FilterIdLow  = 0X0000 | CAN_ID_EXT |CAN_RTR_DATA;                       // 过滤器标识符。要过滤的ID低位           0x0000
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFFFF;																							// 过滤器屏蔽位：高位需要匹配
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;																								// 过滤器屏蔽位：低位不需要匹配
  
  
//  CAN_FilterInitStructure.CAN_FilterIdHigh = 0X0000; 		// 过滤器标识符。要过滤的ID高位   发送源地址ID.12-ID.8
//  CAN_FilterInitStructure.CAN_FilterIdLow  = 0X0000; // 过滤器标识符。要过滤的ID低位
//  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0X0000;																							// 过滤器屏蔽位：高位需要匹配
//  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;																								// 过滤器屏蔽位：低位不需要匹配
  
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;																// 设置指向过滤器的FIFO0
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;												                      // 使能过滤器
  CAN_FilterInit(&CAN_FilterInitStructure);

}
/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/
