
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>

#endif
  #include "IRremote.h"


  namespace trailer{
    
// pins 
    //LEDs Need to be PWM
#define led_closet  5  //parallel rails 0-36 down 37-54 up
#define led_bed     9  //first 9 parallel then bed runner then 9 leds for books
#define led_desk    6  //parallel rails 0-50?
#define led_kitchen 10 //parallel first 8 then the rest of 20 for kitchen 

    //inputs
#define pot_kitchen   A0
#define pot_books     A1
#define pot_R         A2
#define pot_G         A3
#define pot_B         A4
#define pot_masterBright  A5

  //Switches non-pwm
#define sw_topDown    2
#define sw_topUp      3
#define sw_side       4     //both 
#define sw_storage    7     //bed runner and bed crossmembers excluding books
#define sw_ollie      8     //just the 9 book lights
#define sw_door       11    //For now turns on bed runners
#define sw_kitchen    13
#define sw_book       12    //Rainbow madness

// How many NeoPixels are attached to the Arduino?
// 4M (60LEDs/M) = 240 pixels
#define numCloset       54
#define numBed          57
#define numDesk         45
#define numKitchen      18



Adafruit_NeoPixel pix_c = Adafruit_NeoPixel(numCloset, led_closet, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pix_d = Adafruit_NeoPixel(numBed, led_bed, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pix_b = Adafruit_NeoPixel(numDesk, led_desk, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pix_k = Adafruit_NeoPixel(numKitchen, led_kitchen, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel strip[] = {pix_c, pix_b, pix_d, pix_k}; // = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

//StripDef holds all the start and stop values for each strip
//each row of StripDef[] coresponds to onOff[] ie- onOff[2] turns on/off StripDef[2][]
//                  {strip#,start,end}
int stripDef[][3] = {{0,0,36}, //topDown
                    {0,37,54},//topUp
                    {2,0,45},//SideRails(desk)
                    {1,0,51}, //storage
                    {0,0,0}, //Rainbow Switch Not USED
                    {1,0,60}, //door (storage + books)
                    {3,0,18}, //kitchen
                    {1,52,60}}; //books


int onOff[] = {sw_topDown, sw_topUp, sw_side, sw_storage, sw_ollie, sw_door, sw_kitchen, sw_book};
int brightness[] = {pot_masterBright, pot_kitchen, pot_books};
int color[] = {pot_R, pot_G, pot_B};

bool show_topDown = 0; 
bool show_topUp =0; 
bool show_side =0; 
bool show_storage =0; 
bool show_book=0; 
bool show_door =0; 
bool show_kitchen=0; 
bool show_ollie=0; 

  }

using namespace trailer;

void setup() {
   // Start the receiver
     Serial.begin(9600);
   
    pinMode(pot_kitchen, INPUT);
    pinMode(pot_books, INPUT);
    pinMode(pot_R, INPUT);
    pinMode(pot_G , INPUT);
    pinMode(pot_B   , INPUT);
    pinMode(pot_masterBright    , INPUT);
    
    pinMode(sw_topDown, INPUT);
    pinMode(sw_topUp , INPUT);
    pinMode(sw_side  , INPUT);
    pinMode(sw_storage , INPUT);
    pinMode(sw_ollie  , INPUT);
    pinMode(sw_door   , INPUT);
    pinMode(sw_kitchen , INPUT);
    pinMode(sw_book  , INPUT);
  
  for (int i = 0; i<4; i++){
  strip[i].begin();
  strip[i].show(); // Initialize all pixels to 'off'    
  }
    
}


void loop() {
   double bright_val = translateToBright(pot_masterBright); 

   //first check to see if Rainbow mode is activated. If so pass to the rainbow chase method
    
    if( readSwitch(sw_ollie)) rainbowOn(12);
    else {
       
  //otherwise turn each light on and assign it the RGB value from the Controller potentiometers. 
    
    for(int i=0; i<7; i++){
      if(readSwitch(onOff[i])) 
        showStrip(strip[stripDef[i][0]], stripDef[i][1],stripDef[i][2],bright_val);
      else if(!readSwitch(onOff[i]))
        clearStrip(strip[stripDef[i][0]], stripDef[i][1],stripDef[i][2]); 
    }
    }
  
}


double translateToColor(int pinNum){
  //translates potentiometer signal into a percentage
  //currently reading 0 when turned all the way up; so i flipped the signal here
    double val = ((1023.0 - analogRead(pinNum))/1023.0) * 255.0;
    val = constrain(val, 0, 255);
  return val;
}

double translateToBright(int pinNum){
    double val = ((1023.0 - analogRead(pinNum))/1023.0);
    //val = constrain(val, 0, 1);
    //Serial.print("val " );
    //Serial.println( val); 
  return val ;
}
int readSwitch(int pin){
   return !digitalRead(pin); //voltage is opposite what I intended 
}

void showStrip(Adafruit_NeoPixel &pixels,int pixStart, int pixEnd, double bright){
   //bright = bright*255; 
    /*double r = map(analogRead(pot_R), 0, 1023, 0, 255);
    r = constrain(r, 0, 255);*/
    double r = translateToColor(pot_R);
    double g = translateToColor(pot_G);
    double b = translateToColor(pot_B);
   for(int i=pixStart;i<pixEnd;i++){
    pixels.setPixelColor(i, pixels.Color(r*bright, g*bright, b*bright)); 
    //Serial.println(r); 
   }
   pixels.show();
}

void clearStrip(Adafruit_NeoPixel &pixels,int pixStart, int pixEnd){ 
   for(int i=pixStart;i<pixEnd;i++){
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); 
   }
    pixels.show(); // send pixel color to the hardware
}

void rainbowOn(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(int k =0; k<7; k++){ //cycle through the strips
      if(!readSwitch(sw_ollie)) break;
      if(k==5)k=6; //skip the door switch so it elliminates flicker in rainbow mode 
      int stripNum = stripDef[k][0];
    for(i=stripDef[k][1]; i<stripDef[k][2]; i++) {
      strip[stripNum].setPixelColor(i, Wheel((i+j) & 255, stripNum ));
    }
    ///////Turn the strips on and of while in rainbow mode
    if(readSwitch(onOff[k])) strip[stripNum].show();
    //Fix boundary case when either door or storage switch is on but the other isn't (since they control the same strip)
    //else if(readSwitch(onOff[3])||readSwitch(onOff[5])) break;
    else {

      clearStrip(strip[stripNum], stripDef[k][1], stripDef[k][2]); 
    }
    delay(wait);
  }
  }
}
uint32_t Wheel(byte WheelPos, int k) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip[k].Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip[k].Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip[k].Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


