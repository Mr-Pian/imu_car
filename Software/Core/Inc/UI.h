#ifndef __UI_H__
#define __UI_H__
#include "lcd.h"

//定义菜单中功能项的类型
#define TYPE_SUBMENU 101  //具有子菜单的菜单项
#define TYPE_SPECIAL_PARAM   102  //特殊参数项
#define MENU_MAX_ROW 6  //每页最大绘制菜单栏个数

//键值
#define KEY_UP_PRESS 0xFF  //上
#define KEY_DOWN_PRESS 0xFE  //下
#define KEY_ENTER_PRESS 0xFD  //确认
#define KEY_RETURN_PRESS 0xFC  //退出

//语言识别码
#define Chinese 0
#define English 1

#define Title_color WHITE  //标题字体颜色

//定义菜单结构体
typedef struct menu
{
	uint8_t num;        //当前菜单功能项总数
	char * title;       //当前菜单标题
	char * label;       //功能项标题
	uint8_t type;       //当前功能项的类型
	uint32_t up_to;      //当前参数的最大值，不是参数设为0
	void (*Function)(void);  //选择某一功能后执行的功能函数
	struct menu *next;  //下一级菜单
	struct menu *prev;  //上一级菜单
	uint8_t ce;         //语言
	uint16_t Color;     //本条目颜色
	uint16_t X_coord;   //起始x坐标——方便居中
	uint16_t Y_coord;  //起始y坐标
} Menu;

extern Menu menu1_main[4];
extern Menu menu2_run[4];
extern Menu menu2_debug[6];
extern Menu menu2_set[2];
extern Menu menu3_manual_run[6];
extern Menu menu3_car_set[6];
extern Menu menu3_LCD_set[4];
extern Menu menu3_auto_run[3];

void DrawTitle(uint8_t* Title_Name);
void DispCrtMenu(void);
void Display(void);

extern uint8_t Key_val;  //全局变量键值
extern Menu *cur_item;  //全局当前item
extern uint8_t item_index; //全局当前栏目位置
extern uint8_t flag;  //屏蔽按键标志

extern uint16_t Back_ground_color;  //整体背景的颜色
extern uint16_t Back_ground_color_array[11];  //储存颜色的数组

#endif
