#ifndef __TEXT_H
#define __TEXT_H

#include <FastLED.h>


class TextEffect {
  private:
  unsigned long previousMillis;
  int delta;
  int offset;

  public:
  TextEffect();
  void doFrame(CRGB *leds);
};

#endif
