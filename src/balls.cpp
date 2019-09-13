#include <FastLED.h>
#include "gfx.h"
#include "balls.h"

void drawball(CRGB *leds, struct ball *ball) {
    drawpixel(leds, ball->x, ball->y, ball->color);
}

void clearball(CRGB *leds, struct ball *ball) {
    drawpixel(leds, ball->x, ball->y, CRGB::Black);
}

void moveball(struct ball *ball) {
    ball->x += ball->vx;
    ball->y += ball->vy;

    if (ball->x == 0) { ball->vx = 1; }
    if (ball->x == WIDTH - 1) { ball->vx = -1; }
    if (ball->y == 0) { ball->vy = 1; }
    if (ball->y == HEIGHT - 1) { ball->vy = -1; }
}

BallsEffect::BallsEffect() {
  previousMillis = 0;
  delta = 1;

  redball.x = 0;
  redball.y = 0;
  redball.vx = 1;
  redball.vy = 1;
  redball.color = CRGB::Red;

  blueball.x = 5;
  blueball.y = 5;
  blueball.vx = 1;
  blueball.vy = -1;
  blueball.color = CRGB::Blue;

  greenball.x = 3;
  greenball.y = 3;
  greenball.vx = -1;
  greenball.vy = -1;
  greenball.color = CRGB::Green;

  purpleball.x = 6;
  purpleball.y = 6;
  purpleball.vx = -1;
  purpleball.vy = 1;
  purpleball.color = CRGB::Purple;

  yellowball.x = 10;
  yellowball.y = 3;
  yellowball.vx = 1;
  yellowball.vy = -1;
  yellowball.color = CRGB::Yellow;

  cyanball.x = 1;
  cyanball.y = 6;
  cyanball.vx = 1;
  cyanball.vy = 1;
  cyanball.color = CRGB::Cyan;

}

void BallsEffect::doFrame(CRGB *leds) {
  unsigned long currentMillis = millis();
  if ((currentMillis - previousMillis) >= 100) {
    clearball(leds, &redball);
    clearball(leds, &blueball);
    clearball(leds, &greenball);
    clearball(leds, &purpleball);
    clearball(leds, &yellowball);
    clearball(leds, &cyanball);

    moveball(&redball);
    moveball(&blueball);
    moveball(&greenball);
    moveball(&purpleball);
    moveball(&yellowball);
    moveball(&cyanball);

    drawball(leds, &redball);
    drawball(leds, &blueball);
    drawball(leds, &greenball);
    drawball(leds, &purpleball);
    drawball(leds, &yellowball);
    drawball(leds, &cyanball);

    previousMillis = currentMillis;

    FastLED.show();
  }
}
