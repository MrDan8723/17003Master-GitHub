/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : 17003UI.c
  * Version     : V0.1
  * Description : 17003Ѫ����Ŀ�˻�����������Ƴ���
  * Date        : 2017��10��19��
  * Author      : XYD
  * Check       :  
*******************************************************************************/



/*********************File include*********************/
#include "17003UI.h"  
#include "SysWork.h"
#include "DW_Agreement.h"
#include <string.h>
/******************Macro definition********************/

/*************External variable declaration************/

/***************Global variable definition*************/
SampleInfoStruct EditSample[SAMPLE_NUM];                           //�༭�������ݴ�
TestItemInfoStruct EditTestList[MAX_TEST_LIST_NUM];                //��ѡ��Ŀ�б�
ReagentInfoStruct EditReagent;                                     //�༭�е��Լ���Ϣ����

int EditTestListNum=0;            
char ResultName[RESULT_NUM][RESULT_NAME_BYTE]={"  PT(S) ",
                                               " PT(INR)",
                                               " PT(%)  ",
                                               " APTT(S)",
                                               " TT(S)  ",
                                               " FIB(S) ",
                                               "FIB(g/L)",
                                               " AT3(OD)",
                                               " AT3(%) ",
                                               " FDP(OD)",
                                               "FDP(g/L)",
                                               " DD(OD) ",
                                               " DD(g/L)"};
int FirstColNum=0;//�����ʾ�б����һ���
int FirstRowNum=0;//�����ʾ�б����»���

/**************************************************************************************
  * name        : void LCD_Decode(DW_Struct *lcd)
  * Function    : ��ʾ��Э����뺯��
  * Parameters  : Null
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_Decode(DW_Struct *lcd)
{
  int addr= 0;																									  	//�ؼ���ַ
	int value  = 0;																										//�ؼ�ֵ
  int i;

	if(lcd->rx_buf[3] == 0X83)																				//�����ݴ洢��
	{
		addr= lcd->rx_buf[4]<<8 | lcd->rx_buf[5];										    //�ؼ���ַ�������ֽ�
		value	 = lcd->rx_buf[7]<<8 | lcd->rx_buf[8];										//�ؼ�ֵ�������ֽ�
		
/**********��������********************************************************************/
    LCD_SampleEdit(addr,value);
/**********����λ����*************************************************************/
    if(addr>= 0X1500 && addr<=(0X1500+90))
    {
      LCD_CupSeatEdit(addr,value); 
    }
   	
/**********�Լ�λ����*************************************************************/
    LCD_ReagentEdit(addr,value);

/**********�������༭*************************************************************/
		LCD_ResultEdit(addr,value);

/**********��Ӧ����*************************************************************/
    LCD_TestCurveCtrl(addr,value);

/**********����*************************************************************/    
    if(addr == 0x0001)                                              //��ʼ��ť
    {
      DW_JumpPage(&LCD,12);                                         //��ת��������
      RunState= value;
      for(i=0; i< ROW_NUM; i++)
        LCD_RefreshResultTabe(i);
    }
  }
	else if(lcd->rx_buf[3] == 0x81)																		//���Ĵ���
	{
		if(lcd->rx_buf[4] == 0x03)																			//��ǰҳ���ַ
			NowPageNum= (lcd->rx_buf[6]<<8 | lcd->rx_buf[7]);							//�����ֽ�
	} 
}

