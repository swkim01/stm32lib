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
#include "../stm32lib/clcd.h"

/* Private CLCD structure */
typedef struct {
    uint8_t displaycontrol;
    uint8_t displayfunction;
    uint8_t displaymode;
    uint8_t rows;
    uint8_t cols;
} CLCD_Options_t;

/* Private functions */
static void clcd_initpins(void);
static void clcd_write_command(uint8_t cmd);
static void clcd_write4bit(uint8_t cmd);
static void clcd_write_data(uint8_t data);

/* Private variable */
static CLCD_Options_t clcd_options;

/* Pin definitions */
#define CLCD_RS_LOW              gpio_write(CLCD_RS_PIN, 0)
#define CLCD_RS_HIGH             gpio_write(CLCD_RS_PIN, 1)
#define CLCD_E_LOW               gpio_write(CLCD_E_PIN, 0)
#define CLCD_E_HIGH              gpio_write(CLCD_E_PIN, 1)

#define CLCD_E_BLINK             CLCD_E_LOW; clcd_delay(20); CLCD_E_HIGH; clcd_delay(20); CLCD_E_LOW; clcd_delay(20)
#define clcd_delay(x)            delay(x)

/* Commands*/
#define CLCD_CLEARDISPLAY        0x01
#define CLCD_RETURNHOME          0x02
#define CLCD_ENTRYMODESET        0x04
#define CLCD_DISPLAYCONTROL      0x08
#define CLCD_CURSORSHIFT         0x10
#define CLCD_FUNCTIONSET         0x20
#define CLCD_SETCGRAMADDR        0x40
#define CLCD_SETDDRAMADDR        0x80

/* Flags for display entry mode */
#define CLCD_ENTRYRIGHT          0x00
#define CLCD_ENTRYLEFT           0x02
#define CLCD_ENTRYSHIFTINCREMENT 0x01
#define CLCD_ENTRYSHIFTDECREMENT 0x00

/* Flags for display on/off control */
#define CLCD_DISPLAYON           0x04
#define CLCD_DISPLAYOFF          0x00
#define CLCD_CURSORON            0x02
#define CLCD_CURSOROFF           0x00
#define CLCD_BLINKON             0x01
#define CLCD_BLINKOFF            0x00

/* Flags for display/cursor shift */
#define CLCD_DISPLAYMOVE         0x08
#define CLCD_CURSORMOVE          0x00
#define CLCD_MOVERIGHT           0x04
#define CLCD_MOVELEFT            0x00

/* Flags for function set */
#define CLCD_8BITMODE            0x10
#define CLCD_4BITMODE            0x00
#define CLCD_2LINE               0x08
#define CLCD_1LINE               0x00
#define CLCD_5x10DOTS            0x04
#define CLCD_5x8DOTS             0x00

/**
 * @brief  Delays for amount of micro seconds
 * @param  micros: Number of microseconds for delay
 * @retval None
 */
__STATIC_INLINE void delay(__IO uint32_t micros) {
#if !defined(STM32F0xx)
    uint32_t start = DWT->CYCCNT;
    
    /* Go to number of cycles for system */
    micros *= (HAL_RCC_GetHCLKFreq() / 1000000);
    
    /* Delay till end */
    while ((DWT->CYCCNT - start) < micros);
#else
    /* Go to clock cycles */
    micros *= (SystemCoreClock / 1000000) / 5;
    
    /* Wait till done */
    while (micros--);
#endif
}

static uint32_t delay_init(void) {
#if !defined(STM32F0xx)
    uint32_t c;
    
    /* Enable TRC */
    CoreDebug->DEMCR &= ~0x01000000;
    CoreDebug->DEMCR |=  0x01000000;
    
    /* Enable counter */
    DWT->CTRL &= ~0x00000001;
    DWT->CTRL |=  0x00000001;
    
    /* Reset counter */
    DWT->CYCCNT = 0;
    
    /* Check if DWT has started */
    c = DWT->CYCCNT;
    
    /* 2 dummys */
    //__ASM volatile ("NOP");
    //__ASM volatile ("NOP");
    
    /* Return difference, if result is zero, DWT has not started */
    return (DWT->CYCCNT - c);
#else
    /* Return OK */
    return 1;
#endif
}

void clcd_init(uint8_t cols, uint8_t rows) {
    delay_init();
    /* Init pinout */
    clcd_initpins();
    
    /* At least 40ms */
    //clcd_delay(45000);
    clcd_delay(45000);
    
    /* Set LCD width and height */
    clcd_options.rows = rows;
    clcd_options.cols = cols;
    
    /* Try to set 4bit mode */
    clcd_write4bit(0x03);
    clcd_delay(4500);
    clcd_write4bit(0x03);
    clcd_delay(4500);
    //clcd_write_command(0x33);
    
    /* Third goo! */
    clcd_write4bit(0x03);
    clcd_delay(4500);
    /* Set 4-bit interface */
    clcd_write4bit(0x02);
    clcd_delay(100);
    //clcd_write_command(0x32);
    
    clcd_options.displayfunction = CLCD_4BITMODE | CLCD_5x8DOTS | CLCD_1LINE;
    if (rows > 1) {
        clcd_options.displayfunction |= CLCD_2LINE;
    }
    
    /* Set # lines, font size, etc. */
    clcd_write_command(CLCD_FUNCTIONSET | clcd_options.displayfunction);

    /* Turn the display on with no cursor or blinking default */
    clcd_options.displaycontrol = CLCD_DISPLAYON | CLCD_CURSOROFF | CLCD_BLINKOFF;
    clcd_write_command(CLCD_DISPLAYCONTROL | clcd_options.displaycontrol);

    /* Default font directions */
    clcd_options.displaymode = CLCD_ENTRYLEFT | CLCD_ENTRYSHIFTDECREMENT;
    clcd_write_command(CLCD_ENTRYMODESET | clcd_options.displaymode);

    /* Clear lcd */
    clcd_clear();

    /* Delay */
    clcd_delay(4500);
}

