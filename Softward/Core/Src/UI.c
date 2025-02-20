#include "UI.h"
#include "Functions.h"
#include "M24_EEPROM.h"


//����˵�������Ҫ��ȫ�ֱ���
Menu *cur_item = menu1_main;  //��ʼ����ǰ�˵�Ϊ��һ��(menu1_main)
Menu *prev_item = NULL;	    //��ʼ����һ���˵�Ϊ��
uint8_t Key_val=0;  //��ֵ��ʼ��
uint8_t item_index = 0;  //�˵�����ʼ��
uint8_t pre_item_index = 0;  //��ǰ�Ĳ˵����
uint8_t flag = 0;  //���ΰ�����ʶ
uint16_t Back_ground_color = 0x00;
uint16_t Back_ground_color_array[11] = {LGRAY, GRAY, WHITE, BLACK, LGRAYBLUE, YELLOW, GBLUE, MAGENTA, DARKBLUE, DARKGREEN, CYAN};

//�ṹ���ʼ��//�˵�����,�����ｫÿһ���˵��Ĺ������ú�
Menu menu1_main[4] = // ��1�� ���˵� 
{
	{4, "����ѡ��", "����", TYPE_SUBMENU,0, NULL, menu2_run, NULL, Chinese, RED, 90, 90}, //ע�⣬��һ����y������Ĭ��
	{4, "", "����", TYPE_SUBMENU, NULL,0, menu2_debug, NULL, Chinese, ORANGE, 90, 0}, 
	{4, "", "����", TYPE_SUBMENU, NULL,0, menu2_set, NULL, Chinese, YELLOW, 90, 0}, 
	{4, "", "�汾��Ϣ", TYPE_SUBMENU, NULL,0, NULL, NULL, Chinese, DARKGREEN, 70, 0},  
}; 

Menu menu2_run[4] =  // ��2�� ���в˵� 
{
	{4, "���в˵�", "�Զ�����", TYPE_SUBMENU,0, NULL, NULL, menu1_main, Chinese, RED, 70, 90}, 
	{4, "",     "�ֶ�����", TYPE_SUBMENU,0, NULL, menu3_manual_run, menu1_main, Chinese, ORANGE, 70, 0}, 
	{4, "",     "��������", TYPE_SUBMENU,0, NULL, NULL, menu1_main, Chinese, YELLOW, 70, 0},
	{4, "",     "���", TYPE_SUBMENU,0, NULL, NULL, menu1_main, Chinese, DARKGREEN, 90, 0},
};

Menu menu2_debug[6] =  // ��2�� ���Բ˵� 
{
	{6, "���Բ˵�", "Speed_P", SPEED_P,255,(*Change_Param), NULL, menu1_main, English, RED, 10, 55}, 
	{6, "", "Speed_I", SPEED_I,255, (*Change_Param), NULL, menu1_main, English, ORANGE, 10, 0}, 
	{6, "", "Speed_D", SPEED_D,255,(*Change_Param), NULL, menu1_main, English, YELLOW, 10, 0},
	{6, "", "Angle_P", ANGLE_P,255, (*Change_Param), NULL, menu1_main, English, DARKGREEN, 10, 0},
	{6, "", "Angle_I", ANGLE_I,255, (*Change_Param), NULL, menu1_main, English, DODGERBLUE, 10, 0},
	{6, "", "Angle_D", ANGLE_D,255, (*Change_Param), NULL, menu1_main, English, DARKBLUE, 10, 0},
};

Menu menu2_set[2] =  // ��2�� ���ò˵� 
{
	{2, "���ò˵�", "С������", TYPE_SUBMENU,0, NULL, menu3_car_set, menu1_main, Chinese, RED, 70, 90}, 
	{2, "",     "��Ļ����", TYPE_SUBMENU,0, NULL, menu3_LCD_set, menu1_main, Chinese, ORANGE, 70, 0}, 
};

Menu menu3_manual_run[6] =  // ��3�� �ֶ����� 
{
	{6, "�ֶ�����", "Motor1_Speed", MOTOR1_SPEED,100, (*Change_Param), NULL, menu2_run, English, RED, 10, 55}, 
	{6, "",     "Motor2_Speed", MOTOR2_SPEED,100, (*Change_Param), NULL, menu2_run, English, ORANGE, 10, 0}, 
	{6, "",     "Motor1_Status", MOTOR1_STATUS,1, (*Change_Param), NULL, menu2_run, English, YELLOW, 10, 0},
	{6, "",     "Motor2_Status", MOTOR2_STATUS,1, (*Change_Param), NULL, menu2_run, English, DARKGREEN, 10, 0},
	{6, "",     "Run_time", RUN_TIME,60, (*Change_Param), NULL, menu2_run, English, DODGERBLUE, 10, 0},
	{6, "",     "<<< Run >>>", TYPE_SPECIAL_PARAM,0, NULL, NULL, menu2_run, English, DARKBLUE, 55, 0},
};

