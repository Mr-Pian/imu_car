#ifndef _icm42688_h_
#define _icm42688_h_

#include "main.h"

//====================================================硬件 SPI 驱动(STM32 HAL库硬件SPI)====================================================                                          
#define icm42688_CS(x)              ((x) ? (HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET)) : (HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET)))
#define icm42688_TIMEOUT_COUNT      (0x00FF)                                    // icm42688 超时计数
//================================================定义 icm42688 内部地址================================================
//#define icm42688_DEV_ADDR           (0x69)                                      // SA0接地：0x68 SA0上拉：0x69 模块默认上拉
#define icm42688_SPI_W              (0x00)
#define icm42688_SPI_R              (0x80)
#define icm42688_ID	             	(0x47)//WHO AM I的值


typedef struct 
{
	int16_t gyro_x;
	int16_t gyro_y;
	int16_t gyro_z;
	int16_t acc_x;
	int16_t acc_y;
	int16_t acc_z;
	int16_t temp;
}icm42688_st;


/************ICM42688寄存器地址****************/
// Bank 0
#define icm42688_DEVICE_CONFIG             0x11
#define icm42688_DRIVE_CONFIG              0x13
#define icm42688_INT_CONFIG                0x14
#define icm42688_FIFO_CONFIG               0x16
#define icm42688_TEMP_DATA1                0x1D
#define icm42688_TEMP_DATA0                0x1E
#define icm42688_ACCEL_DATA_X1             0x1F
#define icm42688_ACCEL_DATA_X0             0x20
#define icm42688_ACCEL_DATA_Y1             0x21
#define icm42688_ACCEL_DATA_Y0             0x22
#define icm42688_ACCEL_DATA_Z1             0x23
#define icm42688_ACCEL_DATA_Z0             0x24
#define icm42688_GYRO_DATA_X1              0x25
#define icm42688_GYRO_DATA_X0              0x26
#define icm42688_GYRO_DATA_Y1              0x27
#define icm42688_GYRO_DATA_Y0              0x28
#define icm42688_GYRO_DATA_Z1              0x29
#define icm42688_GYRO_DATA_Z0              0x2A
#define icm42688_TMST_FSYNCH               0x2B
#define icm42688_TMST_FSYNCL               0x2C
#define icm42688_INT_STATUS                0x2D
#define icm42688_FIFO_COUNTH               0x2E
#define icm42688_FIFO_COUNTL               0x2F
#define icm42688_FIFO_DATA                 0x30
#define icm42688_APEX_DATA0                0x31
#define icm42688_APEX_DATA1                0x32
#define icm42688_APEX_DATA2                0x33
#define icm42688_APEX_DATA3                0x34
#define icm42688_APEX_DATA4                0x35
#define icm42688_APEX_DATA5                0x36
#define icm42688_INT_STATUS2               0x37
#define icm42688_INT_STATUS3               0x38
#define icm42688_SIGNAL_PATH_RESET         0x4B
#define icm42688_INTF_CONFIG0              0x4C
#define icm42688_INTF_CONFIG1              0x4D
#define icm42688_PWR_MGMT0                 0x4E
#define icm42688_GYRO_CONFIG0              0x4F
#define icm42688_ACCEL_CONFIG0             0x50
#define icm42688_GYRO_CONFIG1              0x51
#define icm42688_GYRO_ACCEL_CONFIG0        0x52
#define icm42688_ACCEL_CONFIG1             0x53
#define icm42688_TMST_CONFIG               0x54
#define icm42688_APEX_CONFIG0              0x56
#define icm42688_SMD_CONFIG                0x57
#define icm42688_FIFO_CONFIG1              0x5F
#define icm42688_FIFO_CONFIG2              0x60
#define icm42688_FIFO_CONFIG3              0x61
#define icm42688_FSYNC_CONFIG              0x62
#define icm42688_INT_CONFIG0               0x63
#define icm42688_INT_CONFIG1               0x64
#define icm42688_INT_SOURCE0               0x65
#define icm42688_INT_SOURCE1               0x66
#define icm42688_INT_SOURCE3               0x68
#define icm42688_INT_SOURCE4               0x69
#define icm42688_FIFO_LOST_PKT0            0x6C
#define icm42688_FIFO_LOST_PKT1            0x6D
#define icm42688_SELF_TEST_CONFIG          0x70
#define icm42688_WHO_AM_I                  0x75
#define icm42688_REG_BANK_SEL              0x76

