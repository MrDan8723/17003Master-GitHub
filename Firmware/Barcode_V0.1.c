/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : Barcode_V0.1.h
  * Version     : V0.1
  * Description : �ſ����E2000��ά����ɨ������������
  * Date        : 2017��12��20��
  * Author      : XYD
  * Check       :  
*******************************************************************************/



/*********************File include*********************/
#include "Barcode_V0.1.h"   

/******************Macro definition********************/

/*************External variable declaration************/

/***************Global variable definition*************/


/**************************************************************************************
  * name        : void BC_GetCode(BarcodeTypeDef *buf)
  * Function    : ����һ֡�������ݣ�У��ͨ���󽫽ṹ���е�rx_flag��1
  * Parameters  : BarcodeTypeDef *buf--ɨ�����ṹ��
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void BC_GetCode(BarcodeTypeDef *buf) 
{
  int i=0;
  
  if(buf->rx_flag == 0)
  {
    buf->rx_buf[buf->rx_byte]= USART_ReceiveData(buf->com);					 //��������
    buf->rx_byte++;
    if(buf->rx_buf[buf->rx_byte-2] == 0x0D &&                        //���֡β
       buf->rx_buf[buf->rx_byte-1] == 0x0A)
    {
      for(i=0;i < buf->rx_byte-2 ; i++)
      {
        buf->code_str[i]= buf->rx_buf[i];
      }
      buf->code_str[i+1]= '\0';
      buf->rx_byte= 0;                                              //�������㣬�����´ν���
      buf->rx_flag= 1;                                              //��λ���ձ�־λ
    }
  }
}


/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/












