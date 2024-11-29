#include <OneWire.h>
#include <DallasTemperature.h>
#include "../lib/config.h"
#include "../lib/com.h"

OneWire oneWire(ONE_WIRE_BUS_PIN);
DallasTemperature temperatureSensor(&oneWire);

uint64_t boardMac = ESP.getEfuseMac();

ComInterface *communicationMethod = nullptr;

String getDeviceAddressString(DeviceAddress &address)
{
  String addressString = "";
  for (uint8_t i = 0; i < 8; i++)
  {
    if (address[i] < 16)
      addressString += "0";
    addressString += String(address[i], HEX);
  }
  return addressString;
}

void setup()
{
  // Initialize debug serial
  Serial.begin(115200);
  Serial.println();

  temperatureSensor.begin();

#if COM_MODE == COM_MODE_WIFI
  communicationMethod = new WifiCom();
#elif CURRENT_COMM_MODE == COMM_MODE_SERIAL
  communicationMethod = new SerialCom();
#else
#error "invalid communication mode selected"
#endif

  communicationMethod->begin();
}

void loop()
{
  // Request temperature readings
  temperatureSensor.requestTemperatures();

  // Read temperature in Celsius
  float tempCelsius = temperatureSensor.getTempCByIndex(0);

  // Send temperature via selected communication method
  DeviceAddress deviceAddress;
  if (!temperatureSensor.getAddress(deviceAddress, 0))
  {
    Serial.println("temperature sensor disconnected");
  }
  else
  {
    communicationMethod->sendTemperatureData(tempCelsius, getDeviceAddressString(deviceAddress));
  }

  // Wait before next reading
  delay(TEMPERATURE_READ_INTERVAL_MS);
}
