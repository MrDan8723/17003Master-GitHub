/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : Calibration.c
  * Version     : V0.1
  * Description : 17003项目定标相关算法
  * Date        : 2017年11月29日
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
  * Function    : 使用线性拟合进行FIB定标
  * Parameters  : CalibrationStruct *data-- 定标结构体
  * Returns     : NULL
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void  FIB_Calibrate(CalibrationStruct *data)
{
  int i;
  float time[4],content[4];                   //拟合数据缓存
  
  for(i=0; i<4 ; i++)                         //以10为底取对数
  {
    time[i]= log10(data->x[i]);
    content[i]= log10(data->y[i]);
  }
  LinearFit(&data->param[1],&data->param[0],&data->r_square,time,content,4);           //计算拟合系数
}

/**************************************************************************************
  * name        : float FIB_CalcContent(CalibrationStruct *data,float time)
  * Function    : 根据测得的时间求取FIB的含量
  * Parameters  : CalibrationStruct *data-- 定标结构体
  * Returns     : float-- 计算结果，含量
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
  * Function    : 使用线性拟合进行AT3定标
  * Parameters  : CalibrationStruct *data-- 定标结构体
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
  * Function    : 计算AT3的活性
  * Parameters  : CalibrationStruct *data-- 定标结构体
                  float od-- 测得的OD值
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
  * Function    : 使用三次多项式进行FDP拟合
  * Parameters  : CalibrationStruct *data-- 定标结构体
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
  * Function    : 计算FDP的含量
  * Parameters  : CalibrationStruct *data-- 定标结构体
                  float od-- 测得的OD值
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
  * Function    : 使用三次多项式进行DD拟合
  * Parameters  : CalibrationStruct *data-- 定标结构体
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
  * Function    : 计算DD的含量
  * Parameters  : CalibrationStruct *data-- 定标结构体
                  float od-- 测得的OD值
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
  * Function    : 使用最小二乘法求解线性拟合函数，求解y= k*x + b中的系数k和b
  * Parameters  : float *k-- 系数k计算结果输出
                  float *b-- 系数b计算结果输出
                  float *rr-- 拟合度R平方
                  float x[]-- 样本点x轴数据
                  float y[]-- 样本点y轴数据
                  int cnt--  样本点个数
  * Returns     : NULL
  * Note        : 最小二乘法
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void  LinearFit(float *k,float *b,float *rr,float x[],float y[],int cnt)
{
  int i;
  float x_avg,y_avg;
  float xx_avg;                                                //x*x的平均值
  float yy_avg;                                                //y*y的平均值
  float xy_avg;                                                //x*y的平均值
  float temp1=0.0,temp2=0.0;                                                  
  
  x_avg= GetAvg(x,cnt);                                        //求取x的平均值
  y_avg= GetAvg(y,cnt);                                        //求取y的平均值
  xx_avg= GetProductAvg(x,x,cnt);                              //求取x*x的平均值
  yy_avg= GetProductAvg(y,y,cnt);                              //求取y*y的平均值
  xy_avg= GetProductAvg(x,y,cnt);                              //求取x*y的平均值
  
  *k= (xy_avg - x_avg*y_avg)/(xx_avg - x_avg*x_avg);           //我也不知道为什么，看文献去
  *b= y_avg - *k * x_avg;                                      //y= k*x + b
  
  for(i=0; i<cnt; i++)                                         //计算拟合度
  {
    temp1 += ((*k * x[i] + *b) - y_avg) * ((*k * x[i] + *b) - y_avg);
    temp2 += (y[i] - y_avg) * (y[i] - y_avg);
  }
  *rr= temp1/temp2;
}


/**************************************************************************************
  * name        : float GetAvg(float a[],int cnt)
  * Function    : 求平均值
  * Parameters  : float a[]-- 待求数组
                  int cnt-- 数组元素个数
  * Returns     : float-- 计算结果
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
float GetAvg(float a[],int cnt)
{
  int i;
  float result=0.0;                            //清零
  
  for(i=0; i<cnt; i++)                         //求和
  {
    result += a[i];
  }
  
  return result = result/cnt;                  //求平均
}

/**************************************************************************************
  * name        : float GetProductAvg(float a[],float b[],int cnt)
  * Function    : 求两个数组对应元素乘积的平均值
  * Parameters  : float a[]-- 待求数组
                  float b[]-- 待求数组
                  int cnt-- 数组元素个数
  * Returns     : float-- 计算结果
  * Note        : 两个数组元素个数必须相同
  * Author      : XYD
  * Check       : 
**************************************************************************************/
float GetProductAvg(float a[],float b[],int cnt)
{
  int i;
  float result=0.0;
  
  for(i=0; i<cnt; i++)                         //求乘积
  {
    result += a[i] * b[i];                   
  }
  
  return result = result/cnt;                  //求平均
}

