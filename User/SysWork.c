/*********************** (C) COPYRIGHT  2018 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : SysWork.c
  * Version     : V0.2
  * Description : Ѫ�������ذ幤������
  * Date        : 2018��5��2��
  * Author      : ZQChen
  * Check       :  
*******************************************************************************/



/*********************File include*********************/
#include "SysWork.h"                 


/******************Macro definition********************/
#define ADC_ENABLE    0     //0--ʧ��AD�壬1--ʹ��AD��
/*************External variable declaration************/
 

/***************Global variable definition*************/
int GetCupState = 0;
int Temp1 = 0;
int Temp2 = 0;

int Action_Flag = 0;


/**************************************************************************************
  * name        : void DetectProcess(void)
  * Function    : �������
  * Parameters  : NULL
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void DetectProcess(void)
{
  int i,j;
  int detect_cnt;
  int sample_cnt;
  int item_cnt;
  int all_item_finish,all_sample_finish;
  int action_flag=0;              //�Ƿ��ж���ִ�У�0-- û�ж���ִ�У�other-- �ж���ִ��
  static int home_cnt;
  
  home_cnt++;
  if(home_cnt >= 10 && Axis[0].position != 0)
  {
    AxisRun2Zero();
    home_cnt= 0;
  }
  
  for(i=1;i<=MaxRackNum;i++)
  {
    for(j=1 ;j <= SINGLE_RACK_NUM; j++)
      LCD_RefreshSampleSeat(i,j);                 //��ʾ���е�����״̬
  }

  //���ȼ�0���Ƿ�����Ŀ���ڷ�������Ҫ�鿴����ʱ���Ƿ����
  //���ȼ�0���Ƿ�����ĿR1��Ӧ���
  //���ȼ�0���Ƿ�����ĿR2��Ӧ���
  
  //���ȼ�1���Ƿ�����ĿR1��Ӧ�꣬��Ҫ��R2
  for(sample_cnt= (MaxRackNum-2)*9;sample_cnt<MaxRackNum*9;sample_cnt++)                      //��ѯ��ǰ��18������
  {
    if(Sample[sample_cnt].state == 1)                                                         //��Ӧ��
    {
      for(item_cnt=0;Sample[sample_cnt].item_list[item_cnt].type != 0;item_cnt++)             //��ѯ�����б��е�������Ŀ
      {
        if(Sample[sample_cnt].item_list[item_cnt].item_state == ITEM_R1_TEST_FINISH)          //����ĿR1��Ӧ��
        {
          AddR2ToSample(sample_cnt,item_cnt);                                                 //�����Ŀ����R2
          
          Sample[sample_cnt].item_list[item_cnt].item_state = ITEM_ADD_R2_FINISH;             //����Ŀ�л���R2������״̬
          StartSoftTimer(&DetectDelay[Sample[sample_cnt].item_list[item_cnt].detect_num],60); //��ʼ����ʱ�������������
          
          action_flag++;
          home_cnt++;
        }
      }
    }
  }
  
  //���ȼ�2���Ƿ��з�Ӧ���������Ҫ��R1
  for(sample_cnt= (MaxRackNum-2)*9;sample_cnt<MaxRackNum*9;sample_cnt++)                      //��ѯ��ǰ��18������
  {
    if(Sample[sample_cnt].state == 1)                                                         //��Ӧ��
    {
      for(item_cnt=0;Sample[sample_cnt].item_list[item_cnt].type != 0;item_cnt++)             //��ѯ�����б��е�������Ŀ
      {
        if(Sample[sample_cnt].item_list[item_cnt].item_state == ITEM_INCUBATE_FINISH)         //����Ŀ�������
        {
          AddR1ToSample(sample_cnt,item_cnt);                                                 //�����Ŀ����R1
          
          Sample[sample_cnt].item_list[item_cnt].item_state = ITEM_ADD_R1_FINISH;             //����Ŀ�л���R1������״̬
          
//          CAN_StartTest(Sample[sample_cnt].item_list[item_cnt].type,Sample[sample_cnt].item_list[item_cnt].detect_num);//CANͨ��֪ͨ��ʼ���
          StartSoftTimer(&DetectDelay[Sample[sample_cnt].item_list[item_cnt].detect_num],200);//��ʼ����ʱ������������ԣ���PT���100s����
          
          action_flag++;
          home_cnt++;
        }
      }
    }
  }
  

  
  //���ȼ�3���Ƿ��пյļ��λ����Ҫ������
  for(detect_cnt=0;detect_cnt<DETECT_SEAT_NUM; detect_cnt++)                              //��ѯ�Ƿ��п��м��λ
  {
    if(DetectSeatState[detect_cnt] == 0)                                                  //�п��м��λ
    {
      for(sample_cnt= (MaxRackNum-2)*9;sample_cnt<MaxRackNum*9;sample_cnt++)              //��ѯ��ǰ��18������
      {
        if(Sample[sample_cnt].state == 1 || Sample[sample_cnt].state == 2)                //��Ӧ�л�ȴ���Ӧ
        {
          for(item_cnt=0;Sample[sample_cnt].item_list[item_cnt].type != 0;item_cnt++)     //��ѯ�����б��е�������Ŀ
          {
            if(Sample[sample_cnt].item_list[item_cnt].item_state == ITEM_READY_FINISH)    //����Ŀ׼�����
            {
              Sample[sample_cnt].state=1;                                                 //�����л�Ϊ��Ӧ״̬
              GetSample2Detect(sample_cnt,item_cnt,detect_cnt);                           //��������
              
              Sample[sample_cnt].item_list[item_cnt].item_state= ITEM_SAMPLE_FINISH;      //�л�����Ŀ״̬Ϊ�������
              Sample[sample_cnt].item_list[item_cnt].detect_num= detect_cnt;              //��¼����Ŀ��ʹ�õļ��λ
              DetectSeatState[detect_cnt]=1;                                              //��Ӧ���λ״̬��1
              StartSoftTimer(&DetectDelay[detect_cnt],TestMethod[Sample[sample_cnt].item_list[item_cnt].type].incubate_time); //����������ʱ
              
              JudgeDiscardCup();                                                          //������������һ�ζ����жϣ�������
                           
              action_flag++;
              
              return;                                                                     //��ʹ���ڶ����Ҫ���м����ģ�һ��ֻ����һ������
             }
          }
        }
      }
    }
  }    

  
  //���ȼ�4���Ƿ��з�Ӧ�����Ҫ����
  for(sample_cnt= (MaxRackNum-2)*9;sample_cnt<MaxRackNum*9;sample_cnt++)                  //��ѯ��ǰ��18������
  {
    if(Sample[sample_cnt].state == 1)                                                     //��Ӧ��
    {
      for(item_cnt=0;Sample[sample_cnt].item_list[item_cnt].type != 0;item_cnt++)         //��ѯ�����б��е�������Ŀ
      {
        if(Sample[sample_cnt].item_list[item_cnt].item_state == ITEM_TEST_FINISH)         //����Ŀ�������
        {
          DiscardCup(Sample[sample_cnt].item_list[item_cnt].detect_num);                  //����
          DetectSeatState[Sample[sample_cnt].item_list[item_cnt].detect_num]=0;           //��Ӧ���λ״̬��0
          Sample[sample_cnt].item_list[item_cnt].item_state= ITEM_DISCARD_FINISH;         //�л�Ϊ�������״̬ 
          
          action_flag++;
          
          return;                                                                         //ÿ��ֻ����һ�ζ���

        }
      }
    }
  }  
  
  //��ͨ�����ж��Ƿ�ȫ����Ӧ��
  all_sample_finish= 0;
  for(sample_cnt= (MaxRackNum-2)*9;sample_cnt<MaxRackNum*9;sample_cnt++)                      //��ѯ��ǰ��18������
  {
    if(Sample[sample_cnt].state == 1)                                                         //��Ӧ��
    {
      all_item_finish= 0;
      for(item_cnt=0;Sample[sample_cnt].item_list[item_cnt].type != 0;item_cnt++)             //��ѯ�����б��е�������Ŀ
      {
        if(Sample[sample_cnt].item_list[item_cnt].item_state == ITEM_DISCARD_FINISH)          //����Ŀ��Ӧ���
          all_item_finish++;
      }
      
      if(all_item_finish >= item_cnt)                                                         //������Ŀ���
        Sample[sample_cnt].state= 0;
      
      all_sample_finish++;
    }
  }
  if(all_sample_finish == 0)                                                                  //��������������
    RunState= 0;
  
  if(action_flag == 0 && Axis[0].position != 0)                                               //����ѭ��û�ж�����ִ�У���һ����
  {
    WashNeedle(); 
    AxisRun2Zero();
  }
}


/**************************************************************************************
  * name        : void TesttProcess(void)
  * Function    : ��������
  * Parameters  : NULL
  * Returns     : Null
  * Note        : 
  * Author      : ZQChen
  * Check       : 
**************************************************************************************/

