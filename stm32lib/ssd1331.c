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
#include "../stm32lib/ssd1331.h"

/* Write command */
#define ssd1331_writecommand(command)      spi_writeread(SSD1331_SPI, (command))
/* Write data */
#define ssd1331_writedata(data)            spi_writeread(SSD1331_SPI, (data))
/* Absolute value */
#define ABS(x)   ((x) > 0 ? (x) : -(x))

/* Private SSD1331 structure */
typedef struct {
    uint16_t cursor_x;
    uint16_t cursor_y;
    uint8_t inverted;
    uint8_t initialized;
} SSD1331_t;

/* Private variable */
static SSD1331_t SSD1331;

uint8_t ssd1331_init(void)
{
    /* SPI 초기화 */
    spi_init(SSD1331_SPI, SSD1331_SPI_PINSPACK);
    //spi_set_frequency(SSD1331_SPI, 8000000);
    
    /* 추가 핀 초기화 */
    gpio_init(SSD1331_RS_PIN, GPIO_OUT);
    gpio_init(SSD1331_DC_PIN, GPIO_OUT);
    gpio_init(SSD1331_CS_PIN, GPIO_OUT);

    gpio_write(SSD1331_RS_PIN, 1);
    gpio_write(SSD1331_DC_PIN, 1);
    gpio_write(SSD1331_CS_PIN, 1);
    //printf("ssd1331_init\r\n");

    /* A little delay */
    HAL_Delay(100);
    
    /* LCD 리셋 */
    gpio_write(SSD1331_DC_PIN, 0);
    gpio_write(SSD1331_RS_PIN, 1);
    gpio_write(SSD1331_CS_PIN, 1);
    HAL_Delay(1);
    gpio_write(SSD1331_RS_PIN, 0);
    HAL_Delay(1);
    gpio_write(SSD1331_RS_PIN, 1);
    HAL_Delay(50);
    gpio_write(SSD1331_CS_PIN, 0);

    /* LCD 초기화 */
    ssd1331_writecommand(0xAE); //display off
    ssd1331_writecommand(0xA0); //Set ReMap   
    ssd1331_writecommand(0xB2); //65k format 2, RGB
    ssd1331_writecommand(0xA1); //Set Display Start Line 
    ssd1331_writecommand(0x00);
    ssd1331_writecommand(0xA2); //Set Display Offset
    ssd1331_writecommand(0x00);
    ssd1331_writecommand(0xA8); //Set Multiplex, reset value = 0x3F
    ssd1331_writecommand(0x3F);
    ssd1331_writecommand(0xAD); //Select supply (must be set before 0xaf
    ssd1331_writecommand(0x8E);
    ssd1331_writecommand(0xB0); //Set power save mode
    ssd1331_writecommand(0x0B); //Disable power save mode
    ssd1331_writecommand(0xB1); //Set Phase Length, reset default: 0x74
    ssd1331_writecommand(0x31); //Disable power save mode
    ssd1331_writecommand(0xB3); //Set Display Clock Divider/)sc, reset value: 0xd0
    ssd1331_writecommand(0xF0); //7:4=Osc. Freq., 3:0=CLK Div Ratiio (A[3:0]+1=1..16)
    ssd1331_writecommand(0x15); // Set Column Address
    ssd1331_writecommand(0x00);
    ssd1331_writecommand(0x5F);
    ssd1331_writecommand(0x75); // Set Row Address
    ssd1331_writecommand(0x00);
    ssd1331_writecommand(0x3F);

    ssd1331_writecommand(0x81); //contrast red, Adafruit: 0x091, UC9664: 0x080 
    ssd1331_writecommand(0x80);
    ssd1331_writecommand(0x82); //contrast green, Adafruit: 0x050, UC9664: 0x080
    ssd1331_writecommand(0x80);
    ssd1331_writecommand(0x83); // contrast blue, Adafruit: 0x07d, UC9664: 0x080
    ssd1331_writecommand(0x80);

    ssd1331_writecommand(0x87); // master current/contrast 0x00..0x0f
    ssd1331_writecommand(0x0F); // UG-9664: 0x0f, Adafruit: 0x06
    ssd1331_writecommand(0x8A); // second precharge speed red
    ssd1331_writecommand(0x64);
    ssd1331_writecommand(0x8B); // second precharge speed green
    ssd1331_writecommand(0x78);
    ssd1331_writecommand(0x8C); // second precharge speed blue
    ssd1331_writecommand(0x64);
    ssd1331_writecommand(0xBB); // set shared precharge level
    ssd1331_writecommand(0x3C); // default = 0x03e
    ssd1331_writecommand(0xBE); // voltage select
    ssd1331_writecommand(0x3E); // default = 0x03e

    ssd1331_writecommand(0xB9); // Reset internal grayscale lookup

    ssd1331_writecommand(0xA4); //Normal Display
    ssd1331_writecommand(0xAF); //--turn on SSD1331 panel
    gpio_write(SSD1331_CS_PIN, 1);
    HAL_Delay(1);
    
    /* 스크린 지움 */
    ssd1331_fill(SSD1331_COLOR_BLACK);
    
    /* 기본 값 설정 */
    SSD1331.cursor_x = 0;
    SSD1331.cursor_y = 0;
    
    /* Initialized OK */
    SSD1331.initialized = 1;

    /* Return OK */
    return 1;
}

