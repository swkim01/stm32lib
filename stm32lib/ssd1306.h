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
#ifndef SSD1306_H
#define SSD1306_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup stm32lib
 * @{
 */

/**
 * @defgroup SSD1306
 * @brief    STM32F4xx를 위한 SSD1306 I2C LCD 드라이버 라이브러리
 * @{
 *
 * SSD1306 LCD 장치는 I2C 통신을 사용한다
 *
 * 이 라이브러리는 선, 사각형, 원을 그리는 기능을 물론
 * 영문/한글 글자를 쓰는 기능을 포함하고 있다.
 *
 * \par 기본 사용 핀들
 *
\verbatim
SSD1306    STM32F4xx    설명

VCC        3.3V         
GND        GND          
SCL        PB8          I2C 를 위한 클럭 핀
SDA        PB9          I2C 를 위한 데이터 핀
\endverbatim
 *
 * 기본 핀을 바꾸려면 이 헤더 파일을 열고 다음과 같은 매크로 정의를 바꾸어야 한다.
 *
\code
//사용된 I2C
#define SSD1306_I2C              I2C_1
//사용된 I2C 핀
#define SSD1306_I2C_PINSPACK     I2C1_PINS2

//I2C 장치 주소
#define SSD1306_I2C_ADDR         0x78

//LCD의 넓이 및 높이
#define SSD1306_WIDTH            128
#define SSD1306_HEIGHT           64
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
 - I2C
 - FONT
 - HANGULFONT
 - string.h
 - stdlib.h
\endverbatim
 */

#include "stm32f4xx_hal.h"
#include "../stm32lib/i2c.h"
#include "../stm32lib/font.h"
#include "../stm32lib/hangulfont.h"
#include "stdlib.h"
#include "string.h"

/**
 * @defgroup SSD1306_매크로
 * @brief    SSD1306 관련 매크로
 * @{
 */

/* I2C 설정 */
#ifndef SSD1306_I2C
#define SSD1306_I2C              I2C_1
#define SSD1306_I2C_PINSPACK     I2C1_PINS2
#endif

/* I2C 주소 */
#ifndef SSD1306_I2C_ADDR
#define SSD1306_I2C_ADDR         0x78
#endif

/* SSD1306 설정 */
/* SSD1306 넓이 픽셀단위 */
#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH            128
#endif
/* SSD1306 높이 픽셀단위 */
#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT           64
#endif

/**
 * @}
 */
 
/**
 * @defgroup SSD1306_자료형
 * @brief    SSD1306 자료형
 * @{
 */

/**
 * @brief  SSD1306 색깔 열거형
 */
typedef enum {
	SSD1306_COLOR_BLACK = 0x00, /*!< 검은색, 픽셀 꺼짐 */
	SSD1306_COLOR_WHITE = 0x01  /*!< 픽셀이 켜짐. 색은 LCD에 따라 다름 */
} SSD1306_Color_t;

/**
 * @}
 */

/**
 * @defgroup SSD1306_함수
 * @brief    SSD1306 관련 함수
 * @{
 */

/**
 * @brief  SSD1306 그래픽 장치를 초기화한다
 * @param  없음
 * @retval 초기화 상태:
 *           - 0: 장치가 I2C 포트에서 감지되지 않음
 *           - > 0: 장치 가 정상적으로 초기화되어 사용 준비가 됨
 */
uint8_t ssd1306_init(void);

/** 
 * @brief  그래픽 장치의 내부 RAM 버퍼를 갱신한다
 * @note   이 함수는 그래픽 장치의 내용이 바뀔 때마다 내부 RAM 버퍼를 갱신하기 위해 호출되어야 한다.
 * @param  없음
 * @retval 없음
 */
void ssd1306_updatescreen(void);

/**
 * @brief  내부 RAM의 픽셀 내용을 반전시킨다
 * @note   호출 후에 갱신된 그래픽 장치 화면을 보기 위해 @ref SSD1306_updatescreen() 를 호출해야 한다
 * @param  없음
 * @retval 없음
 */
void ssd1306_invert(void);

/** 
 * @brief  전체 그래픽 장치를 원하는 색으로 채운다
 * @note   호출 후에 갱신된 그래픽 장치 화면을 보기 위해 @ref SSD1306_updatescreen() 를 호출해야 한다
 * @param  color: 스크린 채움을 위해 사용될 색. 이 매개변수는 @ref SSD1306_Color_t 열거형 값
 * @retval 없음
 */
void ssd1306_fill(SSD1306_Color_t color);

/**
 * @brief  원하는 위치에 픽셀을 그린다
 * @note   호출 후에 갱신된 그래픽 장치 화면을 보기 위해 @ref SSD1306_updatescreen() 를 호출해야 한다
 * @param  x: X 위치. 이 매개변수는 0 과 SSD1306_WIDTH - 1 사이의 값
 * @param  y: Y 위치. 이 매개변수는 0 과 SSD1306_HEIGHT - 1 사이의 값
 * @param  color: 스크린 채움을 위해 사용될 색. 이 매개변수는 @ref SSD1306_Color_t 열거형 값
 * @retval 없음
 */
void ssd1306_drawpixel(uint16_t x, uint16_t y, SSD1306_Color_t color); 

/**
 * @brief  문자열을 위해 원하는 위치로 커서 포인터를 설정한다
 * @param  x: X 위치. 이 매개변수는 0 과 SSD1306_WIDTH - 1 사이의 값
 * @param  y: Y 위치. 이 매개변수는 0 과 SSD1306_HEIGHT - 1 사이의 값
 * @retval 없음
 */
void ssd1306_gotoxy(uint16_t x, uint16_t y);

/**
 * @brief  그래픽 장치에 글자를 쓴다
 * @note   호출 후에 갱신된 그래픽 장치 화면을 보기 위해 @ref SSD1306_updatescreen() 를 호출해야 한다
 * @param  ch: 쓰여질 글자
 * @param  *font: 사용된 폰트에 대한 @ref Font_t 구조체의 포인터
 * @param  color: 그리는 데 사용할 색. 이 매개변수는 @ref SSD1306_Color_t 열거형 값
 * @retval 쓰여진 글자
 */
char ssd1306_putc(uint16_t ch, Font_t *font, SSD1306_Color_t color, uint8_t size);
char ssd1306_putc_gfx(uint16_t ch, Font_t* font, SSD1306_Color_t color, uint8_t size);
char ssd1306_putc_hangul(uint16_t ch, Font_t* font, SSD1306_Color_t color, uint8_t size);

/**
 * @brief  그래픽 장치에 문자열을 쓴다
 * @note   호출 후에 갱신된 그래픽 장치 화면을 보기 위해 @ref SSD1306_updatescreen() 를 호출해야 한다
 * @param  *str: 쓰여질 문자열
 * @param  *fontset: 사용된 폰트셋에 대한 @ref FontSet_t 구조체의 포인터
 * @param  color: 그리는 데 사용할 색. 이 매개변수는 @ref SSD1306_Color_t 열거형 값
 * @retval 성공이면 0 또는 실패할 때의 글자 값
 */
char ssd1306_puts(char* str, FontSet_t* fontset, SSD1306_Color_t color, uint8_t size);

/**
 * @brief  그래픽 장치에 선을 그린다
 * @note   호출 후에 갱신된 그래픽 장치 화면을 보기 위해 @ref SSD1306_updatescreen() 를 호출해야 한다
 * @param  x0: X 시작 위치. 이 매개변수는 0 과 SSD1306_WIDTH - 1 사이의 값
 * @param  y0: Y 시작 위치. 이 매개변수는 0 과 SSD1306_HEIGHT - 1 사이의 값
 * @param  x1: X 끝 위치. 이 매개변수는 0 과 SSD1306_WIDTH - 1 사이의 값
 * @param  y1: Y 끝 위치. 이 매개변수는 0 과 SSD1306_HEIGHT - 1 사이의 값
 * @param  color: 그리는 데 사용할 색. 이 매개변수는 @ref SSD1306_Color_t 열거형 값
 * @retval 없음
 */
void ssd1306_drawline(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SSD1306_Color_t c);

/**
 * @brief  그래픽 장치에 사각형을 그린다
 * @note   호출 후에 갱신된 그래픽 장치 화면을 보기 위해 @ref SSD1306_updatescreen() 를 호출해야 한다
 * @param  x: 가장 왼쪽 위 X 시작 위치. 이 매개변수는 0 과 SSD1306_WIDTH - 1 사이의 값
 * @param  y: 가장 왼쪽 위 Y 시작 위치. 이 매개변수는 0 과 SSD1306_HEIGHT - 1 사이의 값
 * @param  w: 픽셀 단위의 사각형 넓이
 * @param  h: 픽셀 단위의 사각형 높이
 * @param  color: 그리는 데 사용할 색. 이 매개변수는 @ref SSD1306_Color_t 열거형 값
 * @retval 없음
 */
void ssd1306_drawrectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_Color_t c);

