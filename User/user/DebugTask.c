/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : DebugTask.c
  * Version     : V0.1
  * Description : 血凝仪系统调试任务
  * Date        : 2019年6月20日
  * Author      : XYD
  * Check       :  
*******************************************************************************/


/*********************File include*********************/
#include "DebugTask.h"


//系统调试任务
//任务优先级
#define DEBUG_TASK_PRIO		4
//任务堆栈大小	
#define DEBUG_STK_SIZE 		128
//任务控制块
OS_TCB DebugTaskTCB;
//任务堆栈	
CPU_STK DEBUG_TASK_STK[DEBUG_STK_SIZE];
//任务函数
void DebugTask(void *p_arg);


/**************************************************************************************
  * name        : void DebugTask(void *p_arg)
  * Function    : 系统调试任务
  * Parameters  : Null
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/

/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/ 