void TesttProcess(void)
{
	int i,j;
  int detect_cnt;
  int sample_cnt;
	int item_cnt;
	int all_item_finish,all_sample_finish;
	
	Action_Flag=0;
  
  for(i=1;i<=MaxRackNum;i++)
  {
    for(j=1 ;j <= SINGLE_RACK_NUM; j++)
      LCD_RefreshSampleSeat(i,j);                 																									//��ʾ���е�����״̬
  }
	
	for(sample_cnt= (MaxRackNum-2)*9;sample_cnt<MaxRackNum*9;sample_cnt++)                      	//��ѯ��ǰ��18������
	{
		for(item_cnt=0;Sample[sample_cnt].item_list[item_cnt].type != 0;item_cnt++)     						//��ѯ�����б��е�������Ŀ
		{		
			switch(Sample[sample_cnt].item_list[item_cnt].type)
			{
				case PT_TEST:			PT_Test(sample_cnt,item_cnt)     ;break;										//ִ��PT����
				case APTT_TEST:   APTT_Test(sample_cnt,item_cnt)   ;break;										//ִ��APTT����
				case TT_TEST:   	TT_Test(sample_cnt,item_cnt)     ;break;										//ִ��TT����
				case FIB_TEST:   	FIB_Test(sample_cnt,item_cnt)    ;break;										//ִ��FIB����
				case AT3_TEST:		AT3_Test(sample_cnt,item_cnt)    ;break;										//ִ��AT3����
				case FDP_TEST:    PT_Test(sample_cnt,item_cnt)   	 ;break;										//ִ��FDPT����
				case DD_TEST:   	PT_Test(sample_cnt,item_cnt)     ;break;										//ִ��DD����
				default:break;
			}
		}
	}
	JudgeDiscardCup();  																																							//�����ж�
                                                      												
	//��ͨ�����ж��Ƿ�ȫ����Ӧ��
  all_sample_finish= 0;
  for(sample_cnt= (MaxRackNum-2)*9;sample_cnt<MaxRackNum*9;sample_cnt++)                      //��ѯ��ǰ��18������
  {
    if(Sample[sample_cnt].state == 1)                                                         //��Ӧ��
    {
      all_item_finish= 0;
      for(item_cnt=0;Sample[sample_cnt].item_list[item_cnt].type != 0;item_cnt++)             //��ѯ�����б��е�������Ŀ
      {
        if(Sample[sample_cnt].item_list[item_cnt].item_state == ITEM_DISCARD_FINISH)          //����Ŀ��Ӧ���
          all_item_finish++;
      }
      
      if(all_item_finish >= item_cnt)                                                         //������Ŀ���
        Sample[sample_cnt].state= SAMPLE_GRAY;  
			else
				Sample[sample_cnt].state=SAMPLE_YELLOW;	
      all_sample_finish++;
    }
  }
  if(all_sample_finish == 0)                                                                  //��������������
	{
		RunState= 0;
		EditSample[sample_cnt]= Sample[sample_cnt];
		LCD_RefreshEditSampleSeat();                                                							//ˢ������λ��ʾ
    LCD_RefreshEditTestList();                                                  							//ˢ����ѡ��Ŀ�б�
		WashNeedle(); 
    AxisRun2Zero();
	}	
	if(Action_Flag == 0 && Axis[0].position != 0)                                               //����ѭ��û�ж�����ִ�У���һ����
  {
    WashNeedle(); 
    AxisRun2Zero();
  }
}


/**************************************************************************************
  * name        : void PT_Test(void)
  * Function    : PT��������
  * Parameters  : int sample_num�����������   int item_num������Ŀ���
  * Returns     : Null
  * Note        : 
  * Author      : ZQChen
  * Check       : 
**************************************************************************************/
void PT_Test(int sample_num,int item_num)
{
	int detect_cnt;
	for(detect_cnt=0;detect_cnt<8; detect_cnt++)                             													//��ѯ625���ͨ���Ƿ��п��м��λ
  {
    if(DetectSeatState[detect_cnt] == 0)                                                  					//�п��м��λ
    {
			if(Sample[sample_num].item_list[item_num].item_state == ITEM_READY_FINISH)    //����Ŀ׼�����
			{
				Sample[sample_num].state=1;                                                 //�����л�Ϊ��Ӧ״̬
				GetSample2Detect(sample_num,item_num,detect_cnt);                           																					//����
				Sample[sample_num].item_list[item_num].item_state=ITEM_SAMPLE_FINISH;
				Sample[sample_num].item_list[item_num].detect_num= detect_cnt;             																				 		//��¼����Ŀ��ʹ�õļ��λ
				DetectSeatState[detect_cnt]=1;                                              																					//��Ӧ���λ״̬��1
				StartSoftTimer(&DetectDelay[detect_cnt],TestMethod[Sample[sample_num].item_list[item_num].type].incubate_time); 			//����������ʱ
				Action_Flag++;
			}
		}
	}
	if(Sample[sample_num].item_list[item_num].item_state==ITEM_INCUBATE_FINISH)                       										//��ʱʱ�䵽
  {
		AddR1ToSample(sample_num,item_num);                                                 																//�����Ŀ����R1
		CAN_StartTest(Sample[sample_num].item_list[item_num].type,Sample[sample_num].item_list[item_num].detect_num);				//CANͨ��֪ͨ��ʼ���
		Sample[sample_num].item_list[item_num].item_state=ITEM_ADD_R1_FINISH;
		Action_Flag++;
	}
}

/**************************************************************************************
  * name        : void APTT_Test(void)
  * Function    : APTT��������
  * Parameters  : int sample_num�����������   int item_num������Ŀ���
  * Returns     : Null
  * Note        : 
  * Author      : ZQChen
  * Check       : 
**************************************************************************************/
void APTT_Test(int sample_num,int item_num)
{
	int detect_cnt;
	for(detect_cnt=0;detect_cnt<8; detect_cnt++)                             													//��ѯ625���ͨ���Ƿ��п��м��λ
  {
    if(DetectSeatState[detect_cnt] == 0)                                                  					//�п��м��λ
    {
			if(Sample[sample_num].item_list[item_num].item_state == ITEM_READY_FINISH)    //����Ŀ׼�����
			{
				Sample[sample_num].state=1;                                                 //�����л�Ϊ��Ӧ״̬
				GetSample2Detect(sample_num,item_num,detect_cnt);                           																					//��������R1�Լ�
				Sample[sample_num].item_list[item_num].detect_num= detect_cnt;             																				 		//��¼����Ŀ��ʹ�õļ��λ
				DetectSeatState[detect_cnt]=1;                                              																					//��Ӧ���λ״̬��1
				StartSoftTimer(&DetectDelay[detect_cnt],TestMethod[Sample[sample_num].item_list[item_num].type].incubate_time); 			//����������ʱ
				Action_Flag++;
			}
		}
	}
	if(Sample[sample_num].item_list[item_num].item_state==ITEM_INCUBATE_FINISH)                       										//��ʱʱ�䵽
  {
		AddR2ToSample(sample_num,item_num);                                                 																//�����Ŀ����R2
		CAN_StartTest(Sample[sample_num].item_list[item_num].type,Sample[sample_num].item_list[item_num].detect_num);				//CANͨ��֪ͨ��ʼ���
		Sample[sample_num].item_list[item_num].item_state=ITEM_ADD_R2_FINISH;
		Action_Flag++;
	}
}

/**************************************************************************************
  * name        : void TT_Test(void)
  * Function    : TT��������
  * Parameters  : int sample_num�����������   int item_num������Ŀ���
  * Returns     : Null
  * Note        : 
  * Author      : ZQChen
  * Check       : 
**************************************************************************************/
void TT_Test(int sample_num,int item_num)
{
	int detect_cnt;
	for(detect_cnt=0;detect_cnt<8; detect_cnt++)                             													//��ѯ625���ͨ���Ƿ��п��м��λ
  {
    if(DetectSeatState[detect_cnt] == 0)                                                  					//�п��м��λ
    {
			if(Sample[sample_num].item_list[item_num].item_state == ITEM_READY_FINISH)    //����Ŀ׼�����
			{
				Sample[sample_num].state=1;                                                 //�����л�Ϊ��Ӧ״̬
				GetSample2Detect(sample_num,item_num,detect_cnt);                           																					//����
				Sample[sample_num].item_list[item_num].item_state=ITEM_SAMPLE_FINISH;
				Sample[sample_num].item_list[item_num].detect_num= detect_cnt;             																				 		//��¼����Ŀ��ʹ�õļ��λ
				DetectSeatState[detect_cnt]=1;                                              																					//��Ӧ���λ״̬��1
				StartSoftTimer(&DetectDelay[detect_cnt],TestMethod[Sample[sample_num].item_list[item_num].type].incubate_time); 			//����������ʱ
				Action_Flag++;
			}
		}
	}
	if(Sample[sample_num].item_list[item_num].item_state==ITEM_INCUBATE_FINISH)                       										//��ʱʱ�䵽
  {
		AddR1ToSample(sample_num,item_num);                                                 																//�����Ŀ����R1
		CAN_StartTest(Sample[sample_num].item_list[item_num].type,Sample[sample_num].item_list[item_num].detect_num);				//CANͨ��֪ͨ��ʼ���
		Sample[sample_num].item_list[item_num].item_state=ITEM_ADD_R1_FINISH;
		Action_Flag++;
	}                                          																
}

/**************************************************************************************
  * name        : void FIB_Test(void)
  * Function    : FIB��������
  * Parameters  : int sample_num�����������   int item_num������Ŀ���
  * Returns     : Null
  * Note        : 
  * Author      : ZQChen
  * Check       : 
**************************************************************************************/
void FIB_Test(int sample_num,int item_num)
{
	int detect_cnt;
	for(detect_cnt=0;detect_cnt<8; detect_cnt++)                             													//��ѯ625���ͨ���Ƿ��п��м��λ
  {
    if(DetectSeatState[detect_cnt] == 0)                                                  					//�п��м��λ
    {
			if(Sample[sample_num].item_list[item_num].item_state == ITEM_READY_FINISH)    //����Ŀ׼�����
			{
				Sample[sample_num].state=1;                                                 //�����л�Ϊ��Ӧ״̬
				GetSample2Detect(sample_num,item_num,detect_cnt);                           																					//����
				Sample[sample_num].item_list[item_num].item_state=ITEM_SAMPLE_FINISH;
				Sample[sample_num].item_list[item_num].detect_num= detect_cnt;             																				 		//��¼����Ŀ��ʹ�õļ��λ
				DetectSeatState[detect_cnt]=1;                                              																					//��Ӧ���λ״̬��1
				StartSoftTimer(&DetectDelay[detect_cnt],TestMethod[Sample[sample_num].item_list[item_num].type].incubate_time); 			//����������ʱ
				Action_Flag++;
			}
		}
	}
	if(Sample[sample_num].item_list[item_num].item_state==ITEM_INCUBATE_FINISH)                       										//��ʱʱ�䵽
  {
		AddR2ToSample(sample_num,item_num);                                                 																//�����Ŀ����R2
		CAN_StartTest(Sample[sample_num].item_list[item_num].type,Sample[sample_num].item_list[item_num].detect_num);				//CANͨ��֪ͨ��ʼ���
		Sample[sample_num].item_list[item_num].item_state=ITEM_ADD_R2_FINISH;
		Action_Flag++;
	}                                                     																			
}



