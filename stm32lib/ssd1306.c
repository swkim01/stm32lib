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
#include "../stm32lib/ssd1306.h"

/* Write command */
#define ssd1306_writecommand(command)      i2c_write(SSD1306_I2C, SSD1306_I2C_ADDR, 0x00, (command))
/* Write data */
#define ssd1306_writedata(data)            i2c_write(SSD1306_I2C, SSD1306_I2C_ADDR, 0x40, (data))
/* Absolute value */
#define ABS(x)   ((x) > 0 ? (x) : -(x))

/* SSD1306 data buffer */
static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

/* Private SSD1306 structure */
typedef struct {
    uint16_t cursor_x;
    uint16_t cursor_y;
    uint8_t inverted;
    uint8_t initialized;
} SSD1306_t;

/* Private variable */
static SSD1306_t SSD1306;

SSD1306_Res_t ssd1306_init(void)
{
    /* I2C 초기화 */
    i2c_init(SSD1306_I2C, SSD1306_I2C_PINSPACK);
    i2c_set_frequency(SSD1306_I2C, 4000000);
    
    /* 장치 연결 점검 */
    if (i2c_ready(SSD1306_I2C, SSD1306_I2C_ADDR) < 0) {
        /* 오류 반환 */
        return SSD1306_RES_NOTCONNECT;
    }
    //printf("ssd1306_init\r\n");

    /* A little delay */
    HAL_Delay(100);
    
    /* LCD 초기화 */
    ssd1306_writecommand(0xAE); //display off
    ssd1306_writecommand(0x20); //Set Memory Addressing Mode   
    ssd1306_writecommand(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    ssd1306_writecommand(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
    ssd1306_writecommand(0xC8); //Set COM Output Scan Direction
    ssd1306_writecommand(0x00); //---set low column address
    ssd1306_writecommand(0x10); //---set high column address
    ssd1306_writecommand(0x40); //--set start line address
    ssd1306_writecommand(0x81); //--set contrast control register
    ssd1306_writecommand(0xFF);
    ssd1306_writecommand(0xA1); //--set segment re-map 0 to 127
    ssd1306_writecommand(0xA6); //--set normal display
    ssd1306_writecommand(0xA8); //--set multiplex ratio(1 to 64)
    ssd1306_writecommand(0x3F); //
    ssd1306_writecommand(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    ssd1306_writecommand(0xD3); //-set display offset
    ssd1306_writecommand(0x00); //-not offset
    ssd1306_writecommand(0xD5); //--set display clock divide ratio/oscillator frequency
    ssd1306_writecommand(0xF0); //--set divide ratio
    ssd1306_writecommand(0xD9); //--set pre-charge period
    ssd1306_writecommand(0x22); //
    ssd1306_writecommand(0xDA); //--set com pins hardware configuration
    ssd1306_writecommand(0x12);
    ssd1306_writecommand(0xDB); //--set vcomh
    ssd1306_writecommand(0x20); //0x20,0.77xVcc
    ssd1306_writecommand(0x8D); //--set DC-DC enable
    ssd1306_writecommand(0x14); //
    ssd1306_writecommand(0xAF); //--turn on SSD1306 panel
    
    /* 스크린 지움 */
    ssd1306_fill(SSD1306_COLOR_BLACK);
    
    /* 스크린 갱신 */
    ssd1306_updatescreen();
    
    /* 기본 값 설정 */
    SSD1306.cursor_x = 0;
    SSD1306.cursor_y = 0;
    
    /* Initialized OK */
    SSD1306.initialized = 1;
    
    /* Return OK */
    return SSD1306_RES_OK;
}

void ssd1306_updatescreen(void)
{
    uint8_t m;
    
    for (m = 0; m < 8; m++) {
        ssd1306_writecommand(0xB0 + m);
        ssd1306_writecommand(0x00);
        ssd1306_writecommand(0x10);
        
        /* Write multi data */
        i2c_nwrite(SSD1306_I2C, SSD1306_I2C_ADDR, 0x40, &SSD1306_Buffer[SSD1306_WIDTH * m], SSD1306_WIDTH);
    }
}

void ssd1306_invert(void)
{
    uint16_t i;
    
    /* Toggle invert */
    SSD1306.inverted = !SSD1306.inverted;
    
    /* Do memory toggle */
    for (i = 0; i < sizeof(SSD1306_Buffer); i++) {
        SSD1306_Buffer[i] = ~SSD1306_Buffer[i];
    }
}

void ssd1306_fill(SSD1306_Color_t color)
{
    /* Set memory */
    memset(SSD1306_Buffer, (color == SSD1306_COLOR_BLACK) ? 0x00 : 0xFF, sizeof(SSD1306_Buffer));
}

void ssd1306_drawpixel(uint16_t x, uint16_t y, SSD1306_Color_t color)
{
    if (
        x >= SSD1306_WIDTH ||
        y >= SSD1306_HEIGHT
    ) {
        /* Error */
        return;
    }
    
    /* Check if pixels are inverted */
    if (SSD1306.inverted) {
        color = (SSD1306_Color_t)!color;
    }
    
    /* Set color */
    if (color == SSD1306_COLOR_WHITE) {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
    } else {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}

void ssd1306_gotoxy(uint16_t x, uint16_t y)
{
    /* Set write pointers */
    SSD1306.cursor_x = x;
    SSD1306.cursor_y = y;
}

char ssd1306_putc(uint16_t ch, Font_t *font, SSD1306_Color_t color, uint8_t size)
{
    uint32_t i, b, j;
    
    /* Check available space in LCD */
    if (font == NULL ||
        SSD1306_WIDTH <= (SSD1306.cursor_x + size * font->width) ||
        SSD1306_HEIGHT <= (SSD1306.cursor_y + size * font->height)
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
                        ssd1306_drawpixel(SSD1306.cursor_x + j, (SSD1306.cursor_y + i), color);
                    else
                        ssd1306_fillrectangle(SSD1306.cursor_x + j*size, SSD1306.cursor_y + i*size, size, size, color);
                }/* else {
                    ssd1306_drawpixel(SSD1306.cursor_x + j, (SSD1306.cursor_y + i), (SSD1306_Color_t)!color);
                }*/
            }
        }
    } else if (font->type == GFX_FONT) {
        ssd1306_putc_gfx(ch, font, color, size);
    } else if (font->type == COMB_FONT) {
        ssd1306_putc_hangul(ch, font, color, size);
    }
    
    /* Increase pointer */
    //SSD1306.cursor_x += font->width * size;
    
    /* Return character count written */
    return 1;
}

char ssd1306_putc_gfx(uint16_t ch, Font_t* font, SSD1306_Color_t color, uint8_t size)
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
                    ssd1306_drawpixel(SSD1306.cursor_x + xo + xx, (SSD1306.cursor_y + yo + yy), color);
                else
                    ssd1306_fillrectangle(SSD1306.cursor_x + (xo16+xx)*size, SSD1306.cursor_y + (yo16+yy)*size, size, size, color);
            }
            bits <<= 1;
        }
    }
    return 1;
}

