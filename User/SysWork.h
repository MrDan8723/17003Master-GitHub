/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : SysWork.h
  * Version     : V0.1
  * Description : Ѫ�������ذ幤������
  * Date        : 2017��9��22��
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#ifndef _SYSWORK_H_
#define _SYSWORK_H_


/*********************Hardware File include*********************/
#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "USART_Configuration.h"
#include "MNTC_CanProtocol.h"
#include "DS18B20_V0.1.h"
#include "DW_Agreement.h"
#include "Barcode_V0.1.h"
#include "MNTC_CanConfig.h"

/*********************User File include*********************/
#include "17003UI.h"
#include "SysWork.h"
#include "Scope_SendData.h"
#include "Calibration.h"
#include "Delay.h"
#include "TIM.h"
/******************Macro definition********************/
//�忨ID����ͬ��Ŀ���н��з���
#define CAN_MASTER_ID   0X01            //����ID
#define CAN_Z_AXIS_ID   0X02            //Z���ID
#define CAN_X_AXIS_ID   0X03            //X���ID
#define CAN_Y_AXIS_ID   0X04            //Y���ID
#define CAN_PLUNGER_ID  0X05            //�����ð�ID
#define CAN_ADC_ID      0XF6            //ADC��ID
#define CAN_TEMP_ID     0XF7            //�¿ذ��ID
#define CAN_PRP1_ID			0XF8						//�䶯��1ID
#define CAN_PRP2_ID			0XF9						//�䶯��2ID



#define MAX_X_POS       330.0               //X������г�330mm
#define MAX_Y_POS       170.0               //Y������г�170mm
#define MAX_Z_POS       33.0                //�˶���Z������г�33mm
#define SAFE_Z_POS      0                   //Z�ᰲȫ�г̣��ƶ��б�֤��ײ��


//����λ������Ϣ
#define SAMPLE_SPACE_X        25.5                 //����������X������
#define SAMPLE_SPACE_Y        16                 //ͬһ������������������λY��������ľ�

//����λ������Ϣ

#define CUP_SPACE_X           17                      //���ڹ���λX������
#define CUP_SPACE_Y           13.0                      //���ڹ���λY������
#define CUP_RACK_SPACE_Y      21.0                      //��������λ��Y�����ϵļ��
#define CUP_NUM_X             8                         //һ��������X������9������λ
#define CUP_NUM_Y             5                         //һ��������Y������5������λ

//�Լ�λ������Ϣ
#define REAGENT_SPACE1_X       35.0                //���ڴ���Լ�λX������
#define REAGENT_RACK_SPACE_X   -2.0               //��һ������Լ�λ�͵�һ��С���Լ�λX������
#define REAGENT_RACK_SPACE_Y   32.0                //��һ������Լ�λ�͵�һ��С���Լ�λY������
#define REAGENT_SPACE2_X      26.0                //����С���Լ�λX������
#define REAGENT_SPACE2_Y      26.0                //����С���Լ�λY������
#define REAGENT_NUM_X        4                   //С���Լ�λX�������
#define REAGENT_NUM_Y        4                   //С���Լ�λY�������

//���λ������Ϣ
#define DETECT_SPACE_X    16.8              //���ڼ��λX������
#define DETECT_SPACE_Y    0.0               //���ڼ��λY������


//ģ�����Ծ���
#define DETECT_BLOCK_X    53.9              	//���ڼ���X������
#define DETECT_BLOCK_Y    0.0               //���ڼ���Y������

#define DETECT_BLOCK45_X    48              	//����4��5X������
#define DETECT_BLOCK45_Y    0.0               //����4��5Y������

#define DETECT_BLOCK56_X    36              	//����4��5X������
#define DETECT_BLOCK56_Y    0.0               //����4��5Y������

#define DETECT_TO_CUP_X   59								//��һ�����λ�͵�һ������λX������
#define DETECT_TO_CUP_Y   42							//��һ�����λ�͵�һ������λY������

#define DETECT_TO_SAMPLE_X   0  							//��һ�����λ�͵�һ������λX������
#define DETECT_TO_SAMPLE_Y   31						//��һ�����λ�͵�һ������λY������

#define DETECT_TO_REAGENT_X   234							//��һ�����λ�͵�һ���Լ�λX������
#define DETECT_TO_REAGENT_Y   55						//��һ�����λ�͵�һ���Լ�λY������

#define DETECT_TO_WASH_X   200							//��һ�����λ����ϴλX������
#define DETECT_TO_WASH_Y   69					//��һ�����λ����ϴλY������



//
#define GET_CUP_Y_DIST    16              	//ȡ��ʱY���˵ľ���
#define DISCARD_CUP_Y_DIST    17              	//����ʱY���˵ľ���
#define HAVE_CUP_Z_POS    20                //ȡ����֤��ײ��Z������

#define GET_CUP_CUP_Z_POS     -14.5        //ȡ����λ�ձ�׼������ʱZ������           
#define GET_DETECT_CUP_Z_POS  -14.5         //ȡ���λ��׼������ʱZ������
#define DISCARD_CUP_Z_POS     -14.5           //����λ�½����Z����



#define SAMPLE_RACK_NUM   2                 //�����ܸ���
#define SINGLE_RACK_NUM   9                 //������������������
#define SAMPLE_NUM        (SAMPLE_RACK_NUM*SINGLE_RACK_NUM)//������
#define MAX_TEST_LIST_NUM 12                 //һ����������ܹ�������Ŀ����
#define DETECT_SEAT_NUM   12                 //���λ����
#define REAGENT_NUM       19                //�Լ�����
#define CUP_NUM           90                //��Ӧ������


//������Ŀ״̬
#define ITEM_READY_FINISH            0      //׼��
#define ITEM_SAMPLE_FINISH           1      //�����������
#define ITEM_INCUBATE_FINISH         2      //����
#define ITEM_ADD_R1_FINISH           3      //R1�Լ��������
#define ITEM_R1_TEST_FINISH          4      //R1�Լ��������
#define ITEM_ADD_R2_FINISH           5      //R2�Լ��������
#define ITEM_TEST_FINISH             6      //R2��Ӧ���=��Ӧ���
#define ITEM_DISCARD_FINISH          7      //�������

//�Լ�������Ϣ
#define PT_REAGENT        1                 //PT�Լ�
#define APTT_REAGENT      2                 //APTT�Լ�
#define TT_REAGENT        3                 //TT�Լ�
#define FIB_REAGENT       4                 //FIB�Լ�
#define OVB_REAGENT       5                 //OVB����Һ
#define DD_R1_REAGENT     6                 //DD����R1�Լ�
#define DD_R2_REAGENT     7                 //DD����R2�Լ�
#define FDP_R1_REAGENT    8                 //FDP R1
#define FDP_R2_REAGENT    9                 //FDP R2
#define HEPARIN_REAGENT   10                //�����Լ�
#define AT3_R1_REAGENT    11                //AT3��R1�Լ�
#define AT3_R2_REAGENT    12                //AT3��R2�Լ�
#define CaCl2_REAGENT     13                //CaCl2�Լ�
#define CLEAN_REAGENT     14                //��ϴҺ
//�����
#define PT_S_RESULT         0               //PT(S)
#define PT_INR_RESULT       1               //PT(INR)
#define PT_PERCENT_RESULT   2               //PT(%)
#define APTT_S_RESULT       3               //APTT(S)
#define TT_S_RESULT         4               //TT(S)
#define FIB_S_RESULT        5               //FIB(S)
#define FIB_GL_RESULT       6               //FIB(g/L)
#define DD_OD_RESULT        7               //DD(OD)
#define DD_GL_RESULT        8               //DD(g/L)
#define FDP_OD_RESULT       9               //FDP(OD)
#define FDP_GL_RESULT       10              //FDP(g/L)
#define AT3_OD_RESULT       11              //AT3(OD)
#define AT3_PERCENT_RESULT  12              //AT3(%)

//����λ״̬
#define SAMPLE_GRAY       0                 //��--δѡ��/�����
#define SAMPLE_YELLOW     1                 //��--ѡ��/������
#define SAMPLE_GREEN      2                 //��--�������/��Ӧ���
#define SAMPLE_RED        3                 //��-- 

//����λ״̬
#define CUP_GRAY          0                 //��--û�з�Ӧ��
#define CUP_GREEN         1                 //��--�з�Ӧ��

//�Լ�λ״̬
#define REAGENT_GRAY      0                 //��--δ�����Լ�
#define REAGENT_YELLOW    1                 //��--�Լ�����
#define REAGENT_GREEN     2                 //��--�Լ�������
#define REAGENT_RED       3                 //��--�Լ���Ԥ��
#define REAGENT_ENOUGH    20                //ʣ���Լ���>=REAGENT_ENOUGH-- �Լ�������
#define REAGENT_ALARM     10                //ʣ���Լ���>=REAGENT_ALARM-- �Լ�����


