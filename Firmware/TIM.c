/*********************** (C) COPYRIGHT  2015 MNTC  ***************************
	
  * Copyright Wuhan MNTC Technology Inc.
	
  * File name   : TIM.c
  * Version     : V4.0
  * Description : 康录项目主控定时器配置程序，硬件平台PCB V0.5
  * Date        : 2016-08-18
  * Author      : Eric Chou
  * Check       :  
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "TIM.h"

/**************************************************************************************
  * name        : TIM_PWM_Configuration(void)
  * Function    : Configures the TIM.
	             
									PB6     TIM4_CH1
	                PB7     TIM4_CH2
	                PA8     TIM1_CH1
	                PA6     TIM3_CH1
									
  * Parameters  : Null
  * Returns     : Null
  * Author      : 
  * Check       : Eric Chou
**************************************************************************************/

void TIM_PWM(u32 Period)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOB |	RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4, ENABLE);
	
	/* 定时器模式控制端口，设置为浮空输出 */ 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = Period-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 10;														//设置预分频：不预分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;												//时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;					//向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	TIM_TimeBaseStructure.TIM_Period = Period-1;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);											//初始化定时器3
	TIM_TimeBaseStructure.TIM_Period = Period-1;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);											//初始化定时器4
		
	TIM_OCStructInit(& TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;										//PWM模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = Period/2-1;															//设置跳变值，当计数器计数到这个值时，电平发生跳变
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;						//当定时器计数值小于上述设定值时为高电平
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	
	TIM_OC1Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC2Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure); 
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); 
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	TIM_ARRPreloadConfig(TIM1, ENABLE);
		
	TIM_Cmd(TIM1, ENABLE); 
	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}

/**************************************************************************************
  * name        : TIM_TIM2_Configuration(void)
  * Function    : Configures the TIM2.
  * Parameters  : Null
  * Returns     : Null
  * Author      : 
  * Check       : Eric Chou
**************************************************************************************/
void TIM_TIM2_Configuration(u32 Period)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = (Period-1);                  //这个就是自动装载的计数值，由于计数是从0开始的，计数10000次后为9999 计数器的值和预分频的值必须都小于65535
	TIM_TimeBaseStructure.TIM_Prescaler = (7200-1);                //这个就是预分频系数，当由于为0时表示不分频所以要减1	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                   //使用的采样频率之间的分频比例
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	   //向上计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);                //初始化定时器2
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);                    //清除TIM2溢出中断标志
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);                     //打开溢出中断
	TIM_Cmd(TIM2, ENABLE);                                         //计数器使能，开始工作	 
}

/**************************************************************************************
  * name        : void TIM_TIM2_Open(void)
  * Function    : Open the TIM2 Interrupt.
  * Parameters  : Null
  * Returns     : Null
  * Author      : Eric Chou
  * Check       : 
**************************************************************************************/
void TIM_TIM2_Open(void)
{ 
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**************************************************************************************
  * name        : void TIM_TIM2_Closed(void)
  * Function    : Closed the TIM2 Interrupt.
  * Parameters  : Null
  * Returns     : Null
  * Author      : Eric Chou
  * Check       : 
**************************************************************************************/
void TIM_TIM2_Closed(void)
{ 
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/**************************************************************************************
  * name        : TIM_TIM2_Configuration(void)
  * Function    : Configures the TIM2.
  * Parameters  : Null
  * Returns     : Null
  * Author      : 
  * Check       : Eric Chou
**************************************************************************************/
void TIM_TIM3_Configuration(u32 Period)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = (Period-1);                  //这个就是自动装载的计数值，由于计数是从0开始的，计数10000次后为9999 计数器的值和预分频的值必须都小于65535
	TIM_TimeBaseStructure.TIM_Prescaler = (7200-1);                //这个就是预分频系数，当由于为0时表示不分频所以要减1	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                   //使用的采样频率之间的分频比例
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	   //向上计数
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);                //初始化定时器2
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);                    //清除TIM2溢出中断标志
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);                     //打开溢出中断
	TIM_Cmd(TIM3, ENABLE);                                         //计数器使能，开始工作	 
}

/**************************************************************************************
  * name        : void TIM_TIM2_Open(void)
  * Function    : Open the TIM2 Interrupt.
  * Parameters  : Null
  * Returns     : Null
  * Author      : Eric Chou
  * Check       : 
**************************************************************************************/
void TIM_TIM3_Open(void)
{ 
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**************************************************************************************
  * name        : void TIM_TIM2_Closed(void)
  * Function    : Closed the TIM2 Interrupt.
  * Parameters  : Null
  * Returns     : Null
  * Author      : Eric Chou
  * Check       : 
**************************************************************************************/
void TIM_TIM3_Closed(void)
{ 
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);
}
/**************************************************************************************
  * name        : TIM_TIM5_Configuration(void)
  * Function    : Configures the TIM5.
  * Parameters  : Null
  * Returns     : Null
  * Author      : 
  * Check       : Eric Chou
**************************************************************************************/

/**************************************************************************************
  * name        : TIM_TIM5_Configuration(void)
  * Function    : Configures the TIM5.
  * Parameters  : Null
  * Returns     : Null
  * Author      : 
  * Check       : Eric Chou
**************************************************************************************/
void TIM_TIM5_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = (10000-1);                  //这个就是自动装载的计数值，由于计数是从0开始的，计数10000次后为9999 计数器的值和预分频的值必须都小于65535
	TIM_TimeBaseStructure.TIM_Prescaler = (7200-1);                //这个就是预分频系数，当由于为0时表示不分频所以要减1	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                   //使用的采样频率之间的分频比例
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	   //向上计数
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);                //初始化定时器5
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);                    //清除TIM5溢出中断标志
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);                     //打开溢出中断
	TIM_Cmd(TIM5, ENABLE);                                         //计数器使能，开始工作	 
}

/**************************************************************************************
  * name        : void TIM_TIM5_Open(void)
  * Function    : Open the TIM5 Interrupt.
  * Parameters  : Null
  * Returns     : Null
  * Author      : Eric Chou
  * Check       : 
**************************************************************************************/
void TIM_TIM5_Open(void)
{ 
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**************************************************************************************
  * name        : void TIM_TIM5_Closed(void)
  * Function    : Closed the TIM5 Interrupt.
  * Parameters  : Null
  * Returns     : Null
  * Author      : Eric Chou
  * Check       : 
**************************************************************************************/
void TIM_TIM5_Closed(void)
{ 
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);
}
/**************** (C) COPYRIGHT 2016 MNTC ***** END OF FILE ****************/
