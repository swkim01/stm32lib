/*
 *----------------------------------------------------------------------
 * Copyright (C) Seong-Woo Kim, 2016
 * 
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of
 * this software and associated documentation files
 * (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and
 * to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice
 * shall be included in all copies or substantial portions
 * of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *----------------------------------------------------------------------
 */
/** 
 * @author  Seong-Woo Kim, Aquilegia
 * @email   swkim01@gmail.com
 * @website http://cafe.naver.com/openiot
 * @link    http://cafe.naver.com/openiot/300
 * @version v1.0
 * @ide     sw4stm32
 * @license Open Source
 * @brief   STM32F4xx 장치를 위한 라이브러리
 *
 */
#ifndef GPIO_H
#define GPIO_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif
	
/**
 * @addtogroup stm32lib
 * @{
 */

/**
 * @defgroup GPIO
 * @brief    STM32F4xx를 위한 GPIO 라이브러리
 * @{
 *
 * GPIO 라이브러리는 GPIO 핀에 대해 사용되며, GPIO 핀을 초기화하고 입출력을 제어한다.
 *
 * 또한 기존의 SPD/HAL 드라이버를 대체할 수 있다.
 *
 * \par Changelog
 *
\verbatim
 버전 1.0
  - 최초 배포
\endverbatim
 *
 * \par 의존성
 *
\verbatim
 - STM32F4xx HAL
\endverbatim
 */
#include "stm32f4xx_hal.h"

/**
 * @defgroup GPIO_매크로
 * @brief    GPIO 관련 매크로
 * @{
 */

/**
 * @}
 */

/**
 * @defgroup GPIO_자료형
 * @brief    GPIO 초기화를 위해 사용되는 자료형
 * @{
 */

/**
 * @brief GPIO 핀 열거형
 */
typedef enum {
    GPIO_PA_0  = 0x00,
    GPIO_PA_1  = 0x01,
    GPIO_PA_2  = 0x02,
    GPIO_PA_3  = 0x03,
    GPIO_PA_4  = 0x04,
    GPIO_PA_5  = 0x05,
    GPIO_PA_6  = 0x06,
    GPIO_PA_7  = 0x07,
    GPIO_PA_8  = 0x08,
    GPIO_PA_9  = 0x09,
    GPIO_PA_10 = 0x0A,
    GPIO_PA_11 = 0x0B,
    GPIO_PA_12 = 0x0C,
    GPIO_PA_13 = 0x0D,
    GPIO_PA_14 = 0x0E,
    GPIO_PA_15 = 0x0F,

    GPIO_PB_0  = 0x10,
    GPIO_PB_1  = 0x11,
    GPIO_PB_2  = 0x12,
    GPIO_PB_3  = 0x13,
    GPIO_PB_4  = 0x14,
    GPIO_PB_5  = 0x15,
    GPIO_PB_6  = 0x16,
    GPIO_PB_7  = 0x17,
    GPIO_PB_8  = 0x18,
    GPIO_PB_9  = 0x19,
    GPIO_PB_10 = 0x1A,
    GPIO_PB_11 = 0x1B,
    GPIO_PB_12 = 0x1C,
    GPIO_PB_13 = 0x1D,
    GPIO_PB_14 = 0x1E,
    GPIO_PB_15 = 0x1F,

    GPIO_PC_0  = 0x20,
    GPIO_PC_1  = 0x21,
    GPIO_PC_2  = 0x22,
    GPIO_PC_3  = 0x23,
    GPIO_PC_4  = 0x24,
    GPIO_PC_5  = 0x25,
    GPIO_PC_6  = 0x26,
    GPIO_PC_7  = 0x27,
    GPIO_PC_8  = 0x28,
    GPIO_PC_9  = 0x29,
    GPIO_PC_10 = 0x2A,
    GPIO_PC_11 = 0x2B,
    GPIO_PC_12 = 0x2C,
    GPIO_PC_13 = 0x2D,
    GPIO_PC_14 = 0x2E,
    GPIO_PC_15 = 0x2F,

    GPIO_PD_2  = 0x3F,

    GPIO_PH_0  = 0x7F,
    GPIO_PH_1  = 0x7F,

    /* 아두이노 커넥터 이름 */
    GPIO_A0          = GPIO_PA_0,
    GPIO_A1          = GPIO_PA_1,
    GPIO_A2          = GPIO_PA_4,
    GPIO_A3          = GPIO_PB_0,
    GPIO_A4          = GPIO_PC_1,
    GPIO_A5          = GPIO_PC_0,
    GPIO_D0          = GPIO_PA_3,
    GPIO_D1          = GPIO_PA_2,
    GPIO_D2          = GPIO_PA_10,
    GPIO_D3          = GPIO_PB_3,
    GPIO_D4          = GPIO_PB_5,
    GPIO_D5          = GPIO_PB_4,
    GPIO_D6          = GPIO_PB_10,
    GPIO_D7          = GPIO_PA_8,
    GPIO_D8          = GPIO_PA_9,
    GPIO_D9          = GPIO_PC_7,
    GPIO_D10         = GPIO_PB_6,
    GPIO_D11         = GPIO_PA_7,
    GPIO_D12         = GPIO_PA_6,
    GPIO_D13         = GPIO_PA_5,

    /* 일반 신호 이름 */
    GPIO_LED1        = GPIO_PA_5,
    GPIO_LED2        = GPIO_PA_5,
    GPIO_LED3        = GPIO_PA_5,
    GPIO_LED4        = GPIO_PA_5,
    GPIO_USER_BUTTON = GPIO_PC_13,
    GPIO_SERIAL_TX   = GPIO_PA_2,
    GPIO_SERIAL_RX   = GPIO_PA_3,
    GPIO_USBTX       = GPIO_PA_2,
    GPIO_USBRX       = GPIO_PA_3,
    GPIO_I2C_SCL     = GPIO_PB_8,
    GPIO_I2C_SDA     = GPIO_PB_9,
    GPIO_SPI_MOSI    = GPIO_PA_7,
    GPIO_SPI_MISO    = GPIO_PA_6,
    GPIO_SPI_SCK     = GPIO_PA_5,
    GPIO_SPI_CS      = GPIO_PB_6,
    GPIO_PWM_OUT     = GPIO_PB_3,

    /* Not connected */
    GPIO_NC = (int)0xFFFFFFFF,
} GPIO_Pin_t;

