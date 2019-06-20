/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : MNTC_IOCtrl.c
  * Version     : V0.1
  * Description : MNTC实验室IO口基本控制程序
  * Date        : 2019年5月7日
  * Author      : XYD
  * Check       :  
*******************************************************************************/



/*********************File include*********************/
#include "stm32f10x_gpio.h"
#include "MNTC_IOCtrl.h"
/******************Macro definition********************/

/*************External variable declaration************/

/***************Global variable definition*************/

/*******************************************************************************
  * name        : void GPIO_Config(GPIO_TypeDef *port,uint16_t pin,GPIOMode_TypeDef mode_type)
  * Function    : 初始化IO口及其时钟
  * Parameters  : GPIO_TypeDef *port-- 端口
                  uint16_t pin-- 引脚
                  GPIOMode_TypeDef mode_type-- IO口模式
  * Returns     : NULL
  * Note        : 
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void GPIO_Config(GPIO_TypeDef *port,uint16_t pin,GPIOMode_TypeDef mode_type)
{
  GPIO_InitTypeDef GPIO_InitStructure;  
																							
  if(port  == GPIOA)                      //初始化端口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  else if(port  == GPIOB)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  else if(port  == GPIOC)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  else if(port  == GPIOD)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  else if(port  == GPIOE)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
  	
	GPIO_InitStructure.GPIO_Pin = pin;         
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = mode_type;			
	GPIO_Init(port, &GPIO_InitStructure);
}

/*******************************************************************************
  * name        : void EXTI_Config(GPIO_TypeDef *port,uint16_t pin,EXTITrigger_TypeDef trigger_type)
  * Function    : 将某一引脚作为外部中断源初始化
  * Parameters  : GPIO_TypeDef *port-- 端口
                  uint16_t pin-- 引脚
                  EXTITrigger_TypeDef trigger_type-- 触发方式
  * Returns     : NULL
  * Note        : 
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void EXTI_Config(GPIO_TypeDef *port,uint16_t pin,EXTITrigger_TypeDef trigger_type)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  uint8_t source_port,source_pin;
  uint32_t exti_line;
  uint8_t nvic_irq_channel;
  NVIC_InitTypeDef NVIC_InitStructure;

  
  if(GPIOA == port)                                             //选择外部中断源输入端口
    source_port= GPIO_PortSourceGPIOA;
  else if(GPIOB == port)
    source_port= GPIO_PortSourceGPIOB;
  else if(GPIOC == port)
    source_port= GPIO_PortSourceGPIOC;
  else if(GPIOD == port)
    source_port= GPIO_PortSourceGPIOD;
  else if(GPIOE == port)
    source_port= GPIO_PortSourceGPIOE;
  
  switch(pin)                                                   //选择外部中断源输入引脚
  {
    case GPIO_Pin_0: 
    {
      source_pin= GPIO_PinSource0; 
      exti_line= EXTI_Line0;
      nvic_irq_channel= EXTI0_IRQn;
      break;
    }
    case GPIO_Pin_1: 
    {
      source_pin= GPIO_PinSource1; 
      exti_line= EXTI_Line1;
      nvic_irq_channel= EXTI1_IRQn;
      break;
    }
    case GPIO_Pin_2: 
    {
      source_pin= GPIO_PinSource2; 
      exti_line= EXTI_Line2;
      nvic_irq_channel= EXTI2_IRQn;
      break;
    }
    case GPIO_Pin_3: 
    {
      source_pin= GPIO_PinSource3; 
      exti_line= EXTI_Line3;
      nvic_irq_channel= EXTI3_IRQn;
      break;
    }
    case GPIO_Pin_4: 
    {
      source_pin= GPIO_PinSource4; 
      exti_line= EXTI_Line4;
      nvic_irq_channel= EXTI4_IRQn;
      break;
    }
    case GPIO_Pin_5: 
    {
      source_pin= GPIO_PinSource5; 
      exti_line= EXTI_Line5;
      nvic_irq_channel= EXTI9_5_IRQn;
      break;
    }
    case GPIO_Pin_6: 
    {
      source_pin= GPIO_PinSource6; 
      exti_line= EXTI_Line6;
      nvic_irq_channel= EXTI9_5_IRQn;
      break;
    }
    case GPIO_Pin_7: 
    {
      source_pin= GPIO_PinSource7; 
      exti_line= EXTI_Line7;
      nvic_irq_channel= EXTI9_5_IRQn;
      break;
    }
    case GPIO_Pin_8: 
    {
      source_pin= GPIO_PinSource8; 
      exti_line= EXTI_Line8;
      nvic_irq_channel= EXTI9_5_IRQn;
      break;
    }
    case GPIO_Pin_9: 
    {
      source_pin= GPIO_PinSource9; 
      exti_line= EXTI_Line9;
      nvic_irq_channel= EXTI9_5_IRQn;
      break;
    }
    case GPIO_Pin_10: 
    {
      source_pin= GPIO_PinSource10; 
      exti_line= EXTI_Line10;
      nvic_irq_channel= EXTI15_10_IRQn;
      break;
    }
    case GPIO_Pin_11: 
    {
      source_pin= GPIO_PinSource11; 
      exti_line= EXTI_Line11;
      nvic_irq_channel= EXTI15_10_IRQn;
      break;
    }
    case GPIO_Pin_12: 
    {
      source_pin= GPIO_PinSource12; 
      exti_line= EXTI_Line12;
      nvic_irq_channel= EXTI15_10_IRQn;
      break;
    }
    case GPIO_Pin_13: 
    {
      source_pin= GPIO_PinSource13; 
      exti_line= EXTI_Line13;
      nvic_irq_channel= EXTI15_10_IRQn;
      break;
    }
    case GPIO_Pin_14: 
    {
      source_pin= GPIO_PinSource14; 
      exti_line= EXTI_Line14;
      nvic_irq_channel= EXTI15_10_IRQn;
      break;
    }
    case GPIO_Pin_15: 
    {
      source_pin= GPIO_PinSource15; 
      exti_line= EXTI_Line0;
      nvic_irq_channel= EXTI15_10_IRQn;
      break;
    }
    default:break;
  }
  GPIO_EXTILineConfig(source_port, source_pin);                 //设置外部中断源
  
  EXTI_InitStructure.EXTI_Line= exti_line;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = trigger_type;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);	  

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = nvic_irq_channel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
}



/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/