/**
 * \file globals.h
 * \brief Global constants and definitions
 * 
 * \author Enrico Miglino <balearicdynamics@gmail.com>
 * \version 1.0
 * \date Aug 2019
 */

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


struct Rainbow {
  int intensity;        ///< Light intensity, set accordingly with the last sound level reading
  long firstPixelHue;   ///< The first pixel hue value of the rainbow rotating sequence
};

// Audio sensor constants
#define SENSOR_ANALOG A0    ///< Arduino input pin to accept the Sound Sensor's analog output 
#define SENSOR_DIGITAL 3    ///< Arduino input pin to accept the Sound Sensor's digital output
#define SENSOR_LED 4        ///< LED for direct soundsensor feedback

/**
 * The sensor sensitivity level is the Analog value (around the mid of the 10 bits of the AD Arduino port)
 * You can set this value accoridngly with the environmental conditions where the sensor should work.
 * Alternatively, a potentiometer on analog input A1 can be used to dynamically control the final 
 * sensitivity level that is the analog level triggering the LED.
 */
#define SENSOR_SENSITIVITY 520

/**
 * Delay between two samples. Every cycle between the samplea sueinf wcwey loop cycle. This delay
 * is applied to the timed tRainbow funciton that executes one step (with this delay) everytime 
 * it is called.
 */
#define SAMPLE_INTERVAL 3

//! Neopixel constants
#define NEOPIXEL_PIN 6    ///< Neopixel signal pin
//! Number of Neopixel LEDs in the strip (four sequential 8 LEDs strips)
#define NEOPIXEL_LEDS 32
//! Minimum light intensity (absolute min = 0)
#define LIGHT_MIN 5
//! Maximum light intensity (absolute max = 255)
#define LIGHT_MAX 50
