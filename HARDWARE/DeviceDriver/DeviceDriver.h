/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : DeviceDriver.h
  * Version     : V0.1
  * Description : 推片染色机设备驱动程序
  * Date        : 2018年10月18日
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#ifndef _DEVICEDRIVER_H_
#define _DEVICEDRIVER_H_


/*********************File include*********************/
#include "sys.h"

/******************Macro definition********************/

#define POS1_VALVE_ON							PDout(10) = 0				//取样正压阀开
#define POS1_VALVE_OFF						PDout(10) = 1				//取样正压阀开
#define NEG1_VALVE_ON							PDout(11) = 0				//取样负压阀开
#define NEG1_VALVE_OFF						PDout(11) = 1				//取样负压阀开
#define POS2_VALVE_ON							PDout(14) = 0				//染色正压阀开
#define POS2_VALVE_OFF						PDout(14) = 1				//染色正压阀开
#define NEG2_VALVE_ON							PDout(15) = 0				//染色负压阀开
#define NEG2_VALVE_OFF						PDout(15) = 1				//染色负压阀开


#define NEEDLE_VALVE_ON						PDout(12) = 0				//洗针液阀开，清洗内壁
#define NEEDLE_VALVE_OFF					PDout(12) = 1				//洗针液阀关，清洗外壁
#define PUSHER_VALVE_ON						PDout(13) = 0				//开阀，切换到酒精
#define PUSHER_VALVE_OFF					PDout(13) = 1				//关阀，切换到水


#define NEEDLE_INPUT_PUMP_ON      PAout(1) = 0				//洗针泵开
#define NEEDLE_INPUT_PUMP_OFF     PAout(1) = 1				//洗针泵关
#define NEEDLE_OUTPUT_PUMP_ON			PAout(2) = 0				//洗针排废泵开
#define NEEDLE_OUTPUT_PUMP_OFF		PAout(2) = 1				//洗针排废泵关

#define PUSHER_INPUT_PUMP_ON      PAout(3) = 0				//洗刀泵开
#define PUSHER_INPUT_PUMP_OFF     PAout(3) = 1				//洗刀泵关
#define PUSHER_OUTPUT_PUMP_ON			PAout(4) = 0				//洗刀排废泵开
#define PUSHER_OUTPUT_PUMP_OFF		PAout(4) = 1				//洗刀针排废泵关

#define REAGENT1_PUMP_ON					PAout(5) = 0				//试剂1泵开
#define REAGENT1_PUMP_OFF					PAout(5) = 1				//试剂1泵关
#define REAGENT2_PUMP_ON					PAout(6) = 0				//试剂2泵开
#define REAGENT2_PUMP_OFF					PAout(6) = 1				//试剂2泵关
#define REAGENT3_PUMP_ON					PAout(7) = 0				//试剂3泵开
#define REAGENT3_PUMP_OFF					PAout(7) = 1				//试剂3泵关
#define REAGENT4_PUMP_ON					PCout(4) = 0				//试剂4泵开
#define REAGENT4_PUMP_OFF					PCout(4) = 1				//试剂4泵关
#define STAIN_OUTPUT_PUMP_ON   	  PCout(5) = 0				//染色排废泵开
#define STAIN_OUTPUT_PUMP_OFF     PCout(5) = 1				//染色排废泵关

#define AIR1_PUMP_ON							PEout(7) = 0				//负压泵1开
#define AIR1_PUMP_OFF							PEout(7) = 1				//负压泵1关
#define AIR2_PUMP_ON							PBout(1) = 0				//负压泵2开
#define AIR2_PUMP_OFF							PBout(1) = 1				//负压泵2关

/*********Enumeration variable type definition********/



/**************Structure type definition**************/

/*****************Function declaration**************/

void ServoInit(void);																//舵机控制初始化
void ServoPosition(float angle, int ms);						//舵机在指定时间内运动到指定位置
void ServoSetAngle(float angle);										//舵机直接运动到指定位置
void OutputInit(void);
void ComToCan(void);
	
 #endif
/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/ 
 
 
 