//������Ŀ��Ϣ
#define PT_TEST           1                 //PT����
#define APTT_TEST         2                 //APTT����
#define TT_TEST           3                 //TT����
#define FIB_TEST          4                 //FIB����
#define AT3_TEST          5                 //AT3����
#define FDP_TEST          6                 //FDP����
#define DD_TEST           7                 //DD����

//ͨ������
#define SCATTERING_CH     0X00              //ɢ��ͨ��
#define TRANSMISSION_CH   0X01              //͸��ͨ��

//�㷨����
#define THRESHOLD_METHOD    0X00            //��ֵ��
#define DERIVATIVE_1_METHOD 0X01            //һ�׷�          
#define DERIVATIVE_2_METHOD 0X02            //���׷�
#define MEDIAN_METHOD       0X03            //��ֵ��


//CANЭ���û��Զ����
#define CAN_CMD_USER_TESTSS     0X00        //�����ͣ����ָ��
#define CAN_CMD_USER_STOP_TEST  0X07        //ֹͣ���в���
#define CAN_CMD_USER_MIX        0X08        //��������
#define CAN_CMD_USER_LEVEL      0X09        //��λ��Һ��
#define CAN_CMD_USER_PTSET      0X10        //PT��ⷽ������
#define CAN_CMD_USER_APTTSET    0X11        //APTT��ⷽ������
#define CAN_CMD_USER_TTSET      0X12        //TT��ⷽ������
#define CAN_CMD_USER_FIBSET     0X13        //FIB��ⷽ������
#define CAN_CMD_USER_AT3SET     0X14        //AT3��ⷽ������
#define CAN_CMD_USER_FDPSET     0X15        //FDP��ⷽ������ 
#define CAN_CMD_USER_DDPSET     0X16        //DD��ⷽ������
#define CAN_CMD_USER_HELLO      0X17        //����ָ��
#define CAN_CMD_USER_SELF_TEST  0X18        //�Լ�ָ��

#define CAN_DATA_USER_RESULT    0X00        //�����ɣ�������ش�
#define CAN_DATA_USER_TEMP    	0X01        //�¶����ݻش�


#define LED1_ON           GPIO_ResetBits(GPIOD,GPIO_Pin_3)
#define LED1_OFF          GPIO_SetBits(GPIOD,GPIO_Pin_3)

#define LED2_ON           GPIO_ResetBits(GPIOD,GPIO_Pin_4)
#define LED2_OFF          GPIO_SetBits(GPIOD,GPIO_Pin_4)

#define PUMP_OFF          GPIO_SetBits(GPIOD,GPIO_Pin_13)
#define PUMP_ON           GPIO_ResetBits(GPIOD,GPIO_Pin_13)

#define VALVE_OFF         GPIO_SetBits(GPIOD,GPIO_Pin_12)
#define VALVE_ON          GPIO_ResetBits(GPIOD,GPIO_Pin_12)

/*********Enumeration variable type definition********/




/**************Structure type definition**************/

//������ذ�����Ĺ��������Ϣ����Ľṹ��
//δ���ر�˵��ʱ���ṹ��������Ԫ�صķ��䰴XYZ��˳�����
typedef struct _AxisStruct                    
{
  int   finish;                              //ָ����ɱ�־λ��0-- δ��ɣ�1-- ���
  float speed;                               //mm/s,������ͨ��CAN�����·�ʱҪ
  float position;                            //���һ��ָ����ָ������������Ϣ,������ͨ��CAN�����·�ʱҪx10
}AxisStruct;

typedef struct _TestItemInfoStruct     //������Ŀ��Ϣ�ṹ��
{
  int type;                        //������Ŀ���ͣ�����궨��
  int detect_num;                  //��ʹ�õļ��λ��ţ���0��ʼ����
  int item_state;                  //������Ŀ״̬,����궨��
	float item_result;
  float dilution;                  //ϡ��ϵ��
}TestItemInfoStruct;

typedef struct _SampleInfoStruct   //sample information������Ϣ
{
  int rack_num;                    //���������ܼܺţ�
  int seat_num;                    //�ڶ�Ӧ��������������ţ�1~9
  char sample_code[12];            //������ţ�Ĭ��eg��201710190101
  int state;                       //����λ״̬0��--δѡ��/�������  1��--�༭��/��Ӧ�� 2��--�༭���/�ȴ���Ӧ 3��--
  float result[13];                //���Խ������Ӧ��Ŀ�Ľ���������ݶ�Ӧλ��
  TestItemInfoStruct item_list[MAX_TEST_LIST_NUM];    //�����б������ѡ��12��������Ŀ�������ظ�������Ŀ,��Ŀ�����SysWork�ļ��궨��
}SampleInfoStruct;

