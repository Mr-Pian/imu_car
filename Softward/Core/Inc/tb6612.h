#ifndef __TB6612_H__
#define __TB6612_H__
#include "main.h"

#define R 1;
#define L 0;
#define Both 2;

#define Foward 1;
#define Backward 0;

int Motor_Start(uint8_t L_R);
int Motor_SetSpeed(uint8_t Mode,float speed, uint8_t L_R);
int Motor_Off(uint8_t L_R);

#endif

