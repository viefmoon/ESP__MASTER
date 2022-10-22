#include "modbus/modbus.h"
#include "json/json.h"
#include "macro.h"
#include "main/main.h"

uint8_t phSensorCounter=0;
uint8_t phelSensorCounter=0;
uint8_t leafSensorCounter=0;
uint8_t soilSensorCounter=0;
uint8_t thpSensorCounter=0;
uint8_t npkSensorCounter=0;
uint8_t wpSensorCounter=0;
uint8_t rtSensorCounter=0;
uint8_t wSensorCounter=0;
uint8_t w2SensorCounter=0;
uint8_t wsSensorCounter=0;
uint8_t ws2SensorCounter=0;
uint8_t wdSensorCounter=0;
uint8_t wd2SensorCounter=0;
uint8_t uvSensorCounter=0;
uint8_t parSensorCounter=0;
uint8_t totalrSensorCounter=0;

// uint8_t MODBUS::detect_type(uint8_t addressSensor){
//     uint8_t type_sensor;
//     if (addressSensor == 0x01){
//         type_sensor = modbus_enum::MODBUS_SENSOR_WS;
//     } 
//     if ((addressSensor >= 0x02) && (addressSensor <= 0x04)){
//         type_sensor = modbus_enum::MODBUS_SENSOR_WS2;
//     } 
//     else if ((addressSensor >= 0x05) && (addressSensor <= 0x06)){
//         type_sensor = modbus_enum::MODBUS_SENSOR_WD;
//     }
//     else if ((addressSensor >= 0x07) && (addressSensor <= 0x08)){
//         type_sensor = modbus_enum::MODBUS_SENSOR_WD2;
//     }
//     else if ((addressSensor >= 0x09) && (addressSensor <= 0x0B)){
//         type_sensor = modbus_enum::MODBUS_SENSOR_W;
//     }
//     else if ((addressSensor >= 0x0C) && (addressSensor <= 0x0F)){
//         type_sensor = modbus_enum::MODBUS_SENSOR_W2;
//     }
//     else if ((addressSensor >= 0x10) && (addressSensor <= 0x1F)){
//         type_sensor = modbus_enum::MODBUS_SENSOR_SOIL;
//     }
//     else if ((addressSensor >= 0x20) && (addressSensor <= 0x2F)){
//         type_sensor = modbus_enum::MODBUS_SENSOR_THP;
//     }
//     else if ((addressSensor >= 0x30) && (addressSensor <= 0x3F)){
//         type_sensor = modbus_enum::MODBUS_SENSOR_PH;
//     }
//     else if ((addressSensor >= 0x40) && (addressSensor <= 0x9F)){
//         type_sensor = modbus_enum::MODBUS_SENSOR_PH_EL;
//     }
//     else if ((addressSensor >= 0x50) && (addressSensor <= 0x5F)){
//         type_sensor = modbus_enum::MODBUS_SENSOR_NPK;
//     }
//     else if ((addressSensor >= 0x60) && (addressSensor <= 0x6F)){
//         type_sensor = modbus_enum::MODBUS_SENSOR_WP;
//     }
//     else if ((addressSensor >= 0x70) && (addressSensor <= 0x7F)){
//         type_sensor = modbus_enum::MODBUS_SENSOR_UV;
//     }
//     else if ((addressSensor >= 0x80) && (addressSensor <= 0x8F)){
//         type_sensor = modbus_enum::MODBUS_SENSOR_PAR;
//     }
//     else if ((addressSensor >= 0x90) && (addressSensor <= 0x95)){
//         type_sensor = modbus_enum::MODBUS_SENSOR_TOTALR;
//     }
//     else if ((addressSensor >= 0x96) && (addressSensor <= 0x9F)){
//         type_sensor = modbus_enum::MODBUS_SENSOR_RT;
//     }
//     else if ((addressSensor >= 0xA0) && (addressSensor <= 0xAF)){
//         type_sensor = modbus_enum::MODBUS_SENSOR_LEAF;
//     }
//     return type_sensor;
// }

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
//0x60 to 0x6F addresses available for Water potential sensor (Pressure sensor on tensiometer)
//0x70 to 0x7F addresses available for UV radiation sensor
//0x80 to 0x8F addresses available for PAR radiation sensor
//0x90 to 0x9F addresses available for Total solar radiation
//0xA0 to 0xAF addresses available for Leaf sensor (Temperature, leaf humidity)

uint8_t sensorAddresses[] = {0x60,0x61,0xA0,0xA1,0x10,0x11,0x30,0x31,0x20,0x50};
//uint8_t sensorAddresses[] = {0x50};

void MODBUS::swap_bytes(uint16_t *byte)
{
    uint8_t *ptr = (uint8_t*)byte;
    uint8_t lb = ptr[1];
    uint8_t hb = ptr[0];
    *byte = lb | (hb << 8U);
}

