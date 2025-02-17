/**********************ICM42688 by ABO**************/

#include "ICM42688.h"
#include "spi.h"
#include "stdio.h"
icm42688_st icm42688_data = {0,0,0,0,0,0}; 

//-------------------------------------------------------------------------------------------------------------------
// �������     icm42688 д�Ĵ���
// ����˵��     reg             �Ĵ�����ַ
// ����˵��     data            ����
// ���ز���     void
// ʹ��ʾ��     icm42688_write_register(icm42688_PWR_MGMT_1, 0x80);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static void icm42688_write_register(uint8_t reg, uint8_t data)
{
	uint8_t reg_w = reg | icm42688_SPI_W;
  icm42688_CS(0);//Ƭѡѡ��
	HAL_SPI_Transmit(&hspi1,&reg_w,1,0x1000);//�Ĵ���д����
	HAL_SPI_Transmit(&hspi1,&data,1,0x1000);//д������
  icm42688_CS(1);//Ƭѡ�ͷ�
}

//-------------------------------------------------------------------------------------------------------------------
// �������     icm42688 ���Ĵ���
// ����˵��     reg             �Ĵ�����ַ
// ���ز���     uint8           ����
// ʹ��ʾ��     icm42688_read_register(icm42688_WHO_AM_I);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static uint8_t icm42688_read_register(uint8_t reg)
{
    uint8_t data = 0;
		uint8_t reg_r = reg | icm42688_SPI_R;
    icm42688_CS(0);//Ƭѡѡ��
		HAL_SPI_Transmit(&hspi1,&reg_r,1,0x1000);//�Ĵ���������
		HAL_SPI_Receive(&hspi1,&data,1,0x1000);//��������
    icm42688_CS(1);//Ƭѡ�ͷ�
    return data;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     icm42688 ������
// ����˵��     reg             �Ĵ�����ַ
// ����˵��     data            ���ݻ�����
// ����˵��     len             ���ݳ���
// ���ز���     void
// ʹ��ʾ��     icm42688_read_registers(icm42688_ACCEL_XOUT_H, dat, 6);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static void icm42688_read_registers(uint8_t reg, uint8_t *data, uint32_t len)
{
		uint8_t reg_r = reg | icm42688_SPI_R;
    icm42688_CS(0);//Ƭѡѡ��
		HAL_SPI_Transmit(&hspi1,&reg_r,1,0x1000);//�Ĵ���������
		HAL_SPI_Receive(&hspi1,data,len,0x1000);//����len�ֽ�����
	
    icm42688_CS(1);//Ƭѡ�ͷ�
}

//-------------------------------------------------------------------------------------------------------------------
// �������     icm42688 �Լ�
// ����˵��     void
// ���ز���     uint8           1-�Լ�ʧ�� 0-�Լ�ɹ�
// ʹ��ʾ��     icm42688_self_check();
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static uint8_t icm42688_self_check (void)
{
    uint8_t dat = 0, return_state = 0;
    uint16_t timeout_count = 0;

    while(icm42688_ID != dat)                                                          // �ж� ID �Ƿ���ȷ
    {
        if(timeout_count ++ > icm42688_TIMEOUT_COUNT)//�Լ�ʧ�ܣ���ʱ�˳�
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
// �������     ��ȡ icm42688 �¶ȼ�����
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     icm42688_get_temp();                                             // ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void icm42688_get_temp(void)
{
    uint8_t dat[6];
    icm42688_read_registers(icm42688_TEMP_DATA1, dat, 2);
    icm42688_data.temp = (int16_t)(((uint16_t)dat[0] << 8 | dat[1]));
}
//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡ icm42688 ���ٶȼ�����
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     icm42688_get_acc();                                             // ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
// ��ע��Ϣ     
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
// �������     ��ȡicm42688����������
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     icm42688_get_gyro();                                            // ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
// ��ע��Ϣ     
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
// �������     �� icm42688 �¶ȴ���������ת��Ϊʵ����������
// ����˵��     temp_value      ������ļ��ٶȼ�����
// ���ز���     void
// ʹ��ʾ��     temp_data = icm42688_temp_transition(icm42688_temp);           // ��λΪ g(m/s^2)
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
float icm42688_temp_transition (int16_t temp_value)
{
  float temp_data = 0;
	
	temp_data = (float)temp_value / 132.48f + 25;   // ����132.48���ټ�25����λΪ ��C
	
  return temp_data;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �� icm42688 ���ٶȼ�����ת��Ϊʵ����������
// ����˵��     acc_value      ������ļ��ٶȼ�����
// ���ز���     void
// ʹ��ʾ��     float data = icm42688_acc_transition(icm42688_acc_x);           // ��λΪ g(m/s^2)
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
float icm42688_acc_transition (int16_t acc_value)
{
	float acc_data = 0;
	switch(icm42688_ACCEL_FS_SEL)
	{
			case 0x00: acc_data = (float)acc_value / 2048;   break;// ����Ϊ:0x00 ���ٶȼ�����Ϊ:��16g         ��ȡ���ļ��ٶȼ����� ����2048       ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
			case 0x20: acc_data = (float)acc_value / 4096;   break;// ����Ϊ:0x20 ���ٶȼ�����Ϊ:��8g          ��ȡ���ļ��ٶȼ����� ����4096       ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
			case 0x40: acc_data = (float)acc_value / 8192;   break;// ����Ϊ:0x40 ���ٶȼ�����Ϊ:��4g          ��ȡ���ļ��ٶȼ����� ����8192       ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
			case 0x60: acc_data = (float)acc_value / 16384;  break;// ����Ϊ:0x60 ���ٶȼ�����Ϊ:��2g          ��ȡ���ļ��ٶȼ����� ����16384      ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
			default: break;
	}
  return acc_data;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �� icm42688 ����������ת��Ϊʵ����������
// ����˵��     gyro_value      �����������������
// ���ز���     void
// ʹ��ʾ��     float data = icm42688_gyro_transition(icm42688_gyro_x);         // ��λΪ��/s
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
float icm42688_gyro_transition (int16_t gyro_value)
{
	float gyro_data = 0;
	switch(icm42688_GYRO_FS_SEL)
	{
			case 0x00: gyro_data = (float)gyro_value / 16.4f;    break;// ����Ϊ:0x00 ����������Ϊ:��2000  dps     ��ȡ�������������ݳ���16.4          ����ת��Ϊ������λ�����ݣ���λΪ����/s
			case 0x20: gyro_data = (float)gyro_value / 32.8f;    break;// ����Ϊ:0x20 ����������Ϊ:��1000  dps     ��ȡ�������������ݳ���32.8          ����ת��Ϊ������λ�����ݣ���λΪ����/s
			case 0x40: gyro_data = (float)gyro_value / 65.5f;    break;// ����Ϊ:0x40 ����������Ϊ:��500   dps     ��ȡ�������������ݳ���65.5          ����ת��Ϊ������λ�����ݣ���λΪ����/s
			case 0x60: gyro_data = (float)gyro_value / 131.0f;   break;// ����Ϊ:0x60 ����������Ϊ:��250   dps     ��ȡ�������������ݳ���131           ����ת��Ϊ������λ�����ݣ���λΪ����/s
			case 0x80: gyro_data = (float)gyro_value / 262.0f;   break;// ����Ϊ:0x80 ����������Ϊ:��125   dps     ��ȡ�������������ݳ���262           ����ת��Ϊ������λ�����ݣ���λΪ����/s
			case 0xA0: gyro_data = (float)gyro_value / 524.3f;   break;// ����Ϊ:0xA0 ����������Ϊ:��62.5  dps     ��ȡ�������������ݳ���524.3         ����ת��Ϊ������λ�����ݣ���λΪ����/s
			case 0xC0: gyro_data = (float)gyro_value / 1048.6f;  break;// ����Ϊ:0xC0 ����������Ϊ:��31.25 dps     ��ȡ�������������ݳ���1048.6        ����ת��Ϊ������λ�����ݣ���λΪ����/s
			case 0xE0: gyro_data = (float)gyro_value / 2097.2f;  break;// ����Ϊ:0xE0 ����������Ϊ:��15.625dps     ��ȡ�������������ݳ���2097.2        ����ת��Ϊ������λ�����ݣ���λΪ����/s
			default: break;
	}
  return gyro_data;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��ʼ�� icm42688
// ����˵��     void
// ���ز���     uint8           1-��ʼ��ʧ�� 0-��ʼ���ɹ�
// ʹ��ʾ��     icm42688_init();
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
uint8_t icm42688_init (void)
{
    uint8_t val = 0x0, return_state = 0;
    uint16_t timeout_count = 0;

    HAL_Delay(10);                                                      // �ϵ���ʱ

		if(icm42688_self_check())
		{
				printf("imu_selfcheck_err\n");
				return_state = 1;//�������Լ�ʧ��
				return return_state;
		}
		icm42688_write_register(icm42688_REG_BANK_SEL,icm42688_Bank_0);		//����bank 0����Ĵ���
		icm42688_write_register(icm42688_DEVICE_CONFIG, icm42688_SOFT_RESET_CONFIG);      // ��λ�豸
		HAL_Delay(10); //After writing 1 to this SOFT_RESET_CONFIG, wait 10ms for soft reset to be effective, before attempting any other register access

		do
		{                                                                       // �ȴ���λ�ɹ�
			val = icm42688_read_register(icm42688_DEVICE_CONFIG);
			if(timeout_count ++ > icm42688_TIMEOUT_COUNT)
			{
				printf("imu_overtime\n");
				return_state = 1;//��ʱ����
				return return_state;
			}
		}while(0x00 != val);//DEVICE_CONFIG Reset value: 0x00
	
		icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_1); //����bank 1����Ĵ���
		icm42688_write_register(icm42688_INTF_CONFIG4, 0x02); //����Ϊ4��SPIͨ��
	
		icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_0); //����bank 0����Ĵ���
		icm42688_write_register(icm42688_INTF_CONFIG0, 0xB0);
	
		icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_0); //����bank 0����Ĵ���
        icm42688_write_register(icm42688_FIFO_CONFIG, 0x40); //Stream-to-FIFO Mode(page61)
	
	//�������ٶȼơ������ǵ�FIFO
		val = icm42688_read_register(icm42688_INT_SOURCE0);
        icm42688_write_register(icm42688_INT_SOURCE0, icm42688_Bank_0);
        icm42688_write_register(icm42688_FIFO_CONFIG2, 0x00); // watermark
        icm42688_write_register(icm42688_FIFO_CONFIG3, 0x02); // watermark
        icm42688_write_register(icm42688_INT_SOURCE0, val);
        icm42688_write_register(icm42688_FIFO_CONFIG1, 0x47); // Enable the accel and gyro to the FIFO
	
		icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_0); //����bank 0����Ĵ���
        icm42688_write_register(icm42688_INT_CONFIG, 0x36);

		icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_0); //����bank 0����Ĵ���
		val = (icm42688_read_register(icm42688_INT_SOURCE0)|FIFO_THS_INT1_EN);
        icm42688_write_register(icm42688_INT_SOURCE0, val);
	
	// ���ü��ٶȼ�����
			icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_0); //����bank 0����Ĵ���
	val = ((icm42688_read_register(icm42688_ACCEL_CONFIG0)&0x1F)|(icm42688_ACCEL_FS_SEL));//+-4g  //page74
			icm42688_write_register(icm42688_ACCEL_CONFIG0, val);
	
	// ���ü��ٶȼ������������
	icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_0); //����bank 0����Ĵ���
	val = ((icm42688_read_register(icm42688_ACCEL_CONFIG0)&0xF0)|(icm42688_ACCEL_ODR));//1kHz  //page74
			icm42688_write_register(icm42688_ACCEL_CONFIG0, val);
	
	// ��������������
	icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_0); //����bank 0����Ĵ���
	val = ((icm42688_read_register(icm42688_GYRO_CONFIG0)&0x1F)|(icm42688_GYRO_FS_SEL));
	icm42688_write_register(icm42688_GYRO_CONFIG0,val);
	
	// ���������������������
	icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_0); //����bank 0����Ĵ���  
	val = ((icm42688_read_register(icm42688_GYRO_CONFIG0)&0xF0)|icm42688_GYRO_ODR);
	icm42688_write_register(icm42688_GYRO_CONFIG0, val); 
	
		// ���������ǵ�ͨ�˲���
		icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_0); //����bank 0����Ĵ���  
		val = ((icm42688_read_register(icm42688_GYRO_ACCEL_CONFIG0)&0xF0)|0x07);
		icm42688_write_register(icm42688_GYRO_ACCEL_CONFIG0, val); 
	
		// �����������ݲ��˲��� �˳�г������Ҫ��������ÿ����
		icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_1); //����bank 0����Ĵ���  
		val = ((icm42688_read_register(icm42688_GYRO_CONFIG_STATIC10)&0x8F)|0x00);
		icm42688_write_register(icm42688_GYRO_CONFIG_STATIC10, val);
		
		val = ((icm42688_read_register(icm42688_GYRO_CONFIG_STATIC9)&0x8F)|0x00);
		icm42688_write_register(icm42688_GYRO_CONFIG_STATIC9, val);
	
	
	// ���ü��ٶȼ�ģʽ
	icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_0); //����bank 0����Ĵ���  
	val = icm42688_read_register(icm42688_PWR_MGMT0)|(icm42688_ACCEL_MODE); // Accel on in LNM //��ȡPWR��MGMT0��ǰ�Ĵ�����ֵ(page72)
	icm42688_write_register(icm42688_PWR_MGMT0, val);  
	HAL_Delay(10);  
	
	// ����������ģʽ
	icm42688_write_register(icm42688_REG_BANK_SEL, icm42688_Bank_0); //����bank 0����Ĵ���  
	val = icm42688_read_register(icm42688_PWR_MGMT0)|(icm42688_GYRO_MODE); // Gyro on in LN
	icm42688_write_register(icm42688_PWR_MGMT0, val);  //������PWR��MGMT0�Ĵ����� 200us�ڲ������κζ�д�Ĵ����Ĳ���
	HAL_Delay(10);  

    return return_state;
}
