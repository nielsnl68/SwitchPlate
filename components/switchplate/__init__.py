import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import core, automation
from esphome.automation import maybe_simple_id
from esphome.const import (
    CONF_ID, 
    CONF_PAGES, 
    CONF_PAGE_ID,  
    CONF_ROTATION, 
    CONF_FROM, 
    CONF_TO,  
    CONF_TRIGGER_ID, 
    CONF_VISIBLE, 
    CONF_TYPE, 
    CONF_NAME, 
    CONF_MIN_VALUE, 
    CONF_MAX_VALUE,
    CONF_VALUE
    )
from esphome.core import coroutine_with_priority

CODEOWNERS = ["@fvanroie","@nielsnl68"]

#AUTO_LOAD = ['sensor', 'text_sensor', 'binary_sensor']

CONFLICTS_WITH = ['display']
MULTI_CONF = True

CONF_ON_PAGE_CHANGE = "on_page_change"
CONF_OBJECT_ID = 'object_id'
CONF_DISPLAY_DEFINE = 'display'
CONF_HEADER = 'header'
CONF_FOOTER = 'footer'

WIDGET_BUTTON = 'button' #    Binary	Button
WIDGET_SWITCH = 'switch' #	Toggle	Switch
WIDGET_CHECKBOX = 'checkbox' #	Toggle	Checkbox
WIDGET_LABEL = 'label' #	Visual	Label
WIDGET_LED = 'led' #	Visual	LED
WIDGET_SPINNER = 'spinner' #	Visual	Spinner
WIDGET_FRAME = 'object' #	Visual	Base Object
WIDGET_LINE = 'line' # 	Visual	Line
WIDGET_IMG = 'img' # 	Visual	Image
WIDGET_DROPDOWN = 'dropdown' #	Selector	Dropdown List
WIDGET_ROLLER = 'roller' #	Selector	Roller
WIDGET_CPICKER = 'cpicker' #	Selector	Color picker
WIDGET_PROGRESSBAR = 'progressbar' #	Range	Progress Bar
WIDGET_SLIDER = 'slider' #	Range	Slider
WIDGET_ARC = 'arc' #	Range	Arc
WIDGET_LINEMETER = 'linemeter' #	Range	Line Meter
WIDGET_GUAUGE = 'gauge' #	Range	Gauge
WIDGET_GROUPED = 'grouped' # 

WIDGET_BTNMATRIX = 'btnmatrix' #	Selector	Button Matrix
WIDGET_MSGBOX = 'msgbox' # 	Selector	Messagebox

CONF_LEFT = "left"
CONF_TOP = "top"
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


CONF_ALIGN = "align"
CONF_ALIGN_LEFT = "left"
CONF_ALIGN_CENTER = "center"
CONF_ALIGN_RIGHT = "right"

CONF_MODE = "text_mode"
CONF_MODE_EXPAND = "expand"
CONF_MODE_BREAK = "break"
CONF_MODE_DOTS = "dots"
CONF_MODE_SCROLL = "scroll"
CONF_MODE_LOOP = "loop"
CONF_MODE_CROP = "crop"


openHASP_ns = cg.esphome_ns.namespace('switch_plate')

SwitchPlate = openHASP_ns.class_('SwitchPlate', cg.Component)
SwitchPlateRef = SwitchPlate.operator("ref")

SwitchPlateItem = openHASP_ns.class_('SwitchPlateItem')
SwitchPlateItemRef = SwitchPlateItem.operator("ref")

SwitchPlateGroup = openHASP_ns.class_('SwitchPlateGroup')
SwitchPlateGroupRef = SwitchPlateGroup.operator("ref")

SwitchPlatePage = openHASP_ns.class_("SwitchPlatePage")
SwitchPlatePagePtr = SwitchPlatePage.operator("ptr")




CONF_ROTATIONS = {
    0: openHASP_ns.DISPLAY_ROTATION_0_DEGREES,
    90: openHASP_ns.DISPLAY_ROTATION_90_DEGREES,
    180: openHASP_ns.DISPLAY_ROTATION_180_DEGREES,
    270: openHASP_ns.DISPLAY_ROTATION_270_DEGREES,
}


