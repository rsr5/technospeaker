#include <FastLED.h>
#include "gfx.h"
#include "spectrum.h"
#include "spectrum_analyzer.h"

SpectrumEffect::SpectrumEffect() {
  previousMillis = 0;
}

void SpectrumEffect::drawPeaks(CRGB *leds) {
  int i;
  for (i=0; i < 16; i++) {
    if(peaks[i] > 0) {
      drawpixel(leds, i, 7 - peaks[i], 0x808080);
    }
  }
}

void SpectrumEffect::clearPeaks(CRGB *leds) {
  int i;
  for (i=0; i < 16; i++) {
    if(peaks[i] > 0) {
      drawpixel(leds, i, 7 - peaks[i], CRGB::Black);
    }
  }
}

void SpectrumEffect::doFrame(CRGB *leds) {

  int currentMillis = millis();

  if ((currentMillis - previousMillis) > 150) {
    int i;
    clearPeaks(leds);
    for (i = 0; i < 16; i++) {
      if (peaks[i] > 0) { peaks[i] -= 1; }
    }
    drawPeaks(leds);
    FastLED.show();
    previousMillis = currentMillis;
  }

  if (areSamplesAvailable()) {
    acknowledgeSamples();
    clearScreen(leds);

    float *magnitudes = getMagnitudes();
    int i, j, y; 
    float total; 
    float level;
    int height;
    CRGB color;

    for (i = 0; i < 16; i++) {
      total = 0;
      for (j = 0; j < 4; j++) {
        if (i == 0 && j < 2) {
          /* Avoid the four first bins. */
        } else {
          total += magnitudes[(i * 8) + j];
        }
      }
      level = (20.0 * log10(total / 8.0)) - 30.0;
      if (level < 0) { level = 0.00; }
      height = (level / 18.0) * 8.0;
      color = CRGB::Green;

      height -= 1;
      if(height > 7) { height = 7; }
      for (y = 7; y >= 7 - height; y--) {
        drawpixel(leds, i, y, level_colors[y]);
      }

      if (height > peaks[i]) { peaks[i] = height; }
    } 

    drawPeaks(leds);
    FastLED.show();
  
  }
}
