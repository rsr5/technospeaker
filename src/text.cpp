#include <FastLED.h>
#include "gfx.h"
#include "text.h"

#include "fonts/font8x8_ib8x8u.h"

#define FONT8X8 font8x8_ib8x8u

const byte *getImage(byte c)
{
    if (c < 128)
    {
        return FONT8X8[c];
    }
/*
    if (ch >= 128 && ch < 160)
    {
        // miscellaneous
        ch -= 128;
        if (ch < sizeof(font8x8_misc) / 8)
        {
            return font8x8_misc[ch];
        }
        return FONT8X8[0]; // default
    }
    return FONT8X8[ch - 160 + 128];
*/
}


#define BUF_HEIGHT 8
#define BUF_WIDTH 400


uint8_t buffer[BUF_HEIGHT * BUF_WIDTH];


void clearbuffer() {
  memset(buffer, 0, BUF_HEIGHT * BUF_WIDTH * sizeof(uint8_t));
}

void drawpixelbuf(int x, int y) {
  buffer[(y * BUF_WIDTH) + x] = 1;
}

void printchar(int x, int y, byte c) {
    const byte *image = getImage(c);
    for (int i = 0; i < 8; i++) {
      if (image[i] & 0x1) {
        drawpixelbuf(x + 7, y + i);
      }
      for (int j = 0; j < 7; j++) {
        if (image[i] & (2 << j)) {
          drawpixelbuf(x + (7 - j), y + i);
        }
      }
    }
}

void printstr(String *string) {
  for (int i = 0; i < string->length(); i++) {
    printchar(i * 8, 0, string->charAt(i));
  }
}


TextEffect::TextEffect() {
  previousMillis = 0;
  delta = 1;
  offset = 0;

  printstr(&message);
}

int TextEffect::messageWidth() {
  return message.length() * 8;
}

void TextEffect::setMessage(String message) {
  this->message = message;
  clearbuffer();
  printstr(&this->message);
}

void TextEffect::doFrame(CRGB *leds) {
  unsigned long currentMillis = millis();
  if ((currentMillis - previousMillis) >= 100) {
    previousMillis = currentMillis;

    offset += delta;
    if (offset >= messageWidth() - 16) {
      offset = 0;
    }
    if (offset < 0) {
      offset = 0;
      delta = 1;
    }

    for (int j = 0; j < 8; j++) {
      for (int i = 0; i < 16; i++) {
        if (buffer[(j * BUF_WIDTH) + i + offset]) {
          drawpixel(leds, i, j, CRGB::Green);
        } else {
          drawpixel(leds, i, j, CRGB::Black);
        }
      }
    }

    FastLED.show();
  }
}
