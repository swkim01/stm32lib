#ifndef MPU6050_H
#define MPU6050_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"
#include "swi2c.h"

/* 기본 I2C 클록 */
#ifndef MPU6050_I2C_CLOCK
#define MPU6050_I2C_CLOCK        400000            /* 기본 I2C 클록 속도 */
#endif

#define MPU6050_RATE_8KHZ       0   /* 샘플레이트를 8 kHz 로 설정 */
#define MPU6050_RATE_4KHZ       1   /* 샘플레이트를 4 kHz 로 설정 */
#define MPU6050_RATE_2KHZ       3   /* 샘플레이트를 2 kHz 로 설정 */
#define MPU6050_RATE_1KHZ       7   /* 샘플레이트를 1 kHz 로 설정 */
#define MPU6050_RATE_500HZ      15  /* 샘플레이트를 500 Hz 로 설정 */
#define MPU6050_RATE_250HZ      31  /* 샘플레이트를 250 Hz 로 설정 */
#define MPU6050_RATE_125HZ      63  /* 샘플레이트를 125 Hz 로 설정 */
#define MPU6050_RATE_100HZ      79  /* 샘플레이트를 100 Hz 로 설정 */
 
/* MPU6050 장치 주소(2가지) */
typedef enum {
    MPU6050_DEV_0 = 0x00,       /* AD0 핀이 low 로 설정 */
    MPU6050_DEV_1 = 0x02        /* AD0 핀이 high 로 설정 */
} MPU6050_Dev_t;

/* 결과 열거형 */
typedef enum {
    MPU6050_RES_OK = 0x00,      /* 정상 반환 */
    MPU6050_RES_ERR,            /* 오류 */
    MPU6050_RES_NOTCONNECT,     /* 연결된 장치가 없음 */
    MPU6050_RES_INVALID         /* 연결된 장치 주소가 MPU6050이 아님 */
} MPU6050_Res_t;

/* 가속도계 범위 열거형 */
typedef enum {
    MPU6050_ACC_2G = 0x00,      /* +- 2G 범위 */
    MPU6050_ACC_4G = 0x01,      /* +- 4G 범위 */
    MPU6050_ACC_8G = 0x02,      /* +- 8G 범위 */
    MPU6050_ACC_16G = 0x03      /* +- 16G 범위 */
} MPU6050_Acc_t;

/* 자이로 범위 열거형 */
typedef enum {
    MPU6050_GYRO_250S = 0x00,   /* +- 250 degrees/s 범위 */
    MPU6050_GYRO_500S = 0x01,   /* +- 500 degrees/s 범위 */
    MPU6050_GYRO_1000S = 0x02,  /* +- 1000 degrees/s 범위 */
    MPU6050_GYRO_2000S = 0x03   /* +- 2000 degrees/s 범위 */

} MPU6050_Gyro_t;

/* 주 MPU6050 자료형 */
typedef struct _MPU6050_t {
    uint8_t address;            /*!< I2C 장치 주소 */
    float gyro_mult;            /* 자이로 raw 데이터를 degrees/s 로 변환한 값. */
    float acc_mult;             /* 가속도 raw 데이터를 g 로 변환한 값. */
    int16_t acc_x;              /* X 축 가속도 값 */
    int16_t acc_y;              /* Y 축 가속도 값 */
    int16_t acc_z;              /* Z 축 가속도 값 */
    int16_t gyro_x;             /* X 축 자이로 값 */
    int16_t gyro_y;             /* Y 축 자이로 값 */
    int16_t gyro_z;             /* Z 축 자이로 값 */
    float temperature;          /* 온도 값 */
} MPU6050_t;

/* MPU6050 및 I2C 장치를 초기화한다 */
MPU6050_Res_t mpu6050_init(MPU6050_t* mpu6050, MPU6050_Dev_t devnum, MPU6050_Acc_t acc, MPU6050_Gyro_t gyro);

/* 자이로 민감도를 설정한다 */
MPU6050_Res_t mpu6050_setgyro(MPU6050_t* mpu6050, MPU6050_Gyro_t gyro);

/* 가속도계 민감도를 설정한다 */
MPU6050_Res_t mpu6050_setacc(MPU6050_t* mpu6050, MPU6050_Acc_t acc);

/* 데이터 출력율을 설정한다 */
MPU6050_Res_t mpu6050_setrate(MPU6050_t* mpu6050, uint8_t rate);

/* 센서로부터 가속도 데이터를 읽는다 */
MPU6050_Res_t mpu6050_readacc(MPU6050_t* mpu6050);

/* 센서로부터 자이로 데이터를 읽는다 */
MPU6050_Res_t mpu6050_readgyro(MPU6050_t* mpu6050);

/* 센서로부터 온도 데이터를 읽는다 */
MPU6050_Res_t mpu6050_readtemp(MPU6050_t* mpu6050);

/* 센서로부터 가속도, 자이로 및 온도 데이터를 읽는다 */
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
