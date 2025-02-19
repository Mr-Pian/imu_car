#include "Functions.h"
#include "UI.h"
#include "M24_EEPROM.h"
#include "Delay.h"

//�ú���������ui���޸�һ�����
void Change_Param(void)
{
	uint8_t Data[4];
	EEPROM_ReadMultipleBytes(cur_item[item_index].type, Data, 4);  //��һ�ε�ǰ����

	uint8_t Key_set = 0;  //��ֹ��һ�ΰ��´洢ֵ
	while (1)
	{
		if (HAL_GPIO_ReadPin(Key_Up_GPIO_Port, Key_Up_Pin) == GPIO_PIN_RESET)
		{
			if (Data[3] == cur_item[item_index].up_to)  //���޻�0
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
			if (Data[3] == 0)  //���޻ظ�
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
			
			//�ػ�ѡ��
			Key_val = 0;  //��ֹˢ��
			DispCrtMenu();
			break;
		}
		if (HAL_GPIO_ReadPin(Key_Yes_GPIO_Port, Key_Yes_Pin) == GPIO_PIN_SET)
		{
			Key_set = 1;
		}
		if (HAL_GPIO_ReadPin(Key_Yes_GPIO_Port, Key_Yes_Pin) == GPIO_PIN_RESET && Key_set == 1)
		{
			if (EEPROM_WriteMultipleBytes(cur_item[item_index].type, Data, 4) != HAL_ERROR)  //eepromд��
			{
				LCD_Fill(70, 95, 170, 140, GRAYBLUE);
				LCD_ShowString(78, 103, (uint8_t*)"success", WHITE, GRAYBLUE, 24, 0);
				
				Delay_ms(1500);  //��ʱһ���
						
				//ˢ������
				LCD_Fill(cur_item[item_index].X_coord,cur_item->Y_coord+(item_index*30), 230, 24+(cur_item->Y_coord+(item_index*30)), Back_ground_color);
				EEPROM_ReadMultipleBytes(cur_item[item_index].type, Data, 4);
				LCD_ShowIntNum(195,cur_item->Y_coord+(item_index*30), Data[3], 3, cur_item[item_index].Color,Back_ground_color, 24);
				LCD_ShowString(cur_item[item_index].X_coord,cur_item->Y_coord+(item_index*30),(u8* )cur_item[item_index].label, cur_item[item_index].Color, Back_ground_color, 24, 0);
				
				//�ػ�ѡ��
				LCD_Fill(70, 95, 170, 140, Back_ground_color);
				Key_val = 0;  //��ֹˢ��
				DispCrtMenu();
				
				Key_set = 0;  //��ֹ��������
			}
			break;  //�˳�
		}
		
		Delay_ms(100);
	}
}
