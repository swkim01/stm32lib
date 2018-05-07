#ifndef _U8G_STM32_H
#define _U8G_STM32_H

#include "u8g.h"
#include "Arduino.h"

#define DATA_BUFFER_SIZE 1000
#define I2C_TIMEOUT 10000
#define DEVICE_ADDRESS 0x78 //device address is written on back side of your display
#define MASTER_ADDRESS 0x33
//#define SDA 22 // PB7
//#define SCL 10 // PB6
#define SDA 14 // PB9
#define SCL 15 // PB8

uint8_t u8g_com_hw_i2c_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
#endif 