modbus_structs::pHSensorMeasure MODBUS::buffer_to_ph(uint8_t *buffer)
{
    modbus_structs::rawPH raw = *((modbus_structs::rawPH *)buffer);
    swap_bytes(&raw.ph);
    swap_bytes(&raw.ph_simpl);
    swap_bytes(&raw.ph_temp);
    return {(float)(raw.ph / 100.0F),(float)(raw.ph_simpl / 10.0F),(float)(raw.ph_temp / 10.0F)};
}

modbus_structs::pHElSensorMeasure MODBUS::buffer_to_ph_el(uint8_t *buffer)
{
    modbus_structs::rawPHEl raw = *((modbus_structs::rawPHEl *)buffer);
    swap_bytes(&raw.ph_el);
    return {(float)raw.ph_el / 100.0F};
}

modbus_structs::LeafSensorMeasure MODBUS::buffer_to_leaf(uint8_t *buffer)
{
    modbus_structs::rawLeaf raw = *((modbus_structs::rawLeaf *)buffer);
    MODBUS::swap_bytes(&raw.leaf_humidity);
    MODBUS::swap_bytes(&raw.leaf_temperature);
    // Leaf humidity is divided by 10 because it contains one decimal
    return {(float)(raw.leaf_humidity / 10.0F), (float)(raw.leaf_temperature / 100.0F) - 20.0F };
}

modbus_structs::SoilSensorMeasure MODBUS::buffer_to_soil(uint8_t *buffer)
{
    modbus_structs::rawSoil raw = *((modbus_structs::rawSoil *)buffer);

    MODBUS::swap_bytes(&raw.temperature);
    MODBUS::swap_bytes(&raw.moisture);
    MODBUS::swap_bytes(&raw.ec);

    return {(float)raw.temperature / 100.0F, (float)raw.moisture / 100.0F, (float)raw.ec};
}

modbus_structs::ThpSensorMeasure MODBUS::buffer_to_thp(uint8_t *buffer)
{
    modbus_structs::rawThp raw = *((modbus_structs::rawThp *)buffer);

    MODBUS::swap_bytes(&raw.temperature);
    MODBUS::swap_bytes(&raw.humidity);
    MODBUS::swap_bytes(&raw.pressure);

    return {((float)raw.temperature / 100.0F)-40.0F, (float)raw.humidity / 100.0F, (float)raw.pressure / 10.0F};
}

modbus_structs::NpkSensorMeasure MODBUS::buffer_to_npk(uint8_t *buffer)
{
    modbus_structs::rawNpk raw = *((modbus_structs::rawNpk *)buffer);

    MODBUS::swap_bytes(&raw.nitrogen);
    MODBUS::swap_bytes(&raw.phosphorus);
    MODBUS::swap_bytes(&raw.potassium);

    return {(float)raw.nitrogen, (float)raw.phosphorus, (float)raw.potassium};
}

modbus_structs::WpSensorMeasure MODBUS::buffer_to_wp(uint8_t *buffer)
{
    modbus_structs::rawWp raw = *((modbus_structs::rawWp *)buffer);

    MODBUS::swap_bytes(&raw.water_potential);

    return {(float)raw.water_potential / 100.0F};
}


modbus_structs::RtSensorMeasure MODBUS::buffer_to_rt(uint8_t *buffer)
{
    modbus_structs::rawRt raw = *((modbus_structs::rawRt *)buffer);

    MODBUS::swap_bytes(&raw.par);
    MODBUS::swap_bytes(&raw.total_r);
    MODBUS::swap_bytes(&raw.uv);

    return {(float)raw.par, (float)raw.total_r, (float)raw.uv};
}

modbus_structs::WSensorMeasure MODBUS::buffer_to_w(uint8_t *buffer)
{
    modbus_structs::rawW raw = *((modbus_structs::rawW *)buffer);

    MODBUS::swap_bytes(&raw.water);

    return {(float)raw.water};
}

modbus_structs::W2SensorMeasure MODBUS::buffer_to_w2(uint8_t *buffer)
{
    modbus_structs::rawW2 raw = *((modbus_structs::rawW2 *)buffer);

    MODBUS::swap_bytes(&raw.water2);

    return {(float)raw.water2};
}

modbus_structs::WsSensorMeasure MODBUS::buffer_to_ws(uint8_t *buffer)
{
    modbus_structs::rawWs raw = *((modbus_structs::rawWs *)buffer);

    MODBUS::swap_bytes(&raw.wind_speed);

    return {(float)raw.wind_speed};
}

modbus_structs::Ws2SensorMeasure MODBUS::buffer_to_ws2(uint8_t *buffer)
{
    modbus_structs::rawWs2 raw = *((modbus_structs::rawWs2 *)buffer);

    MODBUS::swap_bytes(&raw.wind_speed2);

    return {(float)raw.wind_speed2};
}

modbus_structs::WdSensorMeasure MODBUS::buffer_to_wd(uint8_t *buffer)
{
    modbus_structs::rawWd raw = *((modbus_structs::rawWd *)buffer);

    MODBUS::swap_bytes(&raw.wind_direction);

    return {(float)raw.wind_direction};
}

