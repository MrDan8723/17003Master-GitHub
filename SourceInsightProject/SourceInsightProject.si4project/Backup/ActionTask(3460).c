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
#define ACTION_TASK_PRIO		3
//任务堆栈大小	
#define ACTION_STK_SIZE 		128
//任务控制块
OS_TCB ActionTaskTCB;
//任务堆栈	
CPU_STK ACTION_TASK_STK[ACTION_STK_SIZE];
//任务函数
void ActionTask(void *p_arg);

OS_FLAG_GRP ActionFlag;                         //三轴、柱塞泵、蠕动泵共用的事件标志组，指示动作进行状态



float SamplePosBase[4];                                  //样本架基地址，第一个样本位坐标
float CupPosBase[4];                                     //供杯位基地址，两个供杯位块的左上角第一个供杯位
float ReagentPosBase[4];                                 //试剂位基地址，最左边一个大试剂位，左上角第一个小试剂位
float DetectPosBase[12];                                 //检测位基地址
float WashPosBase[2];                                    //清洗位基地址
float DiscardPosBase[2];                                 //丢杯位基地址



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

/**************************************************************************************
  * name        : void ActionTaskInit(void)
  * Function    : 动作控制任务初始化
  * Parameters  : Null
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void ActionTaskInit(void)
{

}
/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/ 
