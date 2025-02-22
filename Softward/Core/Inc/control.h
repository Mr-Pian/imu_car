#ifndef __CONTROL_H
#define __CONTROL_H
#include "M24_EEPROM.h"
#include "Functions.h"

#define PI 3.14159265							//PIԲ����

//#define Control_Frequency  200.0	//��������ȡƵ��
#define Diameter_67  65.0 				//����ֱ��65mm 
//#define EncoderMultiples   4.0 		//��������Ƶ��
//#define Encoder_precision  13.0 	//���������� 13��
//#define Reduction_Ratio  30.0			//���ٱ�30
//#define Perimeter  210.4867 			//�ܳ�����λmm


typedef struct
{
  float Kp;                       //����ϵ��Proportional
  float Ki;                       //����ϵ��Integral
  float Kd;                       //΢��ϵ��Derivative
 
  float Ek;                       //��ǰ���
  float Ek1;                      //ǰһ����� e(k-1)
  float LocSum;                   //�ۼƻ���λ��
}PID_LocTypeDef;

extern PID_LocTypeDef *PID_AV;
extern PID_LocTypeDef PID_A;
extern PID_LocTypeDef PID_VL;
extern PID_LocTypeDef PID_VR;
void PID_Init(void);
float PID_Loc(float SetValue, float ActualValue, PID_LocTypeDef *PID,float Limit);
float PID_Loc_V(float SetValue, float ActualValue, PID_LocTypeDef *PID,float Limit);

#endif
