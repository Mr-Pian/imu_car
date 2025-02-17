#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "main.h"


#define __HW_SPI//使用硬件spi的时候就有把这取消注释

#ifdef __HW_SPI

#include "spi.h"

#define usr_lcd_spi hspi3

#endif




#define USE_HORIZONTAL 2  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏



#define LCD_W 240
#define LCD_H 240

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t



//-----------------LCD端口定义---------------- 

#ifndef __HW_SPI

#define LCD_SCLK_Clr() HAL_GPIO_WritePin(ST7789_SCL_GPIO_Port,ST7789_SCL_Pin,GPIO_PIN_RESET)//SCL=SCLK

#define LCD_SCLK_Set() HAL_GPIO_WritePin(ST7789_SCL_GPIO_Port,ST7789_SCL_Pin,GPIO_PIN_SET)

#define LCD_MOSI_Clr() HAL_GPIO_WritePin(ST7789_SDA_GPIO_Port,ST7789_SDA_Pin,GPIO_PIN_RESET)//SDA=MOSI
#define LCD_MOSI_Set() HAL_GPIO_WritePin(ST7789_SDA_GPIO_Port,ST7789_SDA_Pin,GPIO_PIN_SET)

#endif



//#define LCD_RES_Clr()  HAL_GPIO_WritePin(ST7789_RES_GPIO_Port,ST7789_RES_Pin,GPIO_PIN_RESET)//RES
//#define LCD_RES_Set()  HAL_GPIO_WritePin(ST7789_RES_GPIO_Port,ST7789_RES_Pin,GPIO_PIN_SET)

#define LCD_DC_Clr()    HAL_GPIO_WritePin(ST7789_DC_GPIO_Port,ST7789_DC_Pin,GPIO_PIN_RESET)//DC
#define LCD_DC_Set()    HAL_GPIO_WritePin(ST7789_DC_GPIO_Port,ST7789_DC_Pin,GPIO_PIN_SET)
 		     
#define LCD_CS_Clr()    HAL_GPIO_WritePin(ST7789_CS_GPIO_Port,ST7789_CS_Pin,GPIO_PIN_RESET)//CS
#define LCD_CS_Set()  HAL_GPIO_WritePin(ST7789_CS_GPIO_Port,ST7789_CS_Pin,GPIO_PIN_SET)

#define LCD_BLK_Clr() HAL_GPIO_WritePin(ST7789_BLK_GPIO_Port,ST7789_BLK_Pin,GPIO_PIN_RESET)//BLK
#define LCD_BLK_Set()  HAL_GPIO_WritePin(ST7789_BLK_GPIO_Port,ST7789_BLK_Pin,GPIO_PIN_SET)






void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(u8 dat);//模拟SPI时序
void LCD_WR_DATA8(u8 dat);//写入一个字节
void LCD_WR_DATA(u16 dat);//写入两个字节
void LCD_WR_REG(u8 dat);//写入一个指令
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//设置坐标函数
void LCD_Init(void);//LCD初始化
#endif




