#ifndef MAIN_H
#define MAIN_H

#define SERIAL_BAUDRATE 19200 // real serial * 2
#define RTC_CODE_MEASURE "s"

#define PostTime    2
#define pinPostLed  24
#define pinRs485    6

extern void checktime();
extern void setAlarm();
extern void printTime();
extern void setTime();
extern bool PostData();
extern void CheckPower();
extern void CheckPow(); //without json add
extern void BlinkyPost();

#endif