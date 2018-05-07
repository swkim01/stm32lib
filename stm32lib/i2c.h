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
#ifndef I2C_H
#define I2C_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif
	
/**
 * @addtogroup stm32lib
 * @{
 */

/**
 * @defgroup I2C
 * @brief    STM32F4xx를 위한 I2C 드라이버 라이브러리
 * @{
 *
 *	모든 I2C 버스 드라이버를 지원한다.
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
 * @defgroup I2C_매크로
 * @brief    I2C 관련 매크로
 * @{
 */

/**
 * @}
 */

/**
 * @defgroup I2C_자료형
 * @brief    I2C 초기화를 위해 사용되는 자료형
 * @{
 */
/**
 * @brief I2C 핀 열거형
 */
typedef enum {
#ifdef I2C1
    I2C_1  = (int)I2C1_BASE,
#endif
#ifdef I2C2
    I2C_2  = (int)I2C2_BASE,
#endif
#ifdef I2C3
    I2C_3  = (int)I2C3_BASE,
#endif
} I2C_t;

/**
 * @brief I2C 핀 자료형
 */
typedef struct I2C_Pins_t {
    I2C_t bus;  /*!< I2C_t */
    GPIO_Pin_t scl;  /*!< GPIO_Pin_t SCL pin */
    GPIO_Pin_t sda;  /*!< GPIO_Pin_t SDA pin */
    uint8_t alternate;
} I2C_Pins_t;

/**
 * @brief I2C 핀 묶음 열거형
 */
typedef enum {
    I2C1_PINS1 = 0,  /*!< I2C1, PB_6(SCL), PB_7(SDA) */
    I2C1_PINS2 = 1,  /*!< I2C1, PB_8(SCL), PB_9(SDA) */
    I2C1_PINS3 = 2,  /*!< I2C1, PB_6(SCL), PB_9(SDA) */
    I2C2_PINS1 = 3,  /*!< I2C2, PB_10(SCL), PB_11(SDA) */
    I2C3_PINS1 = 4,  /*!< I2C3, PA_8(SCL), PC_9(SDA) */
    I2C3_PINS2 = 5,  /*!< I2C3, PB_4(SCL), PC_9(SDA) */
} I2C_PinsPack_t;

/**
 * @}
 */

/**
 * @defgroup I2C_함수
 * @brief    I2C 관련 함수
 * @{
 */
 
/**
 * @brief  I2C 포트를 초기화한다
 * @note   이 함수는 I2C 포트에 대한 클럭을 활성화하고 gpio 핀들을 초기화한다
 * @param  I2C_Num: 초기화하려고 하는 I2C 번호
 * @param  pack: 초기화하려고 하는 I2C 핀 묶음
 * @retval 없음
 */
void i2c_init(I2C_t I2C_Num, I2C_PinsPack_t pack);

/**
 * @brief  I2C 포트를 해제한다
 * @param  I2C_Num: 선택할 I2C 번호
 * @retval 없음
 */
void i2c_deinit(I2C_t I2C_Num);

/**
 * @brief  I2C 핀에 대한 슬레이브 모드를 설정한다
 * @note   핀들은 먼저 @ref i2c_init() 함수를 사용하여 초기화되어야 한다
 * @param  I2C_Num: 선택할 I2C 번호
 * @param  slave: 마스터 또는 슬레이브 모드
 * @retval 없음
 */
void i2c_set_slave(I2C_t I2C_Num, int slave);

/**
 * @brief  I2C 버스에 대한 클록 주파수를 설정한다
 * @param  I2C_Num: 선택할 I2C 번호
 * @param  hz: SCLK 주파수(헤르쯔 단위)
 * @retval 없음
 */
void i2c_set_frequency(I2C_t I2C_Num, int hz);

/**
 * @brief  I2C 슬레이브에 쓴 후에 데이터를 읽는다
 * @param  I2C_Num: 선택할 I2C 번호
 * @param  address: I2C 장치 주소
 * @param  reg: 쓰여질 값
 * @param  data: 읽을 값을 저장하는 메모리 주소
 * @param  length: 읽을 데이터 바이트 수
 * @retval 실행 상태, 1: 정상, -1: 오류
 */
int i2c_nread(I2C_t I2C_Num, int address, int reg, uint8_t* data, int length);

/**
 * @brief  I2C 장치로부터 읽는다
 * @param  I2C_Num: 선택할 I2C 번호
 * @param  address: I2C 장치 주소
 * @param  reg: 쓰여질 값
 * @param  data: 읽을 값을 저장하는 메모리 주소
 * @param  length: 읽을 데이터 바이트 수
 * @retval 실행 상태, 1: 정상, -1: 오류
 */
int i2c_readdata(I2C_t I2C_Num, int address, uint8_t* data, int length);
int i2c_read(I2C_t I2C_Num, int address, uint8_t reg, uint8_t* data);
int i2c_read16(I2C_t I2C_Num, int address, uint16_t reg, uint8_t* data);

/**
 * @brief  I2C 슬레이브에 데이터를 쓴다
 * @param  I2C_Num: 선택할 I2C 번호
 * @param  address: I2C 장치 주소
 * @param  reg: 처음 쓰는 레지스터 값
 * @param  data: 쓸 데이터 값을 저장하는 메모리 주소
 * @param  length: 쓸 데이터 바이트 수
 * @retval 실행 상태, 1: 정상, -1: 오류
 */
int i2c_nwrite(I2C_t I2C_Num, int address, int reg, uint8_t *data, int length);
int i2c_write(I2C_t I2C_Num, int address, uint8_t reg, uint8_t data);
int i2c_write16(I2C_t I2C_Num, int address, uint16_t reg, uint8_t data);

/**
 * @brief  I2C 장치의 준비 상태를 읽는다
 * @param  I2C_Num: 선택할 I2C 번호
 * @param  address: I2C 장치 주소
 * @retval 준비 상태, 1: 정상, -1: 오류
 */
int i2c_ready(I2C_t I2C_Num, int address);

/**
 * @}
 */

/* Private macros ------------------------------------------------------------*/
/** @defgroup I2C_개별_매크로
  * @brief I2C 개별 매크로
  * @{
  */
#define IS_I2C_PINS_VALID(NUM,PINS) ((NUM==I2C_1 && (PINS==I2C1_PINS1 || PINS==I2C1_PINS2 || PINS==I2C1_PINS3)) || (NUM==I2C_2 && (PINS==I2C2_PINS1 || PINS==I2C2_PINS2 || PINS==I2C2_PINS3)) || (NUM==I2C_3 && (PINS==I2C3_PINS1 || PINS==I2C3_PINS2 || PINS==I2C3_PINS3)))

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
