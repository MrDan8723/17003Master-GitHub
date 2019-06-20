/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : stm32f10x_it.c
  * Version     : V0.1
  * Description : �жϷ������
  * Date        : 2017��9��11��
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
  /***�����ݼĴ����ǿձ�־λ***/
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)     // �����ݼĴ����ǿձ�־λ
  {			

	}
  
  /***���ش����־λ***/
  if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)  // ���ش����־λ��1��⵽���ش���
  {
    (void)USART_ReceiveData(USART1);                         // �������ݵ��ֽ�
  }
  /***�������ݼĴ����ձ�־λ***/
  if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)       // ����Ƿ��ͼĴ����ձ�־λ��λ
  {   
    USART_SendData(USART1,0x79);                             // ��һ���ֽڵ�����1����USARTx->DR��д���������Խ���λ����
  }
}
/*******************************************************************************
  * name        : USART2_IRQHandler(void)
  * Function    : usart2 IRQ��
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void USART2_IRQHandler(void)
{
  /***�����ݼĴ����ǿձ�־λ***/  
  if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)     // �����ݼĴ����ǿձ�־λ
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
  /***���ش����־λ***/
  if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)  // ���ش����־λ��1��⵽���ش���
  {
    (void)USART_ReceiveData(USART2);                         // �������ݵ��ֽ�
  }
  /***�������ݼĴ����ձ�־λ***/
  if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)       // ����Ƿ��ͼĴ����ձ�־λ��λ
  {   
    USART_SendData(USART2,0x79);                             // ��һ���ֽڵ�����2����USARTx->DR��д���������Խ���λ����
  }
}

/*******************************************************************************
  * name        : USART3_IRQHandler(void)
  * Function    : usart3 IRQ��
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void USART3_IRQHandler(void)
{
  int x,y,z;
  /***�����ݼĴ����ǿձ�־λ***/  
  if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)     // �����ݼĴ����ǿձ�־λ
  {     
    LabviewDecod();                                          //Labview����
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
  /***���ش����־λ***/
  if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)  // ���ش����־λ��1��⵽���ش���
  {
    (void)USART_ReceiveData(USART3);                         // �������ݵ��ֽ�
  }
  /***�������ݼĴ����ձ�־λ***/
  if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)       // ����Ƿ��ͼĴ����ձ�־λ��λ
  {   
    USART_SendData(USART3,0x79);                             // ��һ���ֽڵ�����3����USARTx->DR��д���������Խ���λ����
  }
}

/*******************************************************************************
  * name        : UART4_IRQHandler(void)
  * Function    : uart4 IRQ��
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void UART4_IRQHandler(void)
{
  /***�����ݼĴ����ǿձ�־λ***/
  if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)      // �����ݼĴ����ǿձ�־λ
  {				
    DW_GetCode(&LCD);
    if(LCD.rx_flag == 1)
    {
      LCD.rx_flag=0;
      LCD_Decode(&LCD);
    }
	}
  /***���ش����־λ***/
  if (USART_GetFlagStatus(UART4, USART_FLAG_ORE) != RESET)  // ���ش����־λ��1��⵽���ش���
  {
    (void)USART_ReceiveData(UART4);                         // �������ݵ��ֽ�
  }
  /***�������ݼĴ����ձ�־λ***/
  if(USART_GetITStatus(UART4, USART_IT_TXE) != RESET)       // ����Ƿ��ͼĴ����ձ�־λ��λ
  {   
		USART_SendData(UART4,0x79);                             // ��һ���ֽڵ�����4����USARTx->DR��д���������Խ���λ����
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
  /***�����ݼĴ����ǿձ�־λ***/
  if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)      // �����ݼĴ����ǿձ�־λ
  {				
    DW_GetCode(&LCD);     
    if(LCD.rx_flag == 1)
    {
      LCD.rx_flag=0;
      LCD_Decode(&LCD);
    }
	}
  /***���ش����־λ***/
  if (USART_GetFlagStatus(UART5, USART_FLAG_ORE) != RESET)  // ���ش����־λ��1��⵽���ش���
  {
    (void)USART_ReceiveData(UART5);                         // �������ݵ��ֽ�
  }
  /***�������ݼĴ����ձ�־λ***/
  if(USART_GetITStatus(UART5, USART_IT_TXE) != RESET)       // ����Ƿ��ͼĴ����ձ�־λ��λ
  {   
		USART_SendData(UART5,0x79);                             // ��һ���ֽڵ�����4����USARTx->DR��д���������Խ���λ����
  }
}