typedef struct _ReagentInfoStruct  //�Լ���Ϣ�ṹ��
{
  int  type;                        //ʵ������
  int  single_cnt;                  //��ƿ����
  int  batch_cnt;                   //��������
  char batch_code[10];              //����
  char date[12];                    //��Ч�ڣ��������
  int  isi;                         //�������ж�ָ����ר���ڼ���PT��INRֵ
}ReagentInfoStruct;

typedef struct _TestMethodInfoStruct  //���Է����ṹ��
{
  int ch_type;                     //ͨ�����ͣ�ȡֵ���궨��
  int calcu_type;                  //�㷨���ͣ�ȡֵ���궨��
  int reference[2];                //�ο�ֵ,[0]--�ο�ֵ���� [1]--�ο�ֵ����
  int sample_vol;                  //�������ul
  int incubate_time;               //����ʱ��
  int r1_type;                     //�Լ�1����
  int r1_vol;                      //�Լ�1���
  int r2_type;                     //�Լ�2����
  int r2_vol;                      //�Լ�2���
  int max_time;                    //�����ʱ��
}TestMethodInfoStruct;


//������ʱ���ṹ�����Ͷ���
typedef struct _SoftTimerStruct
{
	char enable;													//������ʱ��ʹ�ܱ�־λ��1-- ʹ��  2--ʧ��
	int  now;															//��ǰ�Ѽ�ʱ��  ʱ��=now*ʱ��
	int  set;															//�趨ʱ��			ʱ��=set*ʱ��
}SoftTimerStruct;

//���нṹ��
typedef struct _QueueStruct
{
  int front;                            //����
  int rear;                             //��β
  int max_size;                         //���д�С
  int *data;                            //����
}QueueStruct;

/*************External variable declaration************/
extern MNTC_CanProtocolTypeDef CanCom;                          //CANͨ�Žṹ��
extern DW_Struct  LCD;	                                        //��ʾ���ṹ��								
extern int MaxRackNum;                                          //��ǰ�������������Ĭ��2�������µ�������+1
extern SampleInfoStruct Sample[];                               //������Ϣ�����100������
extern SampleInfoStruct EditSample[];       
extern ReagentInfoStruct Reagent[];                             //�Լ���Ϣ�����19���Լ�
extern int CupSeat[];                                           //����λ��Ϣ����0--û�б�����1--�б�
extern int RunState;                                            //�豸����״̬��0--׼����1--������
extern TestMethodInfoStruct TestMethod[];                       //���Է����洢��[0]��ʹ��,PT_TEST,APTT_TEST....
extern SoftTimerStruct DetectDelay[];                           //��������ʱ
extern int DetectSeatState[]; 
extern CalibrationStruct Calibration[4];                        //����ṹ��     
extern CalibrationStruct EditCalibration;                       //�༭�еĶ�������

extern QueueStruct Err;                                         //�������
extern BarcodeTypeDef Barcode;                                  //����ṹ��
extern ReagentInfoStruct EditReagent;                           //�༭�е��Լ���Ϣ����

extern float WashPosBase[];                                     //��ϴλ����ַ
extern float SamplePosBase[];                                   //�����ܻ���ַ����һ������λ����
extern float CupPosBase[];                                      //����λ����ַ�����Ͻǵ�һ������λ
extern float ReagentPosBase[];                                  //�Լ�λ����ַ����һ���Լ�λ����
extern float DetectPosBase[];                                   //���λ����ַ
extern float DiscardPosBase[];                                  //����λ����ַ

extern AxisStruct Axis[4];
extern int Pump[2];																						 //�䶯���˶�״̬ 0-- δ��� 1-- ���
extern int NowPageNum;                                         //��ǰҳ���ַ
extern int ReturnPagNum;                                       //���ص�ҳ���ַ

extern int TestCnt;
extern int Hour,Minute,Second;
extern int LabviewFlag;
extern int LabviewBuf[20];

extern int FirstColNum;//�����ʾ�б����һ���
extern int FirstRowNum;//�����ʾ�б����»���

extern int Temp1;
extern int Temp2;

/*****************Function declaration**************/
//��Ϲ���ʵ��
void DetectProcess(void);
void TestMethodInit(void);                                        //���Է�����ʼ��
void SeatInfoInit(void);                                          //����λ��Ϣ��ʼ��
void GetSample2Detect(int sample_cnt,int item_cnt,int detect_cnt);//ȡָ����������ָ�����λ����ָ�������Ŀ
void AddR1ToSample(int sample_cnt,int item_cnt);                  //��ָ��������ָ����Ŀ�������Ŀ��ʹ�õ�R1
void AddR2ToSample(int sample_cnt,int item_cnt);                  //��ָ��������ָ����Ŀ�������Ŀ��ʹ�õ�R2
void WashNeedle(void);                                            //��ϴ������
void DiscardCup(int detect_num);                                  //��ָ�����λ����
void JudgeDiscardCup(void);                                       //��������������һ�������жϣ�ר��

