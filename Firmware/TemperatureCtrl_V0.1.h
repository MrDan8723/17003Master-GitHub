/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : TemperatureCtrl_V0.1.h
  * Version     : V0.1
  * Description : �¶ȿ��Ƴ���
  * Date        : 2017��9��13��
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#ifndef _TEMPERATURE_V0_1_H_
#define _TEMPERATURE_V0_1_H_


/*********************File include*********************/
#include "stm32f10x.h"

/******************Macro definition********************/
//��ʱ��ͨ��
#define TIM_CH1   1             //��ʱ��ͨ��1
#define TIM_CH2   2             //��ʱ��ͨ��2
#define TIM_CH3   3             //��ʱ��ͨ��3
#define TIM_CH4   4             //��ʱ��ͨ��4

//�¿�PWM���Ƶ�����ã�����Ϊ1KHz
#define PID_PRESCALER   72      //Ԥ��Ƶϵ��
#define PID_PERIOD      1000    //��װ��ֵ
/*********Enumeration variable type definition********/

/**************Structure type definition**************/

//�¶ȿ��ƽṹ��
typedef struct _TempCtrlTypeDef
{
  int           crt_temp;                     //��ǰ�¶�
  int           target_temp;                  //�趨�¶�
  float         kp;                           //����ϵ��
  float         ki;                           //����ϵ��
  float         kd;                           //΢��ϵ��
  int           pid_out;                      //PID���������
  
  int           crt_error;                    //��ǰ���
  int           last_error;                   //��һ�����
  int           prev_error;                   //���ϴ����
  
  TIM_TypeDef   *timer;                       //��ʱ��
  int           ch;                           //��ʱ��ͨ��,�궨��TIM_CH0,TIM_CH1,TIM_CH2,TIM_CH3
  int           remap;                        //�Ƿ�ʹ�ø������� 1--ʹ�ø������ţ�0--��ʹ�ø�������
}TempCtrlTypeDef;


/*****************Function declaration**************/

//�û��ӿں���
void PID_HardwareInit(TempCtrlTypeDef *pid,TIM_TypeDef *timer,int ch,int remap);
void PID_ParamInit(TempCtrlTypeDef *pid,int target_temp,float kp,float ki,float kd);
void PID_IncrementalCtrl(TempCtrlTypeDef *pid);



//�ײ�֧�ֺ������û��������
void PID_PinInit(TempCtrlTypeDef *pid);
void PID_TimerInit(TempCtrlTypeDef *pid);
void PID_SetPulse(TempCtrlTypeDef *pid,int pulse);
int PID_GetPulse(TempCtrlTypeDef *pid);






 #endif

/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/
