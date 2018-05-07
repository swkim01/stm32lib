#ifndef SWI2C_H
#define SWI2C_H

#include "stm32f4xx_hal.h"

#define SWI2C_TIME_OUT    10000
#define SWI2C_WRITE           0
#define SWI2C_READ            1

#define SWI2C_SDA_Pin   GPIO_PIN_9
#define SWI2C_GPIO_Port GPIOB
#define SWI2C_SCL_Pin   GPIO_PIN_8

typedef enum {
    SWI2C_RES_OK = 0x00, /*!< 정상 동작 */
    SWI2C_RES_ERR        /*!< 오류 발생 */
} SWI2C_Res_t;

typedef enum
{
  SWI2C_NAK = 0,
  SWI2C_ACK = 1,
} SWI2C_Ack_t;

/**
 * @brief 특정한 주소의 장치가 연결되었는지 확인한다.
 */
SWI2C_Res_t swi2c_ready(uint8_t addr);

/**
 * @brief 장치 레지스터에 1바이트를 쓴다.
 */
SWI2C_Res_t swi2c_write(uint8_t addr, uint8_t reg, uint8_t byte);

/**
 * @brief 장치 레지스터로부터 1바이트를 읽는다.
 */
SWI2C_Res_t swi2c_read(uint8_t addr, uint8_t reg, uint8_t* byte);

/**
 * @brief 레지스터로부터 여러 바이트를 읽는다.
 */
SWI2C_Res_t swi2c_readmulti(uint8_t addr, uint8_t reg, uint8_t* byte, int length);

/**
 * @brief I2C 인터페이스를 초기화한다.
 */
void swi2c_init();

/**
 * @brief I2C 전송을 시작한다.
 */
void swi2c_start();

/**
 * @brief I2C 전송을 끝낸다.
 */
void swi2c_stop();

/**
 * @brief I2C에 1바이트를 쓴다.
 */
SWI2C_Ack_t swi2c_writebyte (uint8_t ch);

/**
 * @brief I2C로부터 1바이트를 읽는다.
 */
uint8_t swi2c_readbyte (SWI2C_Ack_t ack);

#endif
