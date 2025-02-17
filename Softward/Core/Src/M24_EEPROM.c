#include "M24_EEPROM.h"
#include "i2c.h"

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
        HAL_Delay(5);
        return HAL_OK;
    }
    else
    {
        return HAL_ERROR;
    }
}
