# ESPHome Weather Station


## Software

My local weather data is managed using WeeWX.

WeeWX is a free, open source, software program, written in Python, which interacts with your weather station to produce graphs, reports, and HTML pages. It can optionally publish to weather sites or web servers. It uses modern software concepts, making it simple, robust, and easy to extend. It includes extensive documentation.  (https://weewx.com)

WeeWX Interacts with Home Assistant using MQTT;  Home Assistant is the primary source of truth for current weather data;  While WeeWX performs conversions, averaging, some forecasting and historic data.

## Sensors/Data

* OpenWeatherMap
* Holman Weather Station (using RTL433 and an RTL-SDR)
* Xiaomi Aqara Temp/Humidity Sensors
* Xiaomi Air Purifier 3H
* ESPhome BME/Dallas OneWire/SHT Sensors
* ESPhome SGP30 Gas Sensors

More information on how these sensors and data are collected in Home Assistant and WeeWX is beyond the scope of building this display; but further details will be added soon.

# Building the Display

## Hardware Required

* ESP32 (example uses a DOIT DevKit V1)
* ESPhome compatible 240x320 TFT Screen (example uses an Adafruit ILI1934 2.8" TFT Touchscreen)

## Wiring

![Wiring Diagram](wiring.png)

|ESP32|ILI1934|Description|
|-----|-------|-------|
|VCC  |VCC    |+3.3V Supply|
|GND  |GND    |Ground|
|D27  |CS     |TFT SPI Cable Select|
|D25  |RESET  |TFT Reset|
|D26  |DC     |TFT DC|
|D23  |SDI    |TFT SPI MOSI|
|D18  |SCK    |TFT SPI CLK|
|RX2  |LED    |TFT Backlight|
|D19  |SDO    |TFT SPI MISO|
|-----|-------|------------|

* Touchscreen functionality coming soon.

