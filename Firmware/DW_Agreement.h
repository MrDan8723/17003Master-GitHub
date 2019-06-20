/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : DW_Agreement.c
  * Version     : V0.2    2017年10月19日
                     1.在康录所使用的第一版的基础上删除所有康录项目专用函数，使本文件作为一个
                  纯净的迪文DGUS屏协议支持文件
                     2.修改所有函数入口都包含DGUS屏接口体，降低与外部文件的耦合性
  * Description : 本文件为迪文DGUS屏串口协议支持文件
  * Date        : 2016年8月22日
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#ifndef _DW_AGREEMENT_H_
#define _DW_AGREEMENT_H_

#include "stm32f10x_usart.h"

#define DW_RX_BUF_LEN			255
#define DW_TX_BUF_LEN			255
#define HEAD1 0xA5  													//帧头
#define HEAD2 0x5A

//指令
#define DW_WRITE_REG  0X80 										//写寄存器
#define DW_READ_REG   0X81 										//读寄存器
#define DW_WRITE_DATA 0X82 										//写数据存储器
#define DW_READ_DATA  0X83 										//读数据存储器
#define DW_WRITE_CURVE  0X84                  //写曲线缓冲区

typedef struct _DW_Struct
{
	USART_TypeDef *com;													//指定的串口
	unsigned char rx_buf[DW_RX_BUF_LEN];				//接收数据存储缓冲区
	unsigned char tx_buf[DW_TX_BUF_LEN];				//发送数据缓冲区
	int rx_byte;																//已经接收到的有效字节个数
	int tx_byte;																//需要发送的字节数
	char rx_flag;																//接收完成标志位 1--接收完成  0--接收未完成
}DW_Struct;



void DW_SendCode(DW_Struct *lcd);
void DW_JumpPage(DW_Struct *lcd,int page);
void DW_WriteCurve(DW_Struct *lcd,int ch,int *data,int num);
void DW_CleanCurve(DW_Struct *lcd,int ch);
void DW_SetCurve(DW_Struct *lcd,int pointer,int width,int height,int num,float y_min,float y_max);
void DW_WriteData(DW_Struct *lcd,int adr,int data);
void DW_WriteStr(DW_Struct *lcd,int adr,char *str);
void DW_ReadReg(DW_Struct *lcd,int adr,int byte);
void DW_WriteReg(DW_Struct *lcd, int adr,int byte);
void DW_ReadData(DW_Struct *lcd,int adr,int word);
void DW_WriteBuf(DW_Struct *lcd,int data);
void DW_Write2Buf(DW_Struct *lcd,int data);
void DW_SendCode(DW_Struct *lcd);
void DW_GetCode(DW_Struct *lcd);

#endif

/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/
