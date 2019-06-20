/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : main.c
  * Version     : V0.1
                  V0.1.5  2017/11/10
                    1��������Labview���Թ���
                    2�������˶��źŲɼ���Ŀ���X
                  V0.1.6
                    1�����Ӳ���LCD�Ĺ���
                  V0.1.7
                    1��ʵ����������㷨
                    2��ʵ�ֵ�һ�߽������µ�����������ֵ�㷨
                    3��ʵ�ֶ���ʽ����㷨
                    4��ʵ�ֶ������ߵ���ʾ����������㺯��
                  V0.1.8  2017/12/18
                    1����AD��������������CANͨ�Ź���
                    2��ʵ�ִ����¼������ع���
                    3��������������޸�Ϊ�������Ե�����
                    4���޸��˶�����Ϣ�ṹ��
                    5��ʵ����Ȼ�߽������µ�����������ֵ�㷨
                  V0.1.9  2018/1/3  
                    1��������������޸�Ϊ���µ�����
                    
  * Description : 17003Master_V0.1���ذ����
  * Date        : 2017��9��13��
  * Author      : XYD
  * Check       :  
*******************************************************************************/

/*********************File include*********************/
#include "SysWork.h"      //��SysWork.h�ļ���Ϊ����ͷ�ļ����ļ��������ⲿ�����������ڸ��ļ��н���


/***************Global variable definition*************/

MNTC_CanProtocolTypeDef CanCom;													 //CANͨ�Žṹ��
DW_Struct  LCD;	                                         //��ʾ���ṹ��								
int MaxRackNum=2;                                        //��ǰ�������������Ĭ��2�������µ�������+1
SampleInfoStruct Sample[100];                            //������Ϣ�����100������
ReagentInfoStruct Reagent[19];                           //�Լ���Ϣ�����19���Լ�
int RunState;                                            //�豸����״̬��0--׼����1--������
int CupSeat[90];                                         //����λ��Ϣ����0--û�б�����1--�б�
int DetectSeatState[DETECT_SEAT_NUM];                    //0-- ���λ���� 1-- ���λ��ռ�� 
TestMethodInfoStruct TestMethod[8];                      //���Է����洢��[0]��ʹ��,PT_TEST,APTT_TEST....
CalibrationStruct Calibration[4];                        //�������ݣ�0--FIB,1--AT3,2--FDP,3--DD
CalibrationStruct EditCalibration;                       //�༭�еĶ�������
float SamplePosBase[4];                                  //�����ܻ���ַ����һ������λ����
float CupPosBase[2];                                     //����λ����ַ�����Ͻǵ�һ������λ
float ReagentPosBase[2];                                 //�Լ�λ����ַ����һ���Լ�λ����
float DetectPosBase[DETECT_SEAT_NUM];                    //���λ����ַ
float WashPosBase[2];                                    //��ϴλ����ַ
float DiscardPosBase[2];                                 //����λ����ַ
AxisStruct Axis[4];                                      //������Ϣ�ṹ��
int Pump[2];																						 //�䶯���˶�״̬ 0-- δ��� 1-- ���
SoftTimerStruct DetectDelay[DETECT_SEAT_NUM];            //��������ʱר�������ʱ��
int NowPageNum;                                          //��ǰҳ���ַ
int ReturnPagNum;                                        //���ص�ҳ���ַ
QueueStruct Err;                                         //������� 
BarcodeTypeDef Barcode;                                  //����ṹ��

int TestCnt;                                             //�Ѳ�����Ŀ����
int Hour,Minute,Second;                                  //�Ѳ���ʱ��
int LabviewFlag;
int LabviewBuf[20];


float fib_result1[4],fib_result2;
float rr;
int main(void)
{
  int i,j;
  LCD.com= UART5;
  Barcode.com= USART2;
//  CreatQueue(&Err,20);                                  //�����������
  SampleInfoInit();  
  SeatInfoInit();                                       	//����λ��Ϣ��ʼ��
  TestMethodInit();                                     	//���Է�����ʼ��
  
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
  
  
	MNTC_CanConfig(CAN1,0);																//CAN������������ʼ��
	CAN_ProtocolInit(&CanCom,CAN1,CAN_MASTER_ID);					//CANЭ���ʼ��
  TIM_TIM5_Configuration();                             //���ڶ�ʱ��
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
   

//  WhileDelay_ms(1000);            //������ʱ
//  LCD_StartBar(&LCD);
//	AxisRun2Zero();                 //����	
//	AxisRun2Pos(15,14,-500);

  

	WashNeedle();
	AxisRun2Zero();
	
  /***********ģ������������Ϣ����***********/
  for(i=0;i<8;i++)                                      //����8��������ÿ��������һ��Ѫ������
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


  /***********ģ�����̹���λ����***********/
  for(i=0;i<90;i++)                                     //�������й���λ���б���
    CupSeat[i]= 1;

  /***********ģ�������Լ�λ����***********/
  Reagent[3].type= TT_REAGENT;
  Reagent[3].single_cnt= 50;
  Reagent[4].type= APTT_REAGENT;
  Reagent[4].single_cnt= 50;
  Reagent[5].type= TT_REAGENT;
  Reagent[5].single_cnt= 50;
  Reagent[6].type= FIB_REAGENT;
  Reagent[6].single_cnt= 50;
	
  RunState=1;
	
	
	//��ʹ�����мб�����������ʵ��
	GetSample2Detect(0,0,0);      //��������,0��������0�ż����Ŀ��ʹ��0�ż��λ
	StartSoftTimer(&DetectDelay[0],180); //0�ż��λ��ʱ180s,���з���
	WashNeedle();//ϴ��
	AxisRun2Zero();//����
	while(DetectDelay[0].now < DetectDelay[0].set);//�ȴ��������
	ClearSoftTimer(&DetectDelay[0]);
	AddR1ToSample(0,0);//�����Ŀ����R1
	CAN_StartTest(Sample[0].item_list[0].type,Sample[0].item_list[0].detect_num);//CANͨ��֪ͨ��ʼ���
	
	while(1);
	
	
	
	//ȫ��������
  while(1)
  {
    if(RunState == 1)
      DetectProcess();
  }
}



/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/
