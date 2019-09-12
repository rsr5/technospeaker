#include <ArduinoJson.h>
#include <FastLED.h>

#define NUM_LEDS 12
#define DATA_PIN 2
#define NONE 0
#define SPINNER 1
#define BLINKER 2
#define VOLUME 3


CRGB leds[NUM_LEDS];
CRGB current_colour = CRGB::Red;

String inputString = "";
StaticJsonDocument<200> command;

bool stringComplete = false;
bool showColour = false;

int theme = NONE;
int lastTheme = NONE;

int map_led(int led) {
  static int mapping[12] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0, 1};
  return mapping[led];
}


class SpinnerEffect {
  int current_colour;
  int num_colours;
  CRGB colours[5];
  unsigned long previousMillis;
  int current_led;

  public:
  SpinnerEffect() {
    colours[0] = CRGB::Red;
    colours[1] = CRGB::Orange;
    colours[2] = CRGB::Yellow;
    colours[3] = CRGB::Green;
    colours[4] = CRGB::Blue;

    num_colours = 5;
    current_colour = 0;
    previousMillis = 0;
    current_led = 0;
  }

  void doFrame() {
    unsigned long currentMillis = millis();

    if ((currentMillis - previousMillis) >= 100) {
      leds[map_led(current_led)] = CRGB::Black;
      current_led++;

      if (current_led >= NUM_LEDS) {
        current_led = 0;
        current_colour++;
        if (current_colour > num_colours) {
          current_colour = 0;
        }
      }

      leds[map_led(current_led)] = colours[current_colour];
      FastLED.show();
      previousMillis = currentMillis;
    }
  }
};


class BlinkerEffect {
  unsigned long previousMillis;
  int brightness;
  int delta;

  public:
  BlinkerEffect() {
    previousMillis = 0;
    brightness = 0;
    delta = 25;
  }

  void doFrame() {
    unsigned long currentMillis = millis();

    FastLED.setBrightness(brightness);
    for(int led = 0; led < NUM_LEDS; led++) {
       leds[map_led(led)] = CRGB::White;

       FastLED.show();
    }
    if ((currentMillis - previousMillis) >= 100) {
      brightness += delta;
      if (brightness > 255 || brightness < 0) {
        delta *= -1;
        brightness += delta;
      }
    }
  }
};


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
    static int mapping[12] = {8, 9, 10, 11, 0, 1, 2, 3, 4, 5, 6, 7};

    for(int led = 0; led < num_leds; led++) {
      leds[mapping[led]] = CRGB::Blue;
    }
    for(int led = num_leds; led < NUM_LEDS; led++) {
      leds[mapping[led]] = CRGB::Red;
    }
    FastLED.show();

    if ((currentMillis - previousMillis) >= 2000) {
      theme = lastTheme;
    }
  }
};


void stopLeds() {
 for(int led = 0; led < NUM_LEDS; led++) {
    leds[map_led(led)] = CRGB::Black;
    FastLED.show();
  }
}

void setup() {
  delay(1000);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(126);

  Serial.begin(9600);
  inputString.reserve(200);

  stopLeds();
}

SpinnerEffect spinnerEffect;
BlinkerEffect blinkerEffect;
VolumeEffect volumeEffect;

void loop() {
  if (theme == SPINNER) {
    spinnerEffect.doFrame();
  } else if (theme == BLINKER) {
    blinkerEffect.doFrame();
  } else if (theme == VOLUME) {
    volumeEffect.doFrame();
  } else {
    stopLeds();
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

    if (command["theme"] == "spinner") {
      lastTheme = theme;
      theme = SPINNER;
    } else if (command["theme"] == "blinker") {
      lastTheme = theme;
      theme = BLINKER;
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
