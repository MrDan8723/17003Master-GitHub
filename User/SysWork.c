/*********************** (C) COPYRIGHT  2018 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : SysWork.c
  * Version     : V0.2
  * Description : 血凝仪主控板工作程序
  * Date        : 2018年5月2日
  * Author      : ZQChen
  * Check       :  
*******************************************************************************/



/*********************File include*********************/
#include "SysWork.h"                 


/******************Macro definition********************/
#define ADC_ENABLE    0     //0--失能AD板，1--使能AD板
/*************External variable declaration************/
 

/***************Global variable definition*************/
int GetCupState = 0;
int Temp1 = 0;
int Temp2 = 0;

int Action_Flag = 0;


/**************************************************************************************
  * name        : void DetectProcess(void)
  * Function    : 检测流程
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
  int action_flag=0;              //是否有动作执行，0-- 没有动作执行，other-- 有动作执行
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
      LCD_RefreshSampleSeat(i,j);                 //显示所有的样本状态
  }

  //优先级0，是否有项目正在孵育，需要查看孵育时间是否结束
  //优先级0，是否有项目R1反应完成
  //优先级0，是否有项目R2反应完成
  
  //优先级1，是否有项目R1反应完，需要加R2
  for(sample_cnt= (MaxRackNum-2)*9;sample_cnt<MaxRackNum*9;sample_cnt++)                      //轮询当前的18个样本
  {
    if(Sample[sample_cnt].state == 1)                                                         //反应中
    {
      for(item_cnt=0;Sample[sample_cnt].item_list[item_cnt].type != 0;item_cnt++)             //轮询测试列表中的所有项目
      {
        if(Sample[sample_cnt].item_list[item_cnt].item_state == ITEM_R1_TEST_FINISH)          //该项目R1反应完
        {
          AddR2ToSample(sample_cnt,item_cnt);                                                 //向该项目加入R2
          
          Sample[sample_cnt].item_list[item_cnt].item_state = ITEM_ADD_R2_FINISH;             //该项目切换至R2添加完成状态
          StartSoftTimer(&DetectDelay[Sample[sample_cnt].item_list[item_cnt].detect_num],60); //开始检测计时，这里仅作测试
          
          action_flag++;
          home_cnt++;
        }
      }
    }
  }
  
  //优先级2，是否有反应孵育完成需要加R1
  for(sample_cnt= (MaxRackNum-2)*9;sample_cnt<MaxRackNum*9;sample_cnt++)                      //轮询当前的18个样本
  {
    if(Sample[sample_cnt].state == 1)                                                         //反应中
    {
      for(item_cnt=0;Sample[sample_cnt].item_list[item_cnt].type != 0;item_cnt++)             //轮询测试列表中的所有项目
      {
        if(Sample[sample_cnt].item_list[item_cnt].item_state == ITEM_INCUBATE_FINISH)         //该项目孵育完成
        {
          AddR1ToSample(sample_cnt,item_cnt);                                                 //向该项目加入R1
          
          Sample[sample_cnt].item_list[item_cnt].item_state = ITEM_ADD_R1_FINISH;             //该项目切换至R1添加完成状态
          
//          CAN_StartTest(Sample[sample_cnt].item_list[item_cnt].type,Sample[sample_cnt].item_list[item_cnt].detect_num);//CAN通信通知开始检测
          StartSoftTimer(&DetectDelay[Sample[sample_cnt].item_list[item_cnt].detect_num],200);//开始检测计时，这里仅作测试，按PT检测100s计算
          
          action_flag++;
          home_cnt++;
        }
      }
    }
  }
  

  
  //优先级3，是否有空的检测位，需要加样本
  for(detect_cnt=0;detect_cnt<DETECT_SEAT_NUM; detect_cnt++)                              //查询是否有空闲检测位
  {
    if(DetectSeatState[detect_cnt] == 0)                                                  //有空闲检测位
    {
      for(sample_cnt= (MaxRackNum-2)*9;sample_cnt<MaxRackNum*9;sample_cnt++)              //轮询当前的18个样本
      {
        if(Sample[sample_cnt].state == 1 || Sample[sample_cnt].state == 2)                //反应中或等待反应
        {
          for(item_cnt=0;Sample[sample_cnt].item_list[item_cnt].type != 0;item_cnt++)     //轮询测试列表中的所有项目
          {
            if(Sample[sample_cnt].item_list[item_cnt].item_state == ITEM_READY_FINISH)    //该项目准备完成
            {
              Sample[sample_cnt].state=1;                                                 //样本切换为反应状态
              GetSample2Detect(sample_cnt,item_cnt,detect_cnt);                           //加样动作
              
              Sample[sample_cnt].item_list[item_cnt].item_state= ITEM_SAMPLE_FINISH;      //切换该项目状态为加样完成
              Sample[sample_cnt].item_list[item_cnt].detect_num= detect_cnt;              //记录该项目所使用的检测位
              DetectSeatState[detect_cnt]=1;                                              //相应检测位状态置1
              StartSoftTimer(&DetectDelay[detect_cnt],TestMethod[Sample[sample_cnt].item_list[item_cnt].type].incubate_time); //启动孵育计时
              
              JudgeDiscardCup();                                                          //加样后额外进行一次丢杯判断，并丢杯
                           
              action_flag++;
              
              return;                                                                     //即使存在多个需要进行加样的，一次只进行一个加样
             }
          }
        }
      }
    }
  }    

  
  //优先级4，是否有反应完成需要丢杯
  for(sample_cnt= (MaxRackNum-2)*9;sample_cnt<MaxRackNum*9;sample_cnt++)                  //轮询当前的18个样本
  {
    if(Sample[sample_cnt].state == 1)                                                     //反应中
    {
      for(item_cnt=0;Sample[sample_cnt].item_list[item_cnt].type != 0;item_cnt++)         //轮询测试列表中的所有项目
      {
        if(Sample[sample_cnt].item_list[item_cnt].item_state == ITEM_TEST_FINISH)         //该项目测试完成
        {
          DiscardCup(Sample[sample_cnt].item_list[item_cnt].detect_num);                  //丢杯
          DetectSeatState[Sample[sample_cnt].item_list[item_cnt].detect_num]=0;           //相应检测位状态置0
          Sample[sample_cnt].item_list[item_cnt].item_state= ITEM_DISCARD_FINISH;         //切换为丢杯完成状态 
          
          action_flag++;
          
          return;                                                                         //每次只进行一次丢杯

        }
      }
    }
  }  
  
  //普通任务，判断是否全都反应完
  all_sample_finish= 0;
  for(sample_cnt= (MaxRackNum-2)*9;sample_cnt<MaxRackNum*9;sample_cnt++)                      //轮询当前的18个样本
  {
    if(Sample[sample_cnt].state == 1)                                                         //反应中
    {
      all_item_finish= 0;
      for(item_cnt=0;Sample[sample_cnt].item_list[item_cnt].type != 0;item_cnt++)             //轮询测试列表中的所有项目
      {
        if(Sample[sample_cnt].item_list[item_cnt].item_state == ITEM_DISCARD_FINISH)          //该项目反应完成
          all_item_finish++;
      }
      
      if(all_item_finish >= item_cnt)                                                         //所有项目完成
        Sample[sample_cnt].state= 0;
      
      all_sample_finish++;
    }
  }
  if(all_sample_finish == 0)                                                                  //所有样本检测完成
    RunState= 0;
  
  if(action_flag == 0 && Axis[0].position != 0)                                               //本次循环没有动作被执行，回一次零
  {
    WashNeedle(); 
    AxisRun2Zero();
  }
}


/**************************************************************************************
  * name        : void TesttProcess(void)
  * Function    : 测试流程
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
      LCD_RefreshSampleSeat(i,j);                 																									//显示所有的样本状态
  }
	
	for(sample_cnt= (MaxRackNum-2)*9;sample_cnt<MaxRackNum*9;sample_cnt++)                      	//轮询当前的18个样本
	{
		for(item_cnt=0;Sample[sample_cnt].item_list[item_cnt].type != 0;item_cnt++)     						//轮询测试列表中的所有项目
		{		
			switch(Sample[sample_cnt].item_list[item_cnt].type)
			{
				case PT_TEST:			PT_Test(sample_cnt,item_cnt)     ;break;										//执行PT测试
				case APTT_TEST:   APTT_Test(sample_cnt,item_cnt)   ;break;										//执行APTT测试
				case TT_TEST:   	TT_Test(sample_cnt,item_cnt)     ;break;										//执行TT测试
				case FIB_TEST:   	FIB_Test(sample_cnt,item_cnt)    ;break;										//执行FIB测试
				case AT3_TEST:		AT3_Test(sample_cnt,item_cnt)    ;break;										//执行AT3测试
				case FDP_TEST:    PT_Test(sample_cnt,item_cnt)   	 ;break;										//执行FDPT测试
				case DD_TEST:   	PT_Test(sample_cnt,item_cnt)     ;break;										//执行DD测试
				default:break;
			}
		}
	}
	JudgeDiscardCup();  																																							//丢杯判断
                                                      												
	//普通任务，判断是否全都反应完
  all_sample_finish= 0;
  for(sample_cnt= (MaxRackNum-2)*9;sample_cnt<MaxRackNum*9;sample_cnt++)                      //轮询当前的18个样本
  {
    if(Sample[sample_cnt].state == 1)                                                         //反应中
    {
      all_item_finish= 0;
      for(item_cnt=0;Sample[sample_cnt].item_list[item_cnt].type != 0;item_cnt++)             //轮询测试列表中的所有项目
      {
        if(Sample[sample_cnt].item_list[item_cnt].item_state == ITEM_DISCARD_FINISH)          //该项目反应完成
          all_item_finish++;
      }
      
      if(all_item_finish >= item_cnt)                                                         //所有项目完成
        Sample[sample_cnt].state= SAMPLE_GRAY;  
			else
				Sample[sample_cnt].state=SAMPLE_YELLOW;	
      all_sample_finish++;
    }
  }
  if(all_sample_finish == 0)                                                                  //所有样本检测完成
	{
		RunState= 0;
		EditSample[sample_cnt]= Sample[sample_cnt];
		LCD_RefreshEditSampleSeat();                                                							//刷新样本位显示
    LCD_RefreshEditTestList();                                                  							//刷新已选项目列表
		WashNeedle(); 
    AxisRun2Zero();
	}	
	if(Action_Flag == 0 && Axis[0].position != 0)                                               //本次循环没有动作被执行，回一次零
  {
    WashNeedle(); 
    AxisRun2Zero();
  }
}


/**************************************************************************************
  * name        : void PT_Test(void)
  * Function    : PT测试流程
  * Parameters  : int sample_num——样本编号   int item_num——项目编号
  * Returns     : Null
  * Note        : 
  * Author      : ZQChen
  * Check       : 
**************************************************************************************/
void PT_Test(int sample_num,int item_num)
{
	int detect_cnt;
	for(detect_cnt=0;detect_cnt<8; detect_cnt++)                             													//查询625检测通道是否有空闲检测位
  {
    if(DetectSeatState[detect_cnt] == 0)                                                  					//有空闲检测位
    {
			if(Sample[sample_num].item_list[item_num].item_state == ITEM_READY_FINISH)    //该项目准备完成
			{
				Sample[sample_num].state=1;                                                 //样本切换为反应状态
				GetSample2Detect(sample_num,item_num,detect_cnt);                           																					//加样
				Sample[sample_num].item_list[item_num].item_state=ITEM_SAMPLE_FINISH;
				Sample[sample_num].item_list[item_num].detect_num= detect_cnt;             																				 		//记录该项目所使用的检测位
				DetectSeatState[detect_cnt]=1;                                              																					//相应检测位状态置1
				StartSoftTimer(&DetectDelay[detect_cnt],TestMethod[Sample[sample_num].item_list[item_num].type].incubate_time); 			//启动孵育计时
				Action_Flag++;
			}
		}
	}
	if(Sample[sample_num].item_list[item_num].item_state==ITEM_INCUBATE_FINISH)                       										//定时时间到
  {
		AddR1ToSample(sample_num,item_num);                                                 																//向该项目加入R1
		CAN_StartTest(Sample[sample_num].item_list[item_num].type,Sample[sample_num].item_list[item_num].detect_num);				//CAN通信通知开始检测
		Sample[sample_num].item_list[item_num].item_state=ITEM_ADD_R1_FINISH;
		Action_Flag++;
	}
}

