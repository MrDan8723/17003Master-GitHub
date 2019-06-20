/*********************** (C) COPYRIGHT  2016 MNTC  ***************************

  * Copyright Wuhan MNTC Technology Inc.

  * File name   : Templet.h
  * Version     : V0.1
  * Description : ����MNTC��̹淶��.h�ļ�ģ���ļ�
  * Date        : 2017��9��6��
  * Author      : Author
  * Check       :  
*******************************************************************************/
#ifndef _MNTC_CAN_PROTOCOL_H_
#define _MNTC_CAN_PROTOCOL_H_

#define HAL_LAB_ENA   					0			//HAL��֧��
#define STM32F1XX_STD_LAB_ENA		1			//��׼��֧��

/*********************File include*********************/
#if HAL_LAB_ENA
#include "can.h"
#elif STM32F1XX_STD_LAB_ENA
#include "stm32f10x_can.h"
#endif

/******************Macro definition********************/
/******************Macro definition********************/
//��������ģ�飬δ����ɽ�ֹ�޸�
#define CMD_IO              0X00      //IO������
#define CMD_PLUNGER         0X01      //����������
#define CMD_HOSE						0X02			//�䶯��
#define CMD_AXIS            0X03      //�˶�������
#define CMD_ADC    			    0X04      //ģ��������
#define CMD_TEMP            0X05      //�¶ȿ�������
#define CMD_USER            0XF0      //�û��Զ�������

//IOģ�鹦�ܣ�δ����ɽ�ֹ�޸�
#define CMD_IO_READ         0X00      //��IO�ڲ���
#define CMD_IO_WRITE        0X01      //дIO�ڲ���
#define CMD_IO_TRIGGER      0X02      //IO��ת����
#define CMD_IO_SETENABLE		0X03			//����ʹ�ܵ�ƽ

//������ģ�鹦�ܣ�δ����ɽ�ֹ�޸�
#define CMD_PLUNGER_HOME    0X00      //�����û������
#define CMD_PLUNGER_DRAW    0X01      //�����ó�ȡ����
#define CMD_PLUNGER_PUSH    0X02      //������ע�����
#define CMD_PLUNGER_WASH    0X03      //�����ó�ϴ����
#define CMD_PLUNGER_SPEED   0X04      //�����������ٶȲ���

//�䶯��ģ�鹦�ܣ�δ����ɽ�ֹ�޸�
#define CMD_HOSE_SETCW      0X00      //�䶯�������ƽ
#define CMD_HOSE_SETDIR     0X01      //�䶯�÷���
#define CMD_HOSE_SS   		  0X02      //�䶯����ͣ
#define CMD_HOSE_SPEED			0X03			//�����ٶ�

//�˶���ģ�鹦�ܣ�δ����ɽ�ֹ�޸�
#define CMD_AXIS_HOME       0X00      //�˶���������
#define CMD_AXIS_SPEED   		0X01      //�˶��������ٶȲ���
#define CMD_AXIS_ABSOLUTE   0X02      //�˶�������˶�����
#define CMD_AXIS_RELATIVE		0X03  		//����˶�
#define CMD_AXIS_CONTINUE		0X04			//�����˶�
#define CMD_AXIS_STOP				0X05			//��ͣ

//ģ����ģ�鹦�ܣ�δ����ɽ�ֹ�޸�
#define CMD_ANALOG_READ     0X00       //��ģ����
#define CMD_ANALOG_WRITE	  0X01			 //дģ����

//�¶ȿ���ģ�鹦�ܣ�δ����ɽ�ֹ�޸�
#define CMD_TEMP_SS         0X00      //�¶ȿ�����ͣ����
#define CMD_TEMP_WRITE      0X01      //����Ŀ���¶Ȳ���
#define CMD_TEMP_READ       0X02      //��ȡ��ǰ�¶Ȳ���
#define CMD_TEMP_PARAM_P    0X03      //�����¶ȿ�����P��������
#define CMD_TEMP_PARAM_I    0X04      //�����¶ȿ�����I��������
#define CMD_TEMP_PARAM_D    0X05      //�����¶ȿ�����D��������


/*********Enumeration variable type definition********/



/**************Structure type definition**************/
typedef struct _MNTC_CanProtocolTypeDef
{ 
#if HAL_LAB_ENA
	CAN_HandleTypeDef		hcan;							//CAN���
  CanTxMsgTypeDef     tx_msg;           //���ͻ�����
  CanRxMsgTypeDef     rx_msg;           //���ջ�����
#elif STM32F1XX_STD_LAB_ENA
	CAN_TypeDef* 				com;              //ָ��CAN�˿�  
	CanTxMsg						tx_msg;           //���ͻ�����
	CanRxMsg						rx_msg;           //���ջ�����
#endif
	
	uint8_t      my_id;                   //�����ָ��CAN�ڵ�ID
	char         rx_flag;									//������ɱ�־λ��1--���յ���ȷ����  0-- δ���յ�����
}MNTC_CanProtocolTypeDef;

/*****************Function declaration**************/

//�û��ӿں���
void CAN_ProtocolInit(MNTC_CanProtocolTypeDef *buf,CAN_TypeDef* com, uint8_t my_id);//CANЭ���ʼ������

