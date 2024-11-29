#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#endif

#include <Arduino.h>

class ComInterface
{
public:
    virtual void begin() = 0;
    virtual void sendTemperatureData(float temperature, const String &deviceId) = 0;
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