/**************************************************************************************
  * name        : void APTT_Test(void)
  * Function    : APTT测试流程
  * Parameters  : int sample_num——样本编号   int item_num——项目编号
  * Returns     : Null
  * Note        : 
  * Author      : ZQChen
  * Check       : 
**************************************************************************************/
void APTT_Test(int sample_num,int item_num)
{
	int detect_cnt;
	for(detect_cnt=0;detect_cnt<8; detect_cnt++)                             													//查询625检测通道是否有空闲检测位
  {
    if(DetectSeatState[detect_cnt] == 0)                                                  					//有空闲检测位
    {
			if(Sample[sample_num].item_list[item_num].item_state == ITEM_READY_FINISH)    //该项目准备完成
			{
				Sample[sample_num].state=1;                                                 //样本切换为反应状态
				GetSample2Detect(sample_num,item_num,detect_cnt);                           																					//加样本和R1试剂
				Sample[sample_num].item_list[item_num].detect_num= detect_cnt;             																				 		//记录该项目所使用的检测位
				DetectSeatState[detect_cnt]=1;                                              																					//相应检测位状态置1
				StartSoftTimer(&DetectDelay[detect_cnt],TestMethod[Sample[sample_num].item_list[item_num].type].incubate_time); 			//启动孵育计时
				Action_Flag++;
			}
		}
	}
	if(Sample[sample_num].item_list[item_num].item_state==ITEM_INCUBATE_FINISH)                       										//定时时间到
  {
		AddR2ToSample(sample_num,item_num);                                                 																//向该项目加入R2
		CAN_StartTest(Sample[sample_num].item_list[item_num].type,Sample[sample_num].item_list[item_num].detect_num);				//CAN通信通知开始检测
		Sample[sample_num].item_list[item_num].item_state=ITEM_ADD_R2_FINISH;
		Action_Flag++;
	}
}

/**************************************************************************************
  * name        : void TT_Test(void)
  * Function    : TT测试流程
  * Parameters  : int sample_num——样本编号   int item_num——项目编号
  * Returns     : Null
  * Note        : 
  * Author      : ZQChen
  * Check       : 
**************************************************************************************/
void TT_Test(int sample_num,int item_num)
{
	int detect_cnt;
	for(detect_cnt=0;detect_cnt<8; detect_cnt++)                             													//查询625检测通道是否有空闲检测位
  {
    if(DetectSeatState[detect_cnt] == 0)                                                  					//有空闲检测位
    {
			if(Sample[sample_num].item_list[item_num].item_state == ITEM_READY_FINISH)    //该项目准备完成
			{
				Sample[sample_num].state=1;                                                 //样本切换为反应状态
				GetSample2Detect(sample_num,item_num,detect_cnt);                           																					//加样
				Sample[sample_num].item_list[item_num].item_state=ITEM_SAMPLE_FINISH;
				Sample[sample_num].item_list[item_num].detect_num= detect_cnt;             																				 		//记录该项目所使用的检测位
				DetectSeatState[detect_cnt]=1;                                              																					//相应检测位状态置1
				StartSoftTimer(&DetectDelay[detect_cnt],TestMethod[Sample[sample_num].item_list[item_num].type].incubate_time); 			//启动孵育计时
				Action_Flag++;
			}
		}
	}
	if(Sample[sample_num].item_list[item_num].item_state==ITEM_INCUBATE_FINISH)                       										//定时时间到
  {
		AddR1ToSample(sample_num,item_num);                                                 																//向该项目加入R1
		CAN_StartTest(Sample[sample_num].item_list[item_num].type,Sample[sample_num].item_list[item_num].detect_num);				//CAN通信通知开始检测
		Sample[sample_num].item_list[item_num].item_state=ITEM_ADD_R1_FINISH;
		Action_Flag++;
	}                                          																
}

/**************************************************************************************
  * name        : void FIB_Test(void)
  * Function    : FIB测试流程
  * Parameters  : int sample_num——样本编号   int item_num——项目编号
  * Returns     : Null
  * Note        : 
  * Author      : ZQChen
  * Check       : 
**************************************************************************************/
void FIB_Test(int sample_num,int item_num)
{
	int detect_cnt;
	for(detect_cnt=0;detect_cnt<8; detect_cnt++)                             													//查询625检测通道是否有空闲检测位
  {
    if(DetectSeatState[detect_cnt] == 0)                                                  					//有空闲检测位
    {
			if(Sample[sample_num].item_list[item_num].item_state == ITEM_READY_FINISH)    //该项目准备完成
			{
				Sample[sample_num].state=1;                                                 //样本切换为反应状态
				GetSample2Detect(sample_num,item_num,detect_cnt);                           																					//加样
				Sample[sample_num].item_list[item_num].item_state=ITEM_SAMPLE_FINISH;
				Sample[sample_num].item_list[item_num].detect_num= detect_cnt;             																				 		//记录该项目所使用的检测位
				DetectSeatState[detect_cnt]=1;                                              																					//相应检测位状态置1
				StartSoftTimer(&DetectDelay[detect_cnt],TestMethod[Sample[sample_num].item_list[item_num].type].incubate_time); 			//启动孵育计时
				Action_Flag++;
			}
		}
	}
	if(Sample[sample_num].item_list[item_num].item_state==ITEM_INCUBATE_FINISH)                       										//定时时间到
  {
		AddR2ToSample(sample_num,item_num);                                                 																//向该项目加入R2
		CAN_StartTest(Sample[sample_num].item_list[item_num].type,Sample[sample_num].item_list[item_num].detect_num);				//CAN通信通知开始检测
		Sample[sample_num].item_list[item_num].item_state=ITEM_ADD_R2_FINISH;
		Action_Flag++;
	}                                                     																			
}