void ssd1331_fill(SSD1331_Color_t color)
{
    uint8_t x, y;
    
    for (y = 0; y < SSD1331_HEIGHT; y++)
        for (x = 0; x < SSD1331_WIDTH; x++)
            ssd1331_drawpixel(x, y, color);
}

void ssd1331_drawpixel(uint16_t x, uint16_t y, SSD1331_Color_t color)
{
    uint8_t c[3];

    if (
        x >= SSD1331_WIDTH ||
        y >= SSD1331_HEIGHT
    ) {
        /* Error */
        return;
    }

    gpio_write(SSD1331_DC_PIN, 0); /* command */
    gpio_write(SSD1331_CS_PIN, 0); /* enable chip */
    ssd1331_writecommand(0x15); // set x position
    ssd1331_writecommand(x & 0xff);
    ssd1331_writecommand(0x5F);
    ssd1331_writecommand(0x75); // set y position
    ssd1331_writecommand(y & 0xff);
    ssd1331_writecommand(0x3F);
    gpio_write(SSD1331_DC_PIN, 1); /* data */
    c[0] = ((color>>16)&0xFF)>>2;
    c[1] = ((color>>8)&0xFF)>>2;
    c[2] = ((color>>0)&0xFF)>>2;
    ssd1331_writedata(c[0]);
    ssd1331_writedata(c[1]);
    ssd1331_writedata(c[2]);
    gpio_write(SSD1331_CS_PIN, 1); /* disable chip */
}

void ssd1331_gotoxy(uint16_t x, uint16_t y)
{
    /* Set write pointers */
    SSD1331.cursor_x = x;
    SSD1331.cursor_y = y;
}

char ssd1331_putc(uint16_t ch, Font_t *font, SSD1331_Color_t color, uint8_t size)
{
    uint32_t i, b, j;
    
    /* Check available space in LCD */
    if (font == NULL ||
        SSD1331_WIDTH <= (SSD1331.cursor_x + size * font->width) ||
        SSD1331_HEIGHT <= (SSD1331.cursor_y + size * font->height)
    ) {
        /* Error */
        return 0;
    }
    
    /* Go through font */
    if (font->type == ASCII_FONT) {
        for (i = 0; i < font->height; i++) {
            b = font->data.data[(ch - font->first) * font->height + i];
            for (j = 0; j < font->width; j++) {
                if ((b << j) & 0x8000) {
                    if (size == 1)
                        ssd1331_drawpixel(SSD1331.cursor_x + j, (SSD1331.cursor_y + i), color);
                    else
                        ssd1331_fillrectangle(SSD1331.cursor_x + j*size, SSD1331.cursor_y + i*size, size, size, color);
                }/* else {
                    ssd1331_drawpixel(SSD1331.cursor_x + j, (SSD1331.cursor_y + i), (SSD1331_Color_t)!color);
                }*/
            }
        }
    } else if (font->type == GFX_FONT) {
        ssd1331_putc_gfx(ch, font, color, size);
    } else if (font->type == COMB_FONT) {
        ssd1331_putc_hangul(ch, font, color, size);
    }
    
    /* Increase pointer */
    //SSD1331.cursor_x += font->width * size;
    
    /* Return character written */
    return ch;
}

