/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : Delay.c
  * Version     : V0.1
  * Description : 延时函数文件
  * Date        : 2017年9月29日
  * Author      : ZQChen
  * Check       :  
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include <stm32f10x.h>
 #include "Delay.h"

/* Private constants ---------------------------------------------------------*/
static u8  Fac_Us=0;
static u16 Fac_Ms=0;


/*******************************************************************************
  * name        : Delay_Init(u8 SYSCLK)
  * Function    : Initialize the Systemclock。
  * Parameters  : The frequency is 72 MHZ。
  * Returns     : Null
  * Author      : Eric Chou
  * Check       :
*******************************************************************************/
void Delay_Init(u8 SYSCLK)
{
  SysTick->CTRL&=0xfffffffb;
  Fac_Us=SYSCLK/8;		
  Fac_Ms=(u16)Fac_Us*1000;
}						 		

/*******************************************************************************
  * name        : Delay_Us(u32 nus)
  * Function    : Precise microsecond delay。
  * Parameters  : microsec。Define a us...
  * Returns     : Null
  * Author      : Eric Chou
  * Check       :
*******************************************************************************/
void Delay_Us(u32 microsec)
{		
  u32 temp;	    	
  SysTick->LOAD=microsec*Fac_Us;                                                    		
  SysTick->VAL=0x00;
  SysTick->CTRL=0x01;
  do
  {
    temp=SysTick->CTRL;
  }
  while((temp&0x01)&&!(temp&(1<<16)));
  SysTick->CTRL=0x00;
  SysTick->VAL =0X00;                                                          	
}

/*******************************************************************************
  * name        : Delay_Ms(u16 nms)
  * Function    : Precise millisecond delay。
  * Parameters  : n。Define a ms...
  * Returns     : Null
  * Author      : Eric Chou
  * Check       :
*******************************************************************************/
void Delay_Ms(u16 millisec)
{	 		  	
  u32 temp;		
  SysTick->LOAD=(u32)millisec*Fac_Ms;
  SysTick->VAL =0x00; 
  SysTick->CTRL=0x01;
  do
  {
    temp=SysTick->CTRL;
  }
  while((temp&0x01)&&!(temp&(1<<16)));
  SysTick->CTRL=0x00;
  SysTick->VAL =0X00;
}

/*******************************************************************************
  * name        : Delay_Half_S(u32 halfsec) 
  * Function    : Precise half second delay。
  * Parameters  : sec。Define a s...
  * Returns     : Null
  * Author      : Eric Chou
  * Check       : 
*******************************************************************************/
void Delay_Half_S(u32 halfsec) 
{ 
  uint32_t Cycle_Time=0;
  for(Cycle_Time=1;Cycle_Time<=halfsec;Cycle_Time++)
  {
    Delay_Ms(500);
  }
}

/*******************************************************************************
  * name        : Delay_S(u32 sec)
  * Function    : Precise second delay。
  * Parameters  : sec。Define a s...
  * Returns     : Null
  * Author      : Eric Chou
  * Check       :
*******************************************************************************/
void Delay_S(u32 sec)
{
  u32 Cycle_Time=0;
  u32 temp;
  for(Cycle_Time=1;Cycle_Time<=sec;Cycle_Time++)
  {
    SysTick->LOAD=0x895400;                                                      		
    SysTick->VAL=0x00;
    SysTick->CTRL=0x01;
    do
    {
      temp=SysTick->CTRL;
    }
    while((temp&0x01)&&!(temp&(1<<16)));
    SysTick->CTRL=0x00;
    SysTick->VAL =0X00;
		
  }
}
/**************** (C) COPYRIGHT  2017 DOC-AI *****END OF FILE****************/

