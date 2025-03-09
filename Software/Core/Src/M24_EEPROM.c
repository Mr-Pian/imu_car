#include "M24_EEPROM.h"
#include "i2c.h"
#include "gpio.h"
#include "Delay.h"

#define M24C02_ADDR        (0xA0)

//eeprom read n bytes
HAL_StatusTypeDef EEPROM_ReadMultipleBytes(uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
    if (HAL_I2C_Mem_Read(&hi2c3, M24C02_ADDR | 0x01, MemAddress, I2C_MEMADD_SIZE_8BIT, pData, Size, 1000) == HAL_OK)
    {
        return HAL_OK;
    }
    else
    {
        return HAL_ERROR;
    }
}

//eeprom write n bytes
HAL_StatusTypeDef EEPROM_WriteMultipleBytes(uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
    for(int i=0; i<Size; i++){
        if(EEPROM_WriteByte(MemAddress+i, pData[i]) != HAL_OK){
            return HAL_ERROR;
        }
    }
		return HAL_OK;
}

//EEPROM read
HAL_StatusTypeDef EEPROM_ReadByte(uint16_t MemAddress,uint8_t *data)
{
    if (HAL_I2C_Mem_Read(&hi2c3, M24C02_ADDR | 0x01, MemAddress, I2C_MEMADD_SIZE_8BIT, data, 1, 1000) == HAL_OK)
    {
        return HAL_OK;
    }
    else
    {
        return HAL_ERROR;
    }
}

//EEPROM write byte
HAL_StatusTypeDef EEPROM_WriteByte(uint16_t MemAddress,uint8_t data)
{
    if (HAL_I2C_Mem_Write(&hi2c3, M24C02_ADDR, MemAddress, I2C_MEMADD_SIZE_8BIT,&data,1, 1000) == HAL_OK)
    {
        //write cycle :5ms
        Delay_ms(5);
        return HAL_OK;
    }
    else
    {
        return HAL_ERROR;
    }
}

/************************************************************************************************************
** uint8_t AT24CXX_Check(void)                  				                                                   **                                                              
** 功能描述：初始化检测eeprom                                                                        		     **
** 参数说明：无                          					                                                         **   
** 参数返回：uint8_t                                                                                        **
************************************************************************************************************/

uint8_t M24C02_Check(void)
{
	
	uint8_t WC_Flag = 0x00;
	EEPROM_ReadByte(0xFE, &WC_Flag);  //读只读标志位
	if (WC_Flag == 0x12)
	{
		HAL_GPIO_WritePin(WC_GPIO_Port, WC_Pin, GPIO_PIN_SET);  //WC脚写为高
	}
	
	uint8_t temp = 0x00;
	EEPROM_ReadByte(0xFF, &temp);//避免每次开机都写		   
	if(temp==0X55)
	{
		return 0;		   
	}
	else if(WC_Flag != 0x12)//排除第一次初始化的情况和只读的情况
	{
		uint8_t temp_2 = 0x55;
		EEPROM_WriteByte(0xFF, temp_2);
	  EEPROM_ReadByte(0xFF, &temp);
		if(temp==0X55)
		{
			return 0;
		}	
		
	}
	return 1;											  
}

/************************************************************************************************************
** uint8_t Only_Read_Mode_1(void)                  				                                                 **                                                              
** 功能描述：让eeprom进入只读模式                                                                        		 **
** 参数说明：无                          					                                                         **   
** 参数返回：uint8_t                                                                                        **
************************************************************************************************************/

uint8_t Only_Read_Mode_1(void)
{
	uint8_t temp = 0x12;
	uint8_t temp_2 = 0x00;
	EEPROM_WriteByte(0xFE, temp);  //在标志位0xFE写入标志写锁定的字节0x12
	EEPROM_ReadByte(0xFE, &temp_2);
	if (temp_2 == 0x12)
	{	
		HAL_GPIO_WritePin(WC_GPIO_Port, WC_Pin, GPIO_PIN_SET);  //WC脚写为高
		return 0;
	}
	else
	{
		return 1;
	}
}

/************************************************************************************************************
** uint8_t Only_Read_Mode_0(void)                  				                                                 **                                                              
** 功能描述：让eeprom退出只读模式                                                                        		 **
** 参数说明：无                          					                                                         **   
** 参数返回：uint8_t                                                                                        **
************************************************************************************************************/

uint8_t Only_Read_Mode_0(void)
{
	uint8_t temp = 0x00;
	uint8_t temp_2 = 0xFF;
	HAL_GPIO_WritePin(WC_GPIO_Port, WC_Pin, GPIO_PIN_RESET);  //WC脚写为低
	EEPROM_WriteByte(0xFE, temp);  //在标志位0xFE清除标志写锁定的字节0x00
	EEPROM_ReadByte(0xFE, &temp_2);
	if (temp_2 == 0x00)
	{	
		return 0;
	}
	else
	{
		return 1;
	}
}

/************************************************************************************************************
** uint8_t Erase_Full_Chip(void)                  				                                                 **                                                              
** 功能描述：强制删除全片数据,只读模式对这个无效,慎用                                                          **
** 参数说明：无                          					                                                          **   
** 参数返回：void                                                                                            **
************************************************************************************************************/

void Erase_Full_Chip(void)
{
	HAL_GPIO_WritePin(WC_GPIO_Port, WC_Pin, GPIO_PIN_RESET);  //WC脚写为低
	uint8_t temp = 0x00;  //填充字节
	for (int i = 0x00; i <= 0xFF; i++)
	{
		EEPROM_WriteByte(i, temp);
	}
}
