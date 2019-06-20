/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : ActionTask.c
  * Version     : V0.1
  * Description : Ѫ���ǻ�е��������Դ�ļ�
  * Date        : 2019��6��18��
  * Author      : XYD
  * Check       :  
*******************************************************************************/

/*********************File include*********************/
#include "ActionTask.h"

//������������
//�������ȼ�
#define ACTION_TASK_PRIO		8
//�����ջ��С	
#define ACTION_STK_SIZE 		512
//������ƿ�
OS_TCB ActionTaskTCB;
//�����ջ	
CPU_STK ACTION_TASK_STK[ACTION_STK_SIZE];
//������
void ActionTask(void *p_arg);

OS_FLAG_GRP ActionFlag;                         //���ᡢ�����á��䶯�ù��õ��¼���־�飬ָʾ��������״̬

/**************************************************************************************
  * name        : void ActionTask(void *p_arg)
  * Function    : ������������
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
    //�������ڽ���Ϣ�����еȴ�һ����Ϣ
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
