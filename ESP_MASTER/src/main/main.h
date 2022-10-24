#ifndef MAIN_H
#define MAIN_H

#define SERIAL_BAUDRATE 9600 
#define RTC_CODE_MEASURE "s"

#define PostTime    1

extern void checktime();
extern void setAlarm();
extern void printTime();
extern void setTime();
extern bool PostData();

#endif