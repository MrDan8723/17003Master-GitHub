/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : Calibration.c
  * Version     : V0.1
  * Description : 17003��Ŀ��������㷨
  * Date        : 2017��11��29��
  * Author      : XYD
  * Check       :  
*******************************************************************************/



/*********************File include*********************/
#include "Calibration.h"                 

/******************Macro definition********************/


/*************External variable declaration************/

/***************Global variable definition*************/

/**************************************************************************************
  * name        : void  FIB_Calibrate(CalibrationStruct *data)
  * Function    : ʹ��������Ͻ���FIB����
  * Parameters  : CalibrationStruct *data-- ����ṹ��
  * Returns     : NULL
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void  FIB_Calibrate(CalibrationStruct *data)
{
  int i;
  float time[4],content[4];                   //������ݻ���
  
  for(i=0; i<4 ; i++)                         //��10Ϊ��ȡ����
  {
    time[i]= log10(data->x[i]);
    content[i]= log10(data->y[i]);
  }
  LinearFit(&data->param[1],&data->param[0],&data->r_square,time,content,4);           //�������ϵ��
}

/**************************************************************************************
  * name        : float FIB_CalcContent(CalibrationStruct *data,float time)
  * Function    : ���ݲ�õ�ʱ����ȡFIB�ĺ���
  * Parameters  : CalibrationStruct *data-- ����ṹ��
  * Returns     : float-- ������������
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
float FIB_CalcContent(CalibrationStruct *data,float time)
{
  float log10_time= log10(time);
  float log10_content;
  
  log10_content= data->param[0] + data->param[1]*log10_time ;
  
  return pow(10,log10_content);
}

/**************************************************************************************
  * name        : void  AT3_Calibrate(CalibrationStruct *data)
  * Function    : ʹ��������Ͻ���AT3����
  * Parameters  : CalibrationStruct *data-- ����ṹ��
  * Returns     : NULL
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void AT3_Calibrate(CalibrationStruct *data)
{
  LinearFit(&data->param[1],
            &data->param[0],
            &data->r_square,
            data->x,
            data->y,
            data->num);
}

/**************************************************************************************
  * name        : float AT3_CalcActive(CalibrationStruct *data,float od)
  * Function    : ����AT3�Ļ���
  * Parameters  : CalibrationStruct *data-- ����ṹ��
                  float od-- ��õ�ODֵ
  * Returns     : NULL
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
float AT3_CalcActive(CalibrationStruct *data,float od)
{
  return data->param[0] + data->param[1]*od;
}

/**************************************************************************************
  * name        : void FDP_Calibrate(CalibrationStruct *data)
  * Function    : ʹ�����ζ���ʽ����FDP���
  * Parameters  : CalibrationStruct *data-- ����ṹ��
  * Returns     : NULL
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void FDP_Calibrate(CalibrationStruct *data)
{
//  CubicFit(data->param,
//           &data->r_square,
//           data->x,
//           data->y,
//           data->num);
  
  Spline3(data->param,data->x,data->y,data->num);
}

/**************************************************************************************
  * name        : float FDP_CalcContent(CalibrationStruct *data,float od)
  * Function    : ����FDP�ĺ���
  * Parameters  : CalibrationStruct *data-- ����ṹ��
                  float od-- ��õ�ODֵ
  * Returns     : NULL
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
float FDP_CalcContent(CalibrationStruct *data,float od)
{
  float value;
//  return data->param[0] + 
//         data->param[1]*od +
//         data->param[2]*od*od +
//         data->param[3]*pow(od,3);
  
  CalculateSpline3Value(od,&value,data->x,data->y,data->num,data->param);
  return value;
}

/**************************************************************************************
  * name        : void DD_Calibrate(CalibrationStruct *data)
  * Function    : ʹ�����ζ���ʽ����DD���
  * Parameters  : CalibrationStruct *data-- ����ṹ��
  * Returns     : NULL
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void DD_Calibrate(CalibrationStruct *data)
{
  CubicFit(data->param,
           &data->r_square,
           data->x,
           data->y,
           data->num);
}

/**************************************************************************************
  * name        : float DD_CalcContent(CalibrationStruct *data,float od)
  * Function    : ����DD�ĺ���
  * Parameters  : CalibrationStruct *data-- ����ṹ��
                  float od-- ��õ�ODֵ
  * Returns     : NULL
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
float DD_CalcContent(CalibrationStruct *data,float od)
{
  return data->param[0] + 
         data->param[1]*od +
         data->param[2]*od*od +
         data->param[3]*od*od*od;
//         data->param[3]*pow(od,3);
}

/**************************************************************************************
  * name        : void  LinearFit(float *k,float *b,float *rr,float x[],float y[],int cnt)
  * Function    : ʹ����С���˷����������Ϻ��������y= k*x + b�е�ϵ��k��b
  * Parameters  : float *k-- ϵ��k���������
                  float *b-- ϵ��b���������
                  float *rr-- ��϶�Rƽ��
                  float x[]-- ������x������
                  float y[]-- ������y������
                  int cnt--  ���������
  * Returns     : NULL
  * Note        : ��С���˷�
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void  LinearFit(float *k,float *b,float *rr,float x[],float y[],int cnt)
{
  int i;
  float x_avg,y_avg;
  float xx_avg;                                                //x*x��ƽ��ֵ
  float yy_avg;                                                //y*y��ƽ��ֵ
  float xy_avg;                                                //x*y��ƽ��ֵ
  float temp1=0.0,temp2=0.0;                                                  
  
  x_avg= GetAvg(x,cnt);                                        //��ȡx��ƽ��ֵ
  y_avg= GetAvg(y,cnt);                                        //��ȡy��ƽ��ֵ
  xx_avg= GetProductAvg(x,x,cnt);                              //��ȡx*x��ƽ��ֵ
  yy_avg= GetProductAvg(y,y,cnt);                              //��ȡy*y��ƽ��ֵ
  xy_avg= GetProductAvg(x,y,cnt);                              //��ȡx*y��ƽ��ֵ
  
  *k= (xy_avg - x_avg*y_avg)/(xx_avg - x_avg*x_avg);           //��Ҳ��֪��Ϊʲô��������ȥ
  *b= y_avg - *k * x_avg;                                      //y= k*x + b
  
  for(i=0; i<cnt; i++)                                         //������϶�
  {
    temp1 += ((*k * x[i] + *b) - y_avg) * ((*k * x[i] + *b) - y_avg);
    temp2 += (y[i] - y_avg) * (y[i] - y_avg);
  }
  *rr= temp1/temp2;
}


/**************************************************************************************
  * name        : float GetAvg(float a[],int cnt)
  * Function    : ��ƽ��ֵ
  * Parameters  : float a[]-- ��������
                  int cnt-- ����Ԫ�ظ���
  * Returns     : float-- ������
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
float GetAvg(float a[],int cnt)
{
  int i;
  float result=0.0;                            //����
  
  for(i=0; i<cnt; i++)                         //���
  {
    result += a[i];
  }
  
  return result = result/cnt;                  //��ƽ��
}

/**************************************************************************************
  * name        : float GetProductAvg(float a[],float b[],int cnt)
  * Function    : �����������ӦԪ�س˻���ƽ��ֵ
  * Parameters  : float a[]-- ��������
                  float b[]-- ��������
                  int cnt-- ����Ԫ�ظ���
  * Returns     : float-- ������
  * Note        : ��������Ԫ�ظ���������ͬ
  * Author      : XYD
  * Check       : 
**************************************************************************************/
float GetProductAvg(float a[],float b[],int cnt)
{
  int i;
  float result=0.0;
  
  for(i=0; i<cnt; i++)                         //��˻�
  {
    result += a[i] * b[i];                   
  }
  
  return result = result/cnt;                  //��ƽ��
}

