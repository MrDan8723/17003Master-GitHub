/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : DS18B20_V0.1.c
  * Version     : V0.1
  * Description : DS18B20��������,���ӹ���ʹ��ͬʱ֧�ֶ��DS18B20
  * Date        : 2017��9��13��
  * Author      : XYD
  * Check       :  
*******************************************************************************/



/*********************File include*********************/
#include "DS18B20_V0.1.h"                 

/******************Macro definition********************/

/*************External variable declaration************/

/***************Global variable definition*************/
//CRC = X8 + X5 + X4 + 1
//CRCУ���
const u8 Crc8Table [256]={
0,  94, 188,  226,  97,  63,  221,  131,  194,  156,  126,  32,  163,  253,  31,  65,
157,  195,  33,  127,  252,  162,  64,  30,  95,  1,  227,  189,  62,  96,  130,  220,
35,  125,  159,  193,  66,  28,  254,  160,  225,  191,  93,  3,  128,  222,  60,  98,
190,  224,  2,  92,  223,  129,  99,  61,  124,  34,  192,  158,  29,  67,  161,  255,
70,  24,  250,  164,  39,  121,  155,  197,  132,  218,  56,  102,  229,  187,  89,  7,
219,  133, 103,  57,  186,  228,  6,  88,  25,  71,  165,  251,  120,  38,  196,  154,
101,  59, 217,  135,  4,  90,  184,  230,  167,  249,  27,  69,  198,  152,  122,  36,
248,  166, 68,  26,  153,  199,  37,  123,  58,  100,  134,  216,  91,  5,  231,  185,
140,  210, 48,  110,  237,  179,  81,  15,  78,  16,  242,  172,  47,  113,  147,  205,
17,  79,  173,  243,  112,  46,  204,  146,  211,  141,  111,  49,  178,  236,  14,  80,
175,  241, 19,  77,  206,  144,  114,  44,  109,  51,  209,  143,  12,  82,  176,  238,
50,  108,  142,  208,  83,  13,  239,  177,  240,  174,  76,  18,  145,  207,  45,  115,
202,  148, 118,  40,  171,  245,  23,  73,  8,  86,  180,  234,  105,  55,  213, 139,
87,  9,  235,  181,  54,  104,  138,  212,  149,  203,  41,  119,  244,  170,  72,  22,
233,  183,  85,  11,  136,  214,  52,  106,  43,  117,  151,  201,  74,  20,  246,  168,
116,  42,  200,  150,  21,  75,  169,  247,  182,  232,  10,  84,  215,  137,  107,  53}; 


