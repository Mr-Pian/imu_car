#include "tb6612.h"
#include "tim.h"
#include "ws2812.h"
#include "stdio.h"
/************************************************************************************************************
** int Motor_SetSpeed(uint8_t Mode,float speed, uint8_t L_R)                  				                     **                                                              
** ��������������һ�����ٶ�                                                                    		         **
** ����˵����Foward or Backward�� �ٶȣ� ���ҵ��                          					                         **   
** �������أ�int                                                                                            **
************************************************************************************************************/

//speed �ķ�Χ��0-1000


int Motor_SetSpeed(int Mode,int speed, uint8_t L_R)
{
	
	int pulse = 0;  
	if(0 <= speed && speed <= 1000)
	{
		pulse = 1000 - speed;
		
		if (L_R == 1)  //�ұ�ת
		{
			if (Mode == 1)
			{
				HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,GPIO_PIN_SET);
			}
			if (Mode == 0)
			{
				HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,GPIO_PIN_RESET);	
			}
			__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,pulse);
		}
		if (L_R == 0)  //���ת
		{
			if (Mode == 1)
			{
				HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(BIN2_GPIO_Port,BIN2_Pin,GPIO_PIN_SET);
			}
			if (Mode == 0)
			{
				HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(BIN2_GPIO_Port,BIN2_Pin,GPIO_PIN_RESET);	
			}
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
		}
		return 0;
	}
	else 
	{
		return 1;
	}
}

/************************************************************************************************************
** int Motor_Start(uint8_t L_R)                  				                                                   **                                                              
** ��������������һ����                                                                       		         **
** ����˵�������ҵ��                          					                                                   **   
** �������أ�uint8_t                                                                                        **
************************************************************************************************************/

int Motor_Start(uint8_t L_R)
{
	if (L_R == 0)  //���
	{
		HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
		return 0;
	}
	if (L_R == 1)  //�ұ�
	{
		HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);
		return 0;
	}
	if (L_R == 2)  //ͬʱ
	{
		HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);
		return 0;
	}
	else
	{
		return 1;
	}
}

/************************************************************************************************************
** int Motor_Off(uint8_t L_R)                  				                                                     **                                                              
** ����������ֹͣһ����                                                                        		         **
** ����˵�������ҵ��                          					                                                   **   
** �������أ�uint8_t                                                                                        **
************************************************************************************************************/

int Motor_Off(uint8_t L_R)
{
	if (L_R == 0)  //���
	{
		HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
		return 0;
	}
	if (L_R == 1)  //�ұ�
	{
		HAL_TIM_PWM_Stop(&htim5,TIM_CHANNEL_1);
		return 0;
	}
	if (L_R == 2)  //ͬʱ
	{
		HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim5,TIM_CHANNEL_1);
		return 0;
	}
	else
	{
		return 1;
	}
}

/************************************************************************************************************
** long Get_Speed(uint8_t L_R)                  				                                                   **                                                              
** ������������ȡһ�����ٶ�                                                                        		     **
** ����˵�������ҵ��                          					                                                   **   
** �������أ�long                                                                                           **
************************************************************************************************************/
long Get_Speed(uint8_t L_R)
{
	if(L_R==L) return dif_l;
	else return dif_r;
}

/************************************************************************************************************
** void Motor_RealSpeed(uint8_t L_R)                  				                                             **                                                              
** ����������PID�����ȶ��ٶ�                                                                     		         **
** ����˵�������ҵ�����ٶ�                          					                                               **   
** �������أ�				                                                                                       **
************************************************************************************************************/
uint8_t buffer[4]={0};
void Motor_RealSpeed(int speed,uint8_t L_R)
{
	int PID_FINAL=PID_Loc_V(speed,Get_Speed(L_R)*10,L_R?(&PID_VR):(&PID_VL),2999999);
	if(PID_FINAL>1000)	PID_FINAL=1000;
	if(PID_FINAL<-1000)	PID_FINAL=-1000;
	if(PID_FINAL>0)
	{
		Motor_SetSpeed(Foward, (PID_FINAL), L_R);
	}
	else
	{
		Motor_SetSpeed(Backward, -(PID_FINAL), L_R);
	}

}
/************************************************************************************************************
** void Motor_KeepAngle(int angle,int speed);                				                                       **                                                              
** ����������PID�����ȶ��Ƕ�ǰ��                                                                 		         **
** ����˵������ʼ�Ƕȣ��ٶ�                          					                                               **   
** �������أ�				                                                                                       **
************************************************************************************************************/

void Motor_KeepAngle(int angle,int speed)
{
		int PID_FINAL=PID_Loc_V(angle,Angle_Data.yaw,&PID_A,299999);
		if(PID_FINAL>100)	PID_FINAL=100;
		if(PID_FINAL<-100)	PID_FINAL=-100;
		if(PID_A.Ek<=10)
		{
			Motor_RealSpeed(-PID_FINAL+speed, R);
			Motor_RealSpeed(PID_FINAL+speed, L);
		}
		else
		{
			Motor_RealSpeed(-PID_FINAL, R);
			Motor_RealSpeed(PID_FINAL, L);
		}
		
}

/************************************************************************************************************
** void Motor_Distance(int angle,int distance);                				                      		           **                                                              
** ����������PID�����ȶ��Ƕ�ǰ��һ������                                                                 	   **
** ����˵�������ҵ�����ٶ�                          					                                               **   
** �������أ�				                                                                                       **
************************************************************************************************************/

void Motor_Distance(int angle,int distance)
{
		int PID_FINAL=PID_Loc_V(distance,accu_l+accu_r,&PID_D,30000);
		if(PID_FINAL>50)	PID_FINAL=50;
		if(PID_FINAL<-50)	PID_FINAL=-50;		  
		
		Motor_KeepAngle(angle,PID_FINAL);

}

