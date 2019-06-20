/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : main.c
  * Version     : V0.3
  * Description : Ѫ�������ذ�������Դ�ļ�
  * Date        : 2019��6��18��
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#include "Common.h" 

//��������
//�������ȼ�
#define START_TASK_PRIO		3
//�����ջ��С	
#define START_STK_SIZE 		512
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);




MNTC_CanProtocolTypeDef CanCom; 								//����CANͨ�Žṹ��
unsigned char ComRxBuf[14];											//���ڽ��ջ�����	
int RxFlag;

int main(void)
{
	int cnt;
	OS_ERR err;
	CPU_SR_ALLOC();	
	delay_init();  																							//ʱ�ӳ�ʼ��
	delay_ms(5000);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							//�жϷ�������
	uart_init(115200);  																				//���ڳ�ʼ��
	LED_Init();         																				//LED��ʼ��
	
	OSInit(&err);																								//��ʼ��UCOSIII
	OS_CRITICAL_ENTER();																				//�����ٽ���
	
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,											//������ƿ�
				 (CPU_CHAR	* )"start task", 													//��������
                 (OS_TASK_PTR )start_task, 										//������
                 (void		* )0,																//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     						//�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],						//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,							//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,								//�����ջ��С
                 (OS_MSG_QTY  )0,															//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,																//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,																//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);														//��Ÿú�������ʱ�ķ���ֵ
								 
	OS_CRITICAL_EXIT();																					//�˳��ٽ���	 
	OSStart(&err);  																						//����UCOSIII
	while(1);
}

//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	 //ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN			 //���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif

#if OS_CFG_APP_HOOKS_EN							 //ʹ�ù��Ӻ���
	App_OS_SetAllHooks();			
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//�����ٽ���
	
//	//����ȡ������
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
//	//����ȡ�����¼���־��
//	OSFlagCreate((OS_FLAG_GRP*)&SampleAxisFlag,				//�¼���־��ָ��
//							 (CPU_CHAR*   )"SampleAxisFlag",			//����
//							 (OS_FLAGS    )0X00,									//�¼���־���ʼֵ
//							 (OS_ERR*     )&err);									//������
//							 
//							 
//	//������Ƭģ����Ƭͬ���ź���
//	OSSemCreate((OS_SEM*      )&PushSendSlideSyncSEM,
//	            (CPU_CHAR*    )"push send slide sync sem",
//							(OS_SEM_CTR   )0,
//							(OS_ERR*      )&err);
							

							 
	OS_CRITICAL_EXIT();	//�˳��ٽ���


}





//void USB_LP_CAN1_RX0_IRQHandler(void)
void CAN1_RX0_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS  							//�����Ҫ֧��OS.
	OSIntEnter();											//�����ж�
#endif 
	


#if SYSTEM_SUPPORT_OS  							//�����Ҫ֧��OS.
	OSIntExit();											//���������л����ж�
#endif 	
	
}

/*******************************************************************************
  * name        : USART2_IRQHandler(void)
  * Function    : usart2 IRQ��
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void USART2_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS  							//�����Ҫ֧��OS.
	OSIntEnter();											//�����ж�
#endif
	
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)			 // ���ڿ����ж�
	{
		
		USART_ReceiveData(USART2);															 //��ȡ���� ע�⣺������Ҫ�������ܹ�����жϱ�־λ��
		

		
		USART_ClearITPendingBit(USART2, USART_IT_IDLE);          //����жϱ�־
	}

#if SYSTEM_SUPPORT_OS  							//�����Ҫ֧��OS.
	OSIntExit();											//���������л����ж�
#endif
	
}

/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/