// Bank 1
#define icm42688_SENSOR_CONFIG0            0x03
#define icm42688_GYRO_CONFIG_STATIC2       0x0B
#define icm42688_GYRO_CONFIG_STATIC3       0x0C
#define icm42688_GYRO_CONFIG_STATIC4       0x0D
#define icm42688_GYRO_CONFIG_STATIC5       0x0E
#define icm42688_GYRO_CONFIG_STATIC6       0x0F
#define icm42688_GYRO_CONFIG_STATIC7       0x10
#define icm42688_GYRO_CONFIG_STATIC8       0x11
#define icm42688_GYRO_CONFIG_STATIC9       0x12
#define icm42688_GYRO_CONFIG_STATIC10      0x13
#define icm42688_XG_ST_DATA                0x5F
#define icm42688_YG_ST_DATA                0x60
#define icm42688_ZG_ST_DATA                0x61
#define icm42688_TMSTVAL0                  0x62
#define icm42688_TMSTVAL1                  0x63
#define icm42688_TMSTVAL2                  0x64
#define icm42688_INTF_CONFIG4              0x7A
#define icm42688_INTF_CONFIG5              0x7B
#define icm42688_INTF_CONFIG6              0x7C

// Bank 2
#define icm42688_ACCEL_CONFIG_STATIC2      0x03
#define icm42688_ACCEL_CONFIG_STATIC3      0x04
#define icm42688_ACCEL_CONFIG_STATIC4      0x05
#define icm42688_XA_ST_DATA                0x3B
#define icm42688_YA_ST_DATA                0x3C
#define icm42688_ZA_ST_DATA                0x3D

// Bank 4
#define icm42688_GYRO_ON_OFF_CONFIG        0x0E
#define icm42688_APEX_CONFIG1              0x40
#define icm42688_APEX_CONFIG2              0x41
#define icm42688_APEX_CONFIG3              0x42
#define icm42688_APEX_CONFIG4              0x43
#define icm42688_APEX_CONFIG5              0x44
#define icm42688_APEX_CONFIG6              0x45
#define icm42688_APEX_CONFIG7              0x46
#define icm42688_APEX_CONFIG8              0x47
#define icm42688_APEX_CONFIG9              0x48
#define icm42688_ACCEL_WOM_X_THR           0x4A
#define icm42688_ACCEL_WOM_Y_THR           0x4B
#define icm42688_ACCEL_WOM_Z_THR           0x4C
#define icm42688_INT_SOURCE6               0x4D
#define icm42688_INT_SOURCE7               0x4E
#define icm42688_INT_SOURCE8               0x4F
#define icm42688_INT_SOURCE9               0x50
#define icm42688_INT_SOURCE10              0x51
#define icm42688_OFFSET_USER0              0x77
#define icm42688_OFFSET_USER1              0x78
#define icm42688_OFFSET_USER2              0x79
#define icm42688_OFFSET_USER3              0x7A
#define icm42688_OFFSET_USER4              0x7B
#define icm42688_OFFSET_USER5              0x7C
#define icm42688_OFFSET_USER6              0x7D
#define icm42688_OFFSET_USER7              0x7E
#define icm42688_OFFSET_USER8              0x7F

/** 寄存器赋值宏定义 **/
#define icm42688_SOFT_RESET_CONFIG	(0x01)

#define icm42688_Bank_0				(0x00)
#define icm42688_Bank_1				(0x01)
#define icm42688_Bank_2				(0x02)
#define icm42688_Bank_3				(0x03)
#define icm42688_Bank_4				(0x04)

#define FIFO_THS_INT1_EN			(0x04)//FIFO threshold interrupt

#define icm42688_ACCEL_FS_SEL         (0x40)                     // 加速度计量程
// 设置为:0x00 加速度计量程为:±16g         获取到的加速度计数据 除以2048       可以转化为带物理单位的数据，单位：g(m/s^2)
// 设置为:0x20 加速度计量程为:±8g          获取到的加速度计数据 除以4096       可以转化为带物理单位的数据，单位：g(m/s^2)
// 设置为:0x40 加速度计量程为:±4g          获取到的加速度计数据 除以8192       可以转化为带物理单位的数据，单位：g(m/s^2)
// 设置为:0x60 加速度计量程为:±2g          获取到的加速度计数据 除以16384      可以转化为带物理单位的数据，单位：g(m/s^2)


