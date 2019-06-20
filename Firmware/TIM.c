/*********************** (C) COPYRIGHT  2015 MNTC  ***************************
	
  * Copyright Wuhan MNTC Technology Inc.
	
  * File name   : TIM.c
  * Version     : V4.0
  * Description : ��¼��Ŀ���ض�ʱ�����ó���Ӳ��ƽ̨PCB V0.5
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
	
	/* ��ʱ��ģʽ���ƶ˿ڣ�����Ϊ������� */ 
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
	TIM_TimeBaseStructure.TIM_Prescaler = 10;														//����Ԥ��Ƶ����Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;												//ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;					//���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	TIM_TimeBaseStructure.TIM_Period = Period-1;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);											//��ʼ����ʱ��3
	TIM_TimeBaseStructure.TIM_Period = Period-1;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);											//��ʼ����ʱ��4
		
	TIM_OCStructInit(& TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;										//PWMģʽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = Period/2-1;															//��������ֵ�������������������ֵʱ����ƽ��������
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;						//����ʱ������ֵС�������趨ֵʱΪ�ߵ�ƽ
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
	
	TIM_TimeBaseStructure.TIM_Period = (Period-1);                  //��������Զ�װ�صļ���ֵ�����ڼ����Ǵ�0��ʼ�ģ�����10000�κ�Ϊ9999 ��������ֵ��Ԥ��Ƶ��ֵ���붼С��65535
	TIM_TimeBaseStructure.TIM_Prescaler = (7200-1);                //�������Ԥ��Ƶϵ����������Ϊ0ʱ��ʾ����Ƶ����Ҫ��1	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                   //ʹ�õĲ���Ƶ��֮��ķ�Ƶ����
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	   //���ϼ���
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);                //��ʼ����ʱ��2
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);                    //���TIM2����жϱ�־
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);                     //������ж�
	TIM_Cmd(TIM2, ENABLE);                                         //������ʹ�ܣ���ʼ����	 
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
	
	TIM_TimeBaseStructure.TIM_Period = (Period-1);                  //��������Զ�װ�صļ���ֵ�����ڼ����Ǵ�0��ʼ�ģ�����10000�κ�Ϊ9999 ��������ֵ��Ԥ��Ƶ��ֵ���붼С��65535
	TIM_TimeBaseStructure.TIM_Prescaler = (7200-1);                //�������Ԥ��Ƶϵ����������Ϊ0ʱ��ʾ����Ƶ����Ҫ��1	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                   //ʹ�õĲ���Ƶ��֮��ķ�Ƶ����
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	   //���ϼ���
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);                //��ʼ����ʱ��2
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);                    //���TIM2����жϱ�־
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);                     //������ж�
	TIM_Cmd(TIM3, ENABLE);                                         //������ʹ�ܣ���ʼ����	 
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
	
	TIM_TimeBaseStructure.TIM_Period = (10000-1);                  //��������Զ�װ�صļ���ֵ�����ڼ����Ǵ�0��ʼ�ģ�����10000�κ�Ϊ9999 ��������ֵ��Ԥ��Ƶ��ֵ���붼С��65535
	TIM_TimeBaseStructure.TIM_Prescaler = (7200-1);                //�������Ԥ��Ƶϵ����������Ϊ0ʱ��ʾ����Ƶ����Ҫ��1	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                   //ʹ�õĲ���Ƶ��֮��ķ�Ƶ����
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	   //���ϼ���
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);                //��ʼ����ʱ��5
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);                    //���TIM5����жϱ�־
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);                     //������ж�
	TIM_Cmd(TIM5, ENABLE);                                         //������ʹ�ܣ���ʼ����	 
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
