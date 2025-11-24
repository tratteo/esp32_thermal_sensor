#include <OneWire.h>
#include <DallasTemperature.h>
#include "../lib/config.h"
#include "../lib/com.h"

// Conversion factor for micro seconds to seconds
#define uS_TO_S_FACTOR 1000000ULL

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

  // Print wakeup reason (useful for debugging)
  Serial.print("Wakeup count: ");
  Serial.println(esp_sleep_get_wakeup_cause());

  temperatureSensor.begin();

#if COM_MODE == COM_MODE_WIFI
  communicationMethod = new WifiCom();
#elif CURRENT_COMM_MODE == COMM_MODE_SERIAL
  communicationMethod = new SerialCom();
#else
#error "invalid communication mode selected"
#endif

  communicationMethod->begin();

  // Request temperature readings
  temperatureSensor.requestTemperatures();

  // Read temperature in Celsius
  float tempCelsius = temperatureSensor.getTempCByIndex(0);

  // Send temperature via selected communication method
  DeviceAddress deviceAddress;
  if (!temperatureSensor.getAddress(deviceAddress, 0) || tempCelsius == DEVICE_DISCONNECTED_C)
  {
    Serial.println("temperature sensor disconnected");
  }
  else
  {
    communicationMethod->sendTemperatureData(tempCelsius, getDeviceAddressString(deviceAddress));
  }

  // Clean up
  delete communicationMethod;
  communicationMethod = nullptr;

  // Configure deep sleep timer
  uint64_t sleepTimeUs = TEMPERATURE_READ_INTERVAL_MS * 1000ULL;
  esp_sleep_enable_timer_wakeup(sleepTimeUs);

  Serial.println("Entering deep sleep...");
  Serial.flush(); // Wait for serial to finish transmitting

  // Enter deep sleep
  esp_deep_sleep_start();
}

void loop()
{
  // This will never be called since ESP32 enters deep sleep
  // and restarts from setup() on wakeup
}