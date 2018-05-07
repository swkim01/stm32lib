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
#include "../stm32lib/i2c.h"

/* Private variable */
static I2C_HandleTypeDef i2cHandle;
static int i2c_slave = HAL_I2C_MODE_MASTER;
static uint16_t i2c_hz = 100000; // 100 kHz for default
static I2C_Pins_t i2c_pins[] = {
    {I2C_1, GPIO_PB_6, GPIO_PB_7, GPIO_AF4_I2C1},
    {I2C_1, GPIO_PB_8, GPIO_PB_9, GPIO_AF4_I2C1},
    {I2C_1, GPIO_PB_6, GPIO_PB_9, GPIO_AF4_I2C1},
    {I2C_2, GPIO_PB_10, GPIO_PB_3, GPIO_AF4_I2C2},
    {I2C_3, GPIO_PA_8, GPIO_PC_9, GPIO_AF4_I2C3},
    {I2C_3, GPIO_PB_4, GPIO_PC_9, GPIO_AF4_I2C3},
};

/* Private functions */
static void i2c_internal_init(I2C_TypeDef* I2Cx, uint16_t hz, int slave);

void i2c_init(I2C_t I2C_Num, I2C_PinsPack_t pack) {

    assert_param(IS_I2C_PINS_VALID(I2C_Num, pack));

    /* 클록 활성화 */
#ifdef I2C1
    if (I2C_Num == I2C_1)
        __HAL_RCC_I2C1_CLK_ENABLE();
#endif
#ifdef I2C2
    if (I2C_Num == I2C_2)
        __HAL_RCC_I2C2_CLK_ENABLE();
#endif
#ifdef I2C3
    if (I2C_Num == I2C_3)
        __HAL_RCC_I2C3_CLK_ENABLE();
#endif

    gpio_alternate_init(i2c_pins[pack].scl, GPIO_OD, i2c_pins[pack].alternate);
    gpio_set_pinmode(i2c_pins[pack].scl, GPIO_PM_PULLUP);
    gpio_alternate_init(i2c_pins[pack].sda, GPIO_OD, i2c_pins[pack].alternate);
    gpio_set_pinmode(i2c_pins[pack].sda, GPIO_PM_PULLUP);

    /* 초기화 */
    i2c_internal_init((I2C_TypeDef *)I2C_Num, 100000, HAL_I2C_MODE_MASTER);
}

void i2c_deinit(I2C_t I2C_Num) {
    i2cHandle.Instance = (I2C_TypeDef *)I2C_Num;
    /* 종료화 */
    HAL_I2C_DeInit(&i2cHandle);
/*
#ifdef I2C1
    if (I2C_Num == I2C_1) {
        __HAL_RCC_I2C1_FORCE_RESET();
        __HAL_RCC_I2C1_RELEASE_RESET();
        __HAL_RCC_I2C1_CLK_DISABLE();
    }
#endif
#ifdef I2C2
    if (I2C_Num == I2C_2) {
        __HAL_RCC_I2C2_FORCE_RESET();
        __HAL_RCC_I2C2_RELEASE_RESET();
        __HAL_RCC_I2C2_CLK_DISABLE();
    }
#endif
#ifdef I2C3
    if (I2C_Num == I2C_3) {
        __HAL_RCC_I2C3_FORCE_RESET();
        __HAL_RCC_I2C3_RELEASE_RESET();
        __HAL_RCC_I2C3_CLK_DISABLE();
    }
#endif
#ifdef I2C4
    if (I2C_Num == I2C_4) {
        __HAL_RCC_I2C4_FORCE_RESET();
        __HAL_RCC_I2C4_RELEASE_RESET();
        __HAL_RCC_I2C4_CLK_DISABLE();
    }
#endif
*/
}

static void i2c_internal_init(I2C_TypeDef* I2Cx, uint16_t hz, int slave) {

    i2cHandle.Instance = I2Cx;

#if defined(STM32F7xx)
    uint32_t timing;

    if (clockSpeed >= 3400000) {
        /* 100kHz @ 50MHz APB clock */
        timing = 0x40912732;
    } else if (clockSpeed >= 1000000) {
        /* 100kHz @ 50MHz APB clock */
        timing = 0x40912732;
    } else if (clockSpeed >= 400000) {
        /* 100kHz @ 50MHz APB clock */
        timing = 0x40912732;
    } else {
        /* 100kHz @ 50MHz APB clock */
        timing = 0x40912732;
    }
#endif
#if defined(STM32F0xx)
    /* 100kHz @ 48MHz APB clock */
    uint32_t timing = 0x10805E89;
#endif

    /* I2C 설정 */
    i2cHandle.Init.OwnAddress1 = 0x00;
    i2cHandle.Init.OwnAddress2 = 0x00;
    i2cHandle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    i2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
    i2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
    i2cHandle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
#if defined(STM32F7xx) || defined(STM32F0xx)
    i2cHandle.Init.Timing = timing;
#else
    i2cHandle.Init.ClockSpeed = hz;
    i2cHandle.Init.DutyCycle = I2C_DUTYCYCLE_2;
#endif

    if (slave == HAL_I2C_MODE_SLAVE) /* 주소 Acknowledge 활성화 */
        i2cHandle.Instance->CR1 |= I2C_CR1_ACK;

    /* 초기화 */
    HAL_I2C_Init(&i2cHandle);
}

