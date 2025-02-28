#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__
#include "main.h"


//以下是参数在eeprom中的首地址对照(全部为四个字节)

/************************************************************************/
#define SPEED_P 0x00
#define SPEED_I 0x04
#define SPEED_D 0x08
#define ANGLE_P 0x0C
#define ANGLE_I 0x10
#define ANGLE_D 0x14
#define MOTOR1_SPEED 0x18
#define MOTOR2_SPEED 0x1C
#define MOTOR1_STATUS 0x20
#define MOTOR2_STATUS 0x24
#define RUN_TIME 0x28
#define RGB_LIGHT 0x2C
#define MAX_SPEED 0x30
#define SWERVE_SPEED 0x34
#define LCD_BRIGHTLESS 0x38
#define LCD_B_COLOR 0x3C
#define SLEEP 0x40
#define SLEEP_TIME 0x44
#define AUTO_RUN_MODE 0x48
#define TARGET_POINT 0x4C
/************************************************************************/

void Change_Param(void);
void Back_ground_change(char* name);
void Back_ground_change_back(char* name);
void Change_Brightness(char* name);
void Brightness_change_back(char* name);
void RGB_refresh(char* name);
void EEPROM_OUT(void);
void Success(void);
void Failed(void);
void Enter_Bounce(void);
void Locked(void);
void Unlocked(void);
void EEPROM_Lock(void);
void Cancle(void);
void EEPROM_Erase(void);
void Run_Manual(void);
void Run_Auto(void);

extern uint8_t Run_flag;

#endif