//�û��Զ�������ʵ��
void CAN_Mix(int time);                                               //����
void CAN_StartTest(int test_type,int detect_num);                     //֪ͨAD������һ����
void CAN_SetTestMethod(int test_type,TestMethodInfoStruct *method);   //���Է�������
void CAN_Hello(int board_id);                                         //����
void CAN_SelfTest(int board_id);                                      //�Լ�
void PID_Decode(MNTC_CanProtocolTypeDef *buf);

//��������ʵ��
void SampleInfoInit(void);                                        //��ʼ������λ��Ϣ
void GetSample(int rack_num,int sample_num,int volume);           //�˶���ָ������λ��ȡһ��������
void RunAndAddSample(int vol,int detect_cnt);                     //���ƶ�������ǰץȡ�Ŀձ�������
void GetCupFromEmpty(int cup_num);                                //�˶���ָ������λ��ץȡһ���ձ�
void PutCupFromEmpty(int cup_num);																//�˶���ָ������λ������һ����Ӧ��
void GetReagent(int reagent_num,int volume);                      //�˶���ָ���Լ�λ��ȡһ�����Լ�
void GetCupFromDetect(int detect_num);                            //�Ӽ��λ�б�
void PutCup2Detect(int detect_num);                               //���ñ��ӵ����λ
void PutReagent2Detect(int detect_num,int volume);                //����λ���Լ�

void WhileDelay_ms(int ms);                                       //whileѭ��ʵ�ֵķǾ�ȷ��ʱ
void CAN_Decode(MNTC_CanProtocolTypeDef *buf);                    //CANͨ�Ž���
void ADC_Decode(MNTC_CanProtocolTypeDef *buf);                    //ADC����ص�CAN���ݽ����Ӻ���
void LabviewDecod(void);
void ScanerDecode(BarcodeTypeDef *buf);
void AxisRun2Pos(float x,float y,float z);                        //�������е�ָ��λ��
void AxisRun2Zero(void);                                          //���ᾫȷ����
void AxisSetSpeed(int x_speed,int y_speed,int z_speed);           //�����ٶ�����
void PlungerDraw(int vol,int run_time,int delay_time);            //�����ó�ȡ
void PlungerPush(int vol,int run_time,int delay_time);            //������ע��
void Plunger2Zero(void);                                          //�����û���
void PlungerWash(int vol,int cnt,int delay_time);                 //�����ó�ϴ

void GetSamplePos(float *x_pos,float *y_pos,int rack_num,int sample_num);//�������ָ������λ����
void GetCupPos(float *x_pos,float *y_pos,int cup_num);                   //�������ָ������λ����
void GetReagnetPos(float *x_pos,float *y_pos,int reagent_num);           //�������ָ���Լ�λ����
void GetDetectPos(float *x_pos,float *y_pos,int detect_num);             //�������ָ�����λ����
void TIM_TIM5_Configuration(void);                                       //1S�����ж�
void TIM_TIM5_Open(void);                                                //����ʱ��                                      
void TIM_TIM5_Closed(void);                                              //�ض�ʱ��
void LED_Init(void);                                                     //LED��ʼ��
void StartSoftTimer(SoftTimerStruct *timer,int t);                       //����������ʱ�����Ƚ������� 
void ClearSoftTimer(SoftTimerStruct *timer);                             //����������ʱ��

void CreatQueue(QueueStruct *que,int max_size);                          //����ѭ������
int FullQueue(QueueStruct *que);                                         //����������1--���� 0--δ��
int EmptyQueue(QueueStruct *que);                                        //ѭ�������пգ�1--�� 0--�ǿ�
int InQueue(QueueStruct *que,int data);                                  //ѭ��������ӣ�1--��ӳɹ���0--����
int OutQueue(QueueStruct *que,int *data);                                //ѭ�����г��ӣ�1--���ӳɹ���0--����Ϊ��
void DiscardCup1(int detect_num,int cup_num);


void TesttProcess(void);
void PT_Test(int sample_num,int item_num);
void APTT_Test(int sample_num,int item_num);
void TT_Test(int sample_num,int item_num);
void FIB_Test(int sample_num,int item_num);
void AT3_Test(int sample_num,int item_num);

 #endif
/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/