/**************************************************************************************
  * name        : void AT3_Test(void)
  * Function    : AT3测试流程
  * Parameters  : int sample_num——样本编号   int item_num——项目编号
  * Returns     : Null 
  * Note        : 
  * Author      : ZQChen
  * Check       : 
**************************************************************************************/
void AT3_Test(int sample_num,int item_num)
{
	int detect_cnt;
	for(detect_cnt=8;detect_cnt<12; detect_cnt++)                             													//查询405检测通道是否有空闲检测位
  {
    if(DetectSeatState[detect_cnt] == 0)                                                  					//有空闲检测位
    {
			if(Sample[sample_num].item_list[item_num].item_state == ITEM_READY_FINISH)    //该项目准备完成
			{
				Sample[sample_num].state=1;                                                 //样本切换为反应状态
				GetSample2Detect(sample_num,item_num,detect_cnt);                           																					//加样
				Sample[sample_num].item_list[item_num].item_state=ITEM_SAMPLE_FINISH;
				Sample[sample_num].item_list[item_num].detect_num= detect_cnt;             																				 		//记录该项目所使用的检测位
				DetectSeatState[detect_cnt]=1;                                              																					//相应检测位状态置1
				StartSoftTimer(&DetectDelay[detect_cnt],TestMethod[Sample[sample_num].item_list[item_num].type].incubate_time); 			//启动孵育计时
				Action_Flag++;
			}
		}
	}
	if(Sample[sample_num].item_list[item_num].item_state==ITEM_INCUBATE_FINISH)                       										//定时时间到
  {
		AddR1ToSample(sample_num,item_num);                                                 																//向该项目加入R1
		CAN_StartTest(Sample[sample_num].item_list[item_num].type,Sample[sample_num].item_list[item_num].detect_num);				//CAN通信通知开始检测
		Sample[sample_num].item_list[item_num].item_state=ITEM_ADD_R1_FINISH;
		Action_Flag++;
	}
}



/**************************************************************************************
  * name        : void FDP_Test(void)
  * Function    : FDP测试流程
  * Parameters  : int sample_num——样本编号   int item_num——项目编号
  * Returns     : Null 
  * Note        : 
  * Author      : ZQChen
  * Check       : 
**************************************************************************************/
void FDP_Test(int sample_num,int item_num)
{
	int detect_cnt;
	for(detect_cnt=0;detect_cnt<8; detect_cnt++)                             													//查询625检测通道是否有空闲检测位
  {
    if(DetectSeatState[detect_cnt] == 0)                                                  					//有空闲检测位
    {
			if(Sample[sample_num].item_list[item_num].item_state == ITEM_READY_FINISH)    //该项目准备完成
			{
				Sample[sample_num].state=1;                                                 //样本切换为反应状态
				GetSample2Detect(sample_num,item_num,detect_cnt);                           																					//加样
				Sample[sample_num].item_list[item_num].item_state=ITEM_SAMPLE_FINISH;
				Sample[sample_num].item_list[item_num].detect_num= detect_cnt;             																				 		//记录该项目所使用的检测位
				DetectSeatState[detect_cnt]=1;                                              																					//相应检测位状态置1
				StartSoftTimer(&DetectDelay[detect_cnt],TestMethod[Sample[sample_num].item_list[item_num].type].incubate_time); 			//启动孵育计时
				Action_Flag++;
			}
		}
	}
	if(Sample[sample_num].item_list[item_num].item_state==ITEM_INCUBATE_FINISH)                       										//定时时间到
  {
		AddR1ToSample(sample_num,item_num);                                                 																//向该项目加入R1
		CAN_StartTest(Sample[sample_num].item_list[item_num].type,Sample[sample_num].item_list[item_num].detect_num);				//CAN通信通知开始检测
		Sample[sample_num].item_list[item_num].item_state=ITEM_ADD_R1_FINISH;
		Action_Flag++;
	}                                                     																					
}


/**************************************************************************************
  * name        : void DD_Test(void)
  * Function    : DD测试流程
  * Parameters  : int sample_num——样本编号   int item_num——项目编号
  * Returns     : Null 
  * Note        : 
  * Author      : ZQChen
  * Check       : 
**************************************************************************************/
void DD_Test(int sample_num,int item_num)
{
	int detect_cnt;
	for(detect_cnt=0;detect_cnt<8; detect_cnt++)                             													//查询625检测通道是否有空闲检测位
  {
    if(DetectSeatState[detect_cnt] == 0)                                                  					//有空闲检测位
    {
			if(Sample[sample_num].item_list[item_num].item_state == ITEM_READY_FINISH)    //该项目准备完成
			{
				Sample[sample_num].state=1;                                                 //样本切换为反应状态
				GetSample2Detect(sample_num,item_num,detect_cnt);                           																					//加样
				Sample[sample_num].item_list[item_num].item_state=ITEM_SAMPLE_FINISH;
				Sample[sample_num].item_list[item_num].detect_num= detect_cnt;             																				 		//记录该项目所使用的检测位
				DetectSeatState[detect_cnt]=1;                                              																					//相应检测位状态置1
				StartSoftTimer(&DetectDelay[detect_cnt],TestMethod[Sample[sample_num].item_list[item_num].type].incubate_time); 			//启动孵育计时
				Action_Flag++;
			}
		}
	}
	if(Sample[sample_num].item_list[item_num].item_state==ITEM_INCUBATE_FINISH)                       										//定时时间到
  {
		AddR1ToSample(sample_num,item_num);                                                 																//向该项目加入R1
		CAN_StartTest(Sample[sample_num].item_list[item_num].type,Sample[sample_num].item_list[item_num].detect_num);				//CAN通信通知开始检测
		Sample[sample_num].item_list[item_num].item_state=ITEM_ADD_R1_FINISH;
		Action_Flag++;
	}
}