char ssd1306_putc_hangul(uint16_t ch, Font_t* font, SSD1306_Color_t color, uint8_t size)
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
                    ssd1306_drawpixel(SSD1306.cursor_x + w*8 + j, SSD1306.cursor_y + i, color);
                else
                    ssd1306_fillrectangle(SSD1306.cursor_x + (w*8 + j)*size, SSD1306.cursor_y + i*size, size, size, color);
            }/* else {
                ssd1306_drawpixel(SSD1306.cursor_x + w*8 + j, (SSD1306.cursor_y + i), (SSD1306_Color_t)!color);
            }*/
        }
      }
    }
    return 1;
}

char ssd1306_puts(char* str, FontSet_t* fontset, SSD1306_Color_t color, uint8_t size)
{
    int i;
    char c, c2, c3;
    char count=0;
    uint16_t utf16;
    int cursor_x = SSD1306.cursor_x;

    /* Write characters */
    while (*str) {
        c = *str++;
        count++;

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
            SSD1306.cursor_x = cursor_x;
            SSD1306.cursor_y += size * fontset->height;
        } else {
            /* Write character by character */
            for (i = 0; fontset->fontlist[i] && i < 5; i++) {
                Font_t *font = fontset->fontlist[i];
                if (utf16 >= font->first && utf16 <= font->last) {
                    ssd1306_putc(utf16, font, color, size);
                    SSD1306.cursor_x += size * font->width;
                    break;
                }
            }
        }
    }
    
    /* Everything OK, char count should be returned */
    return count;
}
 

