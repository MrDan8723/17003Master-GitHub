/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : USART_Configuration.h
  * Version     : V4.0
  * Description : ��¼��Ŀ���ذ崮�����ó���Ӳ��ƽ̨PCB V0.5
  * Date        : 2016-08-11
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#ifndef _USART_CONFIGURATION_H
#define _USART_CONFIGURATION_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_usart.h"

void USART_SendBuf(USART_TypeDef *com, uint16_t *buf, int buf_len);		//���ڷ��ͺ���
void Usart1_Configuration(int baud);															//����1Ӳ������
void Usart1_Nvic_Configuration(void);															//����1�ж�����
void Usart2_Configuration(int baud);															//����2Ӳ������
void Usart2_Nvic_Configuration(void);															//����2�ж�����
void Usart3_Configuration(int baud);															//����2Ӳ������
void Usart3_Nvic_Configuration(void);															//����2�ж�����
void Uart4_Configuration(int baud);																//����4Ӳ������
void Uart4_Nvic_Configuration(void);															//����4�ж�����



#endif
/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE****************/


