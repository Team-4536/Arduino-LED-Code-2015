#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN 13
const int NUM_PIXELS = 30;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code


  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  randomSeed(100);
}

void loop() {
  
  //Randomness!
  /*for (int i = 0; i < NUM_PIXELS; i++) {
    
      strip.setPixelColor(i, strip.Color(random(0, 256), random(0, 256), random(0, 256)));
      strip.show();
      delay(50);
  }*/
  
  //Dance of the Snowdrop
  //danceOfTheSnowDrop();
  
  //Mirror
  bigBang(strip.Color(0, 255, 255), 50);
}

/* Reflect reflects only a pair of pixels as opposed to mirror
 * which reflects an entire strip.
 */
//@author Liam
void reflect(uint32_t c, uint32_t pixel) {
  
  strip.setPixelColor(pixel, c);
  strip.setPixelColor((NUM_PIXELS - pixel -1), c);
  strip.show();
}  
  
//Set all the pixels from both ends meeting in the middle
//@author Liam
void mirror(uint32_t c, uint8_t wait) {
  
  for (int i = 0; i < NUM_PIXELS/2; i++) {
    
     reflect(c, i);
     delay(wait);
  }
  
  if (NUM_PIXELS % 2 == 1) {
    
    strip.setPixelColor((NUM_PIXELS/2+1), c);
    strip.show();
    delay(wait);
  }
}

void mirror(uint32_t c, uint8_t wait, uint8_t numPixels) {
  
  for (int i = 0; i < numPixels/2; i++) {
    
     reflect(c, i);
     delay(wait);
  }
  
  if (numPixels % 2 == 1) {
    
    strip.setPixelColor((numPixels/2+1), c);
    strip.show();
    delay(wait);
  }
}

//Mirror that builds up in a spurring manner
//@author Liam
void crescendoMirror(uint32_t c, uint8_t wait) {
  
  for (int i = 0; i < NUM_PIXELS/2+1; i++) {
    
    for (int j = 0; j < i; j++) {
      
      reflect(c, j);
      delay(wait);
    }
    
    for (int j = 0; j < i; j++) {
      
      reflect(strip.Color(0, 0, 0), j);
    }
    delay(wait);
  }
  
  if (NUM_PIXELS % 2 == 1) {
    
    strip.setPixelColor((NUM_PIXELS/2+1), c);
    strip.show();
  }
}

void crescendoMirror(uint32_t c, uint8_t wait, uint8_t numPixels) {
  
  for (int i = 0; i < numPixels/2+1; i++) {
    
    for (int j = 0; j < i; j++) {
      
      reflect(c, j);
      delay(wait);
    }
    
    for (int j = 0; j < i; j++) {
      
      reflect(strip.Color(0, 0, 0), j);
    }
    delay(wait);
  }
  
  if (numPixels % 2 == 1) {
    
    strip.setPixelColor((numPixels/2+1), c);
    strip.show();
  }
}

//A glittering white backdrop of LEDs
//@author Liam
void danceOfTheSnowDrop() {
  
  const uint32_t backDrop = 50;
  
  for (int i = 0; i < NUM_PIXELS; i++) {
    
    strip.setPixelColor(i, strip.Color(backDrop, backDrop, backDrop));
    strip.show();
  }
  
  for (int i = 0; i < random(1, 31); i++) {
    
    uint32_t color = random(200, 256);
    uint32_t pixel = random(1, 31);
        
    strip.setPixelColor(pixel, strip.Color(color, color, color));
    strip.show();
    delay(500);
    strip.setPixelColor(pixel, strip.Color(backDrop, backDrop, backDrop));
    strip.show();
  }
}

//Explosion - LED Display exploding out from the center of the strip
//@author Liam
void explosion(uint32_t c, uint8_t wait) {
  
    for (int i = NUM_PIXELS/2; i < NUM_PIXELS; i++) {
      
      reflect(c, i);
      delay(wait);
    }
}

void explosion(uint32_t c, uint8_t wait, uint8_t numPixels) {
  
    for (int i = NUM_PIXELS/2; i < (NUM_PIXELS/2 + numPixels); i++) {
      
      reflect(c, i);
      delay(wait);
    }
}

//Wondrous display exploding from the center of the strip
//@author Liam
void bigBang(uint32_t c, uint8_t wait) {
  
  const uint32_t backDrop = strip.Color(5, 5, 5);
  
  //mirror(backDrop, wait);
  
  for (int i = 1; i < (NUM_PIXELS/2+1); i++) {
    
    oscillate(c, wait, i);
  }
}

//oscilate - an oscillating LED strip display
//@author Liam
void oscillate(uint32_t c, uint8_t wait) {
  
  const uint32_t backDrop = strip.Color(5, 5, 5);
  
  explosion(c, wait);
  mirror(backDrop, wait);
}

void oscillate(uint32_t c, uint8_t wait, uint8_t oscilHeight) {
  
  const uint32_t backDrop = strip.Color(5, 5, 5);
  
  explosion(c, wait, oscilHeight);
  mirror(backDrop, wait);
}

void oscillate(uint32_t c1, uint32_t c2, uint8_t wait, uint8_t oscilHeight) {
  
  explosion(c1, wait, oscilHeight);
  mirror(c2, wait, NUM_PIXELS-oscilHeight);
} 

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
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
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
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
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

