/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#define MPU6050_I2C_ADDR            0xD0 /* 0x68 */
#define MPU6050_SMPLRT_DIV          0x19
#define MPU6050_CONFIG              0x1A
#define MPU6050_GYRO_CONFIG         0x1B
#define MPU6050_ACCEL_CONFIG        0x1C
#define MPU6050_ACCEL_XOUT_H        0x3B
#define MPU6050_ACCEL_XOUT_L        0x3C
#define MPU6050_ACCEL_YOUT_H        0x3D
#define MPU6050_ACCEL_YOUT_L        0x3E
#define MPU6050_ACCEL_ZOUT_H        0x3F
#define MPU6050_ACCEL_ZOUT_L        0x40
#define MPU6050_TEMP_OUT_H          0x41
#define MPU6050_TEMP_OUT_L          0x42
#define MPU6050_GYRO_XOUT_H         0x43
#define MPU6050_GYRO_XOUT_L         0x44
#define MPU6050_GYRO_YOUT_H         0x45
#define MPU6050_GYRO_YOUT_L         0x46
#define MPU6050_GYRO_ZOUT_H         0x47
#define MPU6050_GYRO_ZOUT_L         0x48
#define MPU6050_I_AM                0x68
#define MPU6050_PWR_MGMT_1          0x6B
#define MPU6050_WHO_AM_I            0x75

#define MPU6050_RATE_1KHZ           7
#define MPU6050_ACC_8G              0x02
#define MPU6050_GYRO_250S           0x00
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
  #define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
  #define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  printf("Hello stm32\r\n");
  /* MPU6050 초기화 */
  //i2c_set_frequency(MPU6050_I2C, 400000);
  uint8_t address = MPU6050_I2C_ADDR;
  /* 장치 연결 점검 */
  if (HAL_I2C_IsDeviceReady(&hi2c1, (uint8_t)address, 2, 5) != HAL_OK) {
    /* 오류 처리 */
    _Error_Handler(__FILE__, __LINE__);
  }
  /* MPU6050 장치 확인 */
  /* 레지스터 주소 전송 */
  uint8_t reg = MPU6050_WHO_AM_I;
  if (HAL_I2C_Master_Transmit(&hi2c1, address, &reg, 1, 1000) != HAL_OK) {
      /* 오류 처리 */
    _Error_Handler(__FILE__, __LINE__);
  }
  uint8_t temp;
  if (HAL_I2C_Master_Receive(&hi2c1, address, &temp, 1, 1000) != HAL_OK) {
    /* 오류 처리 */
    _Error_Handler(__FILE__, __LINE__);
  }
  if (temp != MPU6050_I_AM) {
    _Error_Handler(__FILE__, __LINE__);
  }
  /* MPU6050 기동 */
  uint8_t bytes[2] = {MPU6050_PWR_MGMT_1, 0x00};
  if (HAL_I2C_Master_Transmit(&hi2c1, address, (uint8_t *)bytes, 2, 1000) != HAL_OK) {
    /* 오류 처리 */
    _Error_Handler(__FILE__, __LINE__);
  }
  /* 샘플레이트를 1kHz로 설정 */
  /* 데이터 처리율 설정 */
  bytes[0] = MPU6050_SMPLRT_DIV;
  bytes[1] = MPU6050_RATE_1KHZ;
  if (HAL_I2C_Master_Transmit(&hi2c1, address, (uint8_t *)bytes, 2, 1000) != HAL_OK) {
    /* 오류 처리 */
    _Error_Handler(__FILE__, __LINE__);
  }
  /* 가속도계 설정 */
  reg = MPU6050_ACCEL_CONFIG;
  if (HAL_I2C_Master_Transmit(&hi2c1, address, &reg, 1, 1000) != HAL_OK) {
    /* 오류 처리 */
    _Error_Handler(__FILE__, __LINE__);
  }
  if (HAL_I2C_Master_Receive(&hi2c1, address, &temp, 1, 1000) != HAL_OK) {
    /* 오류 처리 */
    _Error_Handler(__FILE__, __LINE__);
  }
  temp = (temp & 0xE7) | (uint8_t)MPU6050_ACC_8G << 3;
  bytes[0] = reg;
  bytes[1] = temp;
  if (HAL_I2C_Master_Transmit(&hi2c1, address, (uint8_t *)bytes, 2, 1000) != HAL_OK) {
    /* 오류 처리 */
    _Error_Handler(__FILE__, __LINE__);
  }

  /* 자이로스코프 설정 */
  reg = MPU6050_GYRO_CONFIG;
  if (HAL_I2C_Master_Transmit(&hi2c1, address, &reg, 1, 1000) != HAL_OK) {
    /* 오류 처리 */
    _Error_Handler(__FILE__, __LINE__);
  }
  if (HAL_I2C_Master_Receive(&hi2c1, address, &temp, 1, 1000) != HAL_OK) {
    /* 오류 처리 */
    _Error_Handler(__FILE__, __LINE__);
  }
  temp = (temp & 0xE7) | (uint8_t)MPU6050_GYRO_250S << 3;
  bytes[0] = reg;
  bytes[1] = temp;
  if (HAL_I2C_Master_Transmit(&hi2c1, address, (uint8_t *)bytes, 2, 1000) != HAL_OK) {
    /* 오류 처리 */
    _Error_Handler(__FILE__, __LINE__);
  }
  /* Green LED on */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
    /* 전체 원시 데이터(14바이트) 읽기 */
    uint8_t data[14];
    reg = MPU6050_ACCEL_XOUT_H;
    if (HAL_I2C_Master_Transmit(&hi2c1, address, &reg, 1, 1000) != HAL_OK) {
      /* 오류 처리 */
      _Error_Handler(__FILE__, __LINE__);
    }
    if (HAL_I2C_Master_Receive(&hi2c1, address, data, 14, 1000) != HAL_OK) {
      /* 오류 처리 */
      _Error_Handler(__FILE__, __LINE__);
    }
    /* 가속도 값 구성 */
    int16_t acc_x = (int16_t)(data[0] << 8 | data[1]);
    int16_t acc_y = (int16_t)(data[2] << 8 | data[3]);
    int16_t acc_z = (int16_t)(data[4] << 8 | data[5]);
    /* 온도 값 구성 */
    temp = (data[6] << 8 | data[7]);
    int16_t temperature = (float)((float)((int16_t)temp) / (float)340.0 + (float)36.53);
    /* 자이로스코프 값 구성 */
    int16_t gyro_x = (int16_t)(data[8] << 8 | data[9]);
    int16_t gyro_y = (int16_t)(data[10] << 8 | data[11]);
    int16_t gyro_z = (int16_t)(data[12] << 8 | data[13]);

    printf("acc:x=%d,y=%d,z=%d\r\n", acc_x, acc_y, acc_z);

    HAL_Delay(1000);
  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
  while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY);

  return ch;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
