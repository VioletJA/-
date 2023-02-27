/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "i2c.h"
#include "stdio.h"
#include "ds3231.h"
#include "string.h"
#include <stdlib.h>

struct __FILE
{
        int handle;
};
FILE __stdout;

int fputc(int ch, FILE *f)
{
     HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
         return (ch);
}

DateTime TIME;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define SER1_L HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET)
#define SER1_H HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET)

#define SER2_L HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET)
#define SER2_H HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET)

#define STCP_L HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define STCP_H HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)

#define SHCP_L HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET)
#define SHCP_H HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET)
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//1:0100  2:0000 3:1000 4:1001 5:0001 6:0011 7:1010 8:1011 9:0010 0:1100
#define T0  0x0C  
#define T1  0x04  
#define T2  0x00 
#define T3  0x08  
#define T4  0x09 
#define T5  0x01 
#define T6  0x03 
#define T7  0x0b 
#define T8  0x0a 
#define T9  0x02 
#define Tx  0xff 


#define key HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Write_74hc595_Test(uint32_t TxData1,uint32_t TxData2)
{
	int i=0;
	for(i=0;i<8;i++)
	{
		if(TxData1 & 0x80)	
			SER1_H;	
		else
			SER1_L;
		
		if(TxData2 & 0x80) 
		    SER2_H;
		else
			SER2_L;

		SHCP_H;//时钟线上升沿，加载两条数据线到移位寄存器
		TxData1 <<= 1;
		TxData2 <<= 1;
		SHCP_L;
	}
	STCP_H;
	STCP_L;
}

uint32_t Bytes_Config(uint32_t n1,uint32_t n2){
	uint32_t n3 = 0x0;
	n3 = (n1<<4) | n2;
	return n3;
}

uint32_t Num_Select(uint16_t num){
	switch(num){
	case 0:return T0; break;
	case 1:return T1; break;
	case 2:return T2; break;
	case 3:return T3; break;
	case 4:return T4; break;
	case 5:return T5; break;
	case 6:return T6; break;
	case 7:return T7; break;
	case 8:return T8; break;
	case 9:return T9; break;
	default:return Tx;
	}
}

uint16_t Random_Num(){
	uint16_t rnum = 0;
	srand(SysTick->VAL);
	rnum = rand() % 9;    //生成0-9内的随机整数
	return rnum;

}

uint32_t year_L;
uint32_t year_H;
int count=0;
int time_min1;
int time_min2;
int time_hour1;
int time_hour2;
int year1;
int year2;
int year3;
int year4;

int randnum1;
int randnum2;
int randnum3;
int randnum4;
uint32_t randnum_h;
uint32_t randnum_l;

uint32_t hour;
uint32_t min;

int sec_h;
int sec_l;

void year_show()
{
  for(int i=0;i<300000;i++)
	  {
		  count++;
		  if(count>7500)
		  {
			randnum1=Random_Num();
			randnum2=Random_Num();
			randnum3=Random_Num();
			randnum4=Random_Num();
			randnum_h=Bytes_Config(Num_Select(randnum1),Num_Select(randnum2));
			randnum_l=Bytes_Config(Num_Select(randnum3),Num_Select(randnum4));

			Write_74hc595_Test(randnum_h,randnum_l);
			count=0;
		  }
	  }
	  	  for(int i=0;i<250000;i++)
	  {
		  count++;
		  if(count>7500)
		  {
			randnum1=Random_Num();
			randnum2=Random_Num();
			randnum3=Random_Num();
			randnum4=Random_Num();
			randnum_h=Bytes_Config(Num_Select(year1),Num_Select(randnum2));
			randnum_l=Bytes_Config(Num_Select(randnum3),Num_Select(randnum4));

			Write_74hc595_Test(randnum_h,randnum_l);
			count=0;
		  }
	  }
	  	  for(int i=0;i<250000;i++)
	  {
		  count++;
		  if(count>7500)
		  {
			randnum1=Random_Num();
			randnum2=Random_Num();
			randnum3=Random_Num();
			randnum4=Random_Num();
			randnum_h=Bytes_Config(Num_Select(year1),Num_Select(year2));
			randnum_l=Bytes_Config(Num_Select(randnum3),Num_Select(randnum4));

			Write_74hc595_Test(randnum_h,randnum_l);
			count=0;
		  }
	  }
	  	  for(int i=0;i<250000;i++)
	  {
		  count++;
		  if(count>7500)
		  {
			randnum1=Random_Num();
			randnum2=Random_Num();
			randnum3=Random_Num();
			randnum4=Random_Num();
			randnum_h=Bytes_Config(Num_Select(year1),Num_Select(year2));
			randnum_l=Bytes_Config(Num_Select(year3),Num_Select(randnum4));

			Write_74hc595_Test(randnum_h,randnum_l);
			count=0;
		  }
	  }
	  Write_74hc595_Test(year_H,year_L);

}

