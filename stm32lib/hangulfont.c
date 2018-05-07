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
#include "../stm32lib/font.h"

uint8_t hangul_glyph[32] = {0, };

/* process hangul font */
uint8_t *get_hangul_glyph(uint16_t ch, Font_t *font)
{
  const char cho[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 3, 1, 2, 4, 4, 4, 2, 1, 3, 0 };
  const char cho2[] = { 0, 5, 5, 5, 5, 5, 5, 5, 5, 6, 7, 7, 7, 6, 6, 7, 7, 7, 6, 6, 7, 5 };
  const char jong[] = { 0, 0, 2, 0, 2, 1, 2, 1, 2, 3, 0, 2, 1, 3, 3, 1, 2, 1, 3, 3, 1, 1 };

  char first, mid, last;
  char firstType, midType, lastType;
  char i;
  const CombFont_t *combfont = font->data.combfont;
  char glyphbyte = (font->height * font->width) / 8;
  uint8_t *pB, *pF;

  /*------------------------------
    초,중,종성 코드를 분리해 낸다.

    unicode = {[(초성 * 21) + 중성] * 28}+ 종성 + 0xAC00

          0   1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 
    초성 ㄱ   ㄲ ㄴ ㄷ ㄸ ㄹ ㅁ ㅂ ㅃ ㅅ ㅆ ㅇ ㅈ ㅉ ㅊ ㅋ ㅌ ㅍ ㅎ
    중성 ㅏ   ㅐ ㅑ ㅒ ㅓ ㅔ ㅕ ㅖ ㅗ ㅘ ㅙ ㅚ ㅛ ㅜ ㅝ ㅞ ㅟ ㅠ ㅡ ㅢ ㅣ  
    종성 없음 ㄱ ㄲ ㄳ ㄴ ㄵ ㄶ ㄷ ㄹ ㄺ ㄻ ㄼ ㄽ ㄾ ㄿ ㅀ ㅁ ㅂ ㅄ ㅅ ㅆ ㅇ ㅈ ㅊ ㅋ ㅌ ㅍ ㅎ
  ------------------------------*/
  ch -= font->first;
  last = ch % 28;
  ch /= 28;
  mid = ch % 21;
  first = ch / 21;
  printf("cho=%d, jung=%d, jong=%d\r\n", first, mid, last);

  first++;
  mid++;

  /*------------------------------
    초,중,종성 해당 폰트 타입(벌)을 결정한다.
  ------------------------------*/

  /*
   초성 19자:ㄱㄲㄴㄷㄸㄹㅁㅂㅃㅅㅆㅇㅈㅉㅊㅋㅌㅍㅎ
   중성 21자:ㅏㅐㅑㅒㅓㅔㅕㅖㅗㅘㅙㅚㅛㅜㅝㅞㅟㅠㅡㅢㅣ
   종성 27자:ㄱㄲㄳㄴㄵㄶㄷㄹㄺㄻㄼㄽㄾㄿㅀㅁㅂㅄㅆㅇㅈㅊㅋㅌㅍㅎ

   초성
      초성 1벌 : 받침없는 'ㅏㅐㅑㅒㅓㅔㅕㅖㅣ' 와 결합
      초성 2벌 : 받침없는 'ㅗㅛㅡ'
      초성 3벌 : 받침없는 'ㅜㅠ'
      초성 4벌 : 받침없는 'ㅘㅙㅚㅢ'
      초성 5벌 : 받침없는 'ㅝㅞㅟ'
      초성 6벌 : 받침있는 'ㅏㅐㅑㅒㅓㅔㅕㅖㅣ' 와 결합
      초성 7벌 : 받침있는 'ㅗㅛㅜㅠㅡ'
      초성 8벌 : 받침있는 'ㅘㅙㅚㅢㅝㅞㅟ'

   중성
      중성 1벌 : 받침없는 'ㄱㅋ' 와 결합
      중성 2벌 : 받침없는 'ㄱㅋ' 이외의 자음
      중성 3벌 : 받침있는 'ㄱㅋ' 와 결합
      중성 4벌 : 받침있는 'ㄱㅋ' 이외의 자음

   종성
      종성 1벌 : 중성 'ㅏㅑㅘ' 와 결합
      종성 2벌 : 중성 'ㅓㅕㅚㅝㅟㅢㅣ'
      종성 3벌 : 중성 'ㅐㅒㅔㅖㅙㅞ'
      종성 4벌 : 중성 'ㅗㅛㅜㅠㅡ'

  */
  if (!last) {  //받침 없는 경우
    firstType = cho[mid];
    //if (first == 1 || first == 24) midType = 0;
    if (first == 0 || first == 15) midType = 0;
    else midType = 1;
  }
  else {       //받침 있는 경우
    firstType = cho2[mid];
    //if (first == 1 || first == 24) midType = 2;
    if (first == 0 || first == 15) midType = 2;
    else midType = 3;
    lastType = jong[mid];
  }
  memset(hangul_glyph, 0, glyphbyte);

  //초성 
  pB = hangul_glyph;
  pF = (uint8_t *)combfont->bitmap + (firstType*20 + first)*glyphbyte;
  i = glyphbyte;
  while(i--) *pB++ = *pF++; 

  //중성
  pB = hangul_glyph;
  pF = (uint8_t *)combfont->bitmap + (8*20 + midType*22 + mid)*glyphbyte;
  i = glyphbyte;
  while(i--) *pB++ |= *pF++; 

  //종성
  if (last) {
    pB = hangul_glyph;
    pF = (uint8_t *)combfont->bitmap + (8*20 + 4*22 + lastType*28 + last)*glyphbyte;
    i = glyphbyte;
    while(i--) *pB++ |= *pF++; 
  }

  return hangul_glyph;
}


static char c1;  // Last character buffer
char utf8ascii(char ascii)
{
    if (ascii < 128) {   // Standard ASCII-set 0..0x7F handling  
        c1 = 0;
        return (ascii);
    }

    // get previous input
    char last = c1;   // get last char
    c1 = ascii;         // remember actual character

    switch (last) {     // conversion depnding on first UTF8-character
    case 0xC2: return (ascii);  break;
    case 0xC3: return (ascii | 0xC0);  break;
    case 0x82: if(ascii==0xAC) return(0x80);       // special case Euro-symbol
    }

    return  (0);                              // otherwise: return zero, if character has to be ignored
}

void utf8strascii(char* s)
{      
    int k = 0;
    char c;
    for (int i = 0; i < strlen(s); i++) {
        c = utf8ascii(s[i]);
        if (c != 0)
            s[k++] = c;
    }
    s[k] = 0;
}