/**************************************************************************************
  * name        : void GetSample2Detect(int sample_cnt,int item_cnt,int detect_cnt)
  * Function    : 从样本位取样本至检测位
  * Parameters  : int sample_cnt-- 对象样本
                  int item_cnt-- 对象项目
                  int detect_cnt-- 目标检测位
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void GetSample2Detect(int sample_cnt,int item_cnt,int detect_cnt)
{
  int item_type;                                            //项目类型
  int sample_vol;                                           //样本量
  int cup_cnt,i;
	int reagent_num= -1;                                      //试剂位号
	int add_vol=10;																							//补充的试剂量
  
  item_type= Sample[sample_cnt].item_list[item_cnt].type;   //获取项目类型
  sample_vol= TestMethod[item_type].sample_vol;
  WashNeedle();                                             //清洗进样针
  
  //运动到指定位置，取指定量样本
	if((item_type==APTT_TEST)||(item_type==FIB_TEST))
	{
		for(i=0;i<19 ;i++)                                        //查找R1所用试剂所在试剂位
		{
			if(Reagent[i].type == item_type)
			{
				reagent_num=i;
				break;
			}
		}
		GetReagent(reagent_num,TestMethod[item_type].r1_vol+add_vol);   //取指定试剂，实际试剂标号从1开始计数，而数组从0开始计数 
		add_vol=0;
	}
  GetSample(Sample[sample_cnt].rack_num,Sample[sample_cnt].seat_num-1,sample_vol+add_vol);//因为seat_num是要合成样本号用的，需要1-9
	
	if(item_type == APTT_TEST)
	{
		sample_vol=TestMethod[item_type].sample_vol+TestMethod[item_type].r1_vol;
	} 
	
  //取一个空杯起来
  for(cup_cnt=0;cup_cnt<90;cup_cnt++)
  {
    if(CupSeat[cup_cnt] == CUP_GREEN)                       //该供杯位有杯子
    {
      GetCupFromEmpty(cup_cnt);
      break;
    }
  }
  
  RunAndAddSample(sample_vol,detect_cnt);                              //边运行边加样
  WhileDelay_ms(1000);
  
  PutCup2Detect(detect_cnt);                                           //将杯子放入检测位
}

/**************************************************************************************
  * name        : void RunAndAddSample(int vol,int detect_cnt)
  * Function    : 边运行边加样
  * Parameters  : int vol-- 体积
                  int detect_cnt-- 目标检测位
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void RunAndAddSample(int vol,int detect_cnt)
{
  float x,y,z;
  z= 31;                                                        //加样时Z坐标
  
  AxisRun2Pos(Axis[0].position,Axis[1].position,z);
	Axis[3].finish=0;
	PlungerPush(vol,0,0);
    
  GetDetectPos(&x,&y,detect_cnt);  
  AxisRun2Pos(x,y,z);  
}
/**************************************************************************************
  * name        : void AddR1ToSample(int sample_cnt,int item_cnt)
  * Function    : 向样本中添加试剂1
  * Parameters  : int sample_cnt-- 对象样本
                  int item_cnt-- 对象项目
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void AddR1ToSample(int sample_cnt,int item_cnt)
{
  int i;
  int item_type;                                            //取项目类型
  int reagent_num= -1;                                      //试剂位号
  
  item_type= Sample[sample_cnt].item_list[item_cnt].type;   //获取项目类型
  
  WashNeedle();                                             //清洗进样针
  
  for(i=0;i<19 ;i++)                                        //查找R1所用试剂所在试剂位
  {
    if(Reagent[i].type == TestMethod[item_type].r1_type)
    {
      reagent_num=i;
      break;
    }
  }
  GetReagent(reagent_num,TestMethod[item_type].r1_vol+10);   //取指定试剂，实际试剂标号从1开始计数，而数组从0开始计数  

  PutReagent2Detect(Sample[sample_cnt].item_list[item_cnt].detect_num,TestMethod[item_type].r1_vol);      //向检测位加入试剂
}

/**************************************************************************************
  * name        : void AddR2ToSample(int sample_cnt,int item_cnt)
  * Function    : 向样本中添加试剂2
  * Parameters  : int sample_cnt-- 对象样本
                  int item_cnt-- 对象项目
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void AddR2ToSample(int sample_cnt,int item_cnt) 
{
  int i;
  int item_type;                                            //取项目类型
  int reagent_seat= -1;                                     //试剂位号
  
  item_type= Sample[sample_cnt].item_list[item_cnt].type;   //获取项目类型
  
  WashNeedle();                                             //清洗进样针
  
  for(i=0;i<19 && reagent_seat != -1;i++)                   //查找R2所用试剂所在试剂位
  {
    if(Reagent[i].type == TestMethod[item_type].r2_type)
      reagent_seat=i;
  }
  GetReagent(reagent_seat,TestMethod[item_type].r2_vol+10);    //取指定试剂，实际试剂标号从1开始计数，而数组从0开始计数  

  PutReagent2Detect(Sample[sample_cnt].item_list[item_cnt].detect_num,TestMethod[item_type].r2_vol);      //向检测位加入试剂
}

/**************************************************************************************
  * name        : void TestMethodInit(void)
  * Function    : 测试方法初始化
  * Parameters  : NULL
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
 void TestMethodInit(void)
{
  TestMethod[PT_TEST].incubate_time = 180;               //孵育时间180S
  TestMethod[PT_TEST].calcu_type    = 0;                //算法类型，暂不定
  TestMethod[PT_TEST].r1_type       = PT_REAGENT;       //R1是PT试剂
  TestMethod[PT_TEST].r1_vol        = 120;              //R1取100ul
  TestMethod[PT_TEST].r2_type       = 0;                //没有使用R2试剂
  TestMethod[PT_TEST].r2_vol        = 0;              
  TestMethod[PT_TEST].sample_vol    = 60;
  
  TestMethod[APTT_TEST].incubate_time= 180;             //孵育时间180S
  TestMethod[APTT_TEST].calcu_type    = 0;              //算法类型，暂不定
  TestMethod[APTT_TEST].r1_type       = APTT_REAGENT;   //R1是APTT试剂
  TestMethod[APTT_TEST].r1_vol        = 60;             //R1取10ul
  TestMethod[APTT_TEST].r2_type       = CaCl2_REAGENT;  //R2是Cacl2
  TestMethod[APTT_TEST].r2_vol        = 60;              
  TestMethod[APTT_TEST].sample_vol    = 60;
  
  TestMethod[TT_TEST].incubate_time= 180;               //孵育时间180S
  TestMethod[TT_TEST].calcu_type    = 0;                //算法类型，暂不定
  TestMethod[TT_TEST].r1_type       = TT_REAGENT;       //R1是TT试剂
  TestMethod[TT_TEST].r1_vol        = 120;              //R1取10ul
  TestMethod[TT_TEST].r2_type       = 0;                //没有使用R2试剂
  TestMethod[TT_TEST].r2_vol        = 0;              
  TestMethod[TT_TEST].sample_vol    = 60;
  
  TestMethod[FIB_TEST].incubate_time= 180;              //孵育时间180S
  TestMethod[FIB_TEST].calcu_type    = 0;               //算法类型，暂不定
  TestMethod[FIB_TEST].r1_type       = OVB_REAGENT;     //R1是OVB试剂
  TestMethod[FIB_TEST].r1_vol        = 108;             //R1取108ul
  TestMethod[FIB_TEST].r2_type       = FIB_REAGENT;     //R2是FIB试剂
  TestMethod[FIB_TEST].r2_vol        = 60;              //R2取128ul
  TestMethod[FIB_TEST].sample_vol    = 12;
	
	TestMethod[AT3_TEST].incubate_time= 180;              //孵育时间180S
  TestMethod[AT3_TEST].calcu_type    = 0;               //算法类型，暂不定
  TestMethod[AT3_TEST].r1_type       = AT3_R1_REAGENT;  //R1是TT试剂
  TestMethod[AT3_TEST].r1_vol        = 140;             //R1取10ul
  TestMethod[AT3_TEST].r2_type       = 0;               //没有使用R2试剂
  TestMethod[AT3_TEST].r2_vol        = 0;              
  TestMethod[AT3_TEST].sample_vol    = 70;

}

/**************************************************************************************
  * name        : void SeatPosInit(void)
  * Function    : 各种位信息初始化
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
  
  DetectPosBase[0]= x_offset + 14;           													//检测位基座标1
  DetectPosBase[1]= y_offset + 13;
	
  
  DetectPosBase[2]= DetectPosBase[0] + DETECT_BLOCK_X;     						//检测位基座标2
  DetectPosBase[3]= DetectPosBase[1];
	
	DetectPosBase[4]= DetectPosBase[0] + DETECT_BLOCK_X*2;       				//检测位基座标3
  DetectPosBase[5]= DetectPosBase[1];
  
  DetectPosBase[6]= DetectPosBase[0] + DETECT_BLOCK_X*3;         			//检测位基座标4
  DetectPosBase[7]= DetectPosBase[1];
  
  DetectPosBase[8]= DetectPosBase[6] + DETECT_BLOCK45_X;         			//检测位基座标5
  DetectPosBase[9]= DetectPosBase[1];
	
  DetectPosBase[10]= DetectPosBase[8] + DETECT_BLOCK56_X;         		//检测位基座标6
  DetectPosBase[11]= DetectPosBase[1];
	
	SamplePosBase[0]= DetectPosBase[0]+DETECT_TO_SAMPLE_X ;         		//样本位基座标
  SamplePosBase[1]= DetectPosBase[1] + DETECT_TO_SAMPLE_Y;
  
  SamplePosBase[2]= SamplePosBase[0] + SAMPLE_SPACE_X;        				//样本位基座标
  SamplePosBase[3]= SamplePosBase[1] + SAMPLE_SPACE_Y;
  
  CupPosBase[0]= DetectPosBase[0] + DETECT_TO_CUP_X;          				//供杯位基座标
  CupPosBase[1]= DetectPosBase[1] + DETECT_TO_CUP_Y;
  
  ReagentPosBase[0]= DetectPosBase[0] + DETECT_TO_REAGENT_X;        	//试剂位基座标
  ReagentPosBase[1]= DetectPosBase[1] + DETECT_TO_REAGENT_Y;
	
	WashPosBase[0]=  DetectPosBase[0] + DETECT_TO_WASH_X;          			//清洗位基座标
  WashPosBase[1]=  DetectPosBase[1] + DETECT_TO_WASH_Y;
  
  DiscardPosBase[0]= x_offset + 325;      														//丢杯位基地址
  DiscardPosBase[1]= y_offset + 13;
}
/**************************************************************************************
  * name        : void CAN_Decode(MNTC_CanProtocolTypeDef *buf)
  * Function    : CAN接收数据解码程序
  * Parameters  : MNTC_CanProtocolTypeDef *buf-- CAN协议结构体
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_Decode(MNTC_CanProtocolTypeDef *buf)
{

  int source_id,target_id;                                                        //源地址、目标地址
  int sub_type,opr_type;                                                          //操作属性子类、操作属性
  int device_addr;                                                                //设备地址data0
	float position;
  uint16_t value1,value2;
	
  CAN_Receive(buf->com,CAN_FIFO0,&buf->rx_msg);                                   //获取最近一帧CAN接收的数据
	
	if(buf->my_id == CAN_GetRxTargetId(*buf))																				//判断本帧命令是不是发送给本机的
	{
		if(1 == CAN_IsAxisAbsolute(*buf,&position))																		//是绝对运动命令
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
			if(CAN_PRP1_ID == CAN_GetRxSourceId(*buf))																		//蠕动泵1动作完成，回复命令
			{
				Pump[0]= 1; 
			}
			else if(CAN_PRP2_ID == CAN_GetRxSourceId(*buf))																//蠕动泵2动作完成，回复命令
			{
				Pump[1]= 1;
			}
		}
	}
  
//  source_id= buf->rx_msg.ExtId >> 21;                                             //获取报文源地址
//  target_id= (buf->rx_msg.ExtId >> 13) & 0XFF;                                    //获取报文目标地址
//  sub_type = (buf->rx_msg.ExtId >> 5) & 0XFF;                                     //获取报文操作属性子类
//  opr_type = buf->rx_msg.ExtId & 0X1F;                                            //获取操作属性
//  device_addr= buf->rx_msg.Data[0];                                               //获取设备地址
  

//  else if(source_id == CAN_PLUNGER_ID)                                            //报文来自柱塞泵驱动板
//  {
//    if(opr_type == CAN_STATE_TYPE &&
//       sub_type == CAN_STATE_CMD  &&
//       buf->rx_msg.Data[1]== CAN_STATE_CMD_FINISH)
//    {
//      Axis[3].finish=1; 
//    }
//  }
//  else if(source_id == CAN_TEMP_ID)                                               //报文来自温控板
//  {
//      PID_Decode(buf);
//  }
//  else if(source_id == CAN_ADC_ID)                                                //报文来自AD板
//  {
//    ADC_Decode(buf);                                                              //报文解析
//  }
}

/**************************************************************************************
  * name        : void ADC_Decode(MNTC_CanProtocolTypeDef *buf)
  * Function    : ADC板CAN数据解码子函数
  * Parameters  : MNTC_CanProtocolTypeDef *buf-- 通信内容结构体
  * Returns     : Null
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void ADC_Decode(MNTC_CanProtocolTypeDef *buf)
{
//  int sample_cnt;
//  int item_cnt;
//  int sub_type,opr_type;                                                          //操作属性子类、操作属性
//  int device_addr;                                                                //设备地址data0
//  float value;
//  
//  sub_type = (buf->rx_msg.ExtId >> 5) & 0XFF;                                     //获取报文操作属性子类
//  opr_type = buf->rx_msg.ExtId & 0X1F;                                            //获取操作属性
//  device_addr= buf->rx_msg.Data[0];                                               //获取设备地址
//  
//  if(opr_type == CAN_STATE_TYPE &&
//     sub_type == CAN_STATE_CMD)                                                   //命令执行状态
//  {
//    switch(buf->rx_msg.Data[1])
//    {
//      case CAN_STATE_CMD_FINISH:                                                  //命令执行完成
//        
//        break;
//      case CAN_STATE_CMD_ERR:                                                     //错误
//        break;
//      default:break;
//    }
//  }
//  else if(opr_type == CAN_DATA_TYPE &&
//          sub_type == CAN_DATA_USER &&
//          buf->rx_msg.Data[1] == CAN_DATA_USER_RESULT)                            //检测完成
//  {
//    value = ((float)((buf->rx_msg.Data[2]<<8) | buf->rx_msg.Data[3])) / 10.0f;
//    
//    for(sample_cnt= (MaxRackNum-2)*9;sample_cnt<MaxRackNum*9;sample_cnt++)                      //轮询当前的18个样本
//    {
//      if(Sample[sample_cnt].state == 1)                                                         //反应中
//      {
//        for(item_cnt=0;Sample[sample_cnt].item_list[item_cnt].type != 0;item_cnt++)             //轮询测试列表中的所有项目
//        {
//          if(Sample[sample_cnt].item_list[item_cnt].detect_num == device_addr)                  //该通道的测试完成
//          {
//            Sample[sample_cnt].item_list[item_cnt].item_state = ITEM_TEST_FINISH;               //设置该项目测试完成
//						Sample[sample_cnt].item_list[item_cnt].type=0;
//            
//            switch(Sample[sample_cnt].item_list[item_cnt].type)
//            {
//              case PT_TEST: 
//                Sample[sample_cnt].result[PT_S_RESULT]= value;                                        //PT原始结果
//                break;
//              case APTT_TEST:
//                Sample[sample_cnt].result[APTT_S_RESULT]= value;                                      //APTT原始结果
//                break;
//              case TT_TEST:
//                Sample[sample_cnt].result[TT_S_RESULT]= value;                                        //TT原始结果
//                break;
//              case FIB_TEST:
//                Sample[sample_cnt].result[FIB_S_RESULT]= value;                                       //FIB原始结果
////                Sample[sample_cnt].result[FIB_GL_RESULT]= FIB_CalcContent(&Calibration[0],value);     //FIB的g/L的计算结果
//                break;
//              case AT3_TEST:
//                Sample[sample_cnt].result[AT3_OD_RESULT]= value;                                      //AT3原始结果
////                Sample[sample_cnt].result[AT3_PERCENT_RESULT]= AT3_CalcActive(&Calibration[1],value); //AT3活性计算
//                break;
//              case FDP_TEST:
//                Sample[sample_cnt].result[FDP_OD_RESULT]= value;                                      //FDP原始结果
////                Sample[sample_cnt].result[FDP_GL_RESULT]= FDP_CalcContent(&Calibration[2],value);     //FDP含量计算
//                break;
//              case DD_TEST:
//                Sample[sample_cnt].result[DD_OD_RESULT]= value;                                       //DD原始结果
////                Sample[sample_cnt].result[DD_GL_RESULT]= DD_CalcContent(&Calibration[3],value);       //DD含量计算
//                break;
//              default:break;
//            }//end switch
//            LCD_RefreshResultTabe(sample_cnt - FirstRowNum);                                          //实时显示结果
//            return ;                                                                                  //结束
//          }
//        }//end 轮询测试列表中的所有项目
//      }
//    }//end for轮询当前的18个样本

//  }
  
}

/**************************************************************************************
  * name        : void PID_Decode(MNTC_CanProtocolTypeDef *buf)
  * Function    : PID板CAN数据解码子函数
  * Parameters  : MNTC_CanProtocolTypeDef *buf-- 通信内容结构体
  * Returns     : Null
  * Author      : ZQChen
  * Check       : 
**************************************************************************************/
void PID_Decode(MNTC_CanProtocolTypeDef *buf)
{
  
}

