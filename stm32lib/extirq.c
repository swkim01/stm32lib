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
#include "../stm32lib/extirq.h"

#include "../stm32lib/cmsis_nvic.h"

#define EDGE_NONE (0)
#define EDGE_RISE (1)
#define EDGE_FALL (2)
#define EDGE_BOTH (3)

// Max EXTIRQ line
#define MAX_EXTIRQ_LINE (16)

typedef struct {
    uint32_t enable;    // enable state for receiving interrupts
    GPIO_TypeDef *port; // base address of gpio port group
    EXTIRQ_Callback_t callback; // irq callback
} EXTIRQ_LINE_t;

// Used to return the index for channels array.
static EXTIRQ_LINE_t irqs[MAX_EXTIRQ_LINE] = {
    {0, NULL, NULL}, // pin 0
    {0, NULL, NULL}, // pin 1
    {0, NULL, NULL}, // pin 2
    {0, NULL, NULL}, // pin 3
    {0, NULL, NULL}, // pin 4
    {0, NULL, NULL}, // pin 5
    {0, NULL, NULL}, // pin 6
    {0, NULL, NULL}, // pin 7
    {0, NULL, NULL}, // pin 8
    {0, NULL, NULL}, // pin 9
    {0, NULL, NULL}, // pin 10
    {0, NULL, NULL}, // pin 11
    {0, NULL, NULL}, // pin 12
    {0, NULL, NULL}, // pin 13
    {0, NULL, NULL}, // pin 14
    {0, NULL, NULL}, // pin 15
};

static void handle_interrupt_in(uint32_t base_index, uint32_t max_pin)
{
    EXTIRQ_LINE_t *irq_line = &irqs[base_index];
    uint32_t i;

    for (i = 0; i < max_pin; i++, irq_line++) {
        if (irq_line->enable) {
            // Retrieve the gpio and pin that generate the irq
            GPIO_TypeDef *gpio = irq_line->port;
            uint32_t pin = (uint32_t)(1 << (base_index+i));

            // Clear interrupt flag
            if (__HAL_GPIO_EXTI_GET_IT(pin) != RESET) {
                __HAL_GPIO_EXTI_CLEAR_IT(pin);

                // Check which edge has generated the irq
                if (irq_line->callback != NULL) {
                    //printf("irqn=%d\r\n", (base_index+i));
                    if ((gpio->IDR & pin) == 0)
                        irq_line->callback(i, EDGE_FALL);
                    else
                        irq_line->callback(i, EDGE_RISE);
                }
                break;
            }
        }
    }
}

// EXTI line 0
#if NVIC_RAM_IRQVECTOR
static void extirq_handler0(void)
#else
void EXTI0_IRQHandler(void)
#endif
{
    handle_interrupt_in(0, 1);
}

// EXTI line 1
#if NVIC_RAM_IRQVECTOR
static void extirq_handler1(void)
#else
void EXTI1_IRQHandler(void)
#endif
{
    handle_interrupt_in(1, 1);
}

// EXTI line 2
#if NVIC_RAM_IRQVECTOR
static void extirq_handler2(void)
#else
void EXTI2_IRQHandler(void)
#endif
{
    handle_interrupt_in(2, 1);
}

// EXTI line 3
#if NVIC_RAM_IRQVECTOR
static void extirq_handler3(void)
#else
void EXTI3_IRQHandler(void)
#endif
{
    handle_interrupt_in(3, 1);
}

// EXTI line 4
#if NVIC_RAM_IRQVECTOR
static void extirq_handler4(void)
#else
void EXTI4_IRQHandler(void)
#endif
{
    handle_interrupt_in(4, 1);
}

// EXTI lines 5 to 9
#if NVIC_RAM_IRQVECTOR
static void extirq_handler5(void)
#else
void EXTI9_5_IRQHandler(void)
#endif
{
    handle_interrupt_in(5, 5);
}

// EXTI lines 10 to 15
#if NVIC_RAM_IRQVECTOR
static void extirq_handler6(void)
#else
void EXTI15_10_IRQHandler(void)
#endif
{
    handle_interrupt_in(10, 6);
}

