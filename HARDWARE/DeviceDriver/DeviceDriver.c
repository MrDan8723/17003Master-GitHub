/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : DeviceDriver.c
  * Version     : V0.1
  * Description : 推片染色机设备驱动程序
  * Date        : 2018年10月18日
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
  * Function    : 舵机控制初始化
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
	
		/* 定时器模式控制端口，设置为浮空输出 */ 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 1000-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 1440-1;													//设置预分频：不预分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;												//时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;					//向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_OCStructInit(& TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;										//PWM模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 75;														//设置跳变值，当计数器计数到这个值时，电平发生跳变
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High  ;					//当定时器计数值小于上述设定值时为高电平
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_CtrlPWMOutputs(TIM4,ENABLE);
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	
	TIM_Cmd(TIM4, ENABLE);	
}

/**************************************************************************************
  * name        : void ServoPosition(float angle, int ms)
  * Function    : 舵机在指定时间内运动到指定角度
  * Parameters  : float angle-- 指定角度
									int ms-- 运动时间
  * Returns     : Null
  * Note        : 每10ms运动一步，直到达到指定角度
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void ServoPosition(float angle, int ms)
{
	OS_ERR err;
	static float last_angle=90;																		//记录舵机上一次的角度,安装的初始化角度为10度
	int step_angle;																									//每一步运动的角度
	int step;																												//步数
	
	step= ms/100;
	step= step==0?1:step;																						//step不能为0
	step_angle= (int)(angle - last_angle) / step;										//计算每一步运动的角度
	
	for(;step >= 0; step--)
	{
		ServoSetAngle(last_angle + step_angle);
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err); 				//延时100ms
//		delay_ms(100);																									//延时以降低舵机运动速度
		last_angle= last_angle + step_angle;													//存储
	}
	ServoSetAngle(angle);																						//最后补充一次小幅度运动,防止前面的计算过程中存在舍弃值使得实际位置存在偏差
	
	last_angle= angle;																							//存储以备下次使用	
}

/**************************************************************************************
  * name        : void ServoSetAngle(float angle)
  * Function    : 舵机直接运动到指定角度
  * Parameters  : float angle-- 指定角度,度数
  * Returns     : Null
  * Note        : 将输入的角度值转换为实际宽度脉冲输出
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void ServoSetAngle(float angle)
{
	int compare;
	int compare_min= 25;							//0度对应的比较寄存器值，(0.5ms/20ms)*1000
	int compare_max= 125;							//最大角度对应的比较寄存器器值，2.5/20*1000
	int angle_max= 180;								//最大角度
	
	compare= compare_min + (angle/angle_max)*(compare_max - compare_min);
	
	TIM_SetCompare2(TIM4,compare);
}

/**************************************************************************************
  * name        : void OutputInit(void)
  * Function    : 输出IO口初始化
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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;								//推挽输出
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
  * Function    : IO口输出测试函数
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
  * Function    : IO口输出测试函数
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
	
//	DMA_Cmd(DMA1_Channel6, DISABLE);																			//失能DMA传输
	
	if(ComRxBuf[0] == 0xFE && ComRxBuf[13] == 0xFF)//帧头校验
	{		
		CanCom.tx_msg.ExtId= 	((u32)0x01    			<<21) |        //源地址
												((u32)ComRxBuf[2]    	<<13) |        //目标地址
												((u32)0x00         		<<5)  ;       //预留字节     

		
		CanCom.tx_msg.IDE  = CAN_ID_EXT;                        								 //使用扩展模式
		CanCom.tx_msg.RTR  = CAN_RTR_DATA;                        							 //数据帧而不是远程帧
		CanCom.tx_msg.DLC  = 8;                                 								 //数组长度
		
		for(cnt= 0; cnt < 8; cnt++)
		{
			CanCom.tx_msg.Data[cnt]= ComRxBuf[cnt+4];
		}
		CAN_SendData(&CanCom);

	}		
	
	
//	DMA_SetCurrDataCounter(DMA1_Channel6,14);
//	DMA_Cmd(DMA1_Channel6, ENABLE);																				//使能DMA传输
}
/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/