Menu menu3_car_set[6] =  //��3�� С������
{
	{6, "С������", "RGB_Light", RGB_LIGHT,1, (*Change_Param), NULL, menu2_set, English, RED, 10, 55},
	{6, "", "Max_Speed", MAX_SPEED,100, (*Change_Param), NULL, menu2_set, English, ORANGE, 10, 0},
	{6, "", "Swerve_Speed", SWERVE_SPEED,100, (*Change_Param), NULL, menu2_set, English, YELLOW, 10, 0},
	{6, "", "EEPROM_Out", TYPE_SPECIAL_PARAM,0, (*EEPROM_OUT), NULL, menu2_set, English, DARKGREEN, 10, 0},
	{6, "", "EEPROM_Lock", TYPE_SPECIAL_PARAM,0, NULL, NULL, menu2_set, English, DODGERBLUE, 10, 0},
	{6, "", "EEPROM_Erase", TYPE_SPECIAL_PARAM,0, NULL, NULL, menu2_set,English, DARKBLUE, 10, 0},
};

Menu menu3_LCD_set[4] =  //��3�� LCD����
{
	{4, "��Ļ����", "LCD_Brightness", LCD_BRIGHTLESS,99, (*Change_Param), NULL, menu2_set, English, RED, 10, 55},
	{4, "", "LCD_B_color", LCD_B_COLOR,10, (*Change_Param), NULL, menu2_set, English, ORANGE, 10, 0},
	{4, "", "Sleep", SLEEP,1, (*Change_Param), NULL, menu2_set, English, YELLOW, 10, 0},
	{4, "", "Sleep_time", SLEEP_TIME,255, (*Change_Param), NULL, menu2_set, English, DARKGREEN, 10, 0},
};

void DispCrtMenu(void)  //���Ƶ�ǰ�˵���
{
	if(Key_val==KEY_ENTER_PRESS||Key_val==KEY_RETURN_PRESS)  //���˳�ʱˢ����Ļ
	{
		LCD_Fill(0,0,240,240,Back_ground_color);
		LCD_Fill(0, 0, 240, 50, GRAYBLUE);  //��ӡ�����
	}
	uint8_t menu_num = cur_item[0].num;  //��ȡ��ǰ�˵�����Ŀ���� 
	uint8_t i=0, num = menu_num>MENU_MAX_ROW ? MENU_MAX_ROW : menu_num;  //�����������ܳ���ÿһ��������������
	if(item_index>=MENU_MAX_ROW || item_index>=menu_num)  //�˵�������ѡ���Ƿ�Խ�� 
	{ 
		if(item_index==0XFF)  //�����Ϊ item_index=0 �ٰ��ϼ���0-1=0XFF
		{
			item_index = menu_num - 1;   //ѭ�����ص����һ����������ֵ
		}
		if(item_index>=menu_num)  //�����Ϊ����������һ����������ֵ
		{
			item_index = 0;  //ѭ�����ص���һ����������ֵ
		}
		if(item_index>=MENU_MAX_ROW) 
		{
			item_index = 0;
		}
	} 
	
	DrawTitle((uint8_t*)cur_item[0].title);  //���Ʋ˵�������
	
	if (cur_item[i].ce == Chinese)  //�ж���Ŀ�����Ļ���Ӣ��
	{
		for (i=0; i<num; i++)
		{
			if (cur_item[i].Y_coord == 0)
			{
				LCD_ShowChinese(cur_item[i].X_coord,cur_item->Y_coord+(i*30),(u8 *)cur_item[i].label, cur_item[i].Color, i==item_index ? GRAYBLUE:Back_ground_color, 24, 0);  //��������
			}
			else  //����Ĭ�ϵĻ����趨����
			{
				LCD_ShowChinese(cur_item[i].X_coord,cur_item[i].Y_coord,(u8 *)cur_item[i].label, cur_item[i].Color, i==item_index ? GRAYBLUE:Back_ground_color, 24, 0);
			}
		}
	}
	else if (cur_item[i].ce == English)
	{
		for (i=0; i<num; i++)//����ĳһ���˵��µĹ��ܼ� 
		{
			if (cur_item[i].Y_coord == 0)
			{	
				LCD_ShowString(cur_item[i].X_coord,cur_item->Y_coord+(i*30),(u8* )cur_item[i].label, cur_item[i].Color, i==item_index ? GRAYBLUE:Back_ground_color, 24, 0);  //����Ӣ��
			}
			else  //����Ĭ�ϵĻ����趨����
			{
				LCD_ShowString(cur_item[i].X_coord,cur_item[i].Y_coord,(u8 *)cur_item[i].label, cur_item[i].Color, i==item_index ? GRAYBLUE:Back_ground_color, 24, 0);
			}
			
			/*һ�²���Ϊ��ӡ��������*/
			if (cur_item[i].type != TYPE_SUBMENU && cur_item[i].type != TYPE_SPECIAL_PARAM)
			{
				uint8_t Data[4];
				EEPROM_ReadMultipleBytes(cur_item[i].type, Data, 4);
				LCD_ShowIntNum(195,cur_item->Y_coord+(i*30), Data[3], 3, cur_item[i].Color,Back_ground_color, 24);
			}
		}
	}
}



