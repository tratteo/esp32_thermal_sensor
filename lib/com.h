#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#endif

#include <Arduino.h>
#include "time.h"

class ComInterface
{
public:
    const char *ntpServer = "pool.ntp.org";
    virtual void begin()
    {
        configTime(0, 0, ntpServer);
    }
    virtual void sendTemperatureData(float temperature, const String &deviceId) = 0;
    unsigned long getEpochTimestamp()
    {
        time_t now;
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo))
            return (0);
        time(&now);
        return now;
    }
    String getBoardMac()
    {
        uint64_t mac = ESP.getEfuseMac();
        char macStr[18];
        snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
                 (uint8_t)((mac >> 40) & 0xFF),
                 (uint8_t)((mac >> 32) & 0xFF),
                 (uint8_t)((mac >> 24) & 0xFF),
                 (uint8_t)((mac >> 16) & 0xFF),
                 (uint8_t)((mac >> 8) & 0xFF),
                 (uint8_t)(mac & 0xFF));

        return String(macStr);
    }
    virtual ~ComInterface() {}
};

class WifiCom : public ComInterface
{
public:
    void begin() override;
    void sendTemperatureData(float temperature, const String &deviceId) override;

private:
    void connect();
};

class SerialCom : public ComInterface
{
public:
    void begin() override;
    void sendTemperatureData(float temperature, const String &deviceId) override;
};