modbus_structs::Wd2SensorMeasure MODBUS::buffer_to_wd2(uint8_t *buffer)
{
    modbus_structs::rawWd2 raw = *((modbus_structs::rawWd2 *)buffer);

    MODBUS::swap_bytes(&raw.wind_direction2);

    return {(float)raw.wind_direction2};
}

modbus_structs::UvSensorMeasure MODBUS::buffer_to_uv(uint8_t *buffer)
{
    modbus_structs::rawUv raw = *((modbus_structs::rawUv *)buffer);

    MODBUS::swap_bytes(&raw.uv_radiation);

    return {(float)raw.uv_radiation};
}

modbus_structs::ParSensorMeasure MODBUS::buffer_to_par(uint8_t *buffer)
{
    modbus_structs::rawPar raw = *((modbus_structs::rawPar *)buffer);

    MODBUS::swap_bytes(&raw.par_radiation);

    return {(float)raw.par_radiation};
}

modbus_structs::TotalrSensorMeasure MODBUS::buffer_to_totalr(uint8_t *buffer)
{
    modbus_structs::rawTotalr raw = *((modbus_structs::rawTotalr *)buffer);

    MODBUS::swap_bytes(&raw.total_radiation);

    return {(float)raw.total_radiation};
}



uint16_t MODBUS::calculate_crc16(uint8_t *buf, uint8_t len)
{  
	uint16_t crc = 0xFFFF;
	unsigned int i = 0;
	char bit = 0;

	for( i = 0; i < len; i++ )
	{
		crc ^= buf[i];

		for( bit = 0; bit < 8; bit++ )
		{
			if( crc & 0x0001 )
			{
				crc >>= 1;
				crc ^= 0xA001;
			}
			else
			{
				crc >>= 1;
			}
		}
	}
	return crc;
}

void MODBUS::RegisterPHMeasure(modbus_structs::pHSensorMeasure ph, uint8_t index)
{
    switch (index)
    {
    case 1:
        DeviceMeasures.addMeasure(ph.ph, F("pH1"));
        //DeviceMeasures.addMeasure(ph.ph_temp, F("pH1_temp"));
        break;
    case 2:
        DeviceMeasures.addMeasure(ph.ph, F("pH2"));
        //DeviceMeasures.addMeasure(ph.ph_temp, F("pH2_temp"));
        break;
    }
}

void MODBUS::RegisterPHElMeasure(modbus_structs::pHElSensorMeasure ph, uint8_t index)
{
    switch (index)
    {
    case 1:
        DeviceMeasures.addMeasure(ph.ph_el, F("pH_el1"));
        break;
    case 2:
        DeviceMeasures.addMeasure(ph.ph_el, F("pH_el2"));
        break;
    }
}

void MODBUS::RegisterLeafMeasure(modbus_structs::LeafSensorMeasure leaf, uint8_t index)
{
    switch (index)
    {
    case 1:
        DeviceMeasures.addMeasure(leaf.leaf_humidity, F("LH1"));
        DeviceMeasures.addMeasure(leaf.leaf_temperature, F("LT1"));
        break;
    case 2:
        DeviceMeasures.addMeasure(leaf.leaf_humidity, F("LH2"));
        DeviceMeasures.addMeasure(leaf.leaf_temperature, F("LT2"));
        break;
    }
}

void MODBUS::RegisterSoilMeasure(modbus_structs::SoilSensorMeasure soil, uint8_t index)
{
    switch (index)
    {
    case 1:
        DeviceMeasures.addMeasure(soil.ec, F("sec1"));
        DeviceMeasures.addMeasure(soil.humidity, F("sh1"));
        DeviceMeasures.addMeasure(soil.temperature, F("st_1"));
        break;
    case 2:
        DeviceMeasures.addMeasure(soil.ec, F("sec2"));
        DeviceMeasures.addMeasure(soil.humidity, F("sh2"));
        DeviceMeasures.addMeasure(soil.temperature, F("st_2"));
        break;
    }
}

void MODBUS::RegisterThpMeasure(modbus_structs::ThpSensorMeasure thp, uint8_t index)
{
    switch (index)
    {
    case 1:
        DeviceMeasures.addMeasure(thp.temperature, F("at1"));
        DeviceMeasures.addMeasure(thp.humidity, F("ahh1"));
        DeviceMeasures.addMeasure(thp.pressure, F("app1"));
        break;
    case 2:
        DeviceMeasures.addMeasure(thp.temperature, F("T2"));
        DeviceMeasures.addMeasure(thp.humidity, F("H2"));
        DeviceMeasures.addMeasure(thp.pressure, F("AP2"));
        break;
    }
}

void MODBUS::RegisterNpkMeasure(modbus_structs::NpkSensorMeasure npk, uint8_t index)
{
    switch (index)
    {
    case 1:
        DeviceMeasures.addMeasure(npk.nitrogen, F("nit1"));
        DeviceMeasures.addMeasure(npk.phosphorus, F("pho1"));
        DeviceMeasures.addMeasure(npk.potassium, F("pot1"));
        break;
    case 2:
        DeviceMeasures.addMeasure(npk.nitrogen, F("nit2"));
        DeviceMeasures.addMeasure(npk.phosphorus, F("pho2"));
        DeviceMeasures.addMeasure(npk.potassium, F("pot2"));
        break;
    }
}


