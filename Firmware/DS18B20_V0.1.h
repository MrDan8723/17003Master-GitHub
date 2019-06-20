/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : DS18B20_V0.1.c
  * Version     : V0.1
  * Description : DS18B20��������,���ӹ���ʹ��ͬʱ֧�ֶ��DS18B20
  * Date        : 2017��9��13��
  * Author      : XYD
  * Check       :  
*******************************************************************************/

#ifndef _DS18B20_V0_1_H_
#define _DS18B20_V0_1_H_ 


/*********************File include*********************/
#include "stm32f10x.h"

/******************Macro definition********************/


/*********Enumeration variable type definition********/


/**************Structure type definition**************/
//DS18B20�����ṹ��
typedef struct _DS18B20_TypeDef
{
  uint16_t          pin;                //ʹ�õ����ţ���ֵ��GPIO��ʼ��ʱһ��������GPIO_Pin_0
  GPIO_TypeDef*     port;               //ʹ�õ����Ŷ˿ڣ�����GPIOA
  int               temp;               //�¶�ֵ
}DS18B20_TypeDef;

/*****************Function declaration**************/
//�û��ӿں���
uint8_t DS18B20_Init(DS18B20_TypeDef *sensor,GPIO_TypeDef* port,uint16_t pin);
uint8_t DS18B20_Get_Temp(DS18B20_TypeDef *sensor);



//�ײ�֧�ֺ������û��������
void DS18B20_Reset(DS18B20_TypeDef *sensor);	   
uint8_t DS18B20_Check(DS18B20_TypeDef *sensor);
uint8_t DS18B20_Read_Bit(DS18B20_TypeDef *sensor); 		
uint8_t DS18B20_Read_Byte(DS18B20_TypeDef *sensor);
void DS18B20_Write_Byte(DS18B20_TypeDef *sensor,uint8_t dat);
uint8_t GetCRC(uint8_t *str, uint32_t length);
uint8_t DS18B20_Get_Serial(DS18B20_TypeDef *sensor,uint8_t *serial);
void DS18B20_IO_OutputMode(DS18B20_TypeDef *sensor);
void DS18B20_IO_OutputHigh(DS18B20_TypeDef *sensor);  
uint8_t DS18B20_IO_Input(DS18B20_TypeDef *sensor);
void DS18B20_IO_OutputLow(DS18B20_TypeDef *sensor);
void DS18B20_IO_InputMode(DS18B20_TypeDef *sensor);
void DS18B20_Delay_us(uint16_t n);

 #endif
/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/