/*******************************************************************************
  * name        : EXTI2_IRQHandler(void)
  * Function    : 2���ж����жϷ�����򣬽������Լ����ż�⼰����
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void EXTI2_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line2)!= RESET)                                                           
  {
   
		EXTI_ClearITPendingBit(EXTI_Line2);                                         // ���Line2�ϵ��жϱ�־λ 
  }     
}

/*******************************************************************************
  * name        : EXTI3_IRQHandler(void)
  * Function    : 3���ж����жϷ�����򣬽�����ø��λ��⼰����
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void EXTI3_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line3)!= RESET)                                                           
  {
   
		EXTI_ClearITPendingBit(EXTI_Line3);                                         // ���Line3�ϵ��жϱ�־λ 
  }     
}

/*******************************************************************************
  * name        : EXTI4_IRQHandler(void)
  * Function    : 4���ж����жϷ�����򣬽�����PID�屨���жϼ�����
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void EXTI4_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line4)!= RESET)                                                           
  {
   
		EXTI_ClearITPendingBit(EXTI_Line4);                                         // ���Line4�ϵ��жϱ�־λ 
  }     
}

/*******************************************************************************
  * name        : EXTI9_5_IRQHandler(void)
  * Function    : 5~9���ж����жϷ�����򣬽����ڴ������屨�������û���ͣ�����봦��
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line5)!= RESET)       																//�����������ж�                                                    
  {
   
		EXTI_ClearITPendingBit(EXTI_Line5);                                         // ���Line5�ϵ��жϱ�־λ 
  }    
	else if(EXTI_GetITStatus(EXTI_Line9)!= RESET)       													//��ͣ���ж�                                                   
  {
   
		EXTI_ClearITPendingBit(EXTI_Line9);                                         // ���Line9�ϵ��жϱ�־λ 
  }  
}

/*******************************************************************************
  * name        : EXTI15_10_IRQHandler(void)
  * Function    : 10~15���ж����жϷ�����򣬽����ڷ�Һ��λ��⼰����
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
//			while(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15) == 0);//��ͣ
		}
		else
		{
//			CAN_CmdPause(&CanTest,CAN_X_AXIS_ID,0);;
		}
		EXTI_ClearITPendingBit(EXTI_Line15);                                         // ���Line15�ϵ��жϱ�־λ 
  }     
}

/*******************************************************************************
  * name        : void TIM5_IRQHandler(void)
  * Function    : ��ʱ��5��1s�����ж��жϷ������
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
  int detect_num;                                         //��ѯ�ĵ�ǰ�ļ��λ��
  
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
			DW_WriteData(&LCD,0x0200,Temp1);                  //��ʾ�¶�
			DW_WriteData(&LCD,0x0202,Temp2);                  //��ʾ�¶�
		}
    //ʵ���ʱͳ��
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
    
    
    for(sample_cnt= (MaxRackNum-2)*9;sample_cnt<MaxRackNum*9;sample_cnt++)                        //��ѯ��ǰ��18������
		{
			if(Sample[sample_cnt].state == 1)                                                            //��Ӧ��
			{
				for(item_cnt=0;Sample[sample_cnt].item_list[item_cnt].type != 0;item_cnt++)                //��ѯ�����б��е�������Ŀ
				{
					detect_num= Sample[sample_cnt].item_list[item_cnt].detect_num;                           //��ȡ���λ���
					
					if(DetectDelay[detect_num].enable == 1)
					{
						if(Sample[sample_cnt].item_list[item_cnt].item_state == ITEM_ADD_R1_FINISH)            //���R1�Ƿ�Ӧ���
						{
							if(DetectDelay[detect_num].now >= DetectDelay[detect_num].set)                       //��ʱʱ�䵽��������
							{
								ClearSoftTimer(&DetectDelay[detect_num]);                                          //�������Ŀ��ʹ�õ������ʱ��
								if(TestMethod[Sample[sample_cnt].item_list[item_cnt].type].r2_type == 0)           //����Ŀû��R2
								{
									Sample[sample_cnt].item_list[item_cnt].item_state = ITEM_TEST_FINISH;            //���ø���Ŀ�������
								}
								else 
								{
									Sample[sample_cnt].item_list[item_cnt].item_state = ITEM_R1_TEST_FINISH;         //�������
								}
							}       
						}
						else if(Sample[sample_cnt].item_list[item_cnt].item_state == ITEM_ADD_R2_FINISH)       //���R2�Ƿ�Ӧ���
						{
							if(DetectDelay[detect_num].now >= DetectDelay[detect_num].set)                       //��ʱʱ�䵽��������
							{
								ClearSoftTimer(&DetectDelay[detect_num]);                                          //�������Ŀ��ʹ�õ������ʱ��
								Sample[sample_cnt].item_list[item_cnt].item_state = ITEM_TEST_FINISH;              //���ø���Ŀ�������
							}       
						}
						else if(Sample[sample_cnt].item_list[item_cnt].item_state == ITEM_SAMPLE_FINISH)       //����Ŀ�������
						{
							if(DetectDelay[detect_num].now >= DetectDelay[detect_num].set)                       //����Ŀ�������
							{
								ClearSoftTimer(&DetectDelay[detect_num]);                                          //�������Ŀ��ʹ�õ������ʱ��
								Sample[sample_cnt].item_list[item_cnt].item_state= ITEM_INCUBATE_FINISH;           //������Ŀ��״̬����Ϊ������ɣ��ȴ�����R1
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
  * Function    : CANͨ�Ž����ж�
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


