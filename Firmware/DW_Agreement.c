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
#include "DW_Agreement.h"
#include "USART_Configuration.h"


/*******************************************************************************
  * name        : void DW_JumpPage(DW_Struct *lcd,int page)
  * Function    : 页面跳转
  * Parameters  : DW_Struct *lcd-- 结构体
                  int page-- 目标页面地址
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
  * Function    : 画曲线
  * Parameters  : DW_Struct *lcd-- 结构体
                  int ch-- 通道
                  int *data-- 数据
                  int num-- 数据点个数
	* Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: 
*******************************************************************************/
void DW_WriteCurve(DW_Struct *lcd,int ch,int *data,int num)
{
  int i=0;
	lcd->tx_byte=0;
  
	/**********************写曲线缓冲区***********************/
	DW_WriteBuf(lcd,DW_WRITE_CURVE);//单字节 
	
	/**********************曲线通道***********************/
  DW_WriteBuf(lcd,0x01<<ch);
  
  /**********************写数据***********************/	
  for(i=0;i<num;i++)
  {
    DW_Write2Buf(lcd,data[i]);
  }
	
	DW_SendCode(lcd);  
}

/*******************************************************************************
  * name        : void DW_CleanCurve(DW_Struct *lcd,int ch)
  * Function    : 清除曲线缓冲区
  * Parameters  : DW_Struct *lcd-- 结构体
                  int ch-- 通道
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
  * Function    : 设置曲线格式
  * Parameters  : DW_Struct *lcd-- 结构体
                  int pointer-- 曲线控件的描述指针
                  int width-- 曲线范围宽，像素点
                  int height-- 曲线范围高，像素点
                  int num-- 曲线曲线绘制的点的个数
                  float y_min-- 曲线纵轴的最小值
                  float y_max-- 曲线纵轴的最大值
	* Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: 
*******************************************************************************/
void DW_SetCurve(DW_Struct *lcd,int pointer,int width,int height,int num,float y_min,float y_max)
{ 
  int mul_y;                              //满量程曲线的纵轴放大倍数
  int mid_y;                              //中心轴对应的曲线数据值
  int x_snap;
  
  mid_y= (y_max-y_min)/2;
  DW_WriteData(lcd,pointer+0x06,mid_y);   //设置中心轴对应的曲线数据值
  
  mul_y= height * 256 / (y_max-y_min);
  mul_y= mul_y>0x7fff?0x7fff:mul_y;       //限幅
  DW_WriteData(lcd,pointer+0x08,mul_y);   //设置纵轴放大倍数
  
  x_snap= width/num;
  x_snap= x_snap<1?1:x_snap;
  x_snap= x_snap>0xff?0xff:x_snap;
  x_snap= x_snap & 0xff;
  DW_WriteData(lcd,pointer+0x09,x_snap);  //设置横轴间隔
}


/*******************************************************************************
  * name        : void DW_WriteData(DW_Struct *lcd,int adr,int data)
  * Function    : 向控件写入数据
  * Parameters  : DW_Struct *lcd-- 结构体
                  int adr-- 控件地址,int data-- 被写入数据\
	* Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: 
*******************************************************************************/
 void DW_WriteData(DW_Struct *lcd, int adr,int data)
{   
	 lcd->tx_byte=0;
	/**********************写指令***********************/
	DW_WriteBuf(lcd,DW_WRITE_DATA);//单字节
	
	/**********************写地址***********************/
	DW_WriteBuf(lcd,(adr & 0xFF00)>>8);
	DW_WriteBuf(lcd,adr & 0xFF);
	
	/**********************写数据***********************/	
	DW_WriteBuf(lcd,(data & 0xFF00)>>8);
	DW_WriteBuf(lcd,data & 0xFF);
	
	DW_SendCode(lcd);
}

/*******************************************************************************
  * name        : void DW_WriteStr(DW_Struct *lcd, int adr,char *str)
  * Function    : 向控件写入文本数据
  * Parameters  : DW_Struct *lcd-- 结构体 
                  int adr-- 控件地址,char *str-- 指向字符串的指针
	* Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: 
*******************************************************************************/
void DW_WriteStr(DW_Struct *lcd, int adr,char *str)
{
	int i=0;
	lcd->tx_byte=0;
	/**********************写指令***********************/
	DW_WriteBuf(lcd,DW_WRITE_DATA);//单字节
	
	/**********************写地址***********************/
	DW_WriteBuf(lcd,(adr & 0xFF00)>>8);//高字节
	DW_WriteBuf(lcd,adr & 0xFF);//低字节
	
	/**********************写数据***********************/
	for(i=0;str[i] != '\0'; i++)
	{
		DW_WriteBuf(lcd,str[i]);
	}
	DW_SendCode(lcd);
}

/*******************************************************************************
  * name        : void DW_ReadReg(DW_Struct *lcd, int adr,int byte)
  * Function    : 读寄存器值
  * Parameters  : DW_Struct *lcd-- 结构体
                  int adr-- 目标寄存器地址,int byte-- 读取的字节长度
	* Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: DW_ReadData(0x03,2) 读取当前页面地址，页面地址两个字节
*******************************************************************************/
void DW_ReadReg(DW_Struct *lcd, int adr,int byte)
{
	lcd->tx_byte=0;
	/**********************写指令***********************/
	DW_WriteBuf(lcd,DW_READ_REG);//单字节
	
	/**********************写地址***********************/
	DW_WriteBuf(lcd,adr);
	
	DW_WriteBuf(lcd,byte);//字节长
	
	DW_SendCode(lcd);
}

