#ifndef __UI_H__
#define __UI_H__
#include "main.h"

//定义菜单中功能项的类型
#define TYPE_SUBMENU 101  //具有子菜单的菜单项
#define TYPE_PARAM   102  //参数项（用于执行参数设置）
#define MENU_MAX_ROW 10  //每页最大绘制菜单栏个数


typedef void (*MENU_FUN)(const char *);
//定义菜单
typedef struct menu
{
	uint8_t num;        //当前菜单功能项总数
	char * title;       //当前菜单标题
	char * label;       //功能项标题
	uint8_t type;       //当前功能项的类型
	MENU_FUN Function;  //选择某一功能后执行的功能函数
	struct menu *next;  //下一级菜单
	struct menu *prev;  //上一级菜单

} Menu;

Menu menu1_main[4];
Menu menu2_run[4];
Menu menu2_debug[6];
Menu menu2_set[2];
Menu menu3_manual_run[6];
Menu menu3_car_set[7];
Menu menu3_LCD_set[4];

void DrawTitle(uint8_t* Title_Name);

#endif