void MODBUS::RegisterWpMeasure(modbus_structs::WpSensorMeasure wp, uint8_t index)
{
    switch (index)
    {
    case 1:
        DeviceMeasures.addMeasure(wp.water_potential, F("wp1"));
        break;
    case 2:
        DeviceMeasures.addMeasure(wp.water_potential, F("wp2"));
        break;
    }
}

void MODBUS::RegisterRtMeasure(modbus_structs::RtSensorMeasure rt, uint8_t index)
{
    switch (index)
    {
    case 1:
        DeviceMeasures.addMeasure(rt.par, F("par"));
        DeviceMeasures.addMeasure(rt.total_r, F("total"));
        DeviceMeasures.addMeasure(rt.uv, F("uv"));
        break;
    case 2:
        DeviceMeasures.addMeasure(rt.par, F("par"));
        DeviceMeasures.addMeasure(rt.total_r, F("total"));
        DeviceMeasures.addMeasure(rt.uv, F("uv"));
        break;
    }
}

void MODBUS::RegisterWMeasure(modbus_structs::WSensorMeasure w, uint8_t index)
{
    switch (index)
    {
    case 1:
        DeviceMeasures.addMeasure(w.water, F("w1"));
        break;
    case 2:
        DeviceMeasures.addMeasure(w.water, F("w2"));
        break;
    }
}

void MODBUS::RegisterW2Measure(modbus_structs::W2SensorMeasure w, uint8_t index)
{
    switch (index)
    {
    case 1:
        DeviceMeasures.addMeasure(w.water2, F("w1"));
        break;
    case 2:
        DeviceMeasures.addMeasure(w.water2, F("w2"));
        break;
    }
}

void MODBUS::RegisterWsMeasure(modbus_structs::WsSensorMeasure ws, uint8_t index)
{
    switch (index)
    {
    case 1:
        DeviceMeasures.addMeasure(ws.wind_speed, F("ws1"));
        break;
    case 2:
        DeviceMeasures.addMeasure(ws.wind_speed, F("ws2"));
        break;
    }
}

void MODBUS::RegisterWs2Measure(modbus_structs::Ws2SensorMeasure ws, uint8_t index)
{
    switch (index)
    {
    case 1:
        DeviceMeasures.addMeasure(ws.wind_speed2, F("ws1"));
        break;
    case 2:
        DeviceMeasures.addMeasure(ws.wind_speed2, F("ws2"));
        break;
    }
}

void MODBUS::RegisterWdMeasure(modbus_structs::WdSensorMeasure wd, uint8_t index)
{
    switch (index)
    {
    case 1:
        DeviceMeasures.addMeasure(wd.wind_direction, F("wd1"));
        break;
    case 2:
        DeviceMeasures.addMeasure(wd.wind_direction, F("wd2"));
        break;
    }
}

void MODBUS::RegisterWd2Measure(modbus_structs::Wd2SensorMeasure wd, uint8_t index)
{
    switch (index)
    {
    case 1:
        DeviceMeasures.addMeasure(wd.wind_direction2, F("wd1"));
        break;
    case 2:
        DeviceMeasures.addMeasure(wd.wind_direction2, F("wd2"));
        break;
    }
}

void MODBUS::RegisterUvMeasure(modbus_structs::UvSensorMeasure uv, uint8_t index)
{
    switch (index)
    {
    case 1:
        DeviceMeasures.addMeasure(uv.uv_radiation, F("uv1"));
        break;
    case 2:
        DeviceMeasures.addMeasure(uv.uv_radiation, F("uv2"));
        break;
    }
}

void MODBUS::RegisterParMeasure(modbus_structs::ParSensorMeasure par, uint8_t index)
{
    switch (index)
    {
    case 1:
        DeviceMeasures.addMeasure(par.par_radiation, F("par1"));
        break;
    case 2:
        DeviceMeasures.addMeasure(par.par_radiation, F("par1"));
        break;
    }
}

void MODBUS::RegisterTotalrMeasure(modbus_structs::TotalrSensorMeasure totalr, uint8_t index)
{
    switch (index)
    {
    case 1:
        DeviceMeasures.addMeasure(totalr.total_radiation, F("totalr1"));
        break;
    case 2:
        DeviceMeasures.addMeasure(totalr.total_radiation, F("totalr2"));
        break;
    }
}

bool MODBUS::validate_checksum(uint8_t *buffer, uint8_t length)
{
    uint16_t crc = buffer[length-1]<< 8U | (buffer[length-2]);
    uint16_t calc_crc = calculate_crc16(buffer, length - 2);
    // Serial.println(crc,HEX);
    // Serial.println(calc_crc,HEX);
    return crc == calc_crc;
}

