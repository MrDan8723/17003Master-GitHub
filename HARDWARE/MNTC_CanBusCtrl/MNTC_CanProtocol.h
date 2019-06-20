/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : Templet.h
  * Version     : V0.1
  * Description : 用于MNTC编程规范的.h文件模板文件
  * Date        : 2017年9月6日
  * Author      : Author
  * Check       :  
*******************************************************************************/
#ifndef _MNTC_CAN_PROTOCOL_H_
#define _MNTC_CAN_PROTOCOL_H_

#define HAL_LAB_ENA   					0			//HAL库支持
#define STM32F1XX_STD_LAB_ENA		1			//标准库支持

/*********************File include*********************/
#if HAL_LAB_ENA
#include "can.h"
#elif STM32F1XX_STD_LAB_ENA
#include "stm32f10x_can.h"
#endif

/******************Macro definition********************/
/******************Macro definition********************/
//命令所属模块，未经许可禁止修改
#define CMD_IO              0X00      //IO口命令
#define CMD_PLUNGER         0X01      //柱塞泵命令
#define CMD_HOSE						0X02			//蠕动泵
#define CMD_AXIS            0X03      //运动轴命令
#define CMD_ADC    			    0X04      //模拟量命令
#define CMD_TEMP            0X05      //温度控制命令
#define CMD_USER            0XF0      //用户自定义命令

//IO模块功能，未经许可禁止修改
#define CMD_IO_READ         0X00      //读IO口操作
#define CMD_IO_WRITE        0X01      //写IO口操作
#define CMD_IO_TRIGGER      0X02      //IO翻转操作
#define CMD_IO_SETENABLE		0X03			//设置使能电平

//柱塞泵模块功能，未经许可禁止修改
#define CMD_PLUNGER_HOME    0X00      //柱塞泵回零操作
#define CMD_PLUNGER_DRAW    0X01      //柱塞泵抽取操作
#define CMD_PLUNGER_PUSH    0X02      //柱塞泵注射操作
#define CMD_PLUNGER_WASH    0X03      //柱塞泵冲洗操作
#define CMD_PLUNGER_SPEED   0X04      //设置柱塞泵速度操作

//蠕动泵模块功能，未经许可禁止修改
#define CMD_HOSE_SETCW      0X00      //蠕动泵正向电平
#define CMD_HOSE_SETDIR     0X01      //蠕动泵方向
#define CMD_HOSE_SS   		  0X02      //蠕动泵启停
#define CMD_HOSE_SPEED			0X03			//设置速度

//运动轴模块功能，未经许可禁止修改
#define CMD_AXIS_HOME       0X00      //运动轴回零操作
#define CMD_AXIS_SPEED   		0X01      //运动轴设置速度操作
#define CMD_AXIS_ABSOLUTE   0X02      //运动轴绝对运动操作
#define CMD_AXIS_RELATIVE		0X03  		//相对运动
#define CMD_AXIS_CONTINUE		0X04			//持续运动
#define CMD_AXIS_STOP				0X05			//急停

//模拟量模块功能，未经许可禁止修改
#define CMD_ANALOG_READ     0X00       //读模拟量
#define CMD_ANALOG_WRITE	  0X01			 //写模拟量

//温度控制模块功能，未经许可禁止修改
#define CMD_TEMP_SS         0X00      //温度控制启停操作
#define CMD_TEMP_WRITE      0X01      //设置目标温度操作
#define CMD_TEMP_READ       0X02      //读取当前温度操作
#define CMD_TEMP_PARAM_P    0X03      //设置温度控制器P参数操作
#define CMD_TEMP_PARAM_I    0X04      //设置温度控制器I参数操作
#define CMD_TEMP_PARAM_D    0X05      //设置温度控制器D参数操作


/*********Enumeration variable type definition********/



/**************Structure type definition**************/
typedef struct _MNTC_CanProtocolTypeDef
{ 
#if HAL_LAB_ENA
	CAN_HandleTypeDef		hcan;							//CAN句柄
  CanTxMsgTypeDef     tx_msg;           //发送缓冲区
  CanRxMsgTypeDef     rx_msg;           //接收缓冲区
#elif STM32F1XX_STD_LAB_ENA
	CAN_TypeDef* 				com;              //指定CAN端口  
	CanTxMsg						tx_msg;           //发送缓冲区
	CanRxMsg						rx_msg;           //接收缓冲区
#endif
	
	uint8_t      my_id;                   //分配给指定CAN口的ID
	char         rx_flag;									//接收完成标志位，1--接收到正确命令  0-- 未接收到命令
}MNTC_CanProtocolTypeDef;

/*****************Function declaration**************/

//用户接口函数
void CAN_ProtocolInit(MNTC_CanProtocolTypeDef *buf,CAN_TypeDef* com, uint8_t my_id);//CAN协议初始化函数

