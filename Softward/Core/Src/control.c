#include "control.h"

PID_LocTypeDef *PID_AV;
PID_LocTypeDef *PID_A;
PID_LocTypeDef *PID_V;

/************************************************
�������� �� PID_Init
��    �� �� PIDλ��(Location)����
��    �� ��
�� �� ֵ �� 
*************************************************/
void PID_Init(void)
{
	EEPROM_ReadByte(SPEED_P+3, &(PID_AV->Kp));
	EEPROM_ReadByte(SPEED_I+3, &(PID_AV->Ki));
	EEPROM_ReadByte(SPEED_D+3, &(PID_AV->Kd));
	PID_AV->Ek=0;
	PID_AV->Ek1=0;
	PID_AV->LocSum=0;
	EEPROM_ReadByte(ANGLE_P+3, &(PID_A->Kp));
	EEPROM_ReadByte(ANGLE_I+3, &(PID_A->Ki));
	EEPROM_ReadByte(ANGLE_D+3, &(PID_A->Kd));
	PID_A->Ek=0;
	PID_A->Ek1=0;
	PID_A->LocSum=0;
}



/************************************************
�������� �� PID_Loc
��    �� �� PIDλ��(Location)����
��    �� �� SetValue ------ ����ֵ(����ֵ)
            ActualValue --- ʵ��ֵ(����ֵ)
            PID ----------- PID���ݽṹ
�� �� ֵ �� PIDLoc -------- PIDλ��
*************************************************/
float PID_Loc(float SetValue, float ActualValue, PID_LocTypeDef *PID,float Limit)
{
  float PIDLoc;                                  //λ��
	
 	PID->Ek = SetValue - ActualValue;
 	PID->LocSum += PID->Ek;                         //�ۼ����
	
	if(PID->LocSum>Limit)		PID->LocSum=Limit;
	if(PID->LocSum<-Limit)		PID->LocSum=-Limit;
 	PIDLoc = PID->Kp * PID->Ek + (PID->Ki * PID->LocSum) + PID->Kd * (PID->Ek - PID->Ek1);
	
 	PID->Ek1 = PID->Ek;  
	return PIDLoc;
}




float PID_Loc_V(float SetValue, float ActualValue, PID_LocTypeDef *PID,float Limit)
{
  float PIDLoc;                                  //λ��
	
 	PID->Ek = SetValue - ActualValue;
	PID->Ek	= 0.3*PID->Ek+0.7*PID->Ek1;
 	PID->LocSum += PID->Ek;                         //�ۼ����
	
	if(PID->LocSum>Limit)		PID->LocSum=Limit;
	if(PID->LocSum<-Limit)		PID->LocSum=-Limit;
 	PIDLoc = PID->Kp * PID->Ek + (PID->Ki * PID->LocSum) + PID->Kd * (PID->Ek - PID->Ek1);
	
 	PID->Ek1 = PID->Ek;  
	return PIDLoc;
}