CONF_ALIGNS = {
    CONF_ALIGN_LEFT : openHASP_ns.CONF_ALIGN_LEFT,
    CONF_ALIGN_CENTER : openHASP_ns.CONF_ALIGN_CENTER,
    CONF_ALIGN_RIGHT : openHASP_ns.CONF_ALIGN_RIGHT
}

CONF_MODES = {
    CONF_MODE_EXPAND : openHASP_ns.CONF_MODE_EXPAND ,
    CONF_MODE_BREAK : openHASP_ns.CONF_MODE_BREAK ,
    CONF_MODE_DOTS : openHASP_ns.CONF_MODE_DOTS ,
    CONF_MODE_SCROLL : openHASP_ns.CONF_MODE_SCROLL ,
    CONF_MODE_LOOP : openHASP_ns.CONF_MODE_LOOP ,
    CONF_MODE_CROP : openHASP_ns.CONF_MODE_CROP ,

}

CONF_WIDGETS = {
    WIDGET_BUTTON, 
    WIDGET_SWITCH,
    WIDGET_CHECKBOX,
    WIDGET_LABEL,
    WIDGET_LED,
    WIDGET_SPINNER,
    WIDGET_FRAME,
    WIDGET_LINE ,
    WIDGET_IMG ,
    WIDGET_DROPDOWN,
    WIDGET_ROLLER,
    WIDGET_CPICKER,
    WIDGET_PROGRESSBAR,
    WIDGET_SLIDER,
    WIDGET_ARC,
    WIDGET_LINEMETER,
    WIDGET_GUAUGE,
    WIDGET_GROUPED
}


SwitchPlateIsPageCondition  = openHASP_ns.class_( "SwitchPlateIsPageCondition", automation.Action)
SwitchPlateShowNextAction   = openHASP_ns.class_( "SwitchPlateShowNextAction", automation.Action)
SwitchPlateShowPrevAction   = openHASP_ns.class_( "SwitchPlateShowPrevAction", automation.Action)
SwitchPlateOnPageChangeTrigger = openHASP_ns.class_( "SwitchPlateOnPageChangeTrigger", automation.Trigger)



def validate_rotation(value):
    value = cv.string(value)
    if value.endswith("°"):
        value = value[:-1]
    return cv.enum(CONF_ROTATIONS, int=True)(value)


def validate_min_max(config):
    if config[CONF_MAX_VALUE] <= config[CONF_MIN_VALUE]:
        raise cv.Invalid("max_value must be greater than min_value")
    return config

# Use a simple indirection to circumvent the recursion limitation
def switchplate_item_schema(value):
    return SWITCHPLATE_ITEM_SCHEMA(value)


SWITCHPLATE_ITEM_COMMON_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(SwitchPlateItem),
        cv.Optional(CONF_LEFT, default=0):cv.int_range(0,512),
        cv.Optional(CONF_TOP, default=0):cv.int_range(0,512),
        cv.Optional(CONF_WIDTH, default=0):cv.int_range(0,512),
        cv.Optional(CONF_HEIGHT, default=0):cv.int_range(0,512),
        cv.Optional(CONF_VISIBLE, default= True):cv.boolean,
        cv.Optional(CONF_ENABLED, default= True):cv.boolean,
    }
)

SWITCHPLATE_ITEM_MINMAX_SCHEMA = SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
    {
        cv.Optional(CONF_MIN_VALUE, default=0):cv.uint16_t,
        cv.Optional(CONF_MAX_VALUE, default=100):cv.uint16_t,
        cv.Optional(CONF_VALUE, default=0):cv.uint16_t,
    }
   #,validate_min_max
) 

SWITCHPLATE_ITEM_TEXT_SCHEMA = SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
    {
        cv.Optional(CONF_TEXT,"Text"): cv.templatable(cv.string),
        cv.Optional(CONF_MODE,default="crop"): cv.enum(CONF_MODES),
        cv.Optional(CONF_ALIGN,default="left"):cv.enum(CONF_ALIGNS)
    }
)



