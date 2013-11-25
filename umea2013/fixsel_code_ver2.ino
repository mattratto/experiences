/* 
fixsel code
version 2
Matt Ratto 
http://www.github.com/mattratto/experiences/ (look under Umea2013)
with liberal borrowings from 'Touch' code by David Mellis 
*/

#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

#define LEDPIN 3 //neopixel signal pin
#define TOUCHPIN 0 //but it is declared as PB0 in the routine as well 

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, LEDPIN, NEO_GRB + NEO_KHZ800);

int calibration = 0;
int previous;
//int previous;
//uint16_t i, 
int j = 1;
int red[216];
int green[216];
int blue[216];

void setup() {
    
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  for (int i = 0; i < 8; i++) {
    calibration += chargeTime(PB0);
    delay(20);
  }
  calibration = (calibration + 4) / 8;
  
  //code here to set color array web safe color
   for (int i=0; i<215; i++){
   for (int r = 0; r < 255; r += 51) {
            for (int g = 0; g < 255; g += 51) {
                for (int b = 0; b < 255; b += 51) {
                    red[i]=r;
                    green[i]=g;
                    blue[i]=b; 
                }
            }
        }
   }
   int colorRead = EEPROM.read(0);  //EEPROM read 
   if (colorRead >-1 && colorRead<216) { //check to make sure color has been stored
     j=colorRead; 
     strip.setPixelColor(0, red[j],green[j],blue[j]);
     strip.show();
   }else j=0;
}

void loop() {

int n = chargeTime(PB0);
    
    if (n > calibration) { //cap touched
    j=j+1;    
    strip.setPixelColor(0, red[j],green[j],blue[j]);
    strip.show();
    EEPROM.write(0, j);
    }
    //below if should trigger once after n drops below calibration - to reduce number of eeprom writes
    if (previous >= calibration && n < calibration){     
    EEPROM.write(0, j);
    }
    
    previous = n;
    delay(20);
    
}

/*
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
*/

byte chargeTime(byte pin)
{
  byte mask = (1 << pin);
  byte i;
  DDRB &= ~mask; // input
  PORTB |= mask; // pull-up on
  for (i = 0; i < 16; i++) {
    if (PINB & mask) break;
  }
  PORTB &= ~mask; // pull-up off
  DDRB |= mask; // discharge
  return i;
}
