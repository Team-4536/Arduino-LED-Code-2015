#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN 13 //Digital Output

int inputPin = 10;

int pixelNumber = 120; //How many pixels are in the strip?

int maxPixel = 48; // Top most pixel to end code on.

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelNumber, PIN, NEO_GRB + NEO_KHZ800);

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
  
  pinMode(inputPin, INPUT);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void mirrorHeightColor(int height) { // should take in value between 0 and 60 inclusive.
  
  for (int pixel = 0; pixel < (pixelNumber/2); pixel++) {
    
    if (pixel < height){
      
      strip.setPixelColor(pixel, 150, 150, 0);
      strip.setPixelColor((pixelNumber - pixel - 1), 150, 150, 0);
    }
    else if (pixel >= height && pixel <= maxPixel) {
      
      strip.setPixelColor(pixel, 20, 20, 20);
      strip.setPixelColor(pixelNumber - pixel - 1, 20, 20, 20);
    }
    else{
      
      strip.setPixelColor(pixel, 0, 0, 0); // white
      strip.setPixelColor((pixelNumber - pixel - 1), 0, 0, 0); // white
    }
  } 
}

void heightColor(int ledHeight) {
  
  for (int ledPixel = 0; ledPixel < pixelNumber; ledPixel++) {
    if (ledPixel < ledHeight) {
   
      strip.setPixelColor(ledPixel, 150, 150, 0); // Gold
    }
    else {
    
      strip.setPixelColor(ledPixel, 20, 20, 20); // Silver/White
    }
  }
}
  
void loop() {

  unsigned long sum = 0;     //unsigned long required instead of int because pulseIn returns an unsigned long
  unsigned long sampleSize = 20;  //samples over 20 2ms periods
  for(int i = 0; i < sampleSize; i++)
    sum+=pulseIn(inputPin, HIGH, 5000);    //adds the length of the most recent HIGH pulse received by pin D12 to the sum. If no pulse is received in 5000 microseconds, add nothing to sum
  unsigned long pwmValue = sum*255/(2000*sampleSize); //converts a time in microseconds to a PWM pulse, divides by sampleSize to obtain average value. The output pwm frequency is 500Hz, which is why a division by 2000 microseconds is required
  
  //number of pixel code
  if (pwmValue > 0)
    pwmValue *= (maxPixel / 48);
  
  Serial.println(pwmValue);   //outputs a new value roughly every 40ms if a signal is being received, and 100ms if no signal is being received
  
  mirrorHeightColor(pwmValue);
  
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

