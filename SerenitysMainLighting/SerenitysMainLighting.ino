
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>

#endif
  #include "IRremote.h"
  
// the data control pin
#define LEDstrip            6
#define reciever            11
#define motion_pin          3

// How many NeoPixels are attached to the Arduino?
// 4M (60LEDs/M) = 240 pixels
#define NUMPIXELS      240


IRrecv irrecv(reciever);
decode_results results;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LEDstrip, NEO_GRB + NEO_KHZ800);

void setup() {
  
 pixels.begin(); //initiallize adafruit library
   // Start the receiver
     Serial.begin(9600);
     irrecv.enableIRIn();

  pinMode(motion_pin, INPUT);    
   
   setRange(0,60,0,30,30);
   //delay(1000); 
   chaseRange(0,20,50,0,50,35);
}

void loop() {
  //cycle through every pixel starting with red blue and then green
  //use the remote to detect whether or not someone is trying to turn on the lights 
   
    if (irrecv.decode(&results)){ // have we received an IR signal?
    //Serial.println(results.value, HEX);
    translateIR(); 
    irrecv.resume(); // receive the next value
  }

   //bool tripped = digitalRead(motion_pin) == HIGH;
   //Serial.println("tripped? "); Serial.print(tripped); 

   //delay(10000); 
}

void chaseRange(int pixStart, int pixEnd, int cr, int cg, int cb, int holdTime){
  for(int i=pixStart;i<pixEnd;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(cr, cg, cb)); 
    pixels.show(); // send pixel color to the hardware
    delay(holdTime); // Delay for a period of time (in milliseconds) before setting next pixel
  }
} 

void chaseRGB(int pixStart, int pixEnd) {
    chaseRange(pixStart,pixEnd,140,0,0,25);
    chaseRange(pixStart,pixEnd,0,140,0,25);
    chaseRange(pixStart,pixEnd,0,0,140,25);
    chaseRange(pixStart,pixEnd,0,0,0,25); 
}

void setRange(int pixStart, int pixEnd, int cr, int cg, int cb){
  for (int i = pixStart; i<pixEnd; i++){
    pixels.setPixelColor(i, pixels.Color(cr,cg,cb)); //set the color
  }
  pixels.show(); //show all the pixels after they are set
}
//infrared reciever function

void translateIR() {
  switch(results.value)
  {
  case 0xFFA25D: Serial.println("POWER");  setRange(0,NUMPIXELS,0,0,0); break;
  case 0xFFE21D: Serial.println("FUNC/STOP"); break;
  case 0xFF629D: Serial.println("VOL+"); break;
  case 0xFF22DD: Serial.println("FAST BACK");    break;
  case 0xFF02FD: Serial.println("PAUSE");    break;
  case 0xFFC23D: Serial.println("FAST FORWARD");   break;
  case 0xFFE01F: Serial.println("DOWN");    break;
  case 0xFFA857: Serial.println("VOL-");    break;
  case 0xFF906F: Serial.println("UP");    break;
  case 0xFF9867: Serial.println("EQ");    break;
  case 0xFFB04F: Serial.println("ST/REPT");    break;
  case 0xFF6897: Serial.println("0");     setRange(0,100,0,30,30);  break;
  case 0xFF30CF: Serial.println("1");  setRange(0,100,30,0,30);  break;
  case 0xFF18E7: Serial.println("2");  theaterChaseRainbow(60);  break;
  case 0xFF7A85: Serial.println("3");  rainbowCycle(60);  break;
  case 0xFF10EF: Serial.println("4");    break;
  case 0xFF38C7: Serial.println("5");    break;
  case 0xFF5AA5: Serial.println("6");    break;
  case 0xFF42BD: Serial.println("7");    break;
  case 0xFF4AB5: Serial.println("8");    break;
  case 0xFF52AD: Serial.println("9");    break;
  case 0xFFFFFFFF: Serial.println(" REPEAT");break;  
  default: 
    Serial.println(" other button   ");
  }
  delay(500); // Do not get immediate repeat
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      pixels.show();

      delay(wait);

      for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}




