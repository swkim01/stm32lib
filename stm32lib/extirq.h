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
#ifndef EXTIRQ_H
#define EXTIRQ_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup stm32lib
 * @{
 */

/**
 * @defgroup EXTIRQ
 * @brief    STM32F4xx를 위한 외부 인터럽트 라이브러리
 * @{
 *
 * 이 라이브러리는 특정한 핀의 외부 인터럽트를 사용하도록 해 준다.
 *
 * 인터럽트를 사용하려는 핀과 트리거 모드를 지정하여 핀을 초기화하고
 * 인터럽트를 처리할 함수를 작성하고 지정해야 한다.
 *
 * 총 16개의 외부 인터럽트가 있는데, 0번부터 15번까지는 각각 GPIOA~GPIOH포트의 각각의 해당하는 외부 핀들에 대해 연결된다.
 * 예를 들면, 0번 인터럽트 EXTI0은 PA0, PB0, …, PH0 핀에 연결되는데 한 번에 하나의 핀만 사용할 수 있다.
 *
 * 인터럽트 처리를 위한 인터럽트 핸들러는 16개가 아니라 7개이다.
 * 여기서, EXTI0부터 EXTI4 까지는 인터럽트 핸들러가 각각 하나이지만,
 * EXTI5~EXTI9는 전체가 EXTI9_5 인터럽트 핸들러 하나를 공유하고
 * EXTI10~EXTI15는 전체가 EXTI15_10 하나의 인터럽트 핸들러를 공유한다.
 *
 * 만약 외부 인터럽트에 대한 우선순위를 높이고 싶으면, extirq.h 파일을 편집하여 다음 줄을 고쳐야 한다.
\code
#define EXTI_NVIC_PRIORITY     0x04
\endcode
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
 * @defgroup EXTIRQ_매크로
 * @brief    EXTIRQ 관련 매크로
 * @{
 */

/**
 * @brief  NVIC에 사용된 인터럽트 선에 대한 기본 EXTI 우선순위
 */
#ifndef EXTI_NVIC_PRIORITY
#define EXTI_NVIC_PRIORITY     0x04
#endif

 /**
 * @}
 */

/**
 * @defgroup EXTIRQ_자료형
 * @brief    EXTIRQ 관련 자료형
 * @{
 */

/**
 * @brief  결과 열거형
 */
typedef enum {
	EXTIRQ_RES_OK = 0x00, /*!< 정상 동작 */
	EXTIRQ_RES_ERR        /*!< 오류 발생 */
} EXTIRQ_Res_t;

/**
 * @brief  인터럽트 트리거 열거형
 */
typedef enum {
	EXTIRQ_RISING_EDGE = 0x00, /*!< 라이징(rising) 에지에서 인터럽트 트리거, 풀다운 레지스터가 활성화됨 */
	EXTIRQ_FALLING_EDGE,       /*!< 폴링(falling) 에지에서 인터럽트 트리거, 풀업 레지스터가 활성화됨 */
	EXTIRQ_BOTH_EDGE           /*!< 모든 에지에서 인터럽트 트리거, 풀 레지스터가 설정되지 않음 */
} EXTIRQ_Edge_t;

/**
 * @brief  인터럽트 콜백 함수형
 */
typedef void (* EXTIRQ_Callback_t)(int i, int edge);

/**
 * @}
 */

/**
 * @defgroup EXTIRQ_함수
 * @brief    EXTIRQ 관련 함수
 * @{
 */

/**
 * @brief  특정한 핀에 대해 콜백 함수와 함께 외부 인터럽트를 초기화한다
 * @note   이 함수는 GPIO 핀에 대한 클럭을 활성화하고 사용하는 트리거에 맞는 풀 레지스터를 설정한다.
 * 	          - 폴링(falling) 에지: 풀업이 활성화됨
 * 	          - 라이징(rising) 에지: 풀다운이 활성화됨
 * 	          - 모든 에지: 풀이 설정되지 않음
 *
 * @param  GPIO_Pin: EXTI 인터럽트 선에 대한 GPIO 핀
 * @param  callback: EXTI 인터럽트 선에 신호가 들어올 때 실행할 콜백 함수
 * @param  edge: 핀 트리거 에지 소스. 이 매개변수는 열거형.
 * @retval 결과값:
 *            - @arg EXTIRQ_RES_OK: 정상 동작되어 인터럽트가 설정됨
 *            - @arg EXTIRQ_RES_ERR: 오류 발생되어 인터럽트가 설정되지 않음
 */
EXTIRQ_Res_t extirq_init(GPIO_Pin_t GPIO_Pin, EXTIRQ_Callback_t callback, EXTIRQ_Edge_t edge);

/**
 * @brief  인터럽트 선으로부터 GPIO 핀을 해제한다.
 * @param  GPIO_Pin: 비활성화할 GPIO 핀.
 */
void extirq_free(GPIO_Pin_t GPIO_Pin);

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
