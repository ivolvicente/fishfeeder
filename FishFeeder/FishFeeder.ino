/**
 * @licence - GPL
 * @author Ivo Vicente <ivolvicente@gmail.com>
 * @description Arduino Nano code for an automatic fish feeder
 * ----
 * DS3231 Library for RTC Control
 * Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
 * web: http://www.RinkyDinkElectronics.com/ 
 */

#include <Servo.h>
#include <DS3231.h>

// RTC 
DS3231 rtc(SDA, SCL); // Init the DS3231 using the hardware interface
// END: RTC

// FOOD Servo Vars
Servo foodServo;            // Create servo object to control a servo
const int servoPin = 9;     // Arduino PIN Number (PWD)
const int maxDeg = 45;      // Max Degree
const int degDelay = 30;    // Delay between pulses
const int degIncrement = 4; // Degree to increment
int pos;                    // Save servo position
// END: FOOD Servo Vars

// PROJECT VARS
const boolean devMode = false;   // When Dev Mode is true, Serial comms are avaiable
String currTime;                 // Save current time hh:mm
String currDate;                 // Save current date aaaa.mm.dd 
String prevDate = "2000.01.01";  // default for 1st time, then setted to last feed date
int servoInPosition;             // Servo position when loop function start
String feedTime = "22:00";       // Format hh:mm/24h ex: 16:09 
int feedTurn = 3;                // How many times servo make moviment

/**
 * Arduino Setup Function
 */
void setup() { 
  // Initialize the rtc object
  rtc.begin();

  // Serial for Debug Proposes only in Development mode
  if(devMode){
    Serial.begin(9600);
  }
}

/**
 * Arduino Main Loop Function
 */
void loop() {  
  // Reset Servo - Sametimes the servo position is not 0 on Arduino start
  servoInPosition = foodServo.read();
  if(servoInPosition > 0){    
    swl("Reset Food Servo");
    resetServoPosition(servoInPosition);
  }

  // Get current time
  currTime = rtc.getTimeStr(FORMAT_SHORT);
  // Get current date
  currDate = rtc.getDateStr(FORMAT_LONG,FORMAT_BIGENDIAN);

  // Check for conditions to deploy food
  if(feedTime == currTime && prevDate != currDate){
    swl("Its Feeding Time People");
    startFeeding();
    // Set prevDate to currDate to prevent this IF to be called again in same feedTime
    prevDate=currDate;
  }

  // Delay for next turn - 10 seconds  
  delay(10000);
}

/**
 * Define work for Servo, to start and stop is job
 */
void startFeeding(){ 
  int turn = 0; 
  // Attach servo for power to start run in servo
  foodServo.attach(servoPin);   

  // Make servo move
  while (turn < feedTurn){    
    for (pos = 0; pos <= maxDeg; pos += degIncrement) {      
      foodServo.write(pos);      
      delay(degDelay);                    
    }
    for (pos = maxDeg; pos >= 0; pos -= degIncrement) { 
      foodServo.write(pos);              
      delay(degDelay);                       

    }      
    turn++;
  }  
  // Detach servo to stop using power
  foodServo.detach();       
}

/**
 * Servo Position reset
 */
void resetServoPosition(int startPos){
  foodServo.attach(servoPin); 

  // Need to loop to return servo position to zero
  for (pos = startPos; pos >= 0; pos--) { 
    foodServo.write(pos);              
    delay(degDelay);                       
  }     
  foodServo.detach();
}

/**
 * Communication Fuction to send data using Serial port
 */
void swl (String txt){
  if(devMode){
    Serial.println(txt);
  }
}