/**************************************************************************************
  * name        : void CalculateSpline3Value(float xi,float *yi,float x[],float y[],float result[])
  * Function    : 根据三次样条插值结果反向计算
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
  
  //寻找待插值点的位置
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
  
  //根据公式计算待插值点
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
  * Function    : 三次样条插值
  * Parameters  : 
  * Returns     : 
  * Note        : 
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void Spline3(float result[],float x[],float y[],int n)
{
  float h0,h1;
//  float dy0=0,dyn= 0;                                      //x[0],x[n]的一阶导数，用于第一边界条件
  float ddy0= 0,ddyn= 0;                                   //x[0],x[n]的二阶导数，用于自然边界条件
  float ak,bk,ck;
  float *buf;
  int i;
  
  buf= (float *)malloc((n+1)*n);                          //申请一段内存
  memset(buf,0,sizeof(float)*(n+1)*n);                    //初始化内存
 
//  //第一边界条件下求方程组的系数矩阵  
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

  //自然边界条件下求方程组的系数矩阵
  h0= x[1] - x[0];
  h1= x[2] - x[1];
  ak= h1 / (h0+h1);
  bk= h0 / (h0+h1);
  ck=3*(y[1]-y[0])/h0 - ddy0*h0/2; 
  
  buf[0*(n+1)+0]= 2;                                        //矩阵第0行
  buf[0*(n+1)+1]= 1;
  buf[0*(n+1)+n]=ck;  

  for(i=1;i<n-1;i++)                                        //矩阵第1--(n-1)行
  {  
    h0=h1;                                                    //前一区间长度
    h1=x[i+1]-x[i];                                           //当前区间长度
    ak=h1/(h0+h1);  
    bk=h0/(h0+h1);  
    ck=3 * (h0*(y[i+1]-y[i])/h1+h1*(y[i]-y[i-1])/h0) / (h0+h1);
    
    buf[i*(n+1)+i-1]=ak;  
    buf[i*(n+1)+i]=2;  
    buf[i*(n+1)+i+1]=bk;  
    buf[i*(n+1)+n]=ck;  
  }  
  
  h0=h1;                                                      //最后一行
  h1=x[n]-x[n-1];   
  ck=3*(y[n]-y[n-1])/h0 + ddyn*h0/2;   
  buf[(n-1)*(n+1)+n-2]=1;
  buf[(n-1)*(n+1)+n-1]=2;  
  buf[(n-1)*(n+1)+n]=ck;

  TridiagonalMatrix(buf,n,result);                  //追赶法求解三对角矩阵，解为各插值点的一阶导数值
  
  free(buf);
  buf= NULL;
}
/**************************************************************************************
  * name        : void TridiagonalMatrix(float a[],int cnt,float result[])
  * Function    : 追赶法求解三对角矩阵
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
  
  //回带求解
  result[num-1]= a[(num-1)*(num+1)+num] / a[(num-1)*(num+1)+num-1];
  for(i= num-1-1; i>=0 ; i--)
  {
    result[i]= (a[i*(num+1)+num] - a[i*(num+1)+num+1]*result[i+1]);
  }
}

/**************************************************************************************
  * name        : void GaussSolve(float para[],int num,float a[],float b[])
  * Function    : 高斯消元法求解多项式系数
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
  * Function    : 三次多项式拟合
  * Parameters  : 
  * Returns     : 
  * Note        : 系数结果升序放至在result[]中
  * Author      : XYD
  * Check       : 
**************************************************************************************/
void CubicFit(float result[],float *rr,float x[],float y[],int num)
{
  int i,j;
  int poly_n=3;                                                   //三次多项式
  float tempx[6],tempy[6],sumxx[7],sumxy[4],ata[16];
  float temp0,temp1,temp2,y_avg;
  
//  tempx= malloc(sizeof(float) * num);                             //内存分配
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

  y_avg= GetAvg(y,num);                                        //计算y的平均值
  for(i=0; i<num; i++)                                         //计算拟合度
  {
    temp0  = result[0] + result[1]*x[i] + result[2]*pow(x[i],2) + result[3]*pow(x[i],3);
    temp1 += pow(temp0-y_avg,2);
    
    temp2 += (y[i] - y_avg) * (y[i] - y_avg);
  }
  *rr= temp1/temp2;
  
//  free(tempx);                                                    //释放内存
//  free(sumxx);  
//  free(tempy);  
//  free(sumxy);  
//  free(ata); 
}
/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/
 