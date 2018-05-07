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
#ifndef MPU6050_H
#define MPU6050_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup stm32lib
 * @{
 */

/**
 * @defgroup MPU6050
 * @brief    STM32F4xx를 위한 MPU6050 드라이버 라이브러리
 * @{
 *
 * \par 지원하는 기능
 *
\verbatim
- 가속도, 자이로, 온도 데이터 읽기
- 측정을 위한 출력 데이터 처리율 설정
\endverbatim
 *
 * \par MPU6050 처리율
 *
 * 특정한 처리율로 데이터를 출력할 수 있다. 8비트 레지스터 값으로 처리율을 설정한다.
 *
 * 처리율 공식:
\f[
	처리율 = 
		\frac{8 MHz}{REGVAL + 1}
\f]
 * 여기서:
 *  - 8 MHz는 처리율을 위해 사용되는 내부 출력 값
 *  - REGVAL 는 @ref mpu6050_setrate 함수에서 사용되는 값
 *
 * \note 몇몇 표준 처리율을 위해 미리 설정된 상수값이 헤더 파일에 정의되어 있음
 *
 * \par 기본 사용 핀들
 * 
@verbatim
MPU6050     STM32Fxxx     설명
 
SCL         PB8           I2C 를 위한 클럭 핀
SDA         PB9           I2C 를 위한 데이터 핀
IRQ         -             인터럽트 핀
VCC         3.3V
GND         GND
AD0         -             핀이 low이면 I2C 주소는 0xD0, 핀이 high이면 I2C 주소는 0xD2
@endverbatim
 *
 * 기본 핀을 바꾸려면 이 헤더 파일을 열고 다음과 같은 매크로 정의를 바꾸어야 한다.
 *
\code
//사용된 I2C
#define MPU6050_I2C               I2C_1  
//사용된 I2C 핀
#define MPU6050_I2C_PINSPACK      I2C1_PINS2
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
@endverbatim
 */

#include "stm32f4xx_hal.h"
#include "../stm32lib/i2c.h"

/**
 * @defgroup MPU6050_매크로
 * @brief    MPU6050 관련 매크로
 * @{
 */

/* I2C 설정 */
#ifndef MPU6050_I2C
#define MPU6050_I2C              I2C_1
#define MPU6050_I2C_PINSPACK     I2C1_PINS2
#endif

/* 기본 I2C 주소 */
#ifndef MPU6050_I2C_ADDR
#define MPU6050_I2C_ADDR         0xD0
//#define MPU6050_I2C_ADDR         0xD2

#endif

/* 기본 I2C 클럭 */
#ifndef MPU6050_I2C_CLOCK
#define MPU6050_I2C_CLOCK        400000          /*!< 기본 I2C 클럭 속도 */
#endif

/**
 * @brief  미리 정의된 데이터 처리율 상수들
 * @{
 */
#define MPU6050_RATE_8KHZ       0   /*!< 샘플레이트를 8 kHz 로 설정 */
#define MPU6050_RATE_4KHZ       1   /*!< 샘플레이트를 4 kHz 로 설정 */
#define MPU6050_RATE_2KHZ       3   /*!< 샘플레이트를 2 kHz 로 설정 */
#define MPU6050_RATE_1KHZ       7   /*!< 샘플레이트를 1 kHz 로 설정 */
#define MPU6050_RATE_500HZ      15  /*!< 샘플레이트를 500Hz 로 설정 */
#define MPU6050_RATE_250HZ      31  /*!< 샘플레이트를 250Hz 로 설정 */
#define MPU6050_RATE_125HZ      63  /*!< 샘플레이트를 125Hz 로 설정 */
#define MPU6050_RATE_100HZ      79  /*!< 샘플레이트를 100Hz 로 설정 */
/**
 * @}
 */
 
/**
 * @}
 */
 
/**
 * @defgroup MPU6050_자료형
 * @brief    MPU6050 자료형
 * @{
 */

/**
 * @brief  MPU6050은 ADO 입력 핀의 값에 따라 2개의 다른 슬레이브 주소를 가진다.
 *         이런 특징으로 동시에 2개의 다른 MPU6050 센서를 사용할 수 있다.
 */
typedef enum _MPU6050_Dev_t {
	MPU6050_DEV_0 = 0x00, /*!< AD0 핀이 low로 설정 */
	MPU6050_DEV_1 = 0x02  /*!< AD0 핀이 high로 설정 */
} MPU6050_Dev_t;

/**
 * @brief  MPU6050 결과 열거형
 */
typedef enum _MPU6050_Res_t {
	MPU6050_RES_OK = 0x00,       /*!< 정상 OK */
	MPU6050_RES_ERR,             /*!< 오류 */
	MPU6050_RES_NOTCONNECT,      /*!< 지정된 슬레이브 주소를 가진 장치가 연결되지 않음 */
	MPU6050_RES_INVALID          /*!< 연결된 장치가 MPU6050이 아님 */
} MPU6050_Res_t;

/**
 * @brief  가속도계 범위에 대한 매개변수
 */
typedef enum _MPU6050_Acc_t {
	MPU6050_ACC_2G = 0x00, /*!< 범위는 +- 2G */
	MPU6050_ACC_4G = 0x01, /*!< 범위는 +- 4G */
	MPU6050_ACC_8G = 0x02, /*!< 범위는 +- 8G */
	MPU6050_ACC_16G = 0x03 /*!< 범위는 +- 16G */
} MPU6050_Acc_t;

/**
 * @brief  자이로스코프 범위에 대한 매개변수
 */
typedef enum _MPU6050_Gyro_t {
	MPU6050_GYRO_250S = 0x00,  /*!< 범위는 +- 250 degrees/s */
	MPU6050_GYRO_500S = 0x01,  /*!< 범위는 +- 500 degrees/s */
	MPU6050_GYRO_1000S = 0x02, /*!< 범위는 +- 1000 degrees/s */
	MPU6050_GYRO_2000S = 0x03  /*!< 범위는 +- 2000 degrees/s */
} MPU6050_Gyro_t;

/**
 * @brief  주 MPU6050 구조체
 */
typedef struct MPU6050_t {
	/* Private */
	uint8_t address;    /*!< I2C 장치 주소. 내부용 */
	float gyro_mult;    /*!< 자이로스코프 범위 "degrees/s". 내부용 */
	float acc_mult;     /*!< 가속도계 범위 "g". 내부용 */
	/* Public */
	int16_t acc_x;      /*!< 가속도계의 X축 값 */
	int16_t acc_y;      /*!< 가속도계의 Y축 값 */
	int16_t acc_z;      /*!< 가속도계의 Z축 값 */
	int16_t gyro_x;     /*!< 자이로스코프의 X축 값 */
	int16_t gyro_y;     /*!< 자이로스코프의 X축 값 */
	int16_t gyro_z;     /*!< 자이로스코프의 X축 값 */
	float temperature;  /*!< 온도 값(섭씨) */
} MPU6050_t;

/**
 * @}
 */

/**
 * @defgroup MPU6050_함수
 * @brief    MPU6050 관련 함수
 * @{
 */

/**
 * @brief  MPU6050과 I2C 버스를 초기화한다
 * @param  *mpu6050: @ref MPU6050_t 구조체의 포인터
 * @param  devnum: MPU6050은 장치 주소를 설정하는 데 사용되는 AD0 핀을 가진다.
 *          이런 특징으로 동시에 2개의 다른 MPU6050 센서를 사용할 수 있다.
 *          이 매개변수는 @ref MPU6050_Dev_t 열거형 값으로 설정할 수 있다
 *          AD0핀을 low로 설정하면 이 매개변수는 MPU6050_DEV_0이어야 한다
 *          AD0핀을 high로 설정하면 이 매개변수는 MPU6050_DEV_1이어야 한다
 * @param  acc: 설정하는 가속도계 범위. 이 값은 @ref MPU6050_Acc_t 열거형
 * @param  gyro: 설정하는 자이로스코프 범위. 이 값은 @ref MPU6050_Gyro_t 열거형
 * @retval 초기화 상태:
 *            - MPU6050_RES_OK: 정상
 *            - 다른 값: 다른 경우
 */
MPU6050_Res_t mpu6050_init(MPU6050_t* mpu6050, MPU6050_Dev_t devnum, MPU6050_Acc_t acc, MPU6050_Gyro_t gyro);

/**
 * @brief  가속도계 범위를 설정한다
 * @param  *mpu6050: MPU6050 장치를 나타내는 @ref MPU6050_t 구조체의 포인터
 * @param  acc: 설정하는 가속도계 범위. 이 값은 @ref MPU6050_Acc_t 열거형
 * @retval @ref MPU6050_Res_t 열거형 값
 */
MPU6050_Res_t mpu6050_setacc(MPU6050_t* mpu6050, MPU6050_Acc_t acc);

/**
 * @brief  자이로스코프 범위를 설정한다
 * @param  *mpu6050: MPU6050 장치를 나타내는 @ref MPU6050_t 구조체의 포인터
 * @param  gyro: 설정하는 자이로스코프 범위. 이 값은 @ref MPU6050_Gyro_t 열거형
 * @retval @ref MPU6050_Res_t 열거형 값
 */
MPU6050_Res_t mpu6050_setgyro(MPU6050_t* mpu6050, MPU6050_Gyro_t gyro);

/**
 * @brief  데이터 처리율을 설정한다
 * @param  *mpu6050: MPU6050 장치를 나타내는 @ref MPU6050_t 구조체의 포인터
 * @param  rate: 설정하는 데이터 처리율. 이 값은 미리설정된 8비트 값
 * @retval @ref MPU6050_Res_t 열거형 값
 */
MPU6050_Res_t mpu6050_setrate(MPU6050_t* mpu6050, uint8_t rate);

/**
 * @brief  센서로부터 가속도계 값을 읽는다
 * @param  *mpu6050: 값을 저장할 @ref MPU6050_t 구조체의 포인터
 * @retval @ref MPU6050_Res_t 열거형 값
 *            - MPU6050_RES_OK: 정상
 *            - 다른 값: 다른 경우
 */
MPU6050_Res_t mpu6050_readacc(MPU6050_t* mpu6050);

/**
 * @brief  센서로부터 자이로스코프 값을 읽는다
 * @param  *mpu6050: 값을 저장할 @ref MPU6050_t 구조체의 포인터
 * @retval @ref MPU6050_Res_t 열거형 값
 *            - MPU6050_RES_OK: 정상
 *            - 다른 값: 다른 경우
 */
MPU6050_Res_t mpu6050_readgyro(MPU6050_t* mpu6050);

/**
 * @brief  센서로부터 온도 값을 읽는다
 * @param  *mpu6050: 값을 저장할 @ref MPU6050_t 구조체의 포인터
 * @retval @ref MPU6050_Res_t 열거형 값
 *            - MPU6050_RES_OK: 정상
 *            - 다른 값: 다른 경우
 */
MPU6050_Res_t mpu6050_readtemp(MPU6050_t* mpu6050);

/**
 * @brief  센서로부터 가속도계, 자이로스코프, 온도 값을 읽는다
 * @param  *mpu6050: 값을 저장할 @ref MPU6050_t 구조체의 포인터
 * @retval @ref MPU6050_Res_t 열거형 값
 *            - MPU6050_RES_OK: 정상
 *            - 다른 값: 다른 경우
 */
MPU6050_Res_t mpu6050_readall(MPU6050_t* mpu6050);

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
