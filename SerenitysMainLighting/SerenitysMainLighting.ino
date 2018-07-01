
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// the data control pin
#define PIN            6

// How many NeoPixels are attached to the Arduino?
// 4M (60LEDs/M) = 240 pixels
#define NUMPIXELS      24



Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  
 pixels.begin(); //initiallize adafruit library
   checkPixels(30,0,0); 
 
}

void loop() {
  //cycle through every pixel starting with red blue and then green
       
}

void checkPixels(int cR, int cG, int cB){
  for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(cR, cG, cB)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(25); // Delay for a period of time (in milliseconds).

  }
  
} 

void chasePixels(int len, int cR, int cG, int cB) {
    checkPixels(140,0,0); 
    checkPixels(0,140,0); 
    checkPixels(0,0,140);
    checkPixels(0,0,0);  
}


