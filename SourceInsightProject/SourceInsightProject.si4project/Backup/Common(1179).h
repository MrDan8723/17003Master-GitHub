/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : Common.h
  * Version     : V0.1
  * Description : Ѫ���ǹ���ͷ�ļ�
  * Date        : 2019��6��18��
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#ifndef _COMMON_H_
#define _COMMON_H_


/*********************File include*********************/
#include "includes.h"
#include "os_app_hooks.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "includes.h"
#include "USART_DMA_Config.h"
#include "MNTC_CanConfig.h"
#include "MNTC_CanProtocol.h"
#include "MNTC_IOCtrl.h" 
#include "ActionTask.h"
#include "TestTask.h"

/******************Macro definition********************/
//CANͨ���ն�ID�궨��
#define CAN_MASTER_ID   0X01            //����ID
#define CAN_Z_AXIS_ID   0X02            //Z���ID
#define CAN_X_AXIS_ID   0X03            //X���ID
#define CAN_Y_AXIS_ID   0X04            //Y���ID
#define CAN_PLUNGER_ID  0X05            //�����ð�ID
#define CAN_ADC_ID      0XF6            //ADC��ID
#define CAN_TEMP_ID     0XF7            //�¿ذ��ID
#define CAN_PRP1_ID     0XF8            //�䶯��1ID
#define CAN_PRP2_ID     0XF9            //�䶯��2ID

//

/*********Enumeration variable type definition********/

/**************Structure type definition**************/

/*****************Function declaration**************/





 #endif
/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/ 
 
 
 