/*******************************************************************************
  * name        : uint8_t DS18B20_Init(DS18B20_TypeDef *sensor,GPIO_TypeDef* port,uint16_t pin)
  * Function    : ��ʼ��DS18B20
  * Parameters  : DS18B20_TypeDef *sensor-- �¶ȴ������ṹ��
                  GPIO_TypeDef* port-- ���¶ȴ��������ӵ����Ŷ˿ڣ�ȡֵGPIOA/GPIOB/GPIOC...
                  uint16_t pin-- ���¶ȴ��������ӵ����ţ�ȡֵGPIO_Pin_0/GPIO_Pin_1...
  * Returns     : 1--δ��⵽DS18B20��0--��⵽DS18B20
  * Note        : 
  * Author      : XYD
  * Check       : 
*******************************************************************************/
uint8_t DS18B20_Init(DS18B20_TypeDef *sensor,GPIO_TypeDef* port,uint16_t pin)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  if(port == GPIOA)                                               //��ʼ��GPIOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  else if(port == GPIOB)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  else if(port == GPIOC)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  else if(port == GPIOD)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  else if(port == GPIOE)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = pin;	                            //��ʼ��GPIO����			  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(port, &GPIO_InitStructure);
  
  sensor->port= port;                                             //����ת�棬���ں���ʹ��
  sensor->pin = pin;
  
  
  GPIO_SetBits(sensor->port,sensor->pin);    //���1

  DS18B20_Reset(sensor);
  return DS18B20_Check(sensor);
}  
/*******************************************************************************
  * name        : void DS18B20_Reset(DS18B20_TypeDef *sensor)	   	   
  * Function    : ��λDS18B20
  * Parameters  : DS18B20_TypeDef *sensor-- �¶ȴ������ṹ��
  * Returns     : Null
  * Note        : ���Ϊ480uS�ĵ͵�ƽ�ź�
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void DS18B20_Reset(DS18B20_TypeDef *sensor)	   
{        
	DS18B20_IO_OutputMode(sensor);               //����Ϊ���ģʽ
	DS18B20_IO_OutputLow(sensor);;               //����DQ
  DS18B20_Delay_us(750);                    //����750us
  DS18B20_IO_OutputHigh(sensor);;	   
	DS18B20_Delay_us(15);                     // 15us
}

/*******************************************************************************
  * name        : void DS18B20_Check(DS18B20_TypeDef *sensor)	   
  * Function    : ����Ƿ����DS18B20
  * Parameters  : DS18B20_TypeDef *sensor-- �¶ȴ������ṹ��
  * Returns     : 1-- δ��⵽DS18B20,0-- ��⵽DS18B20
  * Note        : 
  * Author      : XYD
  * Check       : 
*******************************************************************************/
uint8_t DS18B20_Check(DS18B20_TypeDef *sensor)
{
  u8 retry=0;
    
	DS18B20_IO_InputMode(sensor);             // �����������ͷŴ��߲�������շ�ʽ��Rx��
  
	/*DSl820 �ȴ�15.60uS ���ҽ��ŷ��ʹ������壨60-240uS�ĵ͵�ƽ�źš���*/
   while ((DS18B20_IO_Input(sensor) == 1) && (retry<200))
	{
		retry++;
		DS18B20_Delay_us(1);
	};	 
    
	if(retry>=200)
        return 1;
	else retry=0;

   while ((DS18B20_IO_Input(sensor) == 0) && (retry<240))
	{
		retry++;
		DS18B20_Delay_us(1);
	};
	if(retry>=240)
        return 1;    
    
	return 0;

}

/*******************************************************************************
  * name        : uint8_t DS18B20_Read_Bit(DS18B20_TypeDef *sensor)   
  * Function    : ��ȡһ��λ
  * Parameters  : DS18B20_TypeDef *sensor-- �¶ȴ������ṹ��
  * Returns     : uint8_t--��ȡ���
  * Note        : 
  * Author      : XYD
  * Check       : 
*******************************************************************************/
uint8_t DS18B20_Read_Bit(DS18B20_TypeDef *sensor) 			 
{
  u8 data;
    
	DS18B20_IO_OutputMode(sensor);        //����Ϊ���ģʽ
  DS18B20_IO_OutputLow(sensor);            //����� 
	DS18B20_Delay_us(3);
  DS18B20_IO_OutputHigh(sensor);           //�����
	DS18B20_IO_InputMode(sensor);         //����Ϊ����ģʽ
	DS18B20_Delay_us(6);
	if(DS18B20_IO_Input(sensor) == 1)     //��ȡһ��λ
  {
      data=1;
  }
  else
  {
      data=0;  
  }
  DS18B20_Delay_us(50); 
    
  return data;
}

/*******************************************************************************
  * name        : uint8_t DS18B20_Read_Byte(DS18B20_TypeDef *sensor)   
  * Function    : ��ȡһ���ֽ�
  * Parameters  : DS18B20_TypeDef *sensor-- �¶ȴ������ṹ��
  * Returns     : uint8_t--��ȡ���
  * Note        : 
  * Author      : XYD
  * Check       : 
*******************************************************************************/
uint8_t DS18B20_Read_Byte(DS18B20_TypeDef *sensor)   
{        
  u8 i,j,dat;
  dat=0;
	for (i=1;i<=8;i++) 
	{
    j=DS18B20_Read_Bit(sensor);
    dat=dat>>1;
    dat+=(j<<7);
  }	

  return dat;
}

