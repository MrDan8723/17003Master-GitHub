/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : QCTask.c
  * Version     : V0.1
  * Description : Ѫ�����ʿ�����
  * Date        : 2019��6��20��
  * Author      : XYD
  * Check       :  
*******************************************************************************/


/*********************File include*********************/
#include "QCTask.h"


//�ʿ�����
//�������ȼ�
#define QC_TASK_PRIO		4
//�����ջ��С	
#define QC_STK_SIZE 		128
//������ƿ�
OS_TCB QCTaskTCB;
//�����ջ	
CPU_STK QC_TASK_STK[QC_STK_SIZE];
//������
void QCTask(void *p_arg);


/**************************************************************************************
  * name        : void QCTask(void *p_arg)
  * Function    : ������������
  * Parameters  : Null
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/

/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/ 



