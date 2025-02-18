#ifndef __UI_H__
#define __UI_H__
#include "lcd.h"

//����˵��й����������
#define TYPE_SUBMENU 101  //�����Ӳ˵��Ĳ˵���
#define TYPE_PARAM   102  //���������ִ�в������ã�
#define MENU_MAX_ROW 10  //ÿҳ�����Ʋ˵�������
#define KEY_UP_PRESS 0xFF
#define KEY_DOWN_PRESS 0xFE
#define KEY_ENTER_PRESS 0xFD
#define KEY_RETURN_PRESS 0xFC


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

extern Menu menu1_main[4];
extern Menu menu2_run[4];
extern Menu menu2_debug[6];
extern Menu menu2_set[2];
extern Menu menu3_manual_run[6];
extern Menu menu3_car_set[7];
extern Menu menu3_LCD_set[4];

void DrawTitle(uint8_t* Title_Name);
void DispCrtMenu(void);
void Display(void);

extern uint8_t Key_val;

#endif