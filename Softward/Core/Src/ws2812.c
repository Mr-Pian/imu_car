#include "ws2812.h"
#include "UI.h"
#include "M24_EEPROM.h"
#include "Functions.h"

Color_type color={0,0,0};//灯带rgb值储存处
long dif_l,dif_r;

//使用PWM+DMA驱动ws2812时要注意定时器是多少位的，如果是16位计数器则DMA为半字，如果是32位计数器则DMA为全字 
 
//显存数组，长度为 灯的数量*24+复位周期
uint32_t WS2812_RGB_Buff[LED_NUM*DATA_LEN+WS2812_RST_NUM] = {0}; 
 
/**
 * 函数：WS2812单灯设置函数
 * 参数：num:灯的位置，R、G、B分别为三个颜色通道的亮度，最大值为255
 * 作用：单独设置每一个WS2812的颜色
***/
void WS2812_Set(uint16_t num,uint8_t Re,uint8_t G,uint8_t B)
{
  uint32_t indexx=(num*(3*8));
  for (uint8_t i = 0;i < 8;i++)
  {
	//填充数组
	WS2812_RGB_Buff[indexx+i]      = (G << i) & (0x80)?WS_H:WS_L;
	WS2812_RGB_Buff[indexx+i + 8]  = (Re << i) & (0x80)?WS_H:WS_L;
	WS2812_RGB_Buff[indexx+i + 16] = (B << i) & (0x80)?WS_H:WS_L;
  }
}
 
//WS2812初始化函数
void WS2812_Init()
{
	//设置关闭所有灯
  for(int i=0;i<LED_NUM;i++)
  {
			WS2812_Set(i,0,0,0);
  }
	
	uint8_t Data[4] = {0};
	EEPROM_ReadMultipleBytes(RGB_LIGHT, Data, 4);
	
	if (Data[3] == 1)
	{
		WS2812_Set_Color(255, 105, 180);
	}
	
}

/************************************************************************************************************
** WS2812_Set_Color(uint8_t R,uint8_t G,uint8_t B)                  				                               **                                                              
** 功能描述：跑马灯效果开启函数                                                                        		   **
** 参数说明：R、G、B分别为三个颜色通道的亮度，最大值为255                          					                   **   
** 参数返回：无                                                                                             **
************************************************************************************************************/
void WS2812_Set_Color(uint8_t Re,uint8_t G,uint8_t B)
{
	color.B=B;
	color.Re=Re;
	color.G=G;
	HAL_TIM_PWM_Start_DMA(&htim2,TIM_CHANNEL_4,(uint32_t *)WS2812_RGB_Buff,sizeof(WS2812_RGB_Buff)/sizeof(uint32_t)); 
}


/************************************************************************************************************
** void WS2812_Off(void)                  				                                                         **                                                              
** 功能描述：跑马灯效果关闭函数                                                                        		   **
** 参数说明：无                                                                					                   **   
** 参数返回：无                                                                                             **
************************************************************************************************************/
void WS2812_Off(void)
{
	for(int i=0;i<LED_NUM;i++)
		{
			WS2812_Set(i,0,0,0);
		}
	HAL_TIM_PWM_Stop_DMA(&htim2,TIM_CHANNEL_4);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint32_t turn=0,Num=0,turn_dmp=0;
	static int com_l=0,last_com_l=0,com_r=0,last_com_r=0;
	if(htim==&htim11)
	{
		com_l=__HAL_TIM_GET_COUNTER(&htim4);//左
		com_r=__HAL_TIM_GET_COUNTER(&htim8);//右
		dif_l=-com_l+last_com_l;
		if(dif_l>60000)dif_l=dif_l-65535;
		if(dif_l<-60000)dif_l=dif_l+65535;
		dif_r=com_r-last_com_r;
		if(dif_r>60000)dif_r-=65535;
		if(dif_r<-60000)dif_r+=65535;
		last_com_l=com_l;
		last_com_r=com_r;
	}
	if(htim==&htim2)
	{
		turn++;
		turn_dmp++;
	}
	if(turn_dmp==4000)
	{
		turn_dmp=0;
		IMU_GetAngle(0.1);
		IMU_DataUpdate();

	}
	if(turn==80000)
	{
		turn=0;
		
		for(int i=0;i<LED_NUM;i++)
		{
			WS2812_Set(i,0,0,0);
		}
		if(Num<=3)WS2812_Set(Num%4,color.Re/10,color.G/10,color.B/10);
		if(Num>=1&&Num<=4)WS2812_Set((Num-1)%4,color.Re,color.G,color.B);
		if(Num>=2&&Num<=5)WS2812_Set((Num-2)%4,color.Re/10,color.G/10,color.B/10);
		if(Num<=3)WS2812_Set(7-((Num-0)%4),color.Re/10,color.G/10,color.B/10);
		if(Num>=1&&Num<=4)WS2812_Set(7-((Num-1)%4),color.Re,color.G,color.B);
		if(Num>=2&&Num<=5)WS2812_Set(7-((Num-2)%4),color.Re/10,color.G/10,color.B/10);
		Num++;
		if(Num==8)
		{
			Num=0;
		}	
	}
	if(htim==&htim9)
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
		HAL_TIM_Base_Stop_IT(&htim9);
	}
}

