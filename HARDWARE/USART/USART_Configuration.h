/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : USART_Configuration.h
  * Version     : V4.0
  * Description : 康录项目主控板串口配置程序，硬件平台PCB V0.5
  * Date        : 2016-08-11
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#ifndef _USART_CONFIGURATION_H
#define _USART_CONFIGURATION_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_usart.h"

void USART_SendBuf(USART_TypeDef *com, uint16_t *buf, int buf_len);		//串口发送函数
void Usart1_Configuration(int baud);															//串口1硬件配置
void Usart1_Nvic_Configuration(void);															//串口1中断配置
void Usart2_Configuration(int baud);															//串口2硬件配置
void Usart2_Nvic_Configuration(void);															//串口2中断配置
void Usart3_Configuration(int baud);															//串口2硬件配置
void Usart3_Nvic_Configuration(void);															//串口2中断配置
void Uart4_Configuration(int baud);																//串口4硬件配置
void Uart4_Nvic_Configuration(void);															//串口4中断配置



#endif
/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE****************/


