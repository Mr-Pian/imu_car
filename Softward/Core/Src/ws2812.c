#include "ws2812.h"

Color_tpye color={0,0,0};//灯带rgb值储存处

//使用PWM+DMA驱动ws2812时要注意定时器是多少位的，如果是16位计数器则DMA为半字，如果是32位计数器则DMA为全字 
 
//显存数组，长度为 灯的数量*24+复位周期
uint32_t WS2812_RGB_Buff[LED_NUM*DATA_LEN+WS2812_RST_NUM] = {0}; 
 
/**
 * 函数：WS2812单灯设置函数
 * 参数：num:灯的位置，R、G、B分别为三个颜色通道的亮度，最大值为255
 * 作用：单独设置每一个WS2812的颜色
***/
void WS2812_Set(uint16_t num,uint8_t R,uint8_t G,uint8_t B)
{
  uint32_t indexx=(num*(3*8));
  for (uint8_t i = 0;i < 8;i++)
  {
	//填充数组
	WS2812_RGB_Buff[indexx+i]      = (G << i) & (0x80)?WS_H:WS_L;
	WS2812_RGB_Buff[indexx+i + 8]  = (R << i) & (0x80)?WS_H:WS_L;
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
  //作用：调用DMA将显存中的内容实时搬运至定时器的比较寄存器
  HAL_TIM_PWM_Start_DMA(&htim2,TIM_CHANNEL_4,(uint32_t *)WS2812_RGB_Buff,sizeof(WS2812_RGB_Buff)/sizeof(uint32_t)); 
	
}
/**
 * 函数：WS2812颜色设置函数
 * 参数：R、G、B分别为三个颜色通道的亮度，最大值为255
 * 作用：设置流水灯颜色
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
