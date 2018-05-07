#include "swi2c.h"

SWI2C_Res_t swi2c_ready(uint8_t addr)
{
    int i = SWI2C_TIME_OUT;

    do {
        swi2c_start();
        if (--i == 0)
            return SWI2C_RES_ERR;
    } while (swi2c_writebyte(addr + SWI2C_WRITE) != SWI2C_ACK);

    return SWI2C_RES_OK;
}


SWI2C_Res_t swi2c_write(uint8_t addr, uint8_t reg, uint8_t byte)
{
    if (swi2c_ready(addr) == SWI2C_RES_ERR)
        return SWI2C_RES_ERR;
    if (swi2c_writebyte(reg) != SWI2C_ACK)
        return SWI2C_RES_ERR;
    if (swi2c_writebyte(byte) != SWI2C_ACK)
        return SWI2C_RES_ERR;
    swi2c_stop();

    return SWI2C_RES_OK;
}

SWI2C_Res_t swi2c_read(uint8_t addr, uint8_t reg, uint8_t* byte)
{
    if (swi2c_ready(addr) == SWI2C_RES_ERR)
        return SWI2C_RES_ERR;
    if (swi2c_writebyte(reg) != SWI2C_ACK)
       return SWI2C_RES_ERR;

    swi2c_start();
    swi2c_writebyte((uint8_t)(addr + SWI2C_READ));
    *byte = swi2c_readbyte(SWI2C_NAK);
    swi2c_stop(); 

    return SWI2C_RES_OK;
}

SWI2C_Res_t swi2c_readmulti(uint8_t addr, uint8_t reg, uint8_t* byte, int length)
{
    int i;

    for (i = 0; i < length; i++) {
        if (swi2c_read(addr, reg + i, byte + i) == SWI2C_RES_ERR)
            return SWI2C_RES_ERR;
    }
    return SWI2C_RES_OK;
}

void swi2c_wait()
{
   volatile int i = 5/*10*/;
   for (;i;--i);
}

void scl_dir(char state)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    if (state) {
        GPIO_InitStruct.Pin =  SWI2C_SCL_Pin;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD/*GPIO_MODE_OUTPUT_PP*/;
        //GPIO_InitStruct.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(SWI2C_GPIO_Port, &GPIO_InitStruct);
    }
    else {
        GPIO_InitStruct.Pin =  SWI2C_SCL_Pin;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        HAL_GPIO_Init(SWI2C_GPIO_Port, &GPIO_InitStruct);
    }
}

void scl_out(char state)
{
    if (state) {
        HAL_GPIO_WritePin(SWI2C_GPIO_Port, SWI2C_SCL_Pin, GPIO_PIN_SET);
    }
    else {
        HAL_GPIO_WritePin(SWI2C_GPIO_Port, SWI2C_SCL_Pin, GPIO_PIN_RESET);
    }
}


void sda_dir(char state)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    if (state) {
        GPIO_InitStruct.Pin =  SWI2C_SDA_Pin;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD/*GPIO_MODE_OUTPUT_PP*/;
        //GPIO_InitStruct.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(SWI2C_GPIO_Port, &GPIO_InitStruct);
    }
    else {
        GPIO_InitStruct.Pin =  SWI2C_SDA_Pin;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        HAL_GPIO_Init(SWI2C_GPIO_Port, &GPIO_InitStruct);
    }
}

void sda_out(char state)
{
    //SDA_DIR(1);
    if (state)
        HAL_GPIO_WritePin(SWI2C_GPIO_Port, SWI2C_SDA_Pin, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(SWI2C_GPIO_Port, SWI2C_SDA_Pin, GPIO_PIN_RESET);
}

char sda_in(void)
{
    if ((HAL_GPIO_ReadPin(SWI2C_GPIO_Port, SWI2C_SDA_Pin)) == GPIO_PIN_SET)
        return 1;
    else
        return 0;
}

#define scl_high()  scl_out(1);
#define scl_low()   scl_out(0);
//#define sda_high()  sda_out(1);
//#define sda_low()   sda_out(0);
#define sda_high()  sda_dir(0);
#define sda_low()   sda_dir(1);sda_out(0)


void swi2c_init()
{
    if (SWI2C_GPIO_Port == GPIOA_BASE)
        __GPIOA_CLK_ENABLE();
    else if (SWI2C_GPIO_Port == GPIOB_BASE)
        __GPIOB_CLK_ENABLE();
    else if (SWI2C_GPIO_Port == GPIOB_BASE)
        __GPIOC_CLK_ENABLE();
#if defined GPIOD_BASE
    else if (SWI2C_GPIO_Port == GPIOD_BASE)
        __GPIOD_CLK_ENABLE();
#endif

    scl_dir(1);
    scl_out(0);
    sda_dir(1);
    sda_out(0);
}


void swi2c_start()
{
    scl_low();
    swi2c_wait();
    sda_high();
    swi2c_wait();
    scl_high();
    swi2c_wait();
    sda_low();
    swi2c_wait();
    scl_low();
}


void swi2c_stop()
{
    scl_low();
    sda_low();
    swi2c_wait();
    scl_high();
    swi2c_wait();
    sda_high();
}


SWI2C_Ack_t swi2c_writebyte(uint8_t ch)
{
    int i = 0;

    scl_low();
    for (i = 8; i; --i) {
        if (ch & 0x80) {
            sda_high();
        } else {
            sda_low();
        }
        swi2c_wait();
        scl_high();
        ch <<= 1;
        swi2c_wait();
        scl_low();
    }
    sda_high();
    swi2c_wait();
    scl_high();
    swi2c_wait();
    i = sda_in() ? SWI2C_NAK: SWI2C_ACK;
    scl_low();

    return((SWI2C_Ack_t)i);
}


uint8_t swi2c_readbyte(SWI2C_Ack_t Ack)
{
    uint8_t ch = 0;
    int i = 0;

    sda_high();
    for (i = 8; i; --i) {
        scl_high();
        ch <<= 1;
        ch |= sda_in() ? 1:0;
        scl_low();
    }
    if (Ack) {
    	sda_low();
    }
    scl_high();
    swi2c_wait();
    scl_low();

    return ch;
}
