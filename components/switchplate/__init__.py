import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import core, automation
from esphome.automation import maybe_simple_id
from esphome.const import (
    CONF_GROUP,
    CONF_ID,
    CONF_PAGES,
    CONF_PAGE_ID,
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
from esphome.components import display, touchscreen, font


#from rgb_color import color
try:
    from . import rgb_color as rc    # "myapp" case
except:
    import rgb_color as rc

# try:
#     from . import styles as sps    # "myapp" case
# except:
#     import styles as sps

CODEOWNERS = ["@nielsnl68"]

AUTO_LOAD = ['sensor', 'text_sensor', 'binary_sensor',
             'touchscreen', 'display', 'font', 'color']

DEPENDENCIES = ['display', 'touchscreen', 'font', 'color']

MULTI_CONF = True

WIDGET_LINE = 'line'  # Visual	Line
WIDGET_IMG = 'img'  # Visual	Image
WIDGET_SPINNER = 'spinner'  # Visual	Spinner
WIDGET_GROUPED = 'grouped'

WIDGET_LED = 'led'  # Visual	LED
WIDGET_LABEL = 'label'  # Visual	Label

WIDGET_BUTTON = 'button'  # Binary	Button
WIDGET_SWITCH = 'switch'  # Toggle	Switch
WIDGET_CHECKBOX = 'checkbox'  # Toggle	Checkbox
WIDGET_DROPDOWN = 'dropdown'  # Selector	Dropdown List
WIDGET_ROLLER = 'roller'  # Selector	Roller
WIDGET_CPICKER = 'cpicker'  # Selector	Color picker

WIDGET_SLIDER = 'slider'  # Range	Slider
WIDGET_PROGRESSBAR = 'progressbar'  # Range	Progress Bar
WIDGET_ARC = 'arc'  # Range	Arc
WIDGET_LINEMETER = 'linemeter'  # Range	Line Meter
WIDGET_GUAUGE = 'gauge'  # Range	Gauge

WIDGET_BTNMATRIX = 'btnmatrix'  # Selector	Button Matrix
WIDGET_MSGBOX = 'msgbox'  # Selector	Messagebox


CONF_ON_PAGE_CHANGE = "on_page_change"

CONF_WIDGET_ID = 'widget_id'
CONF_DISPLAY_DEFINE = 'display_id'
CONF_HEADER = 'header'
CONF_HEADER_HEIGHT = "header_height"
CONF_FOOTER = 'footer'
CONF_FOOTER_HEIGHT = "footer_height"
CONF_TABVIEW = "tabview"

CONF_X = "x"
CONF_Y = "y"
CONF_WIDTH = "width"
CONF_HEIGHT = "height"
CONF_TEXT = "text"

CONF_STATE = "state"
CONF_DISABLED = "disabled"

CONF_SELECTABLE = "selectable"
CONF_SELECTED = "selected"

CONF_CLICKABLE = "clickable"

CONF_FIRSTPAGE = "firstpage"
CONF_STATIC = "static"

CONF_WIDGETS = "widgets"

CONF_VALUE = "value"

CONF_OPTIONS = "options"
CONF_ROWS = "rows"
CONF_INFINITY = "infinite"


CONF_DIRECTION = "direction"
CONF_TEXT_MODE = "mode"
CONF_DEFAULT_FONT = "default_font"
CONF_SPIN_SPEED = "spin_speed"


openHASP_ns = cg.esphome_ns.namespace('switch_plate')

SwitchPlate = openHASP_ns.class_('SwitchPlate', cg.Component)


SwitchPlateItem = openHASP_ns.class_('SwitchPlateItem')
SwitchPlateItemConstPtr = SwitchPlateItem.operator("ptr").operator("const")

SwitchPlateGroup = openHASP_ns.class_('SwitchPlateGroup')

SwitchPlatePage = openHASP_ns.class_("SwitchPlatePage")

SwitchPlateIsPageCondition = openHASP_ns.class_(
    "SwitchPlateIsPageCondition", automation.Action)
SwitchPlateShowNextAction = openHASP_ns.class_(
    "SwitchPlateShowNextAction", automation.Action)
SwitchPlateShowPrevAction = openHASP_ns.class_(
    "SwitchPlateShowPrevAction", automation.Action)
SwitchPlateOnPageChangeTrigger = openHASP_ns.class_(
    "SwitchPlateOnPageChangeTrigger", automation.Trigger)

WidgetMode_ = openHASP_ns # .enum("Mode", is_class=True)
CONF_SPINNER_MODES = {
    "constant": WidgetMode_.SPINNER_MODE_CONSTANT,
    "slowdown": WidgetMode_.SPINNER_MODE_SLOWDOWN,
    "slowdown_stratch": WidgetMode_.SPINNER_MODE_SLOWSTRATCH,
}

WidgetMode_ = openHASP_ns # .enum("Mode", is_class=True)
CONF_MODES = {
    "expand": WidgetMode_.TEXT_MODE_EXPAND,
    "break": WidgetMode_.TEXT_MODE_BREAK,
    "dots": WidgetMode_.TEXT_MODE_DOTS,
    "scroll": WidgetMode_.TEXT_MODE_SCROLL,
    "loop": WidgetMode_.TEXT_MODE_LOOP,
    "crop": WidgetMode_.TEXT_MODE_CROP,
}

Direction_ = openHASP_ns # .enum("Direction", is_class=True)
CONF_DROPDOWN_DIRECTIONS = {
    "DOWN": Direction_.DIRECTION_DOWN,
    "UP": Direction_.DIRECTION_UP,
    "LEFT": Direction_.DIRECTION_LEFT,
    "RIGHT": Direction_.DIRECTION_RIGHT,
}
CONF_SPINNER_DIRECTIONS = {
    "CW": Direction_.DIRECTION_CW,
    "CCW": Direction_.DIRECTION_CCW,
}

CONF_WIDGET_CLASSES = {
    WIDGET_LINE: openHASP_ns.class_('SwitchPlateLine', SwitchPlateItem),
    WIDGET_IMG: openHASP_ns.class_('SwitchPlateImg', SwitchPlateItem),
    WIDGET_SPINNER: openHASP_ns.class_('SwitchPlateSpinner', SwitchPlateItem),

    WIDGET_GROUPED: openHASP_ns.class_('SwitchPlateGroup', SwitchPlateItem),

    WIDGET_LED: openHASP_ns.class_('SwitchPlateLed', SwitchPlateItem),
    WIDGET_LABEL: openHASP_ns.class_('SwitchPlateLabel', SwitchPlateItem),
    WIDGET_BUTTON: openHASP_ns.class_('SwitchPlateButton', SwitchPlateItem),
    WIDGET_SWITCH: openHASP_ns.class_('SwitchPlateSwitch', SwitchPlateItem),
    WIDGET_CHECKBOX: openHASP_ns.class_('SwitchPlateCheckbox', SwitchPlateItem),

    WIDGET_DROPDOWN: openHASP_ns.class_('SwitchPlateDropdown', SwitchPlateItem),
    WIDGET_ROLLER: openHASP_ns.class_('SwitchPlateRoller', SwitchPlateItem),
    WIDGET_CPICKER: openHASP_ns.class_('SwitchPlateColorPicker', SwitchPlateItem),

    WIDGET_PROGRESSBAR: openHASP_ns.class_('SwitchPlateProgressBar', SwitchPlateItem),
    WIDGET_SLIDER: openHASP_ns.class_('SwitchPlateSlider', SwitchPlateItem),
    WIDGET_ARC: openHASP_ns.class_('SwitchPlateArc', SwitchPlateItem),
    WIDGET_LINEMETER: openHASP_ns.class_('SwitchPlateLineMeter', SwitchPlateItem),
    WIDGET_GUAUGE: openHASP_ns.class_('SwitchPlateGauge', SwitchPlateItem),
}


def validate_min_max(config):
    if config[CONF_MAX_VALUE] <= config[CONF_MIN_VALUE]:
        raise cv.Invalid("max_value must be greater than min_value")
    if (not isinstance(config[CONF_VALUE], core.Lambda)):
        if config[CONF_VALUE] < config[CONF_MIN_VALUE]:
            raise cv.Invalid(
                CONF_VALUE+" must be greater or equel than min_value")
        if config[CONF_VALUE] > config[CONF_MAX_VALUE]:
            raise cv.Invalid(
                CONF_VALUE+" must be equel or less than max_value")
    return config

# Use a simple indirection to circumvent the recursion limitation


def switchplate_item_schema(value):
    return SWITCHPLATE_ITEM_SCHEMA(value)

CONF_DIMENSION = 'dimension'
SWITCHPLATE_ITEM_COMMON_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_DIMENSION) : cv.Schema ({
            cv.Optional(CONF_X, default=0): cv.int_range(-1024, 1024),
            cv.Optional(CONF_Y, default=0): cv.int_range(-1024, 1024),
            cv.Optional(CONF_WIDTH, default=-1): cv.int_range(-3, 1024),
            cv.Optional(CONF_HEIGHT, default=-1): cv.int_range(-3, 1024),
        }),
        cv.Optional(CONF_VISIBLE): cv.boolean,
        cv.Optional(CONF_DISABLED): cv.boolean,
    }
)  # .extend(sps.SWITCHPLATE_COMMON_STYLE_SCHEMA)