/**************************************************************************************
  * name        : void AT3_Test(void)
  * Function    : AT3��������
  * Parameters  : int sample_num�����������   int item_num������Ŀ���
  * Returns     : Null 
  * Note        : 
  * Author      : ZQChen
  * Check       : 
**************************************************************************************/
void AT3_Test(int sample_num,int item_num)
{
	int detect_cnt;
	for(detect_cnt=8;detect_cnt<12; detect_cnt++)                             													//��ѯ405���ͨ���Ƿ��п��м��λ
  {
    if(DetectSeatState[detect_cnt] == 0)                                                  					//�п��м��λ
    {
			if(Sample[sample_num].item_list[item_num].item_state == ITEM_READY_FINISH)    //����Ŀ׼�����
			{
				Sample[sample_num].state=1;                                                 //�����л�Ϊ��Ӧ״̬
				GetSample2Detect(sample_num,item_num,detect_cnt);                           																					//����
				Sample[sample_num].item_list[item_num].item_state=ITEM_SAMPLE_FINISH;
				Sample[sample_num].item_list[item_num].detect_num= detect_cnt;             																				 		//��¼����Ŀ��ʹ�õļ��λ
				DetectSeatState[detect_cnt]=1;                                              																					//��Ӧ���λ״̬��1
				StartSoftTimer(&DetectDelay[detect_cnt],TestMethod[Sample[sample_num].item_list[item_num].type].incubate_time); 			//����������ʱ
				Action_Flag++;
			}
		}
	}
	if(Sample[sample_num].item_list[item_num].item_state==ITEM_INCUBATE_FINISH)                       										//��ʱʱ�䵽
  {
		AddR1ToSample(sample_num,item_num);                                                 																//�����Ŀ����R1
		CAN_StartTest(Sample[sample_num].item_list[item_num].type,Sample[sample_num].item_list[item_num].detect_num);				//CANͨ��֪ͨ��ʼ���
		Sample[sample_num].item_list[item_num].item_state=ITEM_ADD_R1_FINISH;
		Action_Flag++;
	}
}



/**************************************************************************************
  * name        : void FDP_Test(void)
  * Function    : FDP��������
  * Parameters  : int sample_num�����������   int item_num������Ŀ���
  * Returns     : Null 
  * Note        : 
  * Author      : ZQChen
  * Check       : 
**************************************************************************************/
void FDP_Test(int sample_num,int item_num)
{
	int detect_cnt;
	for(detect_cnt=0;detect_cnt<8; detect_cnt++)                             													//��ѯ625���ͨ���Ƿ��п��м��λ
  {
    if(DetectSeatState[detect_cnt] == 0)                                                  					//�п��м��λ
    {
			if(Sample[sample_num].item_list[item_num].item_state == ITEM_READY_FINISH)    //����Ŀ׼�����
			{
				Sample[sample_num].state=1;                                                 //�����л�Ϊ��Ӧ״̬
				GetSample2Detect(sample_num,item_num,detect_cnt);                           																					//����
				Sample[sample_num].item_list[item_num].item_state=ITEM_SAMPLE_FINISH;
				Sample[sample_num].item_list[item_num].detect_num= detect_cnt;             																				 		//��¼����Ŀ��ʹ�õļ��λ
				DetectSeatState[detect_cnt]=1;                                              																					//��Ӧ���λ״̬��1
				StartSoftTimer(&DetectDelay[detect_cnt],TestMethod[Sample[sample_num].item_list[item_num].type].incubate_time); 			//����������ʱ
				Action_Flag++;
			}
		}
	}
	if(Sample[sample_num].item_list[item_num].item_state==ITEM_INCUBATE_FINISH)                       										//��ʱʱ�䵽
  {
		AddR1ToSample(sample_num,item_num);                                                 																//�����Ŀ����R1
		CAN_StartTest(Sample[sample_num].item_list[item_num].type,Sample[sample_num].item_list[item_num].detect_num);				//CANͨ��֪ͨ��ʼ���
		Sample[sample_num].item_list[item_num].item_state=ITEM_ADD_R1_FINISH;
		Action_Flag++;
	}                                                     																					
}


/**************************************************************************************
  * name        : void DD_Test(void)
  * Function    : DD��������
  * Parameters  : int sample_num�����������   int item_num������Ŀ���
  * Returns     : Null 
  * Note        : 
  * Author      : ZQChen
  * Check       : 
**************************************************************************************/
void DD_Test(int sample_num,int item_num)
{
	int detect_cnt;
	for(detect_cnt=0;detect_cnt<8; detect_cnt++)                             													//��ѯ625���ͨ���Ƿ��п��м��λ
  {
    if(DetectSeatState[detect_cnt] == 0)                                                  					//�п��м��λ
    {
			if(Sample[sample_num].item_list[item_num].item_state == ITEM_READY_FINISH)    //����Ŀ׼�����
			{
				Sample[sample_num].state=1;                                                 //�����л�Ϊ��Ӧ״̬
				GetSample2Detect(sample_num,item_num,detect_cnt);                           																					//����
				Sample[sample_num].item_list[item_num].item_state=ITEM_SAMPLE_FINISH;
				Sample[sample_num].item_list[item_num].detect_num= detect_cnt;             																				 		//��¼����Ŀ��ʹ�õļ��λ
				DetectSeatState[detect_cnt]=1;                                              																					//��Ӧ���λ״̬��1
				StartSoftTimer(&DetectDelay[detect_cnt],TestMethod[Sample[sample_num].item_list[item_num].type].incubate_time); 			//����������ʱ
				Action_Flag++;
			}
		}
	}
	if(Sample[sample_num].item_list[item_num].item_state==ITEM_INCUBATE_FINISH)                       										//��ʱʱ�䵽
  {
		AddR1ToSample(sample_num,item_num);                                                 																//�����Ŀ����R1
		CAN_StartTest(Sample[sample_num].item_list[item_num].type,Sample[sample_num].item_list[item_num].detect_num);				//CANͨ��֪ͨ��ʼ���
		Sample[sample_num].item_list[item_num].item_state=ITEM_ADD_R1_FINISH;
		Action_Flag++;
	}
}

