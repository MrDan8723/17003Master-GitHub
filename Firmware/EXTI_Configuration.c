/*********************** (C) COPYRIGHT  2017 MNTC  ***************************
	
  * Copyright Wuhan MNTC Technology Inc.
	
  * File name   : EXTI_Configuration.c
  * Version     : V1.0
  * Description : XNY60主控中断配置程序，硬件平台PCB V0.5
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
  * Function    : 15号外部中断线初始化，将用于暂停键输入
  * Parameters  : Null
  * Returns     : Null
  * Author      : ZQChen
  * Check       : 
*******************************************************************************/
void EXTI_PauseConfiguration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;                         	//初始化外部中断寄存器
	NVIC_InitTypeDef NVIC_InitStructure;	                      	///结构声明 
	
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
	EXTI_ClearITPendingBit(EXTI_Line15);         	                //清除IO口中断清除挂起位（清除中断标志位）
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;             //设置外部中断触发（另一种是事件触发）
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;         //设置中断触发方式：下降沿触发方式
	EXTI_InitStructure.EXTI_Line=EXTI_Line15;                      //选择中断线路为9（即选择那个IO作为中断输入）
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource15);    //将GPIOC9挂到中断上	
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;                     	//使能外部中断
	EXTI_Init(&EXTI_InitStructure);                              	//初始化
	/* Configure the NVIC Preemption Priority Bits */  
	/* Configure one bit for preemption priority */
	/* 优先级组 说明了抢占优先级所用的位数，和子优先级所用的位数   在这里是2 */    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	  

  /* Enable the EXTI2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;            //设置中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     //抢占优先级 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;	            //设置响应式优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;		            //使能设置的外部中断通道请求
	NVIC_Init(&NVIC_InitStructure);                               //初始化     */
}

/*******************************************************************************
  * name        : EXTI_PauseEnable(void)
  * Function    : 暂停键外部中断使能
  * Parameters  : Null
  * Returns     : Null
  * Author      : ZQChen
  * Check       : 
*******************************************************************************/
void EXTI_PauseEnable(void)
{
	NVIC_EnableIRQ(EXTI15_10_IRQn);																		//使能10~15号中断线上的中断
//	EXTI->IMR|=1<<9;																							//使能15号外部中断线
}

/*******************************************************************************
  * name        : EXTI_PauseDisable(void)
  * Function    : 暂停键外部中断失能
  * Parameters  : Null
  * Returns     : Null
  * Author      : ZQChen
  * Check       : 
*******************************************************************************/
void EXTI_PauseDisable(void)
{
	NVIC_DisableIRQ(EXTI15_10_IRQn);																//失能10~15号中断线上的中断
//	EXTI->IMR&=~(1<<9);																					//屏蔽15号中断线上的中断
}


/**************** (C) COPYRIGHT 2018 MNTC *****END OF FILE****************/
