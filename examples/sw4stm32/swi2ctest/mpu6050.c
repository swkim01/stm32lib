#include "mpu6050.h"

/* Default I2C address */
#define MPU6050_I2C_ADDR            0xD0

/* Who I am register value */
#define MPU6050_I_AM                0x68

/* MPU6050 registers */
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

/* Gyro sensitivities in degrees/s */
#define MPU6050_GYRO_SENS_250       ((float) 131)
#define MPU6050_GYRO_SENS_500       ((float) 65.5)
#define MPU6050_GYRO_SENS_1000      ((float) 32.8)
#define MPU6050_GYRO_SENS_2000      ((float) 16.4)

/* Acc sensitivities in g/s */
#define MPU6050_ACCE_SENS_2         ((float) 16384)
#define MPU6050_ACCE_SENS_4         ((float) 8192)
#define MPU6050_ACCE_SENS_8         ((float) 4096)
#define MPU6050_ACCE_SENS_16        ((float) 2048)


MPU6050_Res_t mpu6050_init(MPU6050_t* mpu6050, MPU6050_Dev_t devnum, MPU6050_Acc_t acc, MPU6050_Gyro_t gyro)
{
    uint8_t temp;
    
    /* Format I2C address */
    mpu6050->address = MPU6050_I2C_ADDR | (uint8_t)devnum;

    /* Initialize I2C */
    swi2c_init();
    
    /* Check if device is connected */
    if (swi2c_ready(mpu6050->address) != SWI2C_RES_OK) {
        /* Return error */
        return MPU6050_RES_NOTCONNECT;
    }
    
    /* Check who am I */
    swi2c_read(mpu6050->address, MPU6050_WHO_AM_I, &temp);
    if (temp != MPU6050_I_AM) {
        /* Return error */
        return MPU6050_RES_INVALID;
    }

    /* Wakeup MPU6050 */
    swi2c_write(mpu6050->address, MPU6050_PWR_MGMT_1, 0x00);
    
    /* Set sample rate to 1kHz */
    mpu6050_setrate(mpu6050, MPU6050_RATE_1KHZ);
    
    /* Config accelerometer */
    mpu6050_setacc(mpu6050, acc);
    
    /* Config accelerometer */
    mpu6050_setgyro(mpu6050, gyro);
    
    /* Return OK */
    return MPU6050_RES_OK;
}

MPU6050_Res_t mpu6050_setgyro(MPU6050_t* mpu6050, MPU6050_Gyro_t gyro)
{
    uint8_t temp;
    
    /* Config gyroscope */
    swi2c_read(mpu6050->address, MPU6050_GYRO_CONFIG, &temp);
    temp = (temp & 0xE7) | (uint8_t)gyro << 3;
    swi2c_write(mpu6050->address, MPU6050_GYRO_CONFIG, temp);
    
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
    
    /* Return OK */
    return MPU6050_RES_OK;
}

MPU6050_Res_t mpu6050_setacc(MPU6050_t* mpu6050, MPU6050_Acc_t acc)
{
    uint8_t temp;
    
    /* Config accelerometer */
    swi2c_read(mpu6050->address, MPU6050_ACCEL_CONFIG, &temp);
    temp = (temp & 0xE7) | (uint8_t)acc << 3;
    swi2c_write(mpu6050->address, MPU6050_ACCEL_CONFIG, temp);
    
    /* Set sensitivities for multiplying gyro and accelerometer data */
    switch (acc) {
    case MPU6050_ACC_2G:
        mpu6050->acc_mult = (float)1 / MPU6050_ACCE_SENS_2; 
        break;
    case MPU6050_ACC_4G:
        mpu6050->acc_mult = (float)1 / MPU6050_ACCE_SENS_4; 
        break;
    case MPU6050_ACC_8G:
        mpu6050->acc_mult = (float)1 / MPU6050_ACCE_SENS_8; 
        break;
    case MPU6050_ACC_16G:
        mpu6050->acc_mult = (float)1 / MPU6050_ACCE_SENS_16; 
    default:
        break;
    }
    
    /* Return OK */
    return MPU6050_RES_OK;
}

MPU6050_Res_t mpu6050_setrate(MPU6050_t* mpu6050, uint8_t rate)
{
    /* Set data sample rate */
    if (swi2c_write(mpu6050->address, MPU6050_SMPLRT_DIV, rate) != SWI2C_RES_OK) {
        /* Return error */
        return MPU6050_RES_ERR;
    }
    
    /* Return OK */
    return MPU6050_RES_OK;
}

MPU6050_Res_t mpu6050_readacc(MPU6050_t* mpu6050) {
    uint8_t data[6];
    
    /* Read accelerometer data */
    swi2c_readmulti(mpu6050->address, MPU6050_ACCEL_XOUT_H, data, 6);
    
    /* Format */
    mpu6050->acc_x = (int16_t)(data[0] << 8 | data[1]);    
    mpu6050->acc_y = (int16_t)(data[2] << 8 | data[3]);
    mpu6050->acc_z = (int16_t)(data[4] << 8 | data[5]);
    
    /* Return OK */
    return MPU6050_RES_OK;
}

MPU6050_Res_t mpu6050_readgyro(MPU6050_t* mpu6050)
{
    uint8_t data[6];
    
    /* Read gyroscope data */
    swi2c_readmulti(mpu6050->address, MPU6050_GYRO_XOUT_H, data, 6);
    
    /* Format */
    mpu6050->gyro_x = (int16_t)(data[0] << 8 | data[1]);
    mpu6050->gyro_y = (int16_t)(data[2] << 8 | data[3]);
    mpu6050->gyro_z = (int16_t)(data[4] << 8 | data[5]);

    /* Return OK */
    return MPU6050_RES_OK;
}

MPU6050_Res_t mpu6050_readtemp(MPU6050_t* mpu6050)
{
    uint8_t data[2];
    int16_t temp;
    
    /* Read temperature */
    swi2c_readmulti(mpu6050->address, MPU6050_TEMP_OUT_H, data, 2);
    
    /* Format temperature */
    temp = (data[0] << 8 | data[1]);
    mpu6050->temperature = (float)((int16_t)temp / (float)340.0 + (float)36.53);
    
    /* Return OK */
    return MPU6050_RES_OK;
}

MPU6050_Res_t mpu6050_readall(MPU6050_t* mpu6050)
{
    uint8_t data[14];
    int16_t temp;
    
    /* Read full raw data, 14bytes */
    swi2c_readmulti(mpu6050->address, MPU6050_ACCEL_XOUT_H, data, 14);
    
    /* Format accelerometer data */
    mpu6050->acc_x = (int16_t)(data[0] << 8 | data[1]);    
    mpu6050->acc_y = (int16_t)(data[2] << 8 | data[3]);
    mpu6050->acc_z = (int16_t)(data[4] << 8 | data[5]);

    /* Format temperature */
    temp = (data[6] << 8 | data[7]);
    mpu6050->temperature = (float)((float)((int16_t)temp) / (float)340.0 + (float)36.53);
    
    /* Format gyroscope data */
    mpu6050->gyro_x = (int16_t)(data[8] << 8 | data[9]);
    mpu6050->gyro_y = (int16_t)(data[10] << 8 | data[11]);
    mpu6050->gyro_z = (int16_t)(data[12] << 8 | data[13]);

    /* Return OK */
    return MPU6050_RES_OK;
}
