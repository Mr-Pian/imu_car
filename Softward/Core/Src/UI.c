#include "UI.h"
#include "Functions.h"
#include "M24_EEPROM.h"


//定义菜单操作需要的全局变量
Menu *cur_item = menu1_main;  //初始化当前菜单为第一级(menu1_main)
Menu *prev_item = NULL;	    //初始化上一级菜单为空
uint8_t Key_val=0;  //键值初始化
uint8_t item_index = 0;  //菜单栏初始化
uint8_t pre_item_index = 0;  //先前的菜单编号
uint8_t flag = 0;  //屏蔽按键标识
uint16_t Back_ground_color = 0x00;
uint16_t Back_ground_color_array[11] = {LGRAY, GRAY, WHITE, BLACK, LGRAYBLUE, YELLOW, GBLUE, MAGENTA, DARKBLUE, DARKGREEN, CYAN};

//结构体初始化//菜单定义,在这里将每一个菜单的关联设置好
Menu menu1_main[4] = // 第1级 主菜单 
{
	{4, "核心选项", "运行", TYPE_SUBMENU,0, NULL, menu2_run, NULL, Chinese, RED, 90, 90}, //注意，第一个的y不能是默认
	{4, "", "调试", TYPE_SUBMENU, NULL,0, menu2_debug, NULL, Chinese, ORANGE, 90, 0}, 
	{4, "", "设置", TYPE_SUBMENU, NULL,0, menu2_set, NULL, Chinese, YELLOW, 90, 0}, 
	{4, "", "版本信息", TYPE_SUBMENU, NULL,0, NULL, NULL, Chinese, DARKGREEN, 70, 0},  
}; 

Menu menu2_run[4] =  // 第2级 运行菜单 
{
	{4, "运行菜单", "自动运行", TYPE_SUBMENU,0, NULL, NULL, menu1_main, Chinese, RED, 70, 90}, 
	{4, "",     "手动运行", TYPE_SUBMENU,0, NULL, menu3_manual_run, menu1_main, Chinese, ORANGE, 70, 0}, 
	{4, "",     "参数监视", TYPE_SUBMENU,0, NULL, NULL, menu1_main, Chinese, YELLOW, 70, 0},
	{4, "",     "编程", TYPE_SUBMENU,0, NULL, NULL, menu1_main, Chinese, DARKGREEN, 90, 0},
};

Menu menu2_debug[6] =  // 第2级 调试菜单 
{
	{6, "调试菜单", "Speed_P", SPEED_P,255,(*Change_Param), NULL, menu1_main, English, RED, 10, 55}, 
	{6, "", "Speed_I", SPEED_I,255, (*Change_Param), NULL, menu1_main, English, ORANGE, 10, 0}, 
	{6, "", "Speed_D", SPEED_D,255,(*Change_Param), NULL, menu1_main, English, YELLOW, 10, 0},
	{6, "", "Angle_P", ANGLE_P,255, (*Change_Param), NULL, menu1_main, English, DARKGREEN, 10, 0},
	{6, "", "Angle_I", ANGLE_I,255, (*Change_Param), NULL, menu1_main, English, DODGERBLUE, 10, 0},
	{6, "", "Angle_D", ANGLE_D,255, (*Change_Param), NULL, menu1_main, English, DARKBLUE, 10, 0},
};

Menu menu2_set[2] =  // 第2级 设置菜单 
{
	{2, "设置菜单", "小车设置", TYPE_SUBMENU,0, NULL, menu3_car_set, menu1_main, Chinese, RED, 70, 90}, 
	{2, "",     "屏幕设置", TYPE_SUBMENU,0, NULL, menu3_LCD_set, menu1_main, Chinese, ORANGE, 70, 0}, 
};

Menu menu3_manual_run[6] =  // 第3级 手动运行 
{
	{6, "手动运行", "Motor1_Speed", MOTOR1_SPEED,100, (*Change_Param), NULL, menu2_run, English, RED, 10, 55}, 
	{6, "",     "Motor2_Speed", MOTOR2_SPEED,100, (*Change_Param), NULL, menu2_run, English, ORANGE, 10, 0}, 
	{6, "",     "Motor1_Status", MOTOR1_STATUS,1, (*Change_Param), NULL, menu2_run, English, YELLOW, 10, 0},
	{6, "",     "Motor2_Status", MOTOR2_STATUS,1, (*Change_Param), NULL, menu2_run, English, DARKGREEN, 10, 0},
	{6, "",     "Run_time", RUN_TIME,60, (*Change_Param), NULL, menu2_run, English, DODGERBLUE, 10, 0},
	{6, "",     "<<< Run >>>", TYPE_SPECIAL_PARAM,0, NULL, NULL, menu2_run, English, DARKBLUE, 55, 0},
};