//�䶯�ÿ��ƺ���
void CAN_PeristalticPumpSetCW(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num,uint16_t cw);//�����䶯���������ƽ
uint8_t CAN_IsCAN_PeristalticPumpSetCW(MNTC_CanProtocolTypeDef buf, uint16_t *cw);//�жϵ�ǰ֡�ǲ��������䶯���������ƽ����
void CAN_PeristalticPumpSetDir(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num,uint16_t dir);//�����䶯���˶�����
uint8_t CAN_IsPeristalticPumpSetDir(MNTC_CanProtocolTypeDef buf, uint16_t *dir);//�жϵ�ǰ֡�����ǲ��������䶯���˶���������
void CAN_PeristalticPumpRun(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num,uint16_t delay);//�䶯����ͣ����
uint8_t CAN_IsPeristalticPumpRun(MNTC_CanProtocolTypeDef buf,uint16_t *delay);//�жϵ�ǰ֡�ǲ����䶯����ͣ����
void CAN_PeristalticPumpSetSpeed(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num,float speed);//�䶯���ٶ�����
uint8_t CAN_IsPeristalticPumpSetSpeed(MNTC_CanProtocolTypeDef buf, float *speed);//�ж��ǲ����䶯���ٶ���������
void CAN_ReplyPeristalticPumpState(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, int state);//�䶯��״̬�ظ�����

//�����ÿ��ƺ���
void CAN_PlungerHome(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num);//���������û�������
uint8_t CAN_IsPlungerHome(MNTC_CanProtocolTypeDef buf);//�жϵ�ǰ���յ������ǲ��������û�������
void CAN_PlungerDraw(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float volume, int delay_s);//���������ó�ȡ����
uint8_t CAN_IsPlungerDraw(MNTC_CanProtocolTypeDef buf,float *volume, int *delay_s);//�жϵ�ǰ���յ������ǲ��������ó�ȡ����
void CAN_PlungerPush(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float volume, int delay_s);//����������ע������
uint8_t CAN_IsPlungerPush(MNTC_CanProtocolTypeDef buf,float *volume, int *delay_s);//�жϵ�ǰ���յ������ǲ���������ע������
void CAN_PlungerWash(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float volume, int delay_s, int cnt);//������������ϴ����
uint8_t CAN_IsPlungerWash(MNTC_CanProtocolTypeDef buf,float *volume, int *delay_s, int *cnt) ;//�жϵ�ǰ���յ������ǲ�����������ϴ����


//ֱ������ƺ���
void CAN_ReplyAxisPosition(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float position);//�ظ�ֱ�������λ��
void CAN_ReplyAxisSpeed(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, uint16_t speed);//�ظ�ֱ�����ٶ�

void CAN_AxisHome(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num);//����ֱ�����������
uint8_t CAN_IsAxisHome(MNTC_CanProtocolTypeDef buf);													//�жϵ�ǰ����ָ֡���ǲ�����ȷ�����������
void CAN_AxisAbsolute(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float position);//����ֱ��������˶�����
uint8_t CAN_IsAxisAbsolute(MNTC_CanProtocolTypeDef buf,float *position);			//�жϵ�ǰ����ָ֡���ǲ�����ȷ��������˶�����,��ͨ��ָ�뷵�ؾ���λ��ֵ
void CAN_AxisRelative(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, float value);//����ֱ��������˶�����
uint8_t CAN_IsAxisRelative(MNTC_CanProtocolTypeDef buf,float *position);			//�жϵ�ǰ����ָ֡���ǲ�����ȷ��������˶�����,��ͨ��ָ�뷵�����λ��ֵ
void CAN_AxisSpeed(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, uint16_t speed);//����ֱ�����ٶ���������
uint8_t CAN_IsAxisSpeed(MNTC_CanProtocolTypeDef buf,uint16_t *speed);					//�жϵ�ǰ����ָ֡���ǲ�����ȷ�����ٶ���������,��ͨ��ָ�뷵���ٶ�ֵ
void CAN_AxisContinue(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num, uint16_t dir);//����ֱ��������˶�����
uint8_t CAN_IsAxisContinue(MNTC_CanProtocolTypeDef buf, uint16_t *dir);//�жϵ�ǰ����֡�����ǲ���ֱ��������˶�����
void CAN_AxisStop(MNTC_CanProtocolTypeDef *buf,uint8_t target_id,uint8_t object_num);//����ֱ���ἱͣ����
uint8_t CAN_IsAxisStop(MNTC_CanProtocolTypeDef buf);//�жϵ�ǰ����֡�����ǲ���ֱ���ἱͣ����

//����Э���������
uint8_t CAN_GetRxSourceId(MNTC_CanProtocolTypeDef buf);												//��ȡ����֡�е�Դ��ַ
uint8_t CAN_GetRxTargetId(MNTC_CanProtocolTypeDef buf);												//��ȡ����֡�е�Ŀ���ַ
void CAN_SetTxSourceId(MNTC_CanProtocolTypeDef *buf,uint8_t source_id);				//���÷���֡�е�Դ��ַ
void CAN_SetTxTargetId(MNTC_CanProtocolTypeDef *buf,uint8_t target_id);				//���÷���֡�е�Ŀ���ַ
void CAN_SendData(MNTC_CanProtocolTypeDef *buf);															//CAN��������
 #endif

/**************** (C) COPYRIGHT 2016 MNTC *****END OF FILE*******************/ 

  