char ssd1331_putc_gfx(uint16_t ch, Font_t* font, SSD1331_Color_t color, uint8_t size)
{
    const GFXFont_t *gfxfont = font->data.gfxfont;
    ch -= gfxfont->first;
    GFXglyph *glyph  = &(gfxfont->glyph)[ch];
    uint8_t  *bitmap = gfxfont->bitmap;

    uint32_t bo = glyph->bitmapOffset;
    uint8_t  w  = glyph->width, h  = glyph->height;
    int8_t   xo = glyph->xOffset, yo = 0/*glyph->yOffset*/;
    uint8_t  xx, yy, bits = 0, bit = 0;
    int16_t  xo16 = 0, yo16 = 0;
    //printf("ch=%d, offset=%ld, w=%d, h=%d\r\n", ch, bo, w, h);

    if (size > 1) {
        xo16 = xo;
        yo16 = yo;
    }
    for(yy=0; yy<h; yy++) {
        for(xx=0; xx<w; xx++) {
            if(!(bit++ & 7)) {
                bits = bitmap[bo++];
            }
            if(bits & 0x80) {
                if (size == 1)
                    ssd1331_drawpixel(SSD1331.cursor_x + xo + xx, (SSD1331.cursor_y + yo + yy), color);
                else
                    ssd1331_fillrectangle(SSD1331.cursor_x + (xo16+xx)*size, SSD1331.cursor_y + (yo16+yy)*size, size, size, color);
            }
            bits <<= 1;
        }
    }
    return ch;
}

char ssd1331_putc_hangul(uint16_t ch, Font_t* font, SSD1331_Color_t color, uint8_t size)
{
    uint16_t i, j, w;
    uint8_t wb = font->width/8;
    uint8_t b, *pFs;
    
    /* Go through font */
    pFs = get_hangul_glyph(ch, font);
    for (i = 0; i < font->height; i++) {
      for (w = 0; w < wb; w++) {
        b = pFs[i*wb+w];
        for (j = 0; j < 8; j++) {
            if ((b << j) & 0x80) {
                if (size == 1)
                    ssd1331_drawpixel(SSD1331.cursor_x + w*8 + j, SSD1331.cursor_y + i, color);
                else
                    ssd1331_fillrectangle(SSD1331.cursor_x + (w*8 + j)*size, SSD1331.cursor_y + i*size, size, size, color);
            }/* else {
                ssd1331_drawpixel(SSD1331.cursor_x + w*8 + j, (SSD1331.cursor_y + i), (SSD1331_Color_t)!color);
            }*/
        }
      }
    }
    return ch;
}

char ssd1331_puts(char* str, FontSet_t* fontset, SSD1331_Color_t color, uint8_t size)
{
    int i;
    char c, c2, c3;
    uint16_t utf16;
    int cursor_x = SSD1331.cursor_x;

    /* Write characters */
    while (*str) {
        c = *str++;

        // convert utf-8 to unicode
        if (c <= 0x7F){
            utf16 = c;
        } else {
            /*------------------------------
              UTF-8 을 UTF-16으로 변환한다.  UTF-8 1110xxxx 10xxxxxx 10xxxxxx
            ------------------------------*/
            c2 = *str++;
            c3 = *str++;
            utf16 = ((c & 0x0f) << 12) | ((c2 & 0x3f) << 6) | (c3 & 0x3f);
        }
        if (utf16 == '\n') {
            /* Increase pointer */
            SSD1331.cursor_x = cursor_x;
            SSD1331.cursor_y += size * fontset->height;
        } else {
            /* Write character by character */
            for (i = 0; fontset->fontlist[i] && i < 5; i++) {
                Font_t *font = fontset->fontlist[i];
                if (utf16 >= font->first && utf16 <= font->last) {
                    ssd1331_putc(utf16, font, color, size);
                    SSD1331.cursor_x += size * font->width;
                    break;
                }
            }
        }
    }
    
    /* Everything OK, zero should be returned */
    return *str;
}
 

