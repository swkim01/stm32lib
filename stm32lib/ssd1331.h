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
#ifndef SSD1331_H
#define SSD1331_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup stm32lib
 * @{
 */

/**
 * @defgroup SSD1331
 * @brief    STM32F4xx를 위한 SSD1331 SPI LCD 드라이버 라이브러리
 * @{
 *
 * SSD1331 LCD 장치는 SPI 통신을 사용한다
 *
 * 이 라이브러리는 선, 사각형, 원을 그리는 기능을 물론
 * 영문/한글 글자를 쓰는 기능을 포함하고 있다.
 *
 * \par 기본 사용 핀들
 *
\verbatim
SSD1331    STM32F4xx    설명

VCC        3.3V         
GND        GND          
SCLK       D13(PA5)     SPI 를 위한 클럭 핀
MOSI       D11(PA7)     SPI 를 위한 데이터 핀
CS         D10(PB6)     장치 를 위한 칩 선택 핀
D/C        D9(PC7)      장치 를 위한 데이터/명령 선택 핀
RST        D8(PA9)      장치 를 위한 리셋 핀
\endverbatim
 *
 * 기본 핀을 바꾸려면 이 헤더 파일을 열고 다음과 같은 매크로 정의를 바꾸어야 한다.
 *
\code
//사용된 SPI
#define SSD1331_SPI              SPI_1
//사용된 SPI 핀
#define SSD1331_SPI_PINSPACK     SPI1_PINS1

//추가 핀들
#define SSD1331_CS_PIN           GPIO_PB_6
#define SSD1331_DC_PIN           GPIO_PC_7
#define SSD1331_RS_PIN           GPIO_PA_9

//LCD의 넓이 및 높이
#define SSD1331_WIDTH            96
#define SSD1331_HEIGHT           64
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
 - SPI
 - FONT
 - HANGULFONT
 - string.h
 - stdlib.h
\endverbatim
 */

#include "stm32f4xx_hal.h"
#include "../stm32lib/gpio.h"
#include "../stm32lib/spi.h"
#include "../stm32lib/font.h"
#include "../stm32lib/hangulfont.h"
#include "stdlib.h"
#include "string.h"

/**
 * @defgroup SSD1331_매크로
 * @brief    SSD1331 관련 매크로
 * @{
 */

/* SPI 설정 */
#ifndef SSD1331_SPI
#define SSD1331_SPI              SPI_1
#define SSD1331_SPI_PINSPACK     SPI1_PINS1
#endif

/* 장치 핀들 */
#ifndef SSD1331_CS_PIN
#define SSD1331_CS_PIN           GPIO_PB_6
#endif
#ifndef SSD1331_DC_PIN
#define SSD1331_DC_PIN           GPIO_PC_7
#endif
#ifndef SSD1331_RS_PIN
#define SSD1331_RS_PIN           GPIO_PA_9
#endif

/* SSD1331 설정 */
/* SSD1331 넓이 픽셀단위 */
#ifndef SSD1331_WIDTH
#define SSD1331_WIDTH            96
#endif
/* SSD1331 높이 픽셀단위 */
#ifndef SSD1331_HEIGHT
#define SSD1331_HEIGHT           64
#endif

/**
 * @}
 */
 
/**
 * @defgroup SSD1331_자료형
 * @brief    SSD1331 자료형
 * @{
 */

/**
 * @brief  SSD1331 색깔 자료형
 */
typedef uint32_t SSD1331_Color_t;
#define	SSD1331_COLOR_BLACK 0x000000
#define	SSD1331_COLOR_WHITE 0xFFFFFF
#define	SSD1331_COLOR_RED   0xFF0000
#define	SSD1331_COLOR_GREEN 0x00FF00
#define	SSD1331_COLOR_BLUE  0x0000FF
#define	SSD1331_COLOR_YELLOW 0x00FFFF
#define	SSD1331_COLOR_ORANGE 0xFFFF00
#define	SSD1331_COLOR_PURPLE 0xFF00FF

/**
 * @}
 */

/**
 * @defgroup SSD1331_함수
 * @brief    SSD1331 관련 함수
 * @{
 */

/**
 * @brief  SSD1331 그래픽 장치를 초기화한다
 * @param  없음
 * @retval 없음
 */
void ssd1331_init(void);

/** 
 * @brief  전체 그래픽 장치를 원하는 색으로 채운다
 * @param  color: 스크린 채움을 위해 사용될 색. 이 매개변수는 @ref SSD1331_Color_t 자료형 값
 * @retval 없음
 */
void ssd1331_fill(SSD1331_Color_t color);

/**
 * @brief  원하는 위치에 픽셀을 그린다
 * @param  x: X 위치. 이 매개변수는 0 과 SSD1331_WIDTH - 1 사이의 값
 * @param  y: Y 위치. 이 매개변수는 0 과 SSD1331_HEIGHT - 1 사이의 값
 * @param  color: 스크린 채움을 위해 사용될 색. 이 매개변수는 @ref SSD1331_Color_t 자료형 값
 * @retval 없음
 */
void ssd1331_drawpixel(uint16_t x, uint16_t y, SSD1331_Color_t color); 

/**
 * @brief  문자열을 위해 원하는 위치로 커서 포인터를 설정한다
 * @param  x: X 위치. 이 매개변수는 0 과 SSD1331_WIDTH - 1 사이의 값
 * @param  y: Y 위치. 이 매개변수는 0 과 SSD1331_HEIGHT - 1 사이의 값
 * @retval 없음
 */
void ssd1331_gotoxy(uint16_t x, uint16_t y);

/**
 * @brief  그래픽 장치에 글자를 쓴다
 * @param  ch: 쓰여질 글자
 * @param  *font: 사용된 폰트에 대한 @ref Font_t 구조체의 포인터
 * @param  color: 그리는 데 사용할 색. 이 매개변수는 @ref SSD1331_Color_t 자료형 값
 * @retval 쓰여진 글자 수(=1 or 0)
 */
char ssd1331_putc(uint16_t ch, Font_t *font, SSD1331_Color_t color, uint8_t size);
char ssd1331_putc_gfx(uint16_t ch, Font_t* font, SSD1331_Color_t color, uint8_t size);
char ssd1331_putc_hangul(uint16_t ch, Font_t* font, SSD1331_Color_t color, uint8_t size);

/**
 * @brief  그래픽 장치에 문자열을 쓴다
 * @param  *str: 쓰여질 문자열
 * @param  *fontset: 사용된 폰트셋에 대한 @ref FontSet_t 구조체의 포인터
 * @param  color: 그리는 데 사용할 색. 이 매개변수는 @ref SSD1331_Color_t 자료형 값
 * @retval 쓰여진 글자 수
 */
char ssd1331_puts(char* str, FontSet_t* fontset, SSD1331_Color_t color, uint8_t size);

/**
 * @brief  그래픽 장치에 선을 그린다
 * @param  x0: X 시작 위치. 이 매개변수는 0 과 SSD1331_WIDTH - 1 사이의 값
 * @param  y0: Y 시작 위치. 이 매개변수는 0 과 SSD1331_HEIGHT - 1 사이의 값
 * @param  x1: X 끝 위치. 이 매개변수는 0 과 SSD1331_WIDTH - 1 사이의 값
 * @param  y1: Y 끝 위치. 이 매개변수는 0 과 SSD1331_HEIGHT - 1 사이의 값
 * @param  color: 그리는 데 사용할 색. 이 매개변수는 @ref SSD1331_Color_t 자료형 값
 * @retval 없음
 */
void ssd1331_drawline(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SSD1331_Color_t c);

/**
 * @brief  그래픽 장치에 사각형을 그린다
 * @param  x: 가장 왼쪽 위 X 시작 위치. 이 매개변수는 0 과 SSD1331_WIDTH - 1 사이의 값
 * @param  y: 가장 왼쪽 위 Y 시작 위치. 이 매개변수는 0 과 SSD1331_HEIGHT - 1 사이의 값
 * @param  w: 픽셀 단위의 사각형 넓이
 * @param  h: 픽셀 단위의 사각형 높이
 * @param  color: 그리는 데 사용할 색. 이 매개변수는 @ref SSD1331_Color_t 자료형 값
 * @retval 없음
 */
void ssd1331_drawrectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1331_Color_t c);

