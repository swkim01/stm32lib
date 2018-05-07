/*
 *----------------------------------------------------------------------
 * Copyright (C) Seong-Woo Kim, 2018
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
#ifndef HANGULFONTS_H
#define HANGULFONTS_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup stm32lib
 * @{
 */

/**
 * @defgroup HANGULFONT
 * @brief    LCD 라이브러리를 위한 한글 폰트 라이브러리
 * @{
 *
 * LCD 기반 라이브러리에 사용되는 기본 한글 폰트 라이브러리.
 *
 * \par 지원 한글 폰트
 * 
 *  - 16 x 16 픽셀
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
 - string.h
\endverbatim
 */
#include "stm32f4xx_hal.h"
#include "../stm32lib/font.h"
#include "string.h"

/**
 * @defgroup HANGULFONT_매크로
 * @brief    HANGULFONT 매크로
 * @{
 */
#define LOAD_COMBINE_FONT_16
//#define LOAD_NANUM_GOTHIC_FONT_16

/**
 * @}
 */

/**
 * @defgroup HANGULFONT_자료형
 * @brief    HANGULFONT 자료형
 * @{
 */

/**
 * @}
 */

/**
 * @defgroup HANGULFONT_폰트변수
 * @brief    HANGULFONT 폰트변수
 * @{
 */

/**
 * @brief  16 픽셀 한글 폰트 구조체
 */
extern Font_t CombineFont_16x16;
extern Font_t NanumGothicFont_16x16;

/**
 * @}
 */
 
/**
 * @defgroup HANGULFONT_함수
 * @brief    HANGULFONT 함수
 * @{
 */

/**
 * @brief  사용된 폰트와 한글 초중성에 따라 한글 글자를 획득한다
 * @param  han1: 사용된 첫째 한글(초성) 코드
 * @param  han2: 사용된 둘째 한글(중성) 코드
 * @param  han3: 사용된 셋째 한글(종성) 코드
 * @param  *Font: 계산을 위해 사용된 @ref Font_t font 폰트 구조체의 포인터
 * @retval 한글 글자를 위한 포인터
 */
uint8_t *get_hangul_glyph(uint16_t ch, Font_t *font);

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

