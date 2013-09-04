/* MiniLogR - 
 * cheap and easy data logger, with record, play, and clear functions
 * Saves to non-volataile EEPROM memory and maintains memory when reset or turned off
 * Edit eepromMem to set amount of available eeprom 
 * Edit timing to set how often data is recorded - higher numbers = less freq.
 
 The circuit:
 
 * Record button connected between digital pin 5 and ground 
 * Play button connected between digital pin 4 and ground 
 * Erase button connected between digital pin 3 and ground 
 * RED LED to digital pin 11
 * GREEN LED to digital pin 10
 * YELLOW LED to digital pin 9  
 * Uses PWM for output control so no resistors needed 
 * analog sensor connected to analog pin 0, 5V, and ground through 10K resistor
 
 ********Button behavior****************
 * From 'ready' state (green LED), press 'record' to save data to eeprom (blinking red)
 * From 'record' state, press 'record' again to return to ready (green LED)
 * From 'ready' state, press 'play' to go to play (blue LED) 
 * From 'play', press 'play' again to return to ready (green LED)
 * From 'ready' state, press 'erase' to go to 'ready to erase' state (all LEDs on)
 * From 'ready to erase' state, press 'erase' to clear EEPROM
 * From 'ready to erase' state, press any other button to return to ready without clearing
 ****************************************
 *
 * created by Matt Ratto 
 * last modified July 12, 2010
 * for use in Self Sensor, critical making experience
 * Critical Making lab - http://www.criticalmaking.com 
 *
 */


#include <EEPROM.h>
#include <Button.h> //replace with debounce routine so no external lib required

// -------USER CHANGE VARIABLES BELOW----------------------
const int eepromMem = 1024; //set depending on 168 (512) or 328 (1024) 

int timing=500; //use to set how often data is saved - approx. timing numbers here

int lowVal = 30; //use to set lowest expected incoming value from sensor 
int highVal = 200; //use to set highest expected incoming value from sensor 
//---------END SECTION--------------------------------------

int state = 0; 
/* 0 = ready
 1 = recording 
 2 = playing
 3 = ready to clear 
 */

Button buttonRecord = Button(5,BUTTON_PULLUP_INTERNAL);
Button buttonPlay = Button(4,BUTTON_PULLUP_INTERNAL);
Button buttonErase = Button(3,BUTTON_PULLUP_INTERNAL);

int loopNumber = 0; // for timing routine

int statusLEDred=11;
int statusLEDgreen=10; 
int statusLEDblue=9; 

int sensorPin=0; 
int address=0; //for EEPROM read and write

void setup(){  
  Serial.begin(9600); 
  for (int i=eepromMem; i>0; i--) {
    int val = EEPROM.read(i);
    //Serial.println(val);  
    if (val==255) { //look for address without data
      address=i; //set address to location  
      //Serial.println(address); 
    }
  }
  //Serial.print("final address");
  //Serial.println(address); 
}

void loop(){ 


  switch (state) {
  case 0: //waiting for input 
    colorLED(0,50,0); //set LED to Green
    delay(50); 
    

    if (buttonRecord.uniquePress()){
      state = 1; //Record Mode
    }
    if (buttonPlay.uniquePress()){
      state = 2; //Play Mode
    }
    if (buttonErase.uniquePress()){
      state = 3; //Erase Mode
    }
    break; 
  case 1: //recording
    {
      //Serial.println("Recording mode"); 
      colorLED(50,0,0); //set LED to RED
      if (buttonRecord.uniquePress()){
        state = 0;
      } 
      if (loopNumber==timing){
        loopNumber=0;
        colorLED(0,0,0); //set LED OFF
        int sensorInput = (analogRead(0)); 
        int mappedInput = map(sensorInput, lowVal, highVal, 0, 254);  
        EEPROM.write(address,mappedInput); 
        //Serial.print("Wrote"); 
        Serial.println(mappedInput); 
        //Serial.print(" to address "); 
        //Serial.println(address); 
        delay(100); 
        if (address==eepromMem) {
          state = 0; 
        }
        address=address+1; 
      }
      else loopNumber=loopNumber+1; 
    }
    //Serial.println(loopNumber); 
    break; 

  case 2: //playing back 
    //Serial.println("Playback mode"); 
    colorLED(0,0,50); //set LED to blue
    if (buttonPlay.uniquePress()) {
      state = 0;
    } 
      for (int i=0;i<address;i++){
        int val = EEPROM.read(i); 
        Serial.println(val);
        delay(10);
        state = 0;    
      } 
      break; 

    case 3: //eeprom full 
      colorLED(50,50,50); //Turn them all on
     // Serial.println("Ready to Clear or Full mode"); 
      if (buttonErase.uniquePress()) {
        state = 0;
        clearEEPROM();  
      }
      if (buttonPlay.uniquePress()) {
        state = 0; 
      }
      if (buttonRecord.uniquePress()) {
        state = 0; 
      }
      break; 
    }
  }

void colorLED(int RED,int GREEN,int BLUE) {
  analogWrite(statusLEDred, RED); 
  analogWrite(statusLEDgreen, GREEN);
  analogWrite(statusLEDblue, BLUE); 
}

void clearEEPROM() //clear EEPROM routine 
{
  colorLED(10,10,10); //set LED to ?
  // write a 255 to all 1024 bytes of the EEPROM 
  //- change this to 512 if ATmega 168
  for (int i = 0; i < eepromMem; i++){
    EEPROM.write(i, 255); 
    address = 0; //set address to 0
  }
  //Serial.println("EEPROM cleared"); 
} 

