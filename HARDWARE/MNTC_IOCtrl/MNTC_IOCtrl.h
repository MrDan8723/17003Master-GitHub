/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : MNTC_IOCtrl.h
  * Version     : V0.1
  * Description : MNTCʵ����IO�ڻ������Ƴ���
  * Date        : 2019��5��7��
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

//�ӿں����������û�����
void GPIO_Config(GPIO_TypeDef *port,uint16_t pin,GPIOMode_TypeDef mode_type);//IO������������
void EXTI_Config(GPIO_TypeDef *port,uint16_t pin,EXTITrigger_TypeDef trigger_type);//�ⲿ�ж�����


//�ײ㺯�����û��������


 #endif
/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/ 
 
 
 