/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : ActionTask.h
  * Version     : V0.1
  * Description : Ѫ���ǻ�е��������ͷ�ļ�
  * Date        : 2019��6��18��
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#ifndef _ACTION_TASK_H_
#define _ACTION_TASK_H_


/*********************File include*********************/
#include "Common.h"


/******************Macro definition********************/
#define SAMPLE_Y_DIST         16.0       //ͬһ������������������λ���ļ��
#define CUP_X_DIST            17.0       //X�������ڿձ�λ���ļ��
#define CUP_Y_DIST            13.0       //Y�������ڿձ�λ���ļ��
#define L_REAGENT_X_DIST      35.0       //����Լ�λX�����������ļ��
#define S_REAGENT_X_DIST      26.0       //X��������С���Լ�λ���ļ��
#define S_REAGENT_Y_DIST      26.0       //Y��������С���Լ�λ���ļ��
#define TEST_X_DIST           16.8       //ͬһ���������ڼ��λX��������ļ��

#define AXIS_X_FLAG     0X01             //X���˶�״̬��־λ
#define AXIS_Y_FLAG     0X02             //Y���˶�״̬��־λ
#define AXIS_Z_FLAG     0X04             //Z���˶�״̬��־λ
#define PLUNGER_FLAG    0X08             //�������˶�״̬��־λ
#define PRP1_FLAG       0XF1             //�䶯��1�˶�״̬��־λ
#define PRP2_FLAG       0XF2             //�䶯��2�˶�״̬��־λ

/*********Enumeration variable type definition********/
//����״̬ö�����Ͷ���
typedef enum
{
  IDLE=0,         //����״̬
  RUNNING=1       //����״̬
}ActionStateEnum;

/**************Structure type definition**************/
//����Ϣ�ṹ�嶨��
typedef struct 
{
  u8 id;                              //���ID
  ActionStateEnum state;              //��״̬
  
  float speed;                        //���ٶȣ�mm/s
  float position;                     //�ᵱǰ��������
}AxisStruct;

//����������Ϣ��������Ϣ�ṹ�嶨��
typedef struct
{
  u8 type;                            //��������
  s16 Data[4];                        //��������
}ActionMsgStruct;
/*****************Function declaration**************/
//�û��ӿں���
void GetSample(u8 sample_num, u16 vol, u8 cup_num, u8 test_num);               //ȡ������
void GetReagent(u8 reagent_num, u16 vol, u8 test_num);                         //ȡ�Լ�����
void WashNeedle(void);                                                         //ϴ�붯��
void DiscardCup(u8 test_num);                                                  //��������
void AxisHome1(AxisStruct *axis);                                              //ָ�������
void AxisHome(void);                                                           //����ͬʱ����
void AxisRel1(AxisStruct *axis, float distance);                               //��������˶�
void AxisAbs1(AxisStruct *axis, float position);                               //��������˶�
void AxisAbs2(float x, float y);                                               //XY��ͬʱ�˶�
void AxisAbs3(float x, float y, float z);                                      //XYZ��ͬʱ�˶�



//�ڲ����ܺ���
void GetSamplePos(u8 sample_num, float *x, float *y);               //��ȡָ����ŵ�����λ��������
void GetCupPos(u8 cup_num, float *x, float *y);                     //��ȡָ����ŵĿձ�λ��������
void GetReagentPos(u8 reagent_num, float *x, float *y);             //��ȡָ����ŵ��Լ�λ��������
void GetTestPos(u8 test_num, float *x, float *y);                   //��ȡָ����ŵļ��λ��������
void GetCup(float x, float y);                                      //��ָ������ȡһ������
void PutCup(float x, float y);                                      //��һ�����ӵ�ָ������λ
void PlungerHome(void);                                             //�����û��㶯��
void PlungerDraw(u16 vol, u16 delay);                               //�����ó�ȡ����
void PlungerPush(u16 vol, u16 delay);                               //������ע�䶯��
void PeristalticRun(u16 delay);                                     //�䶯������



#endif
/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/

