/*******************************************************************************
  
  * Copyright Wuhan MNTC Technology Inc.
  
  * File name   : stm32f10x_it.h
  * Version     : V0.1
  * Description : Interrupt execution function
  * Date        : 11, Jan 2016 
  * Author      : Eric Chou
  * Check       : 

*******************************************************************************/
/* Includes ------------------------------------------------------------------*/

#include "stm32f10x.h"

/* Private function prototypes -----------------------------------------------*/
void TIM2_IRQHandler(void);
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);


