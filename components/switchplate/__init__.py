import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import core, automation
from esphome.automation import maybe_simple_id
from esphome.const import (
    CONF_GROUP,
    CONF_ID, 
    CONF_PAGES, 
    CONF_PAGE_ID,  
    CONF_ROTATION, 
    CONF_FROM, 
    CONF_TO,  
    CONF_TRIGGER_ID,
    CONF_TYPE, 
    CONF_VISIBLE, 
    CONF_NAME, 
    CONF_MIN_VALUE, 
    CONF_MAX_VALUE,
    CONF_STATE
    )
from esphome.core import coroutine_with_priority
#from rgb_color import color
try:
     from . import rgb_color as rc    # "myapp" case
except:
     import rgb_color as rc

import sys; print(sys.version)


CODEOWNERS = ["@fvanroie","@nielsnl68"]

#AUTO_LOAD = ['sensor', 'text_sensor', 'binary_sensor']

CONFLICTS_WITH = ['display']
MULTI_CONF = True



WIDGET_FRAME = 'frame' #	Visual	Base Object
WIDGET_LINE = 'line' # 	Visual	Line
WIDGET_IMG = 'img' # 	Visual	Image
WIDGET_SPINNER = 'spinner' #	Visual	Spinner
WIDGET_GROUPED = 'grouped' # 

WIDGET_LED = 'led' #	Visual	LED
WIDGET_LABEL = 'label' #	Visual	Label

WIDGET_BUTTON = 'button' #    Binary	Button
WIDGET_SWITCH = 'switch' #	Toggle	Switch
WIDGET_CHECKBOX = 'checkbox' #	Toggle	Checkbox
WIDGET_DROPDOWN = 'dropdown' #	Selector	Dropdown List
WIDGET_ROLLER = 'roller' #	Selector	Roller
WIDGET_CPICKER = 'cpicker' #	Selector	Color picker

WIDGET_SLIDER = 'slider' #	Range	Slider
WIDGET_PROGRESSBAR = 'progressbar' #	Range	Progress Bar
WIDGET_ARC = 'arc' #	Range	Arc
WIDGET_LINEMETER = 'linemeter' #	Range	Line Meter
WIDGET_GUAUGE = 'gauge' #	Range	Gauge

WIDGET_BTNMATRIX = 'btnmatrix' #	Selector	Button Matrix
WIDGET_MSGBOX = 'msgbox' # 	Selector	Messagebox


CONF_ON_PAGE_CHANGE = "on_page_change"
CONF_OBJECT_ID = 'object_id'
CONF_DISPLAY_DEFINE = 'display'
CONF_HEADER = 'header'
CONF_FOOTER = 'footer'
CONF_TABVIEW = "tabview"

CONF_X = "x"
CONF_Y = "y"
CONF_WIDTH = "width"
CONF_HEIGHT = "height"
CONF_TEXT = "text"
CONF_TOGGLE = "troggle"
CONF_STATE = "state"
CONF_TEXT = "text"
CONF_ENABLED = "enabled"

CONF_TOGGLE = "toggle" 

CONF_FIRSTPAGE ="firstpage"
CONF_STATIC = "static"
CONF_SELECTABLE = "selectable"
CONF_OBJECTS = "objects"

CONF_VALUE = "value"

CONF_SPEED = "speed"
CONF_CIRCLE_WIDTH = "circle_width"
CONF_CIRCLE_COLOR = "circle_color"
CONF_SEGMENT_WIDTH = "segment_width"
CONF_SEGMENT_COLOR = "segment_color"

CONF_RADIUS = "radius"

CONF_OPTIONS ="options"
CONF_ROWS = "rows"
CONF_INFINITY = "infinite"

CONF_INDICATPOR_COLOR= "indicator_color"
CONF_KNOP_COLOR = "knop_color"

CONF_ALIGN = "align"
CONF_ALIGN_LEFT = "left"
CONF_ALIGN_CENTER = "center"
CONF_ALIGN_RIGHT = "right"

