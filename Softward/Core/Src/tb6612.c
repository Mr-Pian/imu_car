#include "tb6612.h"
#include "tim.h"
#include "ws2812.h"

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
** void Motor_RealSpeed(uint8_t L_R)                  				                                             **                                                              
** ����������ֹͣһ����                                                                        		         **
** ����˵�������ҵ��                          					                                                   **   
** �������أ�uint8_t                                                                                        **
************************************************************************************************************/
long Get_Speed(uint8_t L_R)
{
	if(L_R==L) return dif_l;
	if(L_R==R) return dif_r;
}

/************************************************************************************************************
** void Motor_RealSpeed(uint8_t L_R)                  				                                             **                                                              
** ����������PID�����ȶ��ٶ�                                                                     		         **
** ����˵�������ҵ�����ٶ�                          					                                               **   
** �������أ�				                                                                                       **
************************************************************************************************************/

void Motor_RealSpeed(int speed,uint8_t L_R)
{
	
	if(PID_Loc_V(speed,Get_Speed(L_R),PID_V,0)>0)
	{
		Motor_SetSpeed(Foward, (int)(PID_Loc_V(speed,Get_Speed(L_R),PID_V,0)), L_R);
	}
	else
	{
		Motor_SetSpeed(Backward, (int)(-PID_Loc_V(speed,Get_Speed(L_R),PID_V,0)), L_R);
	}
}

