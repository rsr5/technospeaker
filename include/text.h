#ifndef __TEXT_H
#define __TEXT_H

#include <FastLED.h>


class TextEffect {
  private:
  unsigned long previousMillis;
  int delta;
  int offset;
  int message_width;

  String message = "";

  public:
  TextEffect();
  int messageWidth();
  void doFrame(CRGB *leds);
  void setMessage(String message);
};

#endif
