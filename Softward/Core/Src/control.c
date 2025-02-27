#include "control.h"


PID_LocTypeDef PID_D={100.0/1000.0,3.0/10000000.0,60.0/1000.0,0,0,0};
PID_LocTypeDef PID_A={150.0/1000.0,2.0/100000.0,25000.0/1000.0,0,0,0};
PID_LocTypeDef PID_VL={6900.0/1000.0,20.0/1000.0,30000.0/1000.0,0,0,0};// {3900.0/1000.0,9.0/1000.0,50000.0/1000.0,0,0,0};
PID_LocTypeDef PID_VR={7200.0/1000.0,20.0/1000.0,20000.0/1000.0,0,0,0};


/************************************************
函数名称 ： PID_Init
功    能 ： PID初始化
参    数 ：
返 回 值 ： 
*************************************************/
void PID_Init(void)
{
//	EEPROM_ReadByte(SPEED_P+3, &(PID_AV->Kp));
//	EEPROM_ReadByte(SPEED_I+3, &(PID_AV->Ki));
//	EEPROM_ReadByte(SPEED_D+3, &(PID_AV->Kd));
//	PID_AV->Ek=0;
//	PID_AV->Ek1=0;
//	PID_AV->LocSum=0;
//	EEPROM_ReadByte(ANGLE_P+3, &(PID_A->Kp));
//	EEPROM_ReadByte(ANGLE_I+3, &(PID_A->Ki));
//	EEPROM_ReadByte(ANGLE_D+3, &(PID_A->Kd));
//	PID_A->Ek=0;
//	PID_A->Ek1=0;
//	PID_A->LocSum=0;

}



/************************************************
函数名称 ： PID_Loc
功    能 ： PID位置(Location)计算
参    数 ： SetValue ------ 给定值(期望值)
            ActualValue --- 实际值(反馈值)
            PID ----------- PID数据结构
返 回 值 ： PIDLoc -------- PID位置
*************************************************/
float PID_Loc(float SetValue, float ActualValue, PID_LocTypeDef *PID,float Limit)
{
  float PIDLoc;                                  //位置
	
 	PID->Ek = SetValue - ActualValue;
 	PID->LocSum += PID->Ek;                         //累计误差
	
	if(PID->LocSum>Limit)		PID->LocSum=Limit;
	if(PID->LocSum<-Limit)		PID->LocSum=-Limit;
 	PIDLoc = PID->Kp * PID->Ek + (PID->Ki * PID->LocSum) + PID->Kd * (PID->Ek - PID->Ek1);
	
 	PID->Ek1 = PID->Ek;  
	return PIDLoc;
}




float PID_Loc_V(float SetValue, float ActualValue, PID_LocTypeDef *PID,float Limit)
{
  float PIDLoc;                                  //位置
	
 	PID->Ek = SetValue - ActualValue;
 	PID->LocSum += PID->Ek;                         //累计误差
	
	if(PID->LocSum>Limit)		PID->LocSum=Limit;
	if(PID->LocSum<-Limit)		PID->LocSum=-Limit;
 	PIDLoc = PID->Kp * PID->Ek + (PID->Ki * PID->LocSum) + PID->Kd * (PID->Ek - PID->Ek1);
	
 	PID->Ek1 = PID->Ek;  
	return PIDLoc;
}
