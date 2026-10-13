#ifndef CONFIG_H
#define CONFIG_H

// Communication Mode Selection
#define COM_MODE_WIFI 1
#define COM_MODE_SERIAL 2

// Local overrides (never committed). Copy lib/config_local.example.h to
// lib/config_local.h and set your real credentials there.
#if defined(__has_include)
#if __has_include("config_local.h")
#include "config_local.h"
#endif
#endif

// Communication mode (override in config_local.h if needed)
#ifndef COM_MODE
#define COM_MODE COM_MODE_WIFI
#endif

// WiFi Configuration (defaults are placeholders; override in config_local.h)
#ifndef WIFI_SSID
#define WIFI_SSID "your-ssid"
#endif
#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "your-password"
#endif
#ifndef WIFI_SERVER_URL
#define WIFI_SERVER_URL "http://your-server.local:5050/api/temperature"
#endif
#ifndef WIFI_CONNECT_TIMEOUT
#define WIFI_CONNECT_TIMEOUT 20000
#endif

// Sensor Configuration
#ifndef ONE_WIRE_BUS_PIN
#define ONE_WIRE_BUS_PIN 4
#endif
#ifndef TEMPERATURE_READ_INTERVAL_MS
#define TEMPERATURE_READ_INTERVAL_MS 20000
#endif

// Payload configuration (defaults are placeholders; override in config_local.h)
#ifndef SENSOR_LOCATION
#define SENSOR_LOCATION "your-location"
#endif
#ifndef INFORMATIONAL
#define INFORMATIONAL false
#endif

#endif
