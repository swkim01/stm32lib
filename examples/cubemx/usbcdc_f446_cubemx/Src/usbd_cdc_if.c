/**
  ******************************************************************************
  * @file           : usbd_cdc_if.c
  * @version        : v1.0_Cube
  * @brief          : Usb device for Virtual Com Port.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_if.h"

/* USER CODE BEGIN INCLUDE */
#include "stm32f4xx_hal.h"

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device library.
  * @{
  */

/** @addtogroup USBD_CDC_IF
  * @{
  */

/** @defgroup USBD_CDC_IF_Private_TypesDefinitions USBD_CDC_IF_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */
USBD_CDC_LineCodingTypeDef LineCoding = {
  115200,                       /* baud rate */
  0x00,                         /* stop bits-1 */
  0x00,                         /* parity - none */
  0x08                          /* nb. of bits 8 */
};

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Defines USBD_CDC_IF_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */
/* Define size for the receive and transmit buffer over CDC */
/* It's up to user to redefine and/or remove those define */
#define APP_RX_DATA_SIZE  2048
#define APP_TX_DATA_SIZE  2048
#define CDC_POLLING_INTERVAL 5
/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Macros USBD_CDC_IF_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Variables USBD_CDC_IF_Private_Variables
  * @brief Private variables.
  * @{
  */
/* Create buffer for reception and transmission           */
/* It's up to user to redefine and/or remove those define */
/** Received data over USB are stored in this buffer      */
uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

/** Data to send over USB CDC are stored in this buffer   */
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

/* USER CODE BEGIN PRIVATE_VARIABLES */
uint32_t UserTxBufPtrIn = 0;    /* Increment this pointer or roll it back to
                                 * start address when data are received over
                                 * USART */
uint32_t UserTxBufPtrOut = 0;   /* Increment this pointer or roll it back to
                                 * start address when data are sent over USB */
uint32_t UserRxBufPtrIn = 0;    /* Increment this pointer or roll it back to
                                 * start address when data are received over
                                 * USB */
uint32_t UserRxBufPtrOut = 0;   /* Increment this pointer or roll it back to
                                 * start address when data are sent over read */

volatile uint32_t USB_received = 0;
uint8_t USBBuffer[64];
uint8_t USBPackSize;

/* UART handler declaration */
extern UART_HandleTypeDef huart2;
/* TIM handler declaration */
extern TIM_HandleTypeDef htim3;

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_Variables USBD_CDC_IF_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_FunctionPrototypes USBD_CDC_IF_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CDC_Init_FS(void);
static int8_t CDC_DeInit_FS(void);
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Receive_FS(uint8_t* pbuf, uint32_t *Len);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */
static void ComPort_Config(void);
static void TIM_Config(void);

/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_CDC_ItfTypeDef USBD_Interface_fops_FS =
{
  CDC_Init_FS,
  CDC_DeInit_FS,
  CDC_Control_FS,
  CDC_Receive_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the CDC media low layer over the FS USB IP
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Init_FS(void)
{
  /* USER CODE BEGIN 3 */
  /* USART configured as follow: - Word Length = 8 Bits - Stop Bit = One Stop
   * bit - Parity = No parity - BaudRate = 115200 baud - Hardware flow control
   * disabled (RTS and CTS signals) */
#if 1
  ComPort_Config();
#else
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK) {
    /* Initialization Error */
    Error_Handler();
  }

  /* Put UART peripheral in IT reception process */
  /* Any data received will be stored in "UserTxBuffer" buffer */
  if (HAL_UART_Receive_IT(&huart2, (uint8_t *) UserTxBufferFS, 1) != HAL_OK) {
    /* Transfer error in reception process */
    Error_Handler();
  }
#endif

#if 1
  /* Configure the TIM Base generation */
  TIM_Config();
#else
  /* Start TIM Base Channel1 */
  if (HAL_TIM_Base_Start_IT(&htim3) != HAL_OK) {
    /* Starting Error */
    Error_Handler();
  }
#endif

  /* Set Application Buffers */
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS, 0);
  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS);
  return (USBD_OK);
  /* USER CODE END 3 */
}

