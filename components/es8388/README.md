# ES8388 I2C I2S Mixer

A work in progress porting the full feature set of the Everest Semi (ES8388 and family) audio mixing ICs for esphome.


## Using the component

From my github:

```
external_components:
  - source: 
      type: git
      url: https://github.com/jaredquinn/esphome-wx/components
    components: [es8388]
```

From a local copy:

```
external_components:
  - source: components
    components: [es8388]

es8388:
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


