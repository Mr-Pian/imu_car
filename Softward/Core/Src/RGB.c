#include "RGB.h"
#include "tim.h"

/*Some Static Colors------------------------------*/
const RGB_Color_TypeDef RED      = {255,0,0};   //��ʾ��ɫRGB����
const RGB_Color_TypeDef GREEN    = {0,255,0};
const RGB_Color_TypeDef BLUE     = {0,0,255};
const RGB_Color_TypeDef SKY      = {0,255,255};
const RGB_Color_TypeDef MAGENTA  = {254,0,220};
const RGB_Color_TypeDef YELLOW   = {127,216,0};
const RGB_Color_TypeDef ORANGE   = {126,106,0};
const RGB_Color_TypeDef OFF    = {0,0,0};
const RGB_Color_TypeDef WHITE    = {254,254,254}; 
const RGB_Color_TypeDef LIGHTGREEN   = {27,216,0};

/*��ά����������PWM������飬ÿһ��24��
���ݴ���һ��LED�����һ��24��0����RESET��*/
uint32_t Pixel_Buf[Pixel_NUM][24];       
/*
���ܣ��趨����RGB LED����ɫ���ѽṹ����RGB��24BITת��Ϊ0���1��
������LedIdΪLED��ţ�Color���������ɫ�ṹ��



���ܣ����һ��װ��24��0�����24������ռ�ձ�Ϊ0��PWM������Ϊ���reset��ʱ��������ʱ��Ϊ24*1.2=30us > 24us(Ҫ�����24us)
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
���ܣ���������
������(&htim2)��ʱ��2��(TIM_CHANNEL_4)ͨ��4��((uint32_t *)Pixel_Buf)���������飬
			(Pixel_NUM+1)*24)���͸����������������
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

	for(i=0;i<8;i++) Pixel_Buf[j][i]   = ( (Color.G & (1 << (7 -i)))? (CODE_1):CODE_0 );//����ĳһ��0~7ת�����G
	for(i=8;i<16;i++) Pixel_Buf[j][i]  = ( (Color.R & (1 << (15-i)))? (CODE_1):CODE_0 );//����ĳһ��8~15ת�����R
	for(i=16;i<24;i++) Pixel_Buf[j][i] = ( (Color.B & (1 << (23-i)))? (CODE_1):CODE_0 );//����ĳһ��16~23ת�����B
}
/************************************************************************************************************
** void RGB_Forward(uint32_t Delay,int R,int G,int B)                  				                             **                                                              
** ������������������ƣ�һ�֣�                                                                    		       **
** ����˵�����ƶ����ʱ�䣨ms�����ƴ���ɫRGBֵ                          					                             **   
** �������أ���                                                                                             **
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
		HAL_Delay(Delay);//�ٶ�
		Num++;
		if (Num == 7)
		{
			Num = 0;
		}
}
