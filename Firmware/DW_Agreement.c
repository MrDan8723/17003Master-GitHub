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
#include "DW_Agreement.h"
#include "USART_Configuration.h"


/*******************************************************************************
  * name        : void DW_JumpPage(DW_Struct *lcd,int page)
  * Function    : ҳ����ת
  * Parameters  : DW_Struct *lcd-- �ṹ��
                  int page-- Ŀ��ҳ���ַ
	* Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: 
*******************************************************************************/
void DW_JumpPage(DW_Struct *lcd, int page)
{
	lcd->tx_byte=0;
	DW_WriteBuf(lcd,0x80);
	DW_WriteBuf(lcd,0X03);
	DW_Write2Buf(lcd,page);
	DW_SendCode(lcd);
}

/*******************************************************************************
  * name        : void DW_WriteCurve(DW_Struct *lcd,int ch,int *data,int num)
  * Function    : ������
  * Parameters  : DW_Struct *lcd-- �ṹ��
                  int ch-- ͨ��
                  int *data-- ����
                  int num-- ���ݵ����
	* Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: 
*******************************************************************************/
void DW_WriteCurve(DW_Struct *lcd,int ch,int *data,int num)
{
  int i=0;
	lcd->tx_byte=0;
  
	/**********************д���߻�����***********************/
	DW_WriteBuf(lcd,DW_WRITE_CURVE);//���ֽ� 
	
	/**********************����ͨ��***********************/
  DW_WriteBuf(lcd,0x01<<ch);
  
  /**********************д����***********************/	
  for(i=0;i<num;i++)
  {
    DW_Write2Buf(lcd,data[i]);
  }
	
	DW_SendCode(lcd);  
}

/*******************************************************************************
  * name        : void DW_CleanCurve(DW_Struct *lcd,int ch)
  * Function    : ������߻�����
  * Parameters  : DW_Struct *lcd-- �ṹ��
                  int ch-- ͨ��
	* Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: 
*******************************************************************************/
void DW_CleanCurve(DW_Struct *lcd,int ch)
{
  DW_WriteReg(lcd,0xEB,0X56+ch); 
}

/*******************************************************************************
  * name        : void DW_SetCurve(DW_Struct *lcd,int pointer,int width,int height,int num,float y_min,float y_max)
  * Function    : �������߸�ʽ
  * Parameters  : DW_Struct *lcd-- �ṹ��
                  int pointer-- ���߿ؼ�������ָ��
                  int width-- ���߷�Χ�����ص�
                  int height-- ���߷�Χ�ߣ����ص�
                  int num-- �������߻��Ƶĵ�ĸ���
                  float y_min-- �����������Сֵ
                  float y_max-- ������������ֵ
	* Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: 
*******************************************************************************/
void DW_SetCurve(DW_Struct *lcd,int pointer,int width,int height,int num,float y_min,float y_max)
{ 
  int mul_y;                              //���������ߵ�����Ŵ���
  int mid_y;                              //�������Ӧ����������ֵ
  int x_snap;
  
  mid_y= (y_max-y_min)/2;
  DW_WriteData(lcd,pointer+0x06,mid_y);   //�����������Ӧ����������ֵ
  
  mul_y= height * 256 / (y_max-y_min);
  mul_y= mul_y>0x7fff?0x7fff:mul_y;       //�޷�
  DW_WriteData(lcd,pointer+0x08,mul_y);   //��������Ŵ���
  
  x_snap= width/num;
  x_snap= x_snap<1?1:x_snap;
  x_snap= x_snap>0xff?0xff:x_snap;
  x_snap= x_snap & 0xff;
  DW_WriteData(lcd,pointer+0x09,x_snap);  //���ú�����
}