void hour_min_show()
{
 
     for(int i=0;i<500000;i++)
	  {
		  count++;
		  if(count>7500)
		  {
			randnum1=Random_Num();
			randnum2=Random_Num();
			randnum3=Random_Num();
			randnum4=Random_Num();
			randnum_h=Bytes_Config(Num_Select(randnum1),Num_Select(randnum2));
			randnum_l=Bytes_Config(Num_Select(randnum3),Num_Select(randnum4));

			Write_74hc595_Test(randnum_h,randnum_l);
			count=0;
		  }
	  }
	  Write_74hc595_Test(hour,min);


}

void sec_show()
{
 
	  Write_74hc595_Test(min,Bytes_Config(Num_Select(sec_h),Num_Select(sec_l)));
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    //外部中断可以加自己相加的东西，也可以啥也不加，执行完返回睡眠前的位置继续执行
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int KEY_READ(void)   //普通延时判断按键，总共四个按键
{
	if(!key)
	{
	  HAL_Delay(10);
		if(!key)
		{
		  return 1;
		}
	}
	return 0;	
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
//  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  I2CInit();
//  TIME.year=2023;
//  TIME.month=2;
//  TIME.dayofmonth=16;
//  DS3231_setDate(23,2,22);
//  DS3231_setTime(16,17,0);
  DS3231_getDate(&TIME);
  DS3231_getTime(&TIME);

int count_time=0;
int count_in=0;
int count_key=0;
int sec_in=0;
int state=0;
int refesh=1;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//	  char txt[10];
//	  sprintf(txt, "date:%d,%d,%d",TIME.year,TIME.month,TIME.dayofmonth);
//	  sprintf(txt, "date:%d,%d,%d",TIME.hour,TIME.minute,TIME.second);
//	  HAL_UART_Transmit(&huart1,(unsigned char *)txt, strlen(txt), 50);
	  
	  DS3231_getDate(&TIME);
      DS3231_getTime(&TIME);
	  
	  time_hour1=TIME.hour/10;
	  time_hour2=TIME.hour%10;
	  
	  time_min1=TIME.minute/10;
	  time_min2=TIME.minute%10;
	  
      hour=Bytes_Config(Num_Select(time_hour1),Num_Select(time_hour2));
	  min=Bytes_Config(Num_Select(time_min1),Num_Select(time_min2));
	  
	  year1=TIME.year/1000;
	  year2=TIME.year%1000/100;	  
	  year3=TIME.year%1000%100/10;
	  year4=TIME.year%1000%100%10;
	  
	  year_H=Bytes_Config(Num_Select(year1),Num_Select(year2));
	  year_L=Bytes_Config(Num_Select(year3),Num_Select(year4));
	  
	  sec_h=TIME.second/10;
	  sec_l=TIME.second%10;
	  
	  if(KEY_READ()==1)
	  {
			state++;
			refesh=1;
		   if(state==2)
		   {
			 state=0;
		   }
	  }
	  
	  
	  if(state==0)
	  {
		 if(refesh==1)
		 {
	       hour_min_show();
			 refesh=0;
		 }
		 else
		   Write_74hc595_Test(hour,min);	 
			 
	  }
	  else if(state==1)
	  {
		  if(refesh==1)
		  {
	        year_show();
			  HAL_Delay(2000);
			  refesh=0;
		  }
		  else
			Write_74hc595_Test(min,Bytes_Config(Num_Select(sec_h),Num_Select(sec_l))); 
	  }
		  
	 // HAL_Delay(300);
	  
	 //  sec_show();
	//  hour_min_show();
	//  Write_74hc595_Test(0xff,0xff);
//	 if(count_time>500)
//	 {
////		 Write_74hc595_Test(0xff,0xff);
////		 HAL_SuspendTick();//停止系统滴答计时器
////		 HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);//电压调节器为低功耗模式，WFI指令进入停止模式
////		 SystemClock_Config();//重新配置系统时钟
////		 HAL_ResumeTick();//恢复系统滴答计时器
//		 count_time=0;
//		 count_in=0;
//		 count_key=0;
//		 sec_in=0;
//	 }

  



		

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
  while (1)
  {
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

