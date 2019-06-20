/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : TemperatureCtrl_V0.1.c
  * Version     : V0.1
  * Description : 温度控制程序
                  1、V0.1扩展至支持多通道温度控制
  * Date        : 2017年9月13日
  * Author      : XYD
  * Check       :  
*******************************************************************************/



/*********************File include*********************/
#include "TemperatureCtrl_V0.1.h"
#include "DS18B20_V0.1.h"
#include "USART_Configuration.h"


/******************Macro definition********************/

/*************External variable declaration************/

/***************Global variable definition*************/


/**************************************************************************************
  * name        : void PID_HardwareInit(TempCtrlTypeDef *pid,TIM_TypeDef *timer,int ch,int remap)
  * Function    : PID控制器硬件初始化
  * Parameters  : TempCtrlTypeDef *pid-- PID控制器参数
                  TIM_TypeDef *time-- 分配给该PID控制器使用的定时器
                  int ch-- 分配给该PID控制器的定时器通道
                  int remap-- 是否使用重定向引脚 0--不使用 1--使用
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void PID_HardwareInit(TempCtrlTypeDef *pid,TIM_TypeDef *timer,int ch,int remap)
{
  pid->timer= timer;
  pid->ch   = ch;
  pid->remap= remap;
  PID_PinInit(pid);
  PID_TimerInit(pid);
}

/**************************************************************************************
  * name        : void PID_ParamInit(TempCtrlTypeDef *pid,int target_temp,float kp,float ki,float kd)
  * Function    : PID控制器参数初始化
  * Parameters  : TempCtrlTypeDef *pid-- PID控制器参数
                  int target_temp-- 目标温度
                  float kp-- 参数p
                  float ki-- 参数i
                  float kd-- 参数d
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void PID_ParamInit(TempCtrlTypeDef *pid,int target_temp,float kp,float ki,float kd)
{
  pid->target_temp= target_temp;
  pid->kp         = kp;
  pid->ki         = ki;
  pid->kd         = kd;
}

/**************************************************************************************
  * name        : void PID_IncrementalCtrl(TempCtrlTypeDef *pid,int crt_temp)
  * Function    : 增量式PID控制算法
  * Parameters  : TempCtrlTypeDef *pid-- PID控制器参数
                  int crt_temp-- 当前温度
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void PID_IncrementalCtrl(TempCtrlTypeDef *pid)
{
  int P=0,I=0,D=0;                                                                //暂存P、I、D计算结果
  int crt_pulse;                                                                  //当前占空比
  
  pid->crt_error= pid->target_temp - pid->crt_temp;                               //计算当前误差
  
  P= pid->kp * (pid->crt_error - pid->last_error);                                //比例计算
  I= pid->ki *  pid->crt_error;                                                   //积分计算
  D= pid->kd * (pid->crt_error - 2 * pid->last_error + pid->prev_error);          //微分计算
  
  pid->pid_out= P + I + D;                                                        //增量计算
  
  pid->prev_error= pid->last_error;                                               //保存误差数据用作下一次计算
  pid->last_error= pid->crt_error;
  
  crt_pulse= PID_GetPulse(pid);
  PID_SetPulse(pid,crt_pulse+pid->pid_out);                                       //控制输出
}

/**************************************************************************************
  * name        : void PID_PinInit(TempCtrlTypeDef *pid)
  * Function    : PWM输出引脚初始化
  * Parameters  : TempCtrlTypeDef *pid-- PID控制器参数
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void PID_PinInit(TempCtrlTypeDef *pid)
{
  GPIO_InitTypeDef GPIO_InitStructure;                                        //定时器引脚配置初始化
   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;						                  //复用推挽输出
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 			                  //使能复用IO时钟
  
  if(pid->remap == 0)                                                         //没有使用复用引脚
  {
    if(pid->timer == TIM1)                                                    //使用定时器1
    {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);                   //使能GPIO时钟
      
      if(pid->ch == TIM_CH1)                                                  //通道1
      {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                             //PA8
      }
      else if(pid->ch == TIM_CH2)                                             //通道2
      {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                             //PA9
      }
      else if(pid->ch == TIM_CH3)                                             //通道3
      {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                            //PA10
      }
      else if(pid->ch == TIM_CH4)                                             //通道4
      {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                            //PA11
      }
      
      GPIO_Init(GPIOA, &GPIO_InitStructure);                                  //初始化PWM输出引脚
    }//end if(pid->timer == TIM1)
    else if(pid->timer == TIM2)                                               //使用定时器2
    {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);                   //使能GPIO时钟
      
      if(pid->ch == TIM_CH1)                                                  //通道1
      {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                             //PA0
      }
      else if(pid->ch == TIM_CH2)                                             //通道2
      {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                             //PA1
      }
      else if(pid->ch == TIM_CH3)                                             //通道3
      {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                             //PA2
      }
      else if(pid->ch == TIM_CH4)                                             //通道4
      {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                             //PA3	
      }
      
      GPIO_Init(GPIOA, &GPIO_InitStructure);                                  //初始化PWM输出引脚
    }//end if(pid->timer == TIM2)
    else if(pid->timer == TIM3)                                               //使用定时器3
    {
      if(pid->ch == TIM_CH1)                                                  //通道1
      {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);                 //使能GPIO时钟
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                             //PA6
        GPIO_Init(GPIOA, &GPIO_InitStructure);                                //初始化PWM输出引脚
      }
      else if(pid->ch == TIM_CH2)                                             //通道2
      {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);                 //使能GPIO时钟
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                             //PA7
        GPIO_Init(GPIOA, &GPIO_InitStructure);                                //初始化PWM输出引脚
      }
      else if(pid->ch == TIM_CH3)                                             //通道3
      {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);                 //使能GPIO时钟
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                             //PB0
        GPIO_Init(GPIOB, &GPIO_InitStructure);                                //初始化PWM输出引脚
      }
      else if(pid->ch == TIM_CH4)                                             //通道4
      {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);                 //使能GPIO时钟
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                             //PB1	
        GPIO_Init(GPIOB, &GPIO_InitStructure);                                //初始化PWM输出引脚
      }
    }//end if(pid->timer == TIM3)
    else if(pid->timer == TIM4)                                               //使用定时器3
    {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);                   //使能GPIO时钟
      
      if(pid->ch == TIM_CH1)                                                  //通道1
      {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                             //PB6
      }
      else if(pid->ch == TIM_CH2)                                             //通道2
      {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                             //PB7
      }
      else if(pid->ch == TIM_CH3)                                             //通道3
      {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                             //PB8
      }
      else if(pid->ch == TIM_CH4)                                             //通道4
      {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                             //PB9	
      }
      
      GPIO_Init(GPIOB, &GPIO_InitStructure);                                  //初始化PWM输出引脚

    }//end if(pid->timer == TIM4)
  }//end if(pid->remap == 0)
  else if(pid->remap == 1)                                                    //使用复用引脚
  {
//    GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2, ENABLE);//引脚重定向需要开启额外的时钟，此处仅举例
  }
}

/**************************************************************************************
  * name        : void PID_TimerInit(TempCtrlTypeDef *pid)
  * Function    : PWM输出定时器初始化
  * Parameters  : TempCtrlTypeDef *pid-- PID控制器参数
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void PID_TimerInit(TempCtrlTypeDef *pid)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 						                  //定时器配置初始化
	TIM_OCInitTypeDef TIM_OCInitStructure;										                  //定时器输出初始化

  if(pid->timer == TIM1)                                                      //定时器1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	
  else if(pid->timer == TIM2)                                                 //定时器2时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  
  else if(pid->timer == TIM3)                                                 //定时器3时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  else if(pid->timer == TIM4)                                                 //定时器4时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  
  TIM_TimeBaseStructure.TIM_Prescaler = PID_PRESCALER-1;											//预分频值
  TIM_TimeBaseStructure.TIM_Period = PID_PERIOD-1; 														//重装值，决定单脉冲周期
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;									//向上计数模式
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;                 							  //时钟分割
  TIM_TimeBaseStructure.TIM_RepetitionCounter=0;															//重复溢出多少次才中断
  TIM_TimeBaseInit(pid->timer, &TIM_TimeBaseStructure); 										  //初始化选定的定时器													
    
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;													  //PWM模式2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;							  //使能输出比较状态
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;									  //有效电平为低
  TIM_OCInitStructure.TIM_Pulse = 0;																			    //比较捕获1的预装载值，决定单脉冲占空比，初始化时设置占空比为0%
  
  if(pid->ch == TIM_CH1)                                                      //通道1
  {
    TIM_OC1PreloadConfig(pid->timer,TIM_OCPreload_Enable);									  //使能选定定时器的通道1预装载寄存器
    TIM_OC1Init(pid->timer,&TIM_OCInitStructure);     											  //初始化选定定时器的通道1
  }
  else if(pid->ch == TIM_CH2)                                                 //通道2
  {
    TIM_OC2PreloadConfig(pid->timer,TIM_OCPreload_Enable);									  //使能选定定时器的通道2预装载寄存器
    TIM_OC2Init(pid->timer,&TIM_OCInitStructure);     											  //初始化选定定时器的通道2
  }
  else if(pid->ch == TIM_CH3)                                                 //通道3
  {
    TIM_OC3PreloadConfig(pid->timer,TIM_OCPreload_Enable);									  //使能选定定时器的通道3预装载寄存器
    TIM_OC3Init(pid->timer,&TIM_OCInitStructure);     											  //初始化选定定时器的通道3
  }
  else if(pid->ch == TIM_CH4)                                                 //通道4
  {
    TIM_OC4PreloadConfig(TIM2,TIM_OCPreload_Enable);									        //使能选定定时器的通道4预装载寄存器
    TIM_OC4Init(pid->timer,&TIM_OCInitStructure);     											  //初始化选定定时器的通道4
  }  
  
  TIM_Cmd(pid->timer,ENABLE);                                                 //打开定时器
  
  if(pid->timer == TIM1 || pid->timer == TIM8)                                //高级定时器的额外操作
    TIM_CtrlPWMOutputs(pid->timer, ENABLE); 
}

/**************************************************************************************
  * name        : void PID_SetPulse(TempCtrlTypeDef *pid,int pulse)
  * Function    : 设置指定控制器的PWM占空比
  * Parameters  : TempCtrlTypeDef *pid-- PID控制器参数
                  int pulse-- 设置的占空比，n=0~1000,对应占空比为0.0%~100.0%
  * Returns     : Null
  * Note        : 本函数会强制限定pulse=0~1000,且要求定时器的预分频值为72，重装载值为1000
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void PID_SetPulse(TempCtrlTypeDef *pid,int pulse)
{
  pulse= pulse<0?0:pulse;                                                       //对n进行下限幅
  pulse= pulse>PID_PERIOD-1?PID_PERIOD-1:pulse;                                 //对n进行上限幅，PID_PERIOD=1000时才能保证pulse对应的占空比为n/1000;
  
  if(pid->ch == TIM_CH1)                                                        //指定定时器通道为通道1
    TIM_SetCompare1(pid->timer,pulse);                                          //设置指定定时器通道1捕获比较寄存器值
  if(pid->ch == TIM_CH2)                                                        //指定定时器通道为通道2
    TIM_SetCompare2(pid->timer,pulse);                                          //设置指定定时器通道2捕获比较寄存器值
  if(pid->ch == TIM_CH3)                                                        //指定定时器通道为通道3
    TIM_SetCompare3(pid->timer,pulse);                                          //设置指定定时器通道3捕获比较寄存器值
  if(pid->ch == TIM_CH4)                                                        //指定定时器通道为通道4
    TIM_SetCompare4(pid->timer,pulse);                                          //设置指定定时器通道4捕获比较寄存器值  
}

/**************************************************************************************
  * name        : int PID_GetPulse(TempCtrlTypeDef *pid)
  * Function    : 读取指定控制器的PWM占空比
  * Parameters  : TempCtrlTypeDef *pid-- PID控制器参数
  * Returns     : int pulse-- 读取的占空比
  * Note        : 本函数会强制限定pulse=0~1000,且要求定时器的预分频值为72，重装载值为1000
  * Author      : XYD
  * Check       : 
**************************************************************************************/
int PID_GetPulse(TempCtrlTypeDef *pid)
{ 
  int pulse;
  if(pid->ch == TIM_CH1)                                                        //指定定时器通道为通道1
    pulse= TIM_GetCapture1(pid->timer);                                         //获取占空比 
  else if(pid->ch == TIM_CH2)                                                   //指定定时器通道为通道2
    pulse= TIM_GetCapture2(pid->timer);                                         //获取占空比 
  else if(pid->ch == TIM_CH3)                                                   //指定定时器通道为通道3
    pulse= TIM_GetCapture3(pid->timer);                                         //获取占空比 
  else if(pid->ch == TIM_CH4)                                                   //指定定时器通道为通道4
    pulse= TIM_GetCapture4(pid->timer);                                         //获取占空比 
  
  return pulse;
}

                    //获取上一次输出占空比
/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/












