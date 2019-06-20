/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : TestTask.c
  * Version     : V0.1
  * Description : Ѫ���ǻ�е�������Դ�ļ�
  * Date        : 2019��6��18��
  * Author      : XYD
  * Check       :  
*******************************************************************************/

/*********************File include*********************/
#include "TestTask.h"


//����0����
//�������ȼ�
#define TEST0_TASK_PRIO		5
//�����ջ��С	
#define TEST0_STK_SIZE 		128
//������ƿ�
OS_TCB Test0TaskTCB;
//�����ջ	
CPU_STK TEST0_TASK_STK[TEST0_STK_SIZE];
//������
void Test0Task(void *p_arg);

//����1����
//�������ȼ�
#define TEST1_TASK_PRIO		5
//�����ջ��С	
#define TEST1_STK_SIZE 		128
//������ƿ�
OS_TCB Test1TaskTCB;
//�����ջ	
CPU_STK TEST1_TASK_STK[TEST1_STK_SIZE];
//������
void Test1Task(void *p_arg);

//����11����
//�������ȼ�
#define TEST11_TASK_PRIO		5
//�����ջ��С	
#define TEST11_STK_SIZE 		128
//������ƿ�
OS_TCB Test11TaskTCB;
//�����ջ	
CPU_STK TEST11_TASK_STK[TEST11_STK_SIZE];
//������
void Test11Task(void *p_arg);


OS_Q TestMsg;                     //���������Ϣ����


TestMethodInfoStruct TestMethodInfo[7];