/*******************************************************************************
  * name        : void DW_WriteReg(DW_Struct *lcd, int adr,int byte)
  * Function    : 写寄存器值
  * Parameters  : DW_Struct *lcd-- 结构体
                  int adr-- 目标寄存器地址,int byte-- 读取的字节长度
	* Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: 
*******************************************************************************/
void DW_WriteReg(DW_Struct *lcd, int adr,int byte)
{
	lcd->tx_byte=0;
	/**********************写指令***********************/
	DW_WriteBuf(lcd,DW_WRITE_REG);//单字节
	
	/**********************写地址***********************/
	DW_WriteBuf(lcd,adr);
	
	DW_WriteBuf(lcd,byte);//字节长
	
	DW_SendCode(lcd);
}
/*******************************************************************************
  * name        : void DW_ReadData(DW_Struct *lcd, int adr,int word)
  * Function    : 读寄存器值
  * Parameters  : DW_Struct *lcd-- 结构体
                  int adr-- 目标寄存器地址,int word-- 读取的字长度 word = 2 x byte
	* Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: DW_ReadData(200,1) 从地址0x200 开始读取1个字长数据  (2个字节)
*******************************************************************************/
void DW_ReadData(DW_Struct *lcd, int adr,int word)
{
	lcd->tx_byte=0;
	/**********************写指令***********************/
	DW_WriteBuf(lcd,DW_READ_DATA);//单字节
	
	/**********************写地址***********************/
	DW_WriteBuf(lcd,(adr & 0xFF00)>>8);
	DW_WriteBuf(lcd,adr & 0xFF);
	
	DW_WriteBuf(lcd,word);//字长
	
	DW_SendCode(lcd);
}

/*******************************************************************************
  * name        : void DW_WriteBuf(DW_Struct *lcd,int data)
  * Function    : 向发送缓冲区中填充一个字节的数据
  * Parameters  : DW_Struct *lcd-- 结构体内容见结构体类型定义注释,int data-- 被填充数据
	* Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: 
*******************************************************************************/
void DW_WriteBuf(DW_Struct *lcd,int data)
{
	lcd->tx_buf[lcd->tx_byte]= data;						//向缓冲区填充一个字节
	lcd->tx_byte++;
}

/*******************************************************************************
  * name        : void DW_Write2Buf(DW_Struct *lcd,int data)
  * Function    : 向发送缓冲区中填充两个字节的数据
  * Parameters  : DW_Struct *buf-- 结构体内容见结构体类型定义注释,int data-- 被填充数据
	* Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: 
*******************************************************************************/
void DW_Write2Buf(DW_Struct *lcd,int data)
{
	lcd->tx_buf[lcd->tx_byte]= (data & 0xFF00)>>8;						//向缓冲区填充高八位
	lcd->tx_byte++;
	lcd->tx_buf[lcd->tx_byte]= (data & 0xFF);									//向缓冲区填充低八位
	lcd->tx_byte++;
}

/*******************************************************************************
  * name        : void DW_SendCode(DW_Struct *lcd)
  * Function    : 按照通信协议发送指定结构体中的一帧数据
  * Parameters  : DW_Struct *lcd-- 结构体内容见结构体类型定义注释
	* Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: 
*******************************************************************************/
void DW_SendCode(DW_Struct *lcd)
{
	int j;																										//循环控制变量
	uint16_t temp_buf[DW_TX_BUF_LEN];
	
	temp_buf[0]= HEAD1;
	temp_buf[1]= HEAD2;
	temp_buf[2]= lcd->tx_byte;
	for(j=0; j < lcd->tx_byte ; j++)
	{
		temp_buf[j+3]= lcd->tx_buf[j];
	}
	USART_SendBuf(lcd->com, temp_buf , lcd->tx_byte+3);				//向指定串口发送指定字节数据
}

/*******************************************************************************
  * name        : void DW_GetCode(DW_Struct *lcd)
  * Function    : 按照迪文通信协议从串口接收一帧数据，存在指定的结构体中
  * Parameters  : DW_Struct *lcd-- 结构体内容见结构体类型定义注释
	* Returns     : Null
  * Author      : XYD
  * Check       : 
	* Note				: 
*******************************************************************************/
void DW_GetCode(DW_Struct *lcd)
{
	if(lcd->rx_flag == 0)
	{
		lcd->rx_buf[lcd->rx_byte]= USART_ReceiveData(lcd->com);							 //接收数据
		if(lcd->rx_buf[0] == HEAD1)
		{
			lcd->rx_byte++;
			lcd->rx_byte= lcd->rx_byte > DW_RX_BUF_LEN ? 0 : lcd->rx_byte;		//接收长度限制
		}
		else lcd->rx_byte= 0;
		
		if(lcd->rx_buf[0] == HEAD1 && 
			 lcd->rx_buf[1] == HEAD2 && 
			 lcd->rx_byte>(lcd->rx_buf[2]+2) )																//确定是正确的帧
		{
			lcd->rx_byte=0;
			lcd->rx_flag=1;
		}	
		else if(lcd->rx_byte > 4 && lcd->rx_buf[1] != HEAD2)								//错误帧
			lcd->rx_byte=0;	
	}
}

/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/

