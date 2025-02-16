#ifndef __RGB_H__
#define __RGB_H__
#include "main.h"

#define CODE_1       (70)       
#define CODE_0       (35)       


typedef struct
{
	uint8_t R;
	uint8_t G;
	uint8_t B;
}RGB_Color_TypeDef;
 
extern const RGB_Color_TypeDef RED  ;   
extern const RGB_Color_TypeDef GREEN   ;  
extern const RGB_Color_TypeDef BLUE    ;   
extern const RGB_Color_TypeDef OFF   ;
extern const RGB_Color_TypeDef SKY     ;
extern const RGB_Color_TypeDef MAGENTA;
extern const RGB_Color_TypeDef YELLOW;
extern const RGB_Color_TypeDef ORANGE;
extern const RGB_Color_TypeDef WHITE;
extern const RGB_Color_TypeDef LIGHTGREEN;
RGB_Color_TypeDef tiaocepan(uint8_t x);

extern uint8_t blesendbuffer[64][3];
extern uint32_t Pixel_Buf[8][24];
extern uint8_t colortpye;
extern int mpu_cont;
#define Pixel_NUM 8
void RGB_SendArray(void); 
void RGB_NewFrame(void);
void Reset_Load(void);
void  RGB_SetOne_Color(uint8_t x,uint8_t y,RGB_Color_TypeDef Color);
void RGB_Forward(uint32_t Delay,int R,int G,int B);
#endif
