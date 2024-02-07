import esphome.codegen as cg
import esphome.config_validation as cv

from esphome.components import i2c
from esphome.const import CONF_ID

es8388_ns = cg.esphome_ns.namespace("es8388")
CONF_ES8388_ID = "es8388_id"
ES8388Component = es8388_ns.class_("ES8388Component", cg.PollingComponent, i2c.I2CDevice) #, cg.PollingComponent)

CONFIG_SCHEMA = cv.All(
    cv.Schema(
       {
          cv.GenerateID(): cv.declare_id(ES8388Component),
       }
    )
    .extend(i2c.i2c_device_schema(0x10))
    .extend(cv.COMPONENT_SCHEMA)
    .extend(cv.polling_component_schema("60s")),
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
