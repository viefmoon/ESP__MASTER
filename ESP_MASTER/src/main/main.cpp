#include <Arduino.h>
#include <HardwareSerial.h>
#include <Stream.h>
#include "DS3231/DS3231_Simple.h"
#include "main/main.h"
#include <modbus/modbus.h>
#include "macro.h"
#include <Wire.h>
#include "json/json.h"
#include <WiFi.h>
#include "time.h"

const char * ssid="REPLACE_WITH_YOUR_SSID";
const char * password="REPLACE_WITH_YOUR_PASSWORD";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 18000;   //Replace with your GMT offset (seconds)
const int   daylightOffset_sec = 0;  //Replace with your daylight offset (seconds)


DS3231_Simple Clock;
DateTime MyDateAndTime;

MODBUS Sensors;

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

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
  //printTime();
  uint8_t AlarmsFired = Clock.checkAlarms();
  if(AlarmsFired & 2)
  {
    MyDateAndTime = Clock.read();//post time
    if(MyDateAndTime.Minute % PostTime  == 00){
      printTime();
      Sprintln(F("Time to make measures"));
      //Si el tiempo se perdio, guardarlo de nuevo, se obtiene del gps
      if(MyDateAndTime.Year<20){
        Sprintln(F("Tiempo perdido, volviendo a setear el tiempo"));
        setTime();
      }
      time_to_String();//to json
      Sensors.makeMeasures();//adding all measures to json
      Sprintln(F("Posting data.."));
      bool postState = postData();
      if(postState){
        Sprintln(F("Post Success!"));
      }
      else{
        Sprintln(F("Post Error"));
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
  struct tm timeinfo_;
  if(getLocalTime(&timeinfo_)){//time obtained 
    DateTime MyTimestamp;
    MyTimestamp.Day    = timeinfo_.tm_mday;
    MyTimestamp.Month  = timeinfo_.tm_mon+1;
    MyTimestamp.Year   = timeinfo_.tm_year+1900; 
    MyTimestamp.Hour   = timeinfo_.tm_hour;
    MyTimestamp.Minute = timeinfo_.tm_min;
    MyTimestamp.Second = timeinfo_.tm_sec;
    Clock.write(MyTimestamp);
    Sprintln(F("Date updated correctly"));
  }
  else{
    Sprintln(F("Error obtaining date"));
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

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println("CONNECTED to WIFI");

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  setAlarm();
}

void loop() {

  float EC  = Sensors.ModBus_MakeCMD_EC(0x60,modbus_enum::MODBUS_CMD_READ);//EC
  float CO2 = Sensors.ModBus_MakeCMD_CO2(0x60,modbus_enum::MODBUS_CMD_READ);//EC
  float LV0 = Sensors.ModBus_MakeCMD_LV0(0x60,modbus_enum::MODBUS_CMD_READ);//EC
  float LV1 = Sensors.ModBus_MakeCMD_LV1(0x60,modbus_enum::MODBUS_CMD_READ);//EC
  checktime(); 
}
