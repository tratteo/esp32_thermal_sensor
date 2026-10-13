#ifndef CONFIG_LOCAL_H
#define CONFIG_LOCAL_H

// Copy this file to lib/config_local.h and fill in your real values.
// config_local.h is gitignored, so your credentials never reach the repo.
//
//   cp lib/config_local.example.h lib/config_local.h

// WiFi Configuration
#define WIFI_SSID "your-ssid"
#define WIFI_PASSWORD "your-password"
#define WIFI_SERVER_URL "http://your-server.local:5050/api/temperature"

// Payload configuration
#define SENSOR_LOCATION "your-location"

// Optional overrides (defaults live in config.h)
// #define COM_MODE COM_MODE_SERIAL
// #define WIFI_CONNECT_TIMEOUT 20000
// #define ONE_WIRE_BUS_PIN 4
// #define TEMPERATURE_READ_INTERVAL_MS 20000
// #define INFORMATIONAL true

#endif