/*******************************************************************************
  * name        : void DS18B20_Write_Byte(DS18B20_TypeDef *sensor,uint8_t dat)   
  * Function    : дһ���ֽ�
  * Parameters  : DS18B20_TypeDef *sensor-- �¶ȴ������ṹ��
                  uint8_t dat-- ��д���ֽ�
  * Returns     : uint8_t--��ȡ���
  * Note        : 
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void DS18B20_Write_Byte(DS18B20_TypeDef *sensor,uint8_t dat)
{             
  u8 j;
  u8 testb;
	DS18B20_IO_OutputMode(sensor);                 //����Ϊ���ģʽ
  for (j=1;j<=8;j++) 
	{
    testb=dat&0x01;
    dat=dat>>1;

    DS18B20_IO_OutputLow(sensor);                 // Write 1
    DS18B20_Delay_us(2);  
    if (testb) 
    {
        DS18B20_IO_OutputHigh(sensor);
    }
    DS18B20_Delay_us(55);
    DS18B20_IO_OutputHigh(sensor);
    DS18B20_Delay_us(5);                          
  }
}

/*******************************************************************************
  * name        : uint8_t DS18B20_Get_Temp(DS18B20_TypeDef *sensor)   
  * Function    : ��ȡ�¶�
  * Parameters  : DS18B20_TypeDef *sensor-- �¶ȴ������ṹ�壬ʵ���¶�*10������0.1�棬��Χ-550~1250
  * Returns     : uint8_t--��ȡ�Ƿ�ɹ���0--CRCУ��ͨ����1--CRCУ��ʧ��
  * Note        : 
  * Author      : XYD
  * Check       : 
*******************************************************************************/
uint8_t DS18B20_Get_Temp(DS18B20_TypeDef *sensor)
{
  uint8_t i, temp;
  uint8_t TL,TH;
  uint8_t str[9];
    
                                               //��ʼ�¶�ת��
  DS18B20_Reset(sensor);	   
	DS18B20_Check(sensor);
  DS18B20_Write_Byte(sensor,0xcc);             // skip rom
  DS18B20_Write_Byte(sensor,0x44);             // convert

  //��ʼ��ȡ�¶�
  DS18B20_Reset(sensor);	   
	DS18B20_Check(sensor);
  DS18B20_Write_Byte(sensor,0xcc);             // skip rom
  DS18B20_Write_Byte(sensor,0xbe);             // convert	

  for (i=0;i<9;i++) 
	{    
    str[i] = DS18B20_Read_Byte(sensor);
  }
  
  if(GetCRC(str, 9) == 0)
  {                                           //CRCУ��ͨ��
    TL = str[0];                              // LSB   
    TH = str[1];                              // MSB   
       
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;                               //�¶�Ϊ��  
    }
    else
    {
        temp=1;                               //�¶�Ϊ��         
    }
    sensor->temp= TH<<8 | TL;                 //��ò�������λ��11λ�¶�ֵ

    //ת�� *0.625
    sensor->temp = sensor->temp * 10;
    sensor->temp = sensor->temp >> 4;

    if(temp == 0) 
      sensor->temp= 0 - sensor->temp;         //�¶�Ϊ��
      
    return 0;                                 //CRCУ��ͨ��
  }
  else
      return 1;                               //CRCУ�����
    
} 


/*******************************************************************************
  * name        : uint8_t GetCRC(uint8_t *str, uint32_t length)
  * Function    : CRCУ��
  * Parameters  : uint8_t *str--��У������, uint32_t length--��У�����鳤��
  * Returns     : uint8_t��ȡ�Ƿ�ɹ���0--CRCУ��ͨ����1--CRCУ��ʧ��
  * Note        : 
  * Author      : XYD
  * Check       : 
*******************************************************************************/
uint8_t GetCRC(uint8_t *str, uint32_t length)
{ 
  uint8_t crc_data=0;
  uint32_t i;
  for(i=0;i<length;i++)                       //���У��
  { 
    crc_data = Crc8Table[crc_data^str[i]];
  }
  return (crc_data);
}


