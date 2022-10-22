#ifndef MODBUS_H
#define MODBUS_H

#include <stdint.h>
#include <HardwareSerial.h>

//0x01 address available for wind speed sensor (4800 bauds) (version 1, cant change baud rate and address)
//0x02 to 0x04 address available for wind speed sensor (version 2)
//0x05 to 0x06 addresses available for wind direction (version 1)
//0x07 to 0x08 addresses available for wind direction (version 2)
//0x09 to 0x0B addresses available for rain sensor 
//0x0C to 0x0F addresses available for rain volume sensor (pluviometer)
//0x10 to 0x1F addresses available for soil sensor (EC,Humidity,temperature)
//0x20 to 0x2F addresses available for temperature, humidity, pressure sensor (WSY-301) 
//0x30 to 0x3F addresses available for PH sensor (AgCi version)
//0x40 to 0x4F addresses available for PH sensor (Electrode Version)
//0x50 to 0x5F addresses available for NPK sensor
 
//0x70 to 0x7F addresses available for UV radiation sensor
//0x80 to 0x8F addresses available for PAR radiation sensor
//0x90 to 0x9F addresses available for Total solar radiation
//0xA0 to 0xAF addresses available for Leaf sensor (Temperature, leaf humidity)

#define MODBUS_CMD_READ_BYTE 0x03

#define RX_LENGHT_WITHOUT_DATA 5
#define MODBUS_TX_BUFFER_LENGHT 8

#define MaxAttempts 6

#define MODBUS_SERIAL Serial2
#define MODBUS_SERIAL_BAUDRATE 19200 //real baud rate*2

////swap bytes (measurements) 
#define PH_BYTE_LENGHT      0x0300  //PH sensor (AgCi version) version
#define PH_EL_BYTE_LENGHT   0x0100  //PH sensor electrode version
#define LEAF_BYTE_LENGHT    0x0200  //LEAF SENSO
#define SOIL_BYTE_LENGHT    0x0300  //ELECTROCONDUCTIVY HUMIDITY AND TEMPERATURE
#define THP_BYTE_LENGHT     0x0300  //TEMPERATURE HUMIDITY AND PRESSURE (WSY-301)
#define NPK_BYTE_LENGHT     0x0300  //NITROGEN, phosphorus, POTASSIUM
#define WP_BYTE_LENGHT      0x0100  //Water potential sensor (Pressure sensor on tensiometer)
#define RT_BYTE_LENGHT      0x0300  //RADIATION SENSORS
#define W_BYTE_LENGHT       0x0100  //RAIN SENSOR, PCB VERSION
#define W2_BYTE_LENGHT      0x0100  //RAIN SENSOR, PLUVIOMETER VERSION (OWN VERSION)
#define WS_BYTE_LENGHT      0x0100  //WIND SPEED SENSOR
#define WS2_BYTE_LENGHT     0x0100  //WIND SPEED V2 SENSOR
#define WD_BYTE_LENGHT      0x0100  //WIND DIRECTION SENSOR
#define WD2_BYTE_LENGHT     0x0100  //WIND DIRECTION SENSOR
#define UV_BYTE_LENGHT      0x0100  //UV RADIARTION SENSOR
#define PAR_BYTE_LENGHT     0x0100  //PAR RADIARTION SENSOR
#define TOTALR_BYTE_LENGHT  0x0100  //TOTAL RADIARTION SENSOR

//Star address
#define PH_START_ADDRESS    0x0000
#define PH_EL_START_ADDRESS 0x0000
#define LEAF_START_ADDRESS  0x0000
#define SOIL_START_ADDRESS  0x0000
#define THP_START_ADDRESS   0x0000
#define NPK_START_ADDRESS   0x1E00
#define WP_START_ADDRESS    0x0000
#define RT_START_ADDRESS    0x0000
#define W_START_ADDRESS     0x0000
#define W2_START_ADDRESS    0x0000
#define WS_START_ADDRESS    0x0000
#define WS2_START_ADDRESS   0x0000
#define WD_START_ADDRESS    0x0000
#define WD2_START_ADDRESS   0x0000
#define UV_START_ADDRESS    0x0000
#define PAR_START_ADDRESS   0x0000
#define TOTALR_START_ADDRESS 0x0000

extern uint8_t sensorAddresses[];

