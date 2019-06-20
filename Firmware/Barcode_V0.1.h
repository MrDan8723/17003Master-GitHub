/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : Barcode_V0.1.h
  * Version     : V0.1
  * Description : 优库电子E2000二维条码扫码器驱动程序
  * Date        : 2017年12月20日
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#ifndef _BARCODE_V0_1_H_
#define _BARCODE_V0_1_H_


/*********************File include*********************/
#include "stm32f10x_usart.h"

/******************Macro definition********************/
#define BARCODE_LEN     50                         //条码长度

/*********Enumeration variable type definition********/



/**************Structure type definition**************/

//结构体类型定义，用于举例怎样定义结构体类型
typedef struct _BarcodeTypeDef
{
	USART_TypeDef *com;													      //指定的串口
	uint16_t rx_buf[BARCODE_LEN];				              //接收数据存储缓冲区
  char code_str[BARCODE_LEN];                       //条码内容
	int rx_byte;																      //已经接收到的有效字节个数
	char rx_flag;																      //接收完成标志位 1--接收完成  0--接收未完成
}BarcodeTypeDef;


/*****************Function declaration**************/

void BC_GetCode(BarcodeTypeDef *buf);





 #endif
/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/
