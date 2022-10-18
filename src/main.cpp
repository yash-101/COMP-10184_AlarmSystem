// 
//  Project 1: Alarm System
//
//  I Yash Chaudhary, 000820480 certify that this material is my original work. 
//  No other person's work has been used without due acknowledgement.
//  I have not made my work available to anyone else."
// 
//  @author Yash Chaudhary
//  @id 000820480
//
//  Completed : STAGE 5
//
#include <Arduino.h> 
 
// digital input pin definitions 
#define PIN_PIR D5
#define PIN_BUTTON D6 
#define LED_ON false  // to represent LED is ON
#define LED_OFF true  // to represent LED is OFF
#define ALARM_ENABLE 0  // to represent Alarm system is active
#define ALARM_DISABLED 1  // to represent alarm system is disabled
#define ALARM_COUNTDOWN 2 // to represent alarm system is in countdown waiting for input
#define ALARM_RINGING 3  // represent alarm is ringing
#define BUTTON_DELAY 200  // milliseconds to delay after button is pressed to avoid last state logic

int iAlarmMode; // Current status of Alarm

/**
 * this function blinks the built-in lights D1 mini.
 * it will track the button press during the blinking period, if pressed it will return int value representing alarm is disabled.
 * 
 * @param blinkPerSec how many times to blink light in one second period
 * @param totalSec total second the light should blink
 * 
 * @return integer value refering whether to disable or ring the alarm
 * */
int blinkLight(int blinkPerSec, int totalSec){
  
  // formula: 1000/(number of times to blink * 2)  [2 is for ON half the time and OFF half time in one cycle]
  int intervalForBlink = 500/blinkPerSec; 

  for(int i = 0; i < totalSec; i++){   // each iterartion of this loop is one second.
    for(int i = 0; i < blinkPerSec; i++){  // each iteration will blink the light once 
      // check button press
      if(digitalRead(PIN_BUTTON) == LOW){
        delay(BUTTON_DELAY);
        return ALARM_DISABLED;  // to disable alarm
      }
      digitalWrite(LED_BUILTIN, LED_OFF);
      delay(intervalForBlink);
      digitalWrite(LED_BUILTIN,LED_ON);
      delay(intervalForBlink);
    }
  } 
  return ALARM_RINGING;   // alarm will start ringing
}

void setup() { 
  // configure the USB serial monitor 
  Serial.begin(115200); 
 
  // configure the LED output 
  pinMode(LED_BUILTIN, OUTPUT); 
 
  // PIR sensor is an INPUT 
  pinMode(PIN_PIR, INPUT); 
 
  // Button is an INPUT 
  pinMode(PIN_BUTTON, INPUT_PULLUP); 

  // setting the LED off and alarm mode to enable
  digitalWrite(LED_BUILTIN,LED_OFF);
  iAlarmMode = ALARM_ENABLE;
} 
 

void loop() { 
  switch (iAlarmMode){
    case ALARM_ENABLE:   // alarm is enable check for motion detection by sensor
      bool bPIR;
      bPIR = digitalRead(PIN_PIR); 
      if(bPIR == true)
        iAlarmMode = ALARM_COUNTDOWN;  // found the motion, start the countdown
      break;
    case ALARM_COUNTDOWN:
      iAlarmMode = blinkLight(4,10);   // blink light for 10 sec with 4 blinks per sec
      if(iAlarmMode == ALARM_DISABLED){    // if btn was pressed during countdown, turn off the led
        digitalWrite(LED_BUILTIN,LED_OFF);
      }else {                              // alarm start ringing by turning led on
        digitalWrite(LED_BUILTIN,LED_ON);
      }
      break;
    case ALARM_DISABLED:             // alarm is diable chekc if btn press to enable the sensor
      if(digitalRead(PIN_BUTTON) == LOW){
        iAlarmMode = ALARM_ENABLE;
        delay(BUTTON_DELAY); // avoid double press
      }
      break;
    default:      //Alarm ringing cannot do nothing 
      break;
  }
}