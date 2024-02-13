#pragma once

#include <utility>
#include <vector>

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "esphome/core/optional.h"
#include "esphome/components/sensor/sensor.h"



typedef enum {
  MIXIN1,  // direct line 1
  MIXIN2,  // direct line 2
  MIXRES,  // reserverd es8388
  MIXADC   // Select from ADC/ALC
} mixsel_t;

typedef enum {
  OUT1,    // Select Line OUT L/R 1
  OUT2,    // Select Line OUT L/R 2
  OUTALL,  // Enable ALL
} outsel_t;

typedef enum {
  IN1,      // Select Line IN L/R 1
  IN2,      // Select Line IN L/R 2
  IN1DIFF,  // differential IN L/R 1
  IN2DIFF   // differential IN L/R 2
} insel_t;

typedef enum {
  DACOUT,     // Select Sink From DAC
  SRCSELOUT,  // Select Sink From SourceSelect()
  MIXALL,     // Sink ALL DAC & SourceSelect()
} mixercontrol_t;

typedef enum {
  DISABLE,  // Disable ALC
  GENERIC,  // Generic Mode
  VOICE,    // Voice Mode
  MUSIC     // Music Mode
} alcmodesel_t;


namespace esphome {
namespace es8388 {



#define ES8388_ADDR 0x10
/* ES8388 register */
#define ES8388_CONTROL1 0x00
#define ES8388_CONTROL2 0x01
#define ES8388_CHIPPOWER 0x02
#define ES8388_ADCPOWER 0x03
#define ES8388_DACPOWER 0x04
#define ES8388_CHIPLOPOW1 0x05
#define ES8388_CHIPLOPOW2 0x06
#define ES8388_ANAVOLMANAG 0x07
#define ES8388_MASTERMODE 0x08
/* ADC */
#define ES8388_ADCCONTROL1 0x09
#define ES8388_ADCCONTROL2 0x0a
#define ES8388_ADCCONTROL3 0x0b
#define ES8388_ADCCONTROL4 0x0c
#define ES8388_ADCCONTROL5 0x0d
#define ES8388_ADCCONTROL6 0x0e
#define ES8388_ADCCONTROL7 0x0f
#define ES8388_ADCCONTROL8 0x10
#define ES8388_ADCCONTROL9 0x11
#define ES8388_ADCCONTROL10 0x12
#define ES8388_ADCCONTROL11 0x13
#define ES8388_ADCCONTROL12 0x14
#define ES8388_ADCCONTROL13 0x15
#define ES8388_ADCCONTROL14 0x16
/* DAC */
#define ES8388_DACCONTROL1 0x17
#define ES8388_DACCONTROL2 0x18
#define ES8388_DACCONTROL3 0x19
#define ES8388_DACCONTROL4 0x1a
#define ES8388_DACCONTROL5 0x1b
#define ES8388_DACCONTROL6 0x1c
#define ES8388_DACCONTROL7 0x1d
#define ES8388_DACCONTROL8 0x1e
#define ES8388_DACCONTROL9 0x1f
#define ES8388_DACCONTROL10 0x20
#define ES8388_DACCONTROL11 0x21
#define ES8388_DACCONTROL12 0x22
#define ES8388_DACCONTROL13 0x23
#define ES8388_DACCONTROL14 0x24
#define ES8388_DACCONTROL15 0x25
#define ES8388_DACCONTROL16 0x26
#define ES8388_DACCONTROL17 0x27
#define ES8388_DACCONTROL18 0x28
#define ES8388_DACCONTROL19 0x29
#define ES8388_DACCONTROL20 0x2a
#define ES8388_DACCONTROL21 0x2b
#define ES8388_DACCONTROL22 0x2c
#define ES8388_DACCONTROL23 0x2d
#define ES8388_DACCONTROL24 0x2e
#define ES8388_DACCONTROL25 0x2f
#define ES8388_DACCONTROL26 0x30
#define ES8388_DACCONTROL27 0x31
#define ES8388_DACCONTROL28 0x32
#define ES8388_DACCONTROL29 0x33
#define ES8388_DACCONTROL30 0x34

enum class InputChannels : std::uint8_t { INPUT1 = 0, INPUT2 = 1, RESERVED = 2, DIFFERENTAL = 3 };

class ES8388Component : public PollingComponent, public i2c::I2CDevice {
 public:
  void loop() override;
  void update() override;

  InputChannels LINSEL_{InputChannels::INPUT2};

  sensor::Sensor *output_volume_sensor_{nullptr};
  sensor::Sensor *input_gain_left_sensor_{nullptr};
  sensor::Sensor *input_gain_right_sensor_{nullptr};

  bool _ready = false;
  uint8_t _vol = 32;

  outsel_t _outSel = OUTALL;
  insel_t _inSel = IN1;

  void setup() override;

  float get_setup_priority() const override { return setup_priority::LATE - 1; }

  bool setOutputVolume();
  bool setOutputVolume(uint8_t vol);
  bool setInputChannels(InputChannels one);
  bool setInputChannels(InputChannels left, InputChannels right);

  uint8_t getOutputVolume();
  uint8_t getInputGain();

  void mode_voice_recording();
  void mode_default();

  void set_output_volume_sensor(sensor::Sensor *output_volume_sensor) { output_volume_sensor_ = output_volume_sensor; }
  void set_input_gain_left_sensor(sensor::Sensor *input_gain_sensor) { input_gain_left_sensor_ = input_gain_sensor; }
  void set_input_gain_right_sensor(sensor::Sensor *input_gain_sensor) { input_gain_right_sensor_ = input_gain_sensor; }
};

template<typename... Ts> class ES8388ConfigureAction : public Action<Ts...> {
 public:
  ES8388ConfigureAction(ES8388Component *parent) : parent_(parent) {}
  TEMPLATABLE_VALUE(uint8_t, volume)
  TEMPLATABLE_VALUE(InputChannels, input_channel)

  void play(Ts... x) { 
     this->parent_->setOutputVolume(this->volume_.value(x...)); 
     this->parent_->setInputChannels(this->input_channel_.value(x...), this->input_channel_.value(x...));
  }

 protected:
  ES8388Component *parent_;
};


}  // namespace es8388
}  // namespace esphome