/**************************************************************************************
  * name        : void CalculateSpline3Value(float xi,float *yi,float x[],float y[],float result[])
  * Function    : ��������������ֵ����������
  * Parameters  : 
  * Returns     : 
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CalculateSpline3Value(float xi,float *yi,float x[],float y[],int num,float result[])
{
  int pos;
  float h0;
  
  //Ѱ�Ҵ���ֵ���λ��
  if(xi >= x[num-1])
  {
    pos= num-2;
  }
  else
  {
    pos=0;
    
    while((pos+1) < num)
    {
      if(xi >= x[pos] && xi <= x[pos+1])
      {
        break;
      }
      pos++;
    }
  }
  
  //���ݹ�ʽ�������ֵ��
  if((pos+1) < num)
  {
    h0= x[pos+1] - x[pos];
    *yi= result[pos]*pow(xi-x[pos+1],2.0)*(xi-x[pos])/pow(h0,2.0)+
         result[pos+1]*pow(xi-x[pos],2.0)*(xi-x[pos+1])/pow(h0,2.0)+
         y[pos]*pow(xi-x[pos+1],2.0)*(2*(xi-x[pos])+h0)/pow(h0,3.0)+
         y[pos+1]*pow(xi-x[pos],2.0)*(-2*(xi-x[pos+1])+h0)/pow(h0,3.0);
  }
}
/**************************************************************************************
  * name        : void Spline3(float result[],float x[],float y[],int n)
  * Function    : ����������ֵ
  * Parameters  : 
  * Returns     : 
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void Spline3(float result[],float x[],float y[],int n)
{
  float h0,h1;
//  float dy0=0,dyn= 0;                                      //x[0],x[n]��һ�׵��������ڵ�һ�߽�����
  float ddy0= 0,ddyn= 0;                                   //x[0],x[n]�Ķ��׵�����������Ȼ�߽�����
  float ak,bk,ck;
  float *buf;
  int i;
  
  buf= (float *)malloc((n+1)*n);                          //����һ���ڴ�
  memset(buf,0,sizeof(float)*(n+1)*n);                    //��ʼ���ڴ�
 
//  //��һ�߽��������󷽳����ϵ������  
//  h0= x[1] - x[0];
//  h1= x[2] - x[1];
//  ak= h1 / (h0+h1);
//  bk= h0 / (h0+h1);
//  ck=3*(h0*(y[2]-y[1])/h1+h1*(y[1]-y[0])/h0)/(h0+h1);
//  
//  buf[0*(n+1)+0]= 1;
//  buf[0*(n+1)+n]=dy0;  
//  
//  buf[1*(n+1)+1]=2;  
//  buf[1*(n+1)+2]=bk;  
//  buf[1*(n+1)+n]=ck-ak*dy0;  

//  for(i=2;i<n-2;i++)  
//  {  
//    h0=h1;  
//    h1=x[i+1]-x[i];  
//    ak=h1/(h0+h1);  
//    bk=h0/(h0+h1);  
//    ck=3*(h0*(y[i+1]-y[i])/h1+h1*(y[i]-y[i-1])/h0)/(h0+h1);  

//    buf[i*(n+1)+i-1]=ak;  
//    buf[i*(n+1)+i]=2;  
//    buf[i*(n+1)+i+1]=bk;  
//    buf[i*(n+1)+n]=ck;  
//  }  
//  h0=h1;  
//  h1=x[n]-x[n-1];  
//  ak=h1/(h0+h1);  
//  bk=h0/(h0+h1);  
//  ck=3*(h0*(y[n]-y[n-1])/h1+h1*(y[n-1]-y[n-2])/h0)/(h0+h1);  
//  buf[(n-2)*(n+1)+n-3]=ak;  
//  buf[(n-2)*(n+1)+n-2]=bk;  
//  buf[(n-2)*(n+1)+n]=ck-bk*dyn;  
//  
//  buf[(n-1)*(n+1)+n-1]=1;  
//  buf[(n-1)*(n+1)+n]=dyn;  

  //��Ȼ�߽��������󷽳����ϵ������
  h0= x[1] - x[0];
  h1= x[2] - x[1];
  ak= h1 / (h0+h1);
  bk= h0 / (h0+h1);
  ck=3*(y[1]-y[0])/h0 - ddy0*h0/2; 
  
  buf[0*(n+1)+0]= 2;                                        //�����0��
  buf[0*(n+1)+1]= 1;
  buf[0*(n+1)+n]=ck;  

  for(i=1;i<n-1;i++)                                        //�����1--(n-1)��
  {  
    h0=h1;                                                    //ǰһ���䳤��
    h1=x[i+1]-x[i];                                           //��ǰ���䳤��
    ak=h1/(h0+h1);  
    bk=h0/(h0+h1);  
    ck=3 * (h0*(y[i+1]-y[i])/h1+h1*(y[i]-y[i-1])/h0) / (h0+h1);
    
    buf[i*(n+1)+i-1]=ak;  
    buf[i*(n+1)+i]=2;  
    buf[i*(n+1)+i+1]=bk;  
    buf[i*(n+1)+n]=ck;  
  }  
  
  h0=h1;                                                      //���һ��
  h1=x[n]-x[n-1];   
  ck=3*(y[n]-y[n-1])/h0 + ddyn*h0/2;   
  buf[(n-1)*(n+1)+n-2]=1;
  buf[(n-1)*(n+1)+n-1]=2;  
  buf[(n-1)*(n+1)+n]=ck;

  TridiagonalMatrix(buf,n,result);                  //׷�Ϸ�������ԽǾ��󣬽�Ϊ����ֵ���һ�׵���ֵ
  
  free(buf);
  buf= NULL;
}
/**************************************************************************************
  * name        : void TridiagonalMatrix(float a[],int cnt,float result[])
  * Function    : ׷�Ϸ�������ԽǾ���
  * Parameters  : 
  * Returns     : 
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void TridiagonalMatrix(float a[],int num,float result[])
{
  int i;
  
  for(i=0; i < num-1; i++)
  {
    a[i*(num+1)+ i+1] /= a[i*(num+1) + i];
    a[i*(num+1)+ num] /= a[i*(num+1) + i];
    
    a[(i+1)*(num+1)+i+1] -= a[(i+1)*(num+1)+i] * a[i*(num+1)+i+1];
    a[(i+1)*(num+1)+num] -= a[(i+1)*(num+1)+i] * a[i*(num+1)+num];
  }
  
  //�ش����
  result[num-1]= a[(num-1)*(num+1)+num] / a[(num-1)*(num+1)+num-1];
  for(i= num-1-1; i>=0 ; i--)
  {
    result[i]= (a[i*(num+1)+num] - a[i*(num+1)+num+1]*result[i+1]);
  }
}

/**************************************************************************************
  * name        : void GaussSolve(float para[],int num,float a[],float b[])
  * Function    : ��˹��Ԫ��������ʽϵ��
  * Parameters  : 
  * Returns     : 
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void GaussSolve(float para[],int num,float a[],float b[])
{
  int i,j,k,r;
  float max;
  
  for(k=0; k < num-1; k++)
  {
    max= fabs(a[k*num+k]);
    r=k;
    
    for (i=k+1;i<num-1;i++)  
    {  
      if (max<fabs(a[i*num+i]))  
      {  
        max=fabs(a[i*num+i]);  
        r=i;  
      }  
    }
    
    if (r!=k)  
    {  
      for (i=0;i<num;i++)             //change array:A[k]&A[r]  
      {  
        max=a[k*num+i];  
        a[k*num+i]=a[r*num+i];  
        a[r*num+i]=max;  
      }  

      max=b[k];                       //change array:b[k]&b[r]  
      b[k]=b[r];  
      b[r]=max;  
    }

    for (i=k+1;i<num;i++)  
    {  
      for (j=k+1;j<num;j++)  
        a[i*num+j] -= a[i*num+k]*a[k*num+j]/a[k*num+k]; 
      
      b[i] -= a[i*num+k]*b[k]/a[k*num+k];  
    }  
  }
  
  for (i=num-1;i>=0;para[i] /= a[i*num+i],i--)  
  {  
    for (j=i+1,para[i]=b[i];j<num;j++)  
      para[i] -= a[i*num+j]*para[j];  
  }  
}

/**************************************************************************************
  * name        : void CubicFit(float result[],float x[],float y[],int num)
  * Function    : ���ζ���ʽ���
  * Parameters  : 
  * Returns     : 
  * Note        : ϵ��������������result[]��
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CubicFit(float result[],float *rr,float x[],float y[],int num)
{
  int i,j;
  int poly_n=3;                                                   //���ζ���ʽ
  float tempx[6],tempy[6],sumxx[7],sumxy[4],ata[16];
  float temp0,temp1,temp2,y_avg;
  
//  tempx= malloc(sizeof(float) * num);                             //�ڴ����
//  tempy= malloc(sizeof(float) * num);
//  sumxx= malloc(sizeof(float) * (2*poly_n+1));
//  sumxy= malloc(sizeof(float) * (poly_n+1));
//  ata  = malloc(sizeof(float) * (poly_n+1)*(poly_n+1));

  for(i=0; i<num; i++)
  {
    tempx[i]= 1.0f;
    tempy[i]= y[i];
  }

  for (i=0;i<2*poly_n+1;i++)  
  {  
      for (sumxx[i]=0,j=0;j<num;j++)  
      {  
          sumxx[i]+=tempx[j];  
          tempx[j]*=x[j];  
      }  
  }  

  for (i=0;i<poly_n+1;i++)  
  {  
      for (sumxy[i]=0,j=0;j<num;j++)  
      {  
          sumxy[i]+=tempy[j];  
          tempy[j]*=x[j];  
      }  
  }  

  for (i=0;i<poly_n+1;i++)  
  {  
      for (j=0;j<poly_n+1;j++)  
      {  
          ata[i*(poly_n+1)+j]=sumxx[i+j];  
      }  
  }  
  GaussSolve(result,poly_n+1,ata,sumxy);    

  y_avg= GetAvg(y,num);                                        //����y��ƽ��ֵ
  for(i=0; i<num; i++)                                         //������϶�
  {
    temp0  = result[0] + result[1]*x[i] + result[2]*pow(x[i],2) + result[3]*pow(x[i],3);
    temp1 += pow(temp0-y_avg,2);
    
    temp2 += (y[i] - y_avg) * (y[i] - y_avg);
  }
  *rr= temp1/temp2;
  
//  free(tempx);                                                    //�ͷ��ڴ�
//  free(sumxx);  
//  free(tempy);  
//  free(sumxy);  
//  free(ata); 
}
/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/
 