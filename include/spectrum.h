#ifndef __SPECTRUM_H
#define __SPECTRUM_H

#include <FastLED.h>

class SpectrumEffect {
  private:
  unsigned long previousMillis;
  int delta;
  int level_colors[8] = {
    0xFF0000,
    0xFF0000,
    0xFF0000,
    0xFFA500,
    0xFFA500,
    0x00FF00,
    0x00FF00,
    0x0000FF
  };
  int peaks[16] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };

  void drawPeaks(CRGB *leds);
  void clearPeaks(CRGB *leds);

  public:
  SpectrumEffect();
  void doFrame(CRGB *leds);
};

#endif