Menu menu3_car_set[6] =  //第3级 小车设置
{
	{6, "小车设置", "RGB_Light", RGB_LIGHT,1, (*Change_Param), NULL, menu2_set, English, RED, 10, 55},
	{6, "", "Max_Speed", MAX_SPEED,100, (*Change_Param), NULL, menu2_set, English, ORANGE, 10, 0},
	{6, "", "Swerve_Speed", SWERVE_SPEED,100, (*Change_Param), NULL, menu2_set, English, YELLOW, 10, 0},
	{6, "", "EEPROM_Out", TYPE_SPECIAL_PARAM,0, (*EEPROM_OUT), NULL, menu2_set, English, DARKGREEN, 10, 0},
	{6, "", "EEPROM_Lock", TYPE_SPECIAL_PARAM,0, NULL, NULL, menu2_set, English, DODGERBLUE, 10, 0},
	{6, "", "EEPROM_Erase", TYPE_SPECIAL_PARAM,0, NULL, NULL, menu2_set,English, DARKBLUE, 10, 0},
};

Menu menu3_LCD_set[4] =  //第3级 LCD设置
{
	{4, "屏幕设置", "LCD_Brightness", LCD_BRIGHTLESS,99, (*Change_Param), NULL, menu2_set, English, RED, 10, 55},
	{4, "", "LCD_B_color", LCD_B_COLOR,10, (*Change_Param), NULL, menu2_set, English, ORANGE, 10, 0},
	{4, "", "Sleep", SLEEP,1, (*Change_Param), NULL, menu2_set, English, YELLOW, 10, 0},
	{4, "", "Sleep_time", SLEEP_TIME,255, (*Change_Param), NULL, menu2_set, English, DARKGREEN, 10, 0},
};

void DispCrtMenu(void)  //绘制当前菜单项
{
	if(Key_val==KEY_ENTER_PRESS||Key_val==KEY_RETURN_PRESS)  //在退出时刷新屏幕
	{
		LCD_Fill(0,0,240,240,Back_ground_color);
		LCD_Fill(0, 0, 240, 50, GRAYBLUE);  //打印标题框
	}
	uint8_t menu_num = cur_item[0].num;  //获取当前菜单的项目数量 
	uint8_t i=0, num = menu_num>MENU_MAX_ROW ? MENU_MAX_ROW : menu_num;  //绘制数量不能超过每一屏的最大绘制数量
	if(item_index>=MENU_MAX_ROW || item_index>=menu_num)  //菜单项上下选择是否越界 
	{ 
		if(item_index==0XFF)  //此情况为 item_index=0 再按上键，0-1=0XFF
		{
			item_index = menu_num - 1;   //循环，回到最后一个功能索引值
		}
		if(item_index>=menu_num)  //此情况为到达最下面一个功能索引值
		{
			item_index = 0;  //循环，回到第一个功能索引值
		}
		if(item_index>=MENU_MAX_ROW) 
		{
			item_index = 0;
		}
	} 
	
	DrawTitle((uint8_t*)cur_item[0].title);  //绘制菜单栏标题
	
	if (cur_item[i].ce == Chinese)  //判断条目是中文还是英文
	{
		for (i=0; i<num; i++)
		{
			if (cur_item[i].Y_coord == 0)
			{
				LCD_ShowChinese(cur_item[i].X_coord,cur_item->Y_coord+(i*30),(u8 *)cur_item[i].label, cur_item[i].Color, i==item_index ? GRAYBLUE:Back_ground_color, 24, 0);  //绘制中文
			}
			else  //不是默认的话按设定绘制
			{
				LCD_ShowChinese(cur_item[i].X_coord,cur_item[i].Y_coord,(u8 *)cur_item[i].label, cur_item[i].Color, i==item_index ? GRAYBLUE:Back_ground_color, 24, 0);
			}
		}
	}
	else if (cur_item[i].ce == English)
	{
		for (i=0; i<num; i++)//绘制某一级菜单下的功能键 
		{
			if (cur_item[i].Y_coord == 0)
			{	
				LCD_ShowString(cur_item[i].X_coord,cur_item->Y_coord+(i*30),(u8* )cur_item[i].label, cur_item[i].Color, i==item_index ? GRAYBLUE:Back_ground_color, 24, 0);  //绘制英文
			}
			else  //不是默认的话按设定绘制
			{
				LCD_ShowString(cur_item[i].X_coord,cur_item[i].Y_coord,(u8 *)cur_item[i].label, cur_item[i].Color, i==item_index ? GRAYBLUE:Back_ground_color, 24, 0);
			}
			
			/*一下部分为打印参数部分*/
			if (cur_item[i].type != TYPE_SUBMENU && cur_item[i].type != TYPE_SPECIAL_PARAM)
			{
				uint8_t Data[4];
				EEPROM_ReadMultipleBytes(cur_item[i].type, Data, 4);
				LCD_ShowIntNum(195,cur_item->Y_coord+(i*30), Data[3], 3, cur_item[i].Color,Back_ground_color, 24);
			}
		}
	}
}



