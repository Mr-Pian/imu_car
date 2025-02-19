#include "Functions.h"
#include "UI.h"
#include "M24_EEPROM.h"
#include "Delay.h"

//该函数用于在ui里修改一般参数
void Change_Param(void)
{
	uint8_t Data[4];
	EEPROM_ReadMultipleBytes(cur_item[item_index].type, Data, 4);  //读一次当前数据

	uint8_t Key_set = 0;  //防止第一次按下存储值
	while (1)
	{
		if (HAL_GPIO_ReadPin(Key_Up_GPIO_Port, Key_Up_Pin) == GPIO_PIN_RESET)
		{
			if (Data[3] == cur_item[item_index].up_to)  //上限回0
			{
				Data[3] = 0;
			}
			else
			{	
				Data[3]++;
			}
			LCD_ShowIntNum(195,cur_item->Y_coord+(item_index*30), Data[3], 3, cur_item[item_index].Color,GRAYBLUE, 24);
		}
		if (HAL_GPIO_ReadPin(Key_Down_GPIO_Port, Key_Down_Pin) == GPIO_PIN_RESET)
		{
			if (Data[3] == 0)  //上限回高
			{
				Data[3] = cur_item[item_index].up_to;
			}
			else
			{	
				Data[3]--;
			}
			LCD_ShowIntNum(195,cur_item->Y_coord+(item_index*30), Data[3], 3, cur_item[item_index].Color,GRAYBLUE, 24);
		}
		if (HAL_GPIO_ReadPin(Key_No_GPIO_Port, Key_No_Pin) == GPIO_PIN_RESET)
		{
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
		if (HAL_GPIO_ReadPin(Key_Yes_GPIO_Port, Key_Yes_Pin) == GPIO_PIN_RESET && Key_set == 1)
		{
			if (EEPROM_WriteMultipleBytes(cur_item[item_index].type, Data, 4) != HAL_ERROR)  //eeprom写入
			{
				LCD_Fill(70, 95, 170, 140, GRAYBLUE);
				LCD_ShowString(78, 103, (uint8_t*)"success", WHITE, GRAYBLUE, 24, 0);
				
				Delay_ms(1500);  //延时一会儿
						
				//刷新数据
				LCD_Fill(cur_item[item_index].X_coord,cur_item->Y_coord+(item_index*30), 230, 24+(cur_item->Y_coord+(item_index*30)), Back_ground_color);
				EEPROM_ReadMultipleBytes(cur_item[item_index].type, Data, 4);
				LCD_ShowIntNum(195,cur_item->Y_coord+(item_index*30), Data[3], 3, cur_item[item_index].Color,Back_ground_color, 24);
				LCD_ShowString(cur_item[item_index].X_coord,cur_item->Y_coord+(item_index*30),(u8* )cur_item[item_index].label, cur_item[item_index].Color, Back_ground_color, 24, 0);
				
				//重绘选项
				LCD_Fill(70, 95, 170, 140, Back_ground_color);
				Key_val = 0;  //防止刷屏
				DispCrtMenu();
				
				Key_set = 0;  //防止反复进入
			}
			break;  //退出
		}
		
		Delay_ms(100);
	}
}
