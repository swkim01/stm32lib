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
#ifndef ADC_H
#define ADC_H 100

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup stm32lib
 * @{
 */

/**
 * @defgroup ADC
 * @brief    STM32F4xx를 위한 ADC 라이브러리
 * @{
 *
 * ADC 라이브러리는 STM32Fxxx MCU 의 ADC 주변장치를 동작시키는 라이브러리이다.
 * 현재 STM32F0xx, STM32F4xx, STM32F7xx MCU 를 지원한다.
 *
 * \par 동작 방법
 * 
 * 현재 ADC 장치의 각 채널로부터 폴링 모드로 아날로그 데이터를 읽는다.
 * 따라서 데이터 읽기 속도가 저속일 때 사용 가능하며, 고속으로 데이터를
 * 읽으려면 DMA 방식이나 다중 읽기 모드로 프로그래밍해야 한다.
 * 또한 내부 온도센서 및 배터리전압을 최대 초 당 한 샘플 정도로 읽을 수 있다.
 *
 * \par ADC 외부 핀 배치
 *
\verbatim
채널      ADC1   ADC2   ADC3

0         PA0    PA0    PA0
1         PA1    PA1    PA1
2         PA2    PA2    PA2
3         PA3    PA3    PA3
4         PA4    PA4    PF6
5         PA5    PA5    PF7
6         PA6    PA6    PF8
7         PA7    PA7    PF9
8         PB0    PB0    PF10
9         PB1    PB1    PF3
10        PC0    PC0    PC0
11        PC1    PC1    PC1
12        PC2    PC2    PC2
13        PC3    PC3    PC3
14        PC4    PC4    PF4
15        PC5    PC5    PF5
\endverbatim
 *
 * \par 온도센서 및 배터리전압 읽기
 *
 * STM32 장치에는 내부 온도센서 및 배터리 전압을 읽는 회로가 포함되어 있다.
 *
 * 여기서  RTC 및 백업 SRAM을 위해 Vbat 핀에 연결된 배터리 전압을 측정한다.
 * 그런데, 온도센서 및 Vbat은 같은 ADC 채널에 연결되어 있을 수 있으므로
 * 한 번에 하나씩만 읽을 수 있다.
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
 * @defgroup ADC_매크로
 * @brief    ADC 관련 매크로
 * @{
 */

/**
 * @brief  기본 공급 전압 (mV)
 */
#ifndef ADC_SUPPLY_VOLTAGE
#define ADC_SUPPLY_VOLTAGE      3300
#endif

/**
 * @brief  VBAT 측정을 위한 Multipliers
 */
#if defined(STM32F405xx) || defined(STM32F407xx) || defined(STM32F415xx) || defined(STM32F417xx)
#define ADC_VBAT_MULTI			2
#else
#define ADC_VBAT_MULTI			4
#endif

/**
 * @brief  온도센서 측정을 위한 Avg_Slope
 */
#define ADC_AVG_SLOPE		2.5

/**
 * @brief  온도센서 측정을 위한 V25
 */
#define ADC_V25			760

/**
 * @}
 */
 
/**
 * @defgroup ADC_자료형
 * @brief    ADC 관련 자료형
 * @{
 */

/**
 * @}
 */

/**
 * @defgroup ADC_함수
 * @brief    ADC 관련 함수
 * @{
 */

/**
 * @brief  ADCx 장치를 초기화한다
 * @param  *ADCx: 초기화할 장치
 * @retval 없음
 */
void adc_initADC(ADC_TypeDef* ADCx);

/**
 * @brief  ADCx 장치와 채널을 초기화한다
 * @param  *ADCx: 초기화할 장치
 * @param  channel: ADCx 채널. 0부터 까지의 값 사용 가능
 * @retval 없음
 */
void adc_init(ADC_TypeDef* ADCx, uint8_t channel);

/**
 * @brief  ADCx 채널로부터 아날로그 데이터를 읽는다
 * @param  *ADCx: 읽을 ADCx 장치
 * @param  channel: 읽을 ADCx 채널. 0부터 까지의 값 사용 가능
 * @retval ADC 값
 */
uint16_t adc_read(ADC_TypeDef* ADCx, uint8_t channel);

/**
 * @brief  내부 온도 센서를 읽는다
 * @param  void
 * @retval 온도 값(C)
 */
float adc_readTempSensor(void);

/**
 * @brief  Vbat 핀 전압을 읽는다
 * @param  void
 * @retval 배터리 전압(mV)
 */
uint16_t adc_readVbat(void);

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