uint8_t MODBUS::detect_type(uint8_t addressSensor){
    uint8_t type_sensor;
    if (addressSensor == 0x01){
        type_sensor = modbus_enum::MODBUS_SENSOR_WS;
    } 
    if ((addressSensor >= 0x02) && (addressSensor <= 0x04)){
        type_sensor = modbus_enum::MODBUS_SENSOR_WS2;
    } 
    else if ((addressSensor >= 0x05) && (addressSensor <= 0x06)){
        type_sensor = modbus_enum::MODBUS_SENSOR_WD;
    }
    else if ((addressSensor >= 0x07) && (addressSensor <= 0x08)){
        type_sensor = modbus_enum::MODBUS_SENSOR_WD2;
    }
    else if ((addressSensor >= 0x09) && (addressSensor <= 0x0B)){
        type_sensor = modbus_enum::MODBUS_SENSOR_W;
    }
    else if ((addressSensor >= 0x0C) && (addressSensor <= 0x0F)){
        type_sensor = modbus_enum::MODBUS_SENSOR_W2;
    }
    else if ((addressSensor >= 0x10) && (addressSensor <= 0x1F)){
        type_sensor = modbus_enum::MODBUS_SENSOR_SOIL;
    }
    else if ((addressSensor >= 0x20) && (addressSensor <= 0x2F)){
        type_sensor = modbus_enum::MODBUS_SENSOR_THP;
    }
    else if ((addressSensor >= 0x30) && (addressSensor <= 0x3F)){
        type_sensor = modbus_enum::MODBUS_SENSOR_PH;
    }
    else if ((addressSensor >= 0x40) && (addressSensor <= 0x4F)){
        type_sensor = modbus_enum::MODBUS_SENSOR_PH_EL;
    }
    else if ((addressSensor >= 0x50) && (addressSensor <= 0x5F)){
        type_sensor = modbus_enum::MODBUS_SENSOR_NPK;
    }
    else if ((addressSensor >= 0x60) && (addressSensor <= 0x6F)){
        type_sensor = modbus_enum::MODBUS_SENSOR_WP;
    }
    else if ((addressSensor >= 0x70) && (addressSensor <= 0x7F)){
        type_sensor = modbus_enum::MODBUS_SENSOR_UV;
    }
    else if ((addressSensor >= 0x80) && (addressSensor <= 0x8F)){
        type_sensor = modbus_enum::MODBUS_SENSOR_PAR;
    }
    else if ((addressSensor >= 0x90) && (addressSensor <= 0x95)){
        type_sensor = modbus_enum::MODBUS_SENSOR_TOTALR;
    }
    else if ((addressSensor >= 0x96) && (addressSensor <= 0x9F)){
        type_sensor = modbus_enum::MODBUS_SENSOR_RT;
    }
    else if ((addressSensor >= 0xA0) && (addressSensor <= 0xAF)){
        type_sensor = modbus_enum::MODBUS_SENSOR_LEAF;
    }
    return type_sensor;
}

