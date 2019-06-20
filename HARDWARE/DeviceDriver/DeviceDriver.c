/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : DeviceDriver.c
  * Version     : V0.1
  * Description : ��ƬȾɫ���豸��������
  * Date        : 2018��10��18��
  * Author      : XYD
  * Check       :  
*******************************************************************************/



/*********************File include*********************/
#include "DeviceDriver.h"                 
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "delay.h"
#include "Common.h"
/******************Macro definition********************/

/*************External variable declaration************/

/***************Global variable definition*************/


/**************************************************************************************
  * name        : void ServoInit(void)
  * Function    : ������Ƴ�ʼ��
  * Parameters  : Null
  * Returns     : Null
  * Note        : PB7-- TIM4_CH2
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void ServoInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE);
	
		/* ��ʱ��ģʽ���ƶ˿ڣ�����Ϊ������� */ 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 1000-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 1440-1;													//����Ԥ��Ƶ����Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;												//ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;					//���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_OCStructInit(& TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;										//PWMģʽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 75;														//��������ֵ�������������������ֵʱ����ƽ��������
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High  ;					//����ʱ������ֵС�������趨ֵʱΪ�ߵ�ƽ
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_CtrlPWMOutputs(TIM4,ENABLE);
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	
	TIM_Cmd(TIM4, ENABLE);	
}

/**************************************************************************************
  * name        : void ServoPosition(float angle, int ms)
  * Function    : �����ָ��ʱ�����˶���ָ���Ƕ�
  * Parameters  : float angle-- ָ���Ƕ�
									int ms-- �˶�ʱ��
  * Returns     : Null
  * Note        : ÿ10ms�˶�һ����ֱ���ﵽָ���Ƕ�
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void ServoPosition(float angle, int ms)
{
	OS_ERR err;
	static float last_angle=90;																		//��¼�����һ�εĽǶ�,��װ�ĳ�ʼ���Ƕ�Ϊ10��
	int step_angle;																									//ÿһ���˶��ĽǶ�
	int step;																												//����
	
	step= ms/100;
	step= step==0?1:step;																						//step����Ϊ0
	step_angle= (int)(angle - last_angle) / step;										//����ÿһ���˶��ĽǶ�
	
	for(;step >= 0; step--)
	{
		ServoSetAngle(last_angle + step_angle);
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err); 				//��ʱ100ms
//		delay_ms(100);																									//��ʱ�Խ��Ͷ���˶��ٶ�
		last_angle= last_angle + step_angle;													//�洢
	}
	ServoSetAngle(angle);																						//��󲹳�һ��С�����˶�,��ֹǰ��ļ�������д�������ֵʹ��ʵ��λ�ô���ƫ��
	
	last_angle= angle;																							//�洢�Ա��´�ʹ��	
}

/**************************************************************************************
  * name        : void ServoSetAngle(float angle)
  * Function    : ���ֱ���˶���ָ���Ƕ�
  * Parameters  : float angle-- ָ���Ƕ�,����
  * Returns     : Null
  * Note        : ������ĽǶ�ֵת��Ϊʵ�ʿ���������
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void ServoSetAngle(float angle)
{
	int compare;
	int compare_min= 25;							//0�ȶ�Ӧ�ıȽϼĴ���ֵ��(0.5ms/20ms)*1000
	int compare_max= 125;							//���Ƕȶ�Ӧ�ıȽϼĴ�����ֵ��2.5/20*1000
	int angle_max= 180;								//���Ƕ�
	
	compare= compare_min + (angle/angle_max)*(compare_max - compare_min);
	
	TIM_SetCompare2(TIM4,compare);
}

/**************************************************************************************
  * name        : void OutputInit(void)
  * Function    : ���IO�ڳ�ʼ��
  * Parameters  : Null
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void OutputInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE); 
	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;								//�������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
	GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1);
	GPIO_SetBits(GPIOC,GPIO_Pin_4 | GPIO_Pin_5);
	GPIO_SetBits(GPIOD,GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	GPIO_SetBits(GPIOE,GPIO_Pin_7);
}

/**************************************************************************************
  * name        : void OutputTest(void)
  * Function    : IO��������Ժ���
  * Parameters  : Null
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void OutputTest(void)
{
	while(1)
	{
		LED0=0;
		LED1=0;
		
		GPIO_SetBits(GPIOA,GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
		GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1);
		GPIO_SetBits(GPIOC,GPIO_Pin_4 | GPIO_Pin_5);
		GPIO_SetBits(GPIOD,GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		GPIO_SetBits(GPIOE,GPIO_Pin_7);
		delay_ms(1000);
		
		LED0=1;
		LED1=1;
		GPIO_ResetBits(GPIOA,GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
		GPIO_ResetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1);
		GPIO_ResetBits(GPIOC,GPIO_Pin_4 | GPIO_Pin_5);
		GPIO_ResetBits(GPIOD,GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		GPIO_ResetBits(GPIOE,GPIO_Pin_7);
		delay_ms(1000);
			
		
		NEEDLE_INPUT_PUMP_ON;
		delay_ms(5000);
		NEEDLE_INPUT_PUMP_OFF;
		
		NEEDLE_OUTPUT_PUMP_ON;
		delay_ms(5000);
		NEEDLE_OUTPUT_PUMP_OFF;
		
		PUSHER_INPUT_PUMP_ON;
		delay_ms(5000);
		PUSHER_INPUT_PUMP_OFF;
		
		PUSHER_OUTPUT_PUMP_ON;
		delay_ms(5000);
		PUSHER_OUTPUT_PUMP_OFF;
		
		REAGENT1_PUMP_ON;
		delay_ms(5000);
		REAGENT1_PUMP_OFF;
		
		REAGENT2_PUMP_ON;
		delay_ms(5000);
		REAGENT2_PUMP_OFF;
		
		REAGENT3_PUMP_ON;
		delay_ms(5000);
		REAGENT3_PUMP_OFF;
		
		REAGENT4_PUMP_ON;
		delay_ms(5000);
		REAGENT4_PUMP_OFF;
		
		STAIN_OUTPUT_PUMP_ON;
		delay_ms(5000);
		STAIN_OUTPUT_PUMP_OFF;
	}
}


/**************************************************************************************
  * name        : void OutputTest(void)
  * Function    : IO��������Ժ���
  * Parameters  : Null
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void ComToCan(void)
{
	int cnt;
	float value;
	int crc=0;
	
//	DMA_Cmd(DMA1_Channel6, DISABLE);																			//ʧ��DMA����
	
	if(ComRxBuf[0] == 0xFE && ComRxBuf[13] == 0xFF)//֡ͷУ��
	{		
		CanCom.tx_msg.ExtId= 	((u32)0x01    			<<21) |        //Դ��ַ
												((u32)ComRxBuf[2]    	<<13) |        //Ŀ���ַ
												((u32)0x00         		<<5)  ;       //Ԥ���ֽ�     

		
		CanCom.tx_msg.IDE  = CAN_ID_EXT;                        								 //ʹ����չģʽ
		CanCom.tx_msg.RTR  = CAN_RTR_DATA;                        							 //����֡������Զ��֡
		CanCom.tx_msg.DLC  = 8;                                 								 //���鳤��
		
		for(cnt= 0; cnt < 8; cnt++)
		{
			CanCom.tx_msg.Data[cnt]= ComRxBuf[cnt+4];
		}
		CAN_SendData(&CanCom);

	}		
	
	
//	DMA_SetCurrDataCounter(DMA1_Channel6,14);
//	DMA_Cmd(DMA1_Channel6, ENABLE);																				//ʹ��DMA����
}
/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/