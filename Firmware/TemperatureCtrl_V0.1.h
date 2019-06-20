/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : TemperatureCtrl_V0.1.h
  * Version     : V0.1
  * Description : 温度控制程序
  * Date        : 2017年9月13日
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#ifndef _TEMPERATURE_V0_1_H_
#define _TEMPERATURE_V0_1_H_


/*********************File include*********************/
#include "stm32f10x.h"

/******************Macro definition********************/
//定时器通道
#define TIM_CH1   1             //定时器通道1
#define TIM_CH2   2             //定时器通道2
#define TIM_CH3   3             //定时器通道3
#define TIM_CH4   4             //定时器通道4

//温控PWM输出频率设置，设置为1KHz
#define PID_PRESCALER   72      //预分频系数
#define PID_PERIOD      1000    //重装载值
/*********Enumeration variable type definition********/

/**************Structure type definition**************/

//温度控制结构体
typedef struct _TempCtrlTypeDef
{
  int           crt_temp;                     //当前温度
  int           target_temp;                  //设定温度
  float         kp;                           //比例系数
  float         ki;                           //积分系数
  float         kd;                           //微分系数
  int           pid_out;                      //PID控制器输出
  
  int           crt_error;                    //当前误差
  int           last_error;                   //上一次误差
  int           prev_error;                   //上上次误差
  
  TIM_TypeDef   *timer;                       //定时器
  int           ch;                           //定时器通道,宏定义TIM_CH0,TIM_CH1,TIM_CH2,TIM_CH3
  int           remap;                        //是否使用复用引脚 1--使用复用引脚，0--不使用复用引脚
}TempCtrlTypeDef;


/*****************Function declaration**************/

//用户接口函数
void PID_HardwareInit(TempCtrlTypeDef *pid,TIM_TypeDef *timer,int ch,int remap);
void PID_ParamInit(TempCtrlTypeDef *pid,int target_temp,float kp,float ki,float kd);
void PID_IncrementalCtrl(TempCtrlTypeDef *pid);



//底层支持函数，用户不需关心
void PID_PinInit(TempCtrlTypeDef *pid);
void PID_TimerInit(TempCtrlTypeDef *pid);
void PID_SetPulse(TempCtrlTypeDef *pid,int pulse);
int PID_GetPulse(TempCtrlTypeDef *pid);






 #endif

/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/