/**************************************************************************************
  * name        : void LabviewDecod(void)
  * Function    : Labview调试
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
    LabviewBuf[i]= USART_ReceiveData(USART3);							 //接收数据
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
  * Function    : 条码扫描器解码程序
  * Parameters  : Null
  * Returns     : Null
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void ScanerDecode(BarcodeTypeDef *buf)
{
  int cnt;
  
  for(cnt=0; cnt<BARCODE_LEN; cnt++)                      //计算有多少个字符
  {
    if(buf->code_str[cnt] == '\0')
      break;
  }
  
  if(cnt == 6)                                            //条码内容为试剂信息，且该试剂的产品编码只有一个字符
  {
    switch(buf->code_str[0])
    {
      case 0x31: EditReagent.type= CLEAN_REAGENT; break;  //清洗液
      case 0x32: EditReagent.type= PT_REAGENT;    break;  //PT试剂
      case 0x33: EditReagent.type= APTT_REAGENT;  break;  //APTT试剂
      case 0x35: EditReagent.type= TT_REAGENT;    break;  //TT试剂
      case 0x36: EditReagent.type= FIB_REAGENT;   break;  //FIB试剂
      default:break;
    }
    memcpy(EditReagent.batch_code, buf->code_str,sizeof(char)*6);//复制条码内容
    EditReagent.date[0]= '2';                             //计算日期
    EditReagent.date[1]= '0';
    EditReagent.date[2]= buf->code_str[1];
    EditReagent.date[3]= buf->code_str[2];
    EditReagent.date[4]= '/';
    EditReagent.date[5]= buf->code_str[3];
    EditReagent.date[6]= buf->code_str[4];
    EditReagent.single_cnt= 50;
  }
  
  
  switch(EditReagent.type)                                        //显示试剂类型
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
    case HEPARIN_REAGENT: DW_WriteStr(&LCD,0x0310,"肝素   ");break;
    case FDP_R1_REAGENT : DW_WriteStr(&LCD,0x0310,"FDP-R1 ");break;
    case FDP_R2_REAGENT : DW_WriteStr(&LCD,0x0310,"FDP-R2 ");break;
    case DD_R1_REAGENT  : DW_WriteStr(&LCD,0x0310,"DD-R1  ");break; 
    case DD_R2_REAGENT  : DW_WriteStr(&LCD,0x0310,"DD-R2  ");break;   
    default:break;
  }
  
  DW_WriteData(&LCD,0x0302,EditReagent.single_cnt);             //显示单瓶余量
  DW_WriteData(&LCD,0x0303,EditReagent.batch_cnt);              //显示批次余量
  DW_WriteStr(&LCD,0x0304,EditReagent.batch_code);              //显示试剂批号
  DW_WriteStr(&LCD,0x0320,EditReagent.date);                    //显示试剂有效期
}

/**************************************************************************************
  * name        : void AxisRun2Pos(float x,float y,float z)
  * Function    : 三轴进行一次定位，定位完成后才退出本函数
  * Parameters  : float x-- x坐标
                  float y-- y坐标
                  float z-- z坐标
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void AxisRun2Pos(float x,float y,float z)
{ 
	//XYZ运动
	if(x != Axis[0].position)
	{
		Axis[0].finish=0;
		CAN_AxisAbsolute(&CanCom,CAN_X_AXIS_ID,0X00,x);		//X轴运动
	}
	if(y != Axis[1].position)
	{
		Axis[1].finish=0;
		CAN_AxisAbsolute(&CanCom,CAN_Y_AXIS_ID,0X00,y);		//Y轴运动
	}
	if(z != Axis[2].position)
	{
		Axis[2].finish=0;
		CAN_AxisAbsolute(&CanCom,CAN_Z_AXIS_ID,0X00,z);		//Z轴运动
	}
	
	//等待运动完成
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
  * Function    : 三轴回零
  * Parameters  : 
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void AxisRun2Zero(void)
{ 		
  AxisRun2Pos(Axis[0].position,Axis[1].position,0); //Z粗回零点
  AxisRun2Pos(0,0,0);                               //xy粗回零点
  
	Axis[0].finish=0;
  Axis[1].finish=0;
  Axis[2].finish=0;
	CAN_AxisHome(&CanCom,CAN_X_AXIS_ID,0x00);					//发送X轴回零命令
	CAN_AxisHome(&CanCom,CAN_Y_AXIS_ID,0x00);					//发送Y轴回零命令
  CAN_AxisHome(&CanCom,CAN_Z_AXIS_ID,0x00);					//发送Z轴回零命令
  while(Axis[0].finish == 0);                       //等待精确回零结束
	while(Axis[1].finish == 0);
	while(Axis[2].finish == 0);
  
  Axis[0].position=0.0;                             //数据进行本地记录
  Axis[1].position=0.0;
  Axis[2].position=0.0;
}


/**************************************************************************************
  * name        : void AxisSetSpeed(int x_speed,int y_speed,int z_speed)
  * Function    : 三轴设置速度，设置速度<=0时使用默认速度
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
  * Function    : 柱塞泵抽取指令
  * Parameters  : int vol-- 体积ul
                  int run_time-- 运行时间，为0时使用默认速度
                  int delay_time-- 动作完成后的延时时间，s
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
  * Function    : 柱塞泵注射指令
  * Parameters  : int vol-- 体积ul
                  int run_time-- 运行时间，为0时使用默认速度
                  int delay_time-- 动作完成后的延时时间，s
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
  * Function    : 柱塞泵回零
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
  * Function    : 柱塞泵注射指令
  * Parameters  : int vol-- 体积ul
                  int cnt-- 冲洗次数
                  int delay_time-- 动作完成后的延时时间，s
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void PlungerWash(int vol,int cnt,int delay_time)
{
//  CAN_PlungerWash(&CanTest,CAN_PLUNGER_ID,0,vol,cnt,delay_time);
//  
//  while(Axis[3].finish != 1);                     //等待完成
//  Axis[3].finish=0;
}

/**************************************************************************************
  * name        : void GetSample(int rack_num,int sample_num,int volume)
  * Function    : 输入样本架号、样本编号和取样体积，完成取样动作
  * Parameters  : int rack_num-- 样本架号
                  int sample_num-- 样本编号
                  int volume-- 取样体积ul
  * Returns     : Null
  * Note        : 约定1号样本架的后续样本架编号为1、3、5、7...,2号样本架的后续样本架编号为2、4、6...
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void GetSample(int rack_num,int sample_num,int volume)
{
  float x,y,z;
  
  GetSamplePos(&x,&y,rack_num,sample_num);            //计算xyz
  z= 0;
  AxisRun2Pos(x,y,z);                                 //运行到该位置
  
  AxisRun2Pos(x,y,z+110);                              //到液面

  PlungerDraw(volume,0,0);                            //吸取样本
	Delay_S(1);	

  AxisRun2Pos(x,y,z); 
//  
}

/**************************************************************************************
  * name        : void GetEmptyCup(int cup_num)
  * Function    : 在指定供杯位取一个杯子
  * Parameters  : int cup_num-- 供杯位编号
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
  AxisRun2Pos(x,y-GET_CUP_Y_DIST,HAVE_CUP_Z_POS);    //第一步，准备
  AxisRun2Pos(x,y-GET_CUP_Y_DIST,GET_CUP_CUP_Z_POS); //第二步，下降,卡口与针位移相反

  AxisRun2Pos(x,y,GET_CUP_CUP_Z_POS);                //第三步，卡杯,卡口与针位移相反
  AxisRun2Pos(x,y,HAVE_CUP_Z_POS);                   //第四步，上升  
	
  CupSeat[cup_num]=CUP_GRAY;                         //标记相应供杯位为空
//	CAN_AxisStateRead(&CanTest,CAN_Z_AXIS_ID);				 //检测夹杯状态
	Delay_Ms(10);																			 //等待状态返回
//	while(GetCupState==0)
//	{	
//		CAN_AxisStateRead(&CanTest,CAN_Z_AXIS_ID);			 //检测夹杯状态
//		Delay_S(1);																			 //等待状态返回
//	}
	for(i=0;i<CUP_NUM;i++)                             //刷新所有杯子显示
    LCD_RefreshCupSeat(i);
//	AxisRun2Pos(x,y,HAVE_CUP_Z_POS);    //第五步，保证夹杯后不会撞到相邻的杯子  
}

/**************************************************************************************
  * name        : void PutEmptyCup(int cup_num)
  * Function    : 在指定供杯位放一个杯子
  * Parameters  : int cup_num-- 供杯位编号
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
  
  AxisRun2Pos(x,y,HAVE_CUP_Z_POS);                 //第一步，准备
  AxisRun2Pos(x,y,GET_CUP_CUP_Z_POS); //第二步，下降,卡口与针位移相反

  AxisRun2Pos(x,y-GET_CUP_Y_DIST,GET_CUP_CUP_Z_POS);                //第三步，退杯,卡口与针位移相反
  AxisRun2Pos(x,y-GET_CUP_Y_DIST,HAVE_CUP_Z_POS);                   //第四步，上升  
  
  CupSeat[cup_num]=1;                                //标记相应供杯位为空
}

/**************************************************************************************
  * name        : void GetCupFromDetect(int detect_num)
  * Function    : 在指定检测位取一个杯子
  * Parameters  : int detect_num-- 检测位编号
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
  
  AxisRun2Pos(x,y-GET_CUP_Y_DIST,z);                             //第一步，准备
  AxisRun2Pos(x,y-GET_CUP_Y_DIST,GET_DETECT_CUP_Z_POS);          //第二步，下降,卡口与针位移相反

  AxisRun2Pos(x,y,GET_DETECT_CUP_Z_POS);                         //第三步，卡杯
  AxisRun2Pos(x,y,HAVE_CUP_Z_POS);                               //第四步，上升 
}

/**************************************************************************************
  * name        : void PutCup2Detect(int detect_num)
  * Function    : 放一个杯子在指定检测位
  * Parameters  : int detect_num-- 检测位编号
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
   
  AxisRun2Pos(x,y,HAVE_CUP_Z_POS);                                               //第一步，准备
  
  AxisRun2Pos(x,y,GET_DETECT_CUP_Z_POS);                            //第二步，下降,卡口与针位移相反
  
  AxisRun2Pos(x,y-GET_CUP_Y_DIST,GET_DETECT_CUP_Z_POS);             //第三步，退杯,卡口与针位移相反
  AxisRun2Pos(x,y-GET_CUP_Y_DIST,0);                                //第四步，上升  
}
/**************************************************************************************
  * name        : void GetReagent(int reagent_num,int volume)
  * Function    : 输入试剂位编号和取样体积，完成取样动作
  * Parameters  : int reagent_num-- 试剂位编号
                  int volume-- 取样体积ul
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void GetReagent(int reagent_num,int volume)
{
  float x,y,z;
  
  GetReagnetPos(&x,&y,reagent_num);                   //计算xyz
  z= 0;
  AxisRun2Pos(x,y,z);                                 //运行到该位置
  

  AxisRun2Pos(x,y,z+93);                              //到液面 

  PlungerDraw(volume,0,0);                            //抽试剂 

  AxisRun2Pos(x,y,z);                                 //回到安全位置   
}

/**************************************************************************************
  * name        : void PutReagent2Detect(int detect_num,int volume)
  * Function    : 加试剂到指定检测位 
  * Parameters  : int reagent_num-- 试剂位编号
                  int volume-- 取样体积ul
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void PutReagent2Detect(int detect_num,int volume)
{
  float x,y;
	AxisRun2Zero(); 
  GetCupFromDetect(detect_num);                     //从检测位抓起杯子
  
  GetDetectPos(&x,&y,detect_num);                   //到液面    
	
  AxisRun2Pos(x,y,30); 
  CAN_Mix(1);                                       //开始混匀
  PlungerPush(volume,0,0);                          //进样,进样时多抽的10ul
//	AxisRun2Pos(x,y,31); 
	WhileDelay_ms(200);
	CAN_Mix(0);                                       //停止混匀
//  WhileDelay_ms(20
  PutCup2Detect(detect_num);                        //放杯子
}

/**************************************************************************************
  * name        : void WashNeedle(void)
  * Function    : 清洗进样针
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
  GetReagent(4,120);                                                            //取清洗液
  
  z= 0;
  AxisRun2Pos(WashPosBase[0],WashPosBase[1],z);                                 //就绪 
  
  AxisRun2Pos(WashPosBase[0],WashPosBase[1],90);                                //下降
	PlungerPush(120,0,0);
	Plunger2Zero();
//  
  PUMP_ON;   
  Delay_S(2);
	VALVE_ON;                                                                     //清洗内壁  
  Delay_S(2);
  PUMP_OFF;
	VALVE_OFF;                                                                    //清洗外壁
  Delay_S(1);
//  
  

  AxisRun2Pos(WashPosBase[0],WashPosBase[1],z);                                 //回到安全位置
}

/**************************************************************************************
  * name        : void DiscardCup(int detect_num)
  * Function    : 丢杯
  * Parameters  : NULL
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void DiscardCup(int detect_num)
{
  GetCupFromDetect(detect_num);                                                       //从检测位取杯
  
  AxisRun2Pos(DiscardPosBase[0],DiscardPosBase[1],HAVE_CUP_Z_POS);                                 //准备
  AxisRun2Pos(DiscardPosBase[0],DiscardPosBase[1],DISCARD_CUP_Z_POS);                 //下降
  AxisRun2Pos(DiscardPosBase[0],DiscardPosBase[1]-GET_CUP_Y_DIST,DISCARD_CUP_Z_POS);  //后退 
  AxisRun2Pos(DiscardPosBase[0],DiscardPosBase[1]-GET_CUP_Y_DIST,0);                  //回位
  
  //假的丢杯动作，保证样机可以动
//  AxisRun2Pos(300,10,10);                                 //准备
//  AxisRun2Pos(300,10,0);                 //下降
//  AxisRun2Pos(300,0,0);  //后退 
//  AxisRun2Pos(300,10,10);                  //回位
//  
//  TestCnt++;
}

/**************************************************************************************
  * name        : void DiscardCup(int detect_num)
  * Function    : 丢杯
  * Parameters  : NULL
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void DiscardCup1(int detect_num,int cup_num)
{
  GetCupFromDetect(detect_num);                                                       //从检测位取杯
	PutCupFromEmpty(cup_num);
  
//  AxisRun2Pos(DiscardPosBase[0],DiscardPosBase[1],0);                                 //准备
//  AxisRun2Pos(DiscardPosBase[0],DiscardPosBase[1],DISCARD_CUP_Z_POS);                 //下降
//  AxisRun2Pos(DiscardPosBase[0],DiscardPosBase[1]-GET_CUP_Y_DIST,DISCARD_CUP_Z_POS);  //后退 
//  AxisRun2Pos(DiscardPosBase[0],DiscardPosBase[1]-GET_CUP_Y_DIST,0);                  //回位
  
  //假的丢杯动作，保证样机可以动
//  AxisRun2Pos(300,10,10);                                 //准备
//  AxisRun2Pos(300,10,0);                 //下降
//  AxisRun2Pos(300,0,0);  //后退 
//  AxisRun2Pos(300,10,10);                  //回位
//  
//  TestCnt++;
}


/**************************************************************************************
  * name        : void JudgeDiscardCup(void)
  * Function    : 一个打包的丢杯函数，方便在加样后进行丢杯
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
  
  for(sample_cnt= (MaxRackNum-2)*9;sample_cnt<MaxRackNum*9;sample_cnt++)                  //轮询当前的18个样本
  {
    if(Sample[sample_cnt].state == 1)                                                     //反应中
    {
      for(item_cnt=0;Sample[sample_cnt].item_list[item_cnt].type != 0;item_cnt++)         //轮询测试列表中的所有项目
      {
        if(Sample[sample_cnt].item_list[item_cnt].item_state == ITEM_TEST_FINISH)         //该项目测试完成
        {
          DiscardCup(Sample[sample_cnt].item_list[item_cnt].detect_num);                  //丢杯
          DetectSeatState[Sample[sample_cnt].item_list[item_cnt].detect_num]=0;           //相应检测位状态置0
          Sample[sample_cnt].item_list[item_cnt].item_state= ITEM_DISCARD_FINISH;         //切换为丢杯完成状态 
					Action_Flag++;
          
          return;                                                                         //每次只进行一次丢杯
        }
      }
    }
  }
}


/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------CAN协议 用户自定义命令------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/**************************************************************************************
  * name        : void CAN_Mix(int time)
  * Function    : 混匀操作
  * Parameters  : int state-- 混匀状态
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
//  CanCom.tx_msg.IDE  = CAN_ID_EXT;                         //使用扩展模式
//  CanCom.tx_msg.RTR  = CAN_RTR_DATA;                       //数据帧而不是远程帧
//  CanCom.tx_msg.DLC  = 3;                                  //数组长度
//  
//  CanCom.tx_msg.Data[0]= CMD_USER;                         //用户自定义命令
//  CanCom.tx_msg.Data[1]= CMD_USER_MIX;                 		 //混匀操作 
//  CanCom.tx_msg.Data[2]= state ;                         	 //混匀时间
////  CanTest.tx_msg.Data[3]= time & 0XFF;

//  CAN_Transmit(CanCom.com, &CanCom.tx_msg);
//  
////  while(Axis[2].finish != 1);                               //等待混匀完成
////  Axis[2].finish=0;
}

/**************************************************************************************
  * name        : void CAN_StartTest(int test_type,int detect_num)
  * Function    : 启动一项测试
  * Parameters  : int test_type-- 测试类型
                  int detect_num-- 所使用的检测器通道
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_StartTest(int test_type,int detect_num)
{
//  CanCom.tx_msg.ExtId= ((u32)CanCom.my_id    <<21) |         //源地址
//                         ((u32)CAN_ADC_ID      <<13) |       //目标板地址
//                         ((u32)CMD_USER    <<5)  |        	 //用户自定义类命令
//                         (u32)CMD_TYPE;                   	 //命令属性
//  
//  CanCom.tx_msg.IDE  = CAN_ID_EXT;                           //使用扩展模式
//  CanCom.tx_msg.RTR  = CAN_RTR_DATA;                         //数据帧而不是远程帧
//  CanCom.tx_msg.DLC  = 8;                                    //数组长度
//  
//  CanCom.tx_msg.Data[0]= detect_num;                         //通道地址
//  CanCom.tx_msg.Data[1]= CMD_USER_TESTSS;                		 //检测起停命令
//  CanCom.tx_msg.Data[2]= 0X01;                               //启动检测
//  CanCom.tx_msg.Data[3]= test_type;                          //检测类型
//  
//  CAN_Transmit(CanCom.com, &CanCom.tx_msg);
}

/**************************************************************************************
  * name        : void CAN_SetTestMethod(int test_type,TestMethodInfoStruct *method)
  * Function    : 设置一项测试的测试方法
  * Parameters  : int test_type-- 测试类型
                  TestMethodInfoStruct *method-- 测试方法存储结构体
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_SetTestMethod(int test_type,TestMethodInfoStruct *method)
{
//  CanCom.tx_msg.ExtId= ((u32)CanTest.my_id    <<21) |            //源地址
//                         ((u32)CAN_ADC_ID      <<13) |            //目标板地址
//                         ((u32)CMD_USER     <<5) |            //用户自定义类命令
//                         (u32)CAN_CMD_TYPE;                       //命令属性
//  
//  CanCom.tx_msg.IDE  = CAN_ID_EXT;                               //使用扩展模式
//  CanCom.tx_msg.RTR  = CAN_RTR_DATA;                             //数据帧而不是远程帧
//  CanCom.tx_msg.DLC  = 8;                                        //数组长度
//  
//  CanCom.tx_msg.Data[0]= 0;                                      //不指定通道
//  CanCom.tx_msg.Data[1]= CAN_CMD_USER_PTSET+(test_type-PT_TEST); //对应项目的参数设置命令
//  CanCom.tx_msg.Data[2]= method->ch_type;                        //通道类型，散射/透射
//  CanCom.tx_msg.Data[3]= method->calcu_type;                     //算法类型，阈值/一阶/二阶/中值法
//  CanCom.tx_msg.Data[4]= method->max_time;                       //最大检测时间
//  
//  CAN_Transmit(CanCom.com, &CanCom.tx_msg);

}

/**************************************************************************************
  * name        : void CAN_Hello(int board_id)
  * Function    : 握手指令
  * Parameters  : int board_id-- 目标板ID
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_Hello(int board_id)
{
//  CanCom.tx_msg.ExtId= ((u32)CanCom.my_id    <<21) |            //源地址
//                         ((u32)board_id        <<13) |            //目标板地址
//                         ((u32)CAN_CMD_USER     <<5) |            //用户自定义类命令
//                         (u32)CAN_CMD_TYPE;                       //命令属性
//  
//  CanCom.tx_msg.IDE  = CAN_ID_EXT;                               //使用扩展模式
//  CanCom.tx_msg.RTR  = CAN_RTR_DATA;                             //数据帧而不是远程帧
//  CanCom.tx_msg.DLC  = 8;                                        //数组长度
//  
//  CanCom.tx_msg.Data[0]= 0;                                      //不指定通道
//  CanCom.tx_msg.Data[1]= CAN_CMD_USER_HELLO;                     //握手指令

//  CAN_Transmit(CanCom.com, &CanCom.tx_msg);
}

/**************************************************************************************
  * name        : void CAN_SelfTest(int board_id)
  * Function    : 自检指令
  * Parameters  : int board_id-- 目标板ID
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_SelfTest(int board_id)
{
//  CanCom.tx_msg.ExtId= ((u32)CanCom.my_id    <<21) |            //源地址
//                         ((u32)board_id        <<13) |            //目标板地址
//                         ((u32)CAN_CMD_USER     <<5) |            //用户自定义类命令
//                         (u32)CAN_CMD_TYPE;                       //命令属性
//  
//  CanCom.tx_msg.IDE  = CAN_ID_EXT;                               //使用扩展模式
//  CanCom.tx_msg.RTR  = CAN_RTR_DATA;                             //数据帧而不是远程帧
//  CanCom.tx_msg.DLC  = 8;                                        //数组长度
//  
//  CanCom.tx_msg.Data[0]= 0;                                      //不指定通道
//  CanCom.tx_msg.Data[1]= CAN_CMD_USER_SELF_TEST;                 //自检

//  CAN_Transmit(CanCom.com, &CanCom.tx_msg);
}
/*--------------------------------------------------------------------------------------------------------------------*/



