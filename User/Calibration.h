/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : Calibration.h
  * Version     : V0.1
  * Description : 17003项目定标相关算法
  * Date        : 2017年11月29日
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#ifndef _CALIBRATION_H_
#define _CALIBRATION_H_


/*********************File include*********************/
#include <math.h>
#include <stdlib.h>
#include <string.h>
/******************Macro definition********************/


/*********Enumeration variable type definition********/



/**************Structure type definition**************/
typedef struct _CalibrationStruct
{
  float param[20];                           //升序存放多项式系数
  float x[10];                               //样本点的横坐标值
  float y[10];                               //样本点的纵坐标值
  float r_square;                           //拟合优度R平方
  int   num;                                //样本点个数
}CalibrationStruct;
/*****************Function declaration**************/

//用户应用函数
void  FIB_Calibrate(CalibrationStruct *data);                                              //FIB定标计算
float FIB_CalcContent(CalibrationStruct *data,float time);                                 //计算FIB含量
void  AT3_Calibrate(CalibrationStruct *data);                                              //AT3定标计算
float AT3_CalcActive(CalibrationStruct *data,float od);                                    //计算AT3活性
void  FDP_Calibrate(CalibrationStruct *data);                                              //FDP定标计算
float FDP_CalcContent(CalibrationStruct *data,float od);                                   //计算FDP含量
void  DD_Calibrate(CalibrationStruct *data);                                               //DD定标计算
float DD_CalcContent(CalibrationStruct *data,float od);                                    //计算DD含量

//底层函数
void CalculateSpline3Value(float xi,float *yi,float x[],float y[],int num,float result[]); //使用三次样条插值的结果计算xi对应的yi
void CubicFit(float result[],float *rr,float x[],float y[],int num);                       //三次多项式拟合
void  LinearFit(float *k,float *b,float *rr,float x[],float y[],int cnt);                  //最小二乘法求解线性拟合
float GetAvg(float a[],int cnt);                                                           //求一个数组的平均值
float GetProductAvg(float a[],float b[],int cnt);                                          //求两个数组乘积的平均值，两个数组可以为同一个数组
void Spline3(float result[],float x[],float y[],int n);                                  //三次样条插值
void TridiagonalMatrix(float a[],int num,float result[]);                                  //三对角矩阵求解
void GaussSolve(float para[],int num,float a[],float b[]);                                 //高斯消元法求解多项式系数


 #endif
/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/
