/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : 17003UI.c
  * Version     : V0.1
  * Description : 17003血凝项目人机交互界面控制程序
  * Date        : 2017年10月19日
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
SampleInfoStruct EditSample[SAMPLE_NUM];                           //编辑中样本暂存
TestItemInfoStruct EditTestList[MAX_TEST_LIST_NUM];                //已选项目列表
ReagentInfoStruct EditReagent;                                     //编辑中的试剂信息缓存

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
int FirstColNum=0;//结果显示列表左右滑动
int FirstRowNum=0;//结果显示列表上下滑动

/**************************************************************************************
  * name        : void LCD_Decode(DW_Struct *lcd)
  * Function    : 显示屏协议解码函数
  * Parameters  : Null
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_Decode(DW_Struct *lcd)
{
  int addr= 0;																									  	//控件地址
	int value  = 0;																										//控件值
  int i;

	if(lcd->rx_buf[3] == 0X83)																				//读数据存储器
	{
		addr= lcd->rx_buf[4]<<8 | lcd->rx_buf[5];										    //控件地址，两个字节
		value	 = lcd->rx_buf[7]<<8 | lcd->rx_buf[8];										//控件值，两个字节
		
/**********样本设置********************************************************************/
    LCD_SampleEdit(addr,value);
/**********供杯位设置*************************************************************/
    if(addr>= 0X1500 && addr<=(0X1500+90))
    {
      LCD_CupSeatEdit(addr,value); 
    }
   	
/**********试剂位设置*************************************************************/
    LCD_ReagentEdit(addr,value);

/**********结果界面编辑*************************************************************/
		LCD_ResultEdit(addr,value);

/**********反应曲线*************************************************************/
    LCD_TestCurveCtrl(addr,value);

/**********其他*************************************************************/    
    if(addr == 0x0001)                                              //开始按钮
    {
      DW_JumpPage(&LCD,12);                                         //跳转至主界面
      RunState= value;
      for(i=0; i< ROW_NUM; i++)
        LCD_RefreshResultTabe(i);
    }
  }
	else if(lcd->rx_buf[3] == 0x81)																		//读寄存器
	{
		if(lcd->rx_buf[4] == 0x03)																			//当前页面地址
			NowPageNum= (lcd->rx_buf[6]<<8 | lcd->rx_buf[7]);							//两个字节
	} 
}

