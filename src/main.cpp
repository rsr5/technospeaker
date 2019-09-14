#include <ArduinoJson.h>
#include <FastLED.h>
#include "gfx.h"
#include "balls.h"
#include "spectrum.h"
#include "text.h"
#include "spectrum_analyzer.h"

#define NONE 0
#define BALLS 1
#define SPECTRUM 2
#define TEXT 4

CRGB leds[NUM_LEDS];
CRGB current_colour = CRGB::Red;

String inputString = "";
StaticJsonDocument<200> command;

bool stringComplete = false;
bool showColour = false;

int theme = BALLS;
int lastTheme = BALLS;

BallsEffect ballsEffect;
SpectrumEffect spectrumEffect;
TextEffect textEffect;


void setup() {
  delay(1000);

  FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(16);

  Serial.begin(9600);
  inputString.reserve(200);

  clearScreen(leds);

  setupSpectrumAnalyzer();
}

void loop() {
  processSpectrumAnalyzer();

  if (theme == BALLS) {
    ballsEffect.doFrame(leds);
  } else if (theme == SPECTRUM) {
    spectrumEffect.doFrame(leds);
  } else if (theme == TEXT) {
    textEffect.doFrame(leds);
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
    } else if (command["theme"] == "spectrum") {
      lastTheme = theme;
      theme = SPECTRUM;
    } else if (command["theme"] == "text") {
      lastTheme = theme;
      theme = TEXT;
      textEffect.setMessage(command["message"]);
    } else if (command["theme"] == "none") {
      theme = NONE;
    }
    inputString = "";
    stringComplete = false;
  }
}
