#include "tb6612.h"
#include "tim.h"


/************************************************************************************************************
** int Motor_SetSpeed(uint8_t Mode,float speed, uint8_t L_R)                  				                     **                                                              
** 功能描述：设置一侧电机速度                                                                    		         **
** 参数说明：Foward or Backward， 速度， 左右电机                          					                         **   
** 参数返回：int                                                                                            **
************************************************************************************************************/

//speed 的范围是0-1000


int Motor_SetSpeed(int Mode,int speed, uint8_t L_R)
{
	
	int pulse = 0;  
	if(0 <= speed && speed <= 1000)
	{
		pulse = 1000 - speed;
		
		if (L_R == 1)  //右边转
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
		if (L_R == 0)  //左边转
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
** 功能描述：启动一侧电机                                                                       		         **
** 参数说明：左右电机                          					                                                   **   
** 参数返回：uint8_t                                                                                        **
************************************************************************************************************/

int Motor_Start(uint8_t L_R)
{
	if (L_R == 0)  //左边
	{
		HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
		return 0;
	}
	if (L_R == 1)  //右边
	{
		HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);
		return 0;
	}
	if (L_R == 2)  //同时
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
** 功能描述：停止一侧电机                                                                        		         **
** 参数说明：左右电机                          					                                                   **   
** 参数返回：uint8_t                                                                                        **
************************************************************************************************************/

int Motor_Off(uint8_t L_R)
{
	if (L_R == 0)  //左边
	{
		HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
		return 0;
	}
	if (L_R == 1)  //右边
	{
		HAL_TIM_PWM_Stop(&htim5,TIM_CHANNEL_1);
		return 0;
	}
	if (L_R == 2)  //同时
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
