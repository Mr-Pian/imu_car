#ifndef __CONTROL_H
#define __CONTROL_H
#include "M24_EEPROM.h"
#include "Functions.h"

#define PI 3.14159265							//PI圆周率

//#define Control_Frequency  200.0	//编码器读取频率
#define Diameter_67  65.0 				//轮子直径65mm 
//#define EncoderMultiples   4.0 		//编码器倍频数
//#define Encoder_precision  13.0 	//编码器精度 13线
//#define Reduction_Ratio  30.0			//减速比30
//#define Perimeter  210.4867 			//周长，单位mm


typedef struct
{
  float Kp;                       //比例系数Proportional
  float Ki;                       //积分系数Integral
  float Kd;                       //微分系数Derivative
 
  float Ek;                       //当前误差
  float Ek1;                      //前一次误差 e(k-1)
  float LocSum;                   //累计积分位置
}PID_LocTypeDef;

extern PID_LocTypeDef *PID_AV;
extern PID_LocTypeDef PID_A;
extern PID_LocTypeDef PID_VL;
extern PID_LocTypeDef PID_VR;
void PID_Init(void);
float PID_Loc(float SetValue, float ActualValue, PID_LocTypeDef *PID,float Limit);
float PID_Loc_V(float SetValue, float ActualValue, PID_LocTypeDef *PID,float Limit);

#endif
