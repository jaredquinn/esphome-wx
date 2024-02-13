import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_FREE,
    CONF_FRAGMENTATION,
    CONF_BLOCK,
    CONF_LOOP_TIME,
    ENTITY_CATEGORY_DIAGNOSTIC,
    UNIT_MILLISECOND,
    UNIT_PERCENT,
    UNIT_DECIBEL,
    UNIT_BYTES,
    ICON_COUNTER,
    ICON_TIMER,
)
from . import CONF_ES8388_ID, ES8388Component

DEPENDENCIES = ["es8388"]
CONF_VOLUME = "output_volume"
CONF_GAIN_LEFT = "input_gain_left"
CONF_GAIN_RIGHT = "input_gain_right"

CONFIG_SCHEMA = {
    cv.GenerateID(CONF_ES8388_ID): cv.use_id(ES8388Component),
    cv.Optional(CONF_VOLUME): sensor.sensor_schema(
            unit_of_measurement=UNIT_PERCENT,
            accuracy_decimals=0,
    ),
    cv.Optional(CONF_GAIN_LEFT): sensor.sensor_schema(
            unit_of_measurement=UNIT_DECIBEL,
            accuracy_decimals=0,
    ),
    cv.Optional(CONF_GAIN_RIGHT): sensor.sensor_schema(
            unit_of_measurement=UNIT_DECIBEL,
            accuracy_decimals=0,
    )
}

async def to_code(config):
    es8388_component = await cg.get_variable(config[CONF_ES8388_ID])

    if output_volume_conf := config.get(CONF_VOLUME):
        sens = await sensor.new_sensor(output_volume_conf)
        cg.add(es8388_component.set_output_volume_sensor(sens))

    if input_gain_conf := config.get(CONF_GAIN_LEFT):
        sens = await sensor.new_sensor(input_gain_conf)
        cg.add(es8388_component.set_input_gain_left_sensor(sens))

    if input_gain_conf := config.get(CONF_GAIN_RIGHT):
        sens = await sensor.new_sensor(input_gain_conf)
        cg.add(es8388_component.set_input_gain_right_sensor(sens))


