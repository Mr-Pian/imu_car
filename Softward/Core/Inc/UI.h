#ifndef __UI_H__
#define __UI_H__
#include "lcd.h"

//����˵��й����������
#define TYPE_SUBMENU 101  //�����Ӳ˵��Ĳ˵���
#define TYPE_SPECIAL_PARAM   102  //���������
#define MENU_MAX_ROW 6  //ÿҳ�����Ʋ˵�������

//��ֵ
#define KEY_UP_PRESS 0xFF  //��
#define KEY_DOWN_PRESS 0xFE  //��
#define KEY_ENTER_PRESS 0xFD  //ȷ��
#define KEY_RETURN_PRESS 0xFC  //�˳�

//����ʶ����
#define Chinese 0
#define English 1

#define Title_color WHITE  //����������ɫ

//����˵��ṹ��
typedef struct menu
{
	uint8_t num;        //��ǰ�˵�����������
	char * title;       //��ǰ�˵�����
	char * label;       //���������
	uint8_t type;       //��ǰ�����������
	uint32_t up_to;      //��ǰ���������ֵ�����ǲ�����Ϊ0
	void (*Function)(void);  //ѡ��ĳһ���ܺ�ִ�еĹ��ܺ���
	struct menu *next;  //��һ���˵�
	struct menu *prev;  //��һ���˵�
	uint8_t ce;         //����
	uint16_t Color;     //����Ŀ��ɫ
	uint16_t X_coord;   //��ʼx���ꡪ���������
	uint16_t Y_coord;  //��ʼy����
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

extern uint8_t Key_val;  //ȫ�ֱ�����ֵ
extern Menu *cur_item;  //ȫ�ֵ�ǰitem
extern uint8_t item_index; //ȫ�ֵ�ǰ��Ŀλ��
extern uint8_t flag;  //���ΰ�����־

extern uint16_t Back_ground_color;  //���屳������ɫ
extern uint16_t Back_ground_color_array[11];  //������ɫ������

#endif
