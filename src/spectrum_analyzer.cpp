#include "spectrum_analyzer.h"

IntervalTimer samplingTimer;
float samples[FFT_SIZE * 2];
float magnitudes[FFT_SIZE];
int sampleCounter = 0;

bool newSamplesAvailable = false;

bool areSamplesAvailable() {
  return newSamplesAvailable;
}

void acknowledgeSamples() { newSamplesAvailable = false; }

float *getMagnitudes() { return magnitudes; }

void setupSpectrumAnalyzer() {
  // Set up ADC and audio input.
  pinMode(AUDIO_INPUT_PIN, INPUT);
  analogReadResolution(ANALOG_READ_RESOLUTION);
  analogReadAveraging(ANALOG_READ_AVERAGING);
  
  // Turn on the power indicator LED.
  pinMode(POWER_LED_PIN, OUTPUT);
  digitalWrite(POWER_LED_PIN, HIGH);
  
  // Begin sampling audio
  samplingBegin();
}

void processSpectrumAnalyzer() {
  // Calculate FFT if a full sample is available.
  if (samplingIsDone()) {
    // Run FFT on sample data.
    arm_cfft_radix4_instance_f32 fft_inst;
    arm_cfft_radix4_init_f32(&fft_inst, FFT_SIZE, 0, 1);
    arm_cfft_radix4_f32(&fft_inst, samples);
    // Calculate magnitude of complex numbers output by the FFT.
    arm_cmplx_mag_f32(samples, magnitudes, FFT_SIZE);
/*
    Serial.print("FFT");
    int i;
    for (i=0; i<NUM_BUCKETS; i++) {
      Serial.print(20*log10(magnitudes[i]));
      Serial.print("::");
    }
    Serial.println();
*/
    // Restart audio sampling.
    samplingBegin();
    newSamplesAvailable = true; 
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

bool samplingIsDone() {
  return sampleCounter >= FFT_SIZE*2;
}