/*******************************************************************************
  * name        : void DW_WriteData(DW_Struct *lcd,int adr,int data)
  * Function    : ��ؼ�д������
  * Parameters  : DW_Struct *lcd-- �ṹ��
                  int adr-- �ؼ���ַ,int data-- ��д������\
	* Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: 
*******************************************************************************/
 void DW_WriteData(DW_Struct *lcd, int adr,int data)
{   
	 lcd->tx_byte=0;
	/**********************дָ��***********************/
	DW_WriteBuf(lcd,DW_WRITE_DATA);//���ֽ�
	
	/**********************д��ַ***********************/
	DW_WriteBuf(lcd,(adr & 0xFF00)>>8);
	DW_WriteBuf(lcd,adr & 0xFF);
	
	/**********************д����***********************/	
	DW_WriteBuf(lcd,(data & 0xFF00)>>8);
	DW_WriteBuf(lcd,data & 0xFF);
	
	DW_SendCode(lcd);
}

/*******************************************************************************
  * name        : void DW_WriteStr(DW_Struct *lcd, int adr,char *str)
  * Function    : ��ؼ�д���ı�����
  * Parameters  : DW_Struct *lcd-- �ṹ�� 
                  int adr-- �ؼ���ַ,char *str-- ָ���ַ�����ָ��
	* Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: 
*******************************************************************************/
void DW_WriteStr(DW_Struct *lcd, int adr,char *str)
{
	int i=0;
	lcd->tx_byte=0;
	/**********************дָ��***********************/
	DW_WriteBuf(lcd,DW_WRITE_DATA);//���ֽ�
	
	/**********************д��ַ***********************/
	DW_WriteBuf(lcd,(adr & 0xFF00)>>8);//���ֽ�
	DW_WriteBuf(lcd,adr & 0xFF);//���ֽ�
	
	/**********************д����***********************/
	for(i=0;str[i] != '\0'; i++)
	{
		DW_WriteBuf(lcd,str[i]);
	}
	DW_SendCode(lcd);
}

/*******************************************************************************
  * name        : void DW_ReadReg(DW_Struct *lcd, int adr,int byte)
  * Function    : ���Ĵ���ֵ
  * Parameters  : DW_Struct *lcd-- �ṹ��
                  int adr-- Ŀ��Ĵ�����ַ,int byte-- ��ȡ���ֽڳ���
	* Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: DW_ReadData(0x03,2) ��ȡ��ǰҳ���ַ��ҳ���ַ�����ֽ�
*******************************************************************************/
void DW_ReadReg(DW_Struct *lcd, int adr,int byte)
{
	lcd->tx_byte=0;
	/**********************дָ��***********************/
	DW_WriteBuf(lcd,DW_READ_REG);//���ֽ�
	
	/**********************д��ַ***********************/
	DW_WriteBuf(lcd,adr);
	
	DW_WriteBuf(lcd,byte);//�ֽڳ�
	
	DW_SendCode(lcd);
}

/*******************************************************************************
  * name        : void DW_WriteReg(DW_Struct *lcd, int adr,int byte)
  * Function    : д�Ĵ���ֵ
  * Parameters  : DW_Struct *lcd-- �ṹ��
                  int adr-- Ŀ��Ĵ�����ַ,int byte-- ��ȡ���ֽڳ���
	* Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: 
*******************************************************************************/
void DW_WriteReg(DW_Struct *lcd, int adr,int byte)
{
	lcd->tx_byte=0;
	/**********************дָ��***********************/
	DW_WriteBuf(lcd,DW_WRITE_REG);//���ֽ�
	
	/**********************д��ַ***********************/
	DW_WriteBuf(lcd,adr);
	
	DW_WriteBuf(lcd,byte);//�ֽڳ�
	
	DW_SendCode(lcd);
}
/*******************************************************************************
  * name        : void DW_ReadData(DW_Struct *lcd, int adr,int word)
  * Function    : ���Ĵ���ֵ
  * Parameters  : DW_Struct *lcd-- �ṹ��
                  int adr-- Ŀ��Ĵ�����ַ,int word-- ��ȡ���ֳ��� word = 2 x byte
	* Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: DW_ReadData(200,1) �ӵ�ַ0x200 ��ʼ��ȡ1���ֳ�����  (2���ֽ�)
*******************************************************************************/
void DW_ReadData(DW_Struct *lcd, int adr,int word)
{
	lcd->tx_byte=0;
	/**********************дָ��***********************/
	DW_WriteBuf(lcd,DW_READ_DATA);//���ֽ�
	
	/**********************д��ַ***********************/
	DW_WriteBuf(lcd,(adr & 0xFF00)>>8);
	DW_WriteBuf(lcd,adr & 0xFF);
	
	DW_WriteBuf(lcd,word);//�ֳ�
	
	DW_SendCode(lcd);
}

