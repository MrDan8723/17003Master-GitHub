/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : ActionTask.c
  * Version     : V0.1
  * Description : 血凝仪机械动作控制源文件
  * Date        : 2019年6月18日
  * Author      : XYD
  * Check       :  
*******************************************************************************/

/*********************File include*********************/
#include "ActionTask.h"

//动作控制任务
//任务优先级
#define ACTION_TASK_PRIO		8
//任务堆栈大小	
#define ACTION_STK_SIZE 		512
//任务控制块
OS_TCB ActionTaskTCB;
//任务堆栈	
CPU_STK ACTION_TASK_STK[ACTION_STK_SIZE];
//任务函数
void ActionTask(void *p_arg);

OS_FLAG_GRP ActionFlag;                         //三轴、柱塞泵、蠕动泵共用的事件标志组，指示动作进行状态

/**************************************************************************************
  * name        : void ActionTask(void *p_arg)
  * Function    : 动作控制任务
  * Parameters  : Null
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void ActionTask(void *p_arg)
{
  OS_ERR err;
  ActionMsgStruct *action_msg;

  while(1)
  {
    //从任务内建消息队列中等待一个消息
    action_msg= OSTaskQPend((OS_TICK     )0,
														(OS_OPT      )OS_OPT_PEND_BLOCKING,
														(OS_MSG_SIZE*)ACTION_MSG_SIZE,
														(CPU_TS*     )0,
														(OS_ERR*     )&err);
    switch(action_msg->type)
    {
			case 0: break;
      default  : break;
    }


    
  }

}
/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/ 