void ssd1331_drawline(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SSD1331_Color_t c)
{
    int16_t dx, dy, sx, sy, err, e2, i, tmp; 
    
    /* Check for overflow */
    if (x0 >= SSD1331_WIDTH) {
        x0 = SSD1331_WIDTH - 1;
    }
    if (x1 >= SSD1331_WIDTH) {
        x1 = SSD1331_WIDTH - 1;
    }
    if (y0 >= SSD1331_HEIGHT) {
        y0 = SSD1331_HEIGHT - 1;
    }
    if (y1 >= SSD1331_HEIGHT) {
        y1 = SSD1331_HEIGHT - 1;
    }
    
    dx = (x0 < x1) ? (x1 - x0) : (x0 - x1); 
    dy = (y0 < y1) ? (y1 - y0) : (y0 - y1); 
    sx = (x0 < x1) ? 1 : -1; 
    sy = (y0 < y1) ? 1 : -1; 
    err = ((dx > dy) ? dx : -dy) / 2; 

    if (dx == 0) {
        if (y1 < y0) {
            tmp = y1;
            y1 = y0;
            y0 = tmp;
        }
        
        if (x1 < x0) {
            tmp = x1;
            x1 = x0;
            x0 = tmp;
        }
        
        /* Vertical line */
        for (i = y0; i <= y1; i++) {
            ssd1331_drawpixel(x0, i, c);
        }
        
        /* Return from function */
        return;
    }
    
    if (dy == 0) {
        if (y1 < y0) {
            tmp = y1;
            y1 = y0;
            y0 = tmp;
        }
        
        if (x1 < x0) {
            tmp = x1;
            x1 = x0;
            x0 = tmp;
        }
        
        /* Horizontal line */
        for (i = x0; i <= x1; i++) {
            ssd1331_drawpixel(i, y0, c);
        }
        
        /* Return from function */
        return;
    }
    
    while (1) {
        ssd1331_drawpixel(x0, y0, c); 
        if (x0 == x1 && y0 == y1) {
            break;
        }
        e2 = err; 
        if (e2 > -dx) {
            err -= dy;
            x0 += sx;
        } 
        if (e2 < dy) {
            err += dx;
            y0 += sy;
        } 
    }
}

void ssd1331_drawrectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1331_Color_t c)
{
    /* Check input parameters */
    if (
        x >= SSD1331_WIDTH ||
        y >= SSD1331_HEIGHT
    ) {
        /* Return error */
        return;
    }
    
    /* Check width and height */
    if ((x + w) >= SSD1331_WIDTH) {
        w = SSD1331_WIDTH - x;
    }
    if ((y + h) >= SSD1331_HEIGHT) {
        h = SSD1331_HEIGHT - y;
    }
    
    /* Draw 4 lines */
    ssd1331_drawline(x, y, x + w, y, c);         /* Top line */
    ssd1331_drawline(x, y + h, x + w, y + h, c); /* Bottom line */
    ssd1331_drawline(x, y, x, y + h, c);         /* Left line */
    ssd1331_drawline(x + w, y, x + w, y + h, c); /* Right line */
}

void ssd1331_fillrectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1331_Color_t c)
{
    uint8_t i;
    
    /* Check input parameters */
    if (
        x >= SSD1331_WIDTH ||
        y >= SSD1331_HEIGHT
    ) {
        /* Return error */
        return;
    }
    
    /* Check width and height */
    if ((x + w) >= SSD1331_WIDTH) {
        w = SSD1331_WIDTH - x;
    }
    if ((y + h) >= SSD1331_HEIGHT) {
        h = SSD1331_HEIGHT - y;
    }
    
    /* Draw lines */
    for (i = 0; i <= h; i++) {
        /* Draw lines */
        ssd1331_drawline(x, y + i, x + w, y + i, c);
    }
}

