/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ws2812.h"
#include "lcd.h "
#include "lcd_init.h "
#include "tb6612.h"
#include "ICM42688.h"
#include "M24_EEPROM.h"
#include "UI.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//printf���ض���

//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
//{
//    if(huart == &huart1)
//    {
//			if(dif_l>10)sprintf((char*)buffer,"0%ld\n",dif_l);
//			if(dif_l<10&&dif_l>0)sprintf((char*)buffer,"00%ld\n",dif_l);
//			HAL_UART_Transmit_DMA(&huart1,buffer,4);
//    }
//}
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*****************************************************************************/
//�������ֶ����еĺ�����
void Run_manual(void)
{
	if (Run_flag == 1)
	{
		uint8_t Speed1[4] = {0};
		uint8_t Speed2[4] = {0};
		uint8_t Status1[4] = {0};
		uint8_t Status2[4] = {0};
		EEPROM_ReadMultipleBytes(MOTOR1_SPEED, Speed1, 4);
		EEPROM_ReadMultipleBytes(MOTOR2_SPEED, Speed2, 4);
		EEPROM_ReadMultipleBytes(MOTOR1_STATUS, Status1, 4);
		EEPROM_ReadMultipleBytes(MOTOR2_STATUS, Status2, 4);
		
		while (Run_flag)
		{
			if (Status1[3] == 1 && Status2[3] == 0)
			{
				Motor_Start(L);
				Motor_RealSpeed(Speed1[3], L);
			}
			else if (Status1[3] == 1 && Status2[3] == 1)
			{
				Motor_Start(Both);
				Motor_RealSpeed(Speed1[3], L);
				Motor_RealSpeed(Speed2[3], R);
			}
			else if (Status1[3] == 0 && Status2[3] == 1)
			{
				Motor_Start(R);
				Motor_RealSpeed(Speed2[3], R);
			}
			else
			{
				HAL_Delay(1500);
				Failed();
				Run_flag = 0;
				return;
			}
		}
	}
}

/*****************************************************************************/
//�����ǵ�һ�ʵ�ִ�к�����
void Run_Auto_1(void)
{
	if (Run_flag == 2)	
	{
		float i=0;
		float now_angle=Angle_Data.yaw;
		int turn=0;
		Motor_Start(Both);
		while(1)
		{
			Motor_Distance(now_angle,59.0+i,9760);
			if(PID_D.Ek<10)
			{
				PID_D.Ek=0;
				PID_D.Ek1=0;
				PID_D.LocSum=0;
				accu_l=0;
				accu_r=0;
				now_angle=Angle_Data.yaw;
				i+=0.6;
				if(i>1.8)i=1.8;
			}
		}

//  ��һ�ζ���
//					while(1)
//			{
//				Motor_Distance(7050,10500);
//				if(PID_D.Ek<10)
//				{
//					accu_l=0;
//					accu_r=0;
//					PID_D.Ek=0;
//					PID_D.Ek1=0;
//					PID_D.LocSum=0;
//					break;
//				}
//			}
			
			Motor_Off(Both);  //�ص��
			//������ʾ
			WS2812_Set_Color(255, 255, 255);  ///��
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET);  //��
			HAL_Delay(200);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET);
			HAL_Delay(400);
			WS2812_Off();			
			Run_flag = 0;  //flag��λ
			Success();
		}
}	

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C3_Init();
  MX_SPI1_Init();
  MX_SPI3_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM8_Init();
  MX_TIM3_Init();
  MX_TIM9_Init();
  MX_TIM11_Init();
  MX_TIM10_Init();
  /* USER CODE BEGIN 2 */
	
	/* �����ʼ�� */
	LCD_Init();  //LCD_Init
	
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);  //��ʾ
	LCD_ShowString(0, 0, (uint8_t*)"IMU init, don't move", WHITE, BLACK, 24, 1);
	LCD_ShowString(0, 30, (uint8_t*)"the car", WHITE, BLACK, 24, 1);
	
	icm42688_init ();  //IMU ��ʼ��
	
	IMU_Calibration();  //IMU��ƯУ׼���ϵ�ʱ��Ҫ�ƶ�С������Ȼ��һֱ��������
	
