#include "u8g_stm32.h"

i2c_t i2c;
static uint8_t control = 0;

#ifndef ARDUINO
/*
void u8g_Delay(uint16_t val)
{
    HAL_Delay(val);
}

void u8g_MicroDelay(void)
{
    int i;
    for (i = 0; i < 1000; i++);
}

void u8g_10MicroDelay(void)
{
    int i;
    for (i = 0; i < 10000; i++);
}
*/
#endif

uint8_t u8g_com_hw_i2c_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
    int8_t ret = 1;

    switch (msg)
    {
    case U8G_COM_MSG_STOP:
        break;

    case U8G_COM_MSG_INIT:
        i2c.sda = digitalPinToPinName(SDA);
        i2c.scl = digitalPinToPinName(SCL);
        i2c_custom_init(&i2c,I2C_100KHz,I2C_ADDRESSINGMODE_7BIT,MASTER_ADDRESS<<1,1);
        u8g_MicroDelay();
        break;

    case U8G_COM_MSG_ADDRESS: /*define cmd(arg_val=0) or data mode(arg_val=1)*/
        u8g_10MicroDelay();
        if (arg_val == 0) {
            control = 0;
        } else {
            control = 0x40;
        }
        break;

    case U8G_COM_MSG_WRITE_BYTE:
      {
        uint8_t buffer[2];
        buffer[0] = control;
        buffer[1] = arg_val;
        if (i2c_master_write(&i2c, DEVICE_ADDRESS, buffer, 2) == I2C_OK)
            ret = 1;
        else
            ret = -1;
        break;
      }
    case U8G_COM_MSG_WRITE_SEQ:
    case U8G_COM_MSG_WRITE_SEQ_P:
      {
        uint8_t buffer[DATA_BUFFER_SIZE];
        uint8_t *ptr = arg_ptr;
        int i;

        buffer[0] = control;
        for (i = 1; i <= arg_val; i++) {
            buffer[i] = *(ptr++);
        }
        if (i2c_master_write(&i2c, DEVICE_ADDRESS, buffer, arg_val+1) == I2C_OK)
            ret = 1;
        else
            ret = -1;
        break;
      }
    }
    return ret;
}
