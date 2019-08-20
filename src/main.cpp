// Audio Spectrum Display
// Copyright 2013 Tony DiCola (tony@tonydicola.com)

// This code is part of the guide at http://learn.adafruit.com/fft-fun-with-fourier-transforms/

#define ARM_MATH_CM4
#include <arm_math.h>
#include "FastLED.h"
#include "gfx.h"

void spectrumSetup();
void samplingBegin();
boolean samplingIsDone();
void spectrumLoop();
void parserLoop();
void parseCommand(char* command);
void ledFrame();

////////////////////////////////////////////////////////////////////////////////
// CONIFIGURATION 
// These values can be changed to alter the behavior of the spectrum display.
////////////////////////////////////////////////////////////////////////////////

int SAMPLE_RATE_HZ = 9000;             // Sample rate of the audio in hertz.
float SPECTRUM_MIN_DB = 30.0;          // Audio intensity (in decibels) that maps to low LED brightness.
float SPECTRUM_MAX_DB = 60.0;          // Audio intensity (in decibels) that maps to high LED brightness.
                                       // Useful for turning the LED display on and off with commands from the serial port.
const int FFT_SIZE = 256;              // Size of the FFT.  Realistically can only be at most 256 
                                       // without running out of memory for buffers and other state.
const int NUM_BUCKETS = 128;
const int AUDIO_INPUT_PIN = 16;        // Input ADC pin for audio data.
const int ANALOG_READ_RESOLUTION = 10; // Bits of resolution for the ADC.
const int ANALOG_READ_AVERAGING = 16;  // Number of samples to average with each ADC reading.
const int POWER_LED_PIN = 13;          // Output pin for power LED (pin 13 to use Teensy 3.0's onboard LED).
                                       // any other changes to the program.


////////////////////////////////////////////////////////////////////////////////
// INTERNAL STATE
// These shouldn't be modified unless you know what you're doing.
////////////////////////////////////////////////////////////////////////////////

IntervalTimer samplingTimer;
float samples[FFT_SIZE*2];
float magnitudes[FFT_SIZE];
int sampleCounter = 0;

////////////////////////////////////////////////////////////////////////////////
// MAIN SKETCH FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

void setup() {
  // Set up serial port.
  Serial.begin(38400);

  delay(2000);  

  // Set up ADC and audio input.
  pinMode(AUDIO_INPUT_PIN, INPUT);
  analogReadResolution(ANALOG_READ_RESOLUTION);
  analogReadAveraging(ANALOG_READ_AVERAGING);
  
  // Turn on the power indicator LED.
  pinMode(POWER_LED_PIN, OUTPUT);
  digitalWrite(POWER_LED_PIN, HIGH);
  
  // Begin sampling audio
  samplingBegin();

  FastLED.addLeds<NEOPIXEL, 2>(leds, NUM_LEDS);
}

void loop() {
  // Calculate FFT if a full sample is available.
  if (samplingIsDone()) {
    // Run FFT on sample data.
    arm_cfft_radix4_instance_f32 fft_inst;
    arm_cfft_radix4_init_f32(&fft_inst, FFT_SIZE, 0, 1);
    arm_cfft_radix4_f32(&fft_inst, samples);
    // Calculate magnitude of complex numbers output by the FFT.
    arm_cmplx_mag_f32(samples, magnitudes, FFT_SIZE);
 
    Serial.print("FFT");
    int i;
    for (i=0; i<NUM_BUCKETS; i++) {
      Serial.print(20*log10(magnitudes[i]));
      Serial.print("::");
    }
    Serial.println();

    // Restart audio sampling.
    samplingBegin();

    ledFrame();
  }
}


void samplingCallback() {
  // Read from the ADC and store the sample data
  samples[sampleCounter] = (float32_t)analogRead(AUDIO_INPUT_PIN);
  // Complex FFT functions require a coefficient for the imaginary part of the input.
  // Since we only have real data, set this coefficient to zero.
  samples[sampleCounter+1] = 0.0;
  // Update sample buffer position and stop after the buffer is filled
  sampleCounter += 2;
  if (sampleCounter >= FFT_SIZE*2) {
    samplingTimer.end();
  }
}

void samplingBegin() {
  // Reset sample buffer position and start callback at necessary rate.
  sampleCounter = 0;
  samplingTimer.begin(samplingCallback, 1000000/SAMPLE_RATE_HZ);
}

boolean samplingIsDone() {
  return sampleCounter >= FFT_SIZE*2;
}


int level_colors[8] = {
  0xDF470A,
  0xDF470A,
  0xDBC42A,
  0xDBC42A,
  0xDBC42A,
  0x2CA215,
  0x2CA215,
  0x2CA215
};

void ledFrame() {
  clearScreen();

  int i, j, y; 
  float total; 
  float level;
  int height;
  CRGB color;

  for (i = 0; i < 16; i++) {
    total = 0;
    for (j = 0; j < 8; j++) {
      if (i == 0 && j < 4) {
        /* Avoid the four first bins. */
      } else {
        total += magnitudes[(i * 8) + j];
      }
    }
    level = (20.0 * log10(total / 8.0)) - 30.0;
    if (level < 0) { level = 0.00; }
    height = (level / 20.0) * 8.0;
    color = CRGB::Green;

    height -= 1;
    if(height > 7) { height = 7; }
    for (y = 7; y >= 7 - height; y--) {
      drawpixel(i, y, level_colors[y]);
    }
  } 

  FastLED.show();
}
