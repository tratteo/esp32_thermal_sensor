#ifndef CONFIG_H
#define CONFIG_H

// Communication Mode Selection
#define COM_MODE_WIFI 1
#define COM_MODE_SERIAL 2
#define COM_MODE COM_MODE_WIFI

// WiFi Configuration
#define WIFI_SSID "PodereGateway"
#define WIFI_PASSWORD NULL
#define WIFI_SERVER_URL "http://tratpi.local:5050/api/temperature"
#define WIFI_CONNECT_TIMEOUT 20000

// Sensor Configuration
#define ONE_WIRE_BUS_PIN 4
#define TEMPERATURE_READ_INTERVAL_MS 20000

// Payload configuration
#define SENSOR_LOCATION "Rooms"

#endif