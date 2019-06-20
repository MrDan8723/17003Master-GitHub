/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : QCTask.c
  * Version     : V0.1
  * Description : 血凝仪质控任务
  * Date        : 2019年6月20日
  * Author      : XYD
  * Check       :  
*******************************************************************************/


/*********************File include*********************/
#include "QCTask.h"


//质控任务
//任务优先级
#define QC_TASK_PRIO		4
//任务堆栈大小	
#define QC_STK_SIZE 		128
//任务控制块
OS_TCB QCTaskTCB;
//任务堆栈	
CPU_STK QC_TASK_STK[QC_STK_SIZE];
//任务函数
void QCTask(void *p_arg);


/**************************************************************************************
  * name        : void QCTask(void *p_arg)
  * Function    : 动作控制任务
  * Parameters  : Null
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/

/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/ 



