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
#include "../stm32lib/spi.h"

/* Private variable */
static SPI_HandleTypeDef spiHandle;
static SPI_Mode_t spi_mode = SPI_Mode0;
static int spi_bits = SPI_DATASIZE_8BIT;
static int spi_slave = SPI_MODE_MASTER;
static uint16_t spi_baudrateprescaler = SPI_BAUDRATEPRESCALER_256;
static SPI_Pins_t spi_pins[] = {
    {SPI_1, GPIO_PA_7,  GPIO_PA_6,  GPIO_PA_5,  GPIO_AF5_SPI1},
    {SPI_1, GPIO_PB_5,  GPIO_PB_4,  GPIO_PB_3,  GPIO_AF5_SPI1},
    {SPI_2, GPIO_PB_14, GPIO_PB_13, GPIO_PB_12, GPIO_AF5_SPI2},
    {SPI_2, GPIO_PC_3,  GPIO_PC_2,  GPIO_PB_10, GPIO_AF5_SPI2},
    {SPI_3, GPIO_PB_5,  GPIO_PB_4,  GPIO_PB_3,  GPIO_AF5_SPI3},
    {SPI_3, GPIO_PC_12, GPIO_PC_11, GPIO_PC_10, GPIO_AF6_SPI3},
};

/* Private functions */
static void spi_internal_init(SPI_TypeDef* SPIx, SPI_Mode_t mode, uint16_t spi_BaudPrescaler, int slave, int bits);

void spi_init(SPI_t SPI_Num, SPI_PinsPack_t pack) {

    assert_param(IS_SPI_PINS_VALID(SPI_Num, pack));

    /* clock enable */
#ifdef SPI1
    if (SPI_Num == SPI_1)
        __HAL_RCC_SPI1_CLK_ENABLE();
#endif
#ifdef SPI2
    if (SPI_Num == SPI_2)
        __HAL_RCC_SPI2_CLK_ENABLE();
#endif
#ifdef SPI3
    if (SPI_Num == SPI_3)
        __HAL_RCC_SPI3_CLK_ENABLE();
#endif
#ifdef SPI4
    if (SPI_Num == SPI_4)
        __HAL_RCC_SPI4_CLK_ENABLE();
#endif
#ifdef SPI5
    if (SPI_Num == SPI_5)
        __HAL_RCC_SPI5_CLK_ENABLE();
#endif
#ifdef SPI6
    if (SPI_Num == SPI_6)
        __HAL_RCC_SPI6_CLK_ENABLE();
#endif

    gpio_alternate_init(spi_pins[pack].mosi, GPIO_AF, spi_pins[pack].alternate);
    gpio_set_pinmode(spi_pins[pack].mosi, GPIO_PM_PULLDOWN);
    gpio_alternate_init(spi_pins[pack].miso, GPIO_AF, spi_pins[pack].alternate);
    gpio_set_pinmode(spi_pins[pack].miso, GPIO_PM_PULLDOWN);
    gpio_alternate_init(spi_pins[pack].sclk, GPIO_AF, spi_pins[pack].alternate);
    gpio_set_pinmode(spi_pins[pack].sclk, GPIO_PM_PULLUP);

    /* Do initialization */
    //spi_internal_init((SPI_TypeDef *)SPI_Num, SPI_Mode3, SPI_BAUDRATEPRESCALER_256, SPI_MODE_MASTER, SPI_DATASIZE_8BIT);
    spi_internal_init((SPI_TypeDef *)SPI_Num, SPI_Mode0, SPI_BAUDRATEPRESCALER_8, SPI_MODE_MASTER, SPI_DATASIZE_8BIT);
}

void spi_deinit(SPI_t SPI_Num) {
#ifdef SPI1
    if (SPI_Num == SPI_1) {
        __HAL_RCC_SPI1_FORCE_RESET();
        __HAL_RCC_SPI1_RELEASE_RESET();
        __HAL_RCC_SPI1_CLK_DISABLE();
    }
#endif
#ifdef SPI2
    if (SPI_Num == SPI_2) {
        __HAL_RCC_SPI2_FORCE_RESET();
        __HAL_RCC_SPI2_RELEASE_RESET();
        __HAL_RCC_SPI2_CLK_DISABLE();
    }
#endif
#ifdef SPI3
    if (SPI_Num == SPI_3) {
        __HAL_RCC_SPI3_FORCE_RESET();
        __HAL_RCC_SPI3_RELEASE_RESET();
        __HAL_RCC_SPI3_CLK_DISABLE();
    }
#endif
#ifdef SPI4
    if (SPI_Num == SPI_4) {
        __HAL_RCC_SPI4_FORCE_RESET();
        __HAL_RCC_SPI4_RELEASE_RESET();
        __HAL_RCC_SPI4_CLK_DISABLE();
    }
#endif
#ifdef SPI5
    if (SPI_Num == SPI_5) {
        __HAL_RCC_SPI5_FORCE_RESET();
        __HAL_RCC_SPI5_RELEASE_RESET();
        __HAL_RCC_SPI5_CLK_DISABLE();
    }
#endif
#ifdef SPI6
    if (SPI_Num == SPI_6) {
        __HAL_RCC_SPI6_FORCE_RESET();
        __HAL_RCC_SPI6_RELEASE_RESET();
        __HAL_RCC_SPI6_CLK_DISABLE();
    }
#endif
}