/**
 * @brief  그래픽 장치에 사각형을 채운다
 * @param  x: 가장 왼쪽 위 X 시작 위치. 이 매개변수는 0 과 SSD1331_WIDTH - 1 사이의 값
 * @param  y: 가장 왼쪽 위 Y 시작 위치. 이 매개변수는 0 과 SSD1331_HEIGHT - 1 사이의 값
 * @param  w: 픽셀 단위의 사각형 넓이
 * @param  h: 픽셀 단위의 사각형 높이
 * @param  color: 채우는 데 사용할 색. 이 매개변수는 @ref SSD1331_Color_t 자료형 값
 * @retval 없음
 */
void ssd1331_fillrectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1331_Color_t c);

/**
 * @brief  그래픽 장치에 삼각형을 그린다
 * @param  x1: 첫째 X 좌표 위치. 이 매개변수는 0 과 SSD1331_WIDTH - 1 사이의 값
 * @param  y1: 첫째 Y 좌표 위치. 이 매개변수는 0 과 SSD1331_HEIGHT - 1 사이의 값
 * @param  x2: 둘째 X 좌표 위치. 이 매개변수는 0 과 SSD1331_WIDTH - 1 사이의 값
 * @param  y2: 둘째 Y 좌표 위치. 이 매개변수는 0 과 SSD1331_HEIGHT - 1 사이의 값
 * @param  x3: 셋째 X 좌표 위치. 이 매개변수는 0 과 SSD1331_WIDTH - 1 사이의 값
 * @param  y3: 셋째 Y 좌표 위치. 이 매개변수는 0 과 SSD1331_HEIGHT - 1 사이의 값
 * @param  color: 그리는 데 사용할 색. 이 매개변수는 @ref SSD1331_Color_t 자료형 값
 * @retval 없음
 */
