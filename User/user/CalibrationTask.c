/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : CalibrationTask.c
  * Version     : V0.1
  * Description : 血凝仪定标任务
  * Date        : 2019年6月20日
  * Author      : XYD
  * Check       :  
*******************************************************************************/


/*********************File include*********************/
#include "CalibrationTask.h"


//定标任务
//任务优先级
#define CALIBRATION_TASK_PRIO		4
//任务堆栈大小	
#define CALIBRATION_STK_SIZE 		128
//任务控制块
OS_TCB CalibrationTaskTCB;
//任务堆栈	
CPU_STK CALIBRATION_TASK_STK[CALIBRATION_STK_SIZE];
//任务函数
void CalibrationTask(void *p_arg);



/**************************************************************************************
  * name        : void CalibrationTask(void *p_arg)
  * Function    : 定标任务
  * Parameters  : Null
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/

/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/ 



