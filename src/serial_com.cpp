#include "../lib/com.h"
#include "../lib/config.h"
#include <ArduinoJson.h>

void SerialCom::begin()
{
    ComInterface::begin();
    // Serial communication is already initialized in main setup
    Serial.println("serial com initialized");
}

void SerialCom::sendTemperatureData(float temperature, const String &deviceId)
{
    // Send temperature data over Serial
    JsonDocument doc;
    doc["temperature"] = temperature;
    doc["sensorId"] = deviceId;
    doc["boardId"] = getBoardMac();
    doc["location"] = SENSOR_LOCATION;
    doc["timestamp"] = getEpochTimestamp();
    doc["com"] = "serial";
    String serialized;
    serializeJsonPretty(doc, serialized);
    Serial.println(serialized);
}