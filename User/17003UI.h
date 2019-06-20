/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : 17003UI.c
  * Version     : V0.1
  * Description : 17003Ѫ����Ŀ�˻�����������Ƴ���
  * Date        : 2017��10��19��
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#ifndef _17003UI_H_
#define _17003UI_H_


/*********************File include*********************/
#include "SysWork.h"
#include "DW_Agreement.h"

/******************Macro definition********************/
#define RESULT_NUM          13                    //������͸���
#define RESULT_NAME_BYTE   (4*2)                  //������б��н����������ֽ���
#define COLUMN_NUM          6                     //�������ʾ������
#define RESULT_BYTE        (8*2)                  //�����������������ֽ���
#define ROW_NUM             11                    //�б���ʾ������

#define MAIN_PAGE						
#define TEST_PAGE

/*********Enumeration variable type definition********/




/**************Structure type definition**************/



/*****************Function declaration**************/
void LCD_Decode(DW_Struct *lcd);                        //���뺯��
void LCD_StartBar(DW_Struct *lcd);                      //��������

void LCD_RefreshSampleSeat(int rack_num,int seat_num);  //����Sample[]ˢ��ĳһָ������λ
void LCD_RefreshEditSampleSeat(void);                   //����EditSample[]ˢ������λ
void LCD_SampleEdit(int addr,int value);                //�༭������Ϣ
void LCD_RefreshEditTestList(void);                     //ˢ����ѡ��Ŀ�б�
void LCD_DeletTestList(int addr);                       //����ѡ��Ŀ�б���ɾ��һ����Ŀ
void LCD_CupSeatEdit(int addr,int value);               //�༭����λ
void LCD_RefreshCupSeat(int num);                       //����CupSeat[]�е���Ϣˢ��һ��ָ������λ����ʾ
void LCD_ReagentEdit(int addr,int value);               //�༭�Լ�λ
void LCD_RefreshReagentSeat(int num);                   //����Reagent[]�е���Ϣˢ��һ��ָ���Լ�λ����ʾ


void LCD_RefreshResultName(void);                       //��ʾ����б�Ľ������
void LCD_RefreshResultTabe(int row);                    //����Sample[]�еĽ��ˢ��һ��ָ������
void LCD_TestCurveCtrl(int addr,int value);             //��Ӧ������ؿ���
void LCD_RefreshTestCurve(int sample_cnt,int test_type);//ˢ�߷�Ӧ���������ʾ


void LCD_CalibrationCtrl(int addr,int value);           //������صĿ���
void LCD_ResultEdit(int addr,int value); 

 #endif
/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/