/**************************************************************************************
  * name        : void LCD_SampleEdit(int addr,int value)
  * Function    : �༭����λ
  * Parameters  : int addr-- �ؼ���ַ
                  int value-- �ؼ�ֵ
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_SampleEdit(int addr,int value)
{
  int sample_cnt,item_cnt;
  int crnt_sample_start;                                                //��ǰ��������ʼ�ţ����ܴ����Ѿ�ʹ��2���������������
  
  crnt_sample_start= (MaxRackNum - SAMPLE_RACK_NUM)*SINGLE_RACK_NUM;    //������ʼ������
  
  if(addr >= 0X1100 &&  addr<= 0X1111)                                  //����λ�����
  {
//    if(RunState == 0 && NowPageNum != 2)
    if(NowPageNum != 2)
    {
      DW_JumpPage(&LCD,2);                                              //��ת���������ý���
      NowPageNum=2;                                                     //��¼��ǰҳ���ַ
      
      //��Sample[]�е���Ϣת�浽�༭������EditSample[]�У�ֻ�ǵ�ǰ�ڻ�����������
      for(sample_cnt= 0; sample_cnt < SAMPLE_NUM; sample_cnt++)         //��ѯ��ǰ����������
      {
        EditSample[sample_cnt]= Sample[sample_cnt+ crnt_sample_start];
      }
    }
    
    if(EditSample[addr-0x1100].state == SAMPLE_GREEN ||                         //�������Ѿ����༭��
       EditSample[addr-0x1100].state == SAMPLE_GRAY)                            //�������û���༭��
    {
      EditSample[addr-0x1100].state= SAMPLE_YELLOW;                             //�л�Ϊѡ��״̬  
    }
    else 
    {
      EditSample[addr-0x1100].state= SAMPLE_GRAY;                               //�л�Ϊδѡ��״̬  
      for(item_cnt=0; item_cnt < MAX_TEST_LIST_NUM; item_cnt++)                 //�����Ŀ�б�
      {
        EditSample[addr-0x1100].item_list[item_cnt].dilution= 0.0;              
        EditSample[addr-0x1100].item_list[item_cnt].type = 0;
      }
    }
    
    memcpy(EditTestList,EditSample[addr-0x1100].item_list,sizeof(EditSample[addr-0x1100].item_list));//��������Ŀ��Ϣ����ѡ��Ŀ�б�����
    for(item_cnt=0; EditTestList[item_cnt].type != 0; item_cnt++)               //������ѡ��Ŀ�б�������ʹ�õļ���
      EditTestListNum++;
    LCD_RefreshEditTestList();                                                  //ˢ����ѡ�б�����
    LCD_RefreshEditSampleSeat();                                                //ˢ������λ��ʾ
    DW_WriteStr(&LCD,0X0100,EditSample[addr-0x1100].sample_code);               //��ʾ��ǰѡ��������������     
  }
  else if(addr >= 0X0104 && addr <= 0X010A)                                     //��Ŀ�б�������
  {
    EditTestList[EditTestListNum].type= addr - 0X0104 + PT_TEST;                //��¼��ѡ��Ŀ
    EditTestListNum++;
    LCD_RefreshEditTestList(); 
  }
  else if(addr == 0X011C)                                                       //�����༭���汣�水��
  { 
    for(sample_cnt=0;sample_cnt<SAMPLE_NUM;sample_cnt++)
    {
      if(EditSample[sample_cnt].state == SAMPLE_YELLOW)                         //����ѡ��״̬������λ
      {
        for(item_cnt=0; item_cnt<MAX_TEST_LIST_NUM;item_cnt++)                  //����ѭ��12���ǽ��һ���������һ��ѡ��8��ڶ���ѡ��1���ڶ��β���Ҫ�����һ������������
        {
          EditSample[sample_cnt].item_list[item_cnt]= EditTestList[item_cnt];   //ת����ѡ��Ŀ�б�����������ѡ��״̬������
        }
        EditSample[sample_cnt].state= SAMPLE_GREEN;                             //ת������ɫ--�������
        Sample[sample_cnt+crnt_sample_start]= EditSample[sample_cnt];
      }
    }
    
    for(item_cnt=0; item_cnt<MAX_TEST_LIST_NUM;item_cnt++)                      //�����ѡ��Ŀ�б�
    {
      EditTestList[item_cnt].dilution=0.0;                                      //��ղ�����Ŀ
      EditTestList[item_cnt].type=0;
      EditTestListNum=0;                                                        //��ռ���
    }
    
    LCD_RefreshEditSampleSeat();                                                //ˢ������λ��ʾ
    LCD_RefreshEditTestList();                                                  //ˢ����ѡ��Ŀ�б�
  }//end if���±����
  else if(addr == 0X011E)                                                       //���ذ���
  {
    DW_JumpPage(&LCD,1);                                                        //��תҳ��
    NowPageNum=1;
  }
  else if(addr >= 0x013F && addr <= 0x01EF)                                     //�����ѡ��Ŀ�б�������
  {
    LCD_DeletTestList(addr); 
    LCD_RefreshEditTestList();                                                  //ˢ����ѡ��Ŀ�б�
  }
  else if(addr == 0x0119)                                                       //���ȫѡ
  {
    for(sample_cnt=0; sample_cnt < SINGLE_RACK_NUM; sample_cnt++)
    {
      if(EditSample[sample_cnt].state != SAMPLE_GREEN)                          //ѡ���������δ�༭��������
      {
        EditSample[sample_cnt].state= SAMPLE_YELLOW;              
      }
    }
    LCD_RefreshEditSampleSeat();                                                //ˢ������λ��ʾ
  }
  else if(addr == 0x011A)                                                       //�Ҽ�ȫѡ
  {
    for(sample_cnt=SINGLE_RACK_NUM; sample_cnt < SAMPLE_NUM; sample_cnt++)
    {
      if(EditSample[sample_cnt].state != SAMPLE_GREEN)                          //ѡ���������δ�༭��������
      {
        EditSample[sample_cnt].state= SAMPLE_YELLOW;              
      }
    }
    LCD_RefreshEditSampleSeat();                                                //ˢ������λ��ʾ
  }
  else if(addr == 0x011B)                                                       //�����ѡ
  {
    for(item_cnt=0; item_cnt<MAX_TEST_LIST_NUM;item_cnt++)                      //�����ѡ��Ŀ�б�
    {
      EditTestList[item_cnt].dilution=0.0;                                      //��ղ�����Ŀ
      EditTestList[item_cnt].type=0;
      EditTestListNum=0;                                                        //��ռ���
    }
    LCD_RefreshEditTestList();                                                  //ˢ����ѡ��Ŀ�б�
  }
  else if(addr == 0x011D)                                                       //ȡ��
  {
    for(sample_cnt=0; sample_cnt < SAMPLE_NUM; sample_cnt++)
    {
      if(EditSample[sample_cnt].state == SAMPLE_YELLOW)                         //����ѡ����ȡ��ѡ��
      {
        EditSample[sample_cnt].state= SAMPLE_GRAY;
      }
    }
    
    for(item_cnt=0; item_cnt<MAX_TEST_LIST_NUM;item_cnt++)                      //�����ѡ��Ŀ�б�
    {
      EditTestList[item_cnt].dilution=0.0;                                      //��ղ�����Ŀ
      EditTestList[item_cnt].type=0;
      EditTestListNum=0;                                                        //��ռ���
    }
    LCD_RefreshEditTestList();                                                  //ˢ����ѡ��Ŀ�б�
    LCD_RefreshEditSampleSeat();                                                //ˢ������λ��ʾ
  }
  
}

/**************************************************************************************
  * name        : void LCD_RefreshEditSampleSeat(void)
  * Function    : ����EditSample[]ˢ������λ
  * Parameters  : NULL
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_RefreshEditSampleSeat(void)
{
  int rack_num=1,seat_num=1;
  int sample_cnt= 0;
  char seat_str[6];
  int addr_increment;                                                        //��ַ����

  for(sample_cnt=0; sample_cnt < SAMPLE_NUM; sample_cnt++)                   //��ѯ�༭�������е���������
  {
    rack_num= EditSample[sample_cnt].rack_num;
    seat_num= EditSample[sample_cnt].seat_num;
    addr_increment= (rack_num-1) * SINGLE_RACK_NUM + (seat_num-1);
    DW_WriteData(&LCD,0x1100+addr_increment,EditSample[sample_cnt].state);   //ˢ������λͼ��
    seat_str[0]= '0' + EditSample[sample_cnt].rack_num/10;                   //��ȡ�ܺ�ʮλ
    seat_str[1]= '0' + EditSample[sample_cnt].rack_num%10;                   //��ȡ�ܺŸ�λ
    seat_str[2]= '-'; 
    seat_str[3]= '0';                                                        //��������λ��
    seat_str[4]= '0'+EditSample[sample_cnt].seat_num;
    seat_str[5]= '\0';
    addr_increment= 0x05*addr_increment;
    DW_WriteStr(&LCD,0x1200+addr_increment,seat_str);
  }
}

/**************************************************************************************
  * name        : void LCD_RefreshSampleSeat(int rack_num,int seat_num)
  * Function    : ����Sample[]ˢ��ĳһָ��������λ��ʾ
  * Parameters  : int rack_num-- ָ��������
                  int seat_num-- ָ��������
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
char seat_str[5];
void LCD_RefreshSampleSeat(int rack_num,int seat_num)
{
  
  int crnt_sample_start;                                                 //��ǰ��������ʼ�ţ����ܴ����Ѿ�ʹ��2���������������
  int sample_cnt;
  int addr_increment;                                                    //��ַ����
  
  crnt_sample_start= (MaxRackNum - SAMPLE_RACK_NUM)*SINGLE_RACK_NUM;     //������ʼ������
  
  for(sample_cnt= crnt_sample_start;sample_cnt < crnt_sample_start + SAMPLE_NUM;sample_cnt++)//��ѯ��ǰ���е�����
  {
    if(Sample[sample_cnt].rack_num == rack_num &&                        //Ѱ�ҵ�ǰ������λ��ָ��λ�õ����� 
       Sample[sample_cnt].seat_num == seat_num)
    {
      addr_increment= (rack_num-1) * SINGLE_RACK_NUM + (seat_num-1);
      DW_WriteData(&LCD,0x1100+addr_increment,Sample[sample_cnt].state); //ˢ��ָ������λͼ��
      seat_str[0]= '0' + Sample[sample_cnt].rack_num;                 //��ȡ�ܺ�
      seat_str[1]= '-'; 
      seat_str[2]= '0';                                                  //��������λ��
      seat_str[3]= '0'+Sample[sample_cnt].seat_num;
      seat_str[4]= '\0';
      addr_increment= 0x05*addr_increment;
			DW_WriteStr(&LCD,0x1200+addr_increment,"      ");
      DW_WriteStr(&LCD,0x1200+addr_increment,seat_str);
    }
  }
}

/**************************************************************************************
  * name        : void LCD_RefreshEditTestList(void)
  * Function    : ˢ����ѡ��Ŀ�б���ʾ
  * Parameters  : NULL
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_RefreshEditTestList(void)
{
  int i;
  
  for(i=0; i < MAX_TEST_LIST_NUM; i++)                                    //ˢ����ѡ��Ŀ�б��ı���ʾ
  {
    switch(EditTestList[i].type)
    {
      case 0        : DW_WriteStr(&LCD,0x023F+i*0x10,"      ");break;     //����ı���ʾ
      case PT_TEST  : DW_WriteStr(&LCD,0x023F+i*0x10,"PT    ");break;     //��Ӧ�ı��ؼ���ʾ��ѡ��ĿΪPT
      case APTT_TEST: DW_WriteStr(&LCD,0x023F+i*0x10,"APTT  ");break;     //��Ӧ�ı��ؼ���ʾ��ѡ��ĿΪAPTT
      case TT_TEST  : DW_WriteStr(&LCD,0x023F+i*0x10,"TT    ");break;     //��Ӧ�ı��ؼ���ʾ��ѡ��ĿΪTT
      case FIB_TEST : DW_WriteStr(&LCD,0x023F+i*0x10,"FIB   ");break;     //��Ӧ�ı��ؼ���ʾ��ѡ��ĿΪFIB
      case AT3_TEST : DW_WriteStr(&LCD,0x023F+i*0x10,"AT3   ");break;     //��Ӧ�ı��ؼ���ʾ��ѡ��ĿΪAT3
      case FDP_TEST : DW_WriteStr(&LCD,0x023F+i*0x10,"FDP   ");break;     //��Ӧ�ı��ؼ���ʾ��ѡ��ĿΪFDP
      case DD_TEST  : DW_WriteStr(&LCD,0x023F+i*0x10,"DD    ");break;     //��Ӧ�ı��ؼ���ʾ��ѡ��ĿΪDD
      default:break;
    }
  }
}

/**************************************************************************************
  * name        : void LCD_DeletTestList(int addr)
  * Function    : ����ѡ��Ŀ�б���ɾ��һ��ѡ��
  * Parameters  : int addr-- ���ɾ���Ŀؼ���ַ
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_DeletTestList(int addr)
{
  int item_cnt;
  int delet_item_num= (addr - 0X013F)/0X0010;                    //��ɾ������Ŀ��ţ���0��ʼ����
  if(EditTestListNum != 0)                                       //��ѡ��Ŀ�б�������ѡ��Ŀ
  {
    if(delet_item_num != MAX_TEST_LIST_NUM-1)                    //�������һ��,�������һ��ᵼ������Խ��
    {
      for(item_cnt= delet_item_num; item_cnt < MAX_TEST_LIST_NUM-1; item_cnt++)
      {
        EditTestList[item_cnt].type    = EditTestList[item_cnt+1].type;
        EditTestList[item_cnt].dilution= EditTestList[item_cnt+1].dilution;
      }
    }
    EditTestList[MAX_TEST_LIST_NUM-1].type    = 0;               //�����Ƿ�ɾ���������һ����һ�����
    EditTestList[MAX_TEST_LIST_NUM-1].dilution= 0.0;   
    
    EditTestListNum--;                                           //������Ӧ����
  }
}
/**************************************************************************************
  * name        : void LCD_CupSeatEdit(int addr,int value)
  * Function    : ����λ�༭
  * Parameters  : int addr-- ���һ������Ŀؼ��ĵ�ַ
                  int value-- ���һ������Ŀؼ���ֵ
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_CupSeatEdit(int addr,int value)
{
  int i;
  int head_cup=-1,end_cup=-1;
  
  CupSeat[addr-0X1500]= value;                                            //��¼�������Ϣ
  
  for(i=0;i<90 && (head_cup == -1 || end_cup == -1);i++)                  //��βһ���ѯ��Լʱ��
  {
    if(CupSeat[i] == CUP_GREEN && head_cup == -1)
      head_cup= i;
    
    if(CupSeat[89-i] == CUP_GREEN && end_cup == -1)
      end_cup= 89-i;
  }
  
  if(value == CUP_GRAY)                                                    //�����ȡ��ѡ��
  {
    end_cup= addr-0X1500-1;                                                //�ÿؼ�֮ǰ��һ���ؼ�Ϊ���һ����ɫ
  }
  
  for(i=0;i<CUP_NUM;i++)                                                   //�����������й���λ״̬
  {
    if(i>= head_cup && i<= end_cup)
      CupSeat[i]= CUP_GREEN;
    else 
      CupSeat[i]= CUP_GRAY;
  }
  
  for(i=0;i<CUP_NUM;i++)                                                        //ˢ�����б�����ʾ
    LCD_RefreshCupSeat(i);

}
/**************************************************************************************
  * name        : void LCD_RefreshCupSeat(int num)
  * Function    : ����CupSeat[]�е���Ϣˢ��һ��ָ������λ����ʾ
  * Parameters  : int num-- ָ������λ
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_RefreshCupSeat(int num)
{
  DW_WriteData(&LCD,0X1500+num,CupSeat[num]);
}  

/**************************************************************************************
  * name        : void LCD_ReagentEdit(int addr,int value)
  * Function    : �Լ���Ϣ�༭
  * Parameters  : int addr-- ���һ�β����Ŀؼ��ĵ�ַ
                  int value-- ���һ�β����Ŀؼ���ֵ
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_ReagentEdit(int addr,int value) 
{
  static int edt_rgnt_num;                                          //edit reagent seat�༭�е��Լ�λ,������0��ʼ��0--���Ϊ1���Լ�λ�������������
  static int edt_rgnt_flag;                                         //1-- ���ڱ༭�Լ���Ϣ��0-- û�б༭�Լ���Ϣ

  if(addr>=0X1300 && addr<=0X1312)                                  //�Լ�λ���
  {
    edt_rgnt_num= addr-0X1300;                                      //�����Լ�λ���
    memcpy(&EditReagent,&Reagent[edt_rgnt_num],sizeof(EditReagent));//�ڴ渳ֵ
    
    DW_WriteData(&LCD,0x0300,edt_rgnt_num+1);                       //��ʾ�Լ�λ
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
    edt_rgnt_flag= 1;                                             //��ʼ�༭�Լ���Ϣ
  }
  
  if(edt_rgnt_flag == 1)
  {
    if(addr == 0x0301)                                            //�Լ�����ѡ��
    {
      switch(value)
      {
        case 0 : DW_WriteStr(&LCD,0x0310,"       ");break;
        case 1 : DW_WriteStr(&LCD,0x0310,"PT     ");EditReagent.type= PT_REAGENT;     break;
        case 2 : DW_WriteStr(&LCD,0x0310,"APTT   ");EditReagent.type= APTT_REAGENT;   break;
        case 3 : DW_WriteStr(&LCD,0x0310,"CaCl2  ");EditReagent.type= CaCl2_REAGENT;  break;
        case 4 : DW_WriteStr(&LCD,0x0310,"TT     ");EditReagent.type= TT_REAGENT;     break;
        case 5 : DW_WriteStr(&LCD,0x0310,"FIB    ");EditReagent.type= FIB_REAGENT;    break;
        case 6 : DW_WriteStr(&LCD,0x0310,"OVB    ");EditReagent.type= OVB_REAGENT;    break;
        case 7 : DW_WriteStr(&LCD,0x0310,"AT3-R1 ");EditReagent.type= AT3_R1_REAGENT; break;
        case 8 : DW_WriteStr(&LCD,0x0310,"AT3-R2 ");EditReagent.type= AT3_R2_REAGENT; break;
        case 9 : DW_WriteStr(&LCD,0x0310,"����   ");EditReagent.type= HEPARIN_REAGENT;break;
        case 10: DW_WriteStr(&LCD,0x0310,"FDP-R1 ");EditReagent.type= FDP_R1_REAGENT; break;
        case 11: DW_WriteStr(&LCD,0x0310,"FDP-R2 ");EditReagent.type= FDP_R2_REAGENT; break;
        case 12: DW_WriteStr(&LCD,0x0310,"DD-R1  ");EditReagent.type= DD_R1_REAGENT;  break; 
        case 13: DW_WriteStr(&LCD,0x0310,"DD-R2  ");EditReagent.type= DD_R2_REAGENT;  break;   
        default:break;
      }
    }
    else if(addr == 0x0302)                                               //��ƿ�Լ�����
    {
      EditReagent.single_cnt= value;
    }
    else if(addr == 0x030A)                                               //����/ȡ��
    {
      if(value == 1)                                                      //����
      {
        memcpy(&Reagent[edt_rgnt_num],&EditReagent,sizeof(EditReagent));  //�ڴ渴��
      }
      else                                                                //ȡ��
      {
        
      }
      DW_JumpPage(&LCD,1);                                                //��ת��������
      edt_rgnt_flag=0;                                                    //�����༭
      LCD_RefreshReagentSeat(edt_rgnt_num);
     }//end if���±���򷵻ؼ�
    }//end �����Լ��༭
}


/**************************************************************************************
  * name        : void LCD_ResultEdit(int addr,int value) 
  * Function    : �������༭
  * Parameters  : int addr-- ���һ�β����Ŀؼ��ĵ�ַ
                  int value-- ���һ�β����Ŀؼ���ֵ
  * Returns     : Null
  * Note        : 
  * Author      : ZQChen
  * Check       : 
**************************************************************************************/
void LCD_ResultEdit(int addr,int value) 
{
	int i;
	if(addr==0x0500)
	{
		FirstColNum=value;
		LCD_RefreshResultName();
		for(i=0; i< ROW_NUM; i++)
      LCD_RefreshResultTabe(i);
		
	}
	else if(addr==0x0501)
	{
		FirstRowNum=value;
		LCD_RefreshResultName();
		for(i=0; i< ROW_NUM; i++)
      LCD_RefreshResultTabe(i);
	}
}  
/**************************************************************************************
  * name        : void LCD_RefreshReagentSeat(int num)
  * Function    : ����Reagent[]�е���Ϣˢ��һ��ָ���Լ�λ����ʾ
  * Parameters  : int num-- ָ���Լ�λ
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_RefreshReagentSeat(int num)
{
  if(Reagent[num].type != 0)                                     //���Լ�λ�Ѿ�������
  {
    if(Reagent[num].single_cnt >= REAGENT_ENOUGH)
      DW_WriteData(&LCD,0x1300+num,REAGENT_GREEN);               //��ʾ��ɫ
    else if(Reagent[num].single_cnt >= REAGENT_ALARM)
      DW_WriteData(&LCD,0x1300+num,REAGENT_YELLOW);              //��ʾ��ɫ
    else 
      DW_WriteData(&LCD,0x1300+num,REAGENT_RED);                 //��ʾ��ɫ
  }
  else                                                           //���Լ�λû�ж����Լ�����
    DW_WriteData(&LCD,0x1300+num,REAGENT_GRAY);                  //��ʾ��ɫ
  
  switch(Reagent[num].type)                                      //��ʾ�Լ�λ�ı�
  {
    case 0              : DW_WriteStr(&LCD,0x1320+num*0X10,"       ");break;
    case PT_REAGENT     : DW_WriteStr(&LCD,0x1320+num*0X10,"PT     ");break;
    case APTT_REAGENT   : DW_WriteStr(&LCD,0x1320+num*0X10,"APTT   ");break;
    case CaCl2_REAGENT  : DW_WriteStr(&LCD,0x1320+num*0X10,"CaCl2  ");break;
    case TT_REAGENT     : DW_WriteStr(&LCD,0x1320+num*0X10,"TT     ");break;
    case FIB_REAGENT    : DW_WriteStr(&LCD,0x1320+num*0X10,"FIB    ");break;
    case OVB_REAGENT    : DW_WriteStr(&LCD,0x1320+num*0X10,"OVB    ");break;
    case AT3_R1_REAGENT : DW_WriteStr(&LCD,0x1320+num*0X10,"AT3-R1 ");break;
    case AT3_R2_REAGENT : DW_WriteStr(&LCD,0x1320+num*0X10,"AT3-R2 ");break;
    case HEPARIN_REAGENT: DW_WriteStr(&LCD,0x1320+num*0X10,"FDP-R1 ");break;
    case FDP_R2_REAGENT : DW_WriteStr(&LCD,0x1320+num*0X10,"FDP-R2 ");break;
    case DD_R1_REAGENT  : DW_WriteStr(&LCD,0x1320+num*0X10,"DD-R1  ");break; 
    case DD_R2_REAGENT  : DW_WriteStr(&LCD,0x1320+num*0X10,"DD-R2  ");break;   
    default:break;
  }    
}

/**************************************************************************************
  * name        : void LCD_StartBar(DW_Struct *lcd)
  * Function    : ������������������ת�����Խ���
  * Parameters  : Null
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_StartBar(DW_Struct *lcd)
{
  int addr;
  int i,j;
  
  DW_JumpPage(lcd,0);                             //��ת����������
  for(addr=0x1000;addr<= 0x1017;addr++)
  {
    DW_WriteData(lcd,addr,0);                     //��Ӧ���������û�
  }
  
  for(addr=0x1000;addr<= 0x1017;addr++)
  {
    DW_WriteData(lcd,addr,1);                     //��Ӧ������������
    WhileDelay_ms(200);
  }
  
  DW_JumpPage(lcd,1);                             //��ת�����Խ���
  
  for(i=1;i<=MaxRackNum;i++)
  {
    for(j=1 ;j <= SINGLE_RACK_NUM; j++)
      LCD_RefreshSampleSeat(i,j);                 //��ʾ���е�����״̬
  }
  
  for(i=0 ;i < REAGENT_NUM; i++)                  //����Reagent[]�е���Ϣˢ���Լ�λ��ʾ
    LCD_RefreshReagentSeat(i);
  
  for(i=0;i<CUP_NUM;i++)                          //ˢ�����б�����ʾ
    LCD_RefreshCupSeat(i);

  
  LCD_RefreshResultName();
}

/**************************************************************************************
  * name        : void LCD_RefreshResultName(void)
  * Function    : ˢ�¼�����б�Ľ��������ʾ
  * Parameters  : Null
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_RefreshResultName(void)
{
  int cnt;
  int addr;
  for(cnt=0; cnt < COLUMN_NUM; cnt++)                       //�����еĽ�����ƶ�ˢ��
  {
    addr= 0x0600 + 0x0010*cnt;                              //������ʾ�ռ��ַ
    DW_WriteStr(&LCD,addr,ResultName[FirstColNum+cnt]);     //��ʾ����   
  }
}

/**************************************************************************************
  * name        : void LCD_RefreshResultTabe(int row)
  * Function    : ����Sample[]�е�����ˢ��ָ����ĳһ�е���ʾ
  * Parameters  : int row-- ָ���У���0��ʼ����
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_RefreshResultTabe(int row)
{
  int addr;
  int sample_cnt;
  int cnt;
  
  sample_cnt= row+FirstRowNum;

  addr= 0x2000+ row*0x0010*(COLUMN_NUM+1);                    //������
  DW_WriteStr(&LCD,addr,Sample[sample_cnt].sample_code);
  for(cnt=1; cnt < COLUMN_NUM+1; cnt++)
  {
    addr= 0x2000+ row*0x0010*(COLUMN_NUM+1) + cnt*0x0010;
    DW_WriteData(&LCD,addr,Sample[sample_cnt].result[FirstColNum+cnt-1]*10);
  } 
}

/**************************************************************************************
  * name        : void LCD_TestCurveCtrl(int addr,int value)
  * Function    : ��Ӧ������صĿ���
  * Parameters  : int addr-- �ؼ���ַ
                  int value-- �ؼ�ֵ
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_TestCurveCtrl(int addr,int value)
{
  static int sample_cnt;
  
  if(addr >= 0x0552 && addr <= 0x055C)                        //ĳһ�����ű����
  {
    sample_cnt= addr-0x0552+FirstRowNum;                      //����ÿؼ���Ӧ������
    if(Sample[sample_cnt].item_list[0].type != 0)             //��Ӧ��������Ч
    {
      DW_JumpPage(&LCD,13);
      NowPageNum= 13;
      ReturnPagNum= 12;                                       //������غ󷵻ص�ҳ��
      LCD_RefreshTestCurve(sample_cnt,Sample[sample_cnt].item_list[0].type);//��ʾ��һ������
    }
  }
  else if(addr == 0X055D)                                     //���ذ���
  {
    DW_JumpPage(&LCD,12);
    NowPageNum= 12;
  }
  else if(addr == 0X056A)                                     //��Ӧ����ѡ��
  {
    LCD_RefreshTestCurve(sample_cnt,value);                   //��ʾһ������
  }
}

/**************************************************************************************
  * name        : void LCD_RefreshTestCurve(int sample_cnt,int test_type)
  * Function    : ˢ�·�Ӧ���߼�����ص���ʾ
  * Parameters  : int sample_cnt-- ����
                  int test_type-- ��������
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_RefreshTestCurve(int sample_cnt,int test_type)
{
  int item_cnt=0;
  char curve_name[12];
  
  for(item_cnt=0;Sample[sample_cnt].item_list[item_cnt].type != 0;item_cnt++)             //��ѯ�����б��е�������Ŀ
  {
    if(Sample[sample_cnt].item_list[item_cnt].type == test_type)
    {
      
      switch(test_type)
      {
//        strncpy(curve_name,Sample[sample_cnt].sample_code,12);                            //����������
        case PT_TEST:
        {
          strcat(curve_name,"    PT    ");
          DW_WriteStr(&LCD,0X0560,curve_name);                                            //��ʾ��������
          DW_WriteStr(&LCD,0X056B,"PT  ");   
          break;
        }
        case APTT_TEST:
        {
          strcat(curve_name,"    APTT  ");
          DW_WriteStr(&LCD,0X0560,curve_name);                                            //��ʾ��������
          DW_WriteStr(&LCD,0X056B,"APTT");   
          break;
        }
        case TT_TEST:
        {
          strcat(curve_name,"    TT    ");
          DW_WriteStr(&LCD,0X0560,curve_name);                                            //��ʾ��������
          DW_WriteStr(&LCD,0X056B,"TT  ");   
          break;
        }
        case FIB_TEST:
        {
          strcat(curve_name,"    FIB   ");
          DW_WriteStr(&LCD,0X0560,curve_name);                                            //��ʾ��������
          DW_WriteStr(&LCD,0X056B,"FIB ");   
          break;
        }
        case AT3_TEST:
        {
          strcat(curve_name,"    AT3   ");
          DW_WriteStr(&LCD,0X0560,curve_name);                                            //��ʾ��������
          DW_WriteStr(&LCD,0X056B,"AT3 ");   
          break;
        }
        case FDP_TEST:
        {
          strcat(curve_name,"    FDP   ");
          DW_WriteStr(&LCD,0X0560,curve_name);                                            //��ʾ��������
          DW_WriteStr(&LCD,0X056B,"FDP ");   
          break;
        }
        case DD_TEST:
        {
          strcat(curve_name,"    DD    ");
          DW_WriteStr(&LCD,0X0560,curve_name);                                            //��ʾ��������
          DW_WriteStr(&LCD,0X056B,"DD  ");   
          break;
        }  
        default:break;
      }
      break;                                                                              //�˳�ѭ��
    }
  }
}

/**************************************************************************************
  * name        : void LCD_CalibrationCtrl(int addr,int value)
  * Function    : ����������صĿ���
  * Parameters  : int addr-- �ؼ���ַ
                  int value-- �ؼ�ֵ
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_CalibrationCtrl(int addr,int value)
{
  if(addr == 0X0720)
  {
    switch(value)
    {
      case 0:
      {
        DW_WriteStr(&LCD,0X0721,"FIB");
        break;
      }
      case 1:
      {
        DW_WriteStr(&LCD,0X0721,"AT3");
        break;
      }
      case 2:
      {
        DW_WriteStr(&LCD,0X0721,"FDP");
        break;
      }
      case 3:
      {
        DW_WriteStr(&LCD,0X0721,"DD ");
        break;
      }
      default:break;
    }
  }
  
}

/**************************************************************************************
  * name        : void LCD_RefreshCalibrationCurve(int curve_type)
  * Function    : ˢ�¶������ߺ͹�ʽ
  * Parameters  : int addr-- �ؼ���ַ
                  int value-- �ؼ�ֵ
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_RefreshCalibrationCurve(int curve_type)
{
  int data[100];
  float delta_x;                                                                        //���߲���
  float xi;                                                                             //���߳�ʼ��ֵ
  int i;
  int num= Calibration[curve_type].num;
  
  switch(curve_type)
  {
    case 0:                                                                             //FIB��������
    {
      DW_WriteStr(&LCD,0X0730,"y= 0.0975*x+0.08765   r= 0.9999");
      delta_x= (Calibration[curve_type].x[3] - Calibration[curve_type].x[0])/100.0f;    //���㲽��
      xi     = Calibration[curve_type].x[0];
      DW_CleanCurve(&LCD,0);
      for(i=0;i<100;i++)
      {
        data[i]= FIB_CalcContent(&Calibration[curve_type],xi);
        xi += delta_x;
      }
      DW_SetCurve(&LCD,0x0710,785,516,100,data[0],data[99]); 
      DW_WriteCurve(&LCD,2,data,100);
      break;
    }
    case 1:                                                                             //AT3��������
    {
      DW_WriteStr(&LCD,0X0730,"y= 0.0975*x+0.08765   r= 0.9999");
      delta_x= (Calibration[curve_type].x[3] - Calibration[curve_type].x[0])/100.0f;    //���㲽��
      xi     = Calibration[curve_type].x[0];
      DW_CleanCurve(&LCD,0);
      for(i=0;i<100;i++)
      {
        data[i]= AT3_CalcActive(&Calibration[curve_type],xi);
        xi += delta_x;
      }
      DW_SetCurve(&LCD,0x0710,785,516,100,data[0],data[99]); 
      DW_WriteCurve(&LCD,2,data,100);
      break;
    }
    case 2:                                                                             //FDP��������
    {
      DW_WriteStr(&LCD,0X0730,"y= 0.0975*x+0.08765   r= 0.9999");
      delta_x= (Calibration[curve_type].x[3] - Calibration[curve_type].x[0])/100.0f;    //���㲽��
      xi     = Calibration[curve_type].x[0];
      DW_CleanCurve(&LCD,0);
      for(i=0;i<100;i++)
      {
        data[i]= AT3_CalcActive(&Calibration[curve_type],xi);
        xi += delta_x;
      }
      DW_SetCurve(&LCD,0x0710,785,516,100,data[0],data[99]); 
      DW_WriteCurve(&LCD,2,data,100);
      break;
    }
    case 3:                                                                             //DD��������
    {
      DW_WriteStr(&LCD,0X0730,"y= 0.0975*x+0.08765   r= 0.9999");
      delta_x= (Calibration[curve_type].x[3] - Calibration[curve_type].x[0])/100.0f;    //���㲽��
      xi     = Calibration[curve_type].x[0];
      DW_CleanCurve(&LCD,0);
      for(i=0;i<100;i++)
      {
        data[i]= AT3_CalcActive(&Calibration[curve_type],xi);
        xi += delta_x;
      }
      DW_SetCurve(&LCD,0x0710,785,516,100,data[0],data[99]); 
      DW_WriteCurve(&LCD,2,data,100);
      break;
    }
  }
}
/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/


