/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : Barcode_V0.1.h
  * Version     : V0.1
  * Description : 优库电子E2000二维条码扫码器驱动程序
  * Date        : 2017年12月20日
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
  * Function    : 接收一帧条码内容，校验通过后将结构体中的rx_flag置1
  * Parameters  : BarcodeTypeDef *buf--扫描器结构体
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
    buf->rx_buf[buf->rx_byte]= USART_ReceiveData(buf->com);					 //接收数据
    buf->rx_byte++;
    if(buf->rx_buf[buf->rx_byte-2] == 0x0D &&                        //检查帧尾
       buf->rx_buf[buf->rx_byte-1] == 0x0A)
    {
      for(i=0;i < buf->rx_byte-2 ; i++)
      {
        buf->code_str[i]= buf->rx_buf[i];
      }
      buf->code_str[i+1]= '\0';
      buf->rx_byte= 0;                                              //计数清零，便于下次接收
      buf->rx_flag= 1;                                              //置位接收标志位
    }
  }
}


/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/