SWITCHPLATE_ITEM_MINMAX_SCHEMA = SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
    {
        cv.Optional(CONF_MIN_VALUE): cv.int_,
        cv.Optional(CONF_MAX_VALUE): cv.int_,
        cv.Optional(CONF_VALUE): cv.templatable(cv.int_),
    }
).add_extra(validate_min_max)

SWITCHPLATE_ITEM_TEXT_SCHEMA = SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
    {
        cv.Optional(CONF_TEXT): cv.templatable(cv.string),
        cv.Optional(CONF_TEXT_MODE): cv.enum(CONF_MODES),
    }
)  # .extend(sps.SWITCHPLATE_TEXT_STYLE_SCHEMA)


SWITCHPLATE_ITEM_SCHEMA = cv.All(
    cv.typed_schema(
        {

            WIDGET_LINE: SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(CONF_WIDGET_CLASSES[WIDGET_LINE]),
                }
            ),
            WIDGET_SPINNER: SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(CONF_WIDGET_CLASSES[WIDGET_SPINNER]),
                    cv.Optional(CONF_SPIN_SPEED, default=1000): cv.int_,
                    cv.Optional(CONF_DIRECTION): cv.enum(CONF_SPINNER_DIRECTIONS),
                }
            ),
            WIDGET_GROUPED: cv.Schema(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(CONF_WIDGET_CLASSES[WIDGET_GROUPED]),
                    cv.Optional(CONF_VISIBLE): cv.boolean,
                    cv.Optional(CONF_DISABLED): cv.boolean,
                    cv.Required(CONF_WIDGETS): cv.All(
                        cv.ensure_list(switchplate_item_schema),
                        cv.Length(min=1)
                    ),
                }
            ),
            WIDGET_LED: SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(CONF_WIDGET_CLASSES[WIDGET_LED]),
                    cv.Optional(CONF_VALUE): cv.templatable(cv.uint8_t),
                }
            ),
            WIDGET_LABEL: SWITCHPLATE_ITEM_TEXT_SCHEMA.extend(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(CONF_WIDGET_CLASSES[WIDGET_LABEL]),

                }
            ),
            WIDGET_BUTTON: SWITCHPLATE_ITEM_TEXT_SCHEMA.extend(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(CONF_WIDGET_CLASSES[WIDGET_BUTTON]),
                    cv.Optional(CONF_SELECTED, default=False): cv.boolean,
                    cv.Optional(CONF_DISABLED): cv.boolean,
                    cv.Optional(CONF_SELECTABLE): cv.boolean,
                    cv.Optional(CONF_CLICKABLE): cv.boolean,
                }
            ),
            WIDGET_SWITCH: SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(CONF_WIDGET_CLASSES[WIDGET_SWITCH]),
                    cv.Optional(CONF_SELECTED, default=False): cv.boolean,

                    # cv.Optional(CONF_ON_TEXT, default="On"): cv.string_strict,
                    # cv.Optional(CONF_OFF_TEXT, default="Off"): cv.string_strict,
                }
            ),
            WIDGET_CHECKBOX: SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(CONF_WIDGET_CLASSES[WIDGET_CHECKBOX]),
                    cv.Optional(CONF_TEXT, default="Checkbox"): cv.templatable(cv.string),
                    cv.Optional(CONF_STATE, default=False): cv.boolean,
                    # cv.Optional(CONF_FORMAT, default="%.1f"): cv.string_strict,
                    # cv.Optional(CONF_VALUE_LAMBDA): cv.returning_lambda,
                    # cv.Optional(CONF_ON_TEXT, default="On"): cv.string_strict,
                    # cv.Optional(CONF_OFF_TEXT, default="Off"): cv.string_strict,
                }
            ),

            WIDGET_PROGRESSBAR: SWITCHPLATE_ITEM_MINMAX_SCHEMA.extend(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(CONF_WIDGET_CLASSES[WIDGET_PROGRESSBAR]),

                }
            ),
            WIDGET_SLIDER: SWITCHPLATE_ITEM_MINMAX_SCHEMA.extend(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(CONF_WIDGET_CLASSES[WIDGET_SLIDER]),

                }
            ),
            WIDGET_LINEMETER: SWITCHPLATE_ITEM_MINMAX_SCHEMA.extend(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(CONF_WIDGET_CLASSES[WIDGET_LINEMETER]),

                }
            ),
            WIDGET_ARC: SWITCHPLATE_ITEM_MINMAX_SCHEMA.extend(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(CONF_WIDGET_CLASSES[WIDGET_ARC]),

                }
            ),
            WIDGET_GUAUGE: SWITCHPLATE_ITEM_MINMAX_SCHEMA.extend(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(CONF_WIDGET_CLASSES[WIDGET_GUAUGE]),

                }
            ),
        },
        default_type="label",
        lower=True,
    )
)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(SwitchPlate),
    cv.Optional(CONF_DISPLAY_DEFINE): cv.use_id(display.DisplayBuffer),
    cv.Optional(touchscreen.CONF_TOUCHSCREEN_ID): cv.use_id(touchscreen.Touchscreen),
    cv.Required(CONF_DEFAULT_FONT): cv.use_id(font.Font),
    cv.Optional(CONF_TABVIEW): cv.boolean,
    cv.Optional(CONF_HEADER): cv.All(
        cv.ensure_list(switchplate_item_schema), 
        cv.Length(min=1)
    ),
    cv.Optional(CONF_HEADER_HEIGHT, default=23): cv.uint8_t,
    cv.Optional(CONF_FOOTER): cv.All(
        cv.ensure_list(switchplate_item_schema), 
        cv.Length(min=1)
    ),
    cv.Optional(CONF_FOOTER_HEIGHT, default=23): cv.uint8_t,
    cv.Required(CONF_PAGES): cv.All(
        cv.ensure_list(
            {
                cv.GenerateID(): cv.declare_id(SwitchPlatePage),
                cv.Optional(CONF_WIDGETS):  cv.All(
                    cv.ensure_list(switchplate_item_schema), 
                    cv.Length(min=1)
                ),
                cv.Optional(CONF_NAME): cv.string,
                cv.Optional(CONF_SELECTABLE): cv.boolean,
                cv.Optional(CONF_VISIBLE): cv.boolean,
                cv.Optional(CONF_DISABLED): cv.boolean,
            }
        ),
        cv.Length(min=1),
    ),
}).extend(cv.COMPONENT_SCHEMA)#.add_extra(cv.has_exactly_one_key([CONF_DISPLAY_DEFINE, touchscreen.CONF_TOUCHSCREEN_ID]))