/*******************************************************************************
  * name        : void DW_WriteBuf(DW_Struct *lcd,int data)
  * Function    : ���ͻ����������һ���ֽڵ�����
  * Parameters  : DW_Struct *lcd-- �ṹ�����ݼ��ṹ�����Ͷ���ע��,int data-- ���������
	* Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: 
*******************************************************************************/
void DW_WriteBuf(DW_Struct *lcd,int data)
{
	lcd->tx_buf[lcd->tx_byte]= data;						//�򻺳������һ���ֽ�
	lcd->tx_byte++;
}

/*******************************************************************************
  * name        : void DW_Write2Buf(DW_Struct *lcd,int data)
  * Function    : ���ͻ���������������ֽڵ�����
  * Parameters  : DW_Struct *buf-- �ṹ�����ݼ��ṹ�����Ͷ���ע��,int data-- ���������
	* Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: 
*******************************************************************************/
void DW_Write2Buf(DW_Struct *lcd,int data)
{
	lcd->tx_buf[lcd->tx_byte]= (data & 0xFF00)>>8;						//�򻺳������߰�λ
	lcd->tx_byte++;
	lcd->tx_buf[lcd->tx_byte]= (data & 0xFF);									//�򻺳������Ͱ�λ
	lcd->tx_byte++;
}

/*******************************************************************************
  * name        : void DW_SendCode(DW_Struct *lcd)
  * Function    : ����ͨ��Э�鷢��ָ���ṹ���е�һ֡����
  * Parameters  : DW_Struct *lcd-- �ṹ�����ݼ��ṹ�����Ͷ���ע��
	* Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: 
*******************************************************************************/
void DW_SendCode(DW_Struct *lcd)
{
	int j;																										//ѭ�����Ʊ���
	uint16_t temp_buf[DW_TX_BUF_LEN];
	
	temp_buf[0]= HEAD1;
	temp_buf[1]= HEAD2;
	temp_buf[2]= lcd->tx_byte;
	for(j=0; j < lcd->tx_byte ; j++)
	{
		temp_buf[j+3]= lcd->tx_buf[j];
	}
	USART_SendBuf(lcd->com, temp_buf , lcd->tx_byte+3);				//��ָ�����ڷ���ָ���ֽ�����
}

/*******************************************************************************
  * name        : void DW_GetCode(DW_Struct *lcd)
  * Function    : ���յ���ͨ��Э��Ӵ��ڽ���һ֡���ݣ�����ָ���Ľṹ����
  * Parameters  : DW_Struct *lcd-- �ṹ�����ݼ��ṹ�����Ͷ���ע��
	* Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: 
*******************************************************************************/
void DW_GetCode(DW_Struct *lcd)
{
	if(lcd->rx_flag == 0)
	{
		lcd->rx_buf[lcd->rx_byte]= USART_ReceiveData(lcd->com);							 //��������
		if(lcd->rx_buf[0] == HEAD1)
		{
			lcd->rx_byte++;
			lcd->rx_byte= lcd->rx_byte > DW_RX_BUF_LEN ? 0 : lcd->rx_byte;		//���ճ�������
		}
		else lcd->rx_byte= 0;
		
		if(lcd->rx_buf[0] == HEAD1 && 
			 lcd->rx_buf[1] == HEAD2 && 
			 lcd->rx_byte>(lcd->rx_buf[2]+2) )																//ȷ������ȷ��֡
		{
			lcd->rx_byte=0;
			lcd->rx_flag=1;
		}	
		else if(lcd->rx_byte > 4 && lcd->rx_buf[1] != HEAD2)								//����֡
			lcd->rx_byte=0;	
	}
}

/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/

