/*
 *----------------------------------------------------------------------
 * Copyright (C) Seong-Woo Kim, 2016
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
#include "../stm32lib/adc.h"

/* Private functions */
static void adc_initpin(GPIO_Pin_t Pin);

/* Private variables */
ADC_HandleTypeDef AdcHandle;

void adc_init(ADC_TypeDef* ADCx, uint8_t channel) {
    if (ADCx == ADC1
#if defined(ADC2)
    || ADCx == ADC2
#endif    
    ) {
        if (channel == 0) {
            adc_initpin(GPIO_PA_0);
        } else if (channel == 1) {
            adc_initpin(GPIO_PA_1);
        } else if (channel == 2) {
            adc_initpin(GPIO_PA_2);
        } else if (channel == 3) {
            adc_initpin(GPIO_PA_3);
        } else if (channel == 4) {
            adc_initpin(GPIO_PA_4);
        } else if (channel == 5) {
            adc_initpin(GPIO_PA_5);
        } else if (channel == 6) {
            adc_initpin(GPIO_PA_6);
        } else if (channel == 7) {
            adc_initpin(GPIO_PA_7);
        } else if (channel == 8) {
            adc_initpin(GPIO_PB_0);
        } else if (channel == 9) {
            adc_initpin(GPIO_PB_1);
        } else if (channel == 10) {
            adc_initpin(GPIO_PC_0);
        } else if (channel == 11) {
            adc_initpin(GPIO_PC_1);
        } else if (channel == 12) {
            adc_initpin(GPIO_PC_2);
        } else if (channel == 13) {
            adc_initpin(GPIO_PC_3);
        } else if (channel == 14) {
            adc_initpin(GPIO_PC_4);
        } else if (channel == 15) {
            adc_initpin(GPIO_PC_5);
        }
    }
#if defined(ADC3) && defined(GPIOF)
    else if (ADCx == ADC3) {
        if (channel == 0) {
            adc_initpin(GPIO_PA_0);
        } else if (channel == 1) {
            adc_initpin(GPIO_PA_1);
        } else if (channel == 2) {
            adc_initpin(GPIO_PA_2);
        } else if (channel == 3) {
            adc_initpin(GPIO_PA_3);
        } else if (channel == 4) {
            adc_initpin(GPIO_PF_6);
        } else if (channel == 5) {
            adc_initpin(GPIO_PF_7);
        } else if (channel == 6) {
            adc_initpin(GPIO_PF_8);
        } else if (channel == 7) {
            adc_initpin(GPIO_PF_9);
        } else if (channel == 8) {
            adc_initpin(GPIO_PF_10);
        } else if (channel == 9) {
            adc_initpin(GPIO_PF_11);
        } else if (channel == 10) {
            adc_initpin(GPIO_PC_0);
        } else if (channel == 11) {
            adc_initpin(GPIO_PC_1);
        } else if (channel == 12) {
            adc_initpin(GPIO_PC_2);
        } else if (channel == 13) {
            adc_initpin(GPIO_PC_3);
        } else if (channel == 14) {
            adc_initpin(GPIO_PF_4);
        } else if (channel == 15) {
            adc_initpin(GPIO_PF_5);
        }
    }
#endif

    /* Init ADC */
    adc_initADC(ADCx);
}

void adc_initADC(ADC_TypeDef* ADCx) {
    /* Enable clock */
#if defined(ADC1)
    if (ADCx == ADC1)
        __HAL_RCC_ADC1_CLK_ENABLE();
#endif
#if defined(ADC2)
    if (ADCx == ADC2)
        __HAL_RCC_ADC2_CLK_ENABLE();
#endif
#if defined(ADC3)
    if (ADCx == ADC3)
        __HAL_RCC_ADC3_CLK_ENABLE();
#endif
    
    /* Configure the ADC peripheral */
    AdcHandle.Instance = ADCx;

    /* Fill settings */
    AdcHandle.Init.Resolution = ADC_RESOLUTION_12B;
    AdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    AdcHandle.Init.ScanConvMode = DISABLE;
    AdcHandle.Init.ContinuousConvMode = DISABLE;
    AdcHandle.Init.DiscontinuousConvMode = DISABLE;
    AdcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    AdcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    AdcHandle.Init.DMAContinuousRequests = DISABLE;
    
#if defined(STM32F0xx)
    AdcHandle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    AdcHandle.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
    AdcHandle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    AdcHandle.Init.LowPowerAutoWait = DISABLE;
    AdcHandle.Init.LowPowerAutoPowerOff  = DISABLE;
#else
    AdcHandle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
    AdcHandle.Init.NbrOfDiscConversion = 0;
    AdcHandle.Init.NbrOfConversion = 1;
    AdcHandle.Init.EOCSelection = DISABLE;
#endif

    /* Init ADC */
    HAL_ADC_Init(&AdcHandle);
}