void ssd1331_drawtriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1331_Color_t color)
{
    /* Draw lines */
    ssd1331_drawline(x1, y1, x2, y2, color);
    ssd1331_drawline(x2, y2, x3, y3, color);
    ssd1331_drawline(x3, y3, x1, y1, color);
}


void ssd1331_filltriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1331_Color_t color)
{
    int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
    yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
    curpixel = 0;
    
    deltax = ABS(x2 - x1);
    deltay = ABS(y2 - y1);
    x = x1;
    y = y1;

    if (x2 >= x1) {
        xinc1 = 1;
        xinc2 = 1;
    } else {
        xinc1 = -1;
        xinc2 = -1;
    }

    if (y2 >= y1) {
        yinc1 = 1;
        yinc2 = 1;
    } else {
        yinc1 = -1;
        yinc2 = -1;
    }

    if (deltax >= deltay){
        xinc1 = 0;
        yinc2 = 0;
        den = deltax;
        num = deltax / 2;
        numadd = deltay;
        numpixels = deltax;
    } else {
        xinc2 = 0;
        yinc1 = 0;
        den = deltay;
        num = deltay / 2;
        numadd = deltax;
        numpixels = deltay;
    }

    for (curpixel = 0; curpixel <= numpixels; curpixel++) {
        ssd1331_drawline(x, y, x3, y3, color);

        num += numadd;
        if (num >= den) {
            num -= den;
            x += xinc1;
            y += yinc1;
        }
        x += xinc2;
        y += yinc2;
    }
}

void ssd1331_drawcircle(int16_t x0, int16_t y0, int16_t r, SSD1331_Color_t c)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    //ssd1331_drawpixel(x0, y0 + r, c);
    //ssd1331_drawpixel(x0, y0 - r, c);
    //ssd1331_drawpixel(x0 + r, y0, c);
    //ssd1331_drawpixel(x0 - r, y0, c);

    while (x <= y) {
        ssd1331_drawpixel(x0 + x, y0 + y, c);
        ssd1331_drawpixel(x0 - x, y0 + y, c);
        ssd1331_drawpixel(x0 + x, y0 - y, c);
        ssd1331_drawpixel(x0 - x, y0 - y, c);

        ssd1331_drawpixel(x0 + y, y0 + x, c);
        ssd1331_drawpixel(x0 - y, y0 + x, c);
        ssd1331_drawpixel(x0 + y, y0 - x, c);
        ssd1331_drawpixel(x0 - y, y0 - x, c);

        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

    }
}

void ssd1331_fillcircle(int16_t x0, int16_t y0, int16_t r, SSD1331_Color_t c)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    //ssd1331_drawpixel(x0, y0 + r, c);
    //ssd1331_drawpixel(x0, y0 - r, c);
    //ssd1331_drawpixel(x0 + r, y0, c);
    //ssd1331_drawpixel(x0 - r, y0, c);
    //ssd1331_drawline(x0 - r, y0, x0 + r, y0, c);

    while (x <= y) {
        ssd1331_drawline(x0 - x, y0 + y, x0 + x, y0 + y, c);
        ssd1331_drawline(x0 + x, y0 - y, x0 - x, y0 - y, c);

        ssd1331_drawline(x0 + y, y0 + x, x0 - y, y0 + x, c);
        ssd1331_drawline(x0 + y, y0 - x, x0 - y, y0 - x, c);
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

    }
}
 
void ssd1331_on(void)
{
    gpio_write(SSD1331_CS_PIN, 0);
    ssd1331_writecommand(0xA4); //Set display:Normal
    ssd1331_writecommand(0xAF); //--turn on SSD1331 panel
    HAL_Delay(1);
    gpio_write(SSD1331_CS_PIN, 1);
}

void ssd1331_off(void)
{
    gpio_write(SSD1331_CS_PIN, 0);
    ssd1331_writecommand(0xA6); //Set display:All pixel off
    ssd1331_writecommand(0xAE); //--turn off SSD1331 panel
    HAL_Delay(1);
    gpio_write(SSD1331_CS_PIN, 1);
}
