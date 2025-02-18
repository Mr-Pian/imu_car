#ifndef __M24_EEPROM_H_
#define __M24_EEPROM_H_
#include "main.h"

HAL_StatusTypeDef EEPROM_ReadByte(uint16_t MemAddress,uint8_t *data);
HAL_StatusTypeDef EEPROM_WriteByte(uint16_t MemAddress,uint8_t data);

HAL_StatusTypeDef EEPROM_ReadMultipleBytes(uint16_t MemAddress, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef EEPROM_WriteMultipleBytes(uint16_t MemAddress, uint8_t *pData, uint16_t Size);

uint8_t M24C02_Check(void);

uint8_t Only_Read_Mode_1(void);
uint8_t Only_Read_Mode_0(void);
		
void Erase_Full_Chip(void);

#endif 
