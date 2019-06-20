/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : TIM.h
  * Version     : V4.0
  * Description : ��¼��Ŀ���ذ嶨ʱ�����ó���Ӳ��ƽ̨PCB V0.5
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