/**************************************************************************************
  * name        : void SampleInfoInit(void)
  * Function    : 初始化样本信息，主要是样本位信息
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
      Sample[sample_cnt].sample_code[8]= '0' + rack_cnt/10;  //样本架号十位
      Sample[sample_cnt].sample_code[9]= '0' + rack_cnt%10;  //样本架号个位
      Sample[sample_cnt].sample_code[10]= '0' + seat_cnt/10; //样本位号十位
      Sample[sample_cnt].sample_code[11]= '0' + seat_cnt%10; //样本位号个位
      Sample[sample_cnt].state      = SAMPLE_GRAY;
      sample_cnt++;
    }
  }
}


/**************************************************************************************
  * name        : void GetSamplePos(float *x_pos,float *y_pos,int rack_num,int sample_num)
  * Function    : 输入样本架号和样本编号，获取该样本位的实际坐标
  * Parameters  : float *x_pos-- 返回的实际坐标的x
                  float *y_pos-- 返回的实际坐标的y
                  int rack_num-- 样本架号
                  int sample_num-- 样本编号
  * Returns     : Null
  * Note        : 约定1号样本架的后续样本架编号为1、3、5、7...,2号样本架的后续样本架编号为2、4、6...
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void GetSamplePos(float *x_pos,float *y_pos,int rack_num,int sample_num)
{
  if(rack_num%2 == 1)                                   //1号样本架位置上
  {
    *x_pos= SamplePosBase[0] ;
  }
  else if(rack_num%2 == 0)                              //2号样本架位置上
  {
    *x_pos= SamplePosBase[2];
  }
  
  *y_pos= SamplePosBase[1] + sample_num*SAMPLE_SPACE_Y;
}

/**************************************************************************************
  * name        : void GetCupPos(float *x_pos,float *y_pos,int cup_num)
  * Function    : 输入供杯位编号，获取该供杯位的实际坐标
  * Parameters  : float *x_pos-- 返回的实际坐标的x
                  float *y_pos-- 返回的实际坐标的y
                  int cup_num--  供杯位编号
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void GetCupPos(float *x_pos,float *y_pos,int cup_num)
{
  *x_pos= CupPosBase[0] + (cup_num%CUP_NUM_X)*CUP_SPACE_X;   //考虑兼容最后一列杯子
  *y_pos= CupPosBase[1] + (cup_num/CUP_NUM_X)*CUP_SPACE_Y;
  
  if(cup_num >= CUP_NUM_X*CUP_NUM_Y)                          //第二个供杯架
    *y_pos+= CUP_RACK_SPACE_Y - CUP_SPACE_Y;
}

/**************************************************************************************
  * name        : void GetReagnetPos(float *x_pos,float *y_pos,int reagent_num)
  * Function    : 输入试剂位编号，获取该试剂位的实际坐标
  * Parameters  : float *x_pos-- 返回的实际坐标的x
                  float *y_pos-- 返回的实际坐标的y
                  int reagent_num--  试剂位编号
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void GetReagnetPos(float *x_pos,float *y_pos,int reagent_num)
{
  if(reagent_num < 3)                                       //大号试剂位
  {
    *x_pos= ReagentPosBase[0] + reagent_num*REAGENT_SPACE1_X;
    *y_pos= ReagentPosBase[1];
  }
  else                                                      //小号试剂位
  {
    *x_pos= ReagentPosBase[0] + REAGENT_RACK_SPACE_X;       //第一个小号试剂位坐标
    *y_pos= ReagentPosBase[1] + REAGENT_RACK_SPACE_Y;
    
    reagent_num -= 3;                                       //除去大号试剂位
    *x_pos += (reagent_num%REAGENT_NUM_X)*REAGENT_SPACE2_X;
    *y_pos += reagent_num/REAGENT_NUM_X * REAGENT_SPACE2_Y;   
  }
}

/**************************************************************************************
  * name        : void GetDetectPos(float *x_pos,float *y_pos,int detect_num)
  * Function    : 输入检测位编号，获取该检测位的实际坐标
  * Parameters  : float *x_pos-- 返回的实际坐标的x
                  float *y_pos-- 返回的实际坐标的y
                  int detect_num--  检测位编号
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
  * Function    : 使用while循环实现的不精确的ms级延时，用于补充定时器精确延时不适用于某些场合的情况
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
  * name        : void StartSoftTimer(SoftTimerStruct *timer，int t)
  * Function    : 开启软件定时器
  * Parameters  : SoftTimerStruct *timer--指定软件定时器
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
  * Function    : 清零软件定时器
  * Parameters  : SoftTimerStruct *timer--指定软件定时器
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
  * Function    : 初始化本板LED
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
  * Function    : 夹杯后状态读取
  * Parameters  : Null
  * Returns     : Null
  * Author      : ZQChen
  * Check       : 
**************************************************************************************/
void GetCup_StateRead(void)
{
	
}



/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/
