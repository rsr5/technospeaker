#include <ArduinoJson.h>
#include <FastLED.h>
#include "gfx.h"
#include "balls.h"

#define NONE 0
#define BALLS 1
#define BLINKER 2
#define VOLUME 3

CRGB leds[NUM_LEDS];
CRGB current_colour = CRGB::Red;

String inputString = "";
StaticJsonDocument<200> command;

bool stringComplete = false;
bool showColour = false;

int theme = BALLS;
int lastTheme = BALLS;

class VolumeEffect {
  unsigned long previousMillis;
  int volume;

  public:
  VolumeEffect() {
    volume = 0;
    previousMillis = 0;
  }

  void setVolume(int newVolume) {
    volume = newVolume;
    previousMillis = millis();
  }

  void doFrame() {
    unsigned long currentMillis = millis();
    int num_leds = (int)((float)volume / 100.0 * NUM_LEDS);

    for(int led = 0; led < num_leds; led++) {
      leds[led] = CRGB::Blue;
    }
    for(int led = num_leds; led < NUM_LEDS; led++) {
      leds[led] = CRGB::Red;
    }
    FastLED.show();

    if ((currentMillis - previousMillis) >= 2000) {
      theme = lastTheme;
    }
  }
};


void setup() {
  delay(1000);

  FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(16);

  Serial.begin(9600);
  inputString.reserve(200);

  clearScreen(leds);
}

BallsEffect ballsEffect;
VolumeEffect volumeEffect;

void loop() {
  if (theme == BALLS) {
    ballsEffect.doFrame(leds);
  } else if (theme == VOLUME) {
    volumeEffect.doFrame();
  } else {
    clearScreen(leds);
  }

  if (Serial.available() > 0) {
    while (Serial.available()) {
      char c = (char)Serial.read();
      inputString += c;
      if (c == '\n') {
        stringComplete = true;
      }
    }
  }

  if (stringComplete) {
    DeserializationError error = deserializeJson(command, inputString);

    if (command["theme"] == "balls") {
      lastTheme = theme;
      theme = BALLS;
    } else if (command["theme"] == "volume") {
      if (theme != VOLUME) {
        lastTheme = theme;
      }
      theme = VOLUME;
      volumeEffect.setVolume(command["arg"]);
    } else if (command["theme"] == "none") {
      theme = NONE;
    }
    inputString = "";
    stringComplete = false;
  }
}