/**
 * @brief GPIO 모드 열거형
 */
typedef enum {
    GPIO_IN   = 0x00, /*!< GPIO 핀을 범용 입력으로 설정 */
    GPIO_OUT  = 0x01, /*!< GPIO 핀을 범용 출력으로 설정 */
    GPIO_AF   = 0x02, /*!< GPIO 핀을 대체 기능으로 설정 */
    GPIO_OD   = 0x03, /*!< GPIO 핀을 출력 드레인으로 설정 */
    GPIO_IT   = 0x00010000, /*!< GPIO 핀을 인터럽트 소스로 설정 */
    GPIO_EVT  = 0x00020000, /*!< GPIO 핀을 이벤트 소스로 설정 */
    GPIO_EXTI = 0x10000000, /*!< GPIO 핀을 외부 인터럽트로 설정 */
} GPIO_Mode_t;

/**
 * @brief  GPIO 속도 열거형
 */
typedef enum {
    GPIO_SP_LOW = 0x00,    /*!< GPIO 느린 속도 */
    GPIO_SP_MEDIUM = 0x01, /*!< GPIO 중간 속도 */
    GPIO_SP_FAST = 0x02,   /*!< GPIO 빠른 속도, STM32F0xx 에서는 사용할 수 없음 */
    GPIO_SP_HIGH = 0x03    /*!< GPIO 높은 속도 */
} GPIO_Speed_t;

/**
 * @brief GPIO 풀(pull) 레지스터 열거형
 */
typedef enum {
    GPIO_PM_NOPULL = 0x00,    /*!< 풀 레지스터 없음 */
    GPIO_PM_PULLUP = 0x01,    /*!< 풀업 레지스터 활성화 */
    GPIO_PM_PULLDOWN = 0x02,  /*!< 풀다운 레지스터 활성화 */
    GPIO_PM_OPENDRAIN = 0x03  /*!< 오픈 드레인 활성화 */
} GPIO_PullMode_t;

/**
 * @}
 */

/**
 * @defgroup GPIO_함수
 * @brief    GPIO 관련 함수
 * @{
 */
 
/**
 * @brief  GPIO 핀을 초기화한다
 * @note   이 함수는 포트에 대한 클럭을 활성화한다
 * @param  GPIO_Pin: 초기화를 위해 사용할 GPIO 핀
 * @param  GPIO_Mode: 선택할  GPIO 모드. 이 매개변수는 @ref GPIO_Mode_t 값.
 * @retval 없음
 */
void gpio_init(GPIO_Pin_t GPIO_Pin, GPIO_Mode_t GPIO_Mode);

/**
 * @brief  GPIO 핀을 대체모드를 포함하여 초기화한다
 * @note   이 함수는 포트에 대한 클럭을 활성화한다
 * @param  GPIO_Pin: 초기화를 위해 사용할 GPIO 핀
 * @param  GPIO_Mode: 선택할 GPIO 모드. 이 매개변수는 @ref GPIO_Mode_t 값.
 * @param  GPIO_Alternate: 초기화를 위한 대체 기능
 * @retval 없음
 */
