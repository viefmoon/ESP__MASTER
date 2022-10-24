#include "json.h"

//static JSON::_JsonMeasureRegister DeviceMeasures;
StaticJsonDocument<JSON_STATIC_RAM_SIZE> jsonRoot;

Add_To_Json DeviceMeasures;

JsonArray measure_array;
JsonArray devices;
JsonObject device;

template <typename T>
bool Add_To_Json::_addMeasure(T &val, const __FlashStringHelper *code)
{
    // Serial.print("val: ");
    // Serial.println(val);
    // Serial.print("code: ");
    // Serial.println(code);
    JsonObject aux = measure_array.createNestedObject();
    aux["v"] = val;
    aux["m"] = code;
    return true;
}

template <typename T>
bool Add_To_Json::_addAttribute(T &val, const __FlashStringHelper *code)
{
    jsonRoot[code] = val;
    return true;
}

bool Add_To_Json::addAttribute(bool &value, const __FlashStringHelper *code) { return _addAttribute(value, code); }
bool Add_To_Json::addAttribute(float &value, const __FlashStringHelper *code) { return _addAttribute(value, code); }
bool Add_To_Json::addAttribute(double &value, const __FlashStringHelper *code) { return _addAttribute(value, code); }
bool Add_To_Json::addAttribute(signed char &value, const __FlashStringHelper *code) { return _addAttribute(value, code); }
bool Add_To_Json::addAttribute(signed long &value, const __FlashStringHelper *code) { return _addAttribute(value, code); }
bool Add_To_Json::addAttribute(signed int &value, const __FlashStringHelper *code) { return _addAttribute(value, code); }
bool Add_To_Json::addAttribute(signed short &value, const __FlashStringHelper *code) { return _addAttribute(value, code); }
bool Add_To_Json::addAttribute(unsigned char &value, const __FlashStringHelper *code) { return _addAttribute(value, code); }
bool Add_To_Json::addAttribute(unsigned long &value, const __FlashStringHelper *code) { return _addAttribute(value, code); }
bool Add_To_Json::addAttribute(unsigned int &value, const __FlashStringHelper *code) { return _addAttribute(value, code); }
bool Add_To_Json::addAttribute(unsigned short &value, const __FlashStringHelper *code) { return _addAttribute(value, code); }
bool Add_To_Json::addAttribute(const char *value, const __FlashStringHelper *code) { return _addAttribute(value, code); }
bool Add_To_Json::addAttribute(char *value, const __FlashStringHelper *code) { return _addAttribute(value, code); }

bool Add_To_Json::addMeasure(bool &value, const __FlashStringHelper *code) { return _addMeasure(value, code); }
bool Add_To_Json::addMeasure(float &value, const __FlashStringHelper *code) { return _addMeasure(value, code); }
bool Add_To_Json::addMeasure(double &value, const __FlashStringHelper *code) { return _addMeasure(value, code); }
bool Add_To_Json::addMeasure(signed char &value, const __FlashStringHelper *code) { return _addMeasure(value, code); }
bool Add_To_Json::addMeasure(signed long &value, const __FlashStringHelper *code) { return _addMeasure(value, code); }
bool Add_To_Json::addMeasure(signed int &value, const __FlashStringHelper *code) { return _addMeasure(value, code); }
bool Add_To_Json::addMeasure(signed short &value, const __FlashStringHelper *code) { return _addMeasure(value, code); }
bool Add_To_Json::addMeasure(unsigned char &value, const __FlashStringHelper *code) { return _addMeasure(value, code); }
bool Add_To_Json::addMeasure(unsigned long &value, const __FlashStringHelper *code) { return _addMeasure(value, code); }
bool Add_To_Json::addMeasure(unsigned int &value, const __FlashStringHelper *code) { return _addMeasure(value, code); }
bool Add_To_Json::addMeasure(unsigned short &value, const __FlashStringHelper *code) { return _addMeasure(value, code); }
bool Add_To_Json::addMeasure(const char *value, const __FlashStringHelper *code) { return _addMeasure(value, code); }
bool Add_To_Json::addMeasure(char *value, const __FlashStringHelper *code) { return _addMeasure(value, code); }

void init_Json()
{
    jsonRoot.clear();
    devices = jsonRoot.createNestedArray(F(DEVICES_KEY_JSON));
    device = devices.createNestedObject();

    device[DEVICES_CODE_KEY_JSON] = DeviceCode;
    measure_array = device.createNestedArray(F(MEASURE_KEY_JSON));
}

void Serialize_Json()
{
    serializeJson(jsonRoot, Serial);
}
