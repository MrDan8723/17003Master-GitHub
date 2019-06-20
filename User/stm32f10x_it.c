/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : stm32f10x_it.c
  * Version     : V0.1
  * Description : 中断服务程序
  * Date        : 2017年9月11日
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#include "stm32f10x_usart.h"
#include "SysWork.h"
#include "17003UI.h"


extern MNTC_CanProtocolTypeDef CanCom;
extern DW_Struct  LCD;									

/*******************************************************************************
  * name        : USART1_IRQHandler(void)
  * Function    : usart1 IRQ?
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void USART1_IRQHandler(void)
{
  /***读数据寄存器非空标志位***/
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)     // 读数据寄存器非空标志位
  {			

	}
  
  /***过载错误标志位***/
  if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)  // 过载错误标志位，1检测到过载错误
  {
    (void)USART_ReceiveData(USART1);                         // 接收数据单字节
  }
  /***发送数据寄存器空标志位***/
  if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)       // 如果是发送寄存器空标志位置位
  {   
    USART_SendData(USART1,0x79);                             // 发一个字节到串口1，对USARTx->DR的写操作，可以将该位清零
  }
}
/*******************************************************************************
  * name        : USART2_IRQHandler(void)
  * Function    : usart2 IRQ。
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void USART2_IRQHandler(void)
{
  /***读数据寄存器非空标志位***/  
  if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)     // 读数据寄存器非空标志位
  { 
		BC_GetCode(&Barcode); 
    if(Barcode.rx_flag == 1)
    {
      
      Barcode.code_str[0]= '2';
      Barcode.code_str[1]= '1';
      Barcode.code_str[2]= '7';
      Barcode.code_str[3]= '0';
      Barcode.code_str[4]= '8';
      Barcode.code_str[5]= '3';
      Barcode.code_str[6]= '\0';
      
      ScanerDecode(&Barcode);
      
      Barcode.rx_flag=0;
      Barcode.rx_byte=0;
    }
  }
  /***过载错误标志位***/
  if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)  // 过载错误标志位，1检测到过载错误
  {
    (void)USART_ReceiveData(USART2);                         // 接收数据单字节
  }
  /***发送数据寄存器空标志位***/
  if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)       // 如果是发送寄存器空标志位置位
  {   
    USART_SendData(USART2,0x79);                             // 发一个字节到串口2，对USARTx->DR的写操作，可以将该位清零
  }
}

/*******************************************************************************
  * name        : USART3_IRQHandler(void)
  * Function    : usart3 IRQ。
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void USART3_IRQHandler(void)
{
  int x,y,z;
  /***读数据寄存器非空标志位***/  
  if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)     // 读数据寄存器非空标志位
  {     
    LabviewDecod();                                          //Labview调试
    if(LabviewFlag == 1)
    {
      x= LabviewBuf[2]<<8 | LabviewBuf[3];
      y= LabviewBuf[5]<<8 | LabviewBuf[6];
      z= LabviewBuf[8]<<8 | LabviewBuf[9];
      x= x>64000?x-65535:x;
      y= y>64000?y-65535:y;
      z= z>64000?z-65535:z;
      AxisRun2Pos(x,y,z);
      LabviewFlag=0;
    }
  }
  /***过载错误标志位***/
  if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)  // 过载错误标志位，1检测到过载错误
  {
    (void)USART_ReceiveData(USART3);                         // 接收数据单字节
  }
  /***发送数据寄存器空标志位***/
  if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)       // 如果是发送寄存器空标志位置位
  {   
    USART_SendData(USART3,0x79);                             // 发一个字节到串口3，对USARTx->DR的写操作，可以将该位清零
  }
}

