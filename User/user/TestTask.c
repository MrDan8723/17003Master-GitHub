/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : TestTask.c
  * Version     : V0.1
  * Description : 血凝仪机械检测块管理源文件
  * Date        : 2019年6月18日
  * Author      : XYD
  * Check       :  
*******************************************************************************/

/*********************File include*********************/
#include "TestTask.h"


//检测块0任务
//任务优先级
#define TEST0_TASK_PRIO		5
//任务堆栈大小	
#define TEST0_STK_SIZE 		128
//任务控制块
OS_TCB Test0TaskTCB;
//任务堆栈	
CPU_STK TEST0_TASK_STK[TEST0_STK_SIZE];
//任务函数
void Test0Task(void *p_arg);

//检测块1任务
//任务优先级
#define TEST1_TASK_PRIO		5
//任务堆栈大小	
#define TEST1_STK_SIZE 		128
//任务控制块
OS_TCB Test1TaskTCB;
//任务堆栈	
CPU_STK TEST1_TASK_STK[TEST1_STK_SIZE];
//任务函数
void Test1Task(void *p_arg);

//检测块11任务
//任务优先级
#define TEST11_TASK_PRIO		5
//任务堆栈大小	
#define TEST11_STK_SIZE 		128
//任务控制块
OS_TCB Test11TaskTCB;
//任务堆栈	
CPU_STK TEST11_TASK_STK[TEST11_STK_SIZE];
//任务函数
void Test11Task(void *p_arg);


OS_Q TestMsg;                     //检测任务消息队列


TestMethodInfoStruct TestMethodInfo[7];