bool MODBUS::ModBus_MakeCMD(uint8_t address, uint8_t function_code)
{
    uint8_t sensor_type = MODBUS::detect_type(address);
    uint8_t dataBytes = 0;

    modbus_structs::Modbus_tx_frame cmd = {address, 0x00, 0x0000, 0x0000, 0x0000};

    switch (function_code)
    {
    case modbus_enum::MODBUS_CMD_READ:
        cmd.tx_functionCode = MODBUS_CMD_READ_BYTE;
        break;
    }

    switch (sensor_type)
    {
    case modbus_enum::MODBUS_SENSOR_WS:
        Sprintln(F("ID FROM WS"));
        wsSensorCounter++;
        cmd.registerStartAddress = WS_START_ADDRESS;
        cmd.registerLength = WS_BYTE_LENGHT;
        dataBytes = 2;
        break;
    case modbus_enum::MODBUS_SENSOR_WS2:
    Sprintln(F("ID FROM WS2"));
        ws2SensorCounter++;
        cmd.registerStartAddress = WS2_START_ADDRESS;
        cmd.registerLength = WS2_BYTE_LENGHT;
        dataBytes = 2;
        break;
    case modbus_enum::MODBUS_SENSOR_WD:
    Sprintln(F("ID FROM WD"));
        wdSensorCounter++;
        cmd.registerStartAddress = WD_START_ADDRESS;
        cmd.registerLength = WD_BYTE_LENGHT;
        dataBytes = 2;
        break;
    case modbus_enum::MODBUS_SENSOR_WD2:
        Sprintln(F("ID FROM WD2"));
        wd2SensorCounter++;
        cmd.registerStartAddress = WD2_START_ADDRESS;
        cmd.registerLength = WD2_BYTE_LENGHT;
        dataBytes = 2;
        break;
    case modbus_enum::MODBUS_SENSOR_W:
    Sprintln(F("ID FROM W"));
        wSensorCounter++;
        cmd.registerStartAddress = W_START_ADDRESS;
        cmd.registerLength = W_BYTE_LENGHT;
        dataBytes = 2;
        break;
    case modbus_enum::MODBUS_SENSOR_W2:
        Sprintln(F("ID FROM W2"));
        w2SensorCounter++;
        cmd.registerStartAddress = W2_START_ADDRESS;
        cmd.registerLength = W2_BYTE_LENGHT;
        dataBytes = 2;
        break;
    case modbus_enum::MODBUS_SENSOR_SOIL:
        Sprintln(F("ID FROM SOIL"));
        soilSensorCounter++;
        cmd.registerStartAddress = SOIL_START_ADDRESS;
        cmd.registerLength = SOIL_BYTE_LENGHT;
        dataBytes = 6;
        break;
    case modbus_enum::MODBUS_SENSOR_THP:
        Sprintln(F("ID FROM THP"));
        thpSensorCounter++;
        cmd.registerStartAddress = THP_START_ADDRESS;
        cmd.registerLength = THP_BYTE_LENGHT;
        dataBytes = 6;
        break;
    case modbus_enum::MODBUS_SENSOR_PH:
        Sprintln(F("ID FROM PH"));
        phSensorCounter++;
        cmd.registerStartAddress = PH_START_ADDRESS;
        cmd.registerLength = PH_BYTE_LENGHT;
        dataBytes = 6;
        break;
    case modbus_enum::MODBUS_SENSOR_PH_EL:
        Sprintln(F("ID FROM PH_EL"));
        phelSensorCounter++;
        cmd.registerStartAddress = PH_EL_START_ADDRESS;
        cmd.registerLength = PH_EL_BYTE_LENGHT;
        dataBytes = 2;
        break;
    case modbus_enum::MODBUS_SENSOR_NPK:
        Sprintln(F("ID FROM NPK SENSOR"));
        npkSensorCounter++;
        cmd.registerStartAddress = NPK_START_ADDRESS;
        cmd.registerLength = NPK_BYTE_LENGHT;
        dataBytes = 6;
        break;
    case modbus_enum::MODBUS_SENSOR_WP:
        Sprintln(F("ID FROM WP"));
        wpSensorCounter++;
        cmd.registerStartAddress = WP_START_ADDRESS;
        cmd.registerLength = WP_BYTE_LENGHT;
        dataBytes = 2;
        break;
    case modbus_enum::MODBUS_SENSOR_UV:
        Sprintln(F("ID FROM UV"));
        uvSensorCounter++;
        cmd.registerStartAddress = UV_START_ADDRESS;
        cmd.registerLength = UV_BYTE_LENGHT;
        dataBytes = 2;
        break;
    case modbus_enum::MODBUS_SENSOR_PAR:
        Sprintln(F("ID FROM PAR"));
        parSensorCounter++;
        cmd.registerStartAddress = PAR_START_ADDRESS;
        cmd.registerLength = PAR_BYTE_LENGHT;
        dataBytes = 2;
        break;
    case modbus_enum::MODBUS_SENSOR_TOTALR:
        Sprintln(F("ID FROM TOTALR"));
        totalrSensorCounter++;
        cmd.registerStartAddress = TOTALR_START_ADDRESS;
        cmd.registerLength = TOTALR_BYTE_LENGHT;
        dataBytes = 2;
        break;
    case modbus_enum::MODBUS_SENSOR_LEAF:
        Sprintln(F("ID FROM LEAF"));
        leafSensorCounter++;
        cmd.registerStartAddress = LEAF_START_ADDRESS;
        cmd.registerLength = LEAF_BYTE_LENGHT;
        dataBytes = 4;
        break;
    case modbus_enum::MODBUS_SENSOR_RT:
        Sprintln(F("ID FROM RT"));
        rtSensorCounter++;
        cmd.registerStartAddress = RT_START_ADDRESS;
        cmd.registerLength = RT_BYTE_LENGHT;
        dataBytes = 6;
        break;
    }

    cmd.tx_crc = MODBUS::calculate_crc16((uint8_t *)&cmd, 6);

    if(dataBytes>0){
        uint8_t attempts=0;
        bool Check_CRC = false;
        while (Check_CRC == false && attempts < MaxAttempts) {
            uint8_t buffer[RX_LENGHT_WITHOUT_DATA + dataBytes]; 
            attempts++;
            MODBUS_SERIAL.write((uint8_t*)&cmd, sizeof(cmd));
            MODBUS_SERIAL.setTimeout(2000);
            if ((MODBUS_SERIAL.readBytes(buffer, RX_LENGHT_WITHOUT_DATA + dataBytes))==(RX_LENGHT_WITHOUT_DATA + dataBytes)){

                uint8_t *buffer_data = &buffer[3]; // the data come after 3 bytes |adress| |cmd| |byte length| |DATA|

                Check_CRC = validate_checksum(buffer, RX_LENGHT_WITHOUT_DATA + dataBytes);
                if(Check_CRC){
                    switch (sensor_type)
                    {
                    case modbus_enum::MODBUS_SENSOR_WS:
                        {
                        modbus_structs::WsSensorMeasure wsMeasures = MODBUS::buffer_to_ws(buffer_data);
                        RegisterWsMeasure(wsMeasures, wsSensorCounter);
                        return true;
                        }
                        break;
                    case modbus_enum::MODBUS_SENSOR_WS2:
                        {
                        modbus_structs::Ws2SensorMeasure ws2Measures = MODBUS::buffer_to_ws2(buffer_data);
                        RegisterWs2Measure(ws2Measures, ws2SensorCounter);
                        return true;
                        }
                        break;
                    case modbus_enum::MODBUS_SENSOR_WD:
                        {
                        modbus_structs::WdSensorMeasure wdMeasures = MODBUS::buffer_to_wd(buffer_data);
                        RegisterWdMeasure(wdMeasures, wdSensorCounter);
                        return true;
                        }
                        break;
                    case modbus_enum::MODBUS_SENSOR_WD2:
                        {
                        modbus_structs::Wd2SensorMeasure wd2Measures = MODBUS::buffer_to_wd2(buffer_data);
                        RegisterWd2Measure(wd2Measures, wd2SensorCounter);
                        return true;
                        }
                        break;
                    case modbus_enum::MODBUS_SENSOR_W:
                        {
                        modbus_structs::WSensorMeasure wMeasures = MODBUS::buffer_to_w(buffer_data);
                        RegisterWMeasure(wMeasures, wSensorCounter);
                        return true;
                        }
                        break;
                    case modbus_enum::MODBUS_SENSOR_W2:
                        {
                        modbus_structs::W2SensorMeasure w2Measures = MODBUS::buffer_to_w2(buffer_data);
                        RegisterW2Measure(w2Measures, w2SensorCounter);
                        return true;
                        }
                        break;
                    case modbus_enum::MODBUS_SENSOR_SOIL:
                        {
                        modbus_structs::SoilSensorMeasure soilMeasures = MODBUS::buffer_to_soil(buffer_data);
                        RegisterSoilMeasure(soilMeasures, soilSensorCounter);
                        return true;
                        }
                        break;
                    case modbus_enum::MODBUS_SENSOR_THP:
                        {
                        modbus_structs::ThpSensorMeasure thpMeasures = MODBUS::buffer_to_thp(buffer_data);
                        RegisterThpMeasure(thpMeasures, thpSensorCounter);
                        return true;
                        }
                        break;
                    case modbus_enum::MODBUS_SENSOR_PH:
                        {
                        modbus_structs::pHSensorMeasure phMeasures = MODBUS::buffer_to_ph(buffer_data);
                        RegisterPHMeasure(phMeasures, phSensorCounter);
                        return true;
                        }
                        break;
                    case modbus_enum::MODBUS_SENSOR_PH_EL:
                        {
                        modbus_structs::pHElSensorMeasure phelMeasures = MODBUS::buffer_to_ph_el(buffer_data);
                        RegisterPHElMeasure(phelMeasures, phelSensorCounter);
                        return true;
                        }
                        break;
                    case modbus_enum::MODBUS_SENSOR_NPK:
                        {
                        modbus_structs::NpkSensorMeasure npkMeasures = MODBUS::buffer_to_npk(buffer_data);
                        RegisterNpkMeasure(npkMeasures, npkSensorCounter);
                        return true;
                        }
                        break;
                    case modbus_enum::MODBUS_SENSOR_WP:
                        {
                        modbus_structs::WpSensorMeasure wpMeasures = MODBUS::buffer_to_wp(buffer_data);
                        RegisterWpMeasure(wpMeasures, wpSensorCounter);
                        return true;
                        }
                        break;
                    case modbus_enum::MODBUS_SENSOR_UV:
                        {
                        modbus_structs::UvSensorMeasure uvMeasures = MODBUS::buffer_to_uv(buffer_data);
                        RegisterUvMeasure(uvMeasures, uvSensorCounter);
                        return true;
                        }
                        break;
                    case modbus_enum::MODBUS_SENSOR_PAR:
                        {
                        modbus_structs::ParSensorMeasure parMeasures = MODBUS::buffer_to_par(buffer_data);
                        RegisterParMeasure(parMeasures, parSensorCounter);
                        return true;
                        }
                        break;
                    case modbus_enum::MODBUS_SENSOR_TOTALR:
                        {
                        modbus_structs::TotalrSensorMeasure totalrMeasures = MODBUS::buffer_to_totalr(buffer_data);
                        RegisterTotalrMeasure(totalrMeasures, totalrSensorCounter);
                        return true;
                        }
                        break;
                    case modbus_enum::MODBUS_SENSOR_LEAF:
                        {
                        modbus_structs::LeafSensorMeasure leafMeasures = MODBUS::buffer_to_leaf( buffer_data);
                        RegisterLeafMeasure(leafMeasures, leafSensorCounter);
                        return true;
                        }
                        break;
                    case modbus_enum::MODBUS_SENSOR_RT:
                        {
                        modbus_structs::RtSensorMeasure rtMeasures = MODBUS::buffer_to_rt(buffer_data);
                        RegisterRtMeasure(rtMeasures, rtSensorCounter);
                        return true;
                        }
                        break;
                    }
                }
                else{
                    Sprintln(F("BAD CRC "));
                }
            }
            else{
                Sprintln(F("Error en la respuesta"));
            }
        }
    }
    else{
        Sprintln(F("Sensor no registrado"));
    }
    return false;
}


