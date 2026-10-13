# AGENTS.md

ESP32 (esp32dev) firmware that reads a DS18B20 temperature sensor, POSTs one reading, then deep-sleeps. PlatformIO + Arduino framework. No tests, no CI, no lint/typecheck.

## Commands

- Build: `pio run`
- Build + flash: `pio run -t upload`
- Serial monitor (115200 baud): `pio device monitor`
- Mock server for the temperature endpoint (receives `POST /api/temperature` on port 5050): `python wifi_test.py`

## Architecture

- `src/main.cpp`: entrypoint. Reads sensor once in `setup()`, sends data, calls `esp_deep_sleep_start()`. `loop()` is never reached.
- `lib/com.h`: `ComInterface` base class plus `WifiCom` and `SerialCom`. Implemented in `src/wifi_com.cpp` and `src/serial_com.cpp`.
- `lib/config.h`: committed configuration and safe defaults (COM_MODE, sensor pin, read interval, server URL). Real credentials/overrides live in `lib/config_local.h` (gitignored), created from `lib/config_local.example.h`; `config.h` picks it up via `__has_include` plus `#ifndef` fallbacks.
- Headers are included with relative paths (`../lib/config.h`), not via PlatformIO library resolution. `.vscode/c_cpp_properties.json` is auto-generated; edit `platformio.ini`, not that file.

## Gotchas

- Never commit credentials. Put them in `lib/config_local.h` (gitignored); `lib/config.h` only carries placeholders. The old WiFi credentials are still in git history and must be treated as leaked (rotate the password).
- Serial mode is broken: `main.cpp` selects it via `#elif CURRENT_COMM_MODE == COMM_MODE_SERIAL`, but those macros are undefined (config.h defines `COM_MODE` and `COM_MODE_SERIAL`). It only works today because `COM_MODE == COM_MODE_WIFI` matches first. Fix the macro names before switching to serial mode.
- `lib/com.h` has a broken include guard: the `#endif` on line 3 closes it before the class body. Safe only because each `.cpp` includes it once; do not add a second include of `com.h` in any translation unit.
- The board deep-sleeps for `TEMPERATURE_READ_INTERVAL_MS` (20s) between reads. To reflash while it sleeps, hold BOOT and reset, or reflash immediately after power-on.
- `WIFI_SERVER_URL` uses the mDNS hostname `tratpi.local`; the target host must be resolvable on the local network.
- `OneWire` is a transitive dependency of DallasTemperature and is not listed in `platformio.ini` `lib_deps`; do not add it manually.
- JSON payload fields must stay consistent across both transports: `temperature`, `sensorId`, `boardId`, `location`, `timestamp`, `informational`, `com` (`"wifi"` vs `"serial"`).
