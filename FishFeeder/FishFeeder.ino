/**
 * @licence - GPL
 * @author Ivo Vicente <ivolvicente@gmail.com>
 * @description Arduino Nano code for an automatic fish feeder
 * ----
 * DS3231 Library for RTC Control
 * Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
 * web: http://www.RinkyDinkElectronics.com/
 * @version 1.0.3
 * @date 21-09-2016
 */

#include <Servo.h>
#include <DS3231.h>

// RTC 
DS3231 rtc(SDA, SCL); // Init the DS3231 using the hardware interface

// FOOD Servo Vars
Servo foodServo;            // Create servo object to control a servo
const int SERVO_PIN = 9;     // Arduino PIN Number (PWD)
const int MAX_DEG = 45;      // Max Degree
const int DEG_DELAY = 30;    // Delay between pulses
const int DEG_INCREMENT = 4; // Degree to increment
int pos = 0;                // Save servo position

// PROJECT VARS
const boolean DEV_MODE = false;   // When Dev Mode is true, Serial comms are avaiable
String currTime ="";             // Save current time hh:mm
String lastFeedTime= "";         // Last time feeder work
int servoInPosition = 0;         // Servo position when loop function start
int i=0;                         // Aux var

// USER VARS
char *feedTime[] = {"10:00","16:30","22:00"}; // Fixed string array with feeding time. Format: HH:mm / 24h 
int feedTurn = 3;                // How many times servo make moviment 


/**
 * Arduino Setup Function
 */
void setup() { 
  // Initialize the rtc object
  rtc.begin();

  // Reset Servo - Sametimes the servo position is not 0 on Arduino boot
  servoInPosition = foodServo.read();
  if(servoInPosition > 0){    
    swl("Reset Food Servo");
    resetServoPosition(servoInPosition);
  }

  // Activate Serial for Debug Proposes only in Development mode
  if(DEV_MODE){
    Serial.begin(9600);
  }
}

/**
 * Arduino Main Loop Function
 */
void loop() {    

  // Get current time
  currTime = rtc.getTimeStr(FORMAT_SHORT);

  // Make a full cicle in feedTime array to check for rigth time to activate the servo
  // *sizeof allow to set any size array, without change this part of code
  for(i=0; i< sizeof(feedTime); i++){
    // Check if Current Time is Equal to feedTime in this array position
    // And currTime is Not Equal to lastFeedTime
    if( currTime == feedTime[i]  && currTime != lastFeedTime){
      swl("Its Feeding Time People");    
      startFeeding();
      // Set lasteFeedTime to currTime to be used as control variable
      lastFeedTime=currTime;      
    }
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
  foodServo.attach(SERVO_PIN);   

  // Make servo move
  while (turn < feedTurn){    
    for (pos = 0; pos <= MAX_DEG; pos += DEG_INCREMENT) {      
      foodServo.write(pos);      
      delay(DEG_DELAY);                    
    }
    for (pos = MAX_DEG; pos >= 0; pos -= DEG_INCREMENT) { 
      foodServo.write(pos);              
      delay(DEG_DELAY);                       

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
  foodServo.attach(SERVO_PIN); 

  // Need to loop to return servo position to zero
  for (pos = startPos; pos >= 0; pos--) { 
    foodServo.write(pos);              
    delay(DEG_DELAY);                       
  }     
  foodServo.detach();
}

/**
 * Communication Fuction to send data using Serial port
 */
void swl (String txt){
  if(DEV_MODE){
    Serial.println(txt);
  }
}