WIDGET_STATUS_OPTIONS = {
    CONF_SELECTED: 2,
    CONF_DISABLED: 4,
    CONF_VISIBLE : 9,
    CONF_CLICKABLE: 10,
    CONF_SELECTABLE: 11,
}


async def item_to_code(config):
    item = cg.new_Pvariable(config[CONF_ID])
    for key, value in config.items():
        if (key in [CONF_ID, CONF_TYPE, CONF_X, CONF_Y, CONF_WIDTH, CONF_HEIGHT]):
            pass
        elif (key == CONF_NAME):
            cg.add(item.set_name(config[CONF_NAME]))
        elif (key == CONF_DIMENSION):
            dim = config[CONF_DIMENSION]
            cg.add(item.set_dimension(dim[CONF_X], dim[CONF_Y], dim[CONF_WIDTH], dim[CONF_HEIGHT]))
        elif (key == CONF_TEXT):
            if isinstance(value, core.Lambda):
                template_ = await cg.templatable(value, [(SwitchPlateItemConstPtr, "it")], cg.std_string)
                cg.add(item.set_text(template_))
            else:
                cg.add(item.set_text(config[CONF_TEXT]))
        elif (key == CONF_VALUE):
            if isinstance(value, core.Lambda):
                template_ = await cg.templatable(value, [(SwitchPlateItemConstPtr, "it")], cg.int32)
                cg.add(item.set_state(template_))
            else:
                cg.add(item.set_state(config[CONF_VALUE]))
        elif (key == CONF_STATE):
            cg.add(item.set_state(1 if config[CONF_STATE] else 0))
        elif (key == CONF_VISIBLE):
            cg.add(item.set_visible(value))
        elif (key == CONF_SELECTABLE):
            cg.add(item.set_selectable(config[CONF_SELECTABLE]))
        elif (key == CONF_WIDGETS):
            for c in config[CONF_WIDGETS]:
                sub = await item_to_code(c)
                cg.add(item.add_widget(sub))
        elif (isinstance(value, core.ID)):
            parent = await cg.get_variable(value)
            cg.add(item.set_variable(key, parent))
        elif (key in WIDGET_STATUS_OPTIONS.keys()):
            cg.add(item.set_widget_options(WIDGET_STATUS_OPTIONS[key], value))
        else:
            cg.add(item.set_variable(key, value))
    return item


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    for key, value in config.items():
        if ((key == CONF_ID) or (key == CONF_TYPE)):
            pass
        elif (key == CONF_DISPLAY_DEFINE):
            parent = await cg.get_variable(value)
            cg.add(var.set_display(parent))
        elif (key == touchscreen.CONF_TOUCHSCREEN_ID):
            parent = await cg.get_variable(value)
            cg.add(var.set_touchscreen(parent))
        elif (key == CONF_PAGES):
            for c in config[CONF_PAGES]:
                item = await item_to_code(c)
                cg.add(var.add_page(item))
        elif (key == CONF_HEADER):
            for c in config[CONF_HEADER]:
                item = await item_to_code(c)
                cg.add(var.add_headerItem(item))
        elif (key == CONF_FOOTER):
            for c in config[CONF_FOOTER]:
                item = await item_to_code(c)
                cg.add(var.add_footerItem(item))
        elif (isinstance(value, core.ID)):
            parent = await cg.get_variable(value)
            cg.add(var.set_variable(key, parent))
        else:
            cg.add(var.set_variable(key, value))