//WS2812_Init();  //WS2812 Init
	    
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);  //��������ʼ��
	HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_ALL);
//	
//	if (M24C02_Check()) Error_Handler();  //EEPROM��ʼ��
//	
	LCD_Fill(0,0,LCD_W,LCD_H,0xFFFE);  //������˵������������0xfffe�������־
	LCD_Fill(0, 0, 240, 50, GRAYBLUE);  //��һ�δ�ӡ�����
	
	
	HAL_TIM_Base_Start_IT(&htim11);  //�������������ٶ�ʱ��
	HAL_TIM_Base_Start_IT(&htim2);  //����2812��ʱ��
	Motor_Start(Both);	
//	float cal_angle=Angle_Data.yaw;
	
	HAL_TIM_Base_Start_IT(&htim11);  //�������������ٶ�ʱ��
	HAL_TIM_Base_Start_IT(&htim2);  //����2812��ʱ��
	
//	DispCrtMenu();  //��һ�δ�ӡui
	
//	HAL_UART_Transmit_DMA(&huart1,buffer,4);
	
  /* USER CODE END 2 */
	
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	accu_l=0;
	accu_r=0;
		float i=0;
		float now_angle=Angle_Data.yaw;
		int turn=0;
		Motor_Start(Both);

	while (1)
	{
//				while(1)
//		{
//			Motor_Distance(now_angle,59.0+i,9760);
//			if(PID_D.Ek<10)
//			{
//				PID_D.Ek=0;
//				PID_D.Ek1=0;
//				PID_D.LocSum=0;
//				accu_l=0;
//				accu_r=0;
//				now_angle=Angle_Data.yaw;
//				i+=0.6;
//			}
//		}
//		Motor_Distance(0,56.8,100);
		while(1)
		{
			Motor_Distance(now_angle,59.0+i,9755);
			if(PID_D.Ek<10)
			{
				PID_D.Ek=0;
				PID_D.Ek1=0;
				PID_D.LocSum=0;
				accu_l=0;
				accu_r=0;
				now_angle=Angle_Data.yaw;
				i+=0.5;
				turn++;
			}
			if(turn==2)
			{
				i=0;	
				turn=0;
				break;
			}
		}
			while(1)
		{
			Motor_Distance(now_angle,118.2,19550);
			if(PID_D.Ek<10)
			{
				PID_D.Ek=0;
				PID_D.Ek1=0;
				PID_D.LocSum=0;
				accu_l=0;
				accu_r=0;
				now_angle=Angle_Data.yaw;
				break;
			}

		}
		while(1)
		{
			Motor_Distance(now_angle,-117.5,9760);
			if(PID_D.Ek<10)
			{
				PID_D.Ek=0;
				PID_D.Ek1=0;
				PID_D.LocSum=0;
				accu_l=0;
				accu_r=0;
				now_angle=Angle_Data.yaw;
				i+=0.5;
				break;
			}
		}
		while(1)
		{
			Motor_Distance(now_angle,-59.0-i,9750);
			if(PID_D.Ek<10)
			{
				PID_D.Ek=0;
				PID_D.Ek1=0;
				PID_D.LocSum=0;
				accu_l=0;
				accu_r=0;
				now_angle=Angle_Data.yaw;
				i+=0.5;
				if(i>1.4)i=1.4;
				break;
			}
		}
		while(1)
		{
			Motor_Distance(now_angle,-116.5,19515);
			if(PID_D.Ek<10)
			{
				PID_D.Ek=0;
				PID_D.Ek1=0;
				PID_D.LocSum=0;
				accu_l=0;
				accu_r=0;
				now_angle=Angle_Data.yaw;
				i+=0.5;
				if(i>1.4)i=1.4;
				break;
			}
		}
		/* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  
	LCD_Init();  //������ʾ
	LCD_Fill(0,0,LCD_W,LCD_H,RED);
	LCD_ShowString(0,0,(uint8_t*)"ERROR_HANDLER",WHITE, RED, 32,0);
	
	while (1)
  {
		;
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
