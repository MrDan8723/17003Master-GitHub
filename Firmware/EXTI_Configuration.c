/*********************** (C) COPYRIGHT  2017 MNTC  ***************************
	
  * Copyright Wuhan MNTC Technology Inc.
	
  * File name   : EXTI_Configuration.c
  * Version     : V1.0
  * Description : XNY60�����ж����ó���Ӳ��ƽ̨PCB V0.5
  * Date        : 2018-04-02
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#include "EXTI_Configuration.h"
#include "stm32f10x.h"


/*******************************************************************************
                              Group          Line   PrePriority   SubPriority
SensorBoard  					NVIC_PriorityGroup_2     5          2             2
PIDBoard    					NVIC_PriorityGroup_2     4          2             2
Pause         				NVIC_PriorityGroup_2     9          2             2
Enzyme        				NVIC_PriorityGroup_2     3          2             2
Door         					NVIC_PriorityGroup_2     2          2             2
Waste        					NVIC_PriorityGroup_2     10         2             2
*******************************************************************************/

/*******************************************************************************
  * name        : EXTI_PauseConfiguration(void)
  * Function    : 15���ⲿ�ж��߳�ʼ������������ͣ������
  * Parameters  : Null
  * Returns     : Null
  * Author      : ZQChen
  * Check       : 
*******************************************************************************/
void EXTI_PauseConfiguration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;                         	//��ʼ���ⲿ�жϼĴ���
	NVIC_InitTypeDef NVIC_InitStructure;	                      	///�ṹ���� 
	
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
	EXTI_ClearITPendingBit(EXTI_Line15);         	                //���IO���ж��������λ������жϱ�־λ��
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;             //�����ⲿ�жϴ�������һ�����¼�������
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;         //�����жϴ�����ʽ���½��ش�����ʽ
	EXTI_InitStructure.EXTI_Line=EXTI_Line15;                      //ѡ���ж���·Ϊ9����ѡ���Ǹ�IO��Ϊ�ж����룩
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource15);    //��GPIOC9�ҵ��ж���	
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;                     	//ʹ���ⲿ�ж�
	EXTI_Init(&EXTI_InitStructure);                              	//��ʼ��
	/* Configure the NVIC Preemption Priority Bits */  
	/* Configure one bit for preemption priority */
	/* ���ȼ��� ˵������ռ���ȼ����õ�λ�����������ȼ����õ�λ��   ��������2 */    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	  

  /* Enable the EXTI2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;            //�����ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     //��ռ���ȼ� 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;	            //������Ӧʽ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;		            //ʹ�����õ��ⲿ�ж�ͨ������
	NVIC_Init(&NVIC_InitStructure);                               //��ʼ��     */
}

/*******************************************************************************
  * name        : EXTI_PauseEnable(void)
  * Function    : ��ͣ���ⲿ�ж�ʹ��
  * Parameters  : Null
  * Returns     : Null
  * Author      : ZQChen
  * Check       : 
*******************************************************************************/
void EXTI_PauseEnable(void)
{
	NVIC_EnableIRQ(EXTI15_10_IRQn);																		//ʹ��10~15���ж����ϵ��ж�
//	EXTI->IMR|=1<<9;																							//ʹ��15���ⲿ�ж���
}

/*******************************************************************************
  * name        : EXTI_PauseDisable(void)
  * Function    : ��ͣ���ⲿ�ж�ʧ��
  * Parameters  : Null
  * Returns     : Null
  * Author      : ZQChen
  * Check       : 
*******************************************************************************/
void EXTI_PauseDisable(void)
{
	NVIC_DisableIRQ(EXTI15_10_IRQn);																//ʧ��10~15���ж����ϵ��ж�
//	EXTI->IMR&=~(1<<9);																					//����15���ж����ϵ��ж�
}


/**************** (C) COPYRIGHT 2018 MNTC *****END OF FILE****************/