//蠕动泵控制函数
void CAN_PeristalticPumpSetCW(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num,uint16_t cw);//设置蠕动泵正方向电平
uint8_t CAN_IsCAN_PeristalticPumpSetCW(MNTC_CanProtocolTypeDef buf, uint16_t *cw);//判断当前帧是不是设置蠕动泵正方向电平命令
void CAN_PeristalticPumpSetDir(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num,uint16_t dir);//设置蠕动泵运动方向
uint8_t CAN_IsPeristalticPumpSetDir(MNTC_CanProtocolTypeDef buf, uint16_t *dir);//判断当前帧命令是不是设置蠕动泵运动方向命令
void CAN_PeristalticPumpRun(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num,uint16_t delay);//蠕动泵起停命令
uint8_t CAN_IsPeristalticPumpRun(MNTC_CanProtocolTypeDef buf,uint16_t *delay);//判断当前帧是不是蠕动泵启停命令
void CAN_PeristalticPumpSetSpeed(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num,float speed);//蠕动泵速度设置
uint8_t CAN_IsPeristalticPumpSetSpeed(MNTC_CanProtocolTypeDef buf, float *speed);//判断是不是蠕动泵速度设置命令
void CAN_ReplyPeristalticPumpState(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, int state);//蠕动泵状态回复函数

//柱塞泵控制函数
void CAN_PlungerHome(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num);//发送柱塞泵回零命令
uint8_t CAN_IsPlungerHome(MNTC_CanProtocolTypeDef buf);//判断当前接收的命令是不是柱塞泵回零命令
void CAN_PlungerDraw(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float volume, int delay_s);//发送柱塞泵抽取命令
uint8_t CAN_IsPlungerDraw(MNTC_CanProtocolTypeDef buf,float *volume, int *delay_s);//判断当前接收的命令是不是柱塞泵抽取命令
void CAN_PlungerPush(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float volume, int delay_s);//发送柱塞泵注射命令
uint8_t CAN_IsPlungerPush(MNTC_CanProtocolTypeDef buf,float *volume, int *delay_s);//判断当前接收的命令是不是柱塞泵注射命令
void CAN_PlungerWash(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float volume, int delay_s, int cnt);//发送柱塞泵清洗命令
uint8_t CAN_IsPlungerWash(MNTC_CanProtocolTypeDef buf,float *volume, int *delay_s, int *cnt) ;//判断当前接收的命令是不是柱塞泵清洗命令


//直线轴控制函数
void CAN_ReplyAxisPosition(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float position);//回复直线轴绝对位置
void CAN_ReplyAxisSpeed(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, uint16_t speed);//回复直线轴速度

void CAN_AxisHome(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num);//发送直线轴回零命令
uint8_t CAN_IsAxisHome(MNTC_CanProtocolTypeDef buf);													//判断当前接收帧指令是不是正确的轴回零命令
void CAN_AxisAbsolute(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float position);//发送直线轴绝对运动命令
uint8_t CAN_IsAxisAbsolute(MNTC_CanProtocolTypeDef buf,float *position);			//判断当前接收帧指令是不是正确的轴绝对运动命令,并通过指针返回绝对位置值
void CAN_AxisRelative(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float value);//发送直线轴相对运动命令
uint8_t CAN_IsAxisRelative(MNTC_CanProtocolTypeDef buf,float *position);			//判断当前接收帧指令是不是正确的轴相对运动命令,并通过指针返回相对位置值
void CAN_AxisSpeed(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, uint16_t speed);//发送直线轴速度设置命令
uint8_t CAN_IsAxisSpeed(MNTC_CanProtocolTypeDef buf,uint16_t *speed);					//判断当前接收帧指令是不是正确的轴速度设置命令,并通过指针返回速度值
void CAN_AxisContinue(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, uint16_t dir);//发送直线轴持续运动命令
uint8_t CAN_IsAxisContinue(MNTC_CanProtocolTypeDef buf, uint16_t *dir);//判断当前接收帧命令是不是直线轴持续运动命令
void CAN_AxisStop(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num);//发送直线轴急停命令
uint8_t CAN_IsAxisStop(MNTC_CanProtocolTypeDef buf);//判断当前接收帧命令是不是直线轴急停命令

//基本协议操作函数
uint8_t CAN_GetRxSourceId(MNTC_CanProtocolTypeDef buf);												//获取接收帧中的源地址
uint8_t CAN_GetRxTargetId(MNTC_CanProtocolTypeDef buf);												//获取接收帧中的目标地址
void CAN_SetTxSourceId(MNTC_CanProtocolTypeDef *buf,uint8_t source_id);				//设置发送帧中的源地址
void CAN_SetTxTargetId(MNTC_CanProtocolTypeDef *buf,uint8_t target_id);				//设置发送帧中的目标地址
void CAN_SendData(MNTC_CanProtocolTypeDef *buf);															//CAN发送数据
 #endif

/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/ 

  