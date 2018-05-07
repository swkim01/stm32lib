/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "swi2c.h"
#include "mpu6050.h"
#if 1
#ifdef __GNUC__
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
  #define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
  #define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */

#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA

UART_HandleTypeDef huart2;
static void USART2_UART_Init(void);
#endif

MPU6050_t mpu6050;

void SystemClock_Config(void);

int main(void)
{
	GPIO_InitTypeDef gpioLed;

	/* 플래시, systick 및 기타 장치 초기화 */
	HAL_Init();
	/* Configure the system clock */
	SystemClock_Config();
#if 1
	USART2_UART_Init();

	printf("Hello stm32\r\n");
#endif

	/* LED2 (GPIO A5 pin) 초기화 */
	__GPIOA_CLK_ENABLE();
	gpioLed.Pin = GPIO_PIN_5;
	gpioLed.Mode = GPIO_MODE_OUTPUT_PP;
	gpioLed.Pull = GPIO_NOPULL;
	gpioLed.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOA, &gpioLed);

	/* MPU6050 초기화 */
	if (mpu6050_init(&mpu6050, 0, MPU6050_ACC_8G, MPU6050_GYRO_250S) == MPU6050_RES_OK) {
		/* Green LED on */
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	}

	for(;;) {
		mpu6050_readall(&mpu6050);

		printf("acc x=%d, y=%d, z=%d\r\n", mpu6050.acc_x, mpu6050.acc_y, mpu6050.acc_z);
		/* Raw data are available for use as needed */
		//MPU6050.Accelerometer_X;
		//MPU6050.Accelerometer_Y;
		//MPU6050.Accelerometer_Z;
		//MPU6050.Gyroscope_X;
		//MPU6050.Gyroscope_Y;
		//MPU6050.Gyroscope_Z;
		//MPU6050.Temperature;
  		//printf("pin=%d\r\n", HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)==GPIO_PIN_RESET);
		HAL_Delay(1000);	/* 100ms 지연시간 */
	}
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

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
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
#if 1
/* USART2 init function */
void USART2_UART_Init(void)
{
  /* GPIO Ports Clock Enable */
  __GPIOA_CLK_ENABLE();

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart2);

}

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
  while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY);

  return ch;
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(huart->Instance==USART2)
  {
    /* Peripheral clock enable */
    __USART2_CLK_ENABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  }

}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{

  if(huart->Instance==USART2)
  {
    /* Peripheral clock disable */
    __USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, USART_TX_Pin|USART_RX_Pin);

    /* Peripheral interrupt DeInit*/
    HAL_NVIC_DisableIRQ(USART2_IRQn);

  }
}
#endif