void MODBUS::makeMeasures()
{
    MODBUS_SERIAL.begin(MODBUS_SERIAL_BAUDRATE);
    digitalWrite(pinRs485, HIGH);
    for (uint8_t i = 0; i < (sizeof(sensorAddresses)/sizeof(uint8_t)); i++) {
        bool state = MODBUS::ModBus_MakeCMD(sensorAddresses[i], modbus_enum::MODBUS_CMD_READ);
        if(state){
            Sprintln(F("Exito, sensor: "));
            //Serial.println(sensorAddresses[i],HEX);
        }
        else{
            Sprintln(F("Error, sensor: "));
            //Serial.println(sensorAddresses[i],HEX);
        }
    }
    digitalWrite(pinRs485, LOW);

    phSensorCounter=0;
    phelSensorCounter=0;
    leafSensorCounter=0;
    soilSensorCounter=0;
    thpSensorCounter=0;
    npkSensorCounter=0;
    wpSensorCounter=0;
    rtSensorCounter=0;
    wSensorCounter=0;
    w2SensorCounter=0;
    wsSensorCounter=0;
    ws2SensorCounter=0;
    wdSensorCounter=0;
    wd2SensorCounter=0;
    uvSensorCounter=0;
    parSensorCounter=0;
    totalrSensorCounter=0;
}


void MODBUS::registerMeasure(uint8_t no_sensor, uint8_t type)
{
    uint8_t * ptr_to_data;
    switch (type)
    {
    case modbus_enum::MODBUS_SENSOR_WS:
        MODBUS::RegisterWsMeasure(MODBUS::buffer_to_ws(ptr_to_data), no_sensor);
        break;
    case modbus_enum::MODBUS_SENSOR_WS2:
        MODBUS::RegisterWs2Measure(MODBUS::buffer_to_ws2(ptr_to_data), no_sensor);
        break;
    case modbus_enum::MODBUS_SENSOR_WD:
        MODBUS::RegisterWdMeasure(MODBUS::buffer_to_wd(ptr_to_data), no_sensor);
        break;
    case modbus_enum::MODBUS_SENSOR_WD2:
        MODBUS::RegisterWd2Measure(MODBUS::buffer_to_wd2(ptr_to_data), no_sensor);
        break;
    case modbus_enum::MODBUS_SENSOR_W:
        MODBUS::RegisterWMeasure(MODBUS::buffer_to_w(ptr_to_data), no_sensor);
        break;
    case modbus_enum::MODBUS_SENSOR_W2:
        MODBUS::RegisterW2Measure(MODBUS::buffer_to_w2(ptr_to_data), no_sensor);
        break;
    case modbus_enum::MODBUS_SENSOR_SOIL:
        MODBUS::RegisterSoilMeasure(MODBUS::buffer_to_soil(ptr_to_data), no_sensor);
        break;
    case modbus_enum::MODBUS_SENSOR_THP:
        MODBUS::RegisterThpMeasure(MODBUS::buffer_to_thp(ptr_to_data), no_sensor);
        break;
    case modbus_enum::MODBUS_SENSOR_PH:
        MODBUS::RegisterPHMeasure(MODBUS::buffer_to_ph(ptr_to_data), no_sensor);
        break;
    case modbus_enum::MODBUS_SENSOR_PH_EL:
        MODBUS::RegisterPHElMeasure(MODBUS::buffer_to_ph_el(ptr_to_data), no_sensor);
        break;
    case modbus_enum::MODBUS_SENSOR_NPK:
        MODBUS::RegisterNpkMeasure(MODBUS::buffer_to_npk(ptr_to_data), no_sensor);
        break;
    case modbus_enum::MODBUS_SENSOR_WP:
        MODBUS::RegisterWpMeasure(MODBUS::buffer_to_wp(ptr_to_data), no_sensor);
        break;
    case modbus_enum::MODBUS_SENSOR_UV:
        MODBUS::RegisterUvMeasure(MODBUS::buffer_to_uv(ptr_to_data), no_sensor);
        break;
    case modbus_enum::MODBUS_SENSOR_PAR:
        MODBUS::RegisterParMeasure(MODBUS::buffer_to_par(ptr_to_data), no_sensor);
        break;
    case modbus_enum::MODBUS_SENSOR_TOTALR:
        MODBUS::RegisterTotalrMeasure(MODBUS::buffer_to_totalr(ptr_to_data), no_sensor);
        break;
    case modbus_enum::MODBUS_SENSOR_LEAF:
        MODBUS::RegisterLeafMeasure(MODBUS::buffer_to_leaf(ptr_to_data), no_sensor);
        break;
    case modbus_enum::MODBUS_SENSOR_RT:
        MODBUS::RegisterRtMeasure(MODBUS::buffer_to_rt(ptr_to_data), no_sensor);
        break;
    default:
        break;
    }
}
