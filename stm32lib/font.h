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
#ifndef FONT_H
#define FONT_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup stm32lib
 * @{
 */

/**
 * @defgroup FONT
 * @brief    LCD 라이브러리를 위한 폰트 라이브러리
 * @{
 *
 * LCD 기반 라이브러리에 사용되는 기본 폰트 라이브러리.
 *
 * \par 지원 폰트들
 * 
 *  - 7 x 10 픽셀
 *  - 8 x 16 픽셀
 *  - 11 x 18 픽셀
 *  - 16 x 26 픽셀
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
#include "string.h"

/**
 * @defgroup FONT_매크로
 * @brief    FONT 관련 매크로
 * @{
 */
#define PROGMEM

/**
 * @defgroup FONT_자료형
 * @brief    FONT 자료형
 * @{
 */

/**
 * @brief 폰트 종류 열거형
 */
typedef enum {
    ASCII_FONT,
    GFX_FONT,
    COMB_FONT,
} FontType_t;

typedef struct {               /* Data stored PER GLYPH */
    uint32_t bitmapOffset;     /* Pointer into GFXfont->bitmap */
    uint8_t  width, height;    /* Bitmap dimensions in pixels */
    uint8_t  xAdvance;         /* Distance to advance cursor (x axis) */
    int8_t   xOffset, yOffset; /* Dist from cursor pos to UL corner */
} GFXglyph;

typedef struct {           /* Data stored for FONT AS A WHOLE: */
    uint8_t  *bitmap;      /* Glyph bitmaps, concatenated */
    GFXglyph *glyph;       /* Glyph array */
    uint16_t  first, last; /* ASCII extents */
    uint8_t   yAdvance;    /* Newline distance (y axis) */
} GFXFont_t;

typedef struct {           /* Data stored for FONT AS A WHOLE: */
    uint8_t  *bitmap;      /* Glyph bitmaps, concatenated */
    uint16_t  first, last; /* ASCII extents */
    uint8_t   yAdvance;    /* Newline distance (y axis) */
} CombFont_t;

/**
 * @brief  폰트 공용체
 */
typedef union {
    const uint16_t   *data; /*!< 폰트 데이터 배열에 대한 포인터 */
    const GFXFont_t  *gfxfont;
    const CombFont_t *combfont;
} FontData_t;

/**
 * @brief  LCD 라이브러리에 사용되는 폰트 구조체
 */
typedef struct {
    FontType_t type;        /*!< 폰트 타입 */
    uint8_t    width;       /*!< 픽셀 단위의 폰트 넓이 */
    uint8_t    height;      /*!< 픽셀 단위의 폰트 높이 */
    uint16_t   first, last; /* font range */
    FontData_t data;        /*!< 폰트 데이터 배열에 대한 포인터 */
} Font_t;

/** 
 * @brief  픽셀 단위의 문자열 넓이 및 높이
 */
typedef struct {
    uint16_t width;       /*!< 픽셀 단위의 문자열 넓이 */
    uint16_t height;      /*!< 픽셀 단위의 문자열 높이 */
} FontSize_t;

/**
 * @brief  LCD 라이브러리에 사용되는 폰트셋 구조체
 */
typedef struct {
    uint16_t width;       /*!< 픽셀 단위의 문자열 넓이 */
    uint16_t height;      /*!< 픽셀 단위의 문자열 높이 */
    Font_t  *fontlist[5]; /*!< 폰트 목록 */
} FontSet_t;

/**
 * @}
 */

/**
 * @defgroup FONT_폰트변수
 * @brief    FONT 폰트변수
 * @{
 */

/**
 * @brief  7 x 10 픽셀 폰트 구조체
 */
extern Font_t Font_7x10;

/**
 * @brief  8 x 16 픽셀 폰트 구조체
 */
extern Font_t Font_8x16;

/**
 * @brief  11 x 18 픽셀 폰트 구조체
 */
extern Font_t Font_11x18;

/**
 * @brief  16 x 26 픽셀 폰트 구조체
 */
extern Font_t Font_16x26;

/**
 * @brief  폰트셋 구조체들
 */
extern FontSet_t FontSet_10;
extern FontSet_t FontSet_16;
extern FontSet_t FontSet_18;
extern FontSet_t FontSet_26;

/**
 * @}
 */
 
/**
 * @defgroup FONT_함수
 * @brief    FONT 함수
 * @{
 */

/**
 * @brief  사용된 폰트에 따라 픽셀단위의 문자열 길이와 높이를 계산한다
 * @param  *str: 길이와 높이를 계산할 문자열
 * @param  *SizeStruct: 정보가 저잘될 빈 @ref FontSize_t 구조체의 포인터
 * @param  *fontset: 계산을 위해 사용될 @ref FontSet_t 폰트셋 구조체의 포인터
 * @retval 없음
 */
void font_getstringsize(char* str, FontSize_t* SizeStruct, FontSet_t* fontset);

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

