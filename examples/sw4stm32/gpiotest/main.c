/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    12-January-2017
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"

#include "../stm32lib/extirq.h"
#include "../stm32lib/gpio.h"

void SystemClock_Config(void);
void button_click(int i, int edge);

int main(void)
{
    /* 플래시, systick 및 기타 장치 초기화 */
    HAL_Init();
    /* Configure the system clock */
    SystemClock_Config();

    /* LED2 (GPIO A5 pin) 초기화 */
    gpio_init(GPIO_LED1, GPIO_OUT);

    /* B5 (GPIO C13 pin) 초기화 */
    extirq_init(GPIO_PC_13, (EXTIRQ_Callback_t)button_click, EXTIRQ_FALLING_EDGE);

    for (;;) {
       gpio_write(GPIO_LED1, 1);
       HAL_Delay(500);	/* 500ms 지연시간 */
       gpio_write(GPIO_LED1, 0);
       HAL_Delay(500);	/* 500ms 지연시간 */
    }
}

void button_click(int i, int edge)
{
    gpio_toggle(GPIO_LED1);
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
