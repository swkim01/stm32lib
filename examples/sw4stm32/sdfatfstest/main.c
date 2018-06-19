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
#include "bsp_sd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* FatFs includes component */
#include "ff_gen_drv.h"
#include "sd_diskio.h"


/* Private define */
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
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

#define MAX_FILES 25

/* Private variables */
SD_HandleTypeDef hsd;
UART_HandleTypeDef huart2;

FATFS SDFatFs;
char SDPath[4];   /* SD logical drive path */
char* pDirectoryFiles[MAX_FILES];
FIL file;
char *text[16];
int written;

/* Private function prototypes */
void SystemClock_Config(void);
static void USART2_UART_Init(void);
void DisplayFiles(void);

int main(void)
{
    uint8_t ret;
    uint32_t counter=0;
    BSP_SD_CardInfo cardInfo;
 
    HAL_Init();
    /* Configure the system clock */
    SystemClock_Config();
    USART2_UART_Init();

    printf("hello stm32\r\n");

    BSP_SD_Init();

    BSP_SD_GetCardInfo(&cardInfo);
    printf("cardblock=%ld, capacity=%ld\r\n", cardInfo.BlockSize, cardInfo.BlockNbr);

    ret = FATFS_LinkDriver(&SD_Driver, SDPath);
    /* Check the mounted device */
    if (f_mount(&SDFatFs, (TCHAR const*)"/", 0) != FR_OK) {
        printf("FATFS Not mounted!!\r\n");
    }
    else {
        /* no operation */
        /* Initialize the Directory Files pointers (heap) */
        for (counter = 0; counter < MAX_FILES; counter++) {
            pDirectoryFiles[counter] = malloc(11);
        }
    }

#if 1
    FILINFO fno;
    uint8_t mode = FA_WRITE | FA_READ;
    if (f_stat("test1.txt", &fno) != FR_OK) {
    	mode |= FA_CREATE_ALWAYS;
    }
    if (f_open(&file, "test1.txt", mode) != FR_OK) {
    	printf("file  Not opened!!\r\n");
    }
    if (f_lseek(&file, 0) != FR_OK) {
        printf("f_lseek Error!!\r\n");
    }
    memset(text, 0, sizeof(text));
    sprintf((char *)text, "helloworld");
    int len = strlen((char const *)text);
    if (f_write(&file, text, len, (void*)&written) != FR_OK) {
        printf("f_write Error!!\r\n");
    }
    if (f_sync(&file) != FR_OK) {
    	printf("f_sync Error!!\r\n");
    }
    if (f_close(&file) != FR_OK) {
        printf("f_close Error!!\r\n");
    }
#endif
    DisplayFiles();

    for(;;) {
        /* Make a little delay */
        HAL_Delay(100);
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
  RCC_OscInitStruct.PLL.PLLQ = 8;
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

void DisplayFiles(void)
{
  DIR directory;
  FILINFO fileInfo;
  FRESULT res;
  uint32_t i=0, j=0;

  /* Open directory */
  res = f_opendir(&directory, "/");
  //printf("res=%d\r\n", res);
  //HAL_Delay(100);
  if ((res != FR_OK)) {
    if (res == FR_NO_FILESYSTEM) {
      /* Display message: SD card not FAT formatted */
      printf("SD Card Not formatted!!\r\n");
    }
    else {
      /* Display message: Fail to open directory */
      printf("SD Card Open failed!!\r\n");
    }
  }
#if 0
  FILINFO fno;
  uint8_t mode = FA_WRITE | FA_READ;
  if (f_stat("test1.txt", &fno) != FR_OK) {
  	mode |= FA_CREATE_ALWAYS;
  }
  if (f_open(&file, "test1.txt", mode) != FR_OK) {
  	printf("file  Not opened!!\r\n");
  }
  if (f_lseek(&file, 0) != FR_OK) {
      printf("f_lseek Error!!\r\n");
  }
  memset(text, 0, sizeof(text));
  sprintf((char *)text, "helloworld");
  int len = strlen((char const *)text);
  if (f_write(&file, text, len, (void*)&written) != FR_OK) {
      printf("f_write Error!!\r\n");
  }
  if (f_sync(&file) != FR_OK) {
	  printf("f_sync Error!!\r\n");
  }
  if (f_close(&file) != FR_OK) {
      printf("f_close Error!!\r\n");
  }
#endif
  printf("list files\r\n");
  /* Get number of files */
  i = 0;
  for (;;) {
    res = f_readdir(&directory, &fileInfo);
    //printf("res=%d\r\n", res);
    //HAL_Delay(0);
    if (res != FR_OK || fileInfo.fname[0] == 0) break;
    if (fileInfo.fname[0] == '.') continue;

    if (!(fileInfo.fattrib & AM_DIR)) {
      do {
        i++;
      } while (fileInfo.fname[i] != 0x2E);

      if (j < MAX_FILES) {
        sprintf(pDirectoryFiles[j], "%-12.12s", fileInfo.fname);
        j++;
        i = 0;
      }
    }
  }
  res = f_closedir(&directory);

  for (i = 0; i < j; i++)
    printf("file = %s\r\n", pDirectoryFiles[i]);
}

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
  if (huart->Instance==USART2) {
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

  if (huart->Instance==USART2) {
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
