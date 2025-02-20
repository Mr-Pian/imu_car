#include "ws2812.h"

Color_tpye color={0,0,0};//�ƴ�rgbֵ���洦

long dif_l=0,dif_r=0;

//ʹ��PWM+DMA����ws2812ʱҪע�ⶨʱ���Ƕ���λ�ģ������16λ��������DMAΪ���֣������32λ��������DMAΪȫ�� 
 
//�Դ����飬����Ϊ �Ƶ�����*24+��λ����
uint32_t WS2812_RGB_Buff[LED_NUM*DATA_LEN+WS2812_RST_NUM] = {0}; 
 
/**
 * ������WS2812�������ú���
 * ������num:�Ƶ�λ�ã�R��G��B�ֱ�Ϊ������ɫͨ�������ȣ����ֵΪ255
 * ���ã���������ÿһ��WS2812����ɫ
***/
void WS2812_Set(uint16_t num,uint8_t R,uint8_t Gr,uint8_t B)
{
  uint32_t indexx=(num*(3*8));
  for (uint8_t i = 0;i < 8;i++)
  {
	//�������
	WS2812_RGB_Buff[indexx+i]      = (Gr << i) & (0x80)?WS_H:WS_L;
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

/************************************************************************************************************
** WS2812_Set_Color(uint8_t R,uint8_t G,uint8_t B)                  				                               **                                                              
** ���������������Ч����������                                                                        		   **
** ����˵����R��G��B�ֱ�Ϊ������ɫͨ�������ȣ����ֵΪ255                          					                   **   
** �������أ���                                                                                             **
************************************************************************************************************/
void WS2812_Set_Color(uint8_t R,uint8_t Gr,uint8_t B)
{
	color.B=B;
	color.R=R;
	color.Gr=Gr;
	HAL_TIM_PWM_Stop_DMA(&htim2,TIM_CHANNEL_4);  //������ʱ��
}


/************************************************************************************************************
** void WS2812_Off(void)                  				                                                         **                                                              
** ���������������Ч���رպ���                                                                        		   **
** ����˵������                                                                					                   **   
** �������أ���                                                                                             **
************************************************************************************************************/
void WS2812_Off(void)
{
	HAL_TIM_Base_Stop_IT(&htim2);
	for(int i=0;i<LED_NUM;i++)
		{
			WS2812_Set(i,0,0,0);
		}
}
/************************************************************************************************************
** void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)               				                       **                                                              
** ����������tim�жϻص���������������������밴������                                                   		   **
** ����˵������                                                                					                   **   
** �������أ���                                                                                             **
************************************************************************************************************/

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint32_t turn=0,Num=0, com_l=0,last_com_l=0,com_r=0,last_com_r=0;
	if(htim==&htim2)
	{
		turn++;
		
	}
	if(turn==80000)
	{
		turn=0;
		com_l=__HAL_TIM_GET_COUNTER(&htim4);//��
		com_r=__HAL_TIM_GET_COUNTER(&htim8);//��
		dif_l=com_l-last_com_l;
		if(dif_l>60000)
		{
			dif_l=65535-dif_l;
		}
		dif_r=com_r-last_com_r;
		if(dif_r>60000)
		{
			dif_r=65535-dif_r;
		}
		last_com_l=com_l;
		last_com_r=com_r;
		for(int i=0;i<LED_NUM;i++)
		{
			WS2812_Set(i,0,0,0);
		}
		if(Num<=3)WS2812_Set(Num%4,color.R/10,color.Gr/10,color.B/10);
		if(Num>=1&&Num<=4)WS2812_Set((Num-1)%4,color.R,color.Gr,color.B);
		if(Num>=2&&Num<=5)WS2812_Set((Num-2)%4,color.R/10,color.Gr/10,color.B/10);
		if(Num<=3)WS2812_Set(7-((Num-0)%4),color.R/10,color.Gr/10,color.B/10);
		if(Num>=1&&Num<=4)WS2812_Set(7-((Num-1)%4),color.R,color.Gr,color.B);
		if(Num>=2&&Num<=5)WS2812_Set(7-((Num-2)%4),color.R/10,color.Gr/10,color.B/10);
		Num++;
		if(Num==8)
		{
			Num=0;
		}	
	}
	if(htim==&htim3)
	{
		if (HAL_GPIO_ReadPin(GPIOB, Key_No_Pin) == GPIO_PIN_RESET)
		{
			Key_val = KEY_RETURN_PRESS;
		}
		if (HAL_GPIO_ReadPin(GPIOB, Key_Yes_Pin) == GPIO_PIN_RESET)
		{
			Key_val = KEY_ENTER_PRESS;
		}
		if (HAL_GPIO_ReadPin(GPIOC, Key_Up_Pin) == GPIO_PIN_RESET)
		{
			Key_val = KEY_UP_PRESS;
		}
		if (HAL_GPIO_ReadPin(GPIOC, Key_Down_Pin) == GPIO_PIN_RESET)
		{
			Key_val = KEY_DOWN_PRESS;
		}
		Display();
		HAL_TIM_Base_Stop_IT(&htim3);
	}
}

