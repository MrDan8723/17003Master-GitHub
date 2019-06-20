/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : DW_Agreement.c
  * Version     : V0.2    2017��10��19��
                     1.�ڿ�¼��ʹ�õĵ�һ��Ļ�����ɾ�����п�¼��Ŀר�ú�����ʹ���ļ���Ϊһ��
                  �����ĵ���DGUS��Э��֧���ļ�
                     2.�޸����к�����ڶ�����DGUS���ӿ��壬�������ⲿ�ļ��������
  * Description : ���ļ�Ϊ����DGUS������Э��֧���ļ�
  * Date        : 2016��8��22��
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#ifndef _DW_AGREEMENT_H_
#define _DW_AGREEMENT_H_

#include "stm32f10x_usart.h"

#define DW_RX_BUF_LEN			255
#define DW_TX_BUF_LEN			255
#define HEAD1 0xA5  													//֡ͷ
#define HEAD2 0x5A

//ָ��
#define DW_WRITE_REG  0X80 										//д�Ĵ���
#define DW_READ_REG   0X81 										//���Ĵ���
#define DW_WRITE_DATA 0X82 										//д���ݴ洢��
#define DW_READ_DATA  0X83 										//�����ݴ洢��
#define DW_WRITE_CURVE  0X84                  //д���߻�����

typedef struct _DW_Struct
{
	USART_TypeDef *com;													//ָ���Ĵ���
	unsigned char rx_buf[DW_RX_BUF_LEN];				//�������ݴ洢������
	unsigned char tx_buf[DW_TX_BUF_LEN];				//�������ݻ�����
	int rx_byte;																//�Ѿ����յ�����Ч�ֽڸ���
	int tx_byte;																//��Ҫ���͵��ֽ���
	char rx_flag;																//������ɱ�־λ 1--�������  0--����δ���
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