/*******************************************************************************
  * name        : UART4_IRQHandler(void)
  * Function    : uart4 IRQ。
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void UART4_IRQHandler(void)
{
  /***读数据寄存器非空标志位***/
  if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)      // 读数据寄存器非空标志位
  {				
    DW_GetCode(&LCD);
    if(LCD.rx_flag == 1)
    {
      LCD.rx_flag=0;
      LCD_Decode(&LCD);
    }
	}
  /***过载错误标志位***/
  if (USART_GetFlagStatus(UART4, USART_FLAG_ORE) != RESET)  // 过载错误标志位，1检测到过载错误
  {
    (void)USART_ReceiveData(UART4);                         // 接收数据单字节
  }
  /***发送数据寄存器空标志位***/
  if(USART_GetITStatus(UART4, USART_IT_TXE) != RESET)       // 如果是发送寄存器空标志位置位
  {   
		USART_SendData(UART4,0x79);                             // 发一个字节到串口4，对USARTx->DR的写操作，可以将该位清零
  }
}

/*******************************************************************************
  * name        : UART5_IRQHandler(void)
  * Function    : uart5 IRQ
  * Parameters  : Null
  * Returns     : Null
  * Author      : ZQChen
  * Check       : 
*******************************************************************************/
void UART5_IRQHandler(void)
{
  /***读数据寄存器非空标志位***/
  if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)      // 读数据寄存器非空标志位
  {				
    DW_GetCode(&LCD);     
    if(LCD.rx_flag == 1)
    {
      LCD.rx_flag=0;
      LCD_Decode(&LCD);
    }
	}
  /***过载错误标志位***/
  if (USART_GetFlagStatus(UART5, USART_FLAG_ORE) != RESET)  // 过载错误标志位，1检测到过载错误
  {
    (void)USART_ReceiveData(UART5);                         // 接收数据单字节
  }
  /***发送数据寄存器空标志位***/
  if(USART_GetITStatus(UART5, USART_IT_TXE) != RESET)       // 如果是发送寄存器空标志位置位
  {   
		USART_SendData(UART5,0x79);                             // 发一个字节到串口4，对USARTx->DR的写操作，可以将该位清零
  }
}

/*******************************************************************************
  * name        : EXTI2_IRQHandler(void)
  * Function    : 2号中断线中断服务程序，将于用试剂舱门检测及处理
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void EXTI2_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line2)!= RESET)                                                           
  {
   
		EXTI_ClearITPendingBit(EXTI_Line2);                                         // 清除Line2上的中断标志位 
  }     
}

/*******************************************************************************
  * name        : EXTI3_IRQHandler(void)
  * Function    : 3号中断线中断服务程序，将用于酶到位检测及处理
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void EXTI3_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line3)!= RESET)                                                           
  {
   
		EXTI_ClearITPendingBit(EXTI_Line3);                                         // 清除Line3上的中断标志位 
  }     
}

/*******************************************************************************
  * name        : EXTI4_IRQHandler(void)
  * Function    : 4号中断线中断服务程序，将用于PID板报警中断及处理
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void EXTI4_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line4)!= RESET)                                                           
  {
   
		EXTI_ClearITPendingBit(EXTI_Line4);                                         // 清除Line4上的中断标志位 
  }     
}

/*******************************************************************************
  * name        : EXTI9_5_IRQHandler(void)
  * Function    : 5~9号中断线中断服务程序，将用于传感器板报警处理及用户暂停键输入处理
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line5)!= RESET)       																//传感器报警中断                                                    
  {
   
		EXTI_ClearITPendingBit(EXTI_Line5);                                         // 清除Line5上的中断标志位 
  }    
	else if(EXTI_GetITStatus(EXTI_Line9)!= RESET)       													//暂停键中断                                                   
  {
   
		EXTI_ClearITPendingBit(EXTI_Line9);                                         // 清除Line9上的中断标志位 
  }  
}

/*******************************************************************************
  * name        : EXTI15_10_IRQHandler(void)
  * Function    : 10~15号中断线中断服务程序，将用于废液到位检测及处理
  * Parameters  : Null
  * Returns     : Null
  * Author      : ZQChen
  * Check       : 
*******************************************************************************/
void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line15)!= RESET)                                                           
  {
		if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15) == 0)
		{
//			CAN_CmdPause(&CanTest,CAN_X_AXIS_ID,1);
//			while(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15) == 0);//暂停
		}
		else
		{
//			CAN_CmdPause(&CanTest,CAN_X_AXIS_ID,0);;
		}
		EXTI_ClearITPendingBit(EXTI_Line15);                                         // 清除Line15上的中断标志位 
  }     
}

