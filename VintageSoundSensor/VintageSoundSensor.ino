/**
 * \file VintageSoundSensor.ino 
 * \brief Sensing the sound level and monitoring the intensity with lights and the gauge level
 * 
 * This is part of the Art-a-Tronic Vintage upcycling creations series
 * 
 * \author Enrico Miglino <balearicdynamics@gmail.com>
 * \version 1.0
 * \date Aug 2019
 */

#include "globals.h"

#undef _DEBUG

int analogValue = 0;  ///< Analog value coming from the Sound Sensor
int digitalValue;     ///< Digital value coming from the Sound Sensor

//! Neopixel class instance
Adafruit_NeoPixel strip(NEOPIXEL_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

//! Rainbow control structure
Rainbow rainbowControl;

/**
 * Initialization and setting on startup
 * 
 * Setup the GPIO pins for the audio sensor and the Neopixel class initial settings:
 * all pixel off and light intensity to the minimum value
 */
void setup()
{
#ifdef _DEBUG
  Serial.begin(115200);
#endif
  
  pinMode(SENSOR_DIGITAL,INPUT);
  pinMode(SENSOR_LED,OUTPUT);

  strip.begin();           // Initialize the NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(LIGHT_MIN); // Set brightness to the minimum value on startup

  // Initialize the rainbow control structure
  rainbowControl.intensity = LIGHT_MIN;
  rainbowControl.firstPixelHue = 0;
}

/**
 * Main loop function
 * 
 * \note The digitalValue is not used to trigger automatically the value that is triggered everytime the sound
 * level goes over the sensor calibration (when the sensor red LED goes off). Here we calculate the 
 * sound level depending on the analog read and the preset limits. \n
 * The digitalValue is considered anyway to force the LED triggering accordingly, but this means that the
 * snesor calibration has not been done well.
 * Sensor calibration should be done anyway: To calibrate the sensor reglulate the trimmer on the sensor board
 * until the calibration red LED goes off or start blinking. This is the expected setting for the sensitivy level
 * In case of very high sound incread decrease the sensitivity level to have always an average value of 500-215 
 * in the analog read.
 */
void loop(){
  analogValue = analogRead(SENSOR_ANALOG);
  digitalValue = digitalRead(SENSOR_DIGITAL);
  
//  if(digitalValue == HIGH) {
//    // Just trigger the LED
//    digitalWrite(SENSOR_LED, HIGH);
//  }
//  else {
    // Check the detected value and trigger the LED accordingly
    if(analogValue > SENSOR_SENSITIVITY) {
      digitalWrite(SENSOR_LED,HIGH);
      } else {
        digitalWrite(SENSOR_LED,LOW);
      }
//  }

  updateIntensity();
  updateRainbowPixel();
  stepRainbow();
  delay(SAMPLE_INTERVAL);                  // Slight pause so that we don't overwhelm the serial interface
}

/**
 * Update the light intensity accordingly with the last sensor reading
 */
void updateIntensity() {

  if(analogValue < 520) {
    rainbowControl.intensity = LIGHT_MIN;
  } else if(analogValue > 540) {
    rainbowControl.intensity = LIGHT_MAX;
  } else {
    rainbowControl.intensity = map(analogValue, 520, 600, LIGHT_MIN, LIGHT_MAX);
  }
  
#ifdef _DEBUG
  Serial.println(rainbowControl.intensity);
//  Serial.println(analogValue);
#endif
}

/**
 * Update the main light rainbow pixel hue
 * 
 * Hue of first pixel runs 5 complete loops through the color wheel.
 * Color wheel has a range of 65536 but it's OK if we roll over, so
 * just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
 * means we'll make 5*65536/256 = 1280 passes through this outer loop:
 */
void updateRainbowPixel() {
  rainbowControl.firstPixelHue += 256;

  // Check for the limit and eventually reset the counter
  if(rainbowControl.firstPixelHue >= 5*65536) {
    rainbowControl.firstPixelHue = 0;
  }
}

/** 
 *  Rainbow cycle along whole strip.
 *  
 *  The function executes a single rainbow step, controlled by the Rainbow
 *  globla structure. The funcion is a single-step event to be interruptable
 *  inside a more conplex cycle.
 */
void stepRainbow() {
  // Set the intensity accordingly with the last sensor reading
  strip.setBrightness(rainbowControl.intensity);

  // Execute a rainbow step
  for(int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
    // Offset pixel hue by an amount to make one full revolution of the
    // color wheel (range of 65536) along the length of the strip
    // (strip.numPixels() steps):
    int pixelHue = rainbowControl.firstPixelHue + (i * 65536L / strip.numPixels());
    // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
    // optionally add saturation and value (brightness) (each 0 to 255).
    // Here we're using just the single-argument hue variant. The result
    // is passed through strip.gamma32() to provide 'truer' colors
    // before assigning to each pixel:
    strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
  }
  strip.show(); // Update strip with new contents
}