CONF_MODE = "mode"
CONF_MODE_EXPAND = "expand"
CONF_MODE_BREAK = "break"
CONF_MODE_DOTS = "dots"
CONF_MODE_SCROLL = "scroll"
CONF_MODE_LOOP = "loop"
CONF_MODE_CROP = "crop"
CONF_MODE_CONSTANT = "constant"
CONF_MODE_SLOWDOWN = "slowdown"
CONF_MODE_SLOWSTRATCH = "slowdown-stratch"

CONF_DIRECTION = "direction"
CONF_DIRECTION_DOWN = "down"
CONF_DIRECTION_UP = "up"
CONF_DIRECTION_LEFT = "left"
CONF_DIRECTION_RIGHT = "right"
CONF_DIRECTION_CW = "cw"
CONF_DIRECTION_CCW = "ccw"


openHASP_ns = cg.esphome_ns.namespace('switch_plate')

SwitchPlate = openHASP_ns.class_('SwitchPlate', cg.Component)


SwitchPlateItem = openHASP_ns.class_('SwitchPlateItem')
SwitchPlateItemConstPtr = SwitchPlateItem.operator("ptr").operator("const")

SwitchPlateGroup = openHASP_ns.class_('SwitchPlateGroup')

SwitchPlatePage = openHASP_ns.class_("SwitchPlatePage")

SwitchPlateIsPageCondition  = openHASP_ns.class_( "SwitchPlateIsPageCondition", automation.Action)
SwitchPlateShowNextAction   = openHASP_ns.class_( "SwitchPlateShowNextAction", automation.Action)
SwitchPlateShowPrevAction   = openHASP_ns.class_( "SwitchPlateShowPrevAction", automation.Action)
SwitchPlateOnPageChangeTrigger = openHASP_ns.class_( "SwitchPlateOnPageChangeTrigger", automation.Trigger)




CONF_ROTATIONS = {
    0: openHASP_ns.DISPLAY_ROTATION_0_DEGREES,
    90: openHASP_ns.DISPLAY_ROTATION_90_DEGREES,
    180: openHASP_ns.DISPLAY_ROTATION_180_DEGREES,
    270: openHASP_ns.DISPLAY_ROTATION_270_DEGREES,
}


CONF_ALIGNS = {
    CONF_ALIGN_LEFT : openHASP_ns.TEXT_ALIGN_LEFT,
    CONF_ALIGN_CENTER : openHASP_ns.TEXT_ALIGN_CENTER,
    CONF_ALIGN_RIGHT : openHASP_ns.TEXT_ALIGN_RIGHT
}

CONF_TEXT_MODES = {
    CONF_MODE_EXPAND : openHASP_ns.TEXT_MODE_EXPAND ,
    CONF_MODE_BREAK : openHASP_ns.TEXT_MODE_BREAK ,
    CONF_MODE_DOTS : openHASP_ns.TEXT_MODE_DOTS ,
    CONF_MODE_SCROLL : openHASP_ns.TEXT_MODE_SCROLL ,
    CONF_MODE_LOOP : openHASP_ns.TEXT_MODE_LOOP ,
    CONF_MODE_CROP : openHASP_ns.TEXT_MODE_CROP ,
}

CONF_SPINNER_MODES = {
    CONF_MODE_CONSTANT : openHASP_ns.SPINNER_MODE_CONSTANT,
    CONF_MODE_SLOWDOWN : openHASP_ns.SPINNER_MODE_SLOWDOWN,
    CONF_MODE_SLOWSTRATCH : openHASP_ns.SPINNER_MODE_SLOWSTRATCH,
}

CONF_DROPDOWN_DIRECTIONS = {
    CONF_DIRECTION_DOWN : openHASP_ns.DIRECTION_DOWN,
    CONF_DIRECTION_UP : openHASP_ns.DIRECTION_UP,
    CONF_DIRECTION_LEFT : openHASP_ns.DIRECTION_LEFT,
    CONF_DIRECTION_RIGHT : openHASP_ns.DIRECTION_RIGHT,
}

