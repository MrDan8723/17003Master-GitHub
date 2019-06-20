/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : MNTC_CanProtocolMaster_V0.1.c
  * Version     : V0.1
										1��ʵ�ֻ���MNTCʵ����V0.4�汾Э�������Դ��
  * Description : MNTCʵ����CANЭ��ӻ�Դ�ļ�
  * Date        : 2018��7��20��
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
/*-------------------------------------------------CANЭ�� �䶯�ú���--------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

/**************************************************************************************
  * name        : void CAN_PeristalticPumpSetCW(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num,int cw)
  * Function    : ͨ��CAN���߷����䶯�������ƽ��������
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
  * Returns     : uint8_t target_id-- Ŀ����ַ
									uint8_t object_num-- ������
									int cw-- �����ƽ 0-- �͵�ƽ���� 1-- �ߵ�ƽ����
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_PeristalticPumpSetCW(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num,uint16_t cw)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //ʹ����չģʽ
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //����֡������Զ��֡
  buf->tx_msg.DLC  = 8;                                 								 //���鳤��
	
	buf->tx_msg.Data[0]= CMD_HOSE;                        							 	 //�䶯��ģ�� 
  buf->tx_msg.Data[1]= CMD_HOSE_SPEED;               								 	 	 //�䶯���ٶ���������
	buf->tx_msg.Data[2]= object_num;                      								 //������
	buf->tx_msg.Data[3]= cw;         	 																	 	 //����

	CAN_SendData(buf);
}

/**************************************************************************************
  * name        : uint8_t CAN_IsPeristalticPumpSetDir(MNTC_CanProtocolTypeDef buf, int *dir) 
  * Function    : �жϵ�ǰ����ָ֡���ǲ�����ȷ���䶯�÷�����������
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
  * Returns     : uint8_t-- 0--������ȷ��ָ��  1-- ����ȷ��ָ��
									int *dir-- ���ط���
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
  * Function    : ͨ��CAN���߷����䶯�÷�����������
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
  * Returns     : uint8_t target_id-- Ŀ����ַ
									uint8_t object_num-- ������
									int dir-- ���� 0-- ���� 1-- ����
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_PeristalticPumpSetDir(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num,uint16_t dir)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //ʹ����չģʽ
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //����֡������Զ��֡
  buf->tx_msg.DLC  = 8;                                 								 //���鳤��
	
	buf->tx_msg.Data[0]= CMD_HOSE;                        							 	 //�䶯��ģ�� 
  buf->tx_msg.Data[1]= CMD_HOSE_SPEED;               								 	 	 //�䶯���ٶ���������
	buf->tx_msg.Data[2]= object_num;                      								 //������
	buf->tx_msg.Data[3]= dir;         	 																	 //����

	CAN_SendData(buf);
}

/**************************************************************************************
  * name        : uint8_t CAN_IsPeristalticPumpSetDir(MNTC_CanProtocolTypeDef buf, int *dir) 
  * Function    : �жϵ�ǰ����ָ֡���ǲ�����ȷ���䶯�÷�����������
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
  * Returns     : uint8_t-- 0--������ȷ��ָ��  1-- ����ȷ��ָ��
									int *dir-- ���ط���
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
  * Function    : ͨ��CAN���߷����䶯������ָ��
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
  * Returns     : uint8_t target_id-- Ŀ����ַ
									uint8_t object_num-- ������
									int delay-- �˶�ʱ�䣬s
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_PeristalticPumpRun(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num,uint16_t delay)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //ʹ����չģʽ
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //����֡������Զ��֡
  buf->tx_msg.DLC  = 8;                                 								 //���鳤��
	
	buf->tx_msg.Data[0]= CMD_HOSE;                        							 	 //�䶯��ģ�� 
  buf->tx_msg.Data[1]= CMD_HOSE_SS;               								 	 		 //�䶯�û�������
	buf->tx_msg.Data[2]= object_num;                      								 //������
	buf->tx_msg.Data[3]= 0x00;
	buf->tx_msg.Data[4]= ((uint16_t)(delay*10.0) & 0XFF00) >> 8;         	 //��ʱ���ֽ�
  buf->tx_msg.Data[5]= ((uint16_t)(delay*10.0) & 0XFF);                	 //��ʱ���ֽ�

	CAN_SendData(buf);
}

/**************************************************************************************
  * name        : uint8_t CAN_IsPeristalticPumpRun(MNTC_CanProtocolTypeDef buf,int *delay)
  * Function    : �жϵ�ǰ����ָ֡���ǲ�����ȷ���䶯����ͣ����
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
  * Returns     : uint8_t-- 0--������ȷ��ָ��  1-- ����ȷ��ָ��
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
  * Function    : ͨ��CAN���߷����䶯���ٶ���������
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
  * Returns     : uint8_t target_id-- Ŀ����ַ
									uint8_t object_num-- ������
									float speed-- �ٶ�
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_PeristalticPumpSetSpeed(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num,float speed)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //ʹ����չģʽ
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //����֡������Զ��֡
  buf->tx_msg.DLC  = 8;                                 								 //���鳤��
	
	buf->tx_msg.Data[0]= CMD_HOSE;                        							 	 //�䶯��ģ�� 
  buf->tx_msg.Data[1]= CMD_HOSE_SPEED;               								 	 	 //�䶯���ٶ���������
	buf->tx_msg.Data[2]= object_num;                      								 //������
	buf->tx_msg.Data[3]= ((uint16_t)(speed*10.0) & 0XFF00) >> 8;         	 //�ٶȸ��ֽ�
  buf->tx_msg.Data[4]= ((uint16_t)(speed*10.0) & 0XFF);                	 //�ٶȵ��ֽ�

	CAN_SendData(buf);
}



/**************************************************************************************
  * name        : uint8_t CAN_IsPeristalticPumpSetSpeed(MNTC_CanProtocolTypeDef buf, float *speed) 
  * Function    : �жϵ�ǰ����ָ֡���ǲ�����ȷ���䶯���ٶ���������
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
  * Returns     : uint8_t-- 0--������ȷ��ָ��  1-- ����ȷ��ָ��
									float *speed-- �����ٶ�
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
  * Function    : ���ص�ǰ�䶯���˶�״̬
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
									uint8_t target_id-- Ŀ��ID
									uint8_t object_num-- ����������
									int state-- ״̬
  * Returns     : 
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_ReplyPeristalticPumpState(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, int state)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //ʹ����չģʽ
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //����֡������Զ��֡
  buf->tx_msg.DLC  = 8;                                 								 //���鳤��
	
	buf->tx_msg.Data[0]= CMD_HOSE;                        								 //�䶯��ģ�� 
  buf->tx_msg.Data[1]= CMD_HOSE_SS;               								 			 //�䶯����ͣ����
	buf->tx_msg.Data[2]= object_num;                      								 //������
	
	buf->tx_msg.Data[3]= state;                          								 	 //�䶯��״̬ 0-- ֹͣ 1-- ����
	
  
  CAN_Transmit(buf->com, &buf->tx_msg);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------CANЭ�� �����ú���--------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/**************************************************************************************
  * name        : void CAN_PlungerHome(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num)
  * Function    : ͨ��CAN���߷��������û���ָ��
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
  * Returns     : uint8_t target_id-- Ŀ����ַ
									uint8_t object_num-- ������
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_PlungerHome(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //ʹ����չģʽ
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //����֡������Զ��֡
  buf->tx_msg.DLC  = 8;                                 								 //���鳤��
	
	buf->tx_msg.Data[0]= CMD_PLUNGER;                        							 //������ģ�� 
  buf->tx_msg.Data[1]= CMD_PLUNGER_HOME;               								 	 //�����û�������
	buf->tx_msg.Data[2]= object_num;                      								 //������
	  
	CAN_SendData(buf);
}

/**************************************************************************************
  * name        : uint8_t CAN_IsPlungerHome(MNTC_CanProtocolTypeDef buf)
  * Function    : �жϵ�ǰ����ָ֡���ǲ�����ȷ�������û�������
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
  * Returns     : uint8_t-- 0--������ȷ��ָ��  1-- ����ȷ��ָ��
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
  * Function    : ͨ��CAN���߷��������ó�ȡָ��
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
  * Returns     : uint8_t target_id-- Ŀ����ַ
									uint8_t object_num-- ������
									float volume-- ��ȡ���
									int delay_s-- ��ʱʱ��
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_PlungerDraw(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float volume, int delay_s)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);

	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //ʹ����չģʽ
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //����֡������Զ��֡
  buf->tx_msg.DLC  = 8;                                 								 //���鳤��
	
	buf->tx_msg.Data[0]= CMD_PLUNGER;                        							 //������ģ�� 
  buf->tx_msg.Data[1]= CMD_PLUNGER_DRAW;               								 	 //�����ó�ȡ����
	buf->tx_msg.Data[2]= object_num;                      								 //������
		
  buf->tx_msg.Data[3]= ((uint16_t)(volume*10.0) & 0XFF00) >> 8;        	 //������ֽ�
  buf->tx_msg.Data[4]= ((uint16_t)(volume*10.0) & 0XFF);               	 //������ֽ�
	buf->tx_msg.Data[5]= ((uint16_t)(delay_s*10.0) & 0XFF00) >> 8;         //��ʱ���ֽ�
  buf->tx_msg.Data[6]= ((uint16_t)(delay_s*10.0) & 0XFF);                //��ʱ���ֽ�
  
	CAN_SendData(buf);
}

/**************************************************************************************
  * name        : uint8_t CAN_IsPlungerDraw(MNTC_CanProtocolTypeDef buf,float *volume, int *delay_s) 
  * Function    : �жϵ�ǰ����ָ֡���ǲ�����ȷ�������ó�ȡ����
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
									float *volume-- ���س�ȡ�����
								  int *delay_s-- ���س�ȡ�����ʱ
  * Returns     : uint8_t-- 0--������ȷ��ָ��  1-- ����ȷ��ָ��
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
  * Function    : ͨ��CAN���߷���������ע��ָ��
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
  * Returns     : uint8_t target_id-- Ŀ����ַ
									uint8_t object_num-- ������
									float volume-- ע�����
									int delay_s-- ��ʱʱ��
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_PlungerPush(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float volume, int delay_s)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //ʹ����չģʽ
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //����֡������Զ��֡
  buf->tx_msg.DLC  = 8;                                 								 //���鳤��
	
	buf->tx_msg.Data[0]= CMD_PLUNGER;                        							 //������ģ�� 
  buf->tx_msg.Data[1]= CMD_PLUNGER_PUSH;               								 	 //������ע������
	buf->tx_msg.Data[2]= object_num;                      								 //������
		
  buf->tx_msg.Data[3]= ((uint16_t)(volume*10.0) & 0XFF00) >> 8;        	 //������ֽ�
  buf->tx_msg.Data[4]= ((uint16_t)(volume*10.0) & 0XFF);               	 //������ֽ�
	buf->tx_msg.Data[5]= ((uint16_t)(delay_s) & 0XFF00) >> 8;         		 //��ʱ���ֽ�
  buf->tx_msg.Data[6]= ((uint16_t)(delay_s) & 0XFF);               			 //��ʱ���ֽ�
  
	CAN_SendData(buf);
}

/**************************************************************************************
  * name        : uint8_t CAN_IsPlungerPush(MNTC_CanProtocolTypeDef buf,float *volume, int *delay_s) 
  * Function    : �жϵ�ǰ����ָ֡���ǲ�����ȷ��������ע������
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
									float *volume-- ����ע������
								  int *delay_s-- ����ע������ʱ
  * Returns     : uint8_t-- 0--������ȷ��ָ��  1-- ����ȷ��ָ��
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
  * Function    : ͨ��CAN���߷��������ó�ȡָ��
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
  * Returns     : uint8_t target_id-- Ŀ����ַ
									uint8_t object_num-- ������
									float volume-- ��ȡ���
									int delay_s-- ��ʱʱ��
									int cnt-- ��ϴ����
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_PlungerWash(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float volume, int delay_s, int cnt)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //ʹ����չģʽ
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //����֡������Զ��֡
  buf->tx_msg.DLC  = 8;                                 								 //���鳤��
	
	buf->tx_msg.Data[0]= CMD_PLUNGER;                        							 //������ģ�� 
  buf->tx_msg.Data[1]= CMD_PLUNGER_WASH;               								 	 //�����ó�ϴ����
	buf->tx_msg.Data[2]= object_num;                      								 //������
		
  buf->tx_msg.Data[3]= ((uint16_t)(volume*10.0) & 0XFF00) >> 8;        	 //������ֽ�
  buf->tx_msg.Data[4]= ((uint16_t)(volume*10.0) & 0XFF);               	 //������ֽ�
	buf->tx_msg.Data[5]= delay_s;         		 														 //��ʱ
	buf->tx_msg.Data[6]= cnt;         		 																 //��ϴ����
  
	CAN_SendData(buf);
}

/**************************************************************************************
  * name        : uint8_t CAN_IsPlungerWash(MNTC_CanProtocolTypeDef buf,float *volume, int *delay_s, int *cnt) 
  * Function    : �жϵ�ǰ����ָ֡���ǲ�����ȷ����������ϴ����
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
									float *volume-- ������ϴ�����
								  int *delay_s-- ������ϴ����ʱ
									int *cnt-- ������ϴ����
  * Returns     : uint8_t-- 0--������ȷ��ָ��  1-- ����ȷ��ָ��
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
/*--------------------------------------------------CANЭ�� ��庯��---------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

/**************************************************************************************
  * name        : void CAN_ReplyAxisPosition(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float position)
  * Function    : �ظ��ᵱǰ����λ��
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
  * Returns     : uint8_t target_id-- Ŀ����ַ
									uint8_t object_num-- ������
									float position-- �ᵱǰ���Ե�ַ
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_ReplyAxisPosition(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float position)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //ʹ����չģʽ
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //����֡������Զ��֡
  buf->tx_msg.DLC  = 8;                                 								 //���鳤��
	
	buf->tx_msg.Data[0]= CMD_AXIS;                        								 //�˶���ģ�� 
  buf->tx_msg.Data[1]= CMD_AXIS_ABSOLUTE;               								 //�����˶�����
	buf->tx_msg.Data[2]= object_num;                      								 //������
	
	if(position >= 0.0)
		buf->tx_msg.Data[3]= 0x00;                          								 //��������Ϊ��
	else 
		buf->tx_msg.Data[3]= 0x01;                          								 //��������Ϊ��
	
  buf->tx_msg.Data[4]= ((uint16_t)(position*10.0) & 0XFF00) >> 8;        //������ֽ�
  buf->tx_msg.Data[5]=  ((uint16_t)(position*10.0) & 0XFF);              //������ֽ�

	CAN_SendData(buf);

}

/**************************************************************************************
  * name        : void CAN_ReplyAxisSpeed(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, uint16_t speed)
  * Function    : �ظ��ᵱǰ���õ��ٶ�
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
  * Returns     : uint8_t target_id-- Ŀ����ַ
									uint8_t object_num-- ������
									uint16_t speed-- �����õ��ٶ�
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_ReplyAxisSpeed(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, uint16_t speed)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //ʹ����չģʽ
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //����֡������Զ��֡
  buf->tx_msg.DLC  = 8;                                 								 //���鳤��
	
	buf->tx_msg.Data[0]= CMD_AXIS;                        								 //�˶���ģ�� 
  buf->tx_msg.Data[1]= CMD_AXIS_SPEED;               										 //�����˶�����
	buf->tx_msg.Data[2]= object_num;                      								 //������
		
  buf->tx_msg.Data[3]= (speed & 0XFF00) >> 8;        										 //�ٶȸ��ֽ�
  buf->tx_msg.Data[4]= (speed & 0XFF);              							 			 //�ٶȵ��ֽ�
  
	CAN_SendData(buf);
}


/**************************************************************************************
  * name        : void CAN_AxisHome(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num)
  * Function    : ͨ��CAN���߷����˶������ָ��
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
  * Returns     : uint8_t target_id-- Ŀ����ַ
									uint8_t object_num-- ������
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_AxisHome(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num)
{	
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //ʹ����չģʽ
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //����֡������Զ��֡
  buf->tx_msg.DLC  = 8;                                 								 //���鳤��
	
	buf->tx_msg.Data[0]= CMD_AXIS;                        								 //�˶���ģ�� 
  buf->tx_msg.Data[1]= CMD_AXIS_HOME;               								 		 //��������
	buf->tx_msg.Data[2]= object_num;                      								 //������
	  
	CAN_SendData(buf);
}

/**************************************************************************************
  * name        : uint8_t CAN_IsAxisHome(MNTC_CanProtocolTypeDef buf)
  * Function    : �жϵ�ǰ����ָ֡���ǲ�����ȷ�����������
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
  * Returns     : uint8_t-- 0--������ȷ��ָ��  1-- ����ȷ��ָ��
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
  * Function    : ͨ��CAN���߷����˶�������˶�����
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
  * Returns     : uint8_t target_id-- Ŀ����ַ
									uint8_t object_num-- ������
									float position-- �˶������λ��ֵ
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_AxisAbsolute(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float position)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //ʹ����չģʽ
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //����֡������Զ��֡
  buf->tx_msg.DLC  = 8;                                 								 //���鳤��
	
	buf->tx_msg.Data[0]= CMD_AXIS;                        								 //�˶���ģ�� 
  buf->tx_msg.Data[1]= CMD_AXIS_ABSOLUTE;               								 //�����˶�����
	buf->tx_msg.Data[2]= object_num;                      								 //������
	
	if(position >= 0.0)
		buf->tx_msg.Data[3]= 0x00;                          								 //��������Ϊ��
	else 
	{
		buf->tx_msg.Data[3]= 0x01;                          								 //��������Ϊ��
		position = 0.0 - position;
	}
	
  buf->tx_msg.Data[4]= ((uint16_t)(position*10.0) & 0XFF00) >> 8;        //������ֽ�
  buf->tx_msg.Data[5]= ((uint16_t)(position*10.0) & 0XFF);               //������ֽ�

	CAN_SendData(buf);
}

/**************************************************************************************
  * name        : uint8_t CAN_IsAxisAbsolute(MNTC_CanProtocolTypeDef buf, float *position)
  * Function    : �жϵ�ǰ����ָ֡���ǲ�����ȷ��������˶�����,��ͨ��ָ�뷵�ؾ���λ��ֵ
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
									float *position-- ���ݾ���λ��ֵ��ָ��
  * Returns     : uint8_t-- 0--������ȷ��ָ��  1-- ����ȷ��ָ��
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
		
		if(buf.rx_msg.Data[3] == 0x01)					//���ݵ�λ��ֵΪ����
			*position= 0 - (*position);
		
		return 1;
	}
	else 
		return 0;
}


/**************************************************************************************
  * name        : void CAN_AxisRelative(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float value)
  * Function    : ͨ��CAN���߷����˶�������˶�����
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
  * Returns     : uint8_t target_id-- Ŀ����ַ
									uint8_t object_num-- ������
									float position-- �˶�������˶�ֵ
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_AxisRelative(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float value)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //ʹ����չģʽ
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //����֡������Զ��֡
  buf->tx_msg.DLC  = 8;                                 								 //���鳤��
	
	buf->tx_msg.Data[0]= CMD_AXIS;                        								 //�˶���ģ�� 
  buf->tx_msg.Data[1]= CMD_AXIS_RELATIVE;               								 //����˶�����
	buf->tx_msg.Data[2]= object_num;                      								 //������
	
	if(value >= 0.0)
		buf->tx_msg.Data[3]= 0x00;                          								 //��������Ϊ��
	else 
	{
		buf->tx_msg.Data[3]= 0x01;                          								 //��������Ϊ��
		value = 0.0 - value;
	}
	
  buf->tx_msg.Data[4]= ((uint16_t)(value*10.0) & 0XFF00) >> 8;        	 //������ֽ�
  buf->tx_msg.Data[5]= ((uint16_t)(value*10.0) & 0XFF);                  //������ֽ�
  
	CAN_SendData(buf);
}

/**************************************************************************************
  * name        : uint8_t CAN_IsAxisRelative(MNTC_CanProtocolTypeDef buf,float *position)
  * Function    : �жϵ�ǰ����ָ֡���ǲ�����ȷ��������˶�����,��ͨ��ָ�뷵�����λ��ֵ
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
									float *position-- �������λ��ֵ��ָ��
  * Returns     : uint8_t-- 0--������ȷ��ָ��  1-- ����ȷ��ָ��
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
		
		if(buf.rx_msg.Data[3] == 0x01)					//���ݵ�λ��ֵΪ����
			*position= 0 - (*position);
		
		return 1;
	}
	else 
		return 0;
}
/**************************************************************************************
  * name        : void CAN_AxisSpeed(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, uint16_t speed)
  * Function    : ͨ��CAN���߷����˶����ٶ���������
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
  * Returns     : uint8_t target_id-- Ŀ����ַ
									uint8_t object_num-- ������
									uint16_t speed-- �����õ��ٶ�
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_AxisSpeed(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, uint16_t speed)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //ʹ����չģʽ
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //����֡������Զ��֡
  buf->tx_msg.DLC  = 8;                                 								 //���鳤��
	
	buf->tx_msg.Data[0]= CMD_AXIS;                        								 //�˶���ģ�� 
  buf->tx_msg.Data[1]= CMD_AXIS_SPEED;               										 //�ٶ���������
	buf->tx_msg.Data[2]= object_num;                      								 //������
		
  buf->tx_msg.Data[3]= (speed & 0XFF00) >> 8;        										 //�ٶȸ��ֽ�
  buf->tx_msg.Data[4]= (speed & 0XFF);              							 			 //�ٶȵ��ֽ�
  
	CAN_SendData(buf); 
}

/**************************************************************************************
  * name        : uint8_t CAN_IsAxisSpeed(MNTC_CanProtocolTypeDef buf,uint16_t *speed)
  * Function    : �жϵ�ǰ����ָ֡���ǲ�����ȷ�����ٶ���������,��ͨ��ָ�뷵���ٶ�ֵ
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
									uint16_t *speed-- �������õ��ٶ�ֵ��ָ��
  * Returns     : uint8_t-- 0--������ȷ��ָ��  1-- ����ȷ��ָ��
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
uint8_t CAN_IsAxisSpeed(MNTC_CanProtocolTypeDef buf,uint16_t *speed)
{
	if(buf.rx_msg.Data[0] == CMD_AXIS && 
		 buf.rx_msg.Data[1] == CMD_AXIS_SPEED)
	{																																//����ȷ���ٶ�������������ٶ�ֵ������֡�����
		*speed= (buf.rx_msg.Data[3] << 8) | buf.rx_msg.Data[4];
		return 1;
	}
	else 
		return 0;
}


/**************************************************************************************
  * name        : void CAN_AxisContinue(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, uint16_t dir)
  * Function    : ͨ��CAN���߷����˶�ֱ��������˶�����
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
  * Returns     : uint8_t target_id-- Ŀ����ַ
									uint8_t object_num-- ������
									uint16_t dir-- �����˶�����
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_AxisContinue(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, uint16_t dir)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //ʹ����չģʽ
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //����֡������Զ��֡
  buf->tx_msg.DLC  = 8;                                 								 //���鳤��
	
	buf->tx_msg.Data[0]= CMD_AXIS;                        								 //�˶���ģ�� 
  buf->tx_msg.Data[1]= CMD_AXIS_CONTINUE;               								 //�����˶�����
	buf->tx_msg.Data[2]= object_num;                      								 //������
		
  buf->tx_msg.Data[3]= dir;        																			 //�ٶȸ��ֽ�
  
	CAN_SendData(buf); 
}

/**************************************************************************************
  * name        : uint8_t CAN_IsAxisContinue(MNTC_CanProtocolTypeDef buf, uint16_t *dir)
  * Function    : �жϵ�ǰ����ָ֡���ǲ�����ȷ��ֱ��������˶�����
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
									uint16_t *dir-- �����˶�����
  * Returns     : uint8_t-- 0--������ȷ��ָ��  1-- ����ȷ��ָ��
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
uint8_t CAN_IsAxisContinue(MNTC_CanProtocolTypeDef buf, uint16_t *dir)
{
	if(buf.rx_msg.Data[0] == CMD_AXIS && 
		 buf.rx_msg.Data[1] == CMD_AXIS_CONTINUE)
	{																																//����ȷֱ��������˶�����
		*dir= buf.rx_msg.Data[3];																			//���������˶�����
		return 1;
	}
	else 
		return 0;
}

/**************************************************************************************
  * name        : void CAN_AxisStop(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num)
  * Function    : ͨ��CAN���߷����˶�ֱ���ἱͣ����
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
  * Returns     : uint8_t target_id-- Ŀ����ַ
									uint8_t object_num-- ������
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CAN_AxisStop(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num)
{
	CAN_SetTxSourceId(buf,buf->my_id);
	CAN_SetTxTargetId(buf,target_id);
	
	buf->tx_msg.IDE  = CAN_ID_EXT;                        								 //ʹ����չģʽ
  buf->tx_msg.RTR  = CAN_RTR_DATA;                        							 //����֡������Զ��֡
  buf->tx_msg.DLC  = 8;                                 								 //���鳤��
	
	buf->tx_msg.Data[0]= CMD_AXIS;                        								 //�˶���ģ�� 
  buf->tx_msg.Data[1]= CMD_AXIS_STOP;               								 		 //��ͣ����
	buf->tx_msg.Data[2]= object_num;                      								 //������
		  
	CAN_SendData(buf); 
}

/**************************************************************************************
  * name        : uint8_t CAN_IsAxisContinue(MNTC_CanProtocolTypeDef buf)
  * Function    : �жϵ�ǰ����ָ֡���ǲ�����ȷ��ֱ���ἱͣ����
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
  * Returns     : uint8_t-- 0--������ȷ��ָ��  1-- ����ȷ��ָ��
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
uint8_t CAN_IsAxisStop(MNTC_CanProtocolTypeDef buf)
{
	if(buf.rx_msg.Data[0] == CMD_AXIS && 
		 buf.rx_msg.Data[1] == CMD_AXIS_STOP)
	{																																//����ȷֱ���ἱͣ����
		return 1;
	}
	else 
		return 0;
}

/*--------------------------------------------------------------------------------------------------------------------*/





