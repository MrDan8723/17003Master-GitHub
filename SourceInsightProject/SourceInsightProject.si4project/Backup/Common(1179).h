/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : Common.h
  * Version     : V0.1
  * Description : 血凝仪公共头文件
  * Date        : 2019年6月18日
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
//CAN通信终端ID宏定义
#define CAN_MASTER_ID   0X01            //主板ID
#define CAN_Z_AXIS_ID   0X02            //Z轴板ID
#define CAN_X_AXIS_ID   0X03            //X轴板ID
#define CAN_Y_AXIS_ID   0X04            //Y轴板ID
#define CAN_PLUNGER_ID  0X05            //柱塞泵板ID
#define CAN_ADC_ID      0XF6            //ADC板ID
#define CAN_TEMP_ID     0XF7            //温控板板ID
#define CAN_PRP1_ID     0XF8            //蠕动泵1ID
#define CAN_PRP2_ID     0XF9            //蠕动泵2ID

//

/*********Enumeration variable type definition********/

/**************Structure type definition**************/

/*****************Function declaration**************/





 #endif
/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/ 
 
 
 