static IRQn_Type get_irqn(GPIO_Pin_t GPIO_Pin)
{
    IRQn_Type irq_n = (IRQn_Type)-15;
    uint32_t pin_index  = GPIO_PIN_VALUE(GPIO_Pin);
    switch (pin_index) {
        case 0:
            irq_n = EXTI0_IRQn;
            break;
        case 1:
            irq_n = EXTI1_IRQn;
            break;
        case 2:
            irq_n = EXTI2_IRQn;
            break;
        case 3:
            irq_n = EXTI3_IRQn;
            break;
        case 4:
            irq_n = EXTI4_IRQn;
            break;
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            irq_n = EXTI9_5_IRQn;
            break;
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            irq_n = EXTI15_10_IRQn;
            break;
        default:
            printf("InterruptIn error: pin not supported.\n");
            return -15;
    }
    return irq_n;
}

#if NVIC_RAM_IRQVECTOR
static uint32_t get_irqvector(GPIO_Pin_t GPIO_Pin)
{
    uint32_t vector = 0;
    uint32_t pin_index  = GPIO_PIN_VALUE(GPIO_Pin);
    switch (pin_index) {
        case 0:
            vector = (uint32_t)&extirq_handler0;
            break;
        case 1:
            vector = (uint32_t)&extirq_handler1;
            break;
        case 2:
            vector = (uint32_t)&extirq_handler2;
            break;
        case 3:
            vector = (uint32_t)&extirq_handler3;
            break;
        case 4:
            vector = (uint32_t)&extirq_handler4;
            break;
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            vector = (uint32_t)&extirq_handler5;
            break;
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            vector = (uint32_t)&extirq_handler6;
            break;
        default:
            printf("InterruptIn error: pin not supported.\n");
            return -1;
    }
    return vector;
}
#endif

EXTIRQ_Res_t extirq_init(GPIO_Pin_t GPIO_Pin, EXTIRQ_Callback_t callback, EXTIRQ_Edge_t edge)
{
    IRQn_Type irq_n = (IRQn_Type)-15;
#if NVIC_RAM_IRQVECTOR
    uint32_t vector = -1;
#endif
    EXTIRQ_LINE_t *irq_line;
    GPIO_TypeDef *GPIOx = gpio_get_port_base(GPIO_Pin);
    uint32_t pin_index  = GPIO_PIN_VALUE(GPIO_Pin);
    GPIO_PullMode_t pullMode;

    /* Check if line is already in use */
    if (
        (EXTI->IMR & pin_index) || /* Interrupt already attached */
        (EXTI->EMR & pin_index)    /* Event already attached */
    ) {
        /* Return error */
        return EXTIRQ_RES_ERR;
    }

    // Select irq number and interrupt routine
    irq_n = get_irqn(GPIO_Pin);
#if NVIC_RAM_IRQVECTOR
    vector = get_irqvector(GPIO_Pin);
#endif
    if (irq_n == -15
#if NVIC_RAM_IRQVECTOR
        || vector == -1
#endif
       )
        return EXTIRQ_RES_ERR;

    /* Check Edge setting */
    if (edge == EXTIRQ_RISING_EDGE)
        pullMode = GPIO_PM_PULLDOWN;
    else if (edge == EXTIRQ_FALLING_EDGE)
        pullMode = GPIO_PM_PULLUP;
    else
        pullMode = GPIO_PM_NOPULL;

    // Configure GPIO
    gpio_alternate_init(GPIO_Pin, (GPIO_Mode_t)(GPIO_EXTI|GPIO_IT|((3-pullMode)<<20)), 0);
    gpio_set_pinmode(GPIO_Pin, pullMode);

    // Enable EXTI interrupt
#if NVIC_RAM_IRQVECTOR
    NVIC_SetVector(irq_n, vector);
#endif
    /* Add to NVIC */
    HAL_NVIC_SetPriority(irq_n, EXTI_NVIC_PRIORITY, GPIO_PIN_VALUE(GPIO_Pin));
    NVIC_EnableIRQ(irq_n);

    // Save informations for future use
    irq_line = &irqs[pin_index];
    irq_line->enable = 1;
    irq_line->port = GPIOx;
    irq_line->callback = callback;

    return 0;
}

void extirq_free(GPIO_Pin_t GPIO_Pin)
{
    uint32_t pin_index  = GPIO_PIN_VALUE(GPIO_Pin);
    EXTIRQ_LINE_t *irq_line = &irqs[pin_index];

    irq_line->enable = 0;
    irq_line->port = NULL;
    irq_line->callback = NULL;

    // Disable EXTI line
    gpio_init(GPIO_Pin, GPIO_IN);
}

void extirq_enable(GPIO_Pin_t GPIO_Pin)
{
    NVIC_EnableIRQ(get_irqn(GPIO_Pin));
}

void extirq_disable(GPIO_Pin_t GPIO_Pin)
{
    NVIC_DisableIRQ(get_irqn(GPIO_Pin));
}
