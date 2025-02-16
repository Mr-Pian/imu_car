#include "RGB.h"
#include "tim.h"

/*Some Static Colors------------------------------*/
const RGB_Color_TypeDef RED      = {255,0,0};   //显示红色RGB数据
const RGB_Color_TypeDef GREEN    = {0,255,0};
const RGB_Color_TypeDef BLUE     = {0,0,255};
const RGB_Color_TypeDef SKY      = {0,255,255};
const RGB_Color_TypeDef MAGENTA  = {254,0,220};
const RGB_Color_TypeDef YELLOW   = {127,216,0};
const RGB_Color_TypeDef ORANGE   = {126,106,0};
const RGB_Color_TypeDef OFF    = {0,0,0};
const RGB_Color_TypeDef WHITE    = {254,254,254}; 
const RGB_Color_TypeDef LIGHTGREEN   = {27,216,0};

/*二维数组存放最终PWM输出数组，每一行24个
数据代表一个LED，最后一行24个0代表RESET码*/
uint32_t Pixel_Buf[Pixel_NUM][24];       
/*
功能：设定单个RGB LED的颜色，把结构体中RGB的24BIT转换为0码和1码
参数：LedId为LED序号，Color：定义的颜色结构体



功能：最后一行装在24个0，输出24个周期占空比为0的PWM波，作为最后reset延时，这里总时长为24*1.2=30us > 24us(要求大于24us)
*/


void  RGB_NewFrame(void){
	uint8_t i;
	uint8_t j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<24;j++)
		{
			Pixel_Buf[i][j]= CODE_0;
		}
	};
}

/*
功能：发送数组
参数：(&htim2)定时器2，(TIM_CHANNEL_4)通道4，((uint32_t *)Pixel_Buf)待发送数组，
			(Pixel_NUM+1)*24)发送个数，数组行列相乘
*/
void RGB_SendArray(void)
{
	HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_4, (uint32_t *)Pixel_Buf,16*24);
}

void RGB_SetOne_Color(uint8_t x,uint8_t y,RGB_Color_TypeDef Color)
{
	uint8_t i; 
	uint8_t j; 
	if(x>8||y>8)return;
	j=x;

	for(i=0;i<8;i++) Pixel_Buf[j][i]   = ( (Color.G & (1 << (7 -i)))? (CODE_1):CODE_0 );//数组某一行0~7转化存放G
	for(i=8;i<16;i++) Pixel_Buf[j][i]  = ( (Color.R & (1 << (15-i)))? (CODE_1):CODE_0 );//数组某一行8~15转化存放R
	for(i=16;i<24;i++) Pixel_Buf[j][i] = ( (Color.B & (1 << (23-i)))? (CODE_1):CODE_0 );//数组某一行16~23转化存放B
}
/************************************************************************************************************
** void RGB_Forward(uint32_t Delay,int R,int G,int B)                  				                             **                                                              
** 功能描述：开启跑马灯（一轮）                                                                    		       **
** 参数说明：移动间隔时间（ms），灯带颜色RGB值                          					                             **   
** 参数返回：无                                                                                             **
************************************************************************************************************/
uint8_t Num = 0;
void RGB_Forward(uint32_t Delay,int R,int G,int B)
{
		RGB_Color_TypeDef run[3]={{R/10,G/10,B/10},{R,G,B},{R/10,G/10,B/10}};
		RGB_NewFrame();
		if(Num>=0&&Num<=3)RGB_SetOne_Color((Num)%4,1,run[2]);
		if(Num>=1&&Num<=4)RGB_SetOne_Color((Num-1)%4,1,run[1]);
		if(Num>=2&&Num<=5)RGB_SetOne_Color((Num-2)%4,1,run[0]);
		if(Num>=0&&Num<=3)RGB_SetOne_Color(7-((Num-0)%4),1,run[2]);
		if(Num>=1&&Num<=4)RGB_SetOne_Color(7-((Num-1)%4),1,run[1]);
		if(Num>=2&&Num<=5)RGB_SetOne_Color(7-((Num-2)%4),1,run[0]);
		//RGB_SendArray();
		HAL_Delay(Delay);//速度
		Num++;
		if (Num == 7)
		{
			Num = 0;
		}
}
