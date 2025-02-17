/**********************ICM42688 by ABO**************/

#include "ICM42688.h"
#include "spi.h"
#include "stdio.h"
icm42688_st icm42688_data = {0,0,0,0,0,0}; 

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     icm42688 写寄存器
// 参数说明     reg             寄存器地址
// 参数说明     data            数据
// 返回参数     void
// 使用示例     icm42688_write_register(icm42688_PWR_MGMT_1, 0x80);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static void icm42688_write_register(uint8_t reg, uint8_t data)
{
	uint8_t reg_w = reg | icm42688_SPI_W;
  icm42688_CS(0);//片选选中
	HAL_SPI_Transmit(&hspi1,&reg_w,1,0x1000);//寄存器写命令
	HAL_SPI_Transmit(&hspi1,&data,1,0x1000);//写入数据
  icm42688_CS(1);//片选释放
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     icm42688 读寄存器
// 参数说明     reg             寄存器地址
// 返回参数     uint8           数据
// 使用示例     icm42688_read_register(icm42688_WHO_AM_I);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static uint8_t icm42688_read_register(uint8_t reg)
{
    uint8_t data = 0;
		uint8_t reg_r = reg | icm42688_SPI_R;
    icm42688_CS(0);//片选选中
		HAL_SPI_Transmit(&hspi1,&reg_r,1,0x1000);//寄存器读命令
		HAL_SPI_Receive(&hspi1,&data,1,0x1000);//接收数据
    icm42688_CS(1);//片选释放
    return data;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     icm42688 读数据
// 参数说明     reg             寄存器地址
// 参数说明     data            数据缓冲区
// 参数说明     len             数据长度
// 返回参数     void
// 使用示例     icm42688_read_registers(icm42688_ACCEL_XOUT_H, dat, 6);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static void icm42688_read_registers(uint8_t reg, uint8_t *data, uint32_t len)
{
		uint8_t reg_r = reg | icm42688_SPI_R;
    icm42688_CS(0);//片选选中
		HAL_SPI_Transmit(&hspi1,&reg_r,1,0x1000);//寄存器读命令
		HAL_SPI_Receive(&hspi1,data,len,0x1000);//接收len字节数据
	
    icm42688_CS(1);//片选释放
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     icm42688 自检
// 参数说明     void
// 返回参数     uint8           1-自检失败 0-自检成功
// 使用示例     icm42688_self_check();
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static uint8_t icm42688_self_check (void)
{
    uint8_t dat = 0, return_state = 0;
    uint16_t timeout_count = 0;

    while(icm42688_ID != dat)                                                          // 判断 ID 是否正确
    {
        if(timeout_count ++ > icm42688_TIMEOUT_COUNT)//自检失败，超时退出
        {
            return_state =  1;
            break;
        }
        dat = icm42688_read_register(icm42688_WHO_AM_I);
        HAL_Delay(10);
    }
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 icm42688 温度计数据
// 参数说明     void
// 返回参数     void
// 使用示例     icm42688_get_temp();                                             // 执行该函数后，直接查看对应的变量即可
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void icm42688_get_temp(void)
{
    uint8_t dat[6];
    icm42688_read_registers(icm42688_TEMP_DATA1, dat, 2);
    icm42688_data.temp = (int16_t)(((uint16_t)dat[0] << 8 | dat[1]));
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 icm42688 加速度计数据
// 参数说明     void
// 返回参数     void
// 使用示例     icm42688_get_acc();                                             // 执行该函数后，直接查看对应的变量即可
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void icm42688_get_acc (void)
{
    uint8_t dat[6];
    icm42688_read_registers(icm42688_ACCEL_DATA_X1, dat, 6);
    icm42688_data.acc_x = (int16_t)(((uint16_t)dat[0] << 8 | dat[1]));
    icm42688_data.acc_y = (int16_t)(((uint16_t)dat[2] << 8 | dat[3]));
    icm42688_data.acc_z = (int16_t)(((uint16_t)dat[4] << 8 | dat[5]));
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取icm42688陀螺仪数据
// 参数说明     void
// 返回参数     void
// 使用示例     icm42688_get_gyro();                                            // 执行该函数后，直接查看对应的变量即可
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void icm42688_get_gyro (void)
{
    uint8_t dat[6];

    icm42688_read_registers(icm42688_GYRO_DATA_X1, dat, 6);
    icm42688_data.gyro_x = (int16_t)(((uint16_t)dat[0] << 8 | dat[1]));
    icm42688_data.gyro_y = (int16_t)(((uint16_t)dat[2] << 8 | dat[3]));
    icm42688_data.gyro_z = (int16_t)(((uint16_t)dat[4] << 8 | dat[5]));
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     将 icm42688 温度传感器数据转换为实际物理数据
// 参数说明     temp_value      任意轴的加速度计数据
// 返回参数     void
// 使用示例     temp_data = icm42688_temp_transition(icm42688_temp);           // 单位为 g(m/s^2)
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
float icm42688_temp_transition (int16_t temp_value)
{
  float temp_data = 0;
	
	temp_data = (float)temp_value / 132.48f + 25;   // 除以132.48，再加25，单位为 °C
	
  return temp_data;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     将 icm42688 加速度计数据转换为实际物理数据
// 参数说明     acc_value      任意轴的加速度计数据
// 返回参数     void
// 使用示例     float data = icm42688_acc_transition(icm42688_acc_x);           // 单位为 g(m/s^2)
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
float icm42688_acc_transition (int16_t acc_value)
{
	float acc_data = 0;
	switch(icm42688_ACCEL_FS_SEL)
	{
			case 0x00: acc_data = (float)acc_value / 2048;   break;// 设置为:0x00 加速度计量程为:±16g         获取到的加速度计数据 除以2048       可以转化为带物理单位的数据，单位：g(m/s^2)
			case 0x20: acc_data = (float)acc_value / 4096;   break;// 设置为:0x20 加速度计量程为:±8g          获取到的加速度计数据 除以4096       可以转化为带物理单位的数据，单位：g(m/s^2)
			case 0x40: acc_data = (float)acc_value / 8192;   break;// 设置为:0x40 加速度计量程为:±4g          获取到的加速度计数据 除以8192       可以转化为带物理单位的数据，单位：g(m/s^2)
			case 0x60: acc_data = (float)acc_value / 16384;  break;// 设置为:0x60 加速度计量程为:±2g          获取到的加速度计数据 除以16384      可以转化为带物理单位的数据，单位：g(m/s^2)
			default: break;
	}
  return acc_data;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     将 icm42688 陀螺仪数据转换为实际物理数据
// 参数说明     gyro_value      任意轴的陀螺仪数据
// 返回参数     void
// 使用示例     float data = icm42688_gyro_transition(icm42688_gyro_x);         // 单位为°/s
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
float icm42688_gyro_transition (int16_t gyro_value)
{
	float gyro_data = 0;
	switch(icm42688_GYRO_FS_SEL)
	{
			case 0x00: gyro_data = (float)gyro_value / 16.4f;    break;// 设置为:0x00 陀螺仪量程为:±2000  dps     获取到的陀螺仪数据除以16.4          可以转化为带物理单位的数据，单位为：°/s
			case 0x20: gyro_data = (float)gyro_value / 32.8f;    break;// 设置为:0x20 陀螺仪量程为:±1000  dps     获取到的陀螺仪数据除以32.8          可以转化为带物理单位的数据，单位为：°/s
			case 0x40: gyro_data = (float)gyro_value / 65.5f;    break;// 设置为:0x40 陀螺仪量程为:±500   dps     获取到的陀螺仪数据除以65.5          可以转化为带物理单位的数据，单位为：°/s
			case 0x60: gyro_data = (float)gyro_value / 131.0f;   break;// 设置为:0x60 陀螺仪量程为:±250   dps     获取到的陀螺仪数据除以131           可以转化为带物理单位的数据，单位为：°/s
			case 0x80: gyro_data = (float)gyro_value / 262.0f;   break;// 设置为:0x80 陀螺仪量程为:±125   dps     获取到的陀螺仪数据除以262           可以转化为带物理单位的数据，单位为：°/s
			case 0xA0: gyro_data = (float)gyro_value / 524.3f;   break;// 设置为:0xA0 陀螺仪量程为:±62.5  dps     获取到的陀螺仪数据除以524.3         可以转化为带物理单位的数据，单位为：°/s
			case 0xC0: gyro_data = (float)gyro_value / 1048.6f;  break;// 设置为:0xC0 陀螺仪量程为:±31.25 dps     获取到的陀螺仪数据除以1048.6        可以转化为带物理单位的数据，单位为：°/s
			case 0xE0: gyro_data = (float)gyro_value / 2097.2f;  break;// 设置为:0xE0 陀螺仪量程为:±15.625dps     获取到的陀螺仪数据除以2097.2        可以转化为带物理单位的数据，单位为：°/s
			default: break;
	}
  return gyro_data;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     初始化 icm42688
// 参数说明     void
// 返回参数     uint8           1-初始化失败 0-初始化成功
// 使用示例     icm42688_init();
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8_t icm42688_init (void)
{
    uint8_t val = 0x0, return_state = 0;
    uint16_t timeout_count = 0;

    HAL_Delay(10);                                                      // 上电延时

		if(icm42688_self_check())
		{
				printf("imu_selfcheck_err\n");
				return_state = 1;//陀螺仪自检失败
				return return_state;
		}
		icm42688_write_register(icm42688_REG_BANK_SEL,icm42688_Bank_0);		//设置bank 0区域寄存器
		icm42688_write_register(icm42688_DEVICE_CONFIG, icm42688_SOFT_RESET_CONFIG);      // 复位设备
		HAL_Delay(10); //After writing 1 to this SOFT_RESET_CONFIG, wait 10ms for soft reset to be effective, before attempting any other register access

		do
		{                                                                       // 等待复位成功
			val = icm42688_read_register(icm42688_DEVICE_CONFIG);
			if(timeout_count ++ > icm42688_TIMEOUT_COUNT)
			{
				printf("imu_overtime\n");
				return_state = 1;//超时错误
				return return_state;
			}
		}while(0x00 != val);//DEVICE_CONFIG Reset value: 0x00
	
		icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_1); //设置bank 1区域寄存器
		icm42688_write_register(icm42688_INTF_CONFIG4, 0x02); //设置为4线SPI通信
	
		icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_0); //设置bank 0区域寄存器
		icm42688_write_register(icm42688_INTF_CONFIG0, 0xB0);
	
		icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_0); //设置bank 0区域寄存器
        icm42688_write_register(icm42688_FIFO_CONFIG, 0x40); //Stream-to-FIFO Mode(page61)
	
	//开启加速度计、陀螺仪的FIFO
		val = icm42688_read_register(icm42688_INT_SOURCE0);
        icm42688_write_register(icm42688_INT_SOURCE0, icm42688_Bank_0);
        icm42688_write_register(icm42688_FIFO_CONFIG2, 0x00); // watermark
        icm42688_write_register(icm42688_FIFO_CONFIG3, 0x02); // watermark
        icm42688_write_register(icm42688_INT_SOURCE0, val);
        icm42688_write_register(icm42688_FIFO_CONFIG1, 0x47); // Enable the accel and gyro to the FIFO
	
		icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_0); //设置bank 0区域寄存器
        icm42688_write_register(icm42688_INT_CONFIG, 0x36);

		icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_0); //设置bank 0区域寄存器
		val = (icm42688_read_register(icm42688_INT_SOURCE0)|FIFO_THS_INT1_EN);
        icm42688_write_register(icm42688_INT_SOURCE0, val);
	
	// 设置加速度计量程
			icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_0); //设置bank 0区域寄存器
	val = ((icm42688_read_register(icm42688_ACCEL_CONFIG0)&0x1F)|(icm42688_ACCEL_FS_SEL));//+-4g  //page74
			icm42688_write_register(icm42688_ACCEL_CONFIG0, val);
	
	// 设置加速度计输出数据速率
	icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_0); //设置bank 0区域寄存器
	val = ((icm42688_read_register(icm42688_ACCEL_CONFIG0)&0xF0)|(icm42688_ACCEL_ODR));//1kHz  //page74
			icm42688_write_register(icm42688_ACCEL_CONFIG0, val);
	
	// 设置陀螺仪量程
	icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_0); //设置bank 0区域寄存器
	val = ((icm42688_read_register(icm42688_GYRO_CONFIG0)&0x1F)|(icm42688_GYRO_FS_SEL));
	icm42688_write_register(icm42688_GYRO_CONFIG0,val);
	
	// 设置陀螺仪输出数据速率
	icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_0); //设置bank 0区域寄存器  
	val = ((icm42688_read_register(icm42688_GYRO_CONFIG0)&0xF0)|icm42688_GYRO_ODR);
	icm42688_write_register(icm42688_GYRO_CONFIG0, val); 
	
		// 设置陀螺仪低通滤波器
		icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_0); //设置bank 0区域寄存器  
		val = ((icm42688_read_register(icm42688_GYRO_ACCEL_CONFIG0)&0xF0)|0x07);
		icm42688_write_register(icm42688_GYRO_ACCEL_CONFIG0, val); 
	
		// 设置陀螺仪陷波滤波器 滤除谐波，还要单独配置每个轴
		icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_1); //设置bank 0区域寄存器  
		val = ((icm42688_read_register(icm42688_GYRO_CONFIG_STATIC10)&0x8F)|0x00);
		icm42688_write_register(icm42688_GYRO_CONFIG_STATIC10, val);
		
		val = ((icm42688_read_register(icm42688_GYRO_CONFIG_STATIC9)&0x8F)|0x00);
		icm42688_write_register(icm42688_GYRO_CONFIG_STATIC9, val);
	
	
	// 设置加速度计模式
	icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_0); //设置bank 0区域寄存器  
	val = icm42688_read_register(icm42688_PWR_MGMT0)|(icm42688_ACCEL_MODE); // Accel on in LNM //读取PWR—MGMT0当前寄存器的值(page72)
	icm42688_write_register(icm42688_PWR_MGMT0, val);  
	HAL_Delay(10);  
	
	// 设置陀螺仪模式
	icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_0); //设置bank 0区域寄存器  
	val = icm42688_read_register(icm42688_PWR_MGMT0)|(icm42688_GYRO_MODE); // Gyro on in LN
	icm42688_write_register(icm42688_PWR_MGMT0, val);  //操作完PWR—MGMT0寄存器后 200us内不能有任何读写寄存器的操作
	HAL_Delay(10);  

    return return_state;
}
