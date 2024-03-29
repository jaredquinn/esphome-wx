#include "es8388_component.h"
#include "esphome/core/hal.h"

#include <soc/io_mux_reg.h>

static const char *const TAG = "es8388";


namespace esphome {
namespace es8388 {


void ES8388Component::update() {
  if (this->output_volume_sensor_ != nullptr) {
    this->output_volume_sensor_->publish_state(getOutputVolume());
  }

  if (this->input_gain_left_sensor_ != nullptr ||
      this->input_gain_right_sensor_ != nullptr) {
     uint8_t data, left, right;
     data = getInputGain();
     ESP_LOGD(TAG, "getInputGain=%d", data);

     if (this->input_gain_left_sensor_ != nullptr) {
       left = data & 0x0f;
       this->input_gain_left_sensor_->publish_state(left*3);
     }
     if (this->input_gain_right_sensor_ != nullptr) {
       right = data >> 4 & 0x0f;
       this->input_gain_right_sensor_->publish_state(right*3);
    }
  }
}


void ES8388Component::loop() {
}

bool ES8388Component::setOutputVolume() {
  if (!_ready) return false;
  bool res = true;
  if (_outSel == OUTALL || _outSel == OUT1) {
    ESP_LOGD(TAG, "Setting OUT1 output volume to %d", _vol);
    res &= this->write_byte(ES8388_DACCONTROL24, _vol);  // LOUT1VOL
    res &= this->write_byte(ES8388_DACCONTROL25, _vol);  // ROUT1VOL
  } else if (_outSel == OUTALL || _outSel == OUT2) {
    ESP_LOGD(TAG, "Setting OUT2 output volume to %d", _vol);
    res &= this->write_byte(ES8388_DACCONTROL26, _vol);  // LOUT2VOL
    res &= this->write_byte(ES8388_DACCONTROL27, _vol);  // ROUT2VOL
  }
  return res;
}

bool ES8388Component::setOutputVolume(uint8_t vol) {
  _vol = vol;
  if (_vol > 33) _vol = 33;
  return setOutputVolume();
}

bool ES8388Component::setInputChannels(InputChannels one) {
  return setInputChannels(one,one);
}

bool ES8388Component::setInputChannels(InputChannels left, InputChannels right) {
  bool res = true;
  uint8_t val, lc, rc = 0;
  lc = (uint8_t)left;
  rc = (uint8_t)right;
  val = lc << 6 | rc << 4 | 0 & 0x03;
  ESP_LOGD(TAG, "SetInputChannels setting ADCCONTROL2 to %d", val);
  res &= this->write_byte(ES8388_ADCCONTROL2, val);  // ROUT2VOL
  return res;
}

uint8_t ES8388Component::getOutputVolume() {
  uint8_t data;
  this->read_byte(ES8388_DACCONTROL24, &data);
  return data;
}

uint8_t ES8388Component::getInputGain() {
  uint8_t data;
  this->read_byte(ES8388_ADCCONTROL1, &data);
  return data;
}

void ES8388Component::mode_default() {
  this->write_byte(0x09, 0x88); // ADC amp 24dB
}

void ES8388Component::mode_voice_recording() {
  ESP_LOGCONFIG(TAG, "Setting voice recording mode");
  this->write_byte(ES8388_ADCCONTROL1, 0x77); //  +21dB : recommended value for ALC & voice recording
  // ALC Config (as recommended by ES8388 user guide for voice recording)
  this->write_byte(ES8388_ADCCONTROL10, 0xe2); // Reg 0x12 = 0xe2 (ALC enable, PGA Max. Gain=23.5dB, Min. Gain=0dB)
  this->write_byte(ES8388_ADCCONTROL11, 0xa0); // Reg 0x13 = 0xa0 (ALC Target=-1.5dB, ALC Hold time =0 mS)
  this->write_byte(ES8388_ADCCONTROL12, 0x12); // Reg 0x14 = 0x12(Decay time =820uS , Attack time = 416 uS)
  this->write_byte(ES8388_ADCCONTROL13, 0x06); // Reg 0x15 = 0x06(ALC mode)
  this->write_byte(ES8388_ADCCONTROL14, 0xc3); // Reg 0x16 = 0xc3(nose gate = -40.5dB, NGG = 0x01(mute ADC))
}

void ES8388Component::setup() {
  ESP_LOGCONFIG(TAG, "Performing Setup");
  PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0_CLK_OUT1);
  WRITE_PERI_REG(PIN_CTRL, READ_PERI_REG(PIN_CTRL) & 0xFFFFFFF0);

  this->write_byte(ES8388_DACCONTROL3, 0x04); // mute
  this->write_byte(ES8388_MASTERMODE, 0x00); // worker mode
  this->write_byte(ES8388_CHIPPOWER, 0xFF);  // Power down DEM/STM
  this->write_byte(ES8388_CONTROL1, 0x05);   // Set Chip to Play&Record Mode
  this->write_byte(ES8388_CONTROL2, 0x40);   // Power Up Analog and Ibias

  this->write_byte(ES8388_DACPOWER, 0x3C);  // Power Up DAC& enable Lout/Rout
  //this->write_byte(0x00, 0x12); // vmidsel/500k ADC/DAC idem

  this->write_byte(ES8388_DACCONTROL1, 0x18); // SFI setting (i2s mode/16 bit)
  this->write_byte(ES8388_DACCONTROL2, 0x02); //  DAC MCLK/LCRK ratio (256)

  this->write_byte(0x26, 0x00); // LIN2/RIN2 for mixer
  this->write_byte(0x27, 0x90); // left DAC to left mixer
  this->write_byte(0x2A, 0x90); // right DAC to right mixer

  this->write_byte(ES8388_DACCONTROL21, 0x80); // DACLRC ADCLRC idem
  this->write_byte(ES8388_DACCONTROL23, 0x00);
  // DAC volume max
  this->write_byte(0x1B, 0x00);
  this->write_byte(0x1A, 0x00);

  this->write_byte(ES8388_ADCPOWER, 0xFF);    // ADC poweroff
  //this->write_byte(ES8388_ADCCONTROL2, 0x50); // MIC2
  this->write_byte(ES8388_ADCCONTROL3, 0x02); // ADC mono left
  this->write_byte(ES8388_ADCCONTROL4, 0x0D); // / Left/Right data, Left/Right justified mode, Bits length, I2S format
  this->write_byte(ES8388_ADCCONTROL5, 0x02); // ADCFsMode,singel SPEED,RATIO=256
  this->write_byte(0x10, 0x00); // ADC Volume
  this->write_byte(0x11, 0x00);
  this->write_byte(0x03, 0x09); // ALC OFF
  this->write_byte(0x2B, 0x80);

  this->write_byte(0x02, 0xF0);
  delay(1);
  this->write_byte(0x02, 0x00);
  this->write_byte(0x04, 0x30); // DAC power-up LOUT1/ROUT1 enabled
  this->write_byte(0x03, 0x00);
  this->write_byte(0x2E, 0x1C); // DAC volume max
  this->write_byte(0x2F, 0x1C);

  this->write_byte(ES8388_CHIPPOWER, 0x00); // power up chip
  this->write_byte(ES8388_ADCPOWER, 0x09); //  //Power on ADC, Enable LIN&RIN, Power off MICBIAS, set int1lp to low power mode
  this->write_byte(ES8388_DACCONTROL3, 0x00); // unmute DAC

  _ready = true;

  mode_voice_recording();
  setOutputVolume(); 
}
}  // namespace es8388
}  // namespace esphome
