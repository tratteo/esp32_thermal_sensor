#include "../lib/com.h"
#include "../lib/config.h"
#include <ArduinoJson.h>

void SerialCom::begin()
{
    // Serial communication is already initialized in main setup
    Serial.println("Serial Communication Initialized");
}

void SerialCom::sendTemperatureData(float temperature, const String &deviceId)
{
    // Send temperature data over Serial
    JsonDocument doc;
    doc["temperature"] = temperature;
    doc["sensor_id"] = deviceId;
    doc["board_id"] = ESP.getEfuseMac();
    doc["location"] = SENSOR_LOCATION;
    doc["com"] = "serial";
    String serialized;
    serializeJsonPretty(doc, serialized);
    Serial.println(serialized);
}