/**
  * @brief  DeInitializes the CDC media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_DeInit_FS(void)
{
  /* USER CODE BEGIN 4 */
  /* DeInitialize the UART peripheral */
  if (HAL_UART_DeInit(&huart2) != HAL_OK) {
    /* Initialization Error */
    Error_Handler();
  }
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  Manage the CDC class requests
  * @param  cmd: Command code
  * @param  pbuf: Buffer containing command data (request parameters)
  * @param  length: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
  /* USER CODE BEGIN 5 */
  switch(cmd)
  {
    case CDC_SEND_ENCAPSULATED_COMMAND:

    break;

    case CDC_GET_ENCAPSULATED_RESPONSE:

    break;

    case CDC_SET_COMM_FEATURE:

    break;

    case CDC_GET_COMM_FEATURE:

    break;

    case CDC_CLEAR_COMM_FEATURE:

    break;

  /*******************************************************************************/
  /* Line Coding Structure                                                       */
  /*-----------------------------------------------------------------------------*/
  /* Offset | Field       | Size | Value  | Description                          */
  /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
  /* 4      | bCharFormat |   1  | Number | Stop bits                            */
  /*                                        0 - 1 Stop bit                       */
  /*                                        1 - 1.5 Stop bits                    */
  /*                                        2 - 2 Stop bits                      */
  /* 5      | bParityType |  1   | Number | Parity                               */
  /*                                        0 - None                             */
  /*                                        1 - Odd                              */
  /*                                        2 - Even                             */
  /*                                        3 - Mark                             */
  /*                                        4 - Space                            */
  /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
  /*******************************************************************************/
    case CDC_SET_LINE_CODING:
    LineCoding.bitrate = (uint32_t) (pbuf[0] | (pbuf[1] << 8) |
                                     (pbuf[2] << 16) | (pbuf[3] << 24));
    LineCoding.format = pbuf[4];
    LineCoding.paritytype = pbuf[5];
    LineCoding.datatype = pbuf[6];

    /* Set the new configuration */
    ComPort_Config();

    break;

    case CDC_GET_LINE_CODING:
    pbuf[0] = (uint8_t) (LineCoding.bitrate);
    pbuf[1] = (uint8_t) (LineCoding.bitrate >> 8);
    pbuf[2] = (uint8_t) (LineCoding.bitrate >> 16);
    pbuf[3] = (uint8_t) (LineCoding.bitrate >> 24);
    pbuf[4] = LineCoding.format;
    pbuf[5] = LineCoding.paritytype;
    pbuf[6] = LineCoding.datatype;

    break;

    case CDC_SET_CONTROL_LINE_STATE:

    break;

    case CDC_SEND_BREAK:

    break;

  default:
    break;
  }

  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Data received over USB OUT endpoint are sent over CDC interface
  *         through this function.
  *
  *         @note
  *         This function will block any OUT packet reception on USB endpoint
  *         untill exiting this function. If you exit this function before transfer
  *         is complete on CDC interface (ie. using DMA controller) it will result
  *         in receiving more data while previous ones are still not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
{
  /* USER CODE BEGIN 6 */
  /* Initiate next USB packet transfer once a packet is received */
  USBPackSize = *Len;
  memcpy(USBBuffer, Buf, USBPackSize);
  USB_received = 1;

  //USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
  //USBD_CDC_ReceivePacket(&hUsbDeviceFS);

  //HAL_UART_Transmit_DMA(&huart2, Buf, *Len);

  return (USBD_OK);
  /* USER CODE END 6 */
}

/**
  * @brief  CDC_Transmit_FS
  *         Data to send over USB IN endpoint are sent over CDC interface
  *         through this function.
  *         @note
  *
  *
  * @param  Buf: Buffer of data to be sent
  * @param  Len: Number of data to be sent (in bytes)
  * @retval USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len)
{
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 7 */
  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
  if (hcdc->TxState != 0){
    return USBD_BUSY;
  }
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, Buf, Len);
  result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);
  /* USER CODE END 7 */
  return result;
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
/**
  * @brief  TIM period elapsed callback
  * @param  htim: TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim)
{
  uint32_t buffptr;
  uint32_t buffsize;

  if (UserTxBufPtrOut != UserTxBufPtrIn) {
    if (UserTxBufPtrOut > UserTxBufPtrIn) { /* Rollback */
      buffsize = APP_TX_DATA_SIZE - UserTxBufPtrOut;
    }
    else {
      buffsize = UserTxBufPtrIn - UserTxBufPtrOut;
    }

    buffptr = UserTxBufPtrOut;

    if (CDC_Transmit_FS(&UserTxBufferFS[buffptr], buffsize) == USBD_OK) {
      UserTxBufPtrOut += buffsize;
      if (UserTxBufPtrOut == APP_RX_DATA_SIZE) {
        UserTxBufPtrOut = 0;
      }
    }
  }

  if (USB_received) {
    USB_received = 0;

    buffsize = APP_RX_DATA_SIZE - UserRxBufPtrIn;
    if (USBPackSize > 0) {
      if (USBPackSize > buffsize) {
        memcpy(((uint8_t *)UserRxBufferFS+UserRxBufPtrIn), &USBBuffer[0], buffsize);
        memcpy((uint8_t *)UserRxBufferFS, &USBBuffer[buffsize], (USBPackSize - buffsize));
      } else {
        memcpy(((uint8_t *)UserRxBufferFS+UserRxBufPtrIn), USBBuffer, USBPackSize);
      }
      UserRxBufPtrIn = ((UserRxBufPtrIn + USBPackSize) % APP_RX_DATA_SIZE);
      HAL_UART_Transmit_DMA(&huart2, USBBuffer, USBPackSize);
    }
  }
}