SWITCHPLATE_ITEM_SCHEMA = cv.All(
    cv.typed_schema(
        {
            WIDGET_FRAME: SWITCHPLATE_ITEM_COMMON_SCHEMA,
            WIDGET_LINE: SWITCHPLATE_ITEM_COMMON_SCHEMA,
            WIDGET_GROUPED: cv.Schema(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(SwitchPlateGroup),
                    cv.Optional(CONF_VISIBLE, default= True):cv.boolean,
                    cv.Optional(CONF_ENABLED, default= True):cv.boolean,
                    cv.Required(CONF_OBJECTS): cv.All(
                        cv.ensure_list(switchplate_item_schema), cv.Length(min=1)
                    ),            
                }
            ),
            WIDGET_LED: SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
                {
                    cv.Optional(CONF_VALUE, default=False): cv.boolean,
                }
            ),
            WIDGET_LABEL: SWITCHPLATE_ITEM_TEXT_SCHEMA,
            WIDGET_BUTTON: SWITCHPLATE_ITEM_TEXT_SCHEMA.extend(
                {
                    cv.Optional(CONF_TOGGLE, default=False): cv.boolean,
                    cv.Optional(CONF_VALUE, default=False): cv.boolean,
                }
            ),
            WIDGET_SWITCH: SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
                {
                    cv.Optional(CONF_VALUE, default=False): cv.boolean,
                    #cv.Optional(CONF_ON_TEXT, default="On"): cv.string_strict,
                    #cv.Optional(CONF_OFF_TEXT, default="Off"): cv.string_strict,
                 
                }
            ),
            WIDGET_CHECKBOX: SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
                {
                    cv.Optional(CONF_TEXT,"Checkbox"): cv.templatable(cv.string),
                    cv.Optional(CONF_VALUE, default=False): cv.boolean,
                    #cv.Optional(CONF_FORMAT, default="%.1f"): cv.string_strict,
                    #cv.Optional(CONF_VALUE_LAMBDA): cv.returning_lambda,
                    #cv.Optional(CONF_ON_TEXT, default="On"): cv.string_strict,
                    #cv.Optional(CONF_OFF_TEXT, default="Off"): cv.string_strict,
                }
            ),

            WIDGET_SPINNER: SWITCHPLATE_ITEM_COMMON_SCHEMA,

            WIDGET_PROGRESSBAR: SWITCHPLATE_ITEM_COMMON_SCHEMA,
            WIDGET_SLIDER: SWITCHPLATE_ITEM_COMMON_SCHEMA,
            WIDGET_LINEMETER: SWITCHPLATE_ITEM_COMMON_SCHEMA,
            WIDGET_ARC: SWITCHPLATE_ITEM_COMMON_SCHEMA,
            WIDGET_GUAUGE: SWITCHPLATE_ITEM_COMMON_SCHEMA,

        },
        default_type="label",
        lower=True,
    ),
    #validate_menu_item,
)

            #.extend({
                    
                    #cv.Optional(CONF_IMMEDIATE_EDIT, default=False): cv.boolean,
                    #cv.Optional(CONF_VALUE_LAMBDA): cv.returning_lambda,
                    #cv.Optional(CONF_ON_NEXT): automation.validate_automation(
                    #    {
                    #        cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(
                    #            DisplayMenuOnNextTrigger
                    #        ),
                    #    }
                    #),
                    #cv.Optional(CONF_ON_PREV): automation.validate_automation(
                    #    {
                    #        cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(
                    #            DisplayMenuOnPrevTrigger
                    #        ),
                    #    }
                    #),
             #   },
            #),



CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(SwitchPlate),
    cv.Required(CONF_DISPLAY_DEFINE): cv.string_strict,
    cv.Optional(CONF_ROTATION): validate_rotation,
    cv.Required(CONF_PAGES): cv.All(
        cv.ensure_list(
            {
                cv.GenerateID(): cv.declare_id(SwitchPlatePage),
                cv.Required(CONF_OBJECTS): cv.All(
                    cv.ensure_list(switchplate_item_schema), cv.Length(min=1)
                ),            
            }
        ),
        cv.Length(min=1),
    ),
    cv.Optional(CONF_ON_PAGE_CHANGE): automation.validate_automation(
        {
            cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(
                SwitchPlateOnPageChangeTrigger
            ),
            cv.Optional(CONF_FROM): cv.use_id(SwitchPlatePage),
            cv.Optional(CONF_TO): cv.use_id(SwitchPlatePage),
        }
    ),
}).extend(cv.COMPONENT_SCHEMA)


def to_code(config):
    if CONF_DISPLAY_DEFINE in config:
        cg.add_define("DISPLAY_CONF_NAME", config[CONF_DISPLAY_DEFINE])
    # else:

    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)


