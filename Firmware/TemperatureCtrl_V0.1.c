/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : TemperatureCtrl_V0.1.c
  * Version     : V0.1
  * Description : �¶ȿ��Ƴ���
                  1��V0.1��չ��֧�ֶ�ͨ���¶ȿ���
  * Date        : 2017��9��13��
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
  * Function    : PID������Ӳ����ʼ��
  * Parameters  : TempCtrlTypeDef *pid-- PID����������
                  TIM_TypeDef *time-- �������PID������ʹ�õĶ�ʱ��
                  int ch-- �������PID�������Ķ�ʱ��ͨ��
                  int remap-- �Ƿ�ʹ���ض������� 0--��ʹ�� 1--ʹ��
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
  * Function    : PID������������ʼ��
  * Parameters  : TempCtrlTypeDef *pid-- PID����������
                  int target_temp-- Ŀ���¶�
                  float kp-- ����p
                  float ki-- ����i
                  float kd-- ����d
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
  * Function    : ����ʽPID�����㷨
  * Parameters  : TempCtrlTypeDef *pid-- PID����������
                  int crt_temp-- ��ǰ�¶�
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void PID_IncrementalCtrl(TempCtrlTypeDef *pid)
{
  int P=0,I=0,D=0;                                                                //�ݴ�P��I��D������
  int crt_pulse;                                                                  //��ǰռ�ձ�
  
  pid->crt_error= pid->target_temp - pid->crt_temp;                               //���㵱ǰ���
  
  P= pid->kp * (pid->crt_error - pid->last_error);                                //��������
  I= pid->ki *  pid->crt_error;                                                   //���ּ���
  D= pid->kd * (pid->crt_error - 2 * pid->last_error + pid->prev_error);          //΢�ּ���
  
  pid->pid_out= P + I + D;                                                        //��������
  
  pid->prev_error= pid->last_error;                                               //�����������������һ�μ���
  pid->last_error= pid->crt_error;
  
  crt_pulse= PID_GetPulse(pid);
  PID_SetPulse(pid,crt_pulse+pid->pid_out);                                       //�������
}

/**************************************************************************************
  * name        : void PID_PinInit(TempCtrlTypeDef *pid)
  * Function    : PWM������ų�ʼ��
  * Parameters  : TempCtrlTypeDef *pid-- PID����������
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void PID_PinInit(TempCtrlTypeDef *pid)
{
  GPIO_InitTypeDef GPIO_InitStructure;                                        //��ʱ���������ó�ʼ��
   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;						                  //�����������
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 			                  //ʹ�ܸ���IOʱ��
  
  if(pid->remap == 0)                                                         //û��ʹ�ø�������
  {
    if(pid->timer == TIM1)                                                    //ʹ�ö�ʱ��1
    {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);                   //ʹ��GPIOʱ��
      
      if(pid->ch == TIM_CH1)                                                  //ͨ��1
      {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                             //PA8
      }
      else if(pid->ch == TIM_CH2)                                             //ͨ��2
      {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                             //PA9
      }
      else if(pid->ch == TIM_CH3)                                             //ͨ��3
      {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                            //PA10
      }
      else if(pid->ch == TIM_CH4)                                             //ͨ��4
      {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                            //PA11
      }
      
      GPIO_Init(GPIOA, &GPIO_InitStructure);                                  //��ʼ��PWM�������
    }//end if(pid->timer == TIM1)
    else if(pid->timer == TIM2)                                               //ʹ�ö�ʱ��2
    {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);                   //ʹ��GPIOʱ��
      
      if(pid->ch == TIM_CH1)                                                  //ͨ��1
      {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                             //PA0
      }
      else if(pid->ch == TIM_CH2)                                             //ͨ��2
      {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                             //PA1
      }
      else if(pid->ch == TIM_CH3)                                             //ͨ��3
      {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                             //PA2
      }
      else if(pid->ch == TIM_CH4)                                             //ͨ��4
      {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                             //PA3	
      }
      
      GPIO_Init(GPIOA, &GPIO_InitStructure);                                  //��ʼ��PWM�������
    }//end if(pid->timer == TIM2)
    else if(pid->timer == TIM3)                                               //ʹ�ö�ʱ��3
    {
      if(pid->ch == TIM_CH1)                                                  //ͨ��1
      {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);                 //ʹ��GPIOʱ��
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                             //PA6
        GPIO_Init(GPIOA, &GPIO_InitStructure);                                //��ʼ��PWM�������
      }
      else if(pid->ch == TIM_CH2)                                             //ͨ��2
      {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);                 //ʹ��GPIOʱ��
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                             //PA7
        GPIO_Init(GPIOA, &GPIO_InitStructure);                                //��ʼ��PWM�������
      }
      else if(pid->ch == TIM_CH3)                                             //ͨ��3
      {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);                 //ʹ��GPIOʱ��
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                             //PB0
        GPIO_Init(GPIOB, &GPIO_InitStructure);                                //��ʼ��PWM�������
      }
      else if(pid->ch == TIM_CH4)                                             //ͨ��4
      {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);                 //ʹ��GPIOʱ��
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                             //PB1	
        GPIO_Init(GPIOB, &GPIO_InitStructure);                                //��ʼ��PWM�������
      }
    }//end if(pid->timer == TIM3)
    else if(pid->timer == TIM4)                                               //ʹ�ö�ʱ��3
    {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);                   //ʹ��GPIOʱ��
      
      if(pid->ch == TIM_CH1)                                                  //ͨ��1
      {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                             //PB6
      }
      else if(pid->ch == TIM_CH2)                                             //ͨ��2
      {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                             //PB7
      }
      else if(pid->ch == TIM_CH3)                                             //ͨ��3
      {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                             //PB8
      }
      else if(pid->ch == TIM_CH4)                                             //ͨ��4
      {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                             //PB9	
      }
      
      GPIO_Init(GPIOB, &GPIO_InitStructure);                                  //��ʼ��PWM�������

    }//end if(pid->timer == TIM4)
  }//end if(pid->remap == 0)
  else if(pid->remap == 1)                                                    //ʹ�ø�������
  {
//    GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2, ENABLE);//�����ض�����Ҫ���������ʱ�ӣ��˴�������
  }
}

/**************************************************************************************
  * name        : void PID_TimerInit(TempCtrlTypeDef *pid)
  * Function    : PWM�����ʱ����ʼ��
  * Parameters  : TempCtrlTypeDef *pid-- PID����������
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void PID_TimerInit(TempCtrlTypeDef *pid)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 						                  //��ʱ�����ó�ʼ��
	TIM_OCInitTypeDef TIM_OCInitStructure;										                  //��ʱ�������ʼ��

  if(pid->timer == TIM1)                                                      //��ʱ��1ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	
  else if(pid->timer == TIM2)                                                 //��ʱ��2ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  
  else if(pid->timer == TIM3)                                                 //��ʱ��3ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  else if(pid->timer == TIM4)                                                 //��ʱ��4ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  
  TIM_TimeBaseStructure.TIM_Prescaler = PID_PRESCALER-1;											//Ԥ��Ƶֵ
  TIM_TimeBaseStructure.TIM_Period = PID_PERIOD-1; 														//��װֵ����������������
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;									//���ϼ���ģʽ
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;                 							  //ʱ�ӷָ�
  TIM_TimeBaseStructure.TIM_RepetitionCounter=0;															//�ظ�������ٴβ��ж�
  TIM_TimeBaseInit(pid->timer, &TIM_TimeBaseStructure); 										  //��ʼ��ѡ���Ķ�ʱ��													
    
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;													  //PWMģʽ2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;							  //ʹ������Ƚ�״̬
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;									  //��Ч��ƽΪ��
  TIM_OCInitStructure.TIM_Pulse = 0;																			    //�Ƚϲ���1��Ԥװ��ֵ������������ռ�ձȣ���ʼ��ʱ����ռ�ձ�Ϊ0%
  
  if(pid->ch == TIM_CH1)                                                      //ͨ��1
  {
    TIM_OC1PreloadConfig(pid->timer,TIM_OCPreload_Enable);									  //ʹ��ѡ����ʱ����ͨ��1Ԥװ�ؼĴ���
    TIM_OC1Init(pid->timer,&TIM_OCInitStructure);     											  //��ʼ��ѡ����ʱ����ͨ��1
  }
  else if(pid->ch == TIM_CH2)                                                 //ͨ��2
  {
    TIM_OC2PreloadConfig(pid->timer,TIM_OCPreload_Enable);									  //ʹ��ѡ����ʱ����ͨ��2Ԥװ�ؼĴ���
    TIM_OC2Init(pid->timer,&TIM_OCInitStructure);     											  //��ʼ��ѡ����ʱ����ͨ��2
  }
  else if(pid->ch == TIM_CH3)                                                 //ͨ��3
  {
    TIM_OC3PreloadConfig(pid->timer,TIM_OCPreload_Enable);									  //ʹ��ѡ����ʱ����ͨ��3Ԥװ�ؼĴ���
    TIM_OC3Init(pid->timer,&TIM_OCInitStructure);     											  //��ʼ��ѡ����ʱ����ͨ��3
  }
  else if(pid->ch == TIM_CH4)                                                 //ͨ��4
  {
    TIM_OC4PreloadConfig(TIM2,TIM_OCPreload_Enable);									        //ʹ��ѡ����ʱ����ͨ��4Ԥװ�ؼĴ���
    TIM_OC4Init(pid->timer,&TIM_OCInitStructure);     											  //��ʼ��ѡ����ʱ����ͨ��4
  }  
  
  TIM_Cmd(pid->timer,ENABLE);                                                 //�򿪶�ʱ��
  
  if(pid->timer == TIM1 || pid->timer == TIM8)                                //�߼���ʱ���Ķ������
    TIM_CtrlPWMOutputs(pid->timer, ENABLE); 
}

/**************************************************************************************
  * name        : void PID_SetPulse(TempCtrlTypeDef *pid,int pulse)
  * Function    : ����ָ����������PWMռ�ձ�
  * Parameters  : TempCtrlTypeDef *pid-- PID����������
                  int pulse-- ���õ�ռ�ձȣ�n=0~1000,��Ӧռ�ձ�Ϊ0.0%~100.0%
  * Returns     : Null
  * Note        : ��������ǿ���޶�pulse=0~1000,��Ҫ��ʱ����Ԥ��ƵֵΪ72����װ��ֵΪ1000
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void PID_SetPulse(TempCtrlTypeDef *pid,int pulse)
{
  pulse= pulse<0?0:pulse;                                                       //��n�������޷�
  pulse= pulse>PID_PERIOD-1?PID_PERIOD-1:pulse;                                 //��n�������޷���PID_PERIOD=1000ʱ���ܱ�֤pulse��Ӧ��ռ�ձ�Ϊn/1000;
  
  if(pid->ch == TIM_CH1)                                                        //ָ����ʱ��ͨ��Ϊͨ��1
    TIM_SetCompare1(pid->timer,pulse);                                          //����ָ����ʱ��ͨ��1����ȽϼĴ���ֵ
  if(pid->ch == TIM_CH2)                                                        //ָ����ʱ��ͨ��Ϊͨ��2
    TIM_SetCompare2(pid->timer,pulse);                                          //����ָ����ʱ��ͨ��2����ȽϼĴ���ֵ
  if(pid->ch == TIM_CH3)                                                        //ָ����ʱ��ͨ��Ϊͨ��3
    TIM_SetCompare3(pid->timer,pulse);                                          //����ָ����ʱ��ͨ��3����ȽϼĴ���ֵ
  if(pid->ch == TIM_CH4)                                                        //ָ����ʱ��ͨ��Ϊͨ��4
    TIM_SetCompare4(pid->timer,pulse);                                          //����ָ����ʱ��ͨ��4����ȽϼĴ���ֵ  
}

/**************************************************************************************
  * name        : int PID_GetPulse(TempCtrlTypeDef *pid)
  * Function    : ��ȡָ����������PWMռ�ձ�
  * Parameters  : TempCtrlTypeDef *pid-- PID����������
  * Returns     : int pulse-- ��ȡ��ռ�ձ�
  * Note        : ��������ǿ���޶�pulse=0~1000,��Ҫ��ʱ����Ԥ��ƵֵΪ72����װ��ֵΪ1000
  * Author      : XYD
  * Check       : 
**************************************************************************************/
int PID_GetPulse(TempCtrlTypeDef *pid)
{ 
  int pulse;
  if(pid->ch == TIM_CH1)                                                        //ָ����ʱ��ͨ��Ϊͨ��1
    pulse= TIM_GetCapture1(pid->timer);                                         //��ȡռ�ձ� 
  else if(pid->ch == TIM_CH2)                                                   //ָ����ʱ��ͨ��Ϊͨ��2
    pulse= TIM_GetCapture2(pid->timer);                                         //��ȡռ�ձ� 
  else if(pid->ch == TIM_CH3)                                                   //ָ����ʱ��ͨ��Ϊͨ��3
    pulse= TIM_GetCapture3(pid->timer);                                         //��ȡռ�ձ� 
  else if(pid->ch == TIM_CH4)                                                   //ָ����ʱ��ͨ��Ϊͨ��4
    pulse= TIM_GetCapture4(pid->timer);                                         //��ȡռ�ձ� 
  
  return pulse;
}

                    //��ȡ��һ�����ռ�ձ�
/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/












