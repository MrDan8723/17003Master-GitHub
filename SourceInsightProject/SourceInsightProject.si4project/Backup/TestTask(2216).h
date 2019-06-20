/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : TestTask.h
  * Version     : V0.1
  * Description : 血凝仪检测块管理任务头文件
  * Date        : 2019年6月18日
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#ifndef _TEST_TASK_H_
#define _TEST_TASK_H_


/*********************File include*********************/
#include "Common.h"


/******************Macro definition********************/


/*********Enumeration variable type definition********/
//检测类型的枚举类型定义
typedef enum
{
  TT=0,         //TT测试
  PT=1,         //PT测试
  APTT=2,       //APTT测试
  FIB=3,        //FIB测试
  DD=4,         //D-二聚体测试
  FDP=5,        //FDP测试
  AT3=6         //AT3测试
}TestTypeEnum;

//通道类型的枚举类型定义
typedef enum
{
  S_CHANNLE=0,        //  scattering channle 散射通道
  T_CHANNLE=1         //  transmission channle 透射通道
}TestChannleEnum;

//算法类型的枚举类型定义
typedef enum
{
  THRESHOLD_METHOD=0,           //阈值法
  DERIVATIVE_1_METHOD=1,        //一阶导数法
  DERIVATIVE_2_METHOD=2,        //二阶导数法
  MEDIAN_METHOD=3               //中值法
}TestAlgorithmEnum;

//试剂类型的枚举类型定义
typedef enum
{
  TT_R1=0,
  PT_R1=1,
  APTT_R1=2,
  APTT_R2=3,
  FIB_R1=4,
  FIB_R2=5,
  DD_R1=6,
  DD_R2=7,
  DD_R3=8,
  FDP_R1=9,
  FDP_R2=10,
  FDP_R3=11,
  AT3_R1=12,
  AT3_R2=13,
  AT3_R3=14,
  
}TestReagentEnum;
  
/**************Structure type definition**************/
//测试方法结构体
typedef struct _TestMethodInfoStruct
{
  TestChannleEnum ch_type;                //通道类型
  TestAlgorithmEnum algorithm_type;       //算法类型
  s16 reference[2];                       //参考值,[0]--参考值下限 [1]--参考值上限
  u16 sample_vol;                         //样本体积ul
  s16 incubate_time;                      //孵育时间
  TestReagentEnum r1_type;                //试剂1类型
  u16 r1_vol;                             //试剂1体积
  TestReagentEnum r2_type;                //试剂2类型
  u16 r2_vol;                             //试剂2体积
  TestReagentEnum r3_type;                //试剂3类型
  u16 r3_vol;                             //试剂3体积
  u16 max_time;                           //最大检测时间
}TestMethodInfoStruct;


/*****************Function declaration**************/


#endif
/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/


