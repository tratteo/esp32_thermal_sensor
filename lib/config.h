#ifndef CONFIG_H
#define CONFIG_H

// Communication Mode Selection
#define COM_MODE_WIFI 1
#define COM_MODE_SERIAL 2
#define COM_MODE COM_MODE_SERIAL

// WiFi Configuration
#define WIFI_SSID "PodereGateway"
#define WIFI_PASSWORD NULL
#define WIFI_SERVER_URL "http://192.168.1.130:3000/temperature"
#define WIFI_CONNECT_TIMEOUT 10000

// OneWire Sensor Configuration
#define ONE_WIRE_BUS_PIN 4 // GPIO pin for OneWire data line, default is 4
#define TEMPERATURE_READ_INTERVAL_MS 2000

// Payload configuration
#define SENSOR_LOCATION "first_floor"

#endif