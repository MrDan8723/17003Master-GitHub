/*********************** (C) COPYRIGHT  2017 MNTC  **************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : USART_DMA_Config.c
  * Version     : V0.1
  * Description : 串口DMA驱动动程序
  * Date        : 2018年6月15日
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#ifndef _USART_DMA_CONFIG_H_
#define _USART_DMA_CONFIG_H_


/*********************File include*********************/
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_dma.h"
/******************Macro definition********************/


/*********Enumeration variable type definition********/



/**************Structure type definition**************/

/*****************Function declaration**************/

void USART1_DMA_Config(int baud,int remap,unsigned char *rx_buf,int length);
void USART2_DMA_Config(int baud,int remap,unsigned char *rx_buf,int length);  



 #endif
/**************** (C) COPYRIGHT 2017 MNTC *****END OF FILE*******************/
 
 
 
 