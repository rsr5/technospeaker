#ifndef __BALLS_H
#define __BALLS_H

#include <FastLED.h>

struct ball {
  int x, y, vx, vy;
  CRGB color;
};

void drawball(CRGB *leds, struct ball *ball);
void clearball(CRGB *leds, struct ball *ball);
void moveball(struct ball *ball);

class BallsEffect {
  private:
  unsigned long previousMillis;
  int delta;

  struct ball redball;
  struct ball blueball;
  struct ball greenball;
  struct ball purpleball;
  struct ball yellowball;
  struct ball cyanball;

  public:
  BallsEffect();
  void doFrame(CRGB *leds);
};

#endif
