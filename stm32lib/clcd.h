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
#ifndef CLCD_H
#define CLCD_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup stm32lib
 * @{
 */

/**
 * @defgroup CLCD
 * @brief    STM32F4xx를 위한 HD44780 CharLCD 드라이버 라이브러리
 * @{
 *
 *	또한 모든 HD44780 호환 캐릭터 드라이버를 지원한다.
 *
 * \par 기본 핀아웃
 *	
\verbatim
LCD   STM32F4xx         설명
GND   GND               Ground
VCC   +5V               LCD를 위한 전원
V0    가변저항          Contrast 전압, 가변저항에 연결
RS    PC0               Register select 핀, 다른 핀 대체 가능
RW    GND               Read/write
E     PC1               Enable 핀, 다른 핀 대체 가능
D0    -                 Data 0 핀, 연결 불필요
D1    -                 Data 1 핀, 연결 불필요
D2    -                 Data 2 핀, 연결 불필요
D3    -                 Data 3 핀, 연결 불필요
D4    PB0               Data 4 핀, 다른 핀 대체 가능
D5    PA4               Data 5 핀, 다른 핀 대체 가능
D6    PA1               Data 6 핀, 다른 핀 대체 가능
D7    PA0               Data 7 핀, 다른 핀 대체 가능
A     +3V3              백라이트 전압
K     GND               백라이트를 위한 접지
\endverbatim
 *	
 * 핀아웃을 바꾸고 싶으면 defines.h 파일이나 소스파일 앞부분에 다음 내용과 같이 적절하게 설정한다.
 *	
\code
//RS - Register select 핀
#define CLCD_RS_PORT     GPIOA
#define CLCD_RS_PIN      0
//E - Enable 핀
#define CLCD_E_PORT      GPIOA
#define CLCD_E_PIN       1
//D4 - Data 4 핀
#define CLCD_D4_PORT     GPIOA
#define CLCD_D4_PIN      4
//D5 - Data 5 핀
#define CLCD_D5_PORT     GPIOB
#define CLCD_D5_PIN      0
//D6 - Data 6 핀
#define CLCD_D6_PORT     GPIOC
#define CLCD_D6_PIN      1
//D7 - Data 7 핀
#define CLCD_D7_PORT     GPIOC
#define CLCD_D7_PIN      0
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
 * @defgroup CLCD_매크로
 * @brief    CLCD 관련 매크로
 * @{
 *
 * 4 비트 모드 : 제어 핀들, 다른 핀으로 대체 가능
 */

/* RS - Register select 핀 */
#ifndef CLCD_RS_PIN
#define CLCD_RS_PORT				GPIOA
/**
 * @brief  RS - Register select 핀
 */
#define CLCD_RS_PIN				GPIO_PA_0
#endif
/* E - Enable 핀 */
#ifndef CLCD_E_PIN
#define CLCD_E_PORT				GPIOA
/**
 * @brief  E - Enable 핀
 */
#define CLCD_E_PIN				GPIO_PA_1
#endif
/* Data 핀 */
/* D4 - Data 4 핀 */
#ifndef CLCD_D4_PIN
#define CLCD_D4_PORT				GPIOA
/**
 * @brief  D4 - Data 4 핀
 */
#define CLCD_D4_PIN				GPIO_PA_4
#endif
/* D5 - Data 5 핀 */
#ifndef CLCD_D5_PIN
#define CLCD_D5_PORT				GPIOB
/**
 * @brief  D5 - Data 5 핀
 */
#define CLCD_D5_PIN				GPIO_PB_0
#endif
/* D6 - Data 6 핀 */
#ifndef CLCD_D6_PIN
#define CLCD_D6_PORT				GPIOC
/**
 * @brief  D6 - Data 6 핀
 */
#define CLCD_D6_PIN				GPIO_PC_1
#endif
/* D7 - Data 7 핀 */
#ifndef CLCD_D7_PIN
#define CLCD_D7_PORT				GPIOC
/**
 * @brief  D7 - Data 7 핀
 */
#define CLCD_D7_PIN				GPIO_PC_0
#endif

/**
 * @}
 */

/**
 * @defgroup CLCD_함수
 * @brief    CLCD 관련 함수
 * @{
 */

/**
 * @brief  HD44780 CLCD를 초기화한다
 * @brief  cols: LCD 넓이
 * @param  rows: LCD 높이 
 * @retval None
없음
 */
void clcd_init(uint8_t cols, uint8_t rows);

/**
 * @brief  홈(처음) 위치로 되돌아온다
 * @param  없음
 * @retval 없음
 */
void clcd_home(void);

/**
 * @brief  LCD 전체를 지운다
 * @param  없음
 * @retval 없음
 */
void clcd_clear(void);

/**
 * @brief  커서 위치를 (col, row) 로 설정한다
 * @param  col: 문자열이 시작할 X 위치
 * @param  row; 문자열이 시작할 Y 위치
 * @retval 없음
 */
void clcd_set_cursor(uint8_t col, uint8_t row);

/**
 * @brief  디스플레이를 켜거나 끈다
 * @param  enable: 켜면 1 또는 끄면 0
 * @retval 없음
 */
void clcd_enable_display(uint8_t enable);

/**
 * @brief  커서를 보이거나 숨긴다
 * @param  show: 보이면 1 또는 숨기면 0
 * @retval 없음
 */
void clcd_show_cursor(uint8_t show);

/**
 * @brief  커서 깜박이기를 활성화하거나 비활성화한다
 * @param  blink: 활성화하면 1 또는 비활성화하면 0
 * @retval 없음
 */
void clcd_blink(uint8_t blink);

/**
 * @brief  디스플레이를 왼쪽으로 이동한다
 * @param  없음
 * @retval 없음
 */
void clcd_move_left(void);

/**
 * @brief  디스플레이를 오른쪽으로 이동한다
 * @param  없음
 * @retval 없음
 */
void clcd_move_right(void);

/**
 * @brief  디스플레이 방향을 왼쪽에서 오른쪽으로 설정한다
 * @param  없음
 * @retval 없음
 */
void clcd_set_left2right(void);

/**
 * @brief  디스플레이 방향을 오른쪽에서 왼쪽으로 설정한다
 * @param  없음
 * @retval 없음
 */
void clcd_set_right2left(void);

/**
 * @brief  자동스크롤을 켜거나 끈다
 * @param  autoscroll: 자동스크롤하면 1 또는 하지 않으면 0
 * @retval 없음
 */
void clcd_autoscroll(uint8_t autoscroll);

/**
 * @brief  LCD에 문자열을 쓴다
 * @param  *str: 디스플레이할 문자열에 대한 포인터
 * @retval 없음
 */
void clcd_message(char* str);

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
