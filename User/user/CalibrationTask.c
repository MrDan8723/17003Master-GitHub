/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : CalibrationTask.c
  * Version     : V0.1
  * Description : Ѫ���Ƕ�������
  * Date        : 2019��6��20��
  * Author      : XYD
  * Check       :  
*******************************************************************************/


/*********************File include*********************/
#include "CalibrationTask.h"


//��������
//�������ȼ�
#define CALIBRATION_TASK_PRIO		4
//�����ջ��С	
#define CALIBRATION_STK_SIZE 		128
//������ƿ�
OS_TCB CalibrationTaskTCB;
//�����ջ	
CPU_STK CALIBRATION_TASK_STK[CALIBRATION_STK_SIZE];
//������
void CalibrationTask(void *p_arg);



/**************************************************************************************
  * name        : void CalibrationTask(void *p_arg)
  * Function    : ��������
  * Parameters  : Null
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/

/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/ 



