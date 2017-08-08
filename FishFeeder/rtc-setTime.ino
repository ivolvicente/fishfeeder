#include <DS3231.h>

// RTC
DS3231 rtc(SDA, SCL); // Init the DS3231 using the hardware interface

void setup()
{
  rtc.begin();
  
  // Set Time breaking time : into , - ex: 12:15:00 => 12,15,00
  // Only need to run once to set rtc time
  //rtc.setTime(12,15,00);
  
  Serial.begin(9600);
}

void loop()
{

  String txt = rtc.getTimeStr();
  Serial.println(txt);

  delay(5000);
}
