/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : MNTC_CanProtocolMaster_V0.1.c
  * Version     : V0.1
										1、实现基于MNTC实验室V0.4版本协议的主机源码
  * Description : MNTC实验室CAN协议从机源文件
  * Date        : 2018年7月20日
  * Author      : XYD
  * Check       :  
*******************************************************************************/



/*********************File include*********************/
#include "MNTC_CanProtocol.h"                 

/******************Macro definition********************/

/*************External variable declaration************/

/***************Global variable definition*************/


/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------CAN协议 蠕动泵函数--------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

/**************************************************************************************
  * name        : void CAN_PeristalticPumpSetCW(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num,int cw)
  * Function    : 通过CAN总线发送蠕动泵正向电平设置命令
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
  * Returns     : uint8_t target_id-- 目标板地址
									uint8_t object_num-- 对象编号
									int cw-- 正向电平 0-- 低电平正向 1-- 高电平正向
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_PeristalticPumpSetCW(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num,uint16_t cw)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //使用扩展模式
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //数据帧而不是远程帧
  buf->tx_msg.DLC  = 8;                                 								 //数组长度
	
	buf->tx_msg.Data[0]= CMD_HOSE;                        							 	 //蠕动泵模块 
  buf->tx_msg.Data[1]= CMD_HOSE_SPEED;               								 	 	 //蠕动泵速度设置命令
	buf->tx_msg.Data[2]= object_num;                      								 //对象编号
	buf->tx_msg.Data[3]= cw;         	 																	 	 //方向

	CAN_SendData(buf);
}

/**************************************************************************************
  * name        : uint8_t CAN_IsPeristalticPumpSetDir(MNTC_CanProtocolTypeDef buf, int *dir) 
  * Function    : 判断当前接收帧指令是不是正确的蠕动泵方向设置命令
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
  * Returns     : uint8_t-- 0--不是正确的指令  1-- 是正确的指令
									int *dir-- 返回方向
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
uint8_t CAN_IsCAN_PeristalticPumpSetCW(MNTC_CanProtocolTypeDef buf, uint16_t *cw) 
{
	if(buf.rx_msg.Data[0] == CMD_HOSE && 
		 buf.rx_msg.Data[1] == CMD_HOSE_SPEED)
	{
		*cw= buf.rx_msg.Data[3];	
		return 1;
	}
	else 
		return 0;
}

/**************************************************************************************
  * name        : void CAN_PeristalticPumpSetDir(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num,int dir)
  * Function    : 通过CAN总线发送蠕动泵方向设置命令
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
  * Returns     : uint8_t target_id-- 目标板地址
									uint8_t object_num-- 对象编号
									int dir-- 方向 0-- 正向 1-- 反向
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_PeristalticPumpSetDir(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num,uint16_t dir)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //使用扩展模式
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //数据帧而不是远程帧
  buf->tx_msg.DLC  = 8;                                 								 //数组长度
	
	buf->tx_msg.Data[0]= CMD_HOSE;                        							 	 //蠕动泵模块 
  buf->tx_msg.Data[1]= CMD_HOSE_SPEED;               								 	 	 //蠕动泵速度设置命令
	buf->tx_msg.Data[2]= object_num;                      								 //对象编号
	buf->tx_msg.Data[3]= dir;         	 																	 //方向

	CAN_SendData(buf);
}

/**************************************************************************************
  * name        : uint8_t CAN_IsPeristalticPumpSetDir(MNTC_CanProtocolTypeDef buf, int *dir) 
  * Function    : 判断当前接收帧指令是不是正确的蠕动泵方向设置命令
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
  * Returns     : uint8_t-- 0--不是正确的指令  1-- 是正确的指令
									int *dir-- 返回方向
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
uint8_t CAN_IsPeristalticPumpSetDir(MNTC_CanProtocolTypeDef buf, uint16_t *dir) 
{
	if(buf.rx_msg.Data[0] == CMD_HOSE && 
		 buf.rx_msg.Data[1] == CMD_HOSE_SPEED)
	{
		*dir= buf.rx_msg.Data[3];	
		return 1;
	}
	else 
		return 0;
}


/**************************************************************************************
  * name        : void CAN_PeristalticPumpRun(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num,int delay)
  * Function    : 通过CAN总线发送蠕动泵启动指令
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
  * Returns     : uint8_t target_id-- 目标板地址
									uint8_t object_num-- 对象编号
									int delay-- 运动时间，s
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_PeristalticPumpRun(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num,uint16_t delay)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //使用扩展模式
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //数据帧而不是远程帧
  buf->tx_msg.DLC  = 8;                                 								 //数组长度
	
	buf->tx_msg.Data[0]= CMD_HOSE;                        							 	 //蠕动泵模块 
  buf->tx_msg.Data[1]= CMD_HOSE_SS;               								 	 		 //蠕动泵回零命令
	buf->tx_msg.Data[2]= object_num;                      								 //对象编号
	buf->tx_msg.Data[3]= 0x00;
	buf->tx_msg.Data[4]= ((uint16_t)(delay*10.0) & 0XFF00) >> 8;         	 //延时高字节
  buf->tx_msg.Data[5]= ((uint16_t)(delay*10.0) & 0XFF);                	 //延时低字节

	CAN_SendData(buf);
}

/**************************************************************************************
  * name        : uint8_t CAN_IsPeristalticPumpRun(MNTC_CanProtocolTypeDef buf,int *delay)
  * Function    : 判断当前接收帧指令是不是正确的蠕动泵起停命令
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
  * Returns     : uint8_t-- 0--不是正确的指令  1-- 是正确的指令
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
uint8_t CAN_IsPeristalticPumpRun(MNTC_CanProtocolTypeDef buf,uint16_t *delay) 
{
	if(buf.rx_msg.Data[0] == CMD_HOSE && 
		 buf.rx_msg.Data[1] == CMD_HOSE_SS)
	{
		*delay= (buf.rx_msg.Data[4] << 8) | buf.rx_msg.Data[5];	
		*delay /= 10.0;
		return 1;
	}
	else 
		return 0;
}

/**************************************************************************************
  * name        : void CAN_PeristalticPumpSetSpeed(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num,float speed)
  * Function    : 通过CAN总线发送蠕动泵速度设置命令
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
  * Returns     : uint8_t target_id-- 目标板地址
									uint8_t object_num-- 对象编号
									float speed-- 速度
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_PeristalticPumpSetSpeed(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num,float speed)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //使用扩展模式
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //数据帧而不是远程帧
  buf->tx_msg.DLC  = 8;                                 								 //数组长度
	
	buf->tx_msg.Data[0]= CMD_HOSE;                        							 	 //蠕动泵模块 
  buf->tx_msg.Data[1]= CMD_HOSE_SPEED;               								 	 	 //蠕动泵速度设置命令
	buf->tx_msg.Data[2]= object_num;                      								 //对象编号
	buf->tx_msg.Data[3]= ((uint16_t)(speed*10.0) & 0XFF00) >> 8;         	 //速度高字节
  buf->tx_msg.Data[4]= ((uint16_t)(speed*10.0) & 0XFF);                	 //速度低字节

	CAN_SendData(buf);
}



/**************************************************************************************
  * name        : uint8_t CAN_IsPeristalticPumpSetSpeed(MNTC_CanProtocolTypeDef buf, float *speed) 
  * Function    : 判断当前接收帧指令是不是正确的蠕动泵速度设置命令
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
  * Returns     : uint8_t-- 0--不是正确的指令  1-- 是正确的指令
									float *speed-- 返回速度
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
uint8_t CAN_IsPeristalticPumpSetSpeed(MNTC_CanProtocolTypeDef buf, float *speed) 
{
	if(buf.rx_msg.Data[0] == CMD_HOSE && 
		 buf.rx_msg.Data[1] == CMD_HOSE_SPEED)
	{
		*speed= (buf.rx_msg.Data[3] << 8) | buf.rx_msg.Data[4];	
		*speed /= 10.0;
		return 1;
	}
	else 
		return 0;
}

/**************************************************************************************
  * name        : void CAN_ReplyPeristalticState(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, int state)
  * Function    : 返回当前蠕动泵运动状态
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
									uint8_t target_id-- 目标ID
									uint8_t object_num-- 操作对象编号
									int state-- 状态
  * Returns     : 
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_ReplyPeristalticPumpState(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, int state)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //使用扩展模式
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //数据帧而不是远程帧
  buf->tx_msg.DLC  = 8;                                 								 //数组长度
	
	buf->tx_msg.Data[0]= CMD_HOSE;                        								 //蠕动泵模块 
  buf->tx_msg.Data[1]= CMD_HOSE_SS;               								 			 //蠕动泵起停命令
	buf->tx_msg.Data[2]= object_num;                      								 //对象编号
	
	buf->tx_msg.Data[3]= state;                          								 	 //蠕动泵状态 0-- 停止 1-- 启动
	
  
  CAN_Transmit(buf->com, &buf->tx_msg);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------CAN协议 柱塞泵函数--------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/**************************************************************************************
  * name        : void CAN_PlungerHome(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num)
  * Function    : 通过CAN总线发送柱塞泵回零指令
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
  * Returns     : uint8_t target_id-- 目标板地址
									uint8_t object_num-- 对象编号
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_PlungerHome(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //使用扩展模式
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //数据帧而不是远程帧
  buf->tx_msg.DLC  = 8;                                 								 //数组长度
	
	buf->tx_msg.Data[0]= CMD_PLUNGER;                        							 //柱塞泵模块 
  buf->tx_msg.Data[1]= CMD_PLUNGER_HOME;               								 	 //柱塞泵回零命令
	buf->tx_msg.Data[2]= object_num;                      								 //对象编号
	  
	CAN_SendData(buf);
}

/**************************************************************************************
  * name        : uint8_t CAN_IsPlungerHome(MNTC_CanProtocolTypeDef buf)
  * Function    : 判断当前接收帧指令是不是正确的柱塞泵回零命令
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
  * Returns     : uint8_t-- 0--不是正确的指令  1-- 是正确的指令
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
uint8_t CAN_IsPlungerHome(MNTC_CanProtocolTypeDef buf) 
{
	if(buf.rx_msg.Data[0] == CMD_PLUNGER && 
		 buf.rx_msg.Data[1] == CMD_PLUNGER_HOME)
		return 1;
	else 
		return 0;
}

/**************************************************************************************
  * name        : void CAN_PlungerDraw(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float volume, int delay_s)
  * Function    : 通过CAN总线发送柱塞泵抽取指令
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
  * Returns     : uint8_t target_id-- 目标板地址
									uint8_t object_num-- 对象编号
									float volume-- 抽取体积
									int delay_s-- 延时时间
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_PlungerDraw(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float volume, int delay_s)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);

	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //使用扩展模式
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //数据帧而不是远程帧
  buf->tx_msg.DLC  = 8;                                 								 //数组长度
	
	buf->tx_msg.Data[0]= CMD_PLUNGER;                        							 //柱塞泵模块 
  buf->tx_msg.Data[1]= CMD_PLUNGER_DRAW;               								 	 //柱塞泵抽取命令
	buf->tx_msg.Data[2]= object_num;                      								 //对象编号
		
  buf->tx_msg.Data[3]= ((uint16_t)(volume*10.0) & 0XFF00) >> 8;        	 //体积高字节
  buf->tx_msg.Data[4]= ((uint16_t)(volume*10.0) & 0XFF);               	 //体积低字节
	buf->tx_msg.Data[5]= ((uint16_t)(delay_s*10.0) & 0XFF00) >> 8;         //延时高字节
  buf->tx_msg.Data[6]= ((uint16_t)(delay_s*10.0) & 0XFF);                //延时低字节
  
	CAN_SendData(buf);
}

/**************************************************************************************
  * name        : uint8_t CAN_IsPlungerDraw(MNTC_CanProtocolTypeDef buf,float *volume, int *delay_s) 
  * Function    : 判断当前接收帧指令是不是正确的柱塞泵抽取命令
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
									float *volume-- 返回抽取的体积
								  int *delay_s-- 返回抽取后的延时
  * Returns     : uint8_t-- 0--不是正确的指令  1-- 是正确的指令
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
uint8_t CAN_IsPlungerDraw(MNTC_CanProtocolTypeDef buf,float *volume, int *delay_s) 
{
	if(buf.rx_msg.Data[0] == CMD_PLUNGER && 
		 buf.rx_msg.Data[1] == CMD_PLUNGER_DRAW)
	{
		*volume= (buf.rx_msg.Data[3] << 8) | buf.rx_msg.Data[4];	
		*volume /= 10.0;
		*delay_s= (buf.rx_msg.Data[5] << 8) | buf.rx_msg.Data[6];			
		return 1;
	}

	else 
		return 0;
}

/**************************************************************************************
  * name        : void CAN_PlungerPush(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float volume, int delay_s)
  * Function    : 通过CAN总线发送柱塞泵注射指令
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
  * Returns     : uint8_t target_id-- 目标板地址
									uint8_t object_num-- 对象编号
									float volume-- 注射体积
									int delay_s-- 延时时间
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_PlungerPush(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float volume, int delay_s)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //使用扩展模式
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //数据帧而不是远程帧
  buf->tx_msg.DLC  = 8;                                 								 //数组长度
	
	buf->tx_msg.Data[0]= CMD_PLUNGER;                        							 //柱塞泵模块 
  buf->tx_msg.Data[1]= CMD_PLUNGER_PUSH;               								 	 //柱塞泵注射命令
	buf->tx_msg.Data[2]= object_num;                      								 //对象编号
		
  buf->tx_msg.Data[3]= ((uint16_t)(volume*10.0) & 0XFF00) >> 8;        	 //体积高字节
  buf->tx_msg.Data[4]= ((uint16_t)(volume*10.0) & 0XFF);               	 //体积低字节
	buf->tx_msg.Data[5]= ((uint16_t)(delay_s) & 0XFF00) >> 8;         		 //延时高字节
  buf->tx_msg.Data[6]= ((uint16_t)(delay_s) & 0XFF);               			 //延时低字节
  
	CAN_SendData(buf);
}

/**************************************************************************************
  * name        : uint8_t CAN_IsPlungerPush(MNTC_CanProtocolTypeDef buf,float *volume, int *delay_s) 
  * Function    : 判断当前接收帧指令是不是正确的柱塞泵注射命令
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
									float *volume-- 返回注射的体积
								  int *delay_s-- 返回注射后的延时
  * Returns     : uint8_t-- 0--不是正确的指令  1-- 是正确的指令
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
uint8_t CAN_IsPlungerPush(MNTC_CanProtocolTypeDef buf,float *volume, int *delay_s) 
{
	if(buf.rx_msg.Data[0] == CMD_PLUNGER && 
		 buf.rx_msg.Data[1] == CMD_PLUNGER_PUSH)
	{
		*volume= (buf.rx_msg.Data[3] << 8) | buf.rx_msg.Data[4];	
		*volume /= 10.0;
		*delay_s= (buf.rx_msg.Data[5] << 8) | buf.rx_msg.Data[6];			
		return 1;
	}

	else 
		return 0;
}

/**************************************************************************************
  * name        : void CAN_PlungerWash(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float volume, int delay_s, int cnt)
  * Function    : 通过CAN总线发送柱塞泵抽取指令
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
  * Returns     : uint8_t target_id-- 目标板地址
									uint8_t object_num-- 对象编号
									float volume-- 抽取体积
									int delay_s-- 延时时间
									int cnt-- 清洗次数
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_PlungerWash(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float volume, int delay_s, int cnt)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //使用扩展模式
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //数据帧而不是远程帧
  buf->tx_msg.DLC  = 8;                                 								 //数组长度
	
	buf->tx_msg.Data[0]= CMD_PLUNGER;                        							 //柱塞泵模块 
  buf->tx_msg.Data[1]= CMD_PLUNGER_WASH;               								 	 //柱塞泵冲洗命令
	buf->tx_msg.Data[2]= object_num;                      								 //对象编号
		
  buf->tx_msg.Data[3]= ((uint16_t)(volume*10.0) & 0XFF00) >> 8;        	 //体积高字节
  buf->tx_msg.Data[4]= ((uint16_t)(volume*10.0) & 0XFF);               	 //体积低字节
	buf->tx_msg.Data[5]= delay_s;         		 														 //延时
	buf->tx_msg.Data[6]= cnt;         		 																 //清洗次数
  
	CAN_SendData(buf);
}

/**************************************************************************************
  * name        : uint8_t CAN_IsPlungerWash(MNTC_CanProtocolTypeDef buf,float *volume, int *delay_s, int *cnt) 
  * Function    : 判断当前接收帧指令是不是正确的柱塞泵清洗命令
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
									float *volume-- 返回清洗的体积
								  int *delay_s-- 返回清洗的延时
									int *cnt-- 返回清洗次数
  * Returns     : uint8_t-- 0--不是正确的指令  1-- 是正确的指令
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
uint8_t CAN_IsPlungerWash(MNTC_CanProtocolTypeDef buf,float *volume, int *delay_s, int *cnt)  
{
	if(buf.rx_msg.Data[0] == CMD_PLUNGER && 
		 buf.rx_msg.Data[1] == CMD_PLUNGER_PUSH)
	{
		*volume= (buf.rx_msg.Data[3] << 8) | buf.rx_msg.Data[4];	
		*volume /= 10.0;
		*delay_s= buf.rx_msg.Data[5];	
		*cnt= buf.rx_msg.Data[6];	
		return 1;
	}

	else 
		return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/




/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------CAN协议 轴板函数---------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

/**************************************************************************************
  * name        : void CAN_ReplyAxisPosition(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float position)
  * Function    : 回复轴当前绝对位置
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
  * Returns     : uint8_t target_id-- 目标板地址
									uint8_t object_num-- 对象编号
									float position-- 轴当前绝对地址
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_ReplyAxisPosition(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float position)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //使用扩展模式
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //数据帧而不是远程帧
  buf->tx_msg.DLC  = 8;                                 								 //数组长度
	
	buf->tx_msg.Data[0]= CMD_AXIS;                        								 //运动轴模块 
  buf->tx_msg.Data[1]= CMD_AXIS_ABSOLUTE;               								 //绝对运动命令
	buf->tx_msg.Data[2]= object_num;                      								 //对象编号
	
	if(position >= 0.0)
		buf->tx_msg.Data[3]= 0x00;                          								 //绝对坐标为正
	else 
		buf->tx_msg.Data[3]= 0x01;                          								 //绝对坐标为负
	
  buf->tx_msg.Data[4]= ((uint16_t)(position*10.0) & 0XFF00) >> 8;        //坐标高字节
  buf->tx_msg.Data[5]=  ((uint16_t)(position*10.0) & 0XFF);              //坐标低字节

	CAN_SendData(buf);

}

/**************************************************************************************
  * name        : void CAN_ReplyAxisSpeed(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, uint16_t speed)
  * Function    : 回复轴当前设置的速度
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
  * Returns     : uint8_t target_id-- 目标板地址
									uint8_t object_num-- 对象编号
									uint16_t speed-- 轴设置的速度
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_ReplyAxisSpeed(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, uint16_t speed)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //使用扩展模式
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //数据帧而不是远程帧
  buf->tx_msg.DLC  = 8;                                 								 //数组长度
	
	buf->tx_msg.Data[0]= CMD_AXIS;                        								 //运动轴模块 
  buf->tx_msg.Data[1]= CMD_AXIS_SPEED;               										 //绝对运动命令
	buf->tx_msg.Data[2]= object_num;                      								 //对象编号
		
  buf->tx_msg.Data[3]= (speed & 0XFF00) >> 8;        										 //速度高字节
  buf->tx_msg.Data[4]= (speed & 0XFF);              							 			 //速度低字节
  
	CAN_SendData(buf);
}


/**************************************************************************************
  * name        : void CAN_AxisHome(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num)
  * Function    : 通过CAN总线发送运动轴回零指令
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
  * Returns     : uint8_t target_id-- 目标板地址
									uint8_t object_num-- 对象编号
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_AxisHome(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num)
{	
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //使用扩展模式
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //数据帧而不是远程帧
  buf->tx_msg.DLC  = 8;                                 								 //数组长度
	
	buf->tx_msg.Data[0]= CMD_AXIS;                        								 //运动轴模块 
  buf->tx_msg.Data[1]= CMD_AXIS_HOME;               								 		 //回零命令
	buf->tx_msg.Data[2]= object_num;                      								 //对象编号
	  
	CAN_SendData(buf);
}

/**************************************************************************************
  * name        : uint8_t CAN_IsAxisHome(MNTC_CanProtocolTypeDef buf)
  * Function    : 判断当前接收帧指令是不是正确的轴回零命令
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
  * Returns     : uint8_t-- 0--不是正确的指令  1-- 是正确的指令
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
uint8_t CAN_IsAxisHome(MNTC_CanProtocolTypeDef buf) 
{
	if(buf.rx_msg.Data[0] == CMD_AXIS && 
		 buf.rx_msg.Data[1] == CMD_AXIS_HOME)
		return 1;
	else 
		return 0;
}

/**************************************************************************************
  * name        : void CAN_AxisAbsolute(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float position)
  * Function    : 通过CAN总线发送运动轴绝对运动命令
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
  * Returns     : uint8_t target_id-- 目标板地址
									uint8_t object_num-- 对象编号
									float position-- 运动轴绝对位置值
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_AxisAbsolute(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float position)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //使用扩展模式
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //数据帧而不是远程帧
  buf->tx_msg.DLC  = 8;                                 								 //数组长度
	
	buf->tx_msg.Data[0]= CMD_AXIS;                        								 //运动轴模块 
  buf->tx_msg.Data[1]= CMD_AXIS_ABSOLUTE;               								 //绝对运动命令
	buf->tx_msg.Data[2]= object_num;                      								 //对象编号
	
	if(position >= 0.0)
		buf->tx_msg.Data[3]= 0x00;                          								 //绝对坐标为正
	else 
	{
		buf->tx_msg.Data[3]= 0x01;                          								 //绝对坐标为负
		position = 0.0 - position;
	}
	
  buf->tx_msg.Data[4]= ((uint16_t)(position*10.0) & 0XFF00) >> 8;        //坐标高字节
  buf->tx_msg.Data[5]= ((uint16_t)(position*10.0) & 0XFF);               //坐标低字节

	CAN_SendData(buf);
}

/**************************************************************************************
  * name        : uint8_t CAN_IsAxisAbsolute(MNTC_CanProtocolTypeDef buf, float *position)
  * Function    : 判断当前接收帧指令是不是正确的轴绝对运动命令,并通过指针返回绝对位置值
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
									float *position-- 传递绝对位置值的指针
  * Returns     : uint8_t-- 0--不是正确的指令  1-- 是正确的指令
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
uint8_t CAN_IsAxisAbsolute(MNTC_CanProtocolTypeDef buf,float *position)
{
	if(buf.rx_msg.Data[0] == CMD_AXIS && 
		 buf.rx_msg.Data[1] == CMD_AXIS_ABSOLUTE)
	{
		*position= (buf.rx_msg.Data[4] << 8) | buf.rx_msg.Data[5];
		
		*position /= 10.0;
		
		if(buf.rx_msg.Data[3] == 0x01)					//传递的位置值为负数
			*position= 0 - (*position);
		
		return 1;
	}
	else 
		return 0;
}


/**************************************************************************************
  * name        : void CAN_AxisRelative(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float value)
  * Function    : 通过CAN总线发送运动轴相对运动命令
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
  * Returns     : uint8_t target_id-- 目标板地址
									uint8_t object_num-- 对象编号
									float position-- 运动轴相对运动值
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_AxisRelative(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float value)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //使用扩展模式
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //数据帧而不是远程帧
  buf->tx_msg.DLC  = 8;                                 								 //数组长度
	
	buf->tx_msg.Data[0]= CMD_AXIS;                        								 //运动轴模块 
  buf->tx_msg.Data[1]= CMD_AXIS_RELATIVE;               								 //相对运动命令
	buf->tx_msg.Data[2]= object_num;                      								 //对象编号
	
	if(value >= 0.0)
		buf->tx_msg.Data[3]= 0x00;                          								 //绝对坐标为正
	else 
	{
		buf->tx_msg.Data[3]= 0x01;                          								 //绝对坐标为负
		value = 0.0 - value;
	}
	
  buf->tx_msg.Data[4]= ((uint16_t)(value*10.0) & 0XFF00) >> 8;        	 //坐标高字节
  buf->tx_msg.Data[5]= ((uint16_t)(value*10.0) & 0XFF);                  //坐标低字节
  
	CAN_SendData(buf);
}

/**************************************************************************************
  * name        : uint8_t CAN_IsAxisRelative(MNTC_CanProtocolTypeDef buf,float *position)
  * Function    : 判断当前接收帧指令是不是正确的轴相对运动命令,并通过指针返回相对位置值
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
									float *position-- 传递相对位置值得指针
  * Returns     : uint8_t-- 0--不是正确的指令  1-- 是正确的指令
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
uint8_t CAN_IsAxisRelative(MNTC_CanProtocolTypeDef buf,float *position)
{
	if(buf.rx_msg.Data[0] == CMD_AXIS && 
		 buf.rx_msg.Data[1] == CMD_AXIS_RELATIVE)
	{
		*position= (buf.rx_msg.Data[4] << 8) | buf.rx_msg.Data[5];
		
		*position /= 10.0;
		
		if(buf.rx_msg.Data[3] == 0x01)					//传递的位置值为负数
			*position= 0 - (*position);
		
		return 1;
	}
	else 
		return 0;
}
/**************************************************************************************
  * name        : void CAN_AxisSpeed(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, uint16_t speed)
  * Function    : 通过CAN总线发送运动轴速度设置命令
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
  * Returns     : uint8_t target_id-- 目标板地址
									uint8_t object_num-- 对象编号
									uint16_t speed-- 轴设置的速度
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_AxisSpeed(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, uint16_t speed)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //使用扩展模式
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //数据帧而不是远程帧
  buf->tx_msg.DLC  = 8;                                 								 //数组长度
	
	buf->tx_msg.Data[0]= CMD_AXIS;                        								 //运动轴模块 
  buf->tx_msg.Data[1]= CMD_AXIS_SPEED;               										 //速度设置命令
	buf->tx_msg.Data[2]= object_num;                      								 //对象编号
		
  buf->tx_msg.Data[3]= (speed & 0XFF00) >> 8;        										 //速度高字节
  buf->tx_msg.Data[4]= (speed & 0XFF);              							 			 //速度低字节
  
	CAN_SendData(buf); 
}

/**************************************************************************************
  * name        : uint8_t CAN_IsAxisSpeed(MNTC_CanProtocolTypeDef buf,uint16_t *speed)
  * Function    : 判断当前接收帧指令是不是正确的轴速度设置命令,并通过指针返回速度值
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
									uint16_t *speed-- 传递设置的速度值的指针
  * Returns     : uint8_t-- 0--不是正确的指令  1-- 是正确的指令
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
uint8_t CAN_IsAxisSpeed(MNTC_CanProtocolTypeDef buf,uint16_t *speed)
{
	if(buf.rx_msg.Data[0] == CMD_AXIS && 
		 buf.rx_msg.Data[1] == CMD_AXIS_SPEED)
	{																																//是正确轴速度设置命令，传递速度值并返回帧检测结果
		*speed= (buf.rx_msg.Data[3] << 8) | buf.rx_msg.Data[4];
		return 1;
	}
	else 
		return 0;
}


/**************************************************************************************
  * name        : void CAN_AxisContinue(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, uint16_t dir)
  * Function    : 通过CAN总线发送运动直线轴持续运动命令
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
  * Returns     : uint8_t target_id-- 目标板地址
									uint8_t object_num-- 对象编号
									uint16_t dir-- 持续运动方向
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_AxisContinue(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, uint16_t dir)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //使用扩展模式
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //数据帧而不是远程帧
  buf->tx_msg.DLC  = 8;                                 								 //数组长度
	
	buf->tx_msg.Data[0]= CMD_AXIS;                        								 //运动轴模块 
  buf->tx_msg.Data[1]= CMD_AXIS_CONTINUE;               								 //持续运动命令
	buf->tx_msg.Data[2]= object_num;                      								 //对象编号
		
  buf->tx_msg.Data[3]= dir;        																			 //速度高字节
  
	CAN_SendData(buf); 
}

/**************************************************************************************
  * name        : uint8_t CAN_IsAxisContinue(MNTC_CanProtocolTypeDef buf, uint16_t *dir)
  * Function    : 判断当前接收帧指令是不是正确的直线轴持续运动命令
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
									uint16_t *dir-- 连续运动方向
  * Returns     : uint8_t-- 0--不是正确的指令  1-- 是正确的指令
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
uint8_t CAN_IsAxisContinue(MNTC_CanProtocolTypeDef buf, uint16_t *dir)
{
	if(buf.rx_msg.Data[0] == CMD_AXIS && 
		 buf.rx_msg.Data[1] == CMD_AXIS_CONTINUE)
	{																																//是正确直线轴持续运动命令
		*dir= buf.rx_msg.Data[3];																			//返回连续运动方向
		return 1;
	}
	else 
		return 0;
}

/**************************************************************************************
  * name        : void CAN_AxisStop(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num)
  * Function    : 通过CAN总线发送运动直线轴急停命令
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
  * Returns     : uint8_t target_id-- 目标板地址
									uint8_t object_num-- 对象编号
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_AxisStop(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //使用扩展模式
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //数据帧而不是远程帧
  buf->tx_msg.DLC  = 8;                                 								 //数组长度
	
	buf->tx_msg.Data[0]= CMD_AXIS;                        								 //运动轴模块 
  buf->tx_msg.Data[1]= CMD_AXIS_STOP;               								 		 //急停命令
	buf->tx_msg.Data[2]= object_num;                      								 //对象编号
		  
	CAN_SendData(buf); 
}

/**************************************************************************************
  * name        : uint8_t CAN_IsAxisContinue(MNTC_CanProtocolTypeDef buf)
  * Function    : 判断当前接收帧指令是不是正确的直线轴急停命令
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
  * Returns     : uint8_t-- 0--不是正确的指令  1-- 是正确的指令
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
uint8_t CAN_IsAxisStop(MNTC_CanProtocolTypeDef buf)
{
	if(buf.rx_msg.Data[0] == CMD_AXIS && 
		 buf.rx_msg.Data[1] == CMD_AXIS_STOP)
	{																																//是正确直线轴急停命令
		return 1;
	}
	else 
		return 0;
}

/*--------------------------------------------------------------------------------------------------------------------*/