extern uint8_t phSensorCounter;
extern uint8_t phElSensorCounter;
extern uint8_t leafSensorCounter;
extern uint8_t soilSensorCounter;
extern uint8_t thpSensorCounter;
extern uint8_t npkSensorCounter;
extern uint8_t wpSensorCounter;
extern uint8_t rtSensorCounter;
extern uint8_t wSensorCounter;
extern uint8_t w2SensorCounter;
extern uint8_t wsSensorCounter;
extern uint8_t ws2SensorCounter;
extern uint8_t wdSensorCounter;
extern uint8_t wd2SensorCounter;
extern uint8_t uvSensorCounter;
extern uint8_t parSensorCounter;
extern uint8_t totalrSensorCounter;

namespace modbus_structs
{
    //Structs to save cmd
    typedef struct
    {
        uint8_t sensorId;
        uint8_t tx_functionCode;
        uint16_t registerStartAddress;
        uint16_t registerLength;
        uint16_t tx_crc;
    } Modbus_tx_frame;
    //*******************************************

    //Structs to save raw and values from sensors
    typedef struct
    {
        uint16_t ph;
        uint16_t ph_simpl;
        uint16_t ph_temp;
    } rawPH;

    typedef struct
    {
        uint16_t ph_el;
    } rawPHEl;

    typedef struct
    {
        uint16_t leaf_humidity;
        uint16_t leaf_temperature;
    } rawLeaf;

    typedef struct
    {
        uint16_t moisture;
        uint16_t temperature;
        uint16_t ec;
    } rawSoil;

    typedef struct
    {
        uint16_t temperature;
        uint16_t humidity;
        uint16_t pressure;
    } rawThp;

    typedef struct
    {
        uint16_t nitrogen;
        uint16_t phosphorus;
        uint16_t potassium;
    } rawNpk;

    typedef struct
    {
        int16_t water_potential;
    } rawWp;


    typedef struct
    {
        uint16_t par;
        uint16_t total_r;
        uint16_t uv;
    } rawRt;

    typedef struct
    {
        uint16_t water;
    } rawW;

    typedef struct
    {
        uint16_t water2;
    } rawW2;

    
    typedef struct
    {
        uint16_t wind_speed;
    } rawWs;

    typedef struct
    {
        uint16_t wind_speed2;
    } rawWs2;

    typedef struct
    {
        uint16_t wind_direction;
    } rawWd;

    typedef struct
    {
        uint16_t wind_direction2;
    } rawWd2;

    typedef struct
    {
        uint16_t uv_radiation;
    } rawUv;

    typedef struct
    {
        uint16_t par_radiation;
    } rawPar;

    typedef struct
    {
        uint16_t total_radiation;
    } rawTotalr;
    //*******************************************

    //Structs to save measures
    typedef struct
    {
        float ph;
        float ph_simpl;
        float ph_temp;
    } pHSensorMeasure;

    typedef struct
    {
        float ph_el;
    } pHElSensorMeasure;

    typedef struct
    {
        float leaf_humidity;
        float leaf_temperature;
    } LeafSensorMeasure;

    typedef struct 
    {
        float humidity;
        float temperature;
        float ec;
    } SoilSensorMeasure;

    typedef struct
    {
        float temperature;
        float humidity;
        float pressure;
    } ThpSensorMeasure;

    typedef struct
    {
        float nitrogen;
        float phosphorus;
        float potassium;
    } NpkSensorMeasure;

    
    typedef struct
    {
        float water_potential;
    } WpSensorMeasure;

    typedef struct
    {
        float par;
        float total_r;
        float uv;
    } RtSensorMeasure;

    typedef struct
    {
        float water;
    } WSensorMeasure;

    typedef struct
    {
        float water2;
    } W2SensorMeasure;

    typedef struct
    {
        float wind_speed;
    } WsSensorMeasure;

    typedef struct
    {
        float wind_speed2;
    } Ws2SensorMeasure;

    typedef struct
    {
        float wind_direction;
    } WdSensorMeasure;

    typedef struct
    {
        float wind_direction2;
    } Wd2SensorMeasure;

    typedef struct
    {
        float uv_radiation;
    } UvSensorMeasure;

    typedef struct
    {
        float par_radiation;
    } ParSensorMeasure;

    typedef struct
    {
        float total_radiation;
    } TotalrSensorMeasure;

    //*******************************************
} //End struct namespace

