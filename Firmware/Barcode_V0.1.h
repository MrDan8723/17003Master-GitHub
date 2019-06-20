/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : Barcode_V0.1.h
  * Version     : V0.1
  * Description : �ſ����E2000��ά����ɨ������������
  * Date        : 2017��12��20��
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#ifndef _BARCODE_V0_1_H_
#define _BARCODE_V0_1_H_


/*********************File include*********************/
#include "stm32f10x_usart.h"

/******************Macro definition********************/
#define BARCODE_LEN     50                         //���볤��

/*********Enumeration variable type definition********/



/**************Structure type definition**************/

//�ṹ�����Ͷ��壬���ھ�����������ṹ������
typedef struct _BarcodeTypeDef
{
	USART_TypeDef *com;													      //ָ���Ĵ���
	uint16_t rx_buf[BARCODE_LEN];				              //�������ݴ洢������
  char code_str[BARCODE_LEN];                       //��������
	int rx_byte;																      //�Ѿ����յ�����Ч�ֽڸ���
	char rx_flag;																      //������ɱ�־λ 1--�������  0--����δ���
}BarcodeTypeDef;


/*****************Function declaration**************/

void BC_GetCode(BarcodeTypeDef *buf);





 #endif
/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/
