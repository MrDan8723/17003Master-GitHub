/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : DebugTask.c
  * Version     : V0.1
  * Description : Ѫ����ϵͳ��������
  * Date        : 2019��6��20��
  * Author      : XYD
  * Check       :  
*******************************************************************************/


/*********************File include*********************/
#include "DebugTask.h"


//ϵͳ��������
//�������ȼ�
#define DEBUG_TASK_PRIO		4
//�����ջ��С	
#define DEBUG_STK_SIZE 		128
//������ƿ�
OS_TCB DebugTaskTCB;
//�����ջ	
CPU_STK DEBUG_TASK_STK[DEBUG_STK_SIZE];
//������
void DebugTask(void *p_arg);


/**************************************************************************************
  * name        : void DebugTask(void *p_arg)
  * Function    : ϵͳ��������
  * Parameters  : Null
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/

/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/ 


