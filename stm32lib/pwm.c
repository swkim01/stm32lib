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
#include "../stm32lib/pwm.h"

static TIM_HandleTypeDef TimHandle;
static PWM_Pin_t pwm_pin[] = {
    {PWM_2, GPIO_PA_0, GPIO_AF1_TIM2, 1, 0, 0, 0, 0},
//    {PWM_5, GPIO_PA_0, GPIO_AF2_TIM5, 1, 0, 0, 0, 0},
    {PWM_2, GPIO_PA_1, GPIO_AF1_TIM2, 2, 0, 0, 0, 0},
//    {PWM_5, GPIO_PA_1, GPIO_AF2_TIM5, 2, 0, 0, 0, 0},
    {PWM_2, GPIO_PA_2, GPIO_AF1_TIM2, 3, 0, 0, 0, 0},
//    {PWM_5, GPIO_PA_2, GPIO_AF2_TIM5, 3, 0, 0, 0, 0},
//    {PWM_9, GPIO_PA_2, GPIO_AF3_TIM9, 1, 0, 0, 0, 0},
    {PWM_2, GPIO_PA_3, GPIO_AF1_TIM2, 4, 0, 0, 0, 0},
//    {PWM_5, GPIO_PA_3, GPIO_AF2_TIM5, 4, 0, 0, 0, 0},
//    {PWM_9, GPIO_PA_3, GPIO_AF3_TIM9, 2, 0, 0, 0, 0},
    {PWM_2, GPIO_PA_5, GPIO_AF1_TIM2, 1, 0, 0, 0, 0},
#if defined(TIM8_BASE)
//    {PWM_8, GPIO_PA_5, GPIO_AF3_TIM8, 1, 1, 0, 0, 0},
#endif
    {PWM_3, GPIO_PA_6, GPIO_AF2_TIM3, 1, 0, 0, 0, 0},
#if defined(TIM13_BASE)
//    {PWM_13, GPIO_PA_6, GPIO_AF9_TIM13, 1, 0, 0, 0, 0},
#endif
    {PWM_1, GPIO_PA_7, GPIO_AF1_TIM1, 1, 1, 0, 0, 0},
//    {PWM_3, GPIO_PA_7, GPIO_AF2_TIM3, 2, 0, 0, 0, 0},
#if defined(TIM8_BASE)
//    {PWM_8, GPIO_PA_7, GPIO_AF3_TIM8, 1, 1, 0, 0, 0},
#endif
#if defined(TIM14_BASE)
//    {PWM_14, GPIO_PA_7, GPIO_AF9_TIM14, 1, 0, 0, 0, 0},
#endif
    {PWM_1, GPIO_PA_8, GPIO_AF1_TIM1, 1, 0, 0, 0, 0},
    {PWM_1, GPIO_PA_9, GPIO_AF1_TIM1, 2, 0, 0, 0, 0},
    {PWM_1, GPIO_PA_10, GPIO_AF1_TIM1, 3, 0, 0, 0, 0},
    {PWM_1, GPIO_PA_11, GPIO_AF1_TIM1, 4, 0, 0, 0, 0},
    {PWM_2, GPIO_PA_15, GPIO_AF1_TIM2, 1, 0, 0, 0, 0},
    {PWM_1, GPIO_PB_0, GPIO_AF1_TIM1, 2, 1, 0, 0, 0},
//    {PWM_3, GPIO_PB_0, GPIO_AF2_TIM3, 3, 0, 0, 0, 0},
#if defined(TIM8_BASE)
//    {PWM_8, GPIO_PB_0, GPIO_AF3_TIM8, 2, 1, 0, 0, 0},
#endif
    {PWM_1, GPIO_PB_1, GPIO_AF1_TIM1, 3, 1, 0, 0, 0},
//    {PWM_3, GPIO_PB_1, GPIO_AF2_TIM3, 4, 0, 0, 0, 0},
#if defined(TIM8_BASE)
//    {PWM_8, GPIO_PB_1, GPIO_AF3_TIM8, 3, 1, 0, 0, 0},
#endif
    {PWM_2, GPIO_PB_2, GPIO_AF1_TIM2, 4, 0, 0, 0, 0},
    {PWM_2, GPIO_PB_3, GPIO_AF1_TIM2, 2, 0, 0, 0, 0},
    {PWM_3, GPIO_PB_4, GPIO_AF2_TIM3, 1, 0, 0, 0, 0},
    {PWM_3, GPIO_PB_5, GPIO_AF2_TIM3, 2, 0, 0, 0, 0},
    {PWM_4, GPIO_PB_6, GPIO_AF2_TIM4, 1, 0, 0, 0, 0},
    {PWM_4, GPIO_PB_7, GPIO_AF2_TIM4, 2, 0, 0, 0, 0},
    {PWM_4, GPIO_PB_8, GPIO_AF2_TIM4, 3, 0, 0, 0, 0},
//    {PWM_2, GPIO_PB_8, GPIO_AF1_TIM2, 1, 0, 0, 0, 0},
//    {PWM_10, GPIO_PB_8, GPIO_AF3_TIM10, 1, 0, 0, 0, 0},
    {PWM_4, GPIO_PB_9, GPIO_AF2_TIM4, 4, 0, 0, 0, 0},
//    {PWM_2, GPIO_PB_9, GPIO_AF1_TIM2, 2, 0, 0, 0, 0},
//    {PWM_11, GPIO_PB_9, GPIO_AF3_TIM11, 1, 0, 0, 0, 0},
    {PWM_2, GPIO_PB_10, GPIO_AF1_TIM2, 3, 0, 0, 0, 0},
    {PWM_1, GPIO_PB_13, GPIO_AF1_TIM1, 1, 1, 0, 0, 0},
    {PWM_1, GPIO_PB_14, GPIO_AF1_TIM1, 2, 1, 0, 0, 0},
#if defined(TIM8_BASE)
//    {PWM_8, GPIO_PB_14, GPIO_AF3_TIM8, 2, 1, 0, 0, 0},
#endif
#if defined(TIM12_BASE)
//    {PWM_12, GPIO_PB_14, GPIO_AF9_TIM12, 1, 0, 0, 0, 0},
#endif
    {PWM_1, GPIO_PB_15, GPIO_AF1_TIM1, 3, 1, 0, 0, 0},
#if defined(TIM8_BASE)
//    {PWM_8, GPIO_PB_15, GPIO_AF8_TIM8, 3, 1, 0, 0, 0},
#endif
#if defined(TIM12_BASE)
//    {PWM_12, GPIO_PB_15, GPIO_AF9_TIM12, 2, 0, 0, 0, 0},
#endif
    {PWM_3, GPIO_PC_6, GPIO_AF2_TIM3, 1, 0, 0, 0, 0},
#if defined(TIM8_BASE)
//    {PWM_8, GPIO_PC_6, GPIO_AF3_TIM8, 1, 0, 0, 0, 0},
#endif
    {PWM_3, GPIO_PC_7, GPIO_AF2_TIM3, 2, 0, 0, 0, 0},
#if defined(TIM8_BASE)
//    {PWM_8, GPIO_PC_7, GPIO_AF3_TIM8, 2, 0, 0, 0, 0},
#endif
    {PWM_3, GPIO_PC_8, GPIO_AF2_TIM3, 3, 0, 0, 0, 0},
#if defined(TIM8_BASE)
//    {PWM_8, GPIO_PC_8, GPIO_AF3_TIM8, 3, 0, 0, 0, 0},
#endif
    {PWM_3, GPIO_PC_9, GPIO_AF2_TIM3, 4, 0, 0, 0, 0},
#if defined(TIM8_BASE)
//    {PWM_8, GPIO_PC_9, GPIO_AF3_TIM8, 4, 0, 0, 0, 0},
#endif
// for Discovery-F429ZI, F407VG, F469NI, Nucleo-F427ZI, F446ZE, etc.
//    {PWM_4, GPIO_PD_12, GPIO_AF2_TIM4, 1, 0, 0, 0, 0},
//    {PWM_4, GPIO_PD_13, GPIO_AF2_TIM4, 2, 0, 0, 0, 0},
//    {PWM_4, GPIO_PD_14, GPIO_AF2_TIM4, 3, 0, 0, 0, 0},
//    {PWM_4, GPIO_PD_15, GPIO_AF2_TIM4, 4, 0, 0, 0, 0},
//    {PWM_9, GPIO_PE_5, GPIO_AF3_TIM9, 1, 0, 0, 0, 0},
//    {PWM_9, GPIO_PE_6, GPIO_AF3_TIM9, 2, 0, 0, 0, 0},
//    {PWM_1, GPIO_PE_8, GPIO_AF1_TIM1, 1, 1, 0, 0, 0},
//    {PWM_1, GPIO_PE_9, GPIO_AF1_TIM1, 1, 0, 0, 0, 0},
//    {PWM_1, GPIO_PE_10, GPIO_AF1_TIM1, 2, 1, 0, 0, 0},
//    {PWM_1, GPIO_PE_11, GPIO_AF1_TIM1, 2, 0, 0, 0, 0},
//    {PWM_1, GPIO_PE_12, GPIO_AF1_TIM1, 3, 1, 0, 0, 0},
//    {PWM_1, GPIO_PE_13, GPIO_AF1_TIM1, 3, 0, 0, 0, 0},
//    {PWM_1, GPIO_PE_14, GPIO_AF1_TIM1, 4, 0, 0, 0, 0},
//    {PWM_10, GPIO_PF_6, GPIO_AF3_TIM10, 1, 0, 0, 0, 0},
//    {PWM_11, GPIO_PF_7, GPIO_AF3_TIM11, 1, 0, 0, 0, 0},
//    {PWM_13, GPIO_PF_8, GPIO_AF9_TIM13, 1, 0, 0, 0, 0},
//    {PWM_14, GPIO_PF_9, GPIO_AF9_TIM14, 1, 0, 0, 0, 0},
};

