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
#include "../stm32lib/mpu6050.h"

/* 자신에 대한 레지스터 값 */
#define MPU6050_I_AM                0x68

/* MPU6050 레지스터들 */
#define MPU6050_AUX_VDDIO           0x01
#define MPU6050_SMPLRT_DIV          0x19
#define MPU6050_CONFIG              0x1A
#define MPU6050_GYRO_CONFIG         0x1B
#define MPU6050_ACCEL_CONFIG        0x1C
#define MPU6050_MOTION_THRESH       0x1F
#define MPU6050_INT_PIN_CFG         0x37
#define MPU6050_INT_ENABLE          0x38
#define MPU6050_INT_STATUS          0x3A
#define MPU6050_ACCEL_XOUT_H        0x3B
#define MPU6050_ACCEL_XOUT_L        0x3C
#define MPU6050_ACCEL_YOUT_H        0x3D
#define MPU6050_ACCEL_YOUT_L        0x3E
#define MPU6050_ACCEL_ZOUT_H        0x3F
#define MPU6050_ACCEL_ZOUT_L        0x40
#define MPU6050_TEMP_OUT_H          0x41
#define MPU6050_TEMP_OUT_L          0x42
#define MPU6050_GYRO_XOUT_H         0x43
#define MPU6050_GYRO_XOUT_L         0x44
#define MPU6050_GYRO_YOUT_H         0x45
#define MPU6050_GYRO_YOUT_L         0x46
#define MPU6050_GYRO_ZOUT_H         0x47
#define MPU6050_GYRO_ZOUT_L         0x48
#define MPU6050_MOT_DETECT_STATUS   0x61
#define MPU6050_SIGNAL_PATH_RESET   0x68
#define MPU6050_MOT_DETECT_CTRL     0x69
#define MPU6050_USER_CTRL           0x6A
#define MPU6050_PWR_MGMT_1          0x6B
#define MPU6050_PWR_MGMT_2          0x6C
#define MPU6050_FIFO_COUNTH         0x72
#define MPU6050_FIFO_COUNTL         0x73
#define MPU6050_FIFO_R_W            0x74
#define MPU6050_WHO_AM_I            0x75

/* 자이로스코프 범위 degrees/s */
#define MPU6050_GYRO_SENS_250       ((float) 131)
#define MPU6050_GYRO_SENS_500       ((float) 65.5)
#define MPU6050_GYRO_SENS_1000      ((float) 32.8)
#define MPU6050_GYRO_SENS_2000      ((float) 16.4)

/* 가속도 범위 g/s */
#define MPU6050_ACC_SENS_2          ((float) 16384)
#define MPU6050_ACC_SENS_4          ((float) 8192)
#define MPU6050_ACC_SENS_8          ((float) 4096)
#define MPU6050_ACC_SENS_16         ((float) 2048)


MPU6050_Res_t mpu6050_init(MPU6050_t* mpu6050, MPU6050_Dev_t devnum, MPU6050_Acc_t acc, MPU6050_Gyro_t gyro)
{
    uint8_t temp;
    
    /* I2C 주소 구성 */
    mpu6050->address = MPU6050_I2C_ADDR | (uint8_t)devnum;
    
    /* I2C 초기화 */
    i2c_init(MPU6050_I2C, MPU6050_I2C_PINSPACK);
    i2c_set_frequency(MPU6050_I2C, 400000);
    
    /* 장치 연결 점검 */
    if (i2c_ready(MPU6050_I2C, mpu6050->address) < 0) {
        /* 오류 반환 */
        return MPU6050_RES_NOTCONNECT;
    }
    
    /* MPU6050 장치 확인 */
    i2c_read(MPU6050_I2C, mpu6050->address, MPU6050_WHO_AM_I, &temp);
    if (temp != MPU6050_I_AM) {
        /* 오류 반환 */
        return MPU6050_RES_INVALID;
    }
    
    /* MPU6050 기동 */
    i2c_write(MPU6050_I2C, mpu6050->address, MPU6050_PWR_MGMT_1, 0x00);
    
    /* 샘플레이트를 1kHz로 설정 */
    mpu6050_setrate(mpu6050, MPU6050_RATE_1KHZ);
    
    /* 가속도계 설정 */
    mpu6050_setacc(mpu6050, acc);
    
    /* 자이로스코프 설정 */
    mpu6050_setgyro(mpu6050, gyro);
    
    /* 정상 반환 */
    return MPU6050_RES_OK;
}

MPU6050_Res_t mpu6050_setacc(MPU6050_t* mpu6050, MPU6050_Acc_t acc)
{
    uint8_t temp;
    
    /* 가속도계 설정 */
    i2c_read(MPU6050_I2C, mpu6050->address, MPU6050_ACCEL_CONFIG, &temp);
    temp = (temp & 0xE7) | (uint8_t)acc << 3;
    i2c_write(MPU6050_I2C, mpu6050->address, MPU6050_ACCEL_CONFIG, temp);
    
    switch (acc) {
    case MPU6050_ACC_2G:
        mpu6050->acc_mult = (float)1 / MPU6050_ACC_SENS_2; 
        break;
    case MPU6050_ACC_4G:
        mpu6050->acc_mult = (float)1 / MPU6050_ACC_SENS_4; 
        break;
    case MPU6050_ACC_8G:
        mpu6050->acc_mult = (float)1 / MPU6050_ACC_SENS_8; 
        break;
    case MPU6050_ACC_16G:
        mpu6050->acc_mult = (float)1 / MPU6050_ACC_SENS_16; 
    default:
        break;
    }
    
    /* 정상 반환 */
    return MPU6050_RES_OK;
}