/**************************************************************************************
  * name        : void GetSample2Detect(int sample_cnt,int item_cnt,int detect_cnt)
  * Function    : ������λȡ���������λ
  * Parameters  : int sample_cnt-- ��������
                  int item_cnt-- ������Ŀ
                  int detect_cnt-- Ŀ����λ
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void GetSample2Detect(int sample_cnt,int item_cnt,int detect_cnt)
{
  int item_type;                                            //��Ŀ����
  int sample_vol;                                           //������
  int cup_cnt,i;
	int reagent_num= -1;                                      //�Լ�λ��
	int add_vol=10;																							//������Լ���
  
  item_type= Sample[sample_cnt].item_list[item_cnt].type;   //��ȡ��Ŀ����
  sample_vol= TestMethod[item_type].sample_vol;
  WashNeedle();                                             //��ϴ������
  
  //�˶���ָ��λ�ã�ȡָ��������
	if((item_type==APTT_TEST)||(item_type==FIB_TEST))
	{
		for(i=0;i<19 ;i++)                                        //����R1�����Լ������Լ�λ
		{
			if(Reagent[i].type == item_type)
			{
				reagent_num=i;
				break;
			}
		}
		GetReagent(reagent_num,TestMethod[item_type].r1_vol+add_vol);   //ȡָ���Լ���ʵ���Լ���Ŵ�1��ʼ�������������0��ʼ���� 
		add_vol=0;
	}
  GetSample(Sample[sample_cnt].rack_num,Sample[sample_cnt].seat_num-1,sample_vol+add_vol);//��Ϊseat_num��Ҫ�ϳ��������õģ���Ҫ1-9
	
	if(item_type == APTT_TEST)
	{
		sample_vol=TestMethod[item_type].sample_vol+TestMethod[item_type].r1_vol;
	} 
	
  //ȡһ���ձ�����
  for(cup_cnt=0;cup_cnt<90;cup_cnt++)
  {
    if(CupSeat[cup_cnt] == CUP_GREEN)                       //�ù���λ�б���
    {
      GetCupFromEmpty(cup_cnt);
      break;
    }
  }
  
  RunAndAddSample(sample_vol,detect_cnt);                              //�����б߼���
  WhileDelay_ms(1000);
  
  PutCup2Detect(detect_cnt);                                           //�����ӷ�����λ
}

/**************************************************************************************
  * name        : void RunAndAddSample(int vol,int detect_cnt)
  * Function    : �����б߼���
  * Parameters  : int vol-- ���
                  int detect_cnt-- Ŀ����λ
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void RunAndAddSample(int vol,int detect_cnt)
{
  float x,y,z;
  z= 31;                                                        //����ʱZ����
  
  AxisRun2Pos(Axis[0].position,Axis[1].position,z);
	Axis[3].finish=0;
	PlungerPush(vol,0,0);
    
  GetDetectPos(&x,&y,detect_cnt);  
  AxisRun2Pos(x,y,z);  
}
/**************************************************************************************
  * name        : void AddR1ToSample(int sample_cnt,int item_cnt)
  * Function    : ������������Լ�1
  * Parameters  : int sample_cnt-- ��������
                  int item_cnt-- ������Ŀ
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void AddR1ToSample(int sample_cnt,int item_cnt)
{
  int i;
  int item_type;                                            //ȡ��Ŀ����
  int reagent_num= -1;                                      //�Լ�λ��
  
  item_type= Sample[sample_cnt].item_list[item_cnt].type;   //��ȡ��Ŀ����
  
  WashNeedle();                                             //��ϴ������
  
  for(i=0;i<19 ;i++)                                        //����R1�����Լ������Լ�λ
  {
    if(Reagent[i].type == TestMethod[item_type].r1_type)
    {
      reagent_num=i;
      break;
    }
  }
  GetReagent(reagent_num,TestMethod[item_type].r1_vol+10);   //ȡָ���Լ���ʵ���Լ���Ŵ�1��ʼ�������������0��ʼ����  

  PutReagent2Detect(Sample[sample_cnt].item_list[item_cnt].detect_num,TestMethod[item_type].r1_vol);      //����λ�����Լ�
}

/**************************************************************************************
  * name        : void AddR2ToSample(int sample_cnt,int item_cnt)
  * Function    : ������������Լ�2
  * Parameters  : int sample_cnt-- ��������
                  int item_cnt-- ������Ŀ
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void AddR2ToSample(int sample_cnt,int item_cnt) 
{
  int i;
  int item_type;                                            //ȡ��Ŀ����
  int reagent_seat= -1;                                     //�Լ�λ��
  
  item_type= Sample[sample_cnt].item_list[item_cnt].type;   //��ȡ��Ŀ����
  
  WashNeedle();                                             //��ϴ������
  
  for(i=0;i<19 && reagent_seat != -1;i++)                   //����R2�����Լ������Լ�λ
  {
    if(Reagent[i].type == TestMethod[item_type].r2_type)
      reagent_seat=i;
  }
  GetReagent(reagent_seat,TestMethod[item_type].r2_vol+10);    //ȡָ���Լ���ʵ���Լ���Ŵ�1��ʼ�������������0��ʼ����  

  PutReagent2Detect(Sample[sample_cnt].item_list[item_cnt].detect_num,TestMethod[item_type].r2_vol);      //����λ�����Լ�
}

/**************************************************************************************
  * name        : void TestMethodInit(void)
  * Function    : ���Է�����ʼ��
  * Parameters  : NULL
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
 void TestMethodInit(void)
{
  TestMethod[PT_TEST].incubate_time = 180;               //����ʱ��180S
  TestMethod[PT_TEST].calcu_type    = 0;                //�㷨���ͣ��ݲ���
  TestMethod[PT_TEST].r1_type       = PT_REAGENT;       //R1��PT�Լ�
  TestMethod[PT_TEST].r1_vol        = 120;              //R1ȡ100ul
  TestMethod[PT_TEST].r2_type       = 0;                //û��ʹ��R2�Լ�
  TestMethod[PT_TEST].r2_vol        = 0;              
  TestMethod[PT_TEST].sample_vol    = 60;
  
  TestMethod[APTT_TEST].incubate_time= 180;             //����ʱ��180S
  TestMethod[APTT_TEST].calcu_type    = 0;              //�㷨���ͣ��ݲ���
  TestMethod[APTT_TEST].r1_type       = APTT_REAGENT;   //R1��APTT�Լ�
  TestMethod[APTT_TEST].r1_vol        = 60;             //R1ȡ10ul
  TestMethod[APTT_TEST].r2_type       = CaCl2_REAGENT;  //R2��Cacl2
  TestMethod[APTT_TEST].r2_vol        = 60;              
  TestMethod[APTT_TEST].sample_vol    = 60;
  
  TestMethod[TT_TEST].incubate_time= 180;               //����ʱ��180S
  TestMethod[TT_TEST].calcu_type    = 0;                //�㷨���ͣ��ݲ���
  TestMethod[TT_TEST].r1_type       = TT_REAGENT;       //R1��TT�Լ�
  TestMethod[TT_TEST].r1_vol        = 120;              //R1ȡ10ul
  TestMethod[TT_TEST].r2_type       = 0;                //û��ʹ��R2�Լ�
  TestMethod[TT_TEST].r2_vol        = 0;              
  TestMethod[TT_TEST].sample_vol    = 60;
  
  TestMethod[FIB_TEST].incubate_time= 180;              //����ʱ��180S
  TestMethod[FIB_TEST].calcu_type    = 0;               //�㷨���ͣ��ݲ���
  TestMethod[FIB_TEST].r1_type       = OVB_REAGENT;     //R1��OVB�Լ�
  TestMethod[FIB_TEST].r1_vol        = 108;             //R1ȡ108ul
  TestMethod[FIB_TEST].r2_type       = FIB_REAGENT;     //R2��FIB�Լ�
  TestMethod[FIB_TEST].r2_vol        = 60;              //R2ȡ128ul
  TestMethod[FIB_TEST].sample_vol    = 12;
	
	TestMethod[AT3_TEST].incubate_time= 180;              //����ʱ��180S
  TestMethod[AT3_TEST].calcu_type    = 0;               //�㷨���ͣ��ݲ���
  TestMethod[AT3_TEST].r1_type       = AT3_R1_REAGENT;  //R1��TT�Լ�
  TestMethod[AT3_TEST].r1_vol        = 140;             //R1ȡ10ul
  TestMethod[AT3_TEST].r2_type       = 0;               //û��ʹ��R2�Լ�
  TestMethod[AT3_TEST].r2_vol        = 0;              
  TestMethod[AT3_TEST].sample_vol    = 70;

}

/**************************************************************************************
  * name        : void SeatPosInit(void)
  * Function    : ����λ��Ϣ��ʼ��
  * Parameters  : NULL
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void SeatInfoInit(void)
{ 
  float x_offset,y_offset;
  
  x_offset= -0.0f;
  y_offset= -0.0f;
  
  DetectPosBase[0]= x_offset + 14;           													//���λ������1
  DetectPosBase[1]= y_offset + 13;
	
  
  DetectPosBase[2]= DetectPosBase[0] + DETECT_BLOCK_X;     						//���λ������2
  DetectPosBase[3]= DetectPosBase[1];
	
	DetectPosBase[4]= DetectPosBase[0] + DETECT_BLOCK_X*2;       				//���λ������3
  DetectPosBase[5]= DetectPosBase[1];
  
  DetectPosBase[6]= DetectPosBase[0] + DETECT_BLOCK_X*3;         			//���λ������4
  DetectPosBase[7]= DetectPosBase[1];
  
  DetectPosBase[8]= DetectPosBase[6] + DETECT_BLOCK45_X;         			//���λ������5
  DetectPosBase[9]= DetectPosBase[1];
	
  DetectPosBase[10]= DetectPosBase[8] + DETECT_BLOCK56_X;         		//���λ������6
  DetectPosBase[11]= DetectPosBase[1];
	
	SamplePosBase[0]= DetectPosBase[0]+DETECT_TO_SAMPLE_X ;         		//����λ������
  SamplePosBase[1]= DetectPosBase[1] + DETECT_TO_SAMPLE_Y;
  
  SamplePosBase[2]= SamplePosBase[0] + SAMPLE_SPACE_X;        				//����λ������
  SamplePosBase[3]= SamplePosBase[1] + SAMPLE_SPACE_Y;
  
  CupPosBase[0]= DetectPosBase[0] + DETECT_TO_CUP_X;          				//����λ������
  CupPosBase[1]= DetectPosBase[1] + DETECT_TO_CUP_Y;
  
  ReagentPosBase[0]= DetectPosBase[0] + DETECT_TO_REAGENT_X;        	//�Լ�λ������
  ReagentPosBase[1]= DetectPosBase[1] + DETECT_TO_REAGENT_Y;
	
	WashPosBase[0]=  DetectPosBase[0] + DETECT_TO_WASH_X;          			//��ϴλ������
  WashPosBase[1]=  DetectPosBase[1] + DETECT_TO_WASH_Y;
  
  DiscardPosBase[0]= x_offset + 325;      														//����λ����ַ
  DiscardPosBase[1]= y_offset + 13;
}
/**************************************************************************************
  * name        : void CAN_Decode(MNTC_CanProtocolTypeDef *buf)
  * Function    : CAN�������ݽ������
  * Parameters  : MNTC_CanProtocolTypeDef *buf-- CANЭ��ṹ��
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_Decode(MNTC_CanProtocolTypeDef *buf)
{

  int source_id,target_id;                                                        //Դ��ַ��Ŀ���ַ
  int sub_type,opr_type;                                                          //�����������ࡢ��������
  int device_addr;                                                                //�豸��ַdata0
	float position;
  uint16_t value1,value2;
	
  CAN_Receive(buf->com,CAN_FIFO0,&buf->rx_msg);                                   //��ȡ���һ֡CAN���յ�����
	
	if(buf->my_id == CAN_GetRxTargetId(*buf))																				//�жϱ�֡�����ǲ��Ƿ��͸�������
	{
		if(1 == CAN_IsAxisAbsolute(*buf,&position))																		//�Ǿ����˶�����
		{
			switch(CAN_GetRxSourceId(*buf))
			{
				case CAN_X_AXIS_ID:	Axis[0].finish=1;	break;
				case CAN_Y_AXIS_ID:	Axis[1].finish=1;	break;
				case CAN_Z_AXIS_ID:	Axis[2].finish=1;	break;
				default: break;
			}
		}
		else if(1 == CAN_IsPeristalticPumpRun(*buf, &value1))
		{
			if(CAN_PRP1_ID == CAN_GetRxSourceId(*buf))																		//�䶯��1������ɣ��ظ�����
			{
				Pump[0]= 1; 
			}
			else if(CAN_PRP2_ID == CAN_GetRxSourceId(*buf))																//�䶯��2������ɣ��ظ�����
			{
				Pump[1]= 1;
			}
		}
	}
  
//  source_id= buf->rx_msg.ExtId >> 21;                                             //��ȡ����Դ��ַ
//  target_id= (buf->rx_msg.ExtId >> 13) & 0XFF;                                    //��ȡ����Ŀ���ַ
//  sub_type = (buf->rx_msg.ExtId >> 5) & 0XFF;                                     //��ȡ���Ĳ�����������
//  opr_type = buf->rx_msg.ExtId & 0X1F;                                            //��ȡ��������
//  device_addr= buf->rx_msg.Data[0];                                               //��ȡ�豸��ַ
  

//  else if(source_id == CAN_PLUNGER_ID)                                            //��������������������
//  {
//    if(opr_type == CAN_STATE_TYPE &&
//       sub_type == CAN_STATE_CMD  &&
//       buf->rx_msg.Data[1]== CAN_STATE_CMD_FINISH)
//    {
//      Axis[3].finish=1; 
//    }
//  }
//  else if(source_id == CAN_TEMP_ID)                                               //���������¿ذ�
//  {
//      PID_Decode(buf);
//  }
//  else if(source_id == CAN_ADC_ID)                                                //��������AD��
//  {
//    ADC_Decode(buf);                                                              //���Ľ���
//  }
}

/**************************************************************************************
  * name        : void ADC_Decode(MNTC_CanProtocolTypeDef *buf)
  * Function    : ADC��CAN���ݽ����Ӻ���
  * Parameters  : MNTC_CanProtocolTypeDef *buf-- ͨ�����ݽṹ��
  * Returns     : Null
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void ADC_Decode(MNTC_CanProtocolTypeDef *buf)
{
//  int sample_cnt;
//  int item_cnt;
//  int sub_type,opr_type;                                                          //�����������ࡢ��������
//  int device_addr;                                                                //�豸��ַdata0
//  float value;
//  
//  sub_type = (buf->rx_msg.ExtId >> 5) & 0XFF;                                     //��ȡ���Ĳ�����������
//  opr_type = buf->rx_msg.ExtId & 0X1F;                                            //��ȡ��������
//  device_addr= buf->rx_msg.Data[0];                                               //��ȡ�豸��ַ
//  
//  if(opr_type == CAN_STATE_TYPE &&
//     sub_type == CAN_STATE_CMD)                                                   //����ִ��״̬
//  {
//    switch(buf->rx_msg.Data[1])
//    {
//      case CAN_STATE_CMD_FINISH:                                                  //����ִ�����
//        
//        break;
//      case CAN_STATE_CMD_ERR:                                                     //����
//        break;
//      default:break;
//    }
//  }
//  else if(opr_type == CAN_DATA_TYPE &&
//          sub_type == CAN_DATA_USER &&
//          buf->rx_msg.Data[1] == CAN_DATA_USER_RESULT)                            //������
//  {
//    value = ((float)((buf->rx_msg.Data[2]<<8) | buf->rx_msg.Data[3])) / 10.0f;
//    
//    for(sample_cnt= (MaxRackNum-2)*9;sample_cnt<MaxRackNum*9;sample_cnt++)                      //��ѯ��ǰ��18������
//    {
//      if(Sample[sample_cnt].state == 1)                                                         //��Ӧ��
//      {
//        for(item_cnt=0;Sample[sample_cnt].item_list[item_cnt].type != 0;item_cnt++)             //��ѯ�����б��е�������Ŀ
//        {
//          if(Sample[sample_cnt].item_list[item_cnt].detect_num == device_addr)                  //��ͨ���Ĳ������
//          {
//            Sample[sample_cnt].item_list[item_cnt].item_state = ITEM_TEST_FINISH;               //���ø���Ŀ�������
//						Sample[sample_cnt].item_list[item_cnt].type=0;
//            
//            switch(Sample[sample_cnt].item_list[item_cnt].type)
//            {
//              case PT_TEST: 
//                Sample[sample_cnt].result[PT_S_RESULT]= value;                                        //PTԭʼ���
//                break;
//              case APTT_TEST:
//                Sample[sample_cnt].result[APTT_S_RESULT]= value;                                      //APTTԭʼ���
//                break;
//              case TT_TEST:
//                Sample[sample_cnt].result[TT_S_RESULT]= value;                                        //TTԭʼ���
//                break;
//              case FIB_TEST:
//                Sample[sample_cnt].result[FIB_S_RESULT]= value;                                       //FIBԭʼ���
////                Sample[sample_cnt].result[FIB_GL_RESULT]= FIB_CalcContent(&Calibration[0],value);     //FIB��g/L�ļ�����
//                break;
//              case AT3_TEST:
//                Sample[sample_cnt].result[AT3_OD_RESULT]= value;                                      //AT3ԭʼ���
////                Sample[sample_cnt].result[AT3_PERCENT_RESULT]= AT3_CalcActive(&Calibration[1],value); //AT3���Լ���
//                break;
//              case FDP_TEST:
//                Sample[sample_cnt].result[FDP_OD_RESULT]= value;                                      //FDPԭʼ���
////                Sample[sample_cnt].result[FDP_GL_RESULT]= FDP_CalcContent(&Calibration[2],value);     //FDP��������
//                break;
//              case DD_TEST:
//                Sample[sample_cnt].result[DD_OD_RESULT]= value;                                       //DDԭʼ���
////                Sample[sample_cnt].result[DD_GL_RESULT]= DD_CalcContent(&Calibration[3],value);       //DD��������
//                break;
//              default:break;
//            }//end switch
//            LCD_RefreshResultTabe(sample_cnt - FirstRowNum);                                          //ʵʱ��ʾ���
//            return ;                                                                                  //����
//          }
//        }//end ��ѯ�����б��е�������Ŀ
//      }
//    }//end for��ѯ��ǰ��18������

//  }
  
}

/**************************************************************************************
  * name        : void PID_Decode(MNTC_CanProtocolTypeDef *buf)
  * Function    : PID��CAN���ݽ����Ӻ���
  * Parameters  : MNTC_CanProtocolTypeDef *buf-- ͨ�����ݽṹ��
  * Returns     : Null
  * Author      : ZQChen
  * Check       : 
**************************************************************************************/
void PID_Decode(MNTC_CanProtocolTypeDef *buf)
{
  
}

