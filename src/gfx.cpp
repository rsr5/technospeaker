#include "gfx.h"

void drawpixel(CRGB *leds, int x, int y, CRGB color) {
  leds[mapp(x, y)] = color;
}

void clearScreen(CRGB *leds) {
  int i;
  for (i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
}