#define icm42688_ACCEL_ODR         	  (0x06)                     // 加速度计输出数据速率
// 设置为:0x03 加速度计输出数据速率为:8kHz (LN mode)
// 设置为:0x04 加速度计输出数据速率为:4kHz (LN mode)
// 设置为:0x05 加速度计输出数据速率为:2kHz (LN mode)
// 设置为:0x06 加速度计输出数据速率为:1kHz (LN mode) (default)
// 设置为:0x07 加速度计输出数据速率为:200Hz (LP or LN mode)
// 设置为:0x08 加速度计输出数据速率为:100Hz (LP or LN mode)
// 设置为:0x09 加速度计输出数据速率为:50Hz (LP or LN mode)
// 设置为:0x0A 加速度计输出数据速率为:25Hz (LP or LN mode)
// 设置为:0x0B 加速度计输出数据速率为:12.5Hz (LP or LN mode)
// 设置为:0x0C 加速度计输出数据速率为:6.25Hz (LP mode)
// 设置为:0x0D 加速度计输出数据速率为:3.125Hz (LP mode)
// 设置为:0x0E 加速度计输出数据速率为:1.5625Hz (LP mode)
// 设置为:0x0F 加速度计输出数据速率为:500Hz (LP or LN mode)

#define icm42688_ACCEL_MODE          (0x03)                      //加速度计模式
// 设置为:0x00 加速度计模式为:Turns accelerometer off (default)
// 设置为:0x01 加速度计模式为:Turns accelerometer off
// 设置为:0x02 加速度计模式为:Places accelerometer in Low Power (LP) Mode
// 设置为:0x03 加速度计模式为:Places accelerometer in Low Noise (LN) Mode

#define icm42688_GYRO_FS_SEL         (0x20)                      // 陀螺仪量程
// 设置为:0x00 陀螺仪量程为:±2000  dps     获取到的陀螺仪数据除以16.4          可以转化为带物理单位的数据，单位为：°/s
// 设置为:0x20 陀螺仪量程为:±1000  dps     获取到的陀螺仪数据除以32.8          可以转化为带物理单位的数据，单位为：°/s
// 设置为:0x40 陀螺仪量程为:±500   dps     获取到的陀螺仪数据除以65.5          可以转化为带物理单位的数据，单位为：°/s
// 设置为:0x60 陀螺仪量程为:±250   dps     获取到的陀螺仪数据除以131           可以转化为带物理单位的数据，单位为：°/s
// 设置为:0x80 陀螺仪量程为:±125   dps     获取到的陀螺仪数据除以262           可以转化为带物理单位的数据，单位为：°/s
// 设置为:0xA0 陀螺仪量程为:±62.5  dps     获取到的陀螺仪数据除以524.3         可以转化为带物理单位的数据，单位为：°/s
// 设置为:0xC0 陀螺仪量程为:±31.25 dps     获取到的陀螺仪数据除以1048.6        可以转化为带物理单位的数据，单位为：°/s
// 设置为:0xE0 陀螺仪量程为:±15.625dps     获取到的陀螺仪数据除以2097.2        可以转化为带物理单位的数据，单位为：°/s

#define icm42688_GYRO_ODR         	  (0x06)                     // 陀螺仪输出数据速率
// 设置为:0x03 加速度计输出数据速率为:8kHz
// 设置为:0x04 加速度计输出数据速率为:4kHz
// 设置为:0x05 加速度计输出数据速率为:2kHz
// 设置为:0x06 加速度计输出数据速率为:1kHz (default)
// 设置为:0x07 加速度计输出数据速率为:200Hz
// 设置为:0x08 加速度计输出数据速率为:100Hz
// 设置为:0x09 加速度计输出数据速率为:50Hz
// 设置为:0x0A 加速度计输出数据速率为:25Hz
// 设置为:0x0B 加速度计输出数据速率为:12.5Hz
// 设置为:0x0F 加速度计输出数据速率为:500Hz

#define icm42688_GYRO_MODE          (0x0C)                      //陀螺仪模式
// 设置为:0x00 陀螺仪模式为:Turns gyroscope off (default)
// 设置为:0x04 陀螺仪模式为:Places gyroscope in Standby Mode
// 设置为:0x0C 陀螺仪模式为:Places gyroscope in Low Noise (LN) Mode
//================================================定义 icm42688 内部地址================================================

extern icm42688_st icm42688_data;
void    icm42688_get_temp           (void);
void    icm42688_get_acc            (void);
void    icm42688_get_gyro           (void);
float   icm42688_temp_transition    (int16_t acc_value);                          // 将 icm42688 加速度计数据转换为实际物理数据
float   icm42688_acc_transition     (int16_t acc_value);                          // 将 icm42688 加速度计数据转换为实际物理数据
float   icm42688_gyro_transition    (int16_t gyro_value);                         // 将 icm42688 陀螺仪数据转换为实际物理数据
uint8_t   icm42688_init               (void);

#endif