/**************************************************************************************
  * name        : void LabviewDecod(void)
  * Function    : Labview����
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LabviewDecod(void)
{
  static int i=0;
  if(0 == LabviewFlag) 
  {
    LabviewBuf[i]= USART_ReceiveData(USART3);							 //��������
    if(LabviewBuf[0] == 'H')
    {
      if(LabviewBuf[i] == 'E')
      {
        i=0;
        LabviewFlag=1;
        return;
      }
      
      i++;
    }
    else 
      i=0;
  }
}

/**************************************************************************************
  * name        : void ScanerDecode(BarcodeTypeDef *buf)
  * Function    : ����ɨ�����������
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void ScanerDecode(BarcodeTypeDef *buf)
{
  int cnt;
  
  for(cnt=0; cnt<BARCODE_LEN; cnt++)                      //�����ж��ٸ��ַ�
  {
    if(buf->code_str[cnt] == '\0')
      break;
  }
  
  if(cnt == 6)                                            //��������Ϊ�Լ���Ϣ���Ҹ��Լ��Ĳ�Ʒ����ֻ��һ���ַ�
  {
    switch(buf->code_str[0])
    {
      case 0x31: EditReagent.type= CLEAN_REAGENT; break;  //��ϴҺ
      case 0x32: EditReagent.type= PT_REAGENT;    break;  //PT�Լ�
      case 0x33: EditReagent.type= APTT_REAGENT;  break;  //APTT�Լ�
      case 0x35: EditReagent.type= TT_REAGENT;    break;  //TT�Լ�
      case 0x36: EditReagent.type= FIB_REAGENT;   break;  //FIB�Լ�
      default:break;
    }
    memcpy(EditReagent.batch_code, buf->code_str,sizeof(char)*6);//������������
    EditReagent.date[0]= '2';                             //��������
    EditReagent.date[1]= '0';
    EditReagent.date[2]= buf->code_str[1];
    EditReagent.date[3]= buf->code_str[2];
    EditReagent.date[4]= '/';
    EditReagent.date[5]= buf->code_str[3];
    EditReagent.date[6]= buf->code_str[4];
    EditReagent.single_cnt= 50;
  }
  
  
  switch(EditReagent.type)                                        //��ʾ�Լ�����
  {
    case 0              : DW_WriteStr(&LCD,0x0310,"       ");break;
    case PT_REAGENT     : DW_WriteStr(&LCD,0x0310,"PT     ");break;
    case APTT_REAGENT   : DW_WriteStr(&LCD,0x0310,"APTT   ");break;
    case CaCl2_REAGENT  : DW_WriteStr(&LCD,0x0310,"CaCl2  ");break;
    case TT_REAGENT     : DW_WriteStr(&LCD,0x0310,"TT     ");break;
    case FIB_REAGENT    : DW_WriteStr(&LCD,0x0310,"FIB    ");break;
    case OVB_REAGENT    : DW_WriteStr(&LCD,0x0310,"OVB    ");break;
    case AT3_R1_REAGENT : DW_WriteStr(&LCD,0x0310,"AT3-R1 ");break;
    case AT3_R2_REAGENT : DW_WriteStr(&LCD,0x0310,"AT3-R2 ");break;
    case HEPARIN_REAGENT: DW_WriteStr(&LCD,0x0310,"����   ");break;
    case FDP_R1_REAGENT : DW_WriteStr(&LCD,0x0310,"FDP-R1 ");break;
    case FDP_R2_REAGENT : DW_WriteStr(&LCD,0x0310,"FDP-R2 ");break;
    case DD_R1_REAGENT  : DW_WriteStr(&LCD,0x0310,"DD-R1  ");break; 
    case DD_R2_REAGENT  : DW_WriteStr(&LCD,0x0310,"DD-R2  ");break;   
    default:break;
  }
  
  DW_WriteData(&LCD,0x0302,EditReagent.single_cnt);             //��ʾ��ƿ����
  DW_WriteData(&LCD,0x0303,EditReagent.batch_cnt);              //��ʾ��������
  DW_WriteStr(&LCD,0x0304,EditReagent.batch_code);              //��ʾ�Լ�����
  DW_WriteStr(&LCD,0x0320,EditReagent.date);                    //��ʾ�Լ���Ч��
}

/**************************************************************************************
  * name        : void AxisRun2Pos(float x,float y,float z)
  * Function    : �������һ�ζ�λ����λ��ɺ���˳�������
  * Parameters  : float x-- x����
                  float y-- y����
                  float z-- z����
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void AxisRun2Pos(float x,float y,float z)
{ 
	//XYZ�˶�
	if(x != Axis[0].position)
	{
		Axis[0].finish=0;
		CAN_AxisAbsolute(&CanCom,CAN_X_AXIS_ID,0X00,x);		//X���˶�
	}
	if(y != Axis[1].position)
	{
		Axis[1].finish=0;
		CAN_AxisAbsolute(&CanCom,CAN_Y_AXIS_ID,0X00,y);		//Y���˶�
	}
	if(z != Axis[2].position)
	{
		Axis[2].finish=0;
		CAN_AxisAbsolute(&CanCom,CAN_Z_AXIS_ID,0X00,z);		//Z���˶�
	}
	
	//�ȴ��˶����
	if(x != Axis[0].position)
	{
		while(Axis[0].finish == 0);
		Axis[0].position=x;
	}
	if(y != Axis[1].position)
	{
		while(Axis[1].finish == 0);
		Axis[1].position=y;
	}
	if(z != Axis[2].position)
	{
		while(Axis[2].finish == 0);
		Axis[2].position=z;
	}
}

/**************************************************************************************
  * name        : void AxisRun2Zero(void)
  * Function    : �������
  * Parameters  : 
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void AxisRun2Zero(void)
{ 		
  AxisRun2Pos(Axis[0].position,Axis[1].position,0); //Z�ֻ����
  AxisRun2Pos(0,0,0);                               //xy�ֻ����
  
	Axis[0].finish=0;
  Axis[1].finish=0;
  Axis[2].finish=0;
	CAN_AxisHome(&CanCom,CAN_X_AXIS_ID,0x00);					//����X���������
	CAN_AxisHome(&CanCom,CAN_Y_AXIS_ID,0x00);					//����Y���������
  CAN_AxisHome(&CanCom,CAN_Z_AXIS_ID,0x00);					//����Z���������
  while(Axis[0].finish == 0);                       //�ȴ���ȷ�������
	while(Axis[1].finish == 0);
	while(Axis[2].finish == 0);
  
  Axis[0].position=0.0;                             //���ݽ��б��ؼ�¼
  Axis[1].position=0.0;
  Axis[2].position=0.0;
}


/**************************************************************************************
  * name        : void AxisSetSpeed(int x_speed,int y_speed,int z_speed)
  * Function    : ���������ٶȣ������ٶ�<=0ʱʹ��Ĭ���ٶ�
  * Parameters  : 
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void AxisSetSpeed(int x_speed,int y_speed,int z_speed)
{
	CAN_AxisSpeed(&CanCom,CAN_X_AXIS_ID,0X00,x_speed);
	CAN_AxisSpeed(&CanCom,CAN_Y_AXIS_ID,0X00,y_speed);
	CAN_AxisSpeed(&CanCom,CAN_Z_AXIS_ID,0X00,z_speed);

}

/**************************************************************************************
  * name        : void PlungerDraw(int vol,int run_time,int delay_time)
  * Function    : �����ó�ȡָ��
  * Parameters  : int vol-- ���ul
                  int run_time-- ����ʱ�䣬Ϊ0ʱʹ��Ĭ���ٶ�
                  int delay_time-- ������ɺ����ʱʱ�䣬s
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void PlungerDraw(int vol,int run_time,int delay_time)
{
//	Axis[3].finish=0;
//  CAN_PlungerDraw(&CanCom,CAN_PLUNGER_ID,0x00,vol,delay_time);
//	while(Axis[3].finish == 0);
}

/**************************************************************************************
  * name        : void PlungerPush(int vol,int run_time,int delay_time)
  * Function    : ������ע��ָ��
  * Parameters  : int vol-- ���ul
                  int run_time-- ����ʱ�䣬Ϊ0ʱʹ��Ĭ���ٶ�
                  int delay_time-- ������ɺ����ʱʱ�䣬s
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void PlungerPush(int vol,int run_time,int delay_time)
{
//	Axis[3].finish=0;
//  CAN_PlungerPush(&CanCom,CAN_PLUNGER_ID,0x00,vol,delay_time);
//	while(Axis[3].finish == 0);
}

/**************************************************************************************
  * name        : void Plunger2Zero(void)
  * Function    : �����û���
  * Parameters  : 
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void Plunger2Zero(void)
{
//	Axis[3].finish=0;
//  CAN_PlungerHome(&CanCom,CAN_PLUNGER_ID,0x00);
//	while(Axis[3].finish == 0);

}

/**************************************************************************************
  * name        : void PlungerWash(int vol,int cnt,int delay_time)
  * Function    : ������ע��ָ��
  * Parameters  : int vol-- ���ul
                  int cnt-- ��ϴ����
                  int delay_time-- ������ɺ����ʱʱ�䣬s
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void PlungerWash(int vol,int cnt,int delay_time)
{
//  CAN_PlungerWash(&CanTest,CAN_PLUNGER_ID,0,vol,cnt,delay_time);
//  
//  while(Axis[3].finish != 1);                     //�ȴ����
//  Axis[3].finish=0;
}

/**************************************************************************************
  * name        : void GetSample(int rack_num,int sample_num,int volume)
  * Function    : ���������ܺš�������ź�ȡ����������ȡ������
  * Parameters  : int rack_num-- �����ܺ�
                  int sample_num-- �������
                  int volume-- ȡ�����ul
  * Returns     : Null
  * Note        : Լ��1�������ܵĺ��������ܱ��Ϊ1��3��5��7...,2�������ܵĺ��������ܱ��Ϊ2��4��6...
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void GetSample(int rack_num,int sample_num,int volume)
{
  float x,y,z;
  
  GetSamplePos(&x,&y,rack_num,sample_num);            //����xyz
  z= 0;
  AxisRun2Pos(x,y,z);                                 //���е���λ��
  
  AxisRun2Pos(x,y,z+110);                              //��Һ��

  PlungerDraw(volume,0,0);                            //��ȡ����
	Delay_S(1);	

  AxisRun2Pos(x,y,z); 
//  
}

/**************************************************************************************
  * name        : void GetEmptyCup(int cup_num)
  * Function    : ��ָ������λȡһ������
  * Parameters  : int cup_num-- ����λ���
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void GetCupFromEmpty(int cup_num)
{
  float x,y,z;
  int i;
  GetCupPos(&x,&y,cup_num);
	
  z= 0;
  AxisRun2Pos(x,y-GET_CUP_Y_DIST,HAVE_CUP_Z_POS);    //��һ����׼��
  AxisRun2Pos(x,y-GET_CUP_Y_DIST,GET_CUP_CUP_Z_POS); //�ڶ������½�,��������λ���෴

  AxisRun2Pos(x,y,GET_CUP_CUP_Z_POS);                //������������,��������λ���෴
  AxisRun2Pos(x,y,HAVE_CUP_Z_POS);                   //���Ĳ�������  
	
  CupSeat[cup_num]=CUP_GRAY;                         //�����Ӧ����λΪ��
//	CAN_AxisStateRead(&CanTest,CAN_Z_AXIS_ID);				 //���б�״̬
	Delay_Ms(10);																			 //�ȴ�״̬����
//	while(GetCupState==0)
//	{	
//		CAN_AxisStateRead(&CanTest,CAN_Z_AXIS_ID);			 //���б�״̬
//		Delay_S(1);																			 //�ȴ�״̬����
//	}
	for(i=0;i<CUP_NUM;i++)                             //ˢ�����б�����ʾ
    LCD_RefreshCupSeat(i);
//	AxisRun2Pos(x,y,HAVE_CUP_Z_POS);    //���岽����֤�б��󲻻�ײ�����ڵı���  
}

/**************************************************************************************
  * name        : void PutEmptyCup(int cup_num)
  * Function    : ��ָ������λ��һ������
  * Parameters  : int cup_num-- ����λ���
  * Returns     : Null
  * Note        : 
  * Author      : ZQChen
  * Check       : 
**************************************************************************************/
void PutCupFromEmpty(int cup_num)
{
  float x,y,z;
  
  GetCupPos(&x,&y,cup_num);
  z= 0;
  
  AxisRun2Pos(x,y,HAVE_CUP_Z_POS);                 //��һ����׼��
  AxisRun2Pos(x,y,GET_CUP_CUP_Z_POS); //�ڶ������½�,��������λ���෴

  AxisRun2Pos(x,y-GET_CUP_Y_DIST,GET_CUP_CUP_Z_POS);                //���������˱�,��������λ���෴
  AxisRun2Pos(x,y-GET_CUP_Y_DIST,HAVE_CUP_Z_POS);                   //���Ĳ�������  
  
  CupSeat[cup_num]=1;                                //�����Ӧ����λΪ��
}

