# TinyHomeLighting
Just a simple Arduino Sketch for my Tiny Home Lighting system

# Version 2 - currently stable and running in my Tiny House
A bit more humble than the first version, this code was driven by the user experience (in this case me!). It became clear I didn't want to have a remote control that could get lost of run out of battery that controls the lights. Also the motion detectors was a bit much and begged the question, "every time I roll over do I really want the lights to turn on?". The answer is definitely not. Sometimes a simple switch is the best choice. 

- The trailer has 4 lighting strips (top lighting, side lighting, kitchen lighting, and underneath the bed)
- 8 switches control various sections of each lighing segment and allow me to turn on the lights I need within an arms length.
- rainbow mode turns any light that was previously on into a rolling rainbow!! 
- normal opperating mode uses the pots to select RGB levels using three pots, and one pot to change the brightness of the color. Simple and effective


# Version 1 - never completed due to physical design changes
So I need some light in my tiny home! But not just any lights, I want a nerdy LED system to reflect my weirdness. I may also use the Arduino that is driving the lights to make a crude security system once they are up and running. 

Feature List: 
  - Place a magnetic sensor to determine when the door is being opened
  - Use a photocell to determine when it is light and dard outside to give the appropriate color lighting upon entering the trailer for the "main" lights 
  - set asside a little strip of lights for a binary clock
  - add a thermometer and use the temperature to determine the color of some lights near the door
   
   
  _More Outlandishy features_
  - add a tiny speaker that chimes within 30 sec if I don't hit a hidden button just inside the door
  - Motion sensors for each corner that then lights up a small LED in that corner to capture movement outside in a crude way


# Repositories
- Adafruit NeoPixel : Drives the WS2812s LED strip 

# WISHLIST 
 1) One feature I've always wanted to add is an alarm clock that fades the lights on with the sunrise outside. I have a few Ideas about how to do this; lighting sensor that detects the outside light, or a clock system that keeps track of the date and sunrise times. This is primarily held up by time. I have all the pieces sitting here. 
