/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : TestTask.h
  * Version     : V0.1
  * Description : Ѫ���Ǽ����������ͷ�ļ�
  * Date        : 2019��6��18��
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#ifndef _TEST_TASK_H_
#define _TEST_TASK_H_


/*********************File include*********************/
#include "Common.h"


/******************Macro definition********************/


/*********Enumeration variable type definition********/
//������͵�ö�����Ͷ���
typedef enum
{
  TT=0,         //TT����
  PT=1,         //PT����
  APTT=2,       //APTT����
  FIB=3,        //FIB����
  DD=4,         //D-���������
  FDP=5,        //FDP����
  AT3=6         //AT3����
}TestTypeEnum;

//ͨ�����͵�ö�����Ͷ���
typedef enum
{
  S_CHANNLE=0,        //  scattering channle ɢ��ͨ��
  T_CHANNLE=1         //  transmission channle ͸��ͨ��
}TestChannleEnum;

//�㷨���͵�ö�����Ͷ���
typedef enum
{
  THRESHOLD_METHOD=0,           //��ֵ��
  DERIVATIVE_1_METHOD=1,        //һ�׵�����
  DERIVATIVE_2_METHOD=2,        //���׵�����
  MEDIAN_METHOD=3               //��ֵ��
}TestAlgorithmEnum;

//�Լ����͵�ö�����Ͷ���
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
//���Է����ṹ��
typedef struct _TestMethodInfoStruct
{
  TestChannleEnum ch_type;                //ͨ������
  TestAlgorithmEnum algorithm_type;       //�㷨����
  s16 reference[2];                       //�ο�ֵ,[0]--�ο�ֵ���� [1]--�ο�ֵ����
  u16 sample_vol;                         //�������ul
  s16 incubate_time;                      //����ʱ��
  TestReagentEnum r1_type;                //�Լ�1����
  u16 r1_vol;                             //�Լ�1���
  TestReagentEnum r2_type;                //�Լ�2����
  u16 r2_vol;                             //�Լ�2���
  TestReagentEnum r3_type;                //�Լ�3����
  u16 r3_vol;                             //�Լ�3���
  u16 max_time;                           //�����ʱ��
}TestMethodInfoStruct;


/*****************Function declaration**************/


#endif
/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/