/**************************************************************************************
  * name        : void GetCupFromDetect(int detect_num)
  * Function    : ��ָ�����λȡһ������
  * Parameters  : int detect_num-- ���λ���
  * Returns     : Null
  * Note        : 
  * Author      : XYD  
  * Check       : 
**************************************************************************************/
void GetCupFromDetect(int detect_num)
{
  float x,y,z;
  GetDetectPos(&x,&y,detect_num);
  z= 0;
  
  AxisRun2Pos(x,y-GET_CUP_Y_DIST,z);                             //��һ����׼��
  AxisRun2Pos(x,y-GET_CUP_Y_DIST,GET_DETECT_CUP_Z_POS);          //�ڶ������½�,��������λ���෴

  AxisRun2Pos(x,y,GET_DETECT_CUP_Z_POS);                         //������������
  AxisRun2Pos(x,y,HAVE_CUP_Z_POS);                               //���Ĳ������� 
}

/**************************************************************************************
  * name        : void PutCup2Detect(int detect_num)
  * Function    : ��һ��������ָ�����λ
  * Parameters  : int detect_num-- ���λ���
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void PutCup2Detect(int detect_num)
{
  float x,y,z;
  
  GetDetectPos(&x,&y,detect_num);
  z= 0;
   
  AxisRun2Pos(x,y,HAVE_CUP_Z_POS);                                               //��һ����׼��
  
  AxisRun2Pos(x,y,GET_DETECT_CUP_Z_POS);                            //�ڶ������½�,��������λ���෴
  
  AxisRun2Pos(x,y-GET_CUP_Y_DIST,GET_DETECT_CUP_Z_POS);             //���������˱�,��������λ���෴
  AxisRun2Pos(x,y-GET_CUP_Y_DIST,0);                                //���Ĳ�������  
}
/**************************************************************************************
  * name        : void GetReagent(int reagent_num,int volume)
  * Function    : �����Լ�λ��ź�ȡ����������ȡ������
  * Parameters  : int reagent_num-- �Լ�λ���
                  int volume-- ȡ�����ul
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void GetReagent(int reagent_num,int volume)
{
  float x,y,z;
  
  GetReagnetPos(&x,&y,reagent_num);                   //����xyz
  z= 0;
  AxisRun2Pos(x,y,z);                                 //���е���λ��
  

  AxisRun2Pos(x,y,z+93);                              //��Һ�� 

  PlungerDraw(volume,0,0);                            //���Լ� 

  AxisRun2Pos(x,y,z);                                 //�ص���ȫλ��   
}

/**************************************************************************************
  * name        : void PutReagent2Detect(int detect_num,int volume)
  * Function    : ���Լ���ָ�����λ 
  * Parameters  : int reagent_num-- �Լ�λ���
                  int volume-- ȡ�����ul
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void PutReagent2Detect(int detect_num,int volume)
{
  float x,y;
	AxisRun2Zero(); 
  GetCupFromDetect(detect_num);                     //�Ӽ��λץ����
  
  GetDetectPos(&x,&y,detect_num);                   //��Һ��    
	
  AxisRun2Pos(x,y,30); 
  CAN_Mix(1);                                       //��ʼ����
  PlungerPush(volume,0,0);                          //����,����ʱ����10ul
//	AxisRun2Pos(x,y,31); 
	WhileDelay_ms(200);
	CAN_Mix(0);                                       //ֹͣ����
//  WhileDelay_ms(20
  PutCup2Detect(detect_num);                        //�ű���
}

/**************************************************************************************
  * name        : void WashNeedle(void)
  * Function    : ��ϴ������
  * Parameters  : NULL 
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void WashNeedle(void)
{
  float z;
	int i; 
  GetReagent(4,120);                                                            //ȡ��ϴҺ
  
  z= 0;
  AxisRun2Pos(WashPosBase[0],WashPosBase[1],z);                                 //���� 
  
  AxisRun2Pos(WashPosBase[0],WashPosBase[1],90);                                //�½�
	PlungerPush(120,0,0);
	Plunger2Zero();
//  
  PUMP_ON;   
  Delay_S(2);
	VALVE_ON;                                                                     //��ϴ�ڱ�  
  Delay_S(2);
  PUMP_OFF;
	VALVE_OFF;                                                                    //��ϴ���
  Delay_S(1);
//  
  

  AxisRun2Pos(WashPosBase[0],WashPosBase[1],z);                                 //�ص���ȫλ��
}

/**************************************************************************************
  * name        : void DiscardCup(int detect_num)
  * Function    : ����
  * Parameters  : NULL
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void DiscardCup(int detect_num)
{
  GetCupFromDetect(detect_num);                                                       //�Ӽ��λȡ��
  
  AxisRun2Pos(DiscardPosBase[0],DiscardPosBase[1],HAVE_CUP_Z_POS);                                 //׼��
  AxisRun2Pos(DiscardPosBase[0],DiscardPosBase[1],DISCARD_CUP_Z_POS);                 //�½�
  AxisRun2Pos(DiscardPosBase[0],DiscardPosBase[1]-GET_CUP_Y_DIST,DISCARD_CUP_Z_POS);  //���� 
  AxisRun2Pos(DiscardPosBase[0],DiscardPosBase[1]-GET_CUP_Y_DIST,0);                  //��λ
  
  //�ٵĶ�����������֤�������Զ�
//  AxisRun2Pos(300,10,10);                                 //׼��
//  AxisRun2Pos(300,10,0);                 //�½�
//  AxisRun2Pos(300,0,0);  //���� 
//  AxisRun2Pos(300,10,10);                  //��λ
//  
//  TestCnt++;
}

/**************************************************************************************
  * name        : void DiscardCup(int detect_num)
  * Function    : ����
  * Parameters  : NULL
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void DiscardCup1(int detect_num,int cup_num)
{
  GetCupFromDetect(detect_num);                                                       //�Ӽ��λȡ��
	PutCupFromEmpty(cup_num);
  
//  AxisRun2Pos(DiscardPosBase[0],DiscardPosBase[1],0);                                 //׼��
//  AxisRun2Pos(DiscardPosBase[0],DiscardPosBase[1],DISCARD_CUP_Z_POS);                 //�½�
//  AxisRun2Pos(DiscardPosBase[0],DiscardPosBase[1]-GET_CUP_Y_DIST,DISCARD_CUP_Z_POS);  //���� 
//  AxisRun2Pos(DiscardPosBase[0],DiscardPosBase[1]-GET_CUP_Y_DIST,0);                  //��λ
  
  //�ٵĶ�����������֤�������Զ�
//  AxisRun2Pos(300,10,10);                                 //׼��
//  AxisRun2Pos(300,10,0);                 //�½�
//  AxisRun2Pos(300,0,0);  //���� 
//  AxisRun2Pos(300,10,10);                  //��λ
//  
//  TestCnt++;
}


/**************************************************************************************
  * name        : void JudgeDiscardCup(void)
  * Function    : һ������Ķ��������������ڼ�������ж���
  * Parameters  : NULL
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void JudgeDiscardCup(void)
{
  int sample_cnt;
  int item_cnt;
  
  for(sample_cnt= (MaxRackNum-2)*9;sample_cnt<MaxRackNum*9;sample_cnt++)                  //��ѯ��ǰ��18������
  {
    if(Sample[sample_cnt].state == 1)                                                     //��Ӧ��
    {
      for(item_cnt=0;Sample[sample_cnt].item_list[item_cnt].type != 0;item_cnt++)         //��ѯ�����б��е�������Ŀ
      {
        if(Sample[sample_cnt].item_list[item_cnt].item_state == ITEM_TEST_FINISH)         //����Ŀ�������
        {
          DiscardCup(Sample[sample_cnt].item_list[item_cnt].detect_num);                  //����
          DetectSeatState[Sample[sample_cnt].item_list[item_cnt].detect_num]=0;           //��Ӧ���λ״̬��0
          Sample[sample_cnt].item_list[item_cnt].item_state= ITEM_DISCARD_FINISH;         //�л�Ϊ�������״̬ 
					Action_Flag++;
          
          return;                                                                         //ÿ��ֻ����һ�ζ���
        }
      }
    }
  }
}


/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------CANЭ�� �û��Զ�������------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/**************************************************************************************
  * name        : void CAN_Mix(int time)
  * Function    : ���Ȳ���
  * Parameters  : int state-- ����״̬
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_Mix(int state)
{
//	CAN_SetTxSourceId(&CanCom,CanCom.my_id);
//  CAN_SetTxTargetId(&CanCom,CAN_Z_AXIS_ID);
//	
//  CanCom.tx_msg.IDE  = CAN_ID_EXT;                         //ʹ����չģʽ
//  CanCom.tx_msg.RTR  = CAN_RTR_DATA;                       //����֡������Զ��֡
//  CanCom.tx_msg.DLC  = 3;                                  //���鳤��
//  
//  CanCom.tx_msg.Data[0]= CMD_USER;                         //�û��Զ�������
//  CanCom.tx_msg.Data[1]= CMD_USER_MIX;                 		 //���Ȳ��� 
//  CanCom.tx_msg.Data[2]= state ;                         	 //����ʱ��
////  CanTest.tx_msg.Data[3]= time & 0XFF;

//  CAN_Transmit(CanCom.com, &CanCom.tx_msg);
//  
////  while(Axis[2].finish != 1);                               //�ȴ��������
////  Axis[2].finish=0;
}

/**************************************************************************************
  * name        : void CAN_StartTest(int test_type,int detect_num)
  * Function    : ����һ�����
  * Parameters  : int test_type-- ��������
                  int detect_num-- ��ʹ�õļ����ͨ��
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_StartTest(int test_type,int detect_num)
{
//  CanCom.tx_msg.ExtId= ((u32)CanCom.my_id    <<21) |         //Դ��ַ
//                         ((u32)CAN_ADC_ID      <<13) |       //Ŀ����ַ
//                         ((u32)CMD_USER    <<5)  |        	 //�û��Զ���������
//                         (u32)CMD_TYPE;                   	 //��������
//  
//  CanCom.tx_msg.IDE  = CAN_ID_EXT;                           //ʹ����չģʽ
//  CanCom.tx_msg.RTR  = CAN_RTR_DATA;                         //����֡������Զ��֡
//  CanCom.tx_msg.DLC  = 8;                                    //���鳤��
//  
//  CanCom.tx_msg.Data[0]= detect_num;                         //ͨ����ַ
//  CanCom.tx_msg.Data[1]= CMD_USER_TESTSS;                		 //�����ͣ����
//  CanCom.tx_msg.Data[2]= 0X01;                               //�������
//  CanCom.tx_msg.Data[3]= test_type;                          //�������
//  
//  CAN_Transmit(CanCom.com, &CanCom.tx_msg);
}

/**************************************************************************************
  * name        : void CAN_SetTestMethod(int test_type,TestMethodInfoStruct *method)
  * Function    : ����һ����ԵĲ��Է���
  * Parameters  : int test_type-- ��������
                  TestMethodInfoStruct *method-- ���Է����洢�ṹ��
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_SetTestMethod(int test_type,TestMethodInfoStruct *method)
{
//  CanCom.tx_msg.ExtId= ((u32)CanTest.my_id    <<21) |            //Դ��ַ
//                         ((u32)CAN_ADC_ID      <<13) |            //Ŀ����ַ
//                         ((u32)CMD_USER     <<5) |            //�û��Զ���������
//                         (u32)CAN_CMD_TYPE;                       //��������
//  
//  CanCom.tx_msg.IDE  = CAN_ID_EXT;                               //ʹ����չģʽ
//  CanCom.tx_msg.RTR  = CAN_RTR_DATA;                             //����֡������Զ��֡
//  CanCom.tx_msg.DLC  = 8;                                        //���鳤��
//  
//  CanCom.tx_msg.Data[0]= 0;                                      //��ָ��ͨ��
//  CanCom.tx_msg.Data[1]= CAN_CMD_USER_PTSET+(test_type-PT_TEST); //��Ӧ��Ŀ�Ĳ�����������
//  CanCom.tx_msg.Data[2]= method->ch_type;                        //ͨ�����ͣ�ɢ��/͸��
//  CanCom.tx_msg.Data[3]= method->calcu_type;                     //�㷨���ͣ���ֵ/һ��/����/��ֵ��
//  CanCom.tx_msg.Data[4]= method->max_time;                       //�����ʱ��
//  
//  CAN_Transmit(CanCom.com, &CanCom.tx_msg);

}

/**************************************************************************************
  * name        : void CAN_Hello(int board_id)
  * Function    : ����ָ��
  * Parameters  : int board_id-- Ŀ���ID
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_Hello(int board_id)
{
//  CanCom.tx_msg.ExtId= ((u32)CanCom.my_id    <<21) |            //Դ��ַ
//                         ((u32)board_id        <<13) |            //Ŀ����ַ
//                         ((u32)CAN_CMD_USER     <<5) |            //�û��Զ���������
//                         (u32)CAN_CMD_TYPE;                       //��������
//  
//  CanCom.tx_msg.IDE  = CAN_ID_EXT;                               //ʹ����չģʽ
//  CanCom.tx_msg.RTR  = CAN_RTR_DATA;                             //����֡������Զ��֡
//  CanCom.tx_msg.DLC  = 8;                                        //���鳤��
//  
//  CanCom.tx_msg.Data[0]= 0;                                      //��ָ��ͨ��
//  CanCom.tx_msg.Data[1]= CAN_CMD_USER_HELLO;                     //����ָ��

//  CAN_Transmit(CanCom.com, &CanCom.tx_msg);
}

/**************************************************************************************
  * name        : void CAN_SelfTest(int board_id)
  * Function    : �Լ�ָ��
  * Parameters  : int board_id-- Ŀ���ID
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_SelfTest(int board_id)
{
//  CanCom.tx_msg.ExtId= ((u32)CanCom.my_id    <<21) |            //Դ��ַ
//                         ((u32)board_id        <<13) |            //Ŀ����ַ
//                         ((u32)CAN_CMD_USER     <<5) |            //�û��Զ���������
//                         (u32)CAN_CMD_TYPE;                       //��������
//  
//  CanCom.tx_msg.IDE  = CAN_ID_EXT;                               //ʹ����չģʽ
//  CanCom.tx_msg.RTR  = CAN_RTR_DATA;                             //����֡������Զ��֡
//  CanCom.tx_msg.DLC  = 8;                                        //���鳤��
//  
//  CanCom.tx_msg.Data[0]= 0;                                      //��ָ��ͨ��
//  CanCom.tx_msg.Data[1]= CAN_CMD_USER_SELF_TEST;                 //�Լ�

//  CAN_Transmit(CanCom.com, &CanCom.tx_msg);
}
/*--------------------------------------------------------------------------------------------------------------------*/



