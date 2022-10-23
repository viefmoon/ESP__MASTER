#include <Arduino.h>
#include <HardwareSerial.h>
#include <SPI.h>
#include <Stream.h>
#include <Adafruit_FONA.h>
#include "DS3231/DS3231_Simple.h"
#include "main/main.h"
#include <modbus/modbus.h>
#include "macro.h"
#include <Wire.h>
#include <Adafruit_INA219/Adafruit_INA219.h>

DS3231_Simple Clock;
DateTime MyDateAndTime;


MODBUS Sensors;

void time_to_String(){
  char arrayCharTime[25];
  if((MyDateAndTime.Minute<10)&&(MyDateAndTime.Hour<10)){
    sprintf(arrayCharTime, "20%d-%d-%dT0%d:0%d:00", MyDateAndTime.Year, MyDateAndTime.Month, MyDateAndTime.Day,MyDateAndTime.Hour,MyDateAndTime.Minute);
  }
  else if(MyDateAndTime.Hour<10){
    sprintf(arrayCharTime, "20%d-%d-%dT0%d:%d:00", MyDateAndTime.Year, MyDateAndTime.Month, MyDateAndTime.Day,MyDateAndTime.Hour,MyDateAndTime.Minute);
  }
  else if(MyDateAndTime.Minute<10){
    sprintf(arrayCharTime, "20%d-%d-%dT%d:0%d:00", MyDateAndTime.Year, MyDateAndTime.Month, MyDateAndTime.Day,MyDateAndTime.Hour,MyDateAndTime.Minute);
  }
  else{
  sprintf(arrayCharTime, "20%d-%d-%dT%d:%d:00", MyDateAndTime.Year, MyDateAndTime.Month, MyDateAndTime.Day,MyDateAndTime.Hour,MyDateAndTime.Minute);
  }
  DeviceMeasures.addAttribute(arrayCharTime, F(RTC_CODE_MEASURE));
}


bool postData(){
  uint8_t postAttempts = 0;
  while (postAttempts < 5) {

  }
  return false;
}

void checktime(){
  printTime();
  uint8_t AlarmsFired = Clock.checkAlarms();
  if(AlarmsFired & 2)
  {
    MyDateAndTime = Clock.read();
    if(MyDateAndTime.Minute % PostTime  == 00){
      printTime();
      Sprintln(F("Time to make measures"));
      //Si el tiempo se perdio, guardarlo de nuevo, se obtiene del gps
      if(MyDateAndTime.Year<20){
        Sprintln(F("Tiempo perdido, volviendo a setear el tiempo"));
        setTime();
      }
      init_Json();
      time_to_String();
      CheckPower();
      GPRS_MODULE.updateGps();
      Sensors.makeMeasures();
      Sprintln(F("Posting data.."));
      bool postState = postData();
      if(postState){
        Sprintln(F("Post Success!"));
        BlinkyPost();
      }
      else{
        Sprintln(F("Post Error, waiting next measures"));
      }
    }
  }
}

void setAlarm(){
  Clock.begin();
  Clock.disableAlarms();
  Clock.setAlarm(DS3231_Simple::ALARM_EVERY_MINUTE); 
}


void setTime(){
  Sprintln(F("Setting time..  "));
  uint8_t year =0, month = 0, day = 0, hour = 0, minute = 0, second = 0; 

  bool state = GPRS_MODULE.getTime(&year, &month, &day, &hour, &minute, &second);
  if (state){  
    DateTime MyTimestamp;
    MyTimestamp.Day    = day;
    MyTimestamp.Month  = month;
    MyTimestamp.Year   = year; 
    MyTimestamp.Hour   = hour;
    MyTimestamp.Minute = minute;
    MyTimestamp.Second = second;
    Clock.write(MyTimestamp);
    Sprintln(F("Date updated correctly"));

  }
  else{
    Sprintln(F("Error obtaining date"));
    GPRS_MODULE.RESET_SIM808();
    delay(60000);
  }
}

void printTime(){
  DateTime MyDateAndTime;
  MyDateAndTime = Clock.read();

  Sprint(F("Hour: "));  Sprint(MyDateAndTime.Hour); Sprint(F("  "));
  Sprint(F("Minute: "));  Sprint(MyDateAndTime.Minute); Sprint(F("  "));
  Sprint(F("Second: "));  Sprint(MyDateAndTime.Second); Sprint(F("  "));
  Sprint(F("Year: "));  Sprint(MyDateAndTime.Year); Sprint(F("  "));
  Sprint(F("Month: "));  Sprint(MyDateAndTime.Month); Sprint(F("  "));
  Sprint(F("Day: "));  Sprint(MyDateAndTime.Day); Sprintln(F("  "));
}

void setup() {

  Serial.begin(SERIAL_BAUDRATE);

  setAlarm();
}

void loop() {
  checktime(); 
  //CheckPow();
}