namespace modbus_enum
{
    //Enum 
    typedef enum
    {
        MODBUS_SENSOR_PH,
        MODBUS_SENSOR_PH_EL,
        MODBUS_SENSOR_LEAF,
        MODBUS_SENSOR_SOIL,
        MODBUS_SENSOR_THP,
        MODBUS_SENSOR_NPK,
        MODBUS_SENSOR_WP,
        MODBUS_SENSOR_RT,
        MODBUS_SENSOR_W,
        MODBUS_SENSOR_W2,
        MODBUS_SENSOR_WS,
        MODBUS_SENSOR_WS2,
        MODBUS_SENSOR_WD,
        MODBUS_SENSOR_WD2,
        MODBUS_SENSOR_UV,
        MODBUS_SENSOR_PAR,
        MODBUS_SENSOR_TOTALR
    } Modbus_SensorTypes;

    typedef enum
    {
        MODBUS_CMD_READ = 3
    } Modbus_SensorsCMD;
    //*******************************************
}//end enum namespace

class MODBUS
{
    public:
    void swap_bytes(uint16_t *);

    modbus_structs::pHSensorMeasure buffer_to_ph(uint8_t *);

    modbus_structs::pHElSensorMeasure buffer_to_ph_el(uint8_t *);

    modbus_structs::LeafSensorMeasure buffer_to_leaf(uint8_t *);

    modbus_structs::SoilSensorMeasure buffer_to_soil(uint8_t *);

    modbus_structs::ThpSensorMeasure buffer_to_thp(uint8_t *);

    modbus_structs::NpkSensorMeasure buffer_to_npk(uint8_t *);
    
    modbus_structs::WpSensorMeasure buffer_to_wp(uint8_t *);

    modbus_structs::RtSensorMeasure buffer_to_rt(uint8_t *);

    modbus_structs::WSensorMeasure buffer_to_w(uint8_t *);

    modbus_structs::W2SensorMeasure buffer_to_w2(uint8_t *);

    modbus_structs::WsSensorMeasure buffer_to_ws(uint8_t *);

    modbus_structs::Ws2SensorMeasure buffer_to_ws2(uint8_t *);

    modbus_structs::WdSensorMeasure buffer_to_wd(uint8_t *);

    modbus_structs::Wd2SensorMeasure buffer_to_wd2(uint8_t *);

    modbus_structs::UvSensorMeasure buffer_to_uv(uint8_t *);

    modbus_structs::ParSensorMeasure buffer_to_par(uint8_t *);

    modbus_structs::TotalrSensorMeasure buffer_to_totalr(uint8_t *);

    uint16_t calculate_crc16(uint8_t *, uint8_t );

    void RegisterPHMeasure(modbus_structs::pHSensorMeasure, uint8_t );

    void RegisterPHElMeasure(modbus_structs::pHElSensorMeasure, uint8_t );

    void RegisterLeafMeasure(modbus_structs::LeafSensorMeasure , uint8_t );

    void RegisterSoilMeasure(modbus_structs::SoilSensorMeasure , uint8_t );

    void RegisterThpMeasure(modbus_structs::ThpSensorMeasure , uint8_t );

    void RegisterNpkMeasure(modbus_structs::NpkSensorMeasure , uint8_t );

    void RegisterWpMeasure(modbus_structs::WpSensorMeasure , uint8_t );

    void RegisterRtMeasure(modbus_structs::RtSensorMeasure , uint8_t );

    void RegisterWMeasure(modbus_structs::WSensorMeasure , uint8_t );

    void RegisterW2Measure(modbus_structs::W2SensorMeasure , uint8_t );

    void RegisterWsMeasure(modbus_structs::WsSensorMeasure , uint8_t );

    void RegisterWs2Measure(modbus_structs::Ws2SensorMeasure , uint8_t );

    void RegisterWdMeasure(modbus_structs::WdSensorMeasure , uint8_t );

    void RegisterWd2Measure(modbus_structs::Wd2SensorMeasure , uint8_t );

    void RegisterUvMeasure(modbus_structs::UvSensorMeasure , uint8_t );

    void RegisterParMeasure(modbus_structs::ParSensorMeasure , uint8_t );

    void RegisterTotalrMeasure(modbus_structs::TotalrSensorMeasure , uint8_t );


    bool validate_checksum(uint8_t *, uint8_t );

    uint8_t detect_type(uint8_t );

    bool ModBus_MakeCMD(uint8_t , uint8_t);

    void makeMeasures();

    void registerMeasure(uint8_t , uint8_t);
};

#endif