/**************************************************************************************
  * name        : void TestTaskInit(void)
  * Function    : 所有检测块任务相关参数初始化
  * Parameters  : Null
  * Returns     : Null
  * Note        : 该函数只能被start_task任务在临界区内调用
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void TestTaskInit(void)
{
  OS_ERR err;
	
	//创建检测块0任务
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

  //创建检测块1任务
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

  //创建检测块11任务
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


  //创建消息队列TestMsg
  OSQCreate((OS_Q*    	  )&TestMsg,            //消息队列
            (CPU_CHAR*    )"Test Msg",          //消息队列名称
            (OS_MSG_QTY   )12,                  //消息队列长度
            (OS_ERR*      )&err);               //错误码

  //默认测试方法初始化
  //TT
  TestMethodInfo[TT].test_type= TT;                       //检测类型TT
  TestMethodInfo[TT].algorithm_type= THRESHOLD_METHOD;    //算法类型-阈值法
  TestMethodInfo[TT].ch_type= S_CHANNLE;                  //散射通道
  TestMethodInfo[TT].sample_vol= 50;                      //样本体积50ul
  TestMethodInfo[TT].r1_type= TT_R1;                      //试剂1类型，TT试剂
  TestMethodInfo[TT].r1_vol= 100;                         //试剂体积100ul
  TestMethodInfo[TT].incubation_time1= 180;               //孵育时间180s

  //PT
  TestMethodInfo[PT].test_type= PT;                       //检测类型PT
  TestMethodInfo[PT].algorithm_type= THRESHOLD_METHOD;    //算法类型-阈值法
  TestMethodInfo[PT].ch_type= S_CHANNLE;                  //散射通道
  TestMethodInfo[PT].sample_vol= 50;                      //样本体积50ul
  TestMethodInfo[PT].r1_type= PT_R1;                      //试剂1类型，PT试剂
  TestMethodInfo[PT].r1_vol= 100;                         //试剂体积100ul
  TestMethodInfo[PT].incubation_time1= 180;               //孵育时间180s

  //APTT

  //FIB

  //DD

  //FDP

  //AT3
	
	

}


/**************************************************************************************
  * name        : void Test0Task(void)
  * Function    : 检测块0检测任务
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
    
    //step1:等待TestMsg消息队列中的一个消息
    //成功获取消息后才开始新一项检测
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
  * Function    : 检测块1检测任务
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
    
    //step1:等待TestMsg消息队列中的一个消息
    //成功获取消息后才开始新一项检测
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
  * Function    : 检测块11检测任务
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
    
    //step1:等待TestMsg消息队列中的一个消息
    //成功获取消息后才开始新一项检测
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
  * Function    : TT检测方法
  * Parameters  : u8 test_num-- 使用的检测位编号
                  TestMsgStruct *test_msg-- 包含检测信息的消息
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void TT_Test(u8 test_num, TestMsgStruct *test_msg) 
{
  OS_ERR err;
  ActionMsgStruct action_msg;   //动作消息队列的消息内容
  u8 cup_num;                   //记录所使用的空杯位编号
  u16 delay;

  //step1:根据已知信息整理一个新的取样动作消息
  cup_num= ApplyCup();                                                  //申请一个空杯
  action_msg.type= 1;                                                   //动作类型，取样动作
  action_msg.Data[0]= test_msg->sample_seat;                             //样本位编号
  action_msg.Data[1]= TestMethodInfo[test_msg->test_type].sample_vol;    //样本体积
  action_msg.Data[2]= cup_num;                                          //使用的空杯编号
  action_msg.Data[3]= test_num;                                         //检测位编号

  //step2:向ActionTask的任务内建消息队列发送一个已经打包好的取样动作消息
  //ActionTask可以根据该消息内容完成取样动作，并反馈一个信号量
  OSTaskQPost((OS_TCB*        )&ActionTaskTCB, 
              (void*          )&action_msg,
              (OS_MSG_SIZE    )ACTION_MSG_SIZE,
              (OS_OPT         )OS_OPT_POST_FIFO,                        //先进先出模式
              (OS_ERR*        )&err);

  //step3:请求任务内嵌信号量
  //ActionTask任务在完成本任务指定动作后将会发送该信号量
  OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);                         //请求任务内嵌信号量

  //step4:延时，完成孵育动作
  delay= TestMethodInfo[test_msg->test_type].incubation_time1;           //从测试方法中取得第一个孵育时间
  OSTimeDlyHMSM(0,0,delay,0,OS_OPT_TIME_HMSM_STRICT,&err);               //延时,完成孵育动作

  //step5:加试剂1，整理加试剂动作所需的消息内容
  action_msg.type= 2;                                                   //动作类型，加试剂动作
  action_msg.Data[0]= TestMethodInfo[test_msg->test_type].r1_type;       //从测试方法中取得第一种试剂类型
  action_msg.Data[1]= TestMethodInfo[test_msg->test_type].r1_vol;        //从测试方法中取得第一种试剂体积
  action_msg.Data[2]= test_num;                                         //检测位编号

  //step6:向ActionTask的任务内建消息队列发送一个已经打包好的加试剂动作消息
  //ActionTask可以根据该消息内容完成加试剂动作，并反馈一个信号量
  OSTaskQPost((OS_TCB*        )&ActionTaskTCB,
              (void*          )&action_msg,
              (OS_MSG_SIZE    )ACTION_MSG_SIZE,
              (OS_OPT         )OS_OPT_POST_LIFO,                        //后进先出模式
              (OS_ERR*        )&err);

  //step7:请求任务内嵌信号量
  //ActionTask任务在完成本任务指定动作后将会发送该信号量
  OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);                         //请求任务内嵌信号量

  //step8:向数据采集板发送数据采集命令
  //此处暂时用延时模拟
  OSTimeDlyHMSM(0,0,180,0,OS_OPT_TIME_HMSM_STRICT,&err);

  //step9:请求任务内嵌信号量
  //数据采集板在完成检测任务后将会发送该信号量
//  OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);                         //请求任务内嵌信号量

  //step10:丢杯动作，整理丢杯动作所需的消息内容
  action_msg.type= 4;                                                   //动作类型，丢杯动作
  action_msg.Data[0]= test_num;                                         //检测位编号

  //step11:向ActionTask的任务内建消息队列发送一个已经打包好的丢杯动作消息
  //ActionTask可以根据该消息内容完成丢杯动作，并反馈一个信号量
  OSTaskQPost((OS_TCB*        )&ActionTaskTCB,
              (void*          )&action_msg,
              (OS_MSG_SIZE    )ACTION_MSG_SIZE,
              (OS_OPT         )OS_OPT_POST_FIFO,                        //先进先出模式
              (OS_ERR*        )&err);
  
  //step12:请求任务内嵌信号量
  //ActionTask任务在完成本任务指定动作后将会发送该信号量
  OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);                         //请求任务内嵌信号量

  //step13:本次检测结束，通过消息将检测结果反馈给主任务
}

/**************************************************************************************
  * name        : void PT_Test(u8 test_num, TestMsgStruct *test_msg)
  * Function    : PT检测方法
  * Parameters  : u8 test_num-- 使用的检测位编号
                  TestMsgStruct *test_msg-- 包含检测信息的消息
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void PT_Test(u8 test_num, TestMsgStruct *test_msg)
{
  OS_ERR err;
  ActionMsgStruct action_msg;   //动作消息队列的消息内容
  u8 cup_num;                   //记录所使用的空杯位编号
  u16 delay;

  //step1:根据已知信息整理一个新的取样动作消息
  cup_num= ApplyCup();                                                  //申请一个空杯
  action_msg.type= 1;                                                   //动作类型，取样动作
  action_msg.Data[0]= test_msg->sample_seat;                            //样本位编号
  action_msg.Data[1]= TestMethodInfo[test_msg->test_type].sample_vol;   //样本体积
  action_msg.Data[2]= cup_num;                                          //使用的空杯编号
  action_msg.Data[3]= test_num;                                         //检测位编号

  //step2:向ActionTask的任务内建消息队列发送一个已经打包好的取样动作消息
  //ActionTask可以根据该消息内容完成取样动作，并反馈一个信号量
  OSTaskQPost((OS_TCB*        )&ActionTaskTCB,
              (void*          )&action_msg,
              (OS_MSG_SIZE    )ACTION_MSG_SIZE,
              (OS_OPT         )OS_OPT_POST_FIFO,                        //先进先出模式
              (OS_ERR*        )&err);

  //step3:请求任务内嵌信号量
  //ActionTask任务在完成本任务指定动作后将会发送该信号量
  OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);                         //请求任务内嵌信号量

  //step4:延时，完成孵育动作
  delay= TestMethodInfo[test_msg->test_type].incubation_time1;           //从测试方法中取得第一个孵育时间
  OSTimeDlyHMSM(0,0,delay,0,OS_OPT_TIME_HMSM_STRICT,&err);               //延时,完成孵育动作

  //step5:加试剂1，整理加试剂动作所需的消息内容
  action_msg.type= 2;                                                   //动作类型，加试剂动作
  action_msg.Data[0]= TestMethodInfo[test_msg->test_type].r1_type;       //从测试方法中取得第一种试剂类型
  action_msg.Data[1]= TestMethodInfo[test_msg->test_type].r1_vol;        //从测试方法中取得第一种试剂体积
  action_msg.Data[2]= test_num;                                         //检测位编号

  //step6:向ActionTask的任务内建消息队列发送一个已经打包好的加试剂动作消息
  //ActionTask可以根据该消息内容完成加试剂动作，并反馈一个信号量
  OSTaskQPost((OS_TCB*        )&ActionTaskTCB,
              (void*          )&action_msg,
              (OS_MSG_SIZE    )ACTION_MSG_SIZE,
              (OS_OPT         )OS_OPT_POST_LIFO,                        //后进先出模式
              (OS_ERR*        )&err);

  //step7:请求任务内嵌信号量
  //ActionTask任务在完成本任务指定动作后将会发送该信号量
  OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);                         //请求任务内嵌信号量

  //step8:向数据采集板发送数据采集命令
  //此处暂时用延时模拟
  OSTimeDlyHMSM(0,0,180,0,OS_OPT_TIME_HMSM_STRICT,&err);

  //step9:请求任务内嵌信号量
  //数据采集板在完成检测任务后将会发送该信号量
//  OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);                         //请求任务内嵌信号量

  //step10:丢杯动作，整理丢杯动作所需的消息内容
  action_msg.type= 4;                                                   //动作类型，丢杯动作
  action_msg.Data[0]= test_num;                                         //检测位编号

  //step11:向ActionTask的任务内建消息队列发送一个已经打包好的丢杯动作消息
  //ActionTask可以根据该消息内容完成丢杯动作，并反馈一个信号量
  OSTaskQPost((OS_TCB*        )&ActionTaskTCB,
              (void*          )&action_msg,
              (OS_MSG_SIZE    )ACTION_MSG_SIZE,
              (OS_OPT         )OS_OPT_POST_FIFO,                        //先进先出模式
              (OS_ERR*        )&err);
  
  //step12:请求任务内嵌信号量
  //ActionTask任务在完成本任务指定动作后将会发送该信号量
  OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);                         //请求任务内嵌信号量

  //step13:本次检测结束，通过消息将检测结果反馈给主任务
}

/**************************************************************************************
  * name        : void APTT_Test(u8 test_num, TestMsgStruct *test_msg)
  * Function    : APTT检测方法
  * Parameters  : u8 test_num-- 使用的检测位编号
                  TestMsgStruct *test_msg-- 包含检测信息的消息
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
  * Function    : FIB检测方法
  * Parameters  : u8 test_num-- 使用的检测位编号
                  TestMsgStruct *test_msg-- 包含检测信息的消息
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
  * Function    : D-二聚体检测方法
  * Parameters  : u8 test_num-- 使用的检测位编号
                  TestMsgStruct *test_msg-- 包含检测信息的消息
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
  * Function    : FDP检测方法
  * Parameters  : u8 test_num-- 使用的检测位编号
                  TestMsgStruct *test_msg-- 包含检测信息的消息
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
  * Function    : AT3检测方法
  * Parameters  : u8 test_num-- 使用的检测位编号
                  TestMsgStruct *test_msg-- 包含检测信息的消息
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void AT3_Test(u8 test_num, TestMsgStruct *test_msg)
{

}
/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/

