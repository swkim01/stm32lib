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
#ifndef PWM_H
#define PWM_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup stm32lib
 * @{
 */

/**
 * @defgroup PWM
 * @brief    STM32F4xx를 위한 PWM 라이브러리
 * @{
 *
 * PWM 라이브러리는 STM32Fxxx MCU 의 TIM 주변장치를 PWM 모드로 동작시키는 라이브러리이다.
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
 * @defgroup PWM_매크로
 * @brief    PWM 관련 매크로
 * @{
 */

 /**
 * @}
 */

/**
 * @defgroup PWM_자료형
 * @brief    PWM 관련 자료형
 * @{
 */

/**
 * @brief PWM 타이머 열거형
 */
typedef enum {
    PWM_1  = (int)TIM1_BASE,
    PWM_2  = (int)TIM2_BASE,
    PWM_3  = (int)TIM3_BASE,
    PWM_4  = (int)TIM4_BASE,
    PWM_5  = (int)TIM5_BASE,
    PWM_6  = (int)TIM6_BASE,
    PWM_7  = (int)TIM7_BASE,
    PWM_8  = (int)TIM8_BASE,
    PWM_9  = (int)TIM9_BASE,
    PWM_10  = (int)TIM10_BASE,
    PWM_11  = (int)TIM11_BASE,
#ifdef TIM12
    PWM_12  = (int)TIM12_BASE,
#endif
#ifdef TIM13
    PWM_13  = (int)TIM13_BASE,
#endif
#ifdef TIM14
    PWM_14  = (int)TIM14_BASE,
#endif
} PWM_t;

/**
 * @brief PWM 핀 구조체
 */
typedef struct pwm_pin_t {
    PWM_t tim;  /*!< PWM_t */
    GPIO_Pin_t gpio;  /*!< GPIO_Pin_t gpio_pin */
    uint8_t alternate;
    uint8_t channel;
    uint8_t invert;
    uint32_t period;
    uint32_t pulse;
    uint32_t prescaler;
} PWM_Pin_t;

/**
 * @brief PWM 핀 번호 열거형
 */
typedef enum {
    PWM_PIN1 = 0,  /*!< PWM_2, PA_0, GPIO_AF1_TIM2, TIM2_CH1 */
    PWM_PIN2 = 1,  /*!< PWM_2, PA_1, GPIO_AF1_TIM2, TIM2_CH2 */
    PWM_PIN3 = 2,  /*!< PWM_2, PA_2, GPIO_AF1_TIM2, TIM2_CH3 */
    PWM_PIN4 = 3,  /*!< PWM_2, PA_3, GPIO_AF1_TIM2, TIM2_CH4 */
    PWM_PIN5 = 4,  /*!< PWM_2, PA_5, GPIO_AF1_TIM2, TIM2_CH1 */
    PWM_PIN6 = 5,  /*!< PWM_3, PA_6, GPIO_AF2_TIM3, TIM3_CH1 */
    PWM_PIN7 = 6,  /*!< PWM_1, PA_7, GPIO_AF1_TIM1, TIM3_CH1N */
    PWM_PIN8 = 7,  /*!< PWM_1, PA_8, GPIO_AF1_TIM1, TIM1_CH1 */
    PWM_PIN9 = 8,  /*!< PWM_1, PA_9, GPIO_AF1_TIM1, TIM1_CH2 */
    PWM_PIN10 = 9,  /*!< PWM_1, PA_10, GPIO_AF1_TIM1, TIM1_CH3 */
    PWM_PIN11 = 10,  /*!< PWM_1, PA_11, GPIO_AF1_TIM1, TIM1_CH4 */
    PWM_PIN12 = 11,  /*!< PWM_2, PA_15, GPIO_AF1_TIM2, TIM2_CH1 */
    PWM_PIN13 = 12,  /*!< PWM_1, PB_0, GPIO_AF1_TIM1, TIM1_CH2N */
    PWM_PIN14 = 13,  /*!< PWM_1, PB_1, GPIO_AF1_TIM1, TIM1_CH3N */
    PWM_PIN15 = 14,  /*!< PWM_2, PB_2, GPIO_AF1_TIM2, TIM2_CH4 */
    PWM_PIN16 = 15,  /*!< PWM_2, PB_3, GPIO_AF1_TIM2, TIM2_CH2 */
    PWM_PIN17 = 16,  /*!< PWM_3, PB_4, GPIO_AF2_TIM3, TIM3_CH1 */
    PWM_PIN18 = 17,  /*!< PWM_3, PB_5, GPIO_AF2_TIM3, TIM3_CH2 */
    PWM_PIN19 = 18,  /*!< PWM_4, PB_6, GPIO_AF2_TIM4, TIM4_CH1 */
    PWM_PIN20 = 19,  /*!< PWM_4, PB_7, GPIO_AF2_TIM4, TIM4_CH2 */
    PWM_PIN21 = 20,  /*!< PWM_4, PB_8, GPIO_AF2_TIM4, TIM4_CH3 */
    PWM_PIN22 = 21,  /*!< PWM_4, PB_9, GPIO_AF2_TIM4, TIM4_CH4 */
    PWM_PIN23 = 22,  /*!< PWM_2, PB_10, GPIO_AF1_TIM2, TIM2_CH3 */
    PWM_PIN24 = 23,  /*!< PWM_1, PB_13, GPIO_AF1_TIM1, TIM1_CH1N */
    PWM_PIN25 = 24,  /*!< PWM_1, PB_14, GPIO_AF1_TIM1, TIM1_CH2N */
    PWM_PIN26 = 25,  /*!< PWM_1, PB_15, GPIO_AF1_TIM1, TIM1_CH3N */
    PWM_PIN27 = 26,  /*!< PWM_3, PC_6, GPIO_AF2_TIM3, TIM3_CH1 */
    PWM_PIN28 = 27,  /*!< PWM_3, PC_7, GPIO_AF2_TIM3, TIM3_CH2 */
    PWM_PIN29 = 28,  /*!< PWM_3, PC_8, GPIO_AF2_TIM3, TIM3_CH3 */
    PWM_PIN30 = 29,  /*!< PWM_3, PC_9, GPIO_AF2_TIM3, TIM3_CH4 */
} PWM_PinIndex_t;

/**
 * @}
 */

/**
 * @defgroup PWM_함수
 * @brief    PWM 관련 함수
 * @{
 */

/** PWM 장치와 출력 핀을 초기화한다
 *
 * @param index 초기화할 PWM 핀 번호
 */
void pwm_init(PWM_PinIndex_t index);

/** PWM 장치와 출력 핀을 해제한다
 *
 * @param index 해제할 PWM 핀 번호
 */
void pwm_deinit(PWM_PinIndex_t index);

/** PWM 출력 듀티 싸이클을 <0.0f, 1.0f> 범위로 설정한다
 *0.0f은 퍼센트, 1.0f은 100 퍼센트를 나타낸다
 *
 * @param index   PWM 핀 번호
 * @param percent 퍼센트를 나타내는 부동소수점 수
 */
void pwm_write(PWM_PinIndex_t index, float percent);

/** Read the current float-point output duty-cycle
현재의 출력 듀티 싸이클 부동소수점 값을 읽는다
 *
 * @param index PWM 핀 번호
 * @return 출력 듀티 싸이클을 나타내는 부동소수점 수
 */
float pwm_read(PWM_PinIndex_t index);

/** 듀티 싸이클은 유지한 채로 주기를 초 단위로 설정한다.
 * 마이크로초(최저 분해능)보다 더 작은 주기는 영으로 설정한다
 *
 * @param index   PWM 핀 번호
 * @param seconds 주기(초)를 나타내는 부동소수점 수
 */
void pwm_period(PWM_PinIndex_t index, float seconds);

/** 듀티 싸이클은 유지한 채로 주기를 밀리초 단위로 설정한다.
 *
 * @param index PWM 핀 번호
 * @param ms    밀리초 주기
 */
void pwm_period_ms(PWM_PinIndex_t index, int ms);

/** 듀티 싸이클은 유지한 채로 주기를 마이크로초 단위로 설정한다.
 *
 * @param index PWM 핀 번호
 * @param us    마이크로초 주기
 */
void pwm_period_us(PWM_PinIndex_t index, int us);

/** 주기는 유지한 채로 펄스 폭을 초 단위로 설정한다.
 *
 * @param index   PWM 핀 번호
 * @param seconds 펄스 폭(초)을 나타내는 부동소수점 수
 */
void pwm_pulsewidth(PWM_PinIndex_t index, float seconds);

/** 주기는 유지한 채로 펄스 폭을 밀리초 단위로 설정한다.
 *
 * @param index PWM 핀 번호
 * @param ms    펄스 폭(밀리초)을 나타내는 부동소수점 수
 */
void pwm_pulsewidth_ms(PWM_PinIndex_t index, int ms);

/** 주기는 유지한 채로 펄스 폭을 마이크로초 단위로 설정한다.
 *
 * @param index PWM 핀 번호
 * @param ms    펄스 폭(마이크로초)을 나타내는 부동소수점 수
 */
void pwm_pulsewidth_us(PWM_PinIndex_t index, int us);

/**
 * @}
 */

/* Private macros ------------------------------------------------------------*/
/** @defgroup PWM_개별_매크로
  * @brief PWM 개별 매크로
  * @{
  */
#define IS_PWM_PIN_INDEX(array, INDEX)         (INDEX >= 0 && INDEX < sizeof(array)/sizeof(PWM_Pin_t))

/**
 * @}
 */
 
/**
 * @}
 */
 
/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif
