/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : TestManageTask.c
  * Version     : V0.1
  * Description : Ѫ���Ǽ���������
  * Date        : 2019��6��20��
  * Author      : XYD
  * Check       :  
*******************************************************************************/


/*********************File include*********************/
#include "TestManageTask.h"

//����������
//�������ȼ�
#define TEST_MANAGE_TASK_PRIO		4
//�����ջ��С	
#define TEST_MANAGE_STK_SIZE 		128
//������ƿ�
OS_TCB TestManageTaskTCB;
//�����ջ	
CPU_STK TEST_MANAGE_TASK_STK[TEST_MANAGE_STK_SIZE];
//������
void TestManageTask(void *p_arg);



/**************************************************************************************
  * name        : u8 ApplyCup()
  * Function    : ����һ���ձ�
  * Parameters  : Null
  * Returns     : u8 -- ����һ�����õĿձ�λ���
  * Note        : �ձ���Դ��TestManageTaskͳһ����
  * Author      : XYD
  * Check       : 
**************************************************************************************/
u8 ApplyCup()
{
	
}
/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/ 




