#include "../lib/com.h"
#include "../lib/config.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

void WifiCom::begin()
{
    ComInterface::begin();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    unsigned long startAttemptTime = millis();

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");

        // Timeout for WiFi connection
        if (millis() - startAttemptTime > WIFI_CONNECT_TIMEOUT)
        {
            Serial.println("\nWiFi connection failed!");
            return;
        }
    }
    Serial.println();
    Serial.println("WiFi Connected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

void WifiCom::sendTemperatureData(float temperature, const String &deviceId)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;

        // Prepare JSON payload
        JsonDocument doc;
        doc["temperature"] = temperature;
        doc["sensorId"] = deviceId;
        doc["boardId"] = getBoardMac();
        doc["location"] = SENSOR_LOCATION;
        doc["timestamp"] = getEpochTimestamp();
        doc["com"] = "wifi";

        String jsonPayload;
        serializeJson(doc, jsonPayload);

        http.begin(WIFI_SERVER_URL);
        http.addHeader("Content-Type", "application/json");

        int httpResponseCode = http.POST(jsonPayload);
        Serial.print("HTTP [");
        Serial.print(httpResponseCode);
        Serial.print("] ");
        Serial.println(http.getString());

        http.end();
    }
    else
    {
        Serial.println("WiFi disconnected. reconnecting...");
        begin();
    }
}