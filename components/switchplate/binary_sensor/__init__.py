import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from .. import openHASP_ns, CONF_WIDGET_ID, SwitchPlateItem

WidgetBinarySensor = openHASP_ns.class_(
    "WidgetBinarySensor",  binary_sensor.BinarySensor, cg.Component
)

CONFIG_SCHEMA = (
    binary_sensor.binary_sensor_schema(WidgetBinarySensor)
    .extend(
        {
            cv.Required(CONF_WIDGET_ID): cv.use_id(SwitchPlateItem),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    var = await binary_sensor.new_binary_sensor(config)
    await cg.register_component(var, config)

    widget_ = await cg.get_variable(config[CONF_WIDGET_ID])
    cg.add(var.set_widget(widget_))