MPU6050_Res_t mpu6050_setgyro(MPU6050_t* mpu6050, MPU6050_Gyro_t gyro)
{
    uint8_t temp;
    
    /* 자이로스코프 설정 */
    i2c_read(MPU6050_I2C, mpu6050->address, MPU6050_GYRO_CONFIG, &temp);
    temp = (temp & 0xE7) | (uint8_t)gyro << 3;
    i2c_write(MPU6050_I2C, mpu6050->address, MPU6050_GYRO_CONFIG, temp);
    
    switch (gyro) {
    case MPU6050_GYRO_250S:
        mpu6050->gyro_mult = (float)1 / MPU6050_GYRO_SENS_250; 
        break;
    case MPU6050_GYRO_500S:
        mpu6050->gyro_mult = (float)1 / MPU6050_GYRO_SENS_500; 
        break;
    case MPU6050_GYRO_1000S:
        mpu6050->gyro_mult = (float)1 / MPU6050_GYRO_SENS_1000; 
        break;
    case MPU6050_GYRO_2000S:
        mpu6050->gyro_mult = (float)1 / MPU6050_GYRO_SENS_2000; 
    default:
        break;
    }
    
    /* 정상 반환 */
    return MPU6050_RES_OK;
}

MPU6050_Res_t mpu6050_setrate(MPU6050_t* mpu6050, uint8_t rate)
{
    /* 데이터 처리율 설정 */
    if (i2c_write(MPU6050_I2C, mpu6050->address, MPU6050_SMPLRT_DIV, rate) < 0) {
        /* 오류 반환 */
        return MPU6050_RES_ERR;
    }
    
    /* 정상 반환 */
    return MPU6050_RES_OK;
}

MPU6050_Res_t mpu6050_readacc(MPU6050_t* mpu6050)
{
    uint8_t data[6];
    
    /* 가속도계 데이터 읽기 */
    i2c_nread(MPU6050_I2C, mpu6050->address, MPU6050_ACCEL_XOUT_H, data, 6);
    
    /* Format */
    mpu6050->acc_x = (int16_t)(data[0] << 8 | data[1]);    
    mpu6050->acc_y = (int16_t)(data[2] << 8 | data[3]);
    mpu6050->acc_z = (int16_t)(data[4] << 8 | data[5]);
    
    /* 정상 반환 */
    return MPU6050_RES_OK;
}

MPU6050_Res_t mpu6050_readgyro(MPU6050_t* mpu6050)
{
    uint8_t data[6];
    
    /* 자이로스코프 데이터 읽기 */
    i2c_nread(MPU6050_I2C, mpu6050->address, MPU6050_GYRO_XOUT_H, data, 6);
    
    /* Format */
    mpu6050->gyro_x = (int16_t)(data[0] << 8 | data[1]);
    mpu6050->gyro_y = (int16_t)(data[2] << 8 | data[3]);
    mpu6050->gyro_z = (int16_t)(data[4] << 8 | data[5]);

    /* 정상 반환 */
    return MPU6050_RES_OK;
}

MPU6050_Res_t mpu6050_readtemp(MPU6050_t* mpu6050)
{
    uint8_t data[2];
    int16_t temp;
    
    /* 온도 데이터 읽기 */
    i2c_nread(MPU6050_I2C, mpu6050->address, MPU6050_TEMP_OUT_H, data, 2);
    
    /* 온도 값 구성 */
    temp = (data[0] << 8 | data[1]);
    mpu6050->temperature = (float)((int16_t)temp / (float)340.0 + (float)36.53);
    
    /* 정상 반환 */
    return MPU6050_RES_OK;
}

MPU6050_Res_t mpu6050_readall(MPU6050_t* mpu6050)
{
    uint8_t data[14];
    int16_t temp;
    
    /* 전체 원시 데이터(14바이트) 읽기 */
    i2c_nread(MPU6050_I2C, mpu6050->address, MPU6050_ACCEL_XOUT_H, data, 14);
    
    /* 가속도 값 구성 */
    mpu6050->acc_x = (int16_t)(data[0] << 8 | data[1]);    
    mpu6050->acc_y = (int16_t)(data[2] << 8 | data[3]);
    mpu6050->acc_z = (int16_t)(data[4] << 8 | data[5]);

    /* 온도 값 구성 */
    temp = (data[6] << 8 | data[7]);
    mpu6050->temperature = (float)((float)((int16_t)temp) / (float)340.0 + (float)36.53);
    
    /* 자이로스코프 값 구성 */
    mpu6050->gyro_x = (int16_t)(data[8] << 8 | data[9]);
    mpu6050->gyro_y = (int16_t)(data[10] << 8 | data[11]);
    mpu6050->gyro_z = (int16_t)(data[12] << 8 | data[13]);

    /* 정상 반환 */
    return MPU6050_RES_OK;
}