void i2c_set_slave(I2C_t I2C_Num, int slave)
{
    i2c_slave = (slave) ? HAL_I2C_MODE_SLAVE : HAL_I2C_MODE_MASTER;
    /* 초기화 */
    i2c_internal_init((I2C_TypeDef *)I2C_Num, i2c_hz, i2c_slave);
}

void i2c_set_frequency(I2C_t I2C_Num, int hz)
{
    i2c_hz = hz;
    /* 초기화 */
    i2c_internal_init((I2C_TypeDef *)I2C_Num, i2c_hz, i2c_slave);
}

int i2c_readdata(I2C_t I2C_Num, int address, uint8_t* data, int length)
{
    i2cHandle.Instance = (I2C_TypeDef *)I2C_Num;

    /* 다중 바이트 수신 */
    if (HAL_I2C_Master_Receive(&i2cHandle, address, data, length, 1000) != HAL_OK) {
        /* 오류 반환 */
        return -1;
    }
    /* 정상 반환 */
    return 1;
}

int i2c_nread(I2C_t I2C_Num, int address, int reg, uint8_t* data, int length)
{
    i2cHandle.Instance = (I2C_TypeDef *)I2C_Num;

    /* 레지스터 주소 전송 */
    if (HAL_I2C_Master_Transmit(&i2cHandle, (uint16_t)address, (uint8_t *)&reg, 1, 1000) != HAL_OK) {
        /* 오류 반환 */
        return -1;
    }
    /* 다중 바이트 수신 */
    return i2c_readdata(I2C_Num, address, data, length);
}

int i2c_read(I2C_t I2C_Num, int address, uint8_t reg, uint8_t* data)
{
    i2cHandle.Instance = (I2C_TypeDef *)I2C_Num;

    /* 전송 시도 */
    if (HAL_I2C_Master_Transmit(&i2cHandle, (uint16_t)address, (uint8_t *)&reg, 1, 1000) != HAL_OK) {
        /* 오류 반환 */
        return -1;
    }
    /* 한 바이트 수신 */
    return i2c_readdata(I2C_Num, address, data, 1);
}

int i2c_read16(I2C_t I2C_Num, int address, uint16_t reg, uint8_t* data)
{
    uint8_t bytes[2] = {(reg >> 8) & 0xFF, reg & 0xFF};
    i2cHandle.Instance = (I2C_TypeDef *)I2C_Num;

    /* 전송 시도 */
    if (HAL_I2C_Master_Transmit(&i2cHandle, (uint16_t)address, (uint8_t *)bytes, 2, 1000) != HAL_OK) {
        /* 오류 반환 */
        return -1;
    }
    /* 한 바이트 수신 */
    return i2c_readdata(I2C_Num, address, data, 1);
}

int i2c_nwrite(I2C_t I2C_Num, int address, int reg, uint8_t *data, int length)
{
    i2cHandle.Instance = (I2C_TypeDef *)I2C_Num;

    /* 전송 시도 */
    if (HAL_I2C_Mem_Write(&i2cHandle, address, reg, reg > 0xFF ? I2C_MEMADD_SIZE_16BIT : I2C_MEMADD_SIZE_8BIT, data, length, 1000) != HAL_OK) {
        /* 오류 반환 */
        return -1;
    }
    /* 정상 반환 */
    return 1;
}

int i2c_write(I2C_t I2C_Num, int address, uint8_t reg, uint8_t data)
{
    uint8_t bytes[2] = {reg, data};
    i2cHandle.Instance = (I2C_TypeDef *)I2C_Num;

    /* 전송 시도 */
    if (HAL_I2C_Master_Transmit(&i2cHandle, (uint16_t)address, (uint8_t *)bytes, 2, 1000) != HAL_OK) {
        /* 오류 반환 */
        return -1;
    }
    /* 정상 반환 */
    return 1;
}

int i2c_write16(I2C_t I2C_Num, int address, uint16_t reg, uint8_t data)
{
    uint8_t bytes[3] = {(reg >> 8) & 0xFF, reg & 0xFF, data};
    i2cHandle.Instance = (I2C_TypeDef *)I2C_Num;

    /* 전송 시도 */
    if (HAL_I2C_Master_Transmit(&i2cHandle, (uint16_t)address, (uint8_t *)bytes, 3, 1000) != HAL_OK) {
        /* 오류 반환 */
        return -1;
    }
    /* 정상 반환 */
    return 1;
}

int i2c_ready(I2C_t I2C_Num, int address)
{
    i2cHandle.Instance = (I2C_TypeDef *)I2C_Num;

    /* 장치가 준비되었는지 점검 */
    if (HAL_I2C_IsDeviceReady(&i2cHandle, (uint8_t)address, 2, 5) != HAL_OK) {
        /* 오류 반환 */
        return -1;
    }
    /* 정상 반환 */
    return 1;
}
