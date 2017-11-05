// Lightbulb PCB sketch (build for "Arduino Gemma" and remember to turn off CLKDIV8 fuse)
// Matthew Petroff, 2017

// Based on:
// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            1
#define LIGHT_SENSOR_PIN 2

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      7

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN);

int delayval = 500; // delay for half a second

void setup() {
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  
  strip.setBrightness(10);
  strip.begin(); // This initializes the NeoPixel library.
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  setBrightness();
  for (uint8_t i = 0; i < 10; i++) {
    setColor(strip.Color(255, 255, 0)); // Yellow
    delay(100);
    setColor(strip.Color(0, 0, 0));
    delay(100);
  }
  
  setBrightness();
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  delay(100);
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  delay(100);
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
  delay(100);
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  delay(100);
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  delay(100);
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
  delay(100);

  setBrightness();
  theaterChase(strip.Color(127, 127, 127), 50); // White
  theaterChase(strip.Color(127, 0, 0), 50); // Red
  theaterChase(strip.Color(0, 0, 127), 50); // Blue

  setBrightness();
  rainbow(20);
  setBrightness();
  rainbowCycle(20);
  //setBrightness();
  //theaterChaseRainbow(50);
}

// Set all pixels to same color
void setColor(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++)
    strip.setPixelColor(i, c);
  strip.show();
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// Use ambient light sensor to set brightness; without a diffuser, almost everything is too bright...
void setBrightness() {
  unsigned int duration = pulseIn(LIGHT_SENSOR_PIN, HIGH, 1000);
  if (!duration)
    duration = 1000;
  duration = min(duration, 50);
  duration = 50 - duration;
  unsigned int brightness = 10 * duration / 50;
  brightness = max(brightness, 2);
  strip.setBrightness(brightness);
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
