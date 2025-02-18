#include "UI.h"
#include "lcd.h"

//定义菜单操作需要的全局变量
Menu *cur_item = menu1_main;  //初始化当前菜单为第一级(menu1_main)
Menu *prev_item = NULL;	    //初始化上一级菜单为空
uint8_t item_index = 0;//当前菜单项索引

//结构体初始化//菜单定义,在这里将每一个菜单的关联设置好
Menu menu1_main[4] = // 第1级 主菜单 
{
	{4, "主菜单", "运行", TYPE_SUBMENU, NULL, menu2_run, NULL}, 
	{4, "", "调试", TYPE_SUBMENU, NULL, menu2_debug, NULL}, 
	{4, "", "设置", TYPE_SUBMENU, NULL, menu2_set, NULL}, 
	{4, "", "版本信息", TYPE_SUBMENU, NULL, NULL, NULL},  
}; 

Menu menu2_run[4] =  // 第2级 运行菜单 
{
	{4, "运行", "自动运行", TYPE_SUBMENU, NULL, NULL, menu1_main}, 
	{4, "",     "手动运行", TYPE_SUBMENU, NULL, menu3_manual_run, menu1_main}, 
	{4, "",     "参数监视", TYPE_SUBMENU, NULL, NULL, menu1_main},
	{4, "",     "编程", TYPE_SUBMENU, NULL, NULL, menu1_main},
};

Menu menu2_debug[6] =  // 第2级 调试菜单 
{
	{6, "调试", "Speed_P", TYPE_PARAM, NULL, NULL, menu1_main}, 
	{6, "", "Speed_I", TYPE_PARAM, NULL, NULL, menu1_main}, 
	{6, "", "Speed_D", TYPE_PARAM, NULL, NULL, menu1_main},
	{6, "", "Angle_P", TYPE_PARAM, NULL, NULL, menu1_main},
	{6, "", "Angle_I", TYPE_PARAM, NULL, NULL, menu1_main},
	{6, "", "Angle_D", TYPE_PARAM, NULL, NULL, menu1_main},
};

Menu menu2_set[2] =  // 第2级 设置菜单 
{
	{2, "设置", "小车设置", TYPE_SUBMENU, NULL, menu3_car_set, menu1_main}, 
	{2, "",     "LCD设置", TYPE_SUBMENU, NULL, menu3_LCD_set, menu1_main}, 
};

Menu menu3_manual_run[6] =  // 第3级 手动运行 
{
	{6, "手动运行", "Motor1_Speed", TYPE_PARAM, NULL, NULL, menu2_run}, 
	{6, "",     "Motor2_Speed", TYPE_PARAM, NULL, NULL, menu2_run}, 
	{6, "",     "Motor1_Status", TYPE_PARAM, NULL, NULL, menu2_run},
	{6, "",     "Motor2_Status", TYPE_PARAM, NULL, NULL, menu2_run},
	{6, "",     "Run_time", TYPE_PARAM, NULL, NULL, menu2_run},
	{6, "",     "Run", TYPE_PARAM, NULL, NULL, menu2_run},
};

Menu menu3_car_set[7] =  //第3级 小车设置
{
	{7, "小车设置", "RGB_Light", TYPE_PARAM, NULL, NULL, menu2_set},
	{7, "", "Run_mode", TYPE_PARAM, NULL, NULL, menu2_set},
	{7, "", "Max_Speed", TYPE_PARAM, NULL, NULL, menu2_set},
	{7, "", "Swerve_Speed", TYPE_PARAM, NULL, NULL, menu2_set},
	{7, "", "EEPROM_Output", TYPE_PARAM, NULL, NULL, menu2_set},
	{7, "", "EEPROM_Lock", TYPE_PARAM, NULL, NULL, menu2_set},
	{7, "", "EEPROM_Erase", TYPE_PARAM, NULL, NULL, menu2_set},
};

Menu menu3_LCD_set[4] =  //第3级 LCD设置
{
	{4, "LCD设置", "LCD_Brightness", TYPE_PARAM, NULL, NULL, menu2_set},
	{4, "", "LCD_Background_color", TYPE_PARAM, NULL, NULL, menu2_set},
	{4, "", "Sleep", TYPE_PARAM, NULL, NULL, menu2_set},
	{4, "", "Sleep_time", TYPE_PARAM, NULL, NULL, menu2_set},
};

void DispCrtMenu(void)//绘制当前菜单项
{
	uint8_t menu_num = cur_item[0].num;//获取当前菜单的项目数量 
	uint8_t i, num = menu_num>MENU_MAX_ROW ? MENU_MAX_ROW : menu_num;//绘制数量不能超过每一屏的最大绘制数量
	if(item_index>=MENU_MAX_ROW || item_index>=menu_num)//菜单项上下选择是否越界 
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
	
	for (i=0; i<num; i++)//绘制某一级菜单下的功能键 
	{
		uint16_t Pointer_Color = LIGHTBLUE;
		LCD_ShowString(144,150+(i+1)*40,200,30,24, (u8 *)cur_item[i].label,i==item_index ? 0:1);
	}
}



//显示函数
void Display(uint8_t value) 
{
	if(value==KEY_UP_PRESS || value==KEY_DOWN_PRESS || value==KEY_ENTER_PRESS || value==KEY_RETURN_PRESS)
	{
		switch(value)//检测按键，进入相应动作
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
							item_index = 0;//重置子菜单项索引 
							DispCrtMenu();
						}
						else
						{
							POINT_COLOR = WHITE;
							LCD_Fill(0,lcddev.height-40,lcddev.width,lcddev.height,BLUE);
							Gui_StrCenter(0,lcddev.height-32,"待设置~~~",24,1);//居中显示
						}
						break; 
					case TYPE_PARAM:  //具有参数设置的菜单项
						if(cur_item[item_index].Function != NULL)
						{ 
							//调用相应的动作函数,并传递参数
							cur_item[item_index].Function((const char *)cur_item[item_index].label);
						}
						else
						{
							POINT_COLOR = WHITE;
							LCD_Fill(0,lcddev.height-40,lcddev.width,lcddev.height,BLUE);
							Gui_StrCenter(0,lcddev.height-32,"待设置~~~",24,1);//居中显示
						}
						break; 
					default: 
						break;
				}
				break; 
			case KEY_RETURN_PRESS: 
				if (prev_item != NULL)//返回上一级菜单的操作 
				{
					cur_item = prev_item;  //设置上一级菜单为当前菜单 
					prev_item = cur_item[0].prev;  //设置当前菜单的上一级菜单 
					item_index = 0;  //重置子菜单项索引
					DispCrtMenu();
				} 
				else
				{
					POINT_COLOR = WHITE;
					LCD_Fill(0,lcddev.height-40,lcddev.width,lcddev.height,BLUE);
					Gui_StrCenter(0,lcddev.height-32,"已是主菜单",24,1);//居中显示
				}
				break; 
		    default: 
		        break;
		}
	}
} 

void DrawTitle(uint8_t* Title_Name)
{
	;
}

