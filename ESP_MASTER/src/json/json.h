#ifndef JSON_H
#define JSON_H

#include <ArduinoJson.h>

//Config
#define JSON_STATIC_RAM_SIZE 1536
#define DATETIME_NAME_JSON "s"
#define MEASURE_KEY_JSON "m"
#define DEVICES_KEY_JSON "d"
#define DEVICES_CODE_KEY_JSON "k"
//

#define DeviceCode "ES009"
#define ServerUrl "k"

extern StaticJsonDocument <JSON_STATIC_RAM_SIZE> jsonRoot;

void init_Json();
void clear_Json();
void Serialize_Json();

struct Add_To_Json
{
    template <typename T>
    bool _addMeasure(T &val, const __FlashStringHelper *code);
    template <typename T>
    bool _addAttribute(T &val, const __FlashStringHelper *code);

    bool addAttribute(bool &value, const __FlashStringHelper *code);
    bool addAttribute(float &value, const __FlashStringHelper *code);
    bool addAttribute(double &value, const __FlashStringHelper *code);
    bool addAttribute(signed char &value, const __FlashStringHelper *code);
    bool addAttribute(signed long &value, const __FlashStringHelper *code);
    bool addAttribute(signed int &value, const __FlashStringHelper *code);
    bool addAttribute(signed short &value, const __FlashStringHelper *code);
    bool addAttribute(unsigned char &value, const __FlashStringHelper *code);
    bool addAttribute(unsigned long &value, const __FlashStringHelper *code);
    bool addAttribute(unsigned int &value, const __FlashStringHelper *code);
    bool addAttribute(unsigned short &value, const __FlashStringHelper *code);
    bool addAttribute(const char *value, const __FlashStringHelper *code);
    bool addAttribute(char *value, const __FlashStringHelper *code);

    bool addMeasure(bool &value, const __FlashStringHelper *code);
    bool addMeasure(float &value, const __FlashStringHelper *code);
    bool addMeasure(double &value, const __FlashStringHelper *code);
    bool addMeasure(signed char &value, const __FlashStringHelper *code);
    bool addMeasure(signed long &value, const __FlashStringHelper *code);
    bool addMeasure(signed int &value, const __FlashStringHelper *code);
    bool addMeasure(signed short &value, const __FlashStringHelper *code);
    bool addMeasure(unsigned char &value, const __FlashStringHelper *code);
    bool addMeasure(unsigned long &value, const __FlashStringHelper *code);
    bool addMeasure(unsigned int &value, const __FlashStringHelper *code);
    bool addMeasure(unsigned short &value, const __FlashStringHelper *code);
    bool addMeasure(const char *value, const __FlashStringHelper *code);
    bool addMeasure(char *value, const __FlashStringHelper *code);
};

extern Add_To_Json DeviceMeasures;

#endif