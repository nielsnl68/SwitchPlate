import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import CONF_RESTORE_MODE, CONF_DURATION
from .. import openHASP_ns, CONF_WIDGET_ID, SwitchPlateItem

WidgetSwitch = openHASP_ns.class_("WidgetSwitch", switch.Switch, cg.Component)

WidgetSwitchRestoreMode = openHASP_ns.enum("WidgetSwitchRestoreMode")
RESTORE_MODES = {
    "RESTORE_FROM_SERVER": WidgetSwitchRestoreMode.WIDGET_SWITCH_RESTORE_FROM_SERVER,
    "RESTORE_DEFAULT_OFF": WidgetSwitchRestoreMode.WIDGET_SWITCH_RESTORE_DEFAULT_OFF,
    "RESTORE_DEFAULT_ON": WidgetSwitchRestoreMode.WIDGET_SWITCH_RESTORE_DEFAULT_ON,
    "ALWAYS_OFF": WidgetSwitchRestoreMode.WIDGET_SWITCH_ALWAYS_OFF,
    "ALWAYS_ON": WidgetSwitchRestoreMode.WIDGET_SWITCH_ALWAYS_ON,
    "RESTORE_INVERTED_DEFAULT_OFF": WidgetSwitchRestoreMode.WIDGET_SWITCH_RESTORE_INVERTED_DEFAULT_OFF,
    "RESTORE_INVERTED_DEFAULT_ON": WidgetSwitchRestoreMode.WIDGET_SWITCH_RESTORE_INVERTED_DEFAULT_ON,
}


CONFIG_SCHEMA = (
    switch.switch_schema(WidgetSwitch, icon="mdi:tablet-dashboard")
    .extend(
        {
            cv.Required(CONF_WIDGET_ID): cv.use_id(SwitchPlateItem),
            cv.Optional(CONF_RESTORE_MODE, default="RESTORE_FROM_SERVER"): cv.enum(
                RESTORE_MODES, upper=True, space="_"
            ),
            cv.Optional(CONF_DURATION): cv.positive_time_period_milliseconds,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)


BridgeClasses = openHASP_ns.enum("BridgeClasses")

async def to_code(config):
    var = await switch.new_switch(config)
    await cg.register_component(var, config)

    widget_ = await cg.get_variable(config[CONF_WIDGET_ID])
    cg.add(var.set_widget(widget_, BridgeClasses.BRIDGE_SWITCH))

    cg.add(var.set_restore_mode(config[CONF_RESTORE_MODE]))
    cg.add(var.set_duration(config.get(CONF_DURATION,0)))