void clcd_clear(void) {
    clcd_write_command(CLCD_CLEARDISPLAY);
    clcd_delay(3000);
}

void clcd_home(void) {
    clcd_write_command(CLCD_RETURNHOME);
    clcd_delay(3000);
}

void clcd_set_cursor(uint8_t col, uint8_t row) {
    uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    
    /* Clamp row to the last row */
    if (row >= clcd_options.rows) {
        row = clcd_options.rows - 1;
    }
    
    /* Set location address */
    clcd_write_command(CLCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void clcd_enable_display(uint8_t enable) {
    if (enable != 0)
        clcd_options.displaycontrol |= CLCD_DISPLAYON;
    else
        clcd_options.displaycontrol &= ~CLCD_DISPLAYON;
    clcd_write_command(CLCD_DISPLAYCONTROL | clcd_options.displaycontrol);
}

void clcd_show_cursor(uint8_t show) {
    if (show != 0)
        clcd_options.displaycontrol |= CLCD_CURSORON;
    else
        clcd_options.displaycontrol &= ~CLCD_CURSORON;
    clcd_write_command(CLCD_DISPLAYCONTROL | clcd_options.displaycontrol);
}

void clcd_blink(uint8_t blink) {
    if (blink != 0)
        clcd_options.displaycontrol |= CLCD_BLINKON;
    else
        clcd_options.displaycontrol &= ~CLCD_BLINKON;
    clcd_write_command(CLCD_DISPLAYCONTROL | clcd_options.displaycontrol);
}

void clcd_move_left(void) {
    clcd_write_command(CLCD_CURSORSHIFT | CLCD_DISPLAYMOVE | CLCD_MOVELEFT);
}

void clcd_move_right(void) {
    clcd_write_command(CLCD_CURSORSHIFT | CLCD_DISPLAYMOVE | CLCD_MOVERIGHT);
}

void clcd_set_left2right(void) {
    clcd_options.displaymode |= CLCD_ENTRYLEFT;
    clcd_write_command(CLCD_ENTRYMODESET | clcd_options.displaymode);
}

void clcd_set_right2left(void) {
    clcd_options.displaymode &= ~CLCD_ENTRYLEFT;
    clcd_write_command(CLCD_ENTRYMODESET | clcd_options.displaymode);
}

void clcd_autoscroll(uint8_t autoscroll) {
    if (autoscroll != 0)
        clcd_options.displaymode |= CLCD_ENTRYSHIFTINCREMENT;
    else
        clcd_options.displaymode &= ~CLCD_ENTRYSHIFTINCREMENT;
    clcd_write_command(CLCD_ENTRYMODESET | clcd_options.displaymode);
}

void clcd_message(char* str) {
    uint8_t col, line=0;

    while (*str) {
        if (*str == '\n') {
            line += 1;
            if (clcd_options.displaymode & CLCD_ENTRYLEFT)
                col = 0;
            else
                col = clcd_options.cols;
            clcd_set_cursor(col, line);
        } else
            clcd_write_data(*str);
        str++;
    }
}

/* Private functions */
static void clcd_write_command(uint8_t cmd) {
    /* Command mode */
    CLCD_RS_LOW;
    
    /* High nibble */
    clcd_write4bit(cmd >> 4);
    /* Low nibble */
    clcd_write4bit(cmd & 0x0F);
}

static void clcd_write_data(uint8_t data) {
    /* Data mode */
    CLCD_RS_HIGH;
    
    /* High 4 bits */
    clcd_write4bit(data >> 4);
    /* Low 4 bits */
    clcd_write4bit(data & 0x0F);
}

static void clcd_write4bit(uint8_t cmd) {
    /* Set output port */
    gpio_write(CLCD_D7_PIN, (cmd >> 3) & 0x01);
    gpio_write(CLCD_D6_PIN, (cmd >> 2) & 0x01);
    gpio_write(CLCD_D5_PIN, (cmd >> 1) & 0x01);
    gpio_write(CLCD_D4_PIN, (cmd >> 0) & 0x01);
    CLCD_E_BLINK;
}

static void clcd_initpins(void) {
    /* Init all pins */
    gpio_init(CLCD_RS_PIN, GPIO_OUT);
    gpio_init(CLCD_E_PIN,  GPIO_OUT);
    gpio_init(CLCD_D4_PIN, GPIO_OUT);
    gpio_init(CLCD_D5_PIN, GPIO_OUT);
    gpio_init(CLCD_D6_PIN, GPIO_OUT);
    gpio_init(CLCD_D7_PIN, GPIO_OUT);
    
    /* Set pins low */
    gpio_write(CLCD_RS_PIN, 0);
    gpio_write(CLCD_E_PIN, 0);
    gpio_write(CLCD_D4_PIN, 0);
    gpio_write(CLCD_D5_PIN, 0);
    gpio_write(CLCD_D6_PIN, 0);
    gpio_write(CLCD_D7_PIN, 0);
}