/**************************************************************************************
  * name        : uint8_t CAN_GetRxSourceId(MNTC_CanProtocolTypeDef buf)
  * Function    : 获取接收帧中的源地址
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
  * Returns     : uint8_t-- 接收帧源地址
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
uint8_t CAN_GetRxSourceId(MNTC_CanProtocolTypeDef buf)
{
	return buf.rx_msg.ExtId >> 21; 
}

/**************************************************************************************
  * name        : uint8_t CAN_GetRxSourceId(MNTC_CanProtocolTypeDef buf)
  * Function    : 获取接收帧中的目标地址
  * Parameters  : MNTC_CanProtocolTypeDef buf-- 缓存结构体
  * Returns     : uint8_t-- 接收帧目标地址
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
uint8_t CAN_GetRxTargetId(MNTC_CanProtocolTypeDef buf)
{
	uint8_t temp;
	temp= (buf.rx_msg.ExtId >> 13) & 0XFF;
	return temp;
}

/**************************************************************************************
  * name        : void CAN_SetTxSourceId(MNTC_CanProtocolTypeDef *buf,uint8_t source_id)
  * Function    : 设置发送帧中的源地址
  * Parameters  : MNTC_CanProtocolTypeDef *buf-- 缓存结构体
									uint8_t source_id-- 源地址
  * Returns     : NULL
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_SetTxSourceId(MNTC_CanProtocolTypeDef *buf,uint8_t source_id)
{
	buf->tx_msg.ExtId= buf->tx_msg.ExtId & ((~(u32)0xFF<<21));								
	buf->tx_msg.ExtId= buf->tx_msg.ExtId | ((u32)source_id<<21);
}

/**************************************************************************************
  * name        : void CAN_SetTxTargetId(MNTC_CanProtocolTypeDef *buf,uint8_t target_id)
  * Function    : 设置发送帧中的目标地址
  * Parameters  : MNTC_CanProtocolTypeDef *buf-- 缓存结构体
									uint8_t target_id-- 目标地址
  * Returns     : NULL
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_SetTxTargetId(MNTC_CanProtocolTypeDef *buf,uint8_t target_id)
{
	buf->tx_msg.ExtId= buf->tx_msg.ExtId & (~((u32)0xFF<<13));
	buf->tx_msg.ExtId= buf->tx_msg.ExtId | ((u32)target_id<<13);
}


/**************************************************************************************
  * name        : void CAN_ProtocolInit(MNTC_CanProtocolTypeDef *buf,CAN_TypeDef* com, uint8_t my_id)
  * Function    : CAN协议初始化
  * Parameters  : MNTC_CanProtocolTypeDef *buf-- 缓存结构体
									CAN_TypeDef* com-- 硬件端口CAN1/CAN2
									uint8_t my_id-- 本机地址
  * Returns     : NULL
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_ProtocolInit(MNTC_CanProtocolTypeDef *buf,CAN_TypeDef* com, uint8_t my_id) 
{
#if HAL_LAB_ENA
	buf->hcan.Instance= com;
	buf->hcan.pRxMsg= &buf->rx_msg;
	buf->hcan.pTxMsg= &buf->tx_msg;
	buf->my_id			= my_id;
#elif STM32F1XX_STD_LAB_ENA
	buf->my_id			= my_id;
	buf->com				= com;
#endif
}

/**************************************************************************************
  * name        : void CAN_SendData(MNTC_CanProtocolTypeDef *buf)
  * Function    : CAN发送数据的接口函数，便于匹配不同的库函数
  * Parameters  : MNTC_CanProtocolTypeDef *buf-- 缓存结构体
  * Returns     : NULL
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_SendData(MNTC_CanProtocolTypeDef *buf)
{
#if HAL_LAB_ENA
	HAL_CAN_Transmit(&buf->hcan,0); 
#elif STM32F1XX_STD_LAB_ENA
  CAN_Transmit(buf->com, &buf->tx_msg);
#endif
}
/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/