void ssd1331_drawtriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1331_Color_t color);

/**
 * @brief  그래픽 장치에 삼각형을 채운다
 * @param  x1: 첫째 X 좌표 위치. 이 매개변수는 0 과 SSD1331_WIDTH - 1 사이의 값
 * @param  y1: 첫째 Y 좌표 위치. 이 매개변수는 0 과 SSD1331_HEIGHT - 1 사이의 값
 * @param  x2: 둘째 X 좌표 위치. 이 매개변수는 0 과 SSD1331_WIDTH - 1 사이의 값
 * @param  y2: 둘째 Y 좌표 위치. 이 매개변수는 0 과 SSD1331_HEIGHT - 1 사이의 값
 * @param  x3: 셋째 X 좌표 위치. 이 매개변수는 0 과 SSD1331_WIDTH - 1 사이의 값
 * @param  y3: 셋째 Y 좌표 위치. 이 매개변수는 0 과 SSD1331_HEIGHT - 1 사이의 값
 * @param  color: 채우는 데 사용할 색. 이 매개변수는 @ref SSD1331_Color_t 자료형 값
 * @retval 없음
 */
void ssd1331_filltriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1331_Color_t color);

/**
 * @brief  그래픽 장치에 원을 그린다
 * @param  x0: 원 중심에 대한 X 위치. 이 매개변수는 0 과 SSD1331_WIDTH - 1 사이의 값
 * @param  y0: 원 중심에 대한 Y 위치. 이 매개변수는 0 과 SSD1331_HEIGHT - 1 사이의 값
 * @param  r: 픽셀 단위의 원 반지름
 * @param  color: 그리는 데 사용할 색. 이 매개변수는 @ref SSD1331_Color_t 자료형 값
 * @retval 없음
 */
void ssd1331_drawcircle(int16_t x0, int16_t y0, int16_t r, SSD1331_Color_t c);

/**
 * @brief  그래픽 장치에 원을 채운다
 * @param  x0: 원 중심에 대한 X 위치. 이 매개변수는 0 과 SSD1331_WIDTH - 1 사이의 값
 * @param  y0: 원 중심에 대한 Y 위치. 이 매개변수는 0 과 SSD1331_HEIGHT - 1 사이의 값
 * @param  r: 픽셀 단위의 원 반지름
 * @param  color: 채우는 데 사용할 색. 이 매개변수는 @ref SSD1331_Color_t 자료형 값
 * @retval 없음
 */
void ssd1331_fillcircle(int16_t x0, int16_t y0, int16_t r, SSD1331_Color_t c);

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
