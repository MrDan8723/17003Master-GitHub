/*********************** (C) COPYRIGHT  2016 MNTC  **************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : CAN_Configuration_V0.2.c
  * Version     : V0.2
  * Description : CAN通信驱动文件
  * Date        : 2018年7月20日
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#ifndef _MNTC_CAN_CONFIG_H_
#define _MNTC_CAN_CONFIG_H_


/*********************File include*********************/
#include "stm32f10x.h"
#include "stm32f10x_can.h"

/******************Macro definition********************/
//添加一个芯片类型的宏定义，方便本驱动程序兼容几种芯片
//#define	CHIP_TYPE		103							//选择103芯片
#define CHIP_TYPE		107							//选择107芯片

/*********Enumeration variable type definition********/
/**************Structure type definition**************/
/*****************Function declaration**************/

/********接口函数，提供给用户调用***********/
void MNTC_CanConfig(CAN_TypeDef* CANx, int remap);//CAN驱动器设置
void MNTC_CanSetFilter(CAN_TypeDef* CANx,uint8_t source_id, uint8_t my_id);//CAN滤波器设置



/********底层函数，用户不需要关心***********/
void MNTC_CanDeviceConfig(CAN_TypeDef* CANx);
void MNTC_CanPinConfig(CAN_TypeDef* CANx, int remap);




 #endif
/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/
