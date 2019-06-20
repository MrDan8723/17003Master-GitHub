/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : TestManageTask.c
  * Version     : V0.1
  * Description : 血凝仪检测管理任务
  * Date        : 2019年6月20日
  * Author      : XYD
  * Check       :  
*******************************************************************************/


/*********************File include*********************/
#include "TestManageTask.h"

//检测管理任务
//任务优先级
#define TEST_MANAGE_TASK_PRIO		4
//任务堆栈大小	
#define TEST_MANAGE_STK_SIZE 		128
//任务控制块
OS_TCB TestManageTaskTCB;
//任务堆栈	
CPU_STK TEST_MANAGE_TASK_STK[TEST_MANAGE_STK_SIZE];
//任务函数
void TestManageTask(void *p_arg);



/**************************************************************************************
  * name        : u8 ApplyCup()
  * Function    : 申请一个空杯
  * Parameters  : Null
  * Returns     : u8 -- 返回一个可用的空杯位编号
  * Note        : 空杯资源由TestManageTask统一管理
  * Author      : XYD
  * Check       : 
**************************************************************************************/
u8 ApplyCup()
{
	
}
/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/ 




