# ES8388 I2C I2S Mixer

*WORK IN PROGRESS*

This is a work in progress porting the full feature set of the Everest Semi (ES8388 and family) audio mixing ICs for esphome.

## About the ES8388

ES8388 is a high performance, low power and low cost audio CODEC.

It consists of 2-ch ADC, 2-ch DAC, microphone amplifier, headphone amplifier, digital sound effects, and analog mixing and gain functions.

The device uses advanced multi-bit delta-sigma modulation technique to convert data between digital and analog. The multi-bit delta-sigma modulators make the device with low sensitivity to clock jitter and low out of band noise.

For full details of the ES8388 IC see it's [datasheet](http://www.everest-semi.com/pdf/ES8388%20DS.pdf).

## Goals

While this IC works perfectly with ESPhome's i2s subsystems for speaker, microphone and media player the chip support previously consisted of a PR that hard coded some sensible defaults in a setup() routine [found in esphome PR#3552](https://github.com/esphome/esphome/pull/3552).

The ultimate goal of this component is to allow users of this family of chips to control all features available on the I2C bus, which consists of 8 master control registers, 14 ADC control registers and 30 DAC control registers - including:

* Input configuration (IN1/IN2/Balanced)
* Output configuration (OUT1/OUT2/Balanced)
* Output volume control
* Input gain control
* Channel Input select to output mix
* Mixing mode
* I2S configuration modes
* High pass filters
* Auto level configuration
* De-emphesis mode

This chip can be found in [AI Tinker's ESP32-A1S SoC/Audio Module](https://docs.ai-thinker.com/en/esp32-a1s) which includes it in some variants.
It can also be found in various audio developmment boards such as [this one on AliExpress](https://www.aliexpress.com/i/1005003572034387.html) selling for around $20AUD.


## Currently Implemented

Initial version implements two sensors that can be added showing the current volume set for the DAC and the input gain set for the ADC.

Input gain is currently reported as a single 8 bit value with each 4 bits representing a different channel.

## Using the component

From my github:

```
external_components:
  - source: 
      type: git
      url: https://github.com/jaredquinn/esphome-wx/components
    components: [es8388]

es8388:
```

From a local copy:

```
external_components:
  - source: components
    components: [es8388]

es8388:
```

## Configurable Options


|Attribute|Type|Default|Notes|
|---------|----|-------|-----|
|output\_volume|uint\_8|32| |

Example:
```
es8388:
  output_volume: 28

```

## Adding the sensors

```
sensor:
  - platform: es8388
    output_volume:
      name: "Output Volume"
    input_gain:
      name: "Input Gain"
```

## Actions

### configure

Perform reconfiguration with the settings available in _Configurable Options_

Example:

```
  - platform: gpio
    pin:
      number: GPIO18
      mode: input_pullup
      inverted: true
    name: "Key 5"
    filters:
      - delayed_off: 10ms
    on_press:
      - es8388.configure:
          output_volume: 8
```