/**************************************************************************************
  * name        : uint8_t CAN_GetRxSourceId(MNTC_CanProtocolTypeDef buf)
  * Function    : ��ȡ����֡�е�Դ��ַ
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
  * Returns     : uint8_t-- ����֡Դ��ַ
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
  * Function    : ��ȡ����֡�е�Ŀ���ַ
  * Parameters  : MNTC_CanProtocolTypeDef buf-- ����ṹ��
  * Returns     : uint8_t-- ����֡Ŀ���ַ
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
  * Function    : ���÷���֡�е�Դ��ַ
  * Parameters  : MNTC_CanProtocolTypeDef *buf-- ����ṹ��
									uint8_t source_id-- Դ��ַ
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
  * Function    : ���÷���֡�е�Ŀ���ַ
  * Parameters  : MNTC_CanProtocolTypeDef *buf-- ����ṹ��
									uint8_t target_id-- Ŀ���ַ
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
  * Function    : CANЭ���ʼ��
  * Parameters  : MNTC_CanProtocolTypeDef *buf-- ����ṹ��
									CAN_TypeDef* com-- Ӳ���˿�CAN1/CAN2
									uint8_t my_id-- ������ַ
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
  * Function    : CAN�������ݵĽӿں���������ƥ�䲻ͬ�Ŀ⺯��
  * Parameters  : MNTC_CanProtocolTypeDef *buf-- ����ṹ��
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