void ssd1306_drawline(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SSD1306_Color_t c)
{
    int16_t dx, dy, sx, sy, err, e2, i, tmp; 
    
    /* Check for overflow */
    if (x0 >= SSD1306_WIDTH) {
        x0 = SSD1306_WIDTH - 1;
    }
    if (x1 >= SSD1306_WIDTH) {
        x1 = SSD1306_WIDTH - 1;
    }
    if (y0 >= SSD1306_HEIGHT) {
        y0 = SSD1306_HEIGHT - 1;
    }
    if (y1 >= SSD1306_HEIGHT) {
        y1 = SSD1306_HEIGHT - 1;
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
            ssd1306_drawpixel(x0, i, c);
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
            ssd1306_drawpixel(i, y0, c);
        }
        
        /* Return from function */
        return;
    }
    
    while (1) {
        ssd1306_drawpixel(x0, y0, c); 
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

void ssd1306_drawrectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_Color_t c)
{
    /* Check input parameters */
    if (
        x >= SSD1306_WIDTH ||
        y >= SSD1306_HEIGHT
    ) {
        /* Return error */
        return;
    }
    
    /* Check width and height */
    if ((x + w) >= SSD1306_WIDTH) {
        w = SSD1306_WIDTH - x;
    }
    if ((y + h) >= SSD1306_HEIGHT) {
        h = SSD1306_HEIGHT - y;
    }
    
    /* Draw 4 lines */
    ssd1306_drawline(x, y, x + w, y, c);         /* Top line */
    ssd1306_drawline(x, y + h, x + w, y + h, c); /* Bottom line */
    ssd1306_drawline(x, y, x, y + h, c);         /* Left line */
    ssd1306_drawline(x + w, y, x + w, y + h, c); /* Right line */
}

void ssd1306_fillrectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_Color_t c)
{
    uint8_t i;
    
    /* Check input parameters */
    if (
        x >= SSD1306_WIDTH ||
        y >= SSD1306_HEIGHT
    ) {
        /* Return error */
        return;
    }
    
    /* Check width and height */
    if ((x + w) >= SSD1306_WIDTH) {
        w = SSD1306_WIDTH - x;
    }
    if ((y + h) >= SSD1306_HEIGHT) {
        h = SSD1306_HEIGHT - y;
    }
    
    /* Draw lines */
    for (i = 0; i <= h; i++) {
        /* Draw lines */
        ssd1306_drawline(x, y + i, x + w, y + i, c);
    }
}

void ssd1306_drawtriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_Color_t color)
{
    /* Draw lines */
    ssd1306_drawline(x1, y1, x2, y2, color);
    ssd1306_drawline(x2, y2, x3, y3, color);
    ssd1306_drawline(x3, y3, x1, y1, color);
}


void ssd1306_filltriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_Color_t color)
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
        ssd1306_drawline(x, y, x3, y3, color);

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

void ssd1306_drawcircle(int16_t x0, int16_t y0, int16_t r, SSD1306_Color_t c)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    //ssd1306_drawpixel(x0, y0 + r, c);
    //ssd1306_drawpixel(x0, y0 - r, c);
    //ssd1306_drawpixel(x0 + r, y0, c);
    //ssd1306_drawpixel(x0 - r, y0, c);

    while (x <= y) {
        ssd1306_drawpixel(x0 + x, y0 + y, c);
        ssd1306_drawpixel(x0 - x, y0 + y, c);
        ssd1306_drawpixel(x0 + x, y0 - y, c);
        ssd1306_drawpixel(x0 - x, y0 - y, c);

        ssd1306_drawpixel(x0 + y, y0 + x, c);
        ssd1306_drawpixel(x0 - y, y0 + x, c);
        ssd1306_drawpixel(x0 + y, y0 - x, c);
        ssd1306_drawpixel(x0 - y, y0 - x, c);

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

void ssd1306_fillcircle(int16_t x0, int16_t y0, int16_t r, SSD1306_Color_t c)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    //ssd1306_drawpixel(x0, y0 + r, c);
    //ssd1306_drawpixel(x0, y0 - r, c);
    //ssd1306_drawpixel(x0 + r, y0, c);
    //ssd1306_drawpixel(x0 - r, y0, c);
    //ssd1306_drawline(x0 - r, y0, x0 + r, y0, c);

    while (x <= y) {
        ssd1306_drawline(x0 - x, y0 + y, x0 + x, y0 + y, c);
        ssd1306_drawline(x0 + x, y0 - y, x0 - x, y0 - y, c);

        ssd1306_drawline(x0 + y, y0 + x, x0 - y, y0 + x, c);
        ssd1306_drawline(x0 + y, y0 - x, x0 - y, y0 - x, c);
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
 
void ssd1306_on(void)
{
    ssd1306_writecommand(0x8D);  
    ssd1306_writecommand(0x14);  
    ssd1306_writecommand(0xAF);  
}

void ssd1306_off(void)
{
    ssd1306_writecommand(0x8D);  
    ssd1306_writecommand(0x10);
    ssd1306_writecommand(0xAE);  
}
