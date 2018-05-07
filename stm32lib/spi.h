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
#ifndef SPI_H
#define SPI_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif
	
/**
 * @addtogroup stm32lib
 * @{
 */

/**
 * @defgroup SPI
 * @brief    STM32F4xx를 위한 SPI 드라이버 라이브러리
 * @{
 *
 *	모든 SPI 버스 드라이버를 지원한다.
 *
 * It features fast initialization methods as well pin input/output methods.
 *
 * It can be used as replacement for STD/HAL drivers SPI library.
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
 - GPIO
\endverbatim
 */
#include "stm32f4xx_hal.h"
#include "../stm32lib/gpio.h"

/**
 * @defgroup SPI_매크로
 * @brief    SPI 관련 매크로
 * @{
 */

/**
 * @}
 */

/**
 * @defgroup SPI_자료형
 * @brief    SPI 초기화를 위해 사용되는 자료형
 * @{
 */
/**
 * @brief SPI 핀 열거형
 */
typedef enum {
#ifdef SPI1
    SPI_1  = (int)SPI1_BASE,
#endif
#ifdef SPI2
    SPI_2  = (int)SPI2_BASE,
#endif
#ifdef SPI3
    SPI_3  = (int)SPI3_BASE,
#endif
#ifdef SPI4
    SPI_4  = (int)SPI4_BASE,
#endif
#ifdef SPI5
    SPI_5  = (int)SPI5_BASE,
#endif
#ifdef SPI6
    SPI_6  = (int)SPI6_BASE,
#endif
} SPI_t;

/**
 * @brief SPI 모드 자료형
 */
typedef enum {
    SPI_Mode0 = 0x00,  /*!< Clock Polarity Low, Clock Phase 1st Edge */
    SPI_Mode1 = 0x01,  /*!< Clock Polarity Low, Clock Phase 2nd Edge */
    SPI_Mode2 = 0x02,  /*!< Clock Polarity High, Clock Phase 1st Edge */
    SPI_Mode3 = 0x03,  /*!< Clock Polarity High, Clock Phase 2nd Edge */
} SPI_Mode_t;

/**
 * @brief SPI 핀 자료형
 */
typedef struct stm32_spi_pins_t {
    SPI_t bus;        /*!< SPI_t */
    GPIO_Pin_t mosi;  /*!< GPIO_Pin_t MOSI pin */
    GPIO_Pin_t miso;  /*!< GPIO_Pin_t MISO pin */
    GPIO_Pin_t sclk;  /*!< GPIO_Pin_t SCLK pin */
    uint8_t alternate;
} SPI_Pins_t;

/**
 * @brief SPI 핀 묶음 열거형
 */
typedef enum {
    SPI1_PINS1 = 0,  /*!< SPI1, PA_7(MOSI), PA_6(MISO), PA_5(SCLK) */
    SPI1_PINS2 = 1,  /*!< SPI1, PB_5(MOSI), PB_4(MISO), PB_3(SCLK) */
    SPI2_PINS1 = 2,  /*!< SPI2, PB_14(MOSI), PB_13(MISO), PB_12(SCLK) */
    SPI2_PINS2 = 3,  /*!< SPI2, PC_3(MOSI), PC_2(MISO), PB_10(SCLK) */
    SPI3_PINS1 = 4,  /*!< SPI3, PB_5(MOSI), PB_4(MISO), PB_3(SCLK) */
    SPI3_PINS2 = 5,  /*!< SPI3, PC_12(MOSI), PC_11(MISO), PC_10(SCLK) */
} SPI_PinsPack_t;

/**
 * @}
 */

/**
 * @defgroup SPI_함수
 * @brief    SPI 관련 함수
 * @{
 */
 
/**
 * @brief  SPI 포트를 초기화한다
 * @note   이 함수는 SPI 포트에 대한 클럭을 활성화하고 gpio 핀들을 초기화한다
 * @param  SPI_Num: 초기화하려고 하는 SPI 번호
 * @param  pack: 초기화하려고 하는 SPI 핀 묶음
 * @retval 없음
 */
void spi_init(SPI_t SPI_Num, SPI_PinsPack_t pack);

/**
 * @brief  SPI 포트를 해제한다
 * @note   클럭이 비활성화될 것이다
 * @param  SPI_Num: 선택할 SPI 번호
 * @retval 없음
 */
void spi_deinit(SPI_t SPI_Num);

/**
 * @brief  Sets format settings to SPI pin(s)
 * @note   Pins HAVE to be initialized first using @ref SPI_Init() function
 * @param  SPI_Num: Select SPI Num.
 * @param  SPI_Mode: Clock polarity & phase option. This parameter can be a value of @ref SPI_Mode_t enumeration
 * @param  slave: master or slave mode.
 * @param  bits: Number of bits per SPI frame (4 - 16)
 * @retval None
 */
void spi_set_format(SPI_t SPI_Num, SPI_Mode_t mode, int slave, int bits);

/**
 * @brief  Set the clock frequency for the SPI bus.
 * @param  SPI_Num: Select SPI Num.
 * @param  hz: SCLK frequency in hz
 * @retval None
 */
//void gpio_write(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint16_t PinState);
void spi_set_frequency(SPI_t SPI_Num, int hz);

/**
 * @brief  Write the SPI Slave and return the response.
 * @param  SPI_Num: Select SPI Num.
 * @param  value: value to be written.
 * @retval response value to be read
 */
int spi_writeread(SPI_t SPI_Num, int value);

/**
 * @brief  Reads the specified spi bus.
 * @param  SPI_Num: Select SPI Num.
 * @retval The input value to be read.
 */
int spi_read(SPI_t SPI_Num);

/**
 * @brief  Write the SPI Slave.
 * @param  SPI_Num: Select SPI Num.
 * @param  value: value to be written.
 * @retval None
 */
void spi_write(SPI_t SPI_Num, int value);

/**
 * @brief  Reads the busy state of spi bus.
 * @param  SPI_Num: Select SPI Num.
 * @retval The busy state. 1 or 0
 */
int spi_busy(SPI_t SPI_Num);

/**
 * @}
 */

/* Private macros ------------------------------------------------------------*/
/** @defgroup SPI_개별_매크로
  * @brief SPI 개별 매크로
 * @{
 */
#define IS_SPI_PINS_VALID(NUM,PINS)         ((NUM==SPI_1 && (PINS==SPI1_PINS1 || PINS==SPI1_PINS2)) || (NUM==SPI_2 && (PINS==SPI2_PINS1 || PINS==SPI2_PINS2)) || (NUM==SPI_3 && (PINS==SPI3_PINS1 || PINS==SPI3_PINS2)))

/**
 * @}
 */

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