/*******************************************************************************
  * name        : uint8_t DS18B20_Get_Serial(DS18B20_TypeDef *sensor,uint8_t *serial)
  * Function    : ��ȡ64λ������
  * Parameters  : DS18B20_TypeDef *sensor-- ָ��������
                  uint8_t *serial--��ȡ��������
  * Returns     : uint8_t--��ȡ�Ƿ�ɹ���0--CRCУ��ͨ����1--CRCУ��ʧ��
  * Note        : 
  * Author      : XYD
  * Check       : 
*******************************************************************************/
uint8_t DS18B20_Get_Serial(DS18B20_TypeDef *sensor,uint8_t *serial)
{
  u8 i;
  u8 crcdata;    

  DS18B20_Reset(sensor);
	DS18B20_Check(sensor);

  DS18B20_Write_Byte(sensor,0x33);                // read rom

  for(i=0;i<8;i++)
  {
     *(serial + i) = DS18B20_Read_Byte(sensor);
  }
  
  crcdata = GetCRC(serial,8);                     // Ϊ0��У����ȷ

  return (crcdata);
}


/*******************************************************************************
  * name        : void DS18B20_IO_OutputMode(DS18B20_TypeDef *sensor)	   
  * Function    : ���ô�������Ӧ��GPIO������Ϊ�������ģʽ
  * Parameters  : DS18B20_TypeDef *sensor-- �¶ȴ������ṹ��
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void DS18B20_IO_OutputMode(DS18B20_TypeDef *sensor)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = sensor->pin;	                         //��ʼ��GPIO����			  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		                 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(sensor->port, &GPIO_InitStructure);
}

/*******************************************************************************
  * name        : void DS18B20_IO_OutputHigh(DS18B20_TypeDef *sensor)	   
  * Function    : ���ô�������Ӧ��GPIO���������
  * Parameters  : DS18B20_TypeDef *sensor-- �¶ȴ������ṹ��
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void DS18B20_IO_OutputHigh(DS18B20_TypeDef *sensor)
{
  GPIO_SetBits(sensor->port,sensor->pin);
}

/*******************************************************************************
  * name        : uint8_t DS18B20_IO_Input(DS18B20_TypeDef *sensor)	   
  * Function    : ��ȡ���������ŵ�ƽ
  * Parameters  : DS18B20_TypeDef *sensor-- �¶ȴ������ṹ��
  * Returns     : uint8_t-- ��ȡ���
  * Note        : 
  * Author      : XYD
  * Check       : 
*******************************************************************************/
uint8_t DS18B20_IO_Input(DS18B20_TypeDef *sensor)
{
  return GPIO_ReadInputDataBit(sensor->port,sensor->pin);
}
/*******************************************************************************
  * name        : void DS18B20_IO_OutputLow(DS18B20_TypeDef *sensor)	   
  * Function    : ���ô�������Ӧ��GPIO���������
  * Parameters  : DS18B20_TypeDef *sensor-- �¶ȴ������ṹ��
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void DS18B20_IO_OutputLow(DS18B20_TypeDef *sensor)
{
  GPIO_ResetBits(sensor->port,sensor->pin);
}

/*******************************************************************************
  * name        : void DS18B20_IO_InputMode(DS18B20_TypeDef *sensor)	   
  * Function    : ���ô�������Ӧ��GPIO������Ϊ��������ģʽ
  * Parameters  : DS18B20_TypeDef *sensor-- �¶ȴ������ṹ��
  * Returns     : Null
  * Note        : 
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void DS18B20_IO_InputMode(DS18B20_TypeDef *sensor)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = sensor->pin;	                         //��ʼ��GPIO����			  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		                   //��������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(sensor->port, &GPIO_InitStructure);
}

/*******************************************************************************
  * name        : void DS18B20_Delay_us(uint16_t n)
  * Function    : ʹ��whileѭ������һ��us�������ʱ��������DS18B20����ʹ�á�
                  ʹ��Systick����ȷ��ʱʱDS18B20�����������ܱ�������ʱ�������жϵ���
  * Parameters  : uint16_t n-- us����ʱ����
  * Returns     : NULL
  * Note        : 
  * Author      : XYD
  * Check       : 
*******************************************************************************/
void DS18B20_Delay_us(uint16_t n)
{
  int i=0;
  
  while(n--)
  {
    i= 10;
    while(i--);
  }
}
/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/
