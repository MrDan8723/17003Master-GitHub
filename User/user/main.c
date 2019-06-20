/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : main.c
  * Version     : V0.3
  * Description : 血凝仪主控板主程序源文件
  * Date        : 2019年6月18日
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#include "Common.h" 

//启动任务
//任务优先级
#define START_TASK_PRIO		3
//任务堆栈大小	
#define START_STK_SIZE 		512
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);




MNTC_CanProtocolTypeDef CanCom; 								//定义CAN通信结构体
unsigned char ComRxBuf[14];											//串口接收缓冲区	
int RxFlag;

int main(void)
{
	int cnt;
	OS_ERR err;
	CPU_SR_ALLOC();	
	delay_init();  																							//时钟初始化
	delay_ms(5000);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							//中断分组配置
	uart_init(115200);  																				//串口初始化
	LED_Init();         																				//LED初始化
	
	OSInit(&err);																								//初始化UCOSIII
	OS_CRITICAL_ENTER();																				//进入临界区
	
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,											//任务控制块
				 (CPU_CHAR	* )"start task", 													//任务名字
                 (OS_TASK_PTR )start_task, 										//任务函数
                 (void		* )0,																//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     						//任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],						//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,							//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,								//任务堆栈大小
                 (OS_MSG_QTY  )0,															//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,																//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,																//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);														//存放该函数错误时的返回值
								 
	OS_CRITICAL_EXIT();																					//退出临界区	 
	OSStart(&err);  																						//开启UCOSIII
	while(1);
}

//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	 //统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN			 //如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if OS_CFG_APP_HOOKS_EN							 //使用钩子函数
	App_OS_SetAllHooks();			
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//进入临界区
	
//	//创建取样任务
//	OSTaskCreate((OS_TCB 		 * )&LoadSampleTaskTCB,		
//							 (CPU_CHAR	 * )"load sample task", 		
//							 (OS_TASK_PTR  )LoadSampleTask, 			
//							 (void			 * )0,					
//							 (OS_PRIO	  	 )LOAD_SAMPLE_TASK_PRIO,       
//							 (CPU_STK    * )&LOAD_SAMPLE_TASK_STK[0],	
//							 (CPU_STK_SIZE )LOAD_SAMPLE_STK_SIZE/10,	
//							 (CPU_STK_SIZE )LOAD_SAMPLE_STK_SIZE,		
//							 (OS_MSG_QTY   )0,					
//							 (OS_TICK	     )0,					
//							 (void   		 * )0,					
//							 (OS_OPT       )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
//							 (OS_ERR 		 * )&err);				
//				 
//								 	
//	//创建取样轴事件标志组
//	OSFlagCreate((OS_FLAG_GRP*)&SampleAxisFlag,				//事件标志组指针
//							 (CPU_CHAR*   )"SampleAxisFlag",			//名字
//							 (OS_FLAGS    )0X00,									//事件标志组初始值
//							 (OS_ERR*     )&err);									//错误码
//							 
//							 
//	//创建推片模块送片同步信号量
//	OSSemCreate((OS_SEM*      )&PushSendSlideSyncSEM,
//	            (CPU_CHAR*    )"push send slide sync sem",
//							(OS_SEM_CTR   )0,
//							(OS_ERR*      )&err);
							

							 
	OS_CRITICAL_EXIT();	//退出临界区


}





//void USB_LP_CAN1_RX0_IRQHandler(void)
void CAN1_RX0_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS  							//如果需要支持OS.
	OSIntEnter();											//进入中断
#endif 
	


#if SYSTEM_SUPPORT_OS  							//如果需要支持OS.
	OSIntExit();											//触发任务切换软中断
#endif 	
	
}

/*******************************************************************************
  * name        : USART2_IRQHandler(void)
  * Function    : usart2 IRQ。
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void USART2_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS  							//如果需要支持OS.
	OSIntEnter();											//进入中断
#endif
	
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)			 // 串口空闲中断
	{
		
		USART_ReceiveData(USART2);															 //读取数据 注意：这句必须要，否则不能够清除中断标志位。
		

		
		USART_ClearITPendingBit(USART2, USART_IT_IDLE);          //清除中断标志
	}

#if SYSTEM_SUPPORT_OS  							//如果需要支持OS.
	OSIntExit();											//触发任务切换软中断
#endif
	
}

/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/