CONF_SPINNER_DIRECTIONS = {
    CONF_DIRECTION_CW : openHASP_ns.DIRECTION_CW,
    CONF_DIRECTION_CCW : openHASP_ns.DIRECTION_CCW,
}

CONF_WIDGETS = {
    WIDGET_FRAME,
    WIDGET_LINE ,
    WIDGET_IMG ,
    WIDGET_SPINNER,

    WIDGET_GROUPED,

    WIDGET_LED,
    WIDGET_LABEL,
    WIDGET_BUTTON, 
    WIDGET_SWITCH,
    WIDGET_CHECKBOX,

    WIDGET_DROPDOWN,
    WIDGET_ROLLER,
    WIDGET_CPICKER,

    WIDGET_PROGRESSBAR,
    WIDGET_SLIDER,
    WIDGET_ARC,
    WIDGET_LINEMETER,
    WIDGET_GUAUGE,
}



def validate_rotation(value):
    value = cv.string(value)
    if value.endswith("°"):
        value = value[:-1]
    return cv.enum(CONF_ROTATIONS, int=True)(value)


def validate_min_max(config):
    if config[CONF_MAX_VALUE] <= config[CONF_MIN_VALUE]:
        raise cv.Invalid("max_value must be greater than min_value")
    if (not isinstance(config[CONF_VALUE], core.Lambda)):
        if config[CONF_VALUE] < config[CONF_MIN_VALUE]:
            raise cv.Invalid(CONF_VALUE+" must be greater or equel than min_value")
        if config[CONF_VALUE] > config[CONF_MAX_VALUE]:
            raise cv.Invalid(CONF_VALUE+" must be equel or less than max_value")
    return config

# Use a simple indirection to circumvent the recursion limitation
def switchplate_item_schema(value):
    return SWITCHPLATE_ITEM_SCHEMA(value)


SWITCHPLATE_ITEM_COMMON_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(SwitchPlateItem),
        cv.Optional(CONF_X):cv.int_range(-512,512),
        cv.Optional(CONF_Y):cv.int_range(-512,512),
        cv.Optional(CONF_WIDTH):cv.int_range(0,512),
        cv.Optional(CONF_HEIGHT):cv.int_range(0,512),
        cv.Optional(CONF_VISIBLE):cv.boolean,
        cv.Optional(CONF_ENABLED):cv.boolean,
    }
)

SWITCHPLATE_ITEM_MINMAX_SCHEMA = SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
    {
        cv.Optional(CONF_MIN_VALUE):cv.int_,
        cv.Optional(CONF_MAX_VALUE):cv.int_,
        cv.Optional(CONF_VALUE): cv.templatable(cv.int_),        
    }
).add_extra(validate_min_max)

SWITCHPLATE_ITEM_TEXT_SCHEMA = SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
    {
        cv.Optional(CONF_TEXT): cv.templatable(cv.string),
        cv.Optional(CONF_MODE): cv.enum(CONF_TEXT_MODES),
        cv.Optional(CONF_ALIGN):cv.enum(CONF_ALIGNS)
    }
)



