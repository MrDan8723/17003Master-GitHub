/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : 17003UI.c
  * Version     : V0.1
  * Description : 17003血凝项目人机交互界面控制程序
  * Date        : 2017年10月19日
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#ifndef _17003UI_H_
#define _17003UI_H_


/*********************File include*********************/
#include "SysWork.h"
#include "DW_Agreement.h"

/******************Macro definition********************/
#define RESULT_NUM          13                    //结果类型个数
#define RESULT_NAME_BYTE   (4*2)                  //检测结果列表中结果名称最大字节数
#define COLUMN_NUM          6                     //检测结果显示多少列
#define RESULT_BYTE        (8*2)                  //检测结果和样本号最大字节数
#define ROW_NUM             11                    //列表显示多少行

#define MAIN_PAGE						
#define TEST_PAGE

/*********Enumeration variable type definition********/




/**************Structure type definition**************/



/*****************Function declaration**************/
void LCD_Decode(DW_Struct *lcd);                        //解码函数
void LCD_StartBar(DW_Struct *lcd);                      //开机动画

void LCD_RefreshSampleSeat(int rack_num,int seat_num);  //根据Sample[]刷新某一指定样本位
void LCD_RefreshEditSampleSeat(void);                   //根据EditSample[]刷新样本位
void LCD_SampleEdit(int addr,int value);                //编辑样本信息
void LCD_RefreshEditTestList(void);                     //刷新已选项目列表
void LCD_DeletTestList(int addr);                       //从已选项目列表中删除一个项目
void LCD_CupSeatEdit(int addr,int value);               //编辑供杯位
void LCD_RefreshCupSeat(int num);                       //根据CupSeat[]中的信息刷新一个指定供杯位的显示
void LCD_ReagentEdit(int addr,int value);               //编辑试剂位
void LCD_RefreshReagentSeat(int num);                   //根据Reagent[]中的信息刷新一个指定试剂位的显示


void LCD_RefreshResultName(void);                       //显示结果列表的结果名称
void LCD_RefreshResultTabe(int row);                    //根据Sample[]中的结果刷新一个指定的行
void LCD_TestCurveCtrl(int addr,int value);             //反应曲线相关控制
void LCD_RefreshTestCurve(int sample_cnt,int test_type);//刷线反应曲线相关显示


void LCD_CalibrationCtrl(int addr,int value);           //定标相关的控制
void LCD_ResultEdit(int addr,int value); 

 #endif
/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/
