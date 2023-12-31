# Holman WS5029 Weather Station Setup

![Weather Station](image/holman.png)

## Required Hardware

* RTL Software Defined Radio (https://www.rtl-sdr.com/)
* Holman (or othe RTL433 supported ISM band RF weather station)

## Required Software

* rtl\_433 - https://github.com/merbanan/rtl_433

# Running RTL433

rtl\_433 supports pushing any discovered data to MQTT; 

```
/usr/bin/rtl_433 -f 917M -F mqtt://mqtt-server-ip 1883
```

This will publish all decoded packets from the specified frequency.  The WS5029 weather station uses the 915Mhz ISM band (917Mhz frequency).

```
rtl_433/miranda/events {"time":"2023-10-17 12:45:04","model":"Holman-WS5029","id":60120,"temperature_C":18.2,"humidity":66,"rain_mm":537.20001,"wind_avg_km_h":0,"wind_dir_deg":45}
rtl_433/miranda/devices/Holman-WS5029/60120/time 2023-10-17 12:45:04
rtl_433/miranda/devices/Holman-WS5029/60120/id 60120
rtl_433/miranda/devices/Holman-WS5029/60120/temperature_C 18.2
rtl_433/miranda/devices/Holman-WS5029/60120/humidity 66
rtl_433/miranda/devices/Holman-WS5029/60120/rain_mm 537.20001
rtl_433/miranda/devices/Holman-WS5029/60120/wind_avg_km_h 0
rtl_433/miranda/devices/Holman-WS5029/60120/wind_dir_deg 45
```

This weather station only reports it's rainfall total.  WeeWX handles converting this into rain per hour, 24 hours and historic totals.