/* Private functions */

void pwm_init(PWM_PinIndex_t index)
{
    // Get the pwm pin info struct
    assert_param(IS_PWM_PIN_INDEX(pwm_pin, index));
    PWM_Pin_t *pin = &pwm_pin[index];

    // Enable TIM clock
#if defined(TIM1_BASE)
    if (pin->tim == PWM_1) __HAL_RCC_TIM1_CLK_ENABLE();
#endif
#if defined(TIM2_BASE)
    if (pin->tim == PWM_2) __HAL_RCC_TIM2_CLK_ENABLE();
#endif
#if defined(TIM3_BASE)
    if (pin->tim == PWM_3) __HAL_RCC_TIM3_CLK_ENABLE();
#endif
#if defined(TIM4_BASE)
    if (pin->tim == PWM_4) __HAL_RCC_TIM4_CLK_ENABLE();
#endif
#if defined(TIM5_BASE)
    if (pin->tim == PWM_5) __HAL_RCC_TIM5_CLK_ENABLE();
#endif
#if defined(TIM8_BASE)
    if (pin->tim == PWM_8) __HAL_RCC_TIM8_CLK_ENABLE();
#endif
#if defined(TIM9_BASE)
    if (pin->tim == PWM_9) __HAL_RCC_TIM9_CLK_ENABLE();
#endif
#if defined(TIM10_BASE)
    if (pin->tim == PWM_10) __HAL_RCC_TIM10_CLK_ENABLE();
#endif
#if defined(TIM11_BASE)
    if (pin->tim == PWM_11) __HAL_RCC_TIM11_CLK_ENABLE();
#endif
#if defined(TIM12_BASE)
    if (pin->tim == PWM_12) __HAL_RCC_TIM12_CLK_ENABLE();
#endif
#if defined(TIM13_BASE)
    if (pin->tim == PWM_13) __HAL_RCC_TIM13_CLK_ENABLE();
#endif
#if defined(TIM14_BASE)
    if (pin->tim == PWM_14) __HAL_RCC_TIM14_CLK_ENABLE();
#endif

    // Configure GPIO
    gpio_alternate_init(pin->gpio, GPIO_AF, pin->alternate);
    gpio_set_pinmode(pin->gpio, GPIO_PM_PULLDOWN);

    pin->period = 0;
    pin->pulse = 0;
    pin->prescaler = 1;

    pwm_period_us(index, 20000); // 20 ms per default
}

