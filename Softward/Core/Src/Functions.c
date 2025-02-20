#include "Functions.h"
#include "UI.h"
#include "M24_EEPROM.h"
#include "Delay.h"
#include <string.h>
#include "TIM.h"
#include "ws2812.h"
#include "usart.h"

/************************************************************************************************************************************/
//以下为通用功能的函数

uint8_t Data[4] = {0};

//该函数用于在ui里修改一般参数
void Change_Param(void)
{

	EEPROM_ReadMultipleBytes(cur_item[item_index].type, Data, 4);  //读一次当前数据

	uint8_t Key_set = 0;  //防止第一次按下存储值
	while (1)
	{
		if (HAL_GPIO_ReadPin(Key_Up_GPIO_Port, Key_Up_Pin) == GPIO_PIN_RESET)  //上
		{
			if (Data[3] == cur_item[item_index].up_to)  //上限回0
			{
				Data[3] = 0;
			}
			else
			{	
				Data[3]++;
			}
			
			//功能函数
			Back_ground_change(cur_item[item_index].label);
			Change_Brightness(cur_item[item_index].label);
			
			//刷新屏幕
			LCD_Fill(cur_item[item_index].X_coord,cur_item->Y_coord+(item_index*30), 230, 24+(cur_item->Y_coord+(item_index*30)), GRAYBLUE);
			LCD_ShowString(cur_item[item_index].X_coord,cur_item->Y_coord+(item_index*30),(u8* )cur_item[item_index].label, cur_item[item_index].Color, GRAYBLUE, 24, 0);
			LCD_ShowIntNum(195,cur_item->Y_coord+(item_index*30), Data[3], 3, cur_item[item_index].Color,GRAYBLUE, 24);
		}
		if (HAL_GPIO_ReadPin(Key_Down_GPIO_Port, Key_Down_Pin) == GPIO_PIN_RESET)  //下
		{
			if (Data[3] == 0)  //上限回高
			{
				Data[3] = cur_item[item_index].up_to;
			}
			else
			{	
				Data[3]--;
			}
			
			//功能函数
			Back_ground_change(cur_item[item_index].label);
			Change_Brightness(cur_item[item_index].label);
			
			//刷新屏幕
			LCD_Fill(cur_item[item_index].X_coord,cur_item->Y_coord+(item_index*30), 230, 24+(cur_item->Y_coord+(item_index*30)), GRAYBLUE);
			LCD_ShowString(cur_item[item_index].X_coord,cur_item->Y_coord+(item_index*30),(u8* )cur_item[item_index].label, cur_item[item_index].Color, GRAYBLUE, 24, 0);
			LCD_ShowIntNum(195,cur_item->Y_coord+(item_index*30), Data[3], 3, cur_item[item_index].Color,GRAYBLUE, 24);
		}
		if (HAL_GPIO_ReadPin(Key_No_GPIO_Port, Key_No_Pin) == GPIO_PIN_RESET)  //退出
		{
			
			//功能函数
			Back_ground_change_back(cur_item[item_index].label);
			Brightness_change_back(cur_item[item_index].label);
			
			LCD_Fill(cur_item[item_index].X_coord,cur_item->Y_coord+(item_index*30), 230, 24+(cur_item->Y_coord+(item_index*30)), Back_ground_color);
			EEPROM_ReadMultipleBytes(cur_item[item_index].type, Data, 4);
			LCD_ShowIntNum(195,cur_item->Y_coord+(item_index*30), Data[3], 3, cur_item[item_index].Color,Back_ground_color, 24);
			LCD_ShowString(cur_item[item_index].X_coord,cur_item->Y_coord+(item_index*30),(u8* )cur_item[item_index].label, cur_item[item_index].Color, Back_ground_color, 24, 0);
			
			//重绘选项
			Key_val = 0;  //防止刷屏
			DispCrtMenu();
			break;
		}
		if (HAL_GPIO_ReadPin(Key_Yes_GPIO_Port, Key_Yes_Pin) == GPIO_PIN_SET)
		{
			Key_set = 1;
		}
		if (HAL_GPIO_ReadPin(Key_Yes_GPIO_Port, Key_Yes_Pin) == GPIO_PIN_RESET && Key_set == 1)  //确认
		{
			if (EEPROM_WriteMultipleBytes(cur_item[item_index].type, Data, 4) != HAL_ERROR)  //eeprom写入
			{
				LCD_Fill(70, 95, 170, 140, DARKGREEN);
				LCD_ShowString(78, 103, (uint8_t*)"Success", WHITE, DARKGREEN, 24, 0);
				
				Delay_ms(1500);  //延时一会儿
						
				//刷新数据
				LCD_Fill(70, 95, 170, 140, Back_ground_color);
				LCD_Fill(cur_item[item_index].X_coord,cur_item->Y_coord+(item_index*30), 230, 24+(cur_item->Y_coord+(item_index*30)), Back_ground_color);
				EEPROM_ReadMultipleBytes(cur_item[item_index].type, Data, 4);
				LCD_ShowIntNum(195,cur_item->Y_coord+(item_index*30), Data[3], 3, cur_item[item_index].Color,Back_ground_color, 24);
				LCD_ShowString(cur_item[item_index].X_coord,cur_item->Y_coord+(item_index*30),(u8* )cur_item[item_index].label, cur_item[item_index].Color, Back_ground_color, 24, 0);
				
				//功能函数
				Back_ground_change(cur_item[item_index].label);
				Change_Brightness(cur_item[item_index].label);
				RGB_refresh(cur_item[item_index].label);
				
				//重绘选项
				Key_val = 0;  //防止刷屏
				DispCrtMenu();
				
				Key_set = 0;  //防止反复进入
			}
			break;  //退出
		}
		
		Delay_ms(100);
	}
}

