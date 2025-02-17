#include "ws2812.h"

Color_tpye color={0,0,0};//�ƴ�rgbֵ���洦

//ʹ��PWM+DMA����ws2812ʱҪע�ⶨʱ���Ƕ���λ�ģ������16λ��������DMAΪ���֣������32λ��������DMAΪȫ�� 
 
//�Դ����飬����Ϊ �Ƶ�����*24+��λ����
uint32_t WS2812_RGB_Buff[LED_NUM*DATA_LEN+WS2812_RST_NUM] = {0}; 
 
/**
 * ������WS2812�������ú���
 * ������num:�Ƶ�λ�ã�R��G��B�ֱ�Ϊ������ɫͨ�������ȣ����ֵΪ255
 * ���ã���������ÿһ��WS2812����ɫ
***/
void WS2812_Set(uint16_t num,uint8_t R,uint8_t G,uint8_t B)
{
  uint32_t indexx=(num*(3*8));
  for (uint8_t i = 0;i < 8;i++)
  {
	//�������
	WS2812_RGB_Buff[indexx+i]      = (G << i) & (0x80)?WS_H:WS_L;
	WS2812_RGB_Buff[indexx+i + 8]  = (R << i) & (0x80)?WS_H:WS_L;
	WS2812_RGB_Buff[indexx+i + 16] = (B << i) & (0x80)?WS_H:WS_L;
  }
}
 
//WS2812��ʼ������
void WS2812_Init()
{
	//���ùر����е�
  for(int i=0;i<LED_NUM;i++)
  {
			WS2812_Set(i,0,0,0);
  }
  //���ã�����DMA���Դ��е�����ʵʱ��������ʱ���ıȽϼĴ���
  HAL_TIM_PWM_Start_DMA(&htim2,TIM_CHANNEL_4,(uint32_t *)WS2812_RGB_Buff,sizeof(WS2812_RGB_Buff)/sizeof(uint32_t)); 
	
}
/**
 * ������WS2812��ɫ���ú���
 * ������R��G��B�ֱ�Ϊ������ɫͨ�������ȣ����ֵΪ255
 * ���ã�������ˮ����ɫ
***/
void WS2812_Set_Color(uint8_t R,uint8_t G,uint8_t B)
{
	color.B=B;
	color.R=R;
	color.G=G;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint32_t turn=0,Num=0;
	if(htim==&htim2)
	{
		turn++;
	}
	if(turn==80000)
	{
		turn=0;
		for(int i=0;i<LED_NUM;i++)
		{
			WS2812_Set(i,0,0,0);
		}
		if(Num<=3)WS2812_Set(Num%4,color.R/10,color.G/10,color.B/10);
		if(Num>=1&&Num<=4)WS2812_Set((Num-1)%4,color.R,color.G,color.B);
		if(Num>=2&&Num<=5)WS2812_Set((Num-2)%4,color.R/10,color.G/10,color.B/10);
		if(Num<=3)WS2812_Set(7-((Num-0)%4),color.R/10,color.G/10,color.B/10);
		if(Num>=1&&Num<=4)WS2812_Set(7-((Num-1)%4),color.R,color.G,color.B);
		if(Num>=2&&Num<=5)WS2812_Set(7-((Num-2)%4),color.R/10,color.G/10,color.B/10);
		Num++;
		if(Num==8)
		{
			Num=0;
		}	
	}
}
void WS2812_Off(void)
{
	HAL_TIM_Base_Stop_IT(&htim2);
	for(int i=0;i<LED_NUM;i++)
		{
			WS2812_Set(i,0,0,0);
		}
}
