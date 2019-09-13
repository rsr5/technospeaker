#ifndef __SPECTRUM_ANALYZER_H
#define __SPECTRUM_ANALYZER_H

#include <Arduino.h>
#define ARM_MATH_CM4
#include <arm_math.h>

void setupSpectrumAnalyzer();
void processSpectrumAnalyzer();
void samplingBegin();
bool samplingIsDone();
void parserLoop();
void parseCommand(char* command);
float *getMagnitudes();
bool areSamplesAvailable();
void acknowledgeSamples();

#define SAMPLE_RATE_HZ 9000          // Sample rate of the audio in hertz.
#define SPECTRUM_MIN_DB 30.0         // Audio intensity (in decibels) that maps to low LED brightness.
#define SPECTRUM_MAX_DB 60.0         // Audio intensity (in decibels) that maps to high LED brightness.
                                     // Useful for turning the LED display on and off with commands from the serial port.
#define FFT_SIZE 256                 // Size of the FFT.  Realistically can only be at most 256 
                                     // without running out of memory for buffers and other state.
#define NUM_BUCKETS 128
#define AUDIO_INPUT_PIN 16           // Input ADC pin for audio data.
#define ANALOG_READ_RESOLUTION 10    // Bits of resolution for the ADC.
#define ANALOG_READ_AVERAGING 16     // Number of samples to average with each ADC reading.
#define POWER_LED_PIN 13             // Power led pin

#endif