//��ʾ����
void Display(void) 
{
	if(Key_val==KEY_UP_PRESS || Key_val==KEY_DOWN_PRESS || Key_val==KEY_ENTER_PRESS || Key_val==KEY_RETURN_PRESS)
	{
		switch(Key_val)//��ⰴ����������Ӧ����
		{
			case KEY_UP_PRESS: 
				item_index--; 
				DispCrtMenu();
				break; 
			case KEY_DOWN_PRESS: 
				item_index++; 
				DispCrtMenu();
				break; 
			case KEY_ENTER_PRESS:
				switch(cur_item[item_index].type)//��⹦��������ͣ�������Ӧ����
				{
					case TYPE_SUBMENU: //�����Ӳ˵��Ĳ˵���
						if(cur_item[item_index].next != NULL)
						{ 
							prev_item = cur_item;//�˼��˵��������һ���˵� 
							cur_item = cur_item[item_index].next;//��ָ�����һ���˵�����Ϊ��ǰ�˵� 
							pre_item_index = item_index;  
							item_index = 0;//�����Ӳ˵������� 
							DispCrtMenu();
						}
						else
						{
							break;  //
						}
						break; 
					default: 
						if(cur_item[item_index].Function != NULL && cur_item[item_index].type != TYPE_SPECIAL_PARAM)
						{ 
							//����ѡ����
							uint8_t Data[4];
							LCD_Fill(cur_item[item_index].X_coord,cur_item->Y_coord+(item_index*30), 230, 24+(cur_item->Y_coord+(item_index*30)), GRAYBLUE);
							EEPROM_ReadMultipleBytes(cur_item[item_index].type, Data, 4);
							LCD_ShowIntNum(195,cur_item->Y_coord+(item_index*30), Data[3], 3, cur_item[item_index].Color,GRAYBLUE, 24);
							LCD_ShowString(cur_item[item_index].X_coord,cur_item->Y_coord+(item_index*30),(u8* )cur_item[item_index].label, cur_item[item_index].Color, GRAYBLUE, 24, 0);
							
							//���ΰ���
							flag = 1;
							//������Ӧ�Ķ�������
							cur_item[item_index].Function();
							//�ָ���������
							flag = 0;
							
						}
						else
						{
							LCD_ShowString(cur_item[item_index].X_coord,cur_item->Y_coord+(item_index*30),(u8* )cur_item[item_index].label, cur_item[item_index].Color, GRAYBLUE, 24, 0);
							LCD_Fill(cur_item[item_index].X_coord,cur_item->Y_coord+(item_index*30), 230, 24+(cur_item->Y_coord+(item_index*30)), GRAYBLUE);
							//���ΰ���
							flag = 1;
							//������Ӧ�Ķ�������
							cur_item[item_index].Function();
							//�ָ���������
							flag = 0;
							
						}
						break;
				}
				break; 
			case KEY_RETURN_PRESS: 
				if (prev_item != NULL)//������һ���˵��Ĳ��� 
				{
					cur_item = prev_item;  //������һ���˵�Ϊ��ǰ�˵� 
					prev_item = cur_item[0].prev;  //���õ�ǰ�˵�����һ���˵� 
					item_index = pre_item_index;  //�����Ӳ˵�������
					DispCrtMenu();
				} 
				else
				{
					break;  //
				}
				break; 
		    default: 
		        break;
		}
	}
	
	Key_val = 0x00;  //����Key_val

} 

void DrawTitle(uint8_t* Title_Name)
{
	LCD_ShowChinese(50, 10, Title_Name, Title_color, GRAYBLUE, 32, 0);  //��ӡ����
}