/**************************************************************************************
  * name        : void LCD_SampleEdit(int addr,int value)
  * Function    : 编辑样本位
  * Parameters  : int addr-- 控件地址
                  int value-- 控件值
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_SampleEdit(int addr,int value)
{
  int sample_cnt,item_cnt;
  int crnt_sample_start;                                                //当前样本的起始号，可能存在已经使用2个以上样本架情况
  
  crnt_sample_start= (MaxRackNum - SAMPLE_RACK_NUM)*SINGLE_RACK_NUM;    //计算起始样本号
  
  if(addr >= 0X1100 &&  addr<= 0X1111)                                  //样本位被点击
  {
//    if(RunState == 0 && NowPageNum != 2)
    if(NowPageNum != 2)
    {
      DW_JumpPage(&LCD,2);                                              //跳转到样本设置界面
      NowPageNum=2;                                                     //记录当前页面地址
      
      //将Sample[]中的信息转存到编辑缓冲区EditSample[]中，只是当前在机的所有样本
      for(sample_cnt= 0; sample_cnt < SAMPLE_NUM; sample_cnt++)         //轮询当前的所有样本
      {
        EditSample[sample_cnt]= Sample[sample_cnt+ crnt_sample_start];
      }
    }
    
    if(EditSample[addr-0x1100].state == SAMPLE_GREEN ||                         //该样本已经被编辑过
       EditSample[addr-0x1100].state == SAMPLE_GRAY)                            //或该样本没被编辑过
    {
      EditSample[addr-0x1100].state= SAMPLE_YELLOW;                             //切换为选定状态  
    }
    else 
    {
      EditSample[addr-0x1100].state= SAMPLE_GRAY;                               //切换为未选定状态  
      for(item_cnt=0; item_cnt < MAX_TEST_LIST_NUM; item_cnt++)                 //清空项目列表
      {
        EditSample[addr-0x1100].item_list[item_cnt].dilution= 0.0;              
        EditSample[addr-0x1100].item_list[item_cnt].type = 0;
      }
    }
    
    memcpy(EditTestList,EditSample[addr-0x1100].item_list,sizeof(EditSample[addr-0x1100].item_list));//拷贝该项目信息到已选项目列表缓冲区
    for(item_cnt=0; EditTestList[item_cnt].type != 0; item_cnt++)               //更新已选项目列表缓冲区所使用的计数
      EditTestListNum++;
    LCD_RefreshEditTestList();                                                  //刷新已选列表缓冲区
    LCD_RefreshEditSampleSeat();                                                //刷新样本位显示
    DW_WriteStr(&LCD,0X0100,EditSample[addr-0x1100].sample_code);               //显示当前选定样本的样本号     
  }
  else if(addr >= 0X0104 && addr <= 0X010A)                                     //项目列表点击输入
  {
    EditTestList[EditTestListNum].type= addr - 0X0104 + PT_TEST;                //记录已选项目
    EditTestListNum++;
    LCD_RefreshEditTestList(); 
  }
  else if(addr == 0X011C)                                                       //样本编辑界面保存按键
  { 
    for(sample_cnt=0;sample_cnt<SAMPLE_NUM;sample_cnt++)
    {
      if(EditSample[sample_cnt].state == SAMPLE_YELLOW)                         //所有选定状态的样本位
      {
        for(item_cnt=0; item_cnt<MAX_TEST_LIST_NUM;item_cnt++)                  //这里循环12次是解决一下情况，第一次选了8项，第二次选了1项，则第二次不仅要保存第一项还得清空另七项
        {
          EditSample[sample_cnt].item_list[item_cnt]= EditTestList[item_cnt];   //转存已选项目列表内容至所有选定状态的样本
        }
        EditSample[sample_cnt].state= SAMPLE_GREEN;                             //转换成绿色--设置完成
        Sample[sample_cnt+crnt_sample_start]= EditSample[sample_cnt];
      }
    }
    
    for(item_cnt=0; item_cnt<MAX_TEST_LIST_NUM;item_cnt++)                      //清空已选项目列表
    {
      EditTestList[item_cnt].dilution=0.0;                                      //清空测试项目
      EditTestList[item_cnt].type=0;
      EditTestListNum=0;                                                        //清空计数
    }
    
    LCD_RefreshEditSampleSeat();                                                //刷新样本位显示
    LCD_RefreshEditTestList();                                                  //刷新已选项目列表
  }//end if按下保存键
  else if(addr == 0X011E)                                                       //返回按键
  {
    DW_JumpPage(&LCD,1);                                                        //跳转页面
    NowPageNum=1;
  }
  else if(addr >= 0x013F && addr <= 0x01EF)                                     //点击已选项目列表中区域
  {
    LCD_DeletTestList(addr); 
    LCD_RefreshEditTestList();                                                  //刷新已选项目列表
  }
  else if(addr == 0x0119)                                                       //左架全选
  {
    for(sample_cnt=0; sample_cnt < SINGLE_RACK_NUM; sample_cnt++)
    {
      if(EditSample[sample_cnt].state != SAMPLE_GREEN)                          //选定左架所有未编辑保存样本
      {
        EditSample[sample_cnt].state= SAMPLE_YELLOW;              
      }
    }
    LCD_RefreshEditSampleSeat();                                                //刷新样本位显示
  }
  else if(addr == 0x011A)                                                       //右架全选
  {
    for(sample_cnt=SINGLE_RACK_NUM; sample_cnt < SAMPLE_NUM; sample_cnt++)
    {
      if(EditSample[sample_cnt].state != SAMPLE_GREEN)                          //选定左架所有未编辑保存样本
      {
        EditSample[sample_cnt].state= SAMPLE_YELLOW;              
      }
    }
    LCD_RefreshEditSampleSeat();                                                //刷新样本位显示
  }
  else if(addr == 0x011B)                                                       //清空已选
  {
    for(item_cnt=0; item_cnt<MAX_TEST_LIST_NUM;item_cnt++)                      //清空已选项目列表
    {
      EditTestList[item_cnt].dilution=0.0;                                      //清空测试项目
      EditTestList[item_cnt].type=0;
      EditTestListNum=0;                                                        //清空计数
    }
    LCD_RefreshEditTestList();                                                  //刷新已选项目列表
  }
  else if(addr == 0x011D)                                                       //取消
  {
    for(sample_cnt=0; sample_cnt < SAMPLE_NUM; sample_cnt++)
    {
      if(EditSample[sample_cnt].state == SAMPLE_YELLOW)                         //所有选定项取消选定
      {
        EditSample[sample_cnt].state= SAMPLE_GRAY;
      }
    }
    
    for(item_cnt=0; item_cnt<MAX_TEST_LIST_NUM;item_cnt++)                      //清空已选项目列表
    {
      EditTestList[item_cnt].dilution=0.0;                                      //清空测试项目
      EditTestList[item_cnt].type=0;
      EditTestListNum=0;                                                        //清空计数
    }
    LCD_RefreshEditTestList();                                                  //刷新已选项目列表
    LCD_RefreshEditSampleSeat();                                                //刷新样本位显示
  }
  
}

/**************************************************************************************
  * name        : void LCD_RefreshEditSampleSeat(void)
  * Function    : 根据EditSample[]刷新样本位
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
  int addr_increment;                                                        //地址增量

  for(sample_cnt=0; sample_cnt < SAMPLE_NUM; sample_cnt++)                   //轮询编辑缓冲区中的所有样本
  {
    rack_num= EditSample[sample_cnt].rack_num;
    seat_num= EditSample[sample_cnt].seat_num;
    addr_increment= (rack_num-1) * SINGLE_RACK_NUM + (seat_num-1);
    DW_WriteData(&LCD,0x1100+addr_increment,EditSample[sample_cnt].state);   //刷新样本位图标
    seat_str[0]= '0' + EditSample[sample_cnt].rack_num/10;                   //提取架号十位
    seat_str[1]= '0' + EditSample[sample_cnt].rack_num%10;                   //提取架号个位
    seat_str[2]= '-'; 
    seat_str[3]= '0';                                                        //计算样本位号
    seat_str[4]= '0'+EditSample[sample_cnt].seat_num;
    seat_str[5]= '\0';
    addr_increment= 0x05*addr_increment;
    DW_WriteStr(&LCD,0x1200+addr_increment,seat_str);
  }
}

/**************************************************************************************
  * name        : void LCD_RefreshSampleSeat(int rack_num,int seat_num)
  * Function    : 根据Sample[]刷新某一指定的样本位显示
  * Parameters  : int rack_num-- 指定样本架
                  int seat_num-- 指定样本号
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
char seat_str[5];
void LCD_RefreshSampleSeat(int rack_num,int seat_num)
{
  
  int crnt_sample_start;                                                 //当前样本的起始号，可能存在已经使用2个以上样本架情况
  int sample_cnt;
  int addr_increment;                                                    //地址增量
  
  crnt_sample_start= (MaxRackNum - SAMPLE_RACK_NUM)*SINGLE_RACK_NUM;     //计算起始样本号
  
  for(sample_cnt= crnt_sample_start;sample_cnt < crnt_sample_start + SAMPLE_NUM;sample_cnt++)//轮询当前所有的样本
  {
    if(Sample[sample_cnt].rack_num == rack_num &&                        //寻找当前样本中位于指定位置的样本 
       Sample[sample_cnt].seat_num == seat_num)
    {
      addr_increment= (rack_num-1) * SINGLE_RACK_NUM + (seat_num-1);
      DW_WriteData(&LCD,0x1100+addr_increment,Sample[sample_cnt].state); //刷新指定样本位图标
      seat_str[0]= '0' + Sample[sample_cnt].rack_num;                 //提取架号
      seat_str[1]= '-'; 
      seat_str[2]= '0';                                                  //计算样本位号
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
  * Function    : 刷新已选项目列表显示
  * Parameters  : NULL
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_RefreshEditTestList(void)
{
  int i;
  
  for(i=0; i < MAX_TEST_LIST_NUM; i++)                                    //刷新已选项目列表文本显示
  {
    switch(EditTestList[i].type)
    {
      case 0        : DW_WriteStr(&LCD,0x023F+i*0x10,"      ");break;     //清空文本显示
      case PT_TEST  : DW_WriteStr(&LCD,0x023F+i*0x10,"PT    ");break;     //对应文本控件显示已选项目为PT
      case APTT_TEST: DW_WriteStr(&LCD,0x023F+i*0x10,"APTT  ");break;     //对应文本控件显示已选项目为APTT
      case TT_TEST  : DW_WriteStr(&LCD,0x023F+i*0x10,"TT    ");break;     //对应文本控件显示已选项目为TT
      case FIB_TEST : DW_WriteStr(&LCD,0x023F+i*0x10,"FIB   ");break;     //对应文本控件显示已选项目为FIB
      case AT3_TEST : DW_WriteStr(&LCD,0x023F+i*0x10,"AT3   ");break;     //对应文本控件显示已选项目为AT3
      case FDP_TEST : DW_WriteStr(&LCD,0x023F+i*0x10,"FDP   ");break;     //对应文本控件显示已选项目为FDP
      case DD_TEST  : DW_WriteStr(&LCD,0x023F+i*0x10,"DD    ");break;     //对应文本控件显示已选项目为DD
      default:break;
    }
  }
}

/**************************************************************************************
  * name        : void LCD_DeletTestList(int addr)
  * Function    : 从已选项目列表中删除一个选项
  * Parameters  : int addr-- 点击删除的控件地址
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_DeletTestList(int addr)
{
  int item_cnt;
  int delet_item_num= (addr - 0X013F)/0X0010;                    //被删除的项目编号，从0开始计算
  if(EditTestListNum != 0)                                       //已选项目列表中有已选项目
  {
    if(delet_item_num != MAX_TEST_LIST_NUM-1)                    //不是最后一项,若是最后一项会导致数组越界
    {
      for(item_cnt= delet_item_num; item_cnt < MAX_TEST_LIST_NUM-1; item_cnt++)
      {
        EditTestList[item_cnt].type    = EditTestList[item_cnt+1].type;
        EditTestList[item_cnt].dilution= EditTestList[item_cnt+1].dilution;
      }
    }
    EditTestList[MAX_TEST_LIST_NUM-1].type    = 0;               //不论是否删除的是最后一项，最后一项都补空
    EditTestList[MAX_TEST_LIST_NUM-1].dilution= 0.0;   
    
    EditTestListNum--;                                           //计数相应处理
  }
}
/**************************************************************************************
  * name        : void LCD_CupSeatEdit(int addr,int value)
  * Function    : 供杯位编辑
  * Parameters  : int addr-- 最近一次输入的控件的地址
                  int value-- 最近一次输入的控件的值
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_CupSeatEdit(int addr,int value)
{
  int i;
  int head_cup=-1,end_cup=-1;
  
  CupSeat[addr-0X1500]= value;                                            //记录输入的信息
  
  for(i=0;i<90 && (head_cup == -1 || end_cup == -1);i++)                  //首尾一起查询节约时间
  {
    if(CupSeat[i] == CUP_GREEN && head_cup == -1)
      head_cup= i;
    
    if(CupSeat[89-i] == CUP_GREEN && end_cup == -1)
      end_cup= 89-i;
  }
  
  if(value == CUP_GRAY)                                                    //如果是取消选中
  {
    end_cup= addr-0X1500-1;                                                //该控件之前的一个控件为最后一个绿色
  }
  
  for(i=0;i<CUP_NUM;i++)                                                   //重新设置所有供杯位状态
  {
    if(i>= head_cup && i<= end_cup)
      CupSeat[i]= CUP_GREEN;
    else 
      CupSeat[i]= CUP_GRAY;
  }
  
  for(i=0;i<CUP_NUM;i++)                                                        //刷新所有杯子显示
    LCD_RefreshCupSeat(i);

}
/**************************************************************************************
  * name        : void LCD_RefreshCupSeat(int num)
  * Function    : 根据CupSeat[]中的信息刷新一个指定供杯位的显示
  * Parameters  : int num-- 指定供杯位
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
  * Function    : 试剂信息编辑
  * Parameters  : int addr-- 最近一次操作的控件的地址
                  int value-- 最近一次操作的控件的值
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_ReagentEdit(int addr,int value) 
{
  static int edt_rgnt_num;                                          //edit reagent seat编辑中的试剂位,计数从0开始，0--标号为1的试剂位，便于数组操作
  static int edt_rgnt_flag;                                         //1-- 正在编辑试剂信息，0-- 没有编辑试剂信息

  if(addr>=0X1300 && addr<=0X1312)                                  //试剂位点击
  {
    edt_rgnt_num= addr-0X1300;                                      //计算试剂位编号
    memcpy(&EditReagent,&Reagent[edt_rgnt_num],sizeof(EditReagent));//内存赋值
    
    DW_WriteData(&LCD,0x0300,edt_rgnt_num+1);                       //显示试剂位
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
    edt_rgnt_flag= 1;                                             //开始编辑试剂信息
  }
  
  if(edt_rgnt_flag == 1)
  {
    if(addr == 0x0301)                                            //试剂类型选择
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
        case 9 : DW_WriteStr(&LCD,0x0310,"肝素   ");EditReagent.type= HEPARIN_REAGENT;break;
        case 10: DW_WriteStr(&LCD,0x0310,"FDP-R1 ");EditReagent.type= FDP_R1_REAGENT; break;
        case 11: DW_WriteStr(&LCD,0x0310,"FDP-R2 ");EditReagent.type= FDP_R2_REAGENT; break;
        case 12: DW_WriteStr(&LCD,0x0310,"DD-R1  ");EditReagent.type= DD_R1_REAGENT;  break; 
        case 13: DW_WriteStr(&LCD,0x0310,"DD-R2  ");EditReagent.type= DD_R2_REAGENT;  break;   
        default:break;
      }
    }
    else if(addr == 0x0302)                                               //单瓶试剂余量
    {
      EditReagent.single_cnt= value;
    }
    else if(addr == 0x030A)                                               //保存/取消
    {
      if(value == 1)                                                      //保存
      {
        memcpy(&Reagent[edt_rgnt_num],&EditReagent,sizeof(EditReagent));  //内存复制
      }
      else                                                                //取消
      {
        
      }
      DW_JumpPage(&LCD,1);                                                //跳转至主界面
      edt_rgnt_flag=0;                                                    //结束编辑
      LCD_RefreshReagentSeat(edt_rgnt_num);
     }//end if按下保存或返回键
    }//end 结束试剂编辑
}


/**************************************************************************************
  * name        : void LCD_ResultEdit(int addr,int value) 
  * Function    : 结果界面编辑
  * Parameters  : int addr-- 最近一次操作的控件的地址
                  int value-- 最近一次操作的控件的值
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
  * Function    : 根据Reagent[]中的信息刷新一个指定试剂位的显示
  * Parameters  : int num-- 指定试剂位
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_RefreshReagentSeat(int num)
{
  if(Reagent[num].type != 0)                                     //该试剂位已经被定义
  {
    if(Reagent[num].single_cnt >= REAGENT_ENOUGH)
      DW_WriteData(&LCD,0x1300+num,REAGENT_GREEN);               //显示绿色
    else if(Reagent[num].single_cnt >= REAGENT_ALARM)
      DW_WriteData(&LCD,0x1300+num,REAGENT_YELLOW);              //显示黄色
    else 
      DW_WriteData(&LCD,0x1300+num,REAGENT_RED);                 //显示红色
  }
  else                                                           //该试剂位没有定义试剂类型
    DW_WriteData(&LCD,0x1300+num,REAGENT_GRAY);                  //显示灰色
  
  switch(Reagent[num].type)                                      //显示试剂位文本
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
  * Function    : 开机动画，结束后跳转至测试界面
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
  
  DW_JumpPage(lcd,0);                             //跳转至启动界面
  for(addr=0x1000;addr<= 0x1017;addr++)
  {
    DW_WriteData(lcd,addr,0);                     //对应进度条块置灰
  }
  
  for(addr=0x1000;addr<= 0x1017;addr++)
  {
    DW_WriteData(lcd,addr,1);                     //对应进度条块置绿
    WhileDelay_ms(200);
  }
  
  DW_JumpPage(lcd,1);                             //跳转至测试界面
  
  for(i=1;i<=MaxRackNum;i++)
  {
    for(j=1 ;j <= SINGLE_RACK_NUM; j++)
      LCD_RefreshSampleSeat(i,j);                 //显示所有的样本状态
  }
  
  for(i=0 ;i < REAGENT_NUM; i++)                  //根据Reagent[]中的信息刷新试剂位显示
    LCD_RefreshReagentSeat(i);
  
  for(i=0;i<CUP_NUM;i++)                          //刷新所有杯子显示
    LCD_RefreshCupSeat(i);

  
  LCD_RefreshResultName();
}

/**************************************************************************************
  * name        : void LCD_RefreshResultName(void)
  * Function    : 刷新检测结果列表的结果名称显示
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
  for(cnt=0; cnt < COLUMN_NUM; cnt++)                       //所有列的结果名称都刷新
  {
    addr= 0x0600 + 0x0010*cnt;                              //计算显示空间地址
    DW_WriteStr(&LCD,addr,ResultName[FirstColNum+cnt]);     //显示名称   
  }
}

/**************************************************************************************
  * name        : void LCD_RefreshResultTabe(int row)
  * Function    : 根据Sample[]中的内容刷新指定的某一行的显示
  * Parameters  : int row-- 指定行，从0开始计数
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

  addr= 0x2000+ row*0x0010*(COLUMN_NUM+1);                    //样本号
  DW_WriteStr(&LCD,addr,Sample[sample_cnt].sample_code);
  for(cnt=1; cnt < COLUMN_NUM+1; cnt++)
  {
    addr= 0x2000+ row*0x0010*(COLUMN_NUM+1) + cnt*0x0010;
    DW_WriteData(&LCD,addr,Sample[sample_cnt].result[FirstColNum+cnt-1]*10);
  } 
}

/**************************************************************************************
  * name        : void LCD_TestCurveCtrl(int addr,int value)
  * Function    : 反应曲线相关的控制
  * Parameters  : int addr-- 控件地址
                  int value-- 控件值
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_TestCurveCtrl(int addr,int value)
{
  static int sample_cnt;
  
  if(addr >= 0x0552 && addr <= 0x055C)                        //某一样本号被点击
  {
    sample_cnt= addr-0x0552+FirstRowNum;                      //计算该控件对应的样本
    if(Sample[sample_cnt].item_list[0].type != 0)             //对应的样本有效
    {
      DW_JumpPage(&LCD,13);
      NowPageNum= 13;
      ReturnPagNum= 12;                                       //点击返回后返回的页面
      LCD_RefreshTestCurve(sample_cnt,Sample[sample_cnt].item_list[0].type);//显示第一个曲线
    }
  }
  else if(addr == 0X055D)                                     //返回按键
  {
    DW_JumpPage(&LCD,12);
    NowPageNum= 12;
  }
  else if(addr == 0X056A)                                     //反应类型选择
  {
    LCD_RefreshTestCurve(sample_cnt,value);                   //显示一条曲线
  }
}

/**************************************************************************************
  * name        : void LCD_RefreshTestCurve(int sample_cnt,int test_type)
  * Function    : 刷新反应曲线及其相关的显示
  * Parameters  : int sample_cnt-- 样本
                  int test_type-- 测试类型
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_RefreshTestCurve(int sample_cnt,int test_type)
{
  int item_cnt=0;
  char curve_name[12];
  
  for(item_cnt=0;Sample[sample_cnt].item_list[item_cnt].type != 0;item_cnt++)             //轮询测试列表中的所有项目
  {
    if(Sample[sample_cnt].item_list[item_cnt].type == test_type)
    {
      
      switch(test_type)
      {
//        strncpy(curve_name,Sample[sample_cnt].sample_code,12);                            //复制样本号
        case PT_TEST:
        {
          strcat(curve_name,"    PT    ");
          DW_WriteStr(&LCD,0X0560,curve_name);                                            //显示曲线名称
          DW_WriteStr(&LCD,0X056B,"PT  ");   
          break;
        }
        case APTT_TEST:
        {
          strcat(curve_name,"    APTT  ");
          DW_WriteStr(&LCD,0X0560,curve_name);                                            //显示曲线名称
          DW_WriteStr(&LCD,0X056B,"APTT");   
          break;
        }
        case TT_TEST:
        {
          strcat(curve_name,"    TT    ");
          DW_WriteStr(&LCD,0X0560,curve_name);                                            //显示曲线名称
          DW_WriteStr(&LCD,0X056B,"TT  ");   
          break;
        }
        case FIB_TEST:
        {
          strcat(curve_name,"    FIB   ");
          DW_WriteStr(&LCD,0X0560,curve_name);                                            //显示曲线名称
          DW_WriteStr(&LCD,0X056B,"FIB ");   
          break;
        }
        case AT3_TEST:
        {
          strcat(curve_name,"    AT3   ");
          DW_WriteStr(&LCD,0X0560,curve_name);                                            //显示曲线名称
          DW_WriteStr(&LCD,0X056B,"AT3 ");   
          break;
        }
        case FDP_TEST:
        {
          strcat(curve_name,"    FDP   ");
          DW_WriteStr(&LCD,0X0560,curve_name);                                            //显示曲线名称
          DW_WriteStr(&LCD,0X056B,"FDP ");   
          break;
        }
        case DD_TEST:
        {
          strcat(curve_name,"    DD    ");
          DW_WriteStr(&LCD,0X0560,curve_name);                                            //显示曲线名称
          DW_WriteStr(&LCD,0X056B,"DD  ");   
          break;
        }  
        default:break;
      }
      break;                                                                              //退出循环
    }
  }
}

/**************************************************************************************
  * name        : void LCD_CalibrationCtrl(int addr,int value)
  * Function    : 定标曲线相关的控制
  * Parameters  : int addr-- 控件地址
                  int value-- 控件值
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
  * Function    : 刷新定标曲线和公式
  * Parameters  : int addr-- 控件地址
                  int value-- 控件值
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void LCD_RefreshCalibrationCurve(int curve_type)
{
  int data[100];
  float delta_x;                                                                        //曲线步长
  float xi;                                                                             //曲线初始点值
  int i;
  int num= Calibration[curve_type].num;
  
  switch(curve_type)
  {
    case 0:                                                                             //FIB定标曲线
    {
      DW_WriteStr(&LCD,0X0730,"y= 0.0975*x+0.08765   r= 0.9999");
      delta_x= (Calibration[curve_type].x[3] - Calibration[curve_type].x[0])/100.0f;    //计算步进
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
    case 1:                                                                             //AT3定标曲线
    {
      DW_WriteStr(&LCD,0X0730,"y= 0.0975*x+0.08765   r= 0.9999");
      delta_x= (Calibration[curve_type].x[3] - Calibration[curve_type].x[0])/100.0f;    //计算步进
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
    case 2:                                                                             //FDP定标曲线
    {
      DW_WriteStr(&LCD,0X0730,"y= 0.0975*x+0.08765   r= 0.9999");
      delta_x= (Calibration[curve_type].x[3] - Calibration[curve_type].x[0])/100.0f;    //计算步进
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
    case 3:                                                                             //DD定标曲线
    {
      DW_WriteStr(&LCD,0X0730,"y= 0.0975*x+0.08765   r= 0.9999");
      delta_x= (Calibration[curve_type].x[3] - Calibration[curve_type].x[0])/100.0f;    //计算步进
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


