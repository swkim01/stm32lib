#ifndef _UCG_STM32_H
#define _UCG_STM32_H

#include "ucg.h"
#include "stm32f4xx_hal.h"
#include "main.h"

#define DATA_BUFFER_SIZE 1000
#define SPI_TIMEOUT 1000
#define SPI_HANDLER hspi1 // use your spi handler

extern SPI_HandleTypeDef SPI_HANDLER;

int16_t ucg_com_ssd1331_hw_spi_fn(ucg_t *ucg, int16_t msg, uint16_t arg, uint8_t *data);

#endif 
