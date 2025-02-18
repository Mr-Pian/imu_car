#include "main.h"
#include "tim.h"

 
#define WS_H           70   // 1 ����Լ���ֵ
#define WS_L           35   // 0 ����Լ���ֵ
#define WS_REST        40   // ��λ�ź���������
#define LED_NUM        8    // WS2812�Ƹ���
#define DATA_LEN       24   // WS2812���ݳ��ȣ�������Ҫ24���ֽ�
#define WS2812_RST_NUM 50   // �ٷ���λʱ��Ϊ50us��40�����ڣ����������ʹ��50������

typedef struct 
{
	uint8_t R;
	uint8_t Gr;
	uint8_t B;
}Color_tpye;

extern Color_tpye color;

void WS2812_Init(void);
void WS2812_Set(uint16_t num,uint8_t R,uint8_t Gr,uint8_t B);
void WS2812_Set_Color(uint8_t R,uint8_t Gr,uint8_t B);
void WS2812_Off(void);