/**
 * @brief  그래픽 장치에 사각형을 채운다
 * @note   호출 후에 갱신된 그래픽 장치 화면을 보기 위해 @ref SSD1306_updatescreen() 를 호출해야 한다
 * @param  x: 가장 왼쪽 위 X 시작 위치. 이 매개변수는 0 과 SSD1306_WIDTH - 1 사이의 값
 * @param  y: 가장 왼쪽 위 Y 시작 위치. 이 매개변수는 0 과 SSD1306_HEIGHT - 1 사이의 값
 * @param  w: 픽셀 단위의 사각형 넓이
 * @param  h: 픽셀 단위의 사각형 높이
 * @param  color: 채우는 데 사용할 색. 이 매개변수는 @ref SSD1306_Color_t 열거형 값
 * @retval 없음
 */
void ssd1306_fillrectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_Color_t c);

/**
 * @brief  그래픽 장치에 삼각형을 그린다
 * @note   호출 후에 갱신된 그래픽 장치 화면을 보기 위해 @ref SSD1306_updatescreen() 를 호출해야 한다
 * @param  x1: 첫째 X 좌표 위치. 이 매개변수는 0 과 SSD1306_WIDTH - 1 사이의 값
 * @param  y1: 첫째 Y 좌표 위치. 이 매개변수는 0 과 SSD1306_HEIGHT - 1 사이의 값
 * @param  x2: 둘째 X 좌표 위치. 이 매개변수는 0 과 SSD1306_WIDTH - 1 사이의 값
 * @param  y2: 둘째 Y 좌표 위치. 이 매개변수는 0 과 SSD1306_HEIGHT - 1 사이의 값
 * @param  x3: 셋째 X 좌표 위치. 이 매개변수는 0 과 SSD1306_WIDTH - 1 사이의 값
 * @param  y3: 셋째 Y 좌표 위치. 이 매개변수는 0 과 SSD1306_HEIGHT - 1 사이의 값
 * @param  color: 그리는 데 사용할 색. 이 매개변수는 @ref SSD1306_Color_t 열거형 값
 * @retval 없음
 */
