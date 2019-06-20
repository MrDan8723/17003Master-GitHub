/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : TIM.h
  * Version     : V4.0
  * Description : 康录项目主控板定时器配置程序，硬件平台PCB V0.5
  * Date        : 2016-08-18
  * Author      : Eric Chou
  * Check       :  
*******************************************************************************/
#include <stm32f10x.h>

void TIM_TIM5_Configuration(void);
void TIM_TIM5_Open(void);
void TIM_TIM5_Closed(void);
void TIM_PWM(u32 Period);
void TIM_TIM2_Configuration(u32 Period);
void TIM_TIM2_Open(void);
void TIM_TIM2_Closed(void);
void TIM_TIM3_Configuration(u32 Period);
void TIM_TIM3_Open(void);
void TIM_TIM3_Closed(void);
