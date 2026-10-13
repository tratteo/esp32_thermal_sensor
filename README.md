# ESP32 Thermal Sensor

Battery-friendly ESP32 firmware that reads a DS18B20 temperature sensor, sends one reading over WiFi (or serial), then deep-sleeps. Each wake-up takes a fresh reading and posts it to a configurable HTTP endpoint.

- Board: `esp32dev` (Espressif 32)
- Framework: Arduino via PlatformIO
- Sensor: DS18B20 on a OneWire bus

## Features

- Single-shot measurement per wake cycle, then deep sleep
- Configurable sleep interval (default 20 s), which keeps the radio and CPU off between readings
- WiFi transport that POSTs a JSON payload to an HTTP endpoint
- Optional serial transport for local debugging
- Deployment secrets kept out of the repository (local config file, gitignored)

## Hardware

- ESP32 development board
- DS18B20 temperature sensor (or a compatible Dallas/Maxim 1-Wire device)
- 4.7 kOhm pull-up resistor between the data line and 3.3 V
- Wiring: DS18B20 data to GPIO 4 (`ONE_WIRE_BUS_PIN`), VCC to 3.3 V, GND to GND

## Requirements

- [PlatformIO](https://platformio.org/) (CLI or the VS Code extension)
- Python 3 with Flask, only if you want to run the bundled mock server

## Configuration

Configuration is split in two layers, following the `.env` / `.env.example` pattern:

| File | Committed | Holds |
| --- | --- | --- |
| `lib/config.h` | Yes | Structural defaults and placeholder values |
| `lib/config_local.example.h` | Yes | Template you copy from |
| `lib/config_local.h` | No (gitignored) | Your real credentials and deployment values |

### 1. Create your local config

```bash
cp lib/config_local.example.h lib/config_local.h
```

Then edit `lib/config_local.h` and set your WiFi credentials, server URL, and location. This file is gitignored, so it never reaches the repository.

### 2. Adjust defaults if needed

Non-secret settings live in `lib/config.h` and can also be overridden in `lib/config_local.h`:

| Option | Default | Description |
| --- | --- | --- |
| `COM_MODE` | `COM_MODE_WIFI` | Transport to use: `COM_MODE_WIFI` or `COM_MODE_SERIAL` |
| `WIFI_SSID` | placeholder | WiFi network name |
| `WIFI_PASSWORD` | placeholder | WiFi password |
| `WIFI_SERVER_URL` | placeholder | Full URL of the temperature endpoint |
| `WIFI_CONNECT_TIMEOUT` | `20000` | WiFi connection timeout in milliseconds |
| `ONE_WIRE_BUS_PIN` | `4` | GPIO connected to the DS18B20 data line |
| `TEMPERATURE_READ_INTERVAL_MS` | `20000` | Deep-sleep duration between readings, in milliseconds |
| `SENSOR_LOCATION` | placeholder | Free-text location sent in the payload |
| `INFORMATIONAL` | `false` | Flag forwarded in the payload for downstream filtering |

Any value defined in `lib/config_local.h` overrides the default in `lib/config.h`.

## Build, flash, and monitor

```bash
pio run              # compile
pio run -t upload    # compile and flash
pio device monitor   # serial monitor at 115200 baud
```

The board spends most of its time in deep sleep. To reflash while it is sleeping, either hold BOOT and reset the board, or plug it in and flash immediately after power-on.

## Payload

Every reading is sent as JSON with a consistent shape across both transports:

```json
{
  "temperature": 21.5,
  "sensorId": "28FF64A1B2C3D4E5",
  "boardId": "A0:B1:C2:D3:E4:F5",
  "location": "your-location",
  "timestamp": 1710000000,
  "informational": false,
  "com": "wifi"
}
```

| Field | Description |
| --- | --- |
| `temperature` | Temperature in Celsius |
| `sensorId` | 1-Wire device address of the sensor |
| `boardId` | ESP32 MAC address |
| `location` | Value of `SENSOR_LOCATION` |
| `timestamp` | Unix epoch time from NTP |
| `informational` | Value of `INFORMATIONAL` |
| `com` | `"wifi"` or `"serial"` |

## Test locally

A small mock server is included to receive and print payloads:

```bash
pip install flask
python wifi_test.py
```

It listens on `0.0.0.0:5050` and exposes `POST /api/temperature`. Point `WIFI_SERVER_URL` at it, for example `http://<your-host>:5050/api/temperature`.

## Troubleshooting

- Server unreachable: `WIFI_SERVER_URL` may use an mDNS hostname such as `tratpi.local`. The host must be resolvable on the same network, or use its IP address.
- WiFi connection fails: increase `WIFI_CONNECT_TIMEOUT`, and confirm the SSID and password in `lib/config_local.h`.
- Use WiFi mode. The optional serial transport is not fully wired up yet.

## For AI agents

See [AGENTS.md](AGENTS.md) for commands, architecture, and known gotchas. This README targets end users; AGENTS.md targets automated agents and contributors.