void ssd1306_drawtriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_Color_t color);

/**
 * @brief  그래픽 장치에 삼각형을 채운다
 * @note   호출 후에 갱신된 그래픽 장치 화면을 보기 위해 @ref SSD1306_updatescreen() 를 호출해야 한다
 * @param  x1: 첫째 X 좌표 위치. 이 매개변수는 0 과 SSD1306_WIDTH - 1 사이의 값
 * @param  y1: 첫째 Y 좌표 위치. 이 매개변수는 0 과 SSD1306_HEIGHT - 1 사이의 값
 * @param  x2: 둘째 X 좌표 위치. 이 매개변수는 0 과 SSD1306_WIDTH - 1 사이의 값
 * @param  y2: 둘째 Y 좌표 위치. 이 매개변수는 0 과 SSD1306_HEIGHT - 1 사이의 값
 * @param  x3: 셋째 X 좌표 위치. 이 매개변수는 0 과 SSD1306_WIDTH - 1 사이의 값
 * @param  y3: 셋째 Y 좌표 위치. 이 매개변수는 0 과 SSD1306_HEIGHT - 1 사이의 값
 * @param  color: 채우는 데 사용할 색. 이 매개변수는 @ref SSD1306_Color_t 열거형 값
 * @retval 없음
 */
void ssd1306_filltriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_Color_t color);

/**
 * @brief  그래픽 장치에 원을 그린다
 * @note   호출 후에 갱신된 그래픽 장치 화면을 보기 위해 @ref SSD1306_updatescreen() 를 호출해야 한다
 * @param  x0: 원 중심에 대한 X 위치. 이 매개변수는 0 과 SSD1306_WIDTH - 1 사이의 값
 * @param  y0: 원 중심에 대한 Y 위치. 이 매개변수는 0 과 SSD1306_HEIGHT - 1 사이의 값
 * @param  r: 픽셀 단위의 원 반지름
 * @param  color: 그리는 데 사용할 색. 이 매개변수는 @ref SSD1306_Color_t 열거형 값
 * @retval 없음
 */
void ssd1306_drawcircle(int16_t x0, int16_t y0, int16_t r, SSD1306_Color_t c);

/**
 * @brief  그래픽 장치에 원을 채운다
 * @note   호출 후에 갱신된 그래픽 장치 화면을 보기 위해 @ref SSD1306_updatescreen() 를 호출해야 한다
 * @param  x0: 원 중심에 대한 X 위치. 이 매개변수는 0 과 SSD1306_WIDTH - 1 사이의 값
 * @param  y0: 원 중심에 대한 Y 위치. 이 매개변수는 0 과 SSD1306_HEIGHT - 1 사이의 값
 * @param  r: 픽셀 단위의 원 반지름
 * @param  color: 채우는 데 사용할 색. 이 매개변수는 @ref SSD1306_Color_t 열거형 값
 * @retval 없음
 */
void ssd1306_fillcircle(int16_t x0, int16_t y0, int16_t r, SSD1306_Color_t c);

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