void pwm_deinit(PWM_PinIndex_t index)
{
    // Get the pwm pin info struct
    assert_param(IS_PWM_PIN_INDEX(pwm_pin, index));
    PWM_Pin_t *pin = &pwm_pin[index];

    // Configure GPIO
    gpio_alternate_init(pin->gpio, GPIO_IN, 0);
}

void pwm_write(PWM_PinIndex_t index, float value)
{
    // Get the pwm pin info struct
    assert_param(IS_PWM_PIN_INDEX(pwm_pin, index));
    PWM_Pin_t *pin = &pwm_pin[index];

    TIM_OC_InitTypeDef sConfig;
    int channel = 0;

    TimHandle.Instance = (TIM_TypeDef *)(pin->tim);

    if (value < (float)0.0) {
        value = 0.0;
    } else if (value > (float)1.0) {
        value = 1.0;
    }

    pin->pulse = (uint32_t)((float)pin->period * value);

    // Configure channels
    sConfig.OCMode       = TIM_OCMODE_PWM1;
    sConfig.Pulse        = pin->pulse / pin->prescaler;
    sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
    sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
    sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
    sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;

    switch (pin->channel) {
    case 1:
        channel = TIM_CHANNEL_1;
        break;
    case 2:
        channel = TIM_CHANNEL_2;
        break;
    case 3:
        channel = TIM_CHANNEL_3;
        break;
    case 4:
        channel = TIM_CHANNEL_4;
        break;
    default:
        return;
    }

    if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, channel) != HAL_OK) {
        printf("Cannot initialize PWM\n");
    }

    if (pin->invert) {
        HAL_TIMEx_PWMN_Start(&TimHandle, channel);
    } else {
        HAL_TIM_PWM_Start(&TimHandle, channel);
    }
}

