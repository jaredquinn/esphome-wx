import esphome.codegen as cg
import esphome.config_validation as cv

from esphome import automation
from esphome.automation import maybe_simple_id

from esphome.components import i2c
from esphome.const import CONF_ID

DEPENDENCIES = ["i2c"]


CONF_ES8388_ID = "es8388_id"
CONF_OUTPUT_VOLUME = "output_volume"

es8388_ns = cg.esphome_ns.namespace("es8388")
ES8388Component = es8388_ns.class_("ES8388Component", cg.PollingComponent, i2c.I2CDevice) #, cg.PollingComponent)

ES8388ConfigureAction = es8388_ns.class_("ES8388ConfigureAction", automation.Action)

CONFIG_SCHEMA_VOLUME = cv.Schema({
    cv.Optional(CONF_OUTPUT_VOLUME, default=32): cv.int_range(0, 32),
})

CONFIG_SCHEMA = cv.All(
    cv.Schema(
       {
          cv.GenerateID(): cv.declare_id(ES8388Component),
       }
    )
    .extend(CONFIG_SCHEMA_VOLUME)
    .extend(i2c.i2c_device_schema(0x10))
    .extend(cv.COMPONENT_SCHEMA)
    .extend(cv.polling_component_schema("60s")),
)
async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    if CONF_OUTPUT_VOLUME in config:
      cg.add(var.setOutputVolume(config[CONF_OUTPUT_VOLUME]))


@automation.register_action(
    "es8388.configure",
    ES8388ConfigureAction,
    cv.Schema(
      {
         cv.GenerateID(): cv.use_id(ES8388Component),
	 cv.Optional(CONF_OUTPUT_VOLUME, default=32): cv.int_range(0, 32),
      }
    ),
)
async def es8388_configure_action_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)

    if CONF_OUTPUT_VOLUME in config:
        template_ = await cg.templatable(config[CONF_OUTPUT_VOLUME], args, cg.uint16)
        cg.add(var.set_volume(template_))

    return var

