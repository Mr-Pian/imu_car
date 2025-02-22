/********************************Copyright (c)**********************************
**
**                   (c) Copyright 2023, Main, China, CD.
**                           All Rights Reserved
**
**                           By(GTC From UESTC)
**                           https://www.uestc.edu.cn
**
**----------------------------------文件信息------------------------------------
** 文件名称: angle.h
** 创建人员: Main
** 创建日期: 2023-03-11
** 文档描述: 
**
********************************End of Head************************************/
 
#ifndef __ANGLE_H_
#define __ANGLE_H_



/*头文件----------------------------------------------------------------------*/
#include "HANA_math.h"
/*宏定义-----------------------------------------------------------------------*/

#define RadtoDeg    	57.324841f				//弧度到角度 (弧度 * 180/3.1415)
#define DegtoRad    	0.0174533f				//角度到弧度 (角度 * 3.1415/180)
#define Acc_Gain  		0.0001221f				//加速度变成G (初始化加速度满量程-+4g LSBa = 2*4/65535.0)
#define Gyro_Gain 		0.0305180f				//角速度变成度dps (初始化陀螺仪满量程+-1000 LSBg = 2*1000/65535.0)  0.0305180f
#define Gyro_Gr	    	0.0010641f			  	//角速度变成弧度(3.1415/180 * LSBg) 
#define VAR_GyX				50.0f		            //1/方差   0.001f
#define VAR_GyY				50.0f		            //1/方差   0.001f
#define VAR_GyZ				50.0f		            //1/方差   0.001f
#define VAR_AcX				2000.0f		            //1/方差   0.001f
#define VAR_AcY				2000.0f		            //1/方差   0.001f
#define VAR_AcZ				2000.0f		            //1/方差   0.001f
#define ALPHA			0.0f					//一阶互补滤波系数


#define ANGLE_TO_RAD(x)    ((x) * PI / 180.0)                                   // 角度转换为弧度
#define RAD_TO_ANGLE(x)    ((x) * 180.0 / PI)                                   // 弧度转换为角度


typedef struct
{
	float x;
	float y;
	float z;
}vector3float_t;

typedef struct
{
	int16_t x;
	int16_t y;
	int16_t z;
}vector3int16_t;

typedef volatile struct {
  float q0;
  float q1;
  float q2;
  float q3;
} Quaternion;

typedef struct{
	vector3int16_t acc;// 加速度
	vector3int16_t gyro;// 角速度
	vector3int16_t mag;// 磁场强度
	vector3int16_t acc_offset;//陀螺仪零偏
	vector3int16_t gyro_offset;//加速度计零偏
	vector3int16_t mag_offset;//磁力计零偏
}IMU_st;

typedef struct{
	float roll;
	float pitch;
	float yaw;
}Angle_st;

extern  IMU_st IMU_Data;
extern  Angle_st Angle_Data;
/*函数描述-----------------------------------------------------------*/


/*函数声明*/
void IMU_Calibration(void);
void IMU_DataUpdate(void);
void IMU_GetAngle(float dt);
/********************************End of File************************************/







#endif