/*******************************************************************************
  * name        : void TIM5_IRQHandler(void)
  * Function    : 定时器5的1s周期中断中断服务程序
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void TIM5_IRQHandler(void)
{
  static int flag;
	static int time=0;
  int i;
  int sample_cnt;
  int item_cnt;
  int detect_num;                                         //查询的当前的检测位号
  
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	{
		if(flag == 0)
    {
      LED1_OFF;
      LED2_OFF;
      flag=1;
    }
    else 
    {
      LED1_ON;
      LED2_ON;
      flag=0;
    }
    time++;
		if(time == 3)
		{
			time=0;
			DW_WriteData(&LCD,0x0200,Temp1);                  //显示温度
			DW_WriteData(&LCD,0x0202,Temp2);                  //显示温度
		}
    //实验计时统计
    Second++;
    if(Second >= 60) 
    {
      Minute++;
      Second=0;
    }
    if(Minute >= 60)
    {
      Hour++;
      Minute=0;
    }
    
    for(i=0;i<DETECT_SEAT_NUM;i++)
    {
      if(DetectDelay[i].enable == 1)
      {
        DetectDelay[i].now++;
      }
    }
    
    
    for(sample_cnt= (MaxRackNum-2)*9;sample_cnt<MaxRackNum*9;sample_cnt++)                        //轮询当前的18个样本
		{
			if(Sample[sample_cnt].state == 1)                                                            //反应中
			{
				for(item_cnt=0;Sample[sample_cnt].item_list[item_cnt].type != 0;item_cnt++)                //轮询测试列表中的所有项目
				{
					detect_num= Sample[sample_cnt].item_list[item_cnt].detect_num;                           //获取检测位编号
					
					if(DetectDelay[detect_num].enable == 1)
					{
						if(Sample[sample_cnt].item_list[item_cnt].item_state == ITEM_ADD_R1_FINISH)            //检查R1是否反应完成
						{
							if(DetectDelay[detect_num].now >= DetectDelay[detect_num].set)                       //定时时间到，测试用
							{
								ClearSoftTimer(&DetectDelay[detect_num]);                                          //清零该项目所使用的软件定时器
								if(TestMethod[Sample[sample_cnt].item_list[item_cnt].type].r2_type == 0)           //该项目没有R2
								{
									Sample[sample_cnt].item_list[item_cnt].item_state = ITEM_TEST_FINISH;            //设置该项目测试完成
								}
								else 
								{
									Sample[sample_cnt].item_list[item_cnt].item_state = ITEM_R1_TEST_FINISH;         //测试完成
								}
							}       
						}
						else if(Sample[sample_cnt].item_list[item_cnt].item_state == ITEM_ADD_R2_FINISH)       //检查R2是否反应完成
						{
							if(DetectDelay[detect_num].now >= DetectDelay[detect_num].set)                       //定时时间到，测试用
							{
								ClearSoftTimer(&DetectDelay[detect_num]);                                          //清零该项目所使用的软件定时器
								Sample[sample_cnt].item_list[item_cnt].item_state = ITEM_TEST_FINISH;              //设置该项目测试完成
							}       
						}
						else if(Sample[sample_cnt].item_list[item_cnt].item_state == ITEM_SAMPLE_FINISH)       //该项目加样完成
						{
							if(DetectDelay[detect_num].now >= DetectDelay[detect_num].set)                       //该项目孵育完成
							{
								ClearSoftTimer(&DetectDelay[detect_num]);                                          //清零该项目所使用的软件定时器
								Sample[sample_cnt].item_list[item_cnt].item_state= ITEM_INCUBATE_FINISH;           //将该项目的状态设置为孵育完成，等待加入R1
							}
						}
					}//end if(DetectDelay[detect_num].enable == 1)
				}// end for
			}
		}//end for
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	}
}

/*******************************************************************************
  * name        : void CAN1_RX0_IRQHandler(void)
  * Function    : CAN通信接收中断
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void CAN1_RX0_IRQHandler(void)
{
  CAN_Decode(&CanCom);
}

void HardFault_Handler(void)
{
  while(1)
  {
    
  }
}

/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/