//显示函数
void Display(void) 
{
	if(Key_val==KEY_UP_PRESS || Key_val==KEY_DOWN_PRESS || Key_val==KEY_ENTER_PRESS || Key_val==KEY_RETURN_PRESS)
	{
		switch(Key_val)//检测按键，进入相应动作
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
				switch(cur_item[item_index].type)//检测功能项的类型，进入相应动作
				{
					case TYPE_SUBMENU: //具有子菜单的菜单项
						if(cur_item[item_index].next != NULL)
						{ 
							prev_item = cur_item;//此级菜单变成了上一级菜单 
							cur_item = cur_item[item_index].next;//将指向的下一级菜单设置为当前菜单 
							pre_item_index = item_index;  
							item_index = 0;//重置子菜单项索引 
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
							//绘制选中栏
							uint8_t Data[4];
							LCD_Fill(cur_item[item_index].X_coord,cur_item->Y_coord+(item_index*30), 230, 24+(cur_item->Y_coord+(item_index*30)), GRAYBLUE);
							EEPROM_ReadMultipleBytes(cur_item[item_index].type, Data, 4);
							LCD_ShowIntNum(195,cur_item->Y_coord+(item_index*30), Data[3], 3, cur_item[item_index].Color,GRAYBLUE, 24);
							LCD_ShowString(cur_item[item_index].X_coord,cur_item->Y_coord+(item_index*30),(u8* )cur_item[item_index].label, cur_item[item_index].Color, GRAYBLUE, 24, 0);
							
							//屏蔽按键
							flag = 1;
							//调用相应的动作函数
							cur_item[item_index].Function();
							//恢复按键屏蔽
							flag = 0;
							
						}
						else
						{
							LCD_ShowString(cur_item[item_index].X_coord,cur_item->Y_coord+(item_index*30),(u8* )cur_item[item_index].label, cur_item[item_index].Color, GRAYBLUE, 24, 0);
							LCD_Fill(cur_item[item_index].X_coord,cur_item->Y_coord+(item_index*30), 230, 24+(cur_item->Y_coord+(item_index*30)), GRAYBLUE);
							//屏蔽按键
							flag = 1;
							//调用相应的动作函数
							cur_item[item_index].Function();
							//恢复按键屏蔽
							flag = 0;
							
						}
						break;
				}
				break; 
			case KEY_RETURN_PRESS: 
				if (prev_item != NULL)//返回上一级菜单的操作 
				{
					cur_item = prev_item;  //设置上一级菜单为当前菜单 
					prev_item = cur_item[0].prev;  //设置当前菜单的上一级菜单 
					item_index = pre_item_index;  //重置子菜单项索引
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
	
	Key_val = 0x00;  //重置Key_val

} 

void DrawTitle(uint8_t* Title_Name)
{
	LCD_ShowChinese(50, 10, Title_Name, Title_color, GRAYBLUE, 32, 0);  //打印标题
}
