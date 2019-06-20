/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : MNTC_IOCtrl.h
  * Version     : V0.1
  * Description : MNTC实验室IO口基本控制程序
  * Date        : 2019年5月7日
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#ifndef _MNTC_IO_CTRL_H_
#define _MNTC_IO_CTRL_H_


/*********************File include*********************/
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
/******************Macro definition********************/


/*********Enumeration variable type definition********/



/**************Structure type definition**************/
/*****************Function declaration**************/

//接口函数，接受用户调用
void GPIO_Config(GPIO_TypeDef *port,uint16_t pin,GPIOMode_TypeDef mode_type);//IO基本功能设置
void EXTI_Config(GPIO_TypeDef *port,uint16_t pin,EXTITrigger_TypeDef trigger_type);//外部中断设置


//底层函数，用户不需关心


 #endif
/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/ 
 
 
 