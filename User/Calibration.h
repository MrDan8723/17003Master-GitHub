/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : Calibration.h
  * Version     : V0.1
  * Description : 17003��Ŀ��������㷨
  * Date        : 2017��11��29��
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
  float param[20];                           //�����Ŷ���ʽϵ��
  float x[10];                               //������ĺ�����ֵ
  float y[10];                               //�������������ֵ
  float r_square;                           //����Ŷ�Rƽ��
  int   num;                                //���������
}CalibrationStruct;
/*****************Function declaration**************/

//�û�Ӧ�ú���
void  FIB_Calibrate(CalibrationStruct *data);                                              //FIB�������
float FIB_CalcContent(CalibrationStruct *data,float time);                                 //����FIB����
void  AT3_Calibrate(CalibrationStruct *data);                                              //AT3�������
float AT3_CalcActive(CalibrationStruct *data,float od);                                    //����AT3����
void  FDP_Calibrate(CalibrationStruct *data);                                              //FDP�������
float FDP_CalcContent(CalibrationStruct *data,float od);                                   //����FDP����
void  DD_Calibrate(CalibrationStruct *data);                                               //DD�������
float DD_CalcContent(CalibrationStruct *data,float od);                                    //����DD����

//�ײ㺯��
void CalculateSpline3Value(float xi,float *yi,float x[],float y[],int num,float result[]); //ʹ������������ֵ�Ľ������xi��Ӧ��yi
void CubicFit(float result[],float *rr,float x[],float y[],int num);                       //���ζ���ʽ���
void  LinearFit(float *k,float *b,float *rr,float x[],float y[],int cnt);                  //��С���˷�����������
float GetAvg(float a[],int cnt);                                                           //��һ�������ƽ��ֵ
float GetProductAvg(float a[],float b[],int cnt);                                          //����������˻���ƽ��ֵ�������������Ϊͬһ������
void Spline3(float result[],float x[],float y[],int n);                                  //����������ֵ
void TridiagonalMatrix(float a[],int num,float result[]);                                  //���ԽǾ������
void GaussSolve(float para[],int num,float a[],float b[]);                                 //��˹��Ԫ��������ʽϵ��


 #endif
/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/