/************************************************************************************************************************************/
//以下为特殊功能的函数

void EEPROM_OUT(void)
{
	uint8_t first_time_in = 0;
	
	if (first_time_in == 0 && HAL_GPIO_ReadPin(Key_Yes_GPIO_Port, Key_Yes_Pin) == GPIO_PIN_RESET)
	{
		while (HAL_GPIO_ReadPin(Key_Yes_GPIO_Port, Key_Yes_Pin) == GPIO_PIN_RESET)
		{
			;
		}
		
		//弹起效果
		LCD_Fill(cur_item[item_index].X_coord,cur_item->Y_coord+(item_index*30), 230, 24+(cur_item->Y_coord+(item_index*30)), Back_ground_color);
		LCD_ShowString(cur_item[item_index].X_coord,cur_item->Y_coord+(item_index*30),(u8* )cur_item[item_index].label, cur_item[item_index].Color, Back_ground_color, 24, 0);
	}
	if(HAL_UART_Transmit(&huart1, (uint8_t*)"baud_rate_115200,from_0x00_to_0xff,send'Y'to_verify", 51, 0x1000) != HAL_ERROR)
	{
		LCD_Fill(70, 95, 170, 140, GRAYBLUE);
		LCD_ShowString(78, 103, (uint8_t*)"Waiting", WHITE, GRAYBLUE, 24, 0);
	}
	else
	{
		LCD_Fill(70, 95, 170, 140, RED);
		LCD_ShowString(78, 103, (uint8_t*)"Failed", WHITE, RED, 24, 0);
		Delay_ms(1500);
		LCD_Fill(70, 95, 170, 140, Back_ground_color);
		Key_val = 0;
		DispCrtMenu();
		return;
	}
	
	uint8_t Receive_data[1] = {0};
	if(HAL_UART_Receive(&huart1, Receive_data, 1, 100) == HAL_TIMEOUT)
	{
//		LCD_Fill(70, 95, 170, 140, RED);
//		LCD_ShowString(78, 103, (uint8_t*)"TimeOut", WHITE, RED, 24, 0);
//		Delay_ms(1500);
//		LCD_Fill(70, 95, 170, 140, Back_ground_color);
//		Key_val = 0;
//		DispCrtMenu();
//		return;
//    在循环里，超时无用，懒得写了
	}
	else if(*Receive_data == 'Y')
	{
		LCD_Fill(70, 95, 170, 140, GRAYBLUE);
		LCD_ShowString(78, 103, (uint8_t*)"Sending", WHITE, GRAYBLUE, 24, 0);
		
		uint8_t Data_Out[256];
		EEPROM_ReadMultipleBytes(0x00, Data_Out, 256);
		if (HAL_UART_Transmit(&huart1, Data_Out, 256, 0x1000) == HAL_OK)
		{
			LCD_Fill(70, 95, 170, 140, DARKGREEN);
			LCD_ShowString(78, 103, (uint8_t*)"Success", WHITE, DARKGREEN, 24, 0);
			Delay_ms(1500);
			LCD_Fill(70, 95, 170, 140, Back_ground_color);
			Key_val = 0;
			DispCrtMenu();
			return;
		}
		else
		{
			LCD_Fill(70, 95, 170, 140, RED);
			LCD_ShowString(78, 103, (uint8_t*)"Failed", WHITE, RED, 24, 0);
			Delay_ms(1500);
			LCD_Fill(70, 95, 170, 140, Back_ground_color);
			Key_val = 0;
			DispCrtMenu();
			return;
		}
	}
	else
	{
		LCD_Fill(70, 95, 170, 140, RED);
		LCD_ShowString(78, 103, (uint8_t*)"Failed", WHITE, RED, 24, 0);
		Delay_ms(1500);
		LCD_Fill(70, 95, 170, 140, Back_ground_color);
		Key_val = 0;
		DispCrtMenu();
		return;
	}
}
	

/************************************************************************************************************************************/
//以下为不同功能的执行函数

//针对切换背景的执行函数
void Back_ground_change(char* name)
{
	if (!strcmp("LCD_B_color", name))
	{
		Back_ground_color = Back_ground_color_array[Data[3]];
		DispCrtMenu(); //刷屏
	}
}

//针对切换背景退出的恢复函数
void Back_ground_change_back(char* name)
{
	if (!strcmp("LCD_B_color", name))
	{
		EEPROM_ReadMultipleBytes(cur_item[item_index].type, Data, 4);  //读一下
		Back_ground_change(name);
	}
}

//针对更改亮度的执行函数
void Change_Brightness(char* name)
{
	if (!strcmp("LCD_Brightness", name))
	{
		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, Data[3]);
	}
}

//针对更改亮度退出的恢复函数
void Brightness_change_back(char* name)
{
	if (!strcmp("LCD_Brightness", name))
	{
		EEPROM_ReadMultipleBytes(cur_item[item_index].type, Data, 4);  //读一下
		Change_Brightness(name);
	}
}

//针对更改RGB退出的恢复函数
void RGB_refresh(char* name)
{
	if (!strcmp("RGB_Light", name))
	{
		EEPROM_ReadMultipleBytes(cur_item[item_index].type, Data, 4);  //读一下
		if (Data[3] == 1)
		{
			HAL_TIM_Base_Start_IT(&htim2);
			WS2812_Set_Color(106, 90, 205);			
		}
		else
		{
			WS2812_Off();
		}
	}
}
