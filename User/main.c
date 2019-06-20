/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : main.c
  * Version     : V0.1
                  V0.1.5  2017/11/10
                    1、增加了Labview调试功能
                    2、增加了对信号采集板的控制X
                  V0.1.6
                    1、增加部分LCD的功能
                  V0.1.7
                    1、实现线性拟合算法
                    2、实现第一边界条件下的三次样条插值算法
                    3、实现多项式拟合算法
                    4、实现定标曲线的显示和相关逆运算函数
                  V0.1.8  2017/12/18
                    1、与AD板联调测试所有CAN通信功能
                    2、实现错误记录队列相关功能
                    3、将基坐标参数修改为经过测试的数据
                    4、修改运动轴信息结构体
                    5、实现自然边界条件下的三次样条插值算法
                  V0.1.9  2018/1/3  
                    1、将基坐标参数修改为最新的数据
                    
  * Description : 17003Master_V0.1主控板程序
  * Date        : 2017年9月13日
  * Author      : XYD
  * Check       :  
*******************************************************************************/

/*********************File include*********************/
#include "SysWork.h"      //将SysWork.h文件作为公共头文件，文件包含和外部变量声明请在该文件中进行


/***************Global variable definition*************/

MNTC_CanProtocolTypeDef CanCom;													 //CAN通信结构体
DW_Struct  LCD;	                                         //显示屏结构体								
int MaxRackNum=2;                                        //当前最大样本架数，默认2，更换新的样本架+1
SampleInfoStruct Sample[100];                            //样本信息，最多100个样本
ReagentInfoStruct Reagent[19];                           //试剂信息，最多19个试剂
int RunState;                                            //设备运行状态，0--准备，1--运行中
int CupSeat[90];                                         //供杯位信息，灰0--没有杯，绿1--有杯
int DetectSeatState[DETECT_SEAT_NUM];                    //0-- 检测位空闲 1-- 检测位被占用 
TestMethodInfoStruct TestMethod[8];                      //测试方法存储，[0]不使用,PT_TEST,APTT_TEST....
CalibrationStruct Calibration[4];                        //定标数据，0--FIB,1--AT3,2--FDP,3--DD
CalibrationStruct EditCalibration;                       //编辑中的定标数据
float SamplePosBase[4];                                  //样本架基地址，第一个样本位坐标
float CupPosBase[2];                                     //供杯位基地址，左上角第一个供杯位
float ReagentPosBase[2];                                 //试剂位基地址，第一个试剂位坐标
float DetectPosBase[DETECT_SEAT_NUM];                    //检测位基地址
float WashPosBase[2];                                    //清洗位基地址
float DiscardPosBase[2];                                 //丢杯位基地址
AxisStruct Axis[4];                                      //三轴信息结构体
int Pump[2];																						 //蠕动泵运动状态 0-- 未完成 1-- 完成
SoftTimerStruct DetectDelay[DETECT_SEAT_NUM];            //各检测块延时专用软件定时器
int NowPageNum;                                          //当前页面地址
int ReturnPagNum;                                        //返回的页面地址
QueueStruct Err;                                         //错误队列 
BarcodeTypeDef Barcode;                                  //条码结构体

int TestCnt;                                             //已测试项目个数
int Hour,Minute,Second;                                  //已测试时间
int LabviewFlag;
int LabviewBuf[20];


float fib_result1[4],fib_result2;
float rr;
int main(void)
{
  int i,j;
  LCD.com= UART5;
  Barcode.com= USART2;
//  CreatQueue(&Err,20);                                  //创建错误队列
  SampleInfoInit();  
  SeatInfoInit();                                       	//各种位信息初始化
  TestMethodInit();                                     	//测试方法初始化
  
  Usart1_Configuration(115200);
  Usart1_Nvic_Configuration();
	Usart2_Configuration(9600);
  Usart2_Nvic_Configuration();
	Usart3_Configuration(115200);
  Usart3_Nvic_Configuration();
  Uart4_Configuration(115200);
  Uart4_Nvic_Configuration();
	Uart5_Configuration(115200);
  Uart5_Nvic_Configuration();
  
  
	MNTC_CanConfig(CAN1,0);																//CAN总线驱动器初始化
	CAN_ProtocolInit(&CanCom,CAN1,CAN_MASTER_ID);					//CAN协议初始化
  TIM_TIM5_Configuration();                             //周期定时器
  TIM_TIM5_Open();
	EXTI_PauseConfiguration();
	EXTI_PauseEnable();
	Delay_Init(72);
  LED_Init();
	
	Pump[0]=0;
	CAN_PeristalticPumpRun(&CanCom,CAN_PRP1_ID,0x00,10);
	while(Pump[0] == 0);
	
	Pump[0]=0;
	CAN_PeristalticPumpRun(&CanCom,CAN_PRP1_ID,0x00,1);
	while(Pump[0] == 0);
	while(1);
   

//  WhileDelay_ms(1000);            //开机延时
//  LCD_StartBar(&LCD);
//	AxisRun2Zero();                 //回零	
//	AxisRun2Pos(15,14,-500);

  

	WashNeedle();
	AxisRun2Zero();
	
  /***********模拟流程样本信息设置***********/
  for(i=0;i<8;i++)                                      //测试8个样本，每个样本做一次血凝四项
  {
    Sample[i].item_list[0].type= PT_TEST;
    Sample[i].item_list[1].type= PT_TEST;
    Sample[i].item_list[2].type= PT_TEST;
    Sample[i].item_list[3].type= PT_TEST;
    Sample[i].item_list[4].type= PT_TEST;
    Sample[i].item_list[5].type= PT_TEST;
    Sample[i].item_list[6].type= PT_TEST;
    Sample[i].state            =2;
    Sample[i].rack_num         =1;
    Sample[i].seat_num         =i+1;
  }
    Sample[8].item_list[0].type= PT_TEST;
    Sample[8].item_list[1].type= PT_TEST;
    Sample[8].item_list[2].type= PT_TEST;
    Sample[8].item_list[3].type= PT_TEST;
    Sample[8].state            =2;
    Sample[8].rack_num         =2;
    Sample[8].seat_num         =1;


  /***********模拟流程供杯位设置***********/
  for(i=0;i<90;i++)                                     //设置所有供杯位都有杯子
    CupSeat[i]= 1;

  /***********模拟流程试剂位设置***********/
  Reagent[3].type= TT_REAGENT;
  Reagent[3].single_cnt= 50;
  Reagent[4].type= APTT_REAGENT;
  Reagent[4].single_cnt= 50;
  Reagent[5].type= TT_REAGENT;
  Reagent[5].single_cnt= 50;
  Reagent[6].type= FIB_REAGENT;
  Reagent[6].single_cnt= 50;
	
  RunState=1;
	
	
	//不使用所有夹杯操作，生化实验
	GetSample2Detect(0,0,0);      //加样动作,0号样本的0号检测项目，使用0号检测位
	StartSoftTimer(&DetectDelay[0],180); //0号检测位延时180s,进行孵育
	WashNeedle();//洗针
	AxisRun2Zero();//回零
	while(DetectDelay[0].now < DetectDelay[0].set);//等待孵育完成
	ClearSoftTimer(&DetectDelay[0]);
	AddR1ToSample(0,0);//向该项目加入R1
	CAN_StartTest(Sample[0].item_list[0].type,Sample[0].item_list[0].detect_num);//CAN通信通知开始检测
	
	while(1);
	
	
	
	//全流程运行
  while(1)
  {
    if(RunState == 1)
      DetectProcess();
  }
}



/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/
