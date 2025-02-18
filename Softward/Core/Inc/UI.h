#ifndef __UI_H__
#define __UI_H__
#include "main.h"

//����˵��й����������
#define TYPE_SUBMENU 101  //�����Ӳ˵��Ĳ˵���
#define TYPE_PARAM   102  //���������ִ�в������ã�
#define MENU_MAX_ROW 10  //ÿҳ�����Ʋ˵�������


typedef void (*MENU_FUN)(const char *);
//����˵�
typedef struct menu
{
	uint8_t num;        //��ǰ�˵�����������
	char * title;       //��ǰ�˵�����
	char * label;       //���������
	uint8_t type;       //��ǰ�����������
	MENU_FUN Function;  //ѡ��ĳһ���ܺ�ִ�еĹ��ܺ���
	struct menu *next;  //��һ���˵�
	struct menu *prev;  //��һ���˵�

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