SWITCHPLATE_ITEM_SCHEMA = cv.All(
    cv.typed_schema(
        {
            WIDGET_FRAME: SWITCHPLATE_ITEM_COMMON_SCHEMA,
            WIDGET_LINE: SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
                {

                }
            ),
            WIDGET_SPINNER: SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
                {
                    cv.Optional(CONF_SPEED, default=1000): cv.int_, 
                    cv.Optional(CONF_DIRECTION): cv.enum(CONF_SPINNER_DIRECTIONS),
                    cv.Optional(CONF_CIRCLE_WIDTH): cv.int_, 
                    cv.Optional(CONF_CIRCLE_COLOR): rc.color, 
                    cv.Optional(CONF_SEGMENT_WIDTH): cv.int_, 
                    cv.Optional(CONF_SEGMENT_COLOR): rc.color, 
                }
            ),
            WIDGET_GROUPED: cv.Schema(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(SwitchPlateGroup),
                    cv.Optional(CONF_VISIBLE):cv.boolean,
                    cv.Optional(CONF_ENABLED):cv.boolean,
                    cv.Required(CONF_OBJECTS): cv.All(
                        cv.ensure_list(switchplate_item_schema), cv.Length(min=1)
                    ),            
                }
            ),
            WIDGET_LED: SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
                {
                    cv.Optional(CONF_VALUE): cv.templatable(cv.uint8_t),
                }
            ),
            WIDGET_LABEL: SWITCHPLATE_ITEM_TEXT_SCHEMA.extend(
                {

                }
            ),
            WIDGET_BUTTON: SWITCHPLATE_ITEM_TEXT_SCHEMA.extend(
                {
                    cv.Optional(CONF_TOGGLE, default=False): cv.boolean,
                    cv.Optional(CONF_STATE, default=False): cv.boolean,
                }
            ),
            WIDGET_SWITCH: SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
                {
                    cv.Optional(CONF_STATE, default=False): cv.boolean,
                    cv.Optional(CONF_RADIUS): cv.boolean,
                    cv.Optional(CONF_INDICATPOR_COLOR): rc.color,
                    cv.Optional(CONF_KNOP_COLOR): rc.color 
                    #cv.Optional(CONF_ON_TEXT, default="On"): cv.string_strict,
                    #cv.Optional(CONF_OFF_TEXT, default="Off"): cv.string_strict,
                }
            ),
            WIDGET_CHECKBOX: SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
                {
                    cv.Optional(CONF_TEXT, default="Checkbox"): cv.templatable(cv.string),
                    cv.Optional(CONF_STATE, default=False): cv.boolean,
                    #cv.Optional(CONF_FORMAT, default="%.1f"): cv.string_strict,
                    #cv.Optional(CONF_VALUE_LAMBDA): cv.returning_lambda,
                    #cv.Optional(CONF_ON_TEXT, default="On"): cv.string_strict,
                    #cv.Optional(CONF_OFF_TEXT, default="Off"): cv.string_strict,
                }
            ),

            WIDGET_PROGRESSBAR: SWITCHPLATE_ITEM_MINMAX_SCHEMA.extend(
                {

                }
            ),
            WIDGET_SLIDER: SWITCHPLATE_ITEM_MINMAX_SCHEMA.extend(
                {

                }
            ),
            WIDGET_LINEMETER: SWITCHPLATE_ITEM_MINMAX_SCHEMA.extend(
                {

                }
            ),
            WIDGET_ARC: SWITCHPLATE_ITEM_MINMAX_SCHEMA.extend(
                {

                }
            ),
            WIDGET_GUAUGE: SWITCHPLATE_ITEM_MINMAX_SCHEMA.extend(
                {

                }
            ),

        },
        default_type="label",
        lower=True,
    ),
    #validate_menu_item,
)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(SwitchPlate),
    cv.Required(CONF_DISPLAY_DEFINE): cv.string_strict,
    cv.Optional(CONF_ROTATION): validate_rotation,
    cv.Optional(CONF_TABVIEW):cv.boolean,
    cv.Optional(CONF_HEADER):  cv.All(
        cv.ensure_list(switchplate_item_schema), cv.Length(min=1)
    ),
    cv.Optional(CONF_FOOTER):  cv.All(
        cv.ensure_list(switchplate_item_schema), cv.Length(min=1)
    ),
    cv.Required(CONF_PAGES): cv.All(
        cv.ensure_list(
            {
                cv.GenerateID(): cv.declare_id(SwitchPlatePage),
                cv.Required(CONF_OBJECTS): cv.All(
                    cv.ensure_list(switchplate_item_schema), cv.Length(min=1)
                ),
                cv.Optional(CONF_NAME) : cv.string,
                cv.Optional(CONF_SELECTABLE): cv.boolean,
            }
        ),
        cv.Length(min=1),
    ),
    cv.Optional(CONF_ON_PAGE_CHANGE): automation.validate_automation(
        {
            cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id( SwitchPlateOnPageChangeTrigger ),
            cv.Optional(CONF_FROM): cv.use_id(SwitchPlatePage),
            cv.Optional(CONF_TO): cv.use_id(SwitchPlatePage),
        }
    ),
}).extend(cv.COMPONENT_SCHEMA)


async def item_to_code(config):
    item = cg.new_Pvariable(config[CONF_ID], config[CONF_TYPE])
    
    for k, v in config.items():
        
        match k:
            case 'id': pass
            case 'text':
                if isinstance(v, core.Lambda):
                    template_ = await cg.templatable( v, [(SwitchPlateItemConstPtr, "it")], cg.std_string )
                    cg.add(item.text(template_))
                else:
                    cg.add(item.text(config[CONF_TEXT]))
            case 'value':
                if isinstance(v, core.Lambda):
                    template_ = await cg.templatable( v, [(SwitchPlateItemConstPtr, "it")], cg.int32)
                    cg.add(item.value(template_))
                else:
                    cg.add(item.value(config[CONF_VALUE]))
            case 'state':
                cg.add(item.value(1 if config[CONF_STATE] else 0))

            case 'visible':
                cg.add(item.visible(v))
            case _:
                cg.add(item.var(k, v))

    return item


async def group_to_code(parent, config):
    group = cg.new_Pvariable(config[CONF_ID])
    
    if CONF_VISIBLE in config:
        cg.add(group.visible(config[CONF_VISIBLE]))
    if CONF_ENABLED in config:
        cg.add(group.enabled(config[CONF_ENABLED]))
    
    for c in config[CONF_OBJECTS]:
        if (c[CONF_TYPE] == CONF_GROUP):
            item = await group_to_code(c)
        else:
            item = await item_to_code(c)
        cg.add(group.add_object(item))
    return group

async def page_to_code(parent, config):
    page = cg.new_Pvariable(config[CONF_ID])
    cg.add(parent.add_page(page))

    if CONF_NAME in config:
        cg.add(page.name(config[CONF_NAME]))
    if CONF_SELECTABLE in config:
        cg.add(page.selectable(config[CONF_SELECTABLE]))
    if CONF_ENABLED in config:
        cg.add(page.enabled(config[CONF_ENABLED]))
    
    for c in config[CONF_OBJECTS]:
        if (c[CONF_TYPE] == CONF_GROUP):
            item = await group_to_code(c)
        else:
            item = await item_to_code(c)
        cg.add(page.add_object(item))


async def to_code(config):
    if CONF_DISPLAY_DEFINE in config:
        cg.add_define("DISPLAY_CONF_NAME", config[CONF_DISPLAY_DEFINE])
    # else:

    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    if CONF_ROTATION in config:
        cg.add(var.rotation(config[CONF_ROTATION]))
    if CONF_TABVIEW in config:
        cg.add(var.tabview(config[CONF_TABVIEW]))
    for c in config[CONF_PAGES]:
        await page_to_code(var, c)
    if CONF_HEADER in config:
        for c in config[CONF_HEADER]:
            if (c[CONF_TYPE] == CONF_GROUP):
                item = await group_to_code(c)
            else:
                item = await item_to_code(c)
            cg.add(var.add_headerItem(item))

    if CONF_FOOTER in config:
        for c in config[CONF_FOOTER]:
            if (c[CONF_TYPE] == CONF_GROUP):
                item = await group_to_code(c)
            else:
                item = await item_to_code(c)
            cg.add(var.add_footerItem(item))