/**************************************************************************************
  * name        : void TestTaskInit(void)
  * Function    : ���м���������ز�����ʼ��
  * Parameters  : Null
  * Returns     : Null
  * Note        : �ú���ֻ�ܱ�start_task�������ٽ����ڵ���
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void TestTaskInit(void)
{
  OS_ERR err;
	
	//��������0����
	OSTaskCreate((OS_TCB 		 * )&Test0TaskTCB,		
							 (CPU_CHAR	 * )"test0 task", 		
							 (OS_TASK_PTR  )Test0Task, 			
							 (void			 * )0,					
							 (OS_PRIO	  	 )TEST0_TASK_PRIO,       
							 (CPU_STK    * )&TEST0_TASK_STK[0],	
							 (CPU_STK_SIZE )TEST0_STK_SIZE/10,	
							 (CPU_STK_SIZE )TEST0_STK_SIZE,		
							 (OS_MSG_QTY   )0,					
							 (OS_TICK	     )0,					
							 (void   		 * )0,					
							 (OS_OPT       )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
							 (OS_ERR 		 * )&err);

  //��������1����
	OSTaskCreate((OS_TCB 		 * )&Test1TaskTCB,		
							 (CPU_CHAR	 * )"test1 task", 		
							 (OS_TASK_PTR  )Test1Task, 			
							 (void			 * )0,					
							 (OS_PRIO	  	 )TEST1_TASK_PRIO,       
							 (CPU_STK    * )&TEST1_TASK_STK[0],	
							 (CPU_STK_SIZE )TEST1_STK_SIZE/10,	
							 (CPU_STK_SIZE )TEST1_STK_SIZE,		
							 (OS_MSG_QTY   )0,					
							 (OS_TICK	     )0,					
							 (void   		 * )0,					
							 (OS_OPT       )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
							 (OS_ERR 		 * )&err);

  //��������11����
	OSTaskCreate((OS_TCB 		 * )&Test11TaskTCB,		
							 (CPU_CHAR	 * )"test11 task", 		
							 (OS_TASK_PTR  )Test11Task, 			
							 (void			 * )0,					
							 (OS_PRIO	  	 )TEST11_TASK_PRIO,       
							 (CPU_STK    * )&TEST11_TASK_STK[0],	
							 (CPU_STK_SIZE )TEST11_STK_SIZE/10,	
							 (CPU_STK_SIZE )TEST11_STK_SIZE,		
							 (OS_MSG_QTY   )0,					
							 (OS_TICK	     )0,					
							 (void   		 * )0,					
							 (OS_OPT       )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
							 (OS_ERR 		 * )&err);


  //������Ϣ����TestMsg
  OSQCreate((OS_Q*    	  )&TestMsg,            //��Ϣ����
            (CPU_CHAR*    )"Test Msg",          //��Ϣ��������
            (OS_MSG_QTY   )12,                  //��Ϣ���г���
            (OS_ERR*      )&err);               //������

  //Ĭ�ϲ��Է�����ʼ��
  //TT
  TestMethodInfo[TT].test_type= TT;                       //�������TT
  TestMethodInfo[TT].algorithm_type= THRESHOLD_METHOD;    //�㷨����-��ֵ��
  TestMethodInfo[TT].ch_type= S_CHANNLE;                  //ɢ��ͨ��
  TestMethodInfo[TT].sample_vol= 50;                      //�������50ul
  TestMethodInfo[TT].r1_type= TT_R1;                      //�Լ�1���ͣ�TT�Լ�
  TestMethodInfo[TT].r1_vol= 100;                         //�Լ����100ul
  TestMethodInfo[TT].incubation_time1= 180;               //����ʱ��180s

  //PT
  TestMethodInfo[PT].test_type= PT;                       //�������PT
  TestMethodInfo[PT].algorithm_type= THRESHOLD_METHOD;    //�㷨����-��ֵ��
  TestMethodInfo[PT].ch_type= S_CHANNLE;                  //ɢ��ͨ��
  TestMethodInfo[PT].sample_vol= 50;                      //�������50ul
  TestMethodInfo[PT].r1_type= PT_R1;                      //�Լ�1���ͣ�PT�Լ�
  TestMethodInfo[PT].r1_vol= 100;                         //�Լ����100ul
  TestMethodInfo[PT].incubation_time1= 180;               //����ʱ��180s

  //APTT

  //FIB

  //DD

  //FDP

  //AT3
	
	

}


/**************************************************************************************
  * name        : void Test0Task(void)
  * Function    : ����0�������
  * Parameters  : Null
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void Test0Task(void *p_arg)
{
  OS_ERR err;
  TestMsgStruct *test_msg;

  while(1)
  {
    
    //step1:�ȴ�TestMsg��Ϣ�����е�һ����Ϣ
    //�ɹ���ȡ��Ϣ��ſ�ʼ��һ����
    test_msg= OSQPend((OS_Q*       )&TestMsg,
                      (OS_TICK     )0,
                      (OS_OPT      )OS_OPT_PEND_BLOCKING,
                      (OS_MSG_SIZE*)TEST_MSG_SIZE,
                      (CPU_TS*     )0,
                      (OS_ERR*     )&err);
    switch(test_msg->test_type)
    {
      case TT  :  TT_Test(0, test_msg); break;
      case PT  :  PT_Test(0, test_msg); break;
      case APTT:  APTT_Test(0, test_msg); break;
      case FIB :  FIB_Test(0, test_msg); break;
      case DD  :  DD_Test(0, test_msg); break;
      case FDP :  FDP_Test(0, test_msg); break;
      case AT3 :  AT3_Test(0, test_msg); break;
      default  : break;
    }


    
  }

}

/**************************************************************************************
  * name        : void Test1Task(void)
  * Function    : ����1�������
  * Parameters  : Null
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void Test1Task(void *p_arg)
{
  OS_ERR err;
  TestMsgStruct *test_msg;

  while(1)
  {
    
    //step1:�ȴ�TestMsg��Ϣ�����е�һ����Ϣ
    //�ɹ���ȡ��Ϣ��ſ�ʼ��һ����
    test_msg= OSQPend((OS_Q*       )&TestMsg,
                      (OS_TICK     )0,
                      (OS_OPT      )OS_OPT_PEND_BLOCKING,
                      (OS_MSG_SIZE*)TEST_MSG_SIZE,
                      (CPU_TS*     )0,
                      (OS_ERR*     )&err);
    switch(test_msg->test_type)
    {
      case TT  :  TT_Test(1, test_msg); break;
      case PT  :  PT_Test(1, test_msg); break;
      case APTT:  APTT_Test(1, test_msg); break;
      case FIB :  FIB_Test(1, test_msg); break;
      case DD  :  DD_Test(1, test_msg); break;
      case FDP :  FDP_Test(1, test_msg); break;
      case AT3 :  AT3_Test(1, test_msg); break;
      default  : break;
    }


    
  }

}

/**************************************************************************************
  * name        : void Test11Task(void)
  * Function    : ����11�������
  * Parameters  : Null
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void Test11Task(void *p_arg)
{
  OS_ERR err;
  TestMsgStruct *test_msg;

  while(1)
  {
    
    //step1:�ȴ�TestMsg��Ϣ�����е�һ����Ϣ
    //�ɹ���ȡ��Ϣ��ſ�ʼ��һ����
    test_msg= OSQPend((OS_Q*       )&TestMsg,
                      (OS_TICK     )0,
                      (OS_OPT      )OS_OPT_PEND_BLOCKING,
                      (OS_MSG_SIZE*)TEST_MSG_SIZE,
                      (CPU_TS*     )0,
                      (OS_ERR*     )&err);
    switch(test_msg->test_type)
    {
      case TT  :  TT_Test(11, test_msg); break;
      case PT  :  PT_Test(11, test_msg); break;
      case APTT:  APTT_Test(11, test_msg); break;
      case FIB :  FIB_Test(11, test_msg); break;
      case DD  :  DD_Test(11, test_msg); break;
      case FDP :  FDP_Test(11, test_msg); break;
      case AT3 :  AT3_Test(11, test_msg); break;
      default  : break;
    }


    
  }

}

/**************************************************************************************
  * name        : void TT_Test(u8 test_num, TestMsgStruct *test_msg)
  * Function    : TT��ⷽ��
  * Parameters  : u8 test_num-- ʹ�õļ��λ���
                  TestMsgStruct *test_msg-- ���������Ϣ����Ϣ
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void TT_Test(u8 test_num, TestMsgStruct *test_msg) 
{
  OS_ERR err;
  ActionMsgStruct action_msg;   //������Ϣ���е���Ϣ����
  u8 cup_num;                   //��¼��ʹ�õĿձ�λ���
  u16 delay;

  //step1:������֪��Ϣ����һ���µ�ȡ��������Ϣ
  cup_num= ApplyCup();                                                  //����һ���ձ�
  action_msg.type= 1;                                                   //�������ͣ�ȡ������
  action_msg.Data[0]= test_msg->sample_seat;                             //����λ���
  action_msg.Data[1]= TestMethodInfo[test_msg->test_type].sample_vol;    //�������
  action_msg.Data[2]= cup_num;                                          //ʹ�õĿձ����
  action_msg.Data[3]= test_num;                                         //���λ���

  //step2:��ActionTask�������ڽ���Ϣ���з���һ���Ѿ�����õ�ȡ��������Ϣ
  //ActionTask���Ը��ݸ���Ϣ�������ȡ��������������һ���ź���
  OSTaskQPost((OS_TCB*        )&ActionTaskTCB, 
              (void*          )&action_msg,
              (OS_MSG_SIZE    )ACTION_MSG_SIZE,
              (OS_OPT         )OS_OPT_POST_FIFO,                        //�Ƚ��ȳ�ģʽ
              (OS_ERR*        )&err);

  //step3:����������Ƕ�ź���
  //ActionTask��������ɱ�����ָ�������󽫻ᷢ�͸��ź���
  OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);                         //����������Ƕ�ź���

  //step4:��ʱ����ɷ�������
  delay= TestMethodInfo[test_msg->test_type].incubation_time1;           //�Ӳ��Է�����ȡ�õ�һ������ʱ��
  OSTimeDlyHMSM(0,0,delay,0,OS_OPT_TIME_HMSM_STRICT,&err);               //��ʱ,��ɷ�������

  //step5:���Լ�1��������Լ������������Ϣ����
  action_msg.type= 2;                                                   //�������ͣ����Լ�����
  action_msg.Data[0]= TestMethodInfo[test_msg->test_type].r1_type;       //�Ӳ��Է�����ȡ�õ�һ���Լ�����
  action_msg.Data[1]= TestMethodInfo[test_msg->test_type].r1_vol;        //�Ӳ��Է�����ȡ�õ�һ���Լ����
  action_msg.Data[2]= test_num;                                         //���λ���

  //step6:��ActionTask�������ڽ���Ϣ���з���һ���Ѿ�����õļ��Լ�������Ϣ
  //ActionTask���Ը��ݸ���Ϣ������ɼ��Լ�������������һ���ź���
  OSTaskQPost((OS_TCB*        )&ActionTaskTCB,
              (void*          )&action_msg,
              (OS_MSG_SIZE    )ACTION_MSG_SIZE,
              (OS_OPT         )OS_OPT_POST_LIFO,                        //����ȳ�ģʽ
              (OS_ERR*        )&err);

  //step7:����������Ƕ�ź���
  //ActionTask��������ɱ�����ָ�������󽫻ᷢ�͸��ź���
  OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);                         //����������Ƕ�ź���

  //step8:�����ݲɼ��巢�����ݲɼ�����
  //�˴���ʱ����ʱģ��
  OSTimeDlyHMSM(0,0,180,0,OS_OPT_TIME_HMSM_STRICT,&err);

  //step9:����������Ƕ�ź���
  //���ݲɼ�������ɼ������󽫻ᷢ�͸��ź���
//  OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);                         //����������Ƕ�ź���

  //step10:���������������������������Ϣ����
  action_msg.type= 4;                                                   //�������ͣ���������
  action_msg.Data[0]= test_num;                                         //���λ���

  //step11:��ActionTask�������ڽ���Ϣ���з���һ���Ѿ�����õĶ���������Ϣ
  //ActionTask���Ը��ݸ���Ϣ������ɶ���������������һ���ź���
  OSTaskQPost((OS_TCB*        )&ActionTaskTCB,
              (void*          )&action_msg,
              (OS_MSG_SIZE    )ACTION_MSG_SIZE,
              (OS_OPT         )OS_OPT_POST_FIFO,                        //�Ƚ��ȳ�ģʽ
              (OS_ERR*        )&err);
  
  //step12:����������Ƕ�ź���
  //ActionTask��������ɱ�����ָ�������󽫻ᷢ�͸��ź���
  OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);                         //����������Ƕ�ź���

  //step13:���μ�������ͨ����Ϣ�������������������
}

/**************************************************************************************
  * name        : void PT_Test(u8 test_num, TestMsgStruct *test_msg)
  * Function    : PT��ⷽ��
  * Parameters  : u8 test_num-- ʹ�õļ��λ���
                  TestMsgStruct *test_msg-- ���������Ϣ����Ϣ
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void PT_Test(u8 test_num, TestMsgStruct *test_msg)
{
  OS_ERR err;
  ActionMsgStruct action_msg;   //������Ϣ���е���Ϣ����
  u8 cup_num;                   //��¼��ʹ�õĿձ�λ���
  u16 delay;

  //step1:������֪��Ϣ����һ���µ�ȡ��������Ϣ
  cup_num= ApplyCup();                                                  //����һ���ձ�
  action_msg.type= 1;                                                   //�������ͣ�ȡ������
  action_msg.Data[0]= test_msg->sample_seat;                            //����λ���
  action_msg.Data[1]= TestMethodInfo[test_msg->test_type].sample_vol;   //�������
  action_msg.Data[2]= cup_num;                                          //ʹ�õĿձ����
  action_msg.Data[3]= test_num;                                         //���λ���

  //step2:��ActionTask�������ڽ���Ϣ���з���һ���Ѿ�����õ�ȡ��������Ϣ
  //ActionTask���Ը��ݸ���Ϣ�������ȡ��������������һ���ź���
  OSTaskQPost((OS_TCB*        )&ActionTaskTCB,
              (void*          )&action_msg,
              (OS_MSG_SIZE    )ACTION_MSG_SIZE,
              (OS_OPT         )OS_OPT_POST_FIFO,                        //�Ƚ��ȳ�ģʽ
              (OS_ERR*        )&err);

  //step3:����������Ƕ�ź���
  //ActionTask��������ɱ�����ָ�������󽫻ᷢ�͸��ź���
  OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);                         //����������Ƕ�ź���

  //step4:��ʱ����ɷ�������
  delay= TestMethodInfo[test_msg->test_type].incubation_time1;           //�Ӳ��Է�����ȡ�õ�һ������ʱ��
  OSTimeDlyHMSM(0,0,delay,0,OS_OPT_TIME_HMSM_STRICT,&err);               //��ʱ,��ɷ�������

  //step5:���Լ�1��������Լ������������Ϣ����
  action_msg.type= 2;                                                   //�������ͣ����Լ�����
  action_msg.Data[0]= TestMethodInfo[test_msg->test_type].r1_type;       //�Ӳ��Է�����ȡ�õ�һ���Լ�����
  action_msg.Data[1]= TestMethodInfo[test_msg->test_type].r1_vol;        //�Ӳ��Է�����ȡ�õ�һ���Լ����
  action_msg.Data[2]= test_num;                                         //���λ���

  //step6:��ActionTask�������ڽ���Ϣ���з���һ���Ѿ�����õļ��Լ�������Ϣ
  //ActionTask���Ը��ݸ���Ϣ������ɼ��Լ�������������һ���ź���
  OSTaskQPost((OS_TCB*        )&ActionTaskTCB,
              (void*          )&action_msg,
              (OS_MSG_SIZE    )ACTION_MSG_SIZE,
              (OS_OPT         )OS_OPT_POST_LIFO,                        //����ȳ�ģʽ
              (OS_ERR*        )&err);

  //step7:����������Ƕ�ź���
  //ActionTask��������ɱ�����ָ�������󽫻ᷢ�͸��ź���
  OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);                         //����������Ƕ�ź���

  //step8:�����ݲɼ��巢�����ݲɼ�����
  //�˴���ʱ����ʱģ��
  OSTimeDlyHMSM(0,0,180,0,OS_OPT_TIME_HMSM_STRICT,&err);

  //step9:����������Ƕ�ź���
  //���ݲɼ�������ɼ������󽫻ᷢ�͸��ź���
//  OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);                         //����������Ƕ�ź���

  //step10:���������������������������Ϣ����
  action_msg.type= 4;                                                   //�������ͣ���������
  action_msg.Data[0]= test_num;                                         //���λ���

  //step11:��ActionTask�������ڽ���Ϣ���з���һ���Ѿ�����õĶ���������Ϣ
  //ActionTask���Ը��ݸ���Ϣ������ɶ���������������һ���ź���
  OSTaskQPost((OS_TCB*        )&ActionTaskTCB,
              (void*          )&action_msg,
              (OS_MSG_SIZE    )ACTION_MSG_SIZE,
              (OS_OPT         )OS_OPT_POST_FIFO,                        //�Ƚ��ȳ�ģʽ
              (OS_ERR*        )&err);
  
  //step12:����������Ƕ�ź���
  //ActionTask��������ɱ�����ָ�������󽫻ᷢ�͸��ź���
  OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);                         //����������Ƕ�ź���

  //step13:���μ�������ͨ����Ϣ�������������������
}

/**************************************************************************************
  * name        : void APTT_Test(u8 test_num, TestMsgStruct *test_msg)
  * Function    : APTT��ⷽ��
  * Parameters  : u8 test_num-- ʹ�õļ��λ���
                  TestMsgStruct *test_msg-- ���������Ϣ����Ϣ
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void APTT_Test(u8 test_num, TestMsgStruct *test_msg)
{

}

/**************************************************************************************
  * name        : void FIB_Test(u8 test_num, TestMsgStruct *test_msg)
  * Function    : FIB��ⷽ��
  * Parameters  : u8 test_num-- ʹ�õļ��λ���
                  TestMsgStruct *test_msg-- ���������Ϣ����Ϣ
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void FIB_Test(u8 test_num, TestMsgStruct *test_msg)
{

}

/**************************************************************************************
  * name        : void DD_Test(u8 test_num, TestMsgStruct *test_msg)
  * Function    : D-�������ⷽ��
  * Parameters  : u8 test_num-- ʹ�õļ��λ���
                  TestMsgStruct *test_msg-- ���������Ϣ����Ϣ
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void DD_Test(u8 test_num, TestMsgStruct *test_msg)
{

}

/**************************************************************************************
  * name        : void FDP_Test(u8 test_num, TestMsgStruct *test_msg)
  * Function    : FDP��ⷽ��
  * Parameters  : u8 test_num-- ʹ�õļ��λ���
                  TestMsgStruct *test_msg-- ���������Ϣ����Ϣ
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void FDP_Test(u8 test_num, TestMsgStruct *test_msg)
{

}

/**************************************************************************************
  * name        : void AT3_Test(u8 test_num, TestMsgStruct *test_msg)
  * Function    : AT3��ⷽ��
  * Parameters  : u8 test_num-- ʹ�õļ��λ���
                  TestMsgStruct *test_msg-- ���������Ϣ����Ϣ
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void AT3_Test(u8 test_num, TestMsgStruct *test_msg)
{

}
/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/