/**
  * @brief  Rx Transfer completed callback
  * @param  huart: UART handle
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart)
{
  if (huart->Instance==USART2) {
    /* Increment Index for buffer writing */
    UserTxBufPtrIn++;

    /* To avoid buffer overflow */
    if (UserTxBufPtrIn == APP_RX_DATA_SIZE)
    {
      UserTxBufPtrIn = 0;
    }

    /* Start another reception: provide the buffer pointer with offset and the
     * buffer size */
    HAL_UART_Receive_IT(huart, (uint8_t *) (UserTxBufferFS + UserTxBufPtrIn), 1);
  }
}

/**
  * @brief  Tx Transfer completed callback
  * @param  huart: UART handle
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef * huart)
{
  if (huart->Instance==USART2) {
    /* Initiate next USB packet transfer once UART completes transfer
     * (transmitting data over Tx line) */
    USBD_CDC_ReceivePacket(&hUsbDeviceFS);
  }
}

/**
  * @brief  ComPort_Config
  *         Configure the COM Port with the parameters received from host.
  * @param  None.
  * @retval None
  * @note   When a configuration is not supported, a default value is used.
  */
static void ComPort_Config(void)
{
  if (HAL_UART_DeInit(&huart2) != HAL_OK) {
    /* Initialization Error */
    Error_Handler();
  }

  /* set the Stop bit */
  switch (LineCoding.format)
  {
  case 0:
    huart2.Init.StopBits = UART_STOPBITS_1;
    break;
  case 2:
    huart2.Init.StopBits = UART_STOPBITS_2;
    break;
  default:
    huart2.Init.StopBits = UART_STOPBITS_1;
    break;
  }

  /* set the parity bit */
  switch (LineCoding.paritytype)
  {
  case 0:
    huart2.Init.Parity = UART_PARITY_NONE;
    break;
  case 1:
    huart2.Init.Parity = UART_PARITY_ODD;
    break;
  case 2:
    huart2.Init.Parity = UART_PARITY_EVEN;
    break;
  default:
    huart2.Init.Parity = UART_PARITY_NONE;
    break;
  }

  /* set the data type : only 8bits and 9bits is supported */
  switch (LineCoding.datatype)
  {
  case 0x07:
    /* With this configuration a parity (Even or Odd) must be set */
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    break;
  case 0x08:
    if (huart2.Init.Parity == UART_PARITY_NONE) {
      huart2.Init.WordLength = UART_WORDLENGTH_8B;
    }
    else {
      huart2.Init.WordLength = UART_WORDLENGTH_9B;
    }

    break;
  default:
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    break;
  }

  huart2.Instance = USART2;
  huart2.Init.BaudRate = LineCoding.bitrate;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&huart2) != HAL_OK) {
    /* Initialization Error */
    Error_Handler();
  }

  /* Start reception: provide the buffer pointer with offset and the buffer
   * size */
  HAL_UART_Receive_IT(&huart2, (uint8_t *) (UserTxBufferFS + UserTxBufPtrIn), 1);
}

/**
  * @brief  TIM_Config: Configure TIMx timer
  * @param  None.
  * @retval None
  */
static void TIM_Config(void)
{
  /* Set TIMx instance */
  htim3.Instance = TIM3;

  /* Initialize TIM3 peripheral as follows: + Period = (CDC_POLLING_INTERVAL *
   * 10000) - 1 + Prescaler = ((APB1 frequency / 1000000) - 1) + ClockDivision
   * = 0 + Counter direction = Up */
  htim3.Init.Period = (CDC_POLLING_INTERVAL * 1000) - 1;
  htim3.Init.Prescaler = 84 - 1;
  htim3.Init.ClockDivision = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  /* Start TIM Base Channel1 */
  if (HAL_TIM_Base_Start_IT(&htim3) != HAL_OK) {
    /* Starting Error */
    Error_Handler();
  }
}

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