float pwm_read(PWM_PinIndex_t index)
{
    // Get the pwm pin info struct
    assert_param(IS_PWM_PIN_INDEX(pwm_pin, index));
    PWM_Pin_t *pin = &pwm_pin[index];
    float value = 0;

    if (pin->period > 0) {
        value = (float)(pin->pulse) / (float)(pin->period);
    }
    return ((value > (float)1.0) ? (float)(1.0) : (value));
}

void pwm_period(PWM_PinIndex_t index, float seconds)
{
    pwm_period_us(index, seconds * 1000000.0f);
}

void pwm_period_ms(PWM_PinIndex_t index, int ms)
{
    pwm_period_us(index, ms * 1000);
}

void pwm_period_us(PWM_PinIndex_t index, int us)
{
    // Get the pwm pin info struct
    assert_param(IS_PWM_PIN_INDEX(pwm_pin, index));
    PWM_Pin_t *pin = &pwm_pin[index];

    TimHandle.Instance = (TIM_TypeDef *)(pin->tim);
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    uint32_t PclkFreq;
    uint32_t APBxCLKDivider;
    float dc = pwm_read(index);

    __HAL_TIM_DISABLE(&TimHandle);

    // Get clock configuration
    // Note: PclkFreq contains here the Latency (not used after)
    HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &PclkFreq);

    // Get the PCLK and APBCLK divider related to the timer
    switch (pin->tim) {
      
    // APB1 clock
#if defined(TIM2_BASE)
    case PWM_2:
#endif
#if defined(TIM3_BASE)
    case PWM_3:
#endif
#if defined(TIM4_BASE)
    case PWM_4:
#endif
#if defined(TIM5_BASE)
    case PWM_5:
#endif
#if defined(TIM12_BASE)
    case PWM_12:
#endif
#if defined(TIM13_BASE)
    case PWM_13:
#endif
#if defined(TIM14_BASE)
    case PWM_14:
#endif
        PclkFreq = HAL_RCC_GetPCLK1Freq();
        APBxCLKDivider = RCC_ClkInitStruct.APB1CLKDivider;
        break;
        
    // APB2 clock
#if defined(TIM1_BASE)
    case PWM_1:
#endif
#if defined(TIM8_BASE)
    case PWM_8:
#endif
#if defined(TIM9_BASE)
    case PWM_9:
#endif
#if defined(TIM10_BASE)
    case PWM_10:
#endif
#if defined(TIM11_BASE)
    case PWM_11:
#endif
        PclkFreq = HAL_RCC_GetPCLK2Freq();
        APBxCLKDivider = RCC_ClkInitStruct.APB2CLKDivider;
        break;
    default:
        return;
    }

    /* To make it simple, we use to possible prescaler values which lead to:
     * pwm unit = 1us, period/pulse can be from 1us to 65535us
     * or
     * pwm unit = 500us, period/pulse can be from 500us to ~32.76sec
     * Be careful that all the channels of a PWM shares the same prescaler
     */
    if (us > 0xFFFF) {
        pin->prescaler = 500;
    } else {
        pin->prescaler = 1;
    }

    // TIMxCLK = PCLKx when the APB prescaler = 1 else TIMxCLK = 2 * PCLKx
    if (APBxCLKDivider == RCC_HCLK_DIV1)
        TimHandle.Init.Prescaler = (uint16_t)(((PclkFreq) / 1000000) * pin->prescaler) - 1; // 1 us tick
    else
        TimHandle.Init.Prescaler = (uint16_t)(((PclkFreq * 2) / 1000000) * pin->prescaler) - 1; // 1 us tick

    if (TimHandle.Init.Prescaler > 0xFFFF)
        printf("PWM: out of range prescaler");

    TimHandle.Init.Period        = (us - 1) / pin->prescaler;
    if (TimHandle.Init.Period > 0xFFFF)
        printf("PWM: out of range period");

    TimHandle.Init.ClockDivision = 0;
    TimHandle.Init.CounterMode   = TIM_COUNTERMODE_UP;

    if (HAL_TIM_PWM_Init(&TimHandle) != HAL_OK) {
        printf("Cannot initialize PWM\n");
    }

    // Save for future use
    pin->period = us;

    // Set duty cycle again
    pwm_write(index, dc);

    __HAL_TIM_ENABLE(&TimHandle);
}

void pwm_pulsewidth(PWM_PinIndex_t index, float seconds)
{
    pwm_pulsewidth_us(index, seconds * 1000000.0f);
}

void pwm_pulsewidth_ms(PWM_PinIndex_t index, int ms)
{
    pwm_pulsewidth_us(index, ms * 1000);
}

void pwm_pulsewidth_us(PWM_PinIndex_t index, int us)
{
    // Get the pwm pin info struct
    assert_param(IS_PWM_PIN_INDEX(pwm_pin, index));
    PWM_Pin_t *pin = &pwm_pin[index];
    float value = (float)us / (float)pin->period;
    pwm_write(index, value);
}