uint16_t adc_read(ADC_TypeDef* ADCx, uint8_t channel) {
    ADC_ChannelConfTypeDef sConfig;
    
    /* Configure ADC regular channel */  
    switch (channel) {
    case 0:
        sConfig.Channel = ADC_CHANNEL_0; break;
    case 1:
        sConfig.Channel = ADC_CHANNEL_1; break;
    case 2:
        sConfig.Channel = ADC_CHANNEL_2; break;
    case 3:
        sConfig.Channel = ADC_CHANNEL_3; break;
    case 4:
        sConfig.Channel = ADC_CHANNEL_4; break;
    case 5:
        sConfig.Channel = ADC_CHANNEL_5; break;
    case 6:
        sConfig.Channel = ADC_CHANNEL_6; break;
    case 7:
        sConfig.Channel = ADC_CHANNEL_7; break;
    case 8:
        sConfig.Channel = ADC_CHANNEL_8; break;
    case 9:
        sConfig.Channel = ADC_CHANNEL_9; break;
    case 10:
        sConfig.Channel = ADC_CHANNEL_10; break;
    case 11:
        sConfig.Channel = ADC_CHANNEL_11; break;
    case 12:
        sConfig.Channel = ADC_CHANNEL_12; break;
    case 13:
        sConfig.Channel = ADC_CHANNEL_13; break;
    case 14:
        sConfig.Channel = ADC_CHANNEL_14; break;
    case 15:
        sConfig.Channel = ADC_CHANNEL_15; break;
    case 16:
        sConfig.Channel = ADC_CHANNEL_16; break;
    case 17:
        sConfig.Channel = ADC_CHANNEL_17; break;
    case 18:
        sConfig.Channel = ADC_CHANNEL_18; break;
    default:
        sConfig.Channel = channel; break;
    }
    sConfig.Rank = 1;
#if defined(STM32F0xx)
    sConfig.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
#else
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    sConfig.Offset = 0;
#endif

    /* Set handle */
    AdcHandle.Instance = ADCx;

    /* Return zero */
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK) {
        return 0;
    }

    /* Start conversion */  
    if (HAL_ADC_Start(&AdcHandle) != HAL_OK) {
        return 0;
    }

#if defined(STM32F0xx)
    /* Poll for end */
    if (HAL_ADC_PollForConversion(&AdcHandle, 10) == HAL_OK) {
        /* Get the converted value of regular channel */
        return HAL_ADC_GetValue(&AdcHandle);
    }
#else
    /* Poll for end */
    if (HAL_ADC_PollForConversion(&AdcHandle, 10) == HAL_OK) {
        /* Check if the continous conversion of regular channel is finished */
        if ((HAL_ADC_GetState(&AdcHandle) & HAL_ADC_STATE_EOC_REG)
            == HAL_ADC_STATE_EOC_REG) {
            /* Get the converted value of regular channel */
            return HAL_ADC_GetValue(&AdcHandle);
        }
    }
#endif
    
    /* Return zero */
    return 0;
}

#if defined(ADC_CCR_TSVREFE)
float adc_readTempSensor(void) {
    uint32_t result;
    float temp;
    
    /* Read battery voltage */
    result = adc_read(ADC1, ADC_CHANNEL_TEMPSENSOR);
    
    /* Convert to voltage */
    result = result * ADC_SUPPLY_VOLTAGE / 0xFFF;
    temp = (float)(result - ADC_V25) / ADC_AVG_SLOPE + 25.;
    
    /* Return value in Celsius */
    return temp;
}
#endif

#if defined(ADC_CCR_VBATEN) || defined(ADC_CCR_VBATE)
uint16_t adc_readVbat(void) {
    uint32_t result;
    
    /* Read battery voltage */
    result = adc_read(ADC1, ADC_CHANNEL_VBAT);
    
    /* Convert to voltage */
    result = result * ADC_VBAT_MULTI * ADC_SUPPLY_VOLTAGE / 0xFFF;
    
    /* Return value in mV */
    return (uint16_t) result;
}
#endif

static void adc_initpin(GPIO_Pin_t Pin) {
    /* Enable GPIO pin */
    gpio_alternate_init(Pin, GPIO_AN, 0);
    gpio_set_pinmode(Pin, GPIO_PM_PULLDOWN);
}
