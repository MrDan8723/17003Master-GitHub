/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : ActionTask.h
  * Version     : V0.1
  * Description : 血凝仪机械动作控制头文件
  * Date        : 2019年6月18日
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#ifndef _ACTION_TASK_H_
#define _ACTION_TASK_H_


/*********************File include*********************/
#include "Common.h"


/******************Macro definition********************/
#define SAMPLE_Y_DIST         16.0       //同一个样本架上相邻样本位中心间距
#define CUP_X_DIST            17.0       //X方向相邻空杯位中心间距
#define CUP_Y_DIST            13.0       //Y方向相邻空杯位中心间距
#define L_REAGENT_X_DIST      35.0       //大号试剂位X方向相邻中心间距
#define S_REAGENT_X_DIST      26.0       //X方向相邻小号试剂位中心间距
#define S_REAGENT_Y_DIST      26.0       //Y方向相邻小号试剂位中心间距
#define TEST_X_DIST           16.8       //同一检测块上相邻检测位X方向的中心间距

#define AXIS_X_FLAG     0X01             //X轴运动状态标志位
#define AXIS_Y_FLAG     0X02             //Y轴运动状态标志位
#define AXIS_Z_FLAG     0X04             //Z轴运动状态标志位
#define PLUNGER_FLAG    0X08             //柱塞泵运动状态标志位
#define PRP1_FLAG       0XF1             //蠕动泵1运动状态标志位
#define PRP2_FLAG       0XF2             //蠕动泵2运动状态标志位

/*********Enumeration variable type definition********/
//动作状态枚举类型定义
typedef enum
{
  IDLE=0,         //空闲状态
  RUNNING=1       //运行状态
}ActionStateEnum;

/**************Structure type definition**************/
//轴信息结构体定义
typedef struct 
{
  u8 id;                              //轴板ID
  ActionStateEnum state;              //轴状态
  
  float speed;                        //轴速度，mm/s
  float position;                     //轴当前绝对坐标
}AxisStruct;

//动作任务消息队列中消息结构体定义
typedef struct
{
  u8 type;                            //动作类型
  s16 Data[4];                        //其他数据
}ActionMsgStruct;
/*****************Function declaration**************/
//用户接口函数
void GetSample(u8 sample_num, u16 vol, u8 cup_num, u8 test_num);               //取样动作
void GetReagent(u8 reagent_num, u16 vol, u8 test_num);                         //取试剂动作
void WashNeedle(void);                                                         //洗针动作
void DiscardCup(u8 test_num);                                                  //丢杯动作
void AxisHome1(AxisStruct *axis);                                              //指定轴回零
void AxisHome(void);                                                           //三轴同时回零
void AxisRel1(AxisStruct *axis, float distance);                               //单轴相对运动
void AxisAbs1(AxisStruct *axis, float position);                               //单轴绝对运动
void AxisAbs2(float x, float y);                                               //XY轴同时运动
void AxisAbs3(float x, float y, float z);                                      //XYZ轴同时运动



//内部功能函数
void GetSamplePos(u8 sample_num, float *x, float *y);               //获取指定编号的样本位中心坐标
void GetCupPos(u8 cup_num, float *x, float *y);                     //获取指定编号的空杯位中心坐标
void GetReagentPos(u8 reagent_num, float *x, float *y);             //获取指定编号的试剂位中心坐标
void GetTestPos(u8 test_num, float *x, float *y);                   //获取指定编号的检测位中心坐标
void GetCup(float x, float y);                                      //从指定坐标取一个杯子
void PutCup(float x, float y);                                      //放一个杯子到指定坐标位
void PlungerHome(void);                                             //柱塞泵回零动作
void PlungerDraw(u16 vol, u16 delay);                               //柱塞泵抽取动作
void PlungerPush(u16 vol, u16 delay);                               //柱塞泵注射动作
void PeristalticRun(u16 delay);                                     //蠕动泵运行



#endif
/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/