static void spi_internal_init(SPI_TypeDef* SPIx, SPI_Mode_t mode, uint16_t spi_BaudRatePrescaler, int slave, int bits) {

    spiHandle.Instance = SPIx;

    /* Fill SPI settings */
    spiHandle.Init.Mode = slave;
    spiHandle.Init.BaudRatePrescaler = spi_BaudRatePrescaler;
    spiHandle.Init.Direction = SPI_DIRECTION_2LINES;
    spiHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    spiHandle.Init.CRCPolynomial = 7;
    spiHandle.Init.DataSize = bits;
    spiHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
    spiHandle.Init.NSS = SPI_NSS_SOFT;
    spiHandle.Init.TIMode = SPI_TIMODE_DISABLE;

    /* SPI mode */
    if (mode == SPI_Mode0) {
        spiHandle.Init.CLKPolarity = SPI_POLARITY_LOW;
        spiHandle.Init.CLKPhase = SPI_PHASE_1EDGE;
    } else if (mode == SPI_Mode1) {
        spiHandle.Init.CLKPolarity = SPI_POLARITY_LOW;
        spiHandle.Init.CLKPhase = SPI_PHASE_2EDGE;
    } else if (mode == SPI_Mode2) {
        spiHandle.Init.CLKPolarity = SPI_POLARITY_HIGH;
        spiHandle.Init.CLKPhase = SPI_PHASE_1EDGE;
    } else if (mode == SPI_Mode3) {
        spiHandle.Init.CLKPolarity = SPI_POLARITY_HIGH;
        spiHandle.Init.CLKPhase = SPI_PHASE_2EDGE;
    }

    __HAL_SPI_DISABLE(&spiHandle);

    /* Do initialization */
    HAL_SPI_Init(&spiHandle);

    __HAL_SPI_ENABLE(&spiHandle);
}

void spi_set_format(SPI_t SPI_Num, SPI_Mode_t mode, int slave, int bits) {

    spi_mode = mode;
    if (bits == 16) {
        spi_bits = SPI_DATASIZE_16BIT;
    } else {
        spi_bits = SPI_DATASIZE_8BIT;
    }
    spi_slave = (slave) ? SPI_MODE_SLAVE : SPI_MODE_MASTER;

    /* Do initialization */
    spi_internal_init((SPI_TypeDef *)SPI_Num, spi_mode, spi_baudrateprescaler, spi_slave, spi_bits);
}

void spi_set_frequency(SPI_t SPI_Num, int hz)
{
    uint32_t apb_freq;
    uint8_t i;

    /* Calculate max SPI clock */
    if (0
#ifdef SPI1     
        || SPI_Num == SPI_1
#endif
#ifdef SPI4
        || SPI_Num == SPI_4
#endif
#ifdef SPI5
        || SPI_Num == SPI_5
#endif
#ifdef SPI6
        || SPI_Num == SPI_6
#endif
    ) {
        apb_freq = HAL_RCC_GetPCLK2Freq();
    } else {
        apb_freq = HAL_RCC_GetPCLK1Freq();
    }

    /* Calculate prescaler value */
    /* Bits 5:3 in CR1 SPI registers are prescalers */
    /* 000 = 2, 001 = 4, 002 = 8, ..., 111 = 256 */
    for (i = 0; i < 8; i++)
        if (apb_freq / (1 << (i + 1)) <= hz)
            break;
    spi_baudrateprescaler = i << 3;

    /* Do initialization */
    spi_internal_init((SPI_TypeDef *)SPI_Num, spi_mode, spi_baudrateprescaler, spi_slave, spi_bits);
}

static inline int ssp_readable(SPI_TypeDef *SPIx)
{
    int status;
    spiHandle.Instance = SPIx;
    // Check if data is received
    status = ((__HAL_SPI_GET_FLAG(&spiHandle, SPI_FLAG_RXNE) != RESET) ? 1 : 0);
    return status;
}

static inline int ssp_writeable(SPI_TypeDef *SPIx)
{
    int status;
    spiHandle.Instance = SPIx;
    // Check if data is transmitted
    status = ((__HAL_SPI_GET_FLAG(&spiHandle, SPI_FLAG_TXE) != RESET) ? 1 : 0);
    return status;
}

static inline void ssp_write(SPI_t SPI_Num, int value)
{
    SPI_TypeDef *SPIx = (SPI_TypeDef *)SPI_Num;
    while (!ssp_writeable(SPIx));
    SPIx->DR = (uint16_t)value;
}

static inline int ssp_read(SPI_t SPI_Num)
{
    SPI_TypeDef *SPIx = (SPI_TypeDef *)SPI_Num;
    while (!ssp_readable(SPIx));
    //return (int)(__IO uint8_t *)&SPIx->DR;
    return (int)SPIx->DR;
}

static inline int ssp_busy(SPI_TypeDef *SPIx)
{
    int status;
    spiHandle.Instance = SPIx;
    status = ((__HAL_SPI_GET_FLAG(&spiHandle, SPI_FLAG_BSY) != RESET) ? 1 : 0);
    return status;
}

int spi_writeread(SPI_t SPI_Num, int value)
{
    ssp_write(SPI_Num, value);
    return ssp_read(SPI_Num);
}

int spi_read(SPI_t SPI_Num)
{
    SPI_TypeDef *SPIx = (SPI_TypeDef *)SPI_Num;
    while (!ssp_readable(SPIx));
    //return (int)*(__IO uint8_t *)&SPIx->DR;
    return (int)SPIx->DR;
}

void spi_write(SPI_t SPI_Num, int value)
{
    SPI_TypeDef *SPIx = (SPI_TypeDef *)SPI_Num;
    while (!ssp_writeable(SPIx));
    //*(__IO uint8_t *)&SPIx->DR = (uint8_t)value;
    SPIx->DR = (uint8_t)value;
}

int spi_busy(SPI_t SPI_Num)
{
    return ssp_busy((SPI_TypeDef *)SPI_Num);
}
