/*********************** (C) COPYRIGHT  2016 MNTC  **************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : CAN_Configuration_V0.2.c
  * Version     : V0.2
  * Description : CANͨ�������ļ�
  * Date        : 2018��7��20��
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#ifndef _MNTC_CAN_CONFIG_H_
#define _MNTC_CAN_CONFIG_H_


/*********************File include*********************/
#include "stm32f10x.h"
#include "stm32f10x_can.h"

/******************Macro definition********************/
//���һ��оƬ���͵ĺ궨�壬���㱾����������ݼ���оƬ
//#define	CHIP_TYPE		103							//ѡ��103оƬ
#define CHIP_TYPE		107							//ѡ��107оƬ

/*********Enumeration variable type definition********/
/**************Structure type definition**************/
/*****************Function declaration**************/

/********�ӿں������ṩ���û�����***********/
void MNTC_CanConfig(CAN_TypeDef* CANx, int remap);//CAN����������
void MNTC_CanSetFilter(CAN_TypeDef* CANx,uint8_t source_id, uint8_t my_id);//CAN�˲�������



/********�ײ㺯�����û�����Ҫ����***********/
void MNTC_CanDeviceConfig(CAN_TypeDef* CANx);
void MNTC_CanPinConfig(CAN_TypeDef* CANx, int remap);




 #endif
/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/