void gpio_alternate_init(GPIO_Pin_t GPIO_Pin, GPIO_Mode_t GPIO_Mode, uint8_t GPIO_Alternate);

/**
 * @brief  GPIO 핀을 해제한다
 * @param  GPIO_Pin: 선택할 GPIO 핀
 * @retval 없음
 */
void gpio_deinit(GPIO_Pin_t GPIO_Pin);

/**
  * @brief  GPIO 핀에 풀(pull) 레지스터를 설정한다.
  * @note   먼저 @ref GPIO_Init() 함수를 사용하여 초기화되어야 한다.
  * @param  GPIO_Pin: 선택할 GPIO 핀.
  * @param  GPIO_PullMode: 풀 레지스터 모드. 이 매개변수는 @ref GPIO_PullMode_t 열거형 값.
  * @retval 없음
  */
void gpio_set_pinmode(GPIO_Pin_t GPIO_Pin, GPIO_PullMode_t GPIO_PullMode);

/**
  * @brief  선택된 데이터 포트 값을 1로 설정하거나 8으로 클리어한다.
  * @param  GPIO_Pin: 쓸 포트 핀.
  * @param  PinState: 선택된 포트 핀에 쓰여질 값. 다음 값들 중 하나임:
  *            @arg 0 또는 GPIO_PIN_RESET: 포트 핀을 클리어
  *            @arg 1 또는 GPIO_PIN_SET: 포트 핀을 셋
  * @retval 없음
  */
void gpio_write(GPIO_Pin_t GPIO_Pin, uint16_t PinState);

/**
  * @brief  선택된 핀을 토글한다.
  * @param  GPIO_Pin: 토글할 포트 핀.
  * @retval 없음
  */
void gpio_toggle(GPIO_Pin_t GPIO_Pin);

/**
  * @brief  선택된 입력 포트 핀을 읽는다.
  * @param  GPIO_Pin: 읽을 포트 핀.
  * @retval 입력 포트 핀 값(0 또는 1).
  */
uint16_t gpio_read(GPIO_Pin_t GPIO_Pin);

/** 
 * @brief  라이브러리에서 사용 중이며 해제되지 않은 핀들을 획득한다.
 * @param  *GPIOx: 조사할 사용 중인 GPIO 핀의 포트에 대한 포인터
 * @retval 사용 중인 핀에 대한 비트 값들
 */
uint16_t gpio_get_usedpins(GPIO_TypeDef* GPIOx);

/** 
 * @brief  라이브러리에서 사용하지 않거나 해제된 핀들을 획득한다.
 * @param  *GPIOx: 조사할 사용 중인 GPIO 핀의 포트에 대한 포인터
 * @retval 사용하고 있지 않은 핀에 대한 비트 값들
 */
uint16_t gpio_get_freepins(GPIO_TypeDef* GPIOx);

/**
 * @}
 */

/* Private macros ------------------------------------------------------------*/
/** @defgroup GPIO_개별_매크로
  * @brief GPIO 개별 매크로
  * @{
  */
#define IS_GPIO_PIN_NUMBER(PIN)         (PIN & ~0x7F)
#define IS_GPIO_PIN_OUTPUT(PIN)         (gpio_outputpins[gpio_get_port_offset(PIN)] & GPIO_REG_VALUE(PIN))
#define IS_GPIO_PIN_INPUT(PIN)          (gpio_inputpins[gpio_get_port_offset(PIN)] & GPIO_REG_VALUE(PIN))
#define GPIO_REG_VALUE(PIN)             (1 << (PIN & 0xF))
#define GPIO_PIN_VALUE(PIN)             (PIN & 0xF)

/**
 * @brief  원하는 GPIOx 포트로부터 포트 소스 획득
 * @param  GPIOx: 포트 소스를 계산하기 위한 GPIO 포트
 * @retval GPIO 에 대해 계산된 포트 소스
 */
uint16_t gpio_get_port_source(GPIO_TypeDef* GPIOx);

/**
 * @brief  원하는 GPIO 핀으로부터 포트 오프셋 획득
 * @param  GPIO_Pin: 포트 오프셋을 계산하기 위한 GPIO 핀
 * @retval GPIO 에 대해 계산된 포트 오프셋
 */
uint8_t gpio_get_port_offset(GPIO_Pin_t GPIO_Pin);

/**
 * @brief  원하는 GPIO 핀으로부터 포트 베이스 주소 획득
 * @param  GPIO_Pin: 포트 베이스 주소를 계산하기 위한 GPIO 핀
 * @retval GPIO 에 대해 계산된 포트 베이스 주소
 */
GPIO_TypeDef* gpio_get_port_base(GPIO_Pin_t GPIO_Pin);

/**
 * @}
 */
/**
 * @}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
