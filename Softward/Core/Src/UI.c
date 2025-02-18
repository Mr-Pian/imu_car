#include "UI.h"


//����˵�������Ҫ��ȫ�ֱ���
Menu *cur_item = menu1_main;  //��ʼ����ǰ�˵�Ϊ��һ��(menu1_main)
Menu *prev_item = NULL;	    //��ʼ����һ���˵�Ϊ��
uint8_t item_index = 0;//��ǰ�˵�������
uint8_t Key_val=0;

//�ṹ���ʼ��//�˵�����,�����ｫÿһ���˵��Ĺ������ú�
Menu menu1_main[4] = // ��1�� ���˵� 
{
	{4, "����ѡ��", "����", TYPE_SUBMENU, NULL, menu2_run, NULL}, 
	{4, "", "����", TYPE_SUBMENU, NULL, menu2_debug, NULL}, 
	{4, "", "����", TYPE_SUBMENU, NULL, menu2_set, NULL}, 
	{4, "", "�汾��Ϣ", TYPE_SUBMENU, NULL, NULL, NULL},  
}; 

Menu menu2_run[4] =  // ��2�� ���в˵� 
{
	{4, "���в˵�", "�Զ�����", TYPE_SUBMENU, NULL, NULL, menu1_main}, 
	{4, "",     "�ֶ�����", TYPE_SUBMENU, NULL, menu3_manual_run, menu1_main}, 
	{4, "",     "��������", TYPE_SUBMENU, NULL, NULL, menu1_main},
	{4, "",     "���", TYPE_SUBMENU, NULL, NULL, menu1_main},
};

Menu menu2_debug[6] =  // ��2�� ���Բ˵� 
{
	{6, "���Բ˵�", "Speed_P", TYPE_PARAM, NULL, NULL, menu1_main}, 
	{6, "", "Speed_I", TYPE_PARAM, NULL, NULL, menu1_main}, 
	{6, "", "Speed_D", TYPE_PARAM, NULL, NULL, menu1_main},
	{6, "", "Angle_P", TYPE_PARAM, NULL, NULL, menu1_main},
	{6, "", "Angle_I", TYPE_PARAM, NULL, NULL, menu1_main},
	{6, "", "Angle_D", TYPE_PARAM, NULL, NULL, menu1_main},
};

Menu menu2_set[2] =  // ��2�� ���ò˵� 
{
	{2, "���ò˵�", "С������", TYPE_SUBMENU, NULL, menu3_car_set, menu1_main}, 
	{2, "",     "��Ļ����", TYPE_SUBMENU, NULL, menu3_LCD_set, menu1_main}, 
};

Menu menu3_manual_run[6] =  // ��3�� �ֶ����� 
{
	{6, "�ֶ�����", "Motor1_Speed", TYPE_PARAM, NULL, NULL, menu2_run}, 
	{6, "",     "Motor2_Speed", TYPE_PARAM, NULL, NULL, menu2_run}, 
	{6, "",     "Motor1_Status", TYPE_PARAM, NULL, NULL, menu2_run},
	{6, "",     "Motor2_Status", TYPE_PARAM, NULL, NULL, menu2_run},
	{6, "",     "Run_time", TYPE_PARAM, NULL, NULL, menu2_run},
	{6, "",     "Run", TYPE_PARAM, NULL, NULL, menu2_run},
};

Menu menu3_car_set[7] =  //��3�� С������
{
	{7, "С������", "RGB_Light", TYPE_PARAM, NULL, NULL, menu2_set},
	{7, "", "Run_mode", TYPE_PARAM, NULL, NULL, menu2_set},
	{7, "", "Max_Speed", TYPE_PARAM, NULL, NULL, menu2_set},
	{7, "", "Swerve_Speed", TYPE_PARAM, NULL, NULL, menu2_set},
	{7, "", "EEPROM_Output", TYPE_PARAM, NULL, NULL, menu2_set},
	{7, "", "EEPROM_Lock", TYPE_PARAM, NULL, NULL, menu2_set},
	{7, "", "EEPROM_Erase", TYPE_PARAM, NULL, NULL, menu2_set},
};

Menu menu3_LCD_set[4] =  //��3�� LCD����
{
	{4, "��Ļ����", "LCD_Brightness", TYPE_PARAM, NULL, NULL, menu2_set},
	{4, "", "LCD_Background_color", TYPE_PARAM, NULL, NULL, menu2_set},
	{4, "", "Sleep", TYPE_PARAM, NULL, NULL, menu2_set},
	{4, "", "Sleep_time", TYPE_PARAM, NULL, NULL, menu2_set},
};

void DispCrtMenu(void)//���Ƶ�ǰ�˵���
{
	uint8_t menu_num = cur_item[0].num;//��ȡ��ǰ�˵�����Ŀ���� 
	uint8_t i, num = menu_num>MENU_MAX_ROW ? MENU_MAX_ROW : menu_num;//�����������ܳ���ÿһ��������������
	if(item_index>=MENU_MAX_ROW || item_index>=menu_num)//�˵�������ѡ���Ƿ�Խ�� 
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
	
	for (i=0; i<num; i++)//����ĳһ���˵��µĹ��ܼ� 
	{
		uint16_t Pointer_Color = LIGHTBLUE;
		LCD_ShowChinese(100,(i+1)*40,(u8 *)cur_item[i].label, WHITE, i==item_index ? LIGHTBLUE:BLACK, 24, 0);
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
							item_index = 0;//�����Ӳ˵������� 
							DispCrtMenu();
						}
						else
						{
							LCD_DrawRectangle(105, 90, 135, 150, DARKBLUE);
							LCD_ShowChar(110, 110, (uint8_t)"Undefined", WHITE, DARKBLUE, 24, 0);  //������ʾ
						}
						break; 
					case TYPE_PARAM:  //���в������õĲ˵���
						if(cur_item[item_index].Function != NULL)
						{ 
							//������Ӧ�Ķ�������,�����ݲ���
							cur_item[item_index].Function((const char *)cur_item[item_index].label);
						}
						else
						{
							LCD_DrawRectangle(105, 90, 135, 150, DARKBLUE);
							LCD_ShowChar(110, 110, (uint8_t)"Undefined", WHITE, DARKBLUE, 24, 0);  //������ʾ
						}
						break; 
					default: 
						break;
				}
				break; 
			case KEY_RETURN_PRESS: 
				if (prev_item != NULL)//������һ���˵��Ĳ��� 
				{
					cur_item = prev_item;  //������һ���˵�Ϊ��ǰ�˵� 
					prev_item = cur_item[0].prev;  //���õ�ǰ�˵�����һ���˵� 
					item_index = 0;  //�����Ӳ˵�������
					DispCrtMenu();
				} 
				else
				{
					LCD_DrawRectangle(105, 90, 135, 150, DARKBLUE);
					LCD_ShowChar(110, 110, (uint8_t)"Can't Back", WHITE, DARKBLUE, 24, 0);  //������ʾ
				}
				break; 
		    default: 
		        break;
		}
	}
	Key_val = 0x00;
} 

void DrawTitle(uint8_t* Title_Name)
{
	LCD_DrawRectangle(0, 0, 240, 60, GRAY);
	LCD_ShowChar(110, 110, *Title_Name, WHITE, GRAY, 32, 0);  //������ʾ
}