/**************************************************************************************
  * name        : void SampleInfoInit(void)
  * Function    : ��ʼ��������Ϣ����Ҫ������λ��Ϣ
  * Parameters  : 
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void SampleInfoInit(void)
{
  int rack_cnt,seat_cnt;
  int sample_cnt=0;
  
  for(rack_cnt=1; rack_cnt <= MaxRackNum; rack_cnt++)
  {
    for(seat_cnt=1; seat_cnt <= SINGLE_RACK_NUM; seat_cnt++)
    {
      Sample[sample_cnt].rack_num   = rack_cnt;
      Sample[sample_cnt].seat_num   = seat_cnt;
//      Sample[sample_cnt].sample_code= rack_cnt*100+seat_cnt;
      strncpy(Sample[sample_cnt].sample_code,"20171225",8);
      Sample[sample_cnt].sample_code[8]= '0' + rack_cnt/10;  //�����ܺ�ʮλ
      Sample[sample_cnt].sample_code[9]= '0' + rack_cnt%10;  //�����ܺŸ�λ
      Sample[sample_cnt].sample_code[10]= '0' + seat_cnt/10; //����λ��ʮλ
      Sample[sample_cnt].sample_code[11]= '0' + seat_cnt%10; //����λ�Ÿ�λ
      Sample[sample_cnt].state      = SAMPLE_GRAY;
      sample_cnt++;
    }
  }
}


/**************************************************************************************
  * name        : void GetSamplePos(float *x_pos,float *y_pos,int rack_num,int sample_num)
  * Function    : ���������ܺź�������ţ���ȡ������λ��ʵ������
  * Parameters  : float *x_pos-- ���ص�ʵ�������x
                  float *y_pos-- ���ص�ʵ�������y
                  int rack_num-- �����ܺ�
                  int sample_num-- �������
  * Returns     : Null
  * Note        : Լ��1�������ܵĺ��������ܱ��Ϊ1��3��5��7...,2�������ܵĺ��������ܱ��Ϊ2��4��6...
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void GetSamplePos(float *x_pos,float *y_pos,int rack_num,int sample_num)
{
  if(rack_num%2 == 1)                                   //1��������λ����
  {
    *x_pos= SamplePosBase[0] ;
  }
  else if(rack_num%2 == 0)                              //2��������λ����
  {
    *x_pos= SamplePosBase[2];
  }
  
  *y_pos= SamplePosBase[1] + sample_num*SAMPLE_SPACE_Y;
}

/**************************************************************************************
  * name        : void GetCupPos(float *x_pos,float *y_pos,int cup_num)
  * Function    : ���빩��λ��ţ���ȡ�ù���λ��ʵ������
  * Parameters  : float *x_pos-- ���ص�ʵ�������x
                  float *y_pos-- ���ص�ʵ�������y
                  int cup_num--  ����λ���
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void GetCupPos(float *x_pos,float *y_pos,int cup_num)
{
  *x_pos= CupPosBase[0] + (cup_num%CUP_NUM_X)*CUP_SPACE_X;   //���Ǽ������һ�б���
  *y_pos= CupPosBase[1] + (cup_num/CUP_NUM_X)*CUP_SPACE_Y;
  
  if(cup_num >= CUP_NUM_X*CUP_NUM_Y)                          //�ڶ���������
    *y_pos+= CUP_RACK_SPACE_Y - CUP_SPACE_Y;
}

/**************************************************************************************
  * name        : void GetReagnetPos(float *x_pos,float *y_pos,int reagent_num)
  * Function    : �����Լ�λ��ţ���ȡ���Լ�λ��ʵ������
  * Parameters  : float *x_pos-- ���ص�ʵ�������x
                  float *y_pos-- ���ص�ʵ�������y
                  int reagent_num--  �Լ�λ���
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void GetReagnetPos(float *x_pos,float *y_pos,int reagent_num)
{
  if(reagent_num < 3)                                       //����Լ�λ
  {
    *x_pos= ReagentPosBase[0] + reagent_num*REAGENT_SPACE1_X;
    *y_pos= ReagentPosBase[1];
  }
  else                                                      //С���Լ�λ
  {
    *x_pos= ReagentPosBase[0] + REAGENT_RACK_SPACE_X;       //��һ��С���Լ�λ����
    *y_pos= ReagentPosBase[1] + REAGENT_RACK_SPACE_Y;
    
    reagent_num -= 3;                                       //��ȥ����Լ�λ
    *x_pos += (reagent_num%REAGENT_NUM_X)*REAGENT_SPACE2_X;
    *y_pos += reagent_num/REAGENT_NUM_X * REAGENT_SPACE2_Y;   
  }
}

/**************************************************************************************
  * name        : void GetDetectPos(float *x_pos,float *y_pos,int detect_num)
  * Function    : ������λ��ţ���ȡ�ü��λ��ʵ������
  * Parameters  : float *x_pos-- ���ص�ʵ�������x
                  float *y_pos-- ���ص�ʵ�������y
                  int detect_num--  ���λ���
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void GetDetectPos(float *x_pos,float *y_pos,int detect_num)
{
  
  switch(detect_num)
  {
    case 0: 
    {
      *x_pos= DetectPosBase[0];
      *y_pos= DetectPosBase[1];
      break;
    }
    case 1: 
    {
      *x_pos= DetectPosBase[0]+DETECT_SPACE_X;
      *y_pos= DetectPosBase[1]+DETECT_SPACE_Y;
      break;
    }
    case 2: 
    {
      *x_pos= DetectPosBase[2];
      *y_pos= DetectPosBase[3];
      break;
    }
    case 3: 
    {
      *x_pos= DetectPosBase[2]+DETECT_SPACE_X;
      *y_pos= DetectPosBase[3]+DETECT_SPACE_Y;
      break;
    }
    case 4: 
    {
      *x_pos= DetectPosBase[4];
      *y_pos= DetectPosBase[5];
      break;
    }
    case 5: 
    {
      *x_pos= DetectPosBase[4]+DETECT_SPACE_X;
      *y_pos= DetectPosBase[5]+DETECT_SPACE_Y;
      break;
    }
    case 6: 
    {
      *x_pos= DetectPosBase[6];
      *y_pos= DetectPosBase[7];
      break;
    }
    case 7: 
    {
      *x_pos= DetectPosBase[6]+DETECT_SPACE_X;
      *y_pos= DetectPosBase[7]+DETECT_SPACE_Y;
      break;
    }
    case 8: 
    {
      *x_pos= DetectPosBase[8];
      *y_pos= DetectPosBase[9];
      break;
    }
    case 9: 
    {
      *x_pos= DetectPosBase[8]+DETECT_SPACE_X;
      *y_pos= DetectPosBase[9]+DETECT_SPACE_Y;
      break;
    }
		case 10: 
    {
      *x_pos= DetectPosBase[10];
      *y_pos= DetectPosBase[11];
      break;
    }
    case 11: 
    {
      *x_pos= DetectPosBase[10]+DETECT_SPACE_X;
      *y_pos= DetectPosBase[11]+DETECT_SPACE_Y;
      break;
    }
    default:break;
  }
  
}

/**************************************************************************************
  * name        : void WhileDelay_ms(int ms)
  * Function    : ʹ��whileѭ��ʵ�ֵĲ���ȷ��ms����ʱ�����ڲ��䶨ʱ����ȷ��ʱ��������ĳЩ���ϵ����
  * Parameters  : int ms-- ms
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void WhileDelay_ms(int ms)
{
  int us,i;
  
  while(ms-- )
  {
    us= 1000;
    while(us--)
    {
      i= 10;
      while(i--);
    }
  }
}

/*******************************************************************************
  * name        : void StartSoftTimer(SoftTimerStruct *timer��int t)
  * Function    : ���������ʱ��
  * Parameters  : SoftTimerStruct *timer--ָ�������ʱ��
  * Returns     : NULL
  * Author      : XYD
  * Check       : XYD
*******************************************************************************/
void StartSoftTimer(SoftTimerStruct *timer,int t) 
{
  timer->now=0;
	timer->set=t;
	timer->enable=1;
}

/*******************************************************************************
  * name        : void ClearSoftTimer(SoftTimerStruct *timer)
  * Function    : ���������ʱ��
  * Parameters  : SoftTimerStruct *timer--ָ�������ʱ��
  * Returns     : NULL
  * Author      : XYD
  * Check       : XYD
*******************************************************************************/
void ClearSoftTimer(SoftTimerStruct *timer)
{
  timer->now=0;
	timer->set=0;
	timer->enable=0;
}

/**************************************************************************************
  * name        : void LED_Init(void)
  * Function    : ��ʼ������LED
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LED_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  PUMP_OFF;
  VALVE_OFF;
}

/**************************************************************************************
  * name        : void GetCup_StateRead(void)
  * Function    : �б���״̬��ȡ
  * Parameters  : Null
  * Returns     : Null
  * Author      : ZQChen
  * Check       : 
**************************************************************************************/
void GetCup_StateRead(void)
{
	
}



/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/
