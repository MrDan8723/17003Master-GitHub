/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : DeviceDriver.h
  * Version     : V0.1
  * Description : ��ƬȾɫ���豸��������
  * Date        : 2018��10��18��
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#ifndef _DEVICEDRIVER_H_
#define _DEVICEDRIVER_H_


/*********************File include*********************/
#include "sys.h"

/******************Macro definition********************/

#define POS1_VALVE_ON							PDout(10) = 0				//ȡ����ѹ����
#define POS1_VALVE_OFF						PDout(10) = 1				//ȡ����ѹ����
#define NEG1_VALVE_ON							PDout(11) = 0				//ȡ����ѹ����
#define NEG1_VALVE_OFF						PDout(11) = 1				//ȡ����ѹ����
#define POS2_VALVE_ON							PDout(14) = 0				//Ⱦɫ��ѹ����
#define POS2_VALVE_OFF						PDout(14) = 1				//Ⱦɫ��ѹ����
#define NEG2_VALVE_ON							PDout(15) = 0				//Ⱦɫ��ѹ����
#define NEG2_VALVE_OFF						PDout(15) = 1				//Ⱦɫ��ѹ����


#define NEEDLE_VALVE_ON						PDout(12) = 0				//ϴ��Һ��������ϴ�ڱ�
#define NEEDLE_VALVE_OFF					PDout(12) = 1				//ϴ��Һ���أ���ϴ���
#define PUSHER_VALVE_ON						PDout(13) = 0				//�������л����ƾ�
#define PUSHER_VALVE_OFF					PDout(13) = 1				//�ط����л���ˮ


#define NEEDLE_INPUT_PUMP_ON      PAout(1) = 0				//ϴ��ÿ�
#define NEEDLE_INPUT_PUMP_OFF     PAout(1) = 1				//ϴ��ù�
#define NEEDLE_OUTPUT_PUMP_ON			PAout(2) = 0				//ϴ���ŷϱÿ�
#define NEEDLE_OUTPUT_PUMP_OFF		PAout(2) = 1				//ϴ���ŷϱù�

#define PUSHER_INPUT_PUMP_ON      PAout(3) = 0				//ϴ���ÿ�
#define PUSHER_INPUT_PUMP_OFF     PAout(3) = 1				//ϴ���ù�
#define PUSHER_OUTPUT_PUMP_ON			PAout(4) = 0				//ϴ���ŷϱÿ�
#define PUSHER_OUTPUT_PUMP_OFF		PAout(4) = 1				//ϴ�����ŷϱù�

#define REAGENT1_PUMP_ON					PAout(5) = 0				//�Լ�1�ÿ�
#define REAGENT1_PUMP_OFF					PAout(5) = 1				//�Լ�1�ù�
#define REAGENT2_PUMP_ON					PAout(6) = 0				//�Լ�2�ÿ�
#define REAGENT2_PUMP_OFF					PAout(6) = 1				//�Լ�2�ù�
#define REAGENT3_PUMP_ON					PAout(7) = 0				//�Լ�3�ÿ�
#define REAGENT3_PUMP_OFF					PAout(7) = 1				//�Լ�3�ù�
#define REAGENT4_PUMP_ON					PCout(4) = 0				//�Լ�4�ÿ�
#define REAGENT4_PUMP_OFF					PCout(4) = 1				//�Լ�4�ù�
#define STAIN_OUTPUT_PUMP_ON   	  PCout(5) = 0				//Ⱦɫ�ŷϱÿ�
#define STAIN_OUTPUT_PUMP_OFF     PCout(5) = 1				//Ⱦɫ�ŷϱù�

#define AIR1_PUMP_ON							PEout(7) = 0				//��ѹ��1��
#define AIR1_PUMP_OFF							PEout(7) = 1				//��ѹ��1��
#define AIR2_PUMP_ON							PBout(1) = 0				//��ѹ��2��
#define AIR2_PUMP_OFF							PBout(1) = 1				//��ѹ��2��

/*********Enumeration variable type definition********/



/**************Structure type definition**************/

/*****************Function declaration**************/

void ServoInit(void);																//������Ƴ�ʼ��
void ServoPosition(float angle, int ms);						//�����ָ��ʱ�����˶���ָ��λ��
void ServoSetAngle(float angle);										//���ֱ���˶���ָ��λ��
void OutputInit(void);
void ComToCan(void);
	
 #endif
/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/ 
 
 
 