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
  uint8_t Kp;                       //����ϵ��Proportional
  uint8_t Ki;                       //����ϵ��Integral
  uint8_t Kd;                       //΢��ϵ��Derivative
 
  uint8_t Ek;                       //��ǰ���
  uint8_t Ek1;                      //ǰһ����� e(k-1)
  uint8_t LocSum;                   //�ۼƻ���λ��
}PID_LocTypeDef;

extern PID_LocTypeDef *PID_AV;
extern PID_LocTypeDef *PID_A;
extern PID_LocTypeDef *PID_V;

void PID_Init(void);
float PID_Loc(float SetValue, float ActualValue, PID_LocTypeDef *PID,float Limit);


#endif
