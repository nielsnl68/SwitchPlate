from colorsys import hls_to_rgb
from tkinter import TRUE

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation, core
from esphome.components import color, display, font, image, time, touchscreen
from esphome.const import (CONF_FORMAT, CONF_FROM, CONF_GROUP, CONF_ID,
                           CONF_MAX_VALUE, CONF_MIN_VALUE, CONF_PAGE_ID,
                           CONF_PAGES, CONF_TIME_ID, CONF_TO, CONF_TRIGGER_ID,
                           CONF_TYPE, CONF_VISIBLE)
from esphome.core import coroutine_with_priority, HexInt

#from . import styles as style    # "myapp" case

CODEOWNERS = ["@nielsnl68"]

AUTO_LOAD = [
    "sensor",
    "text_sensor",
    "binary_sensor",
    "touchscreen",
    "display",
    "font",
    "color",
]

DEPENDENCIES = ["display", "touchscreen", "font", "color"]

MULTI_CONF = True

WIDGET_LINE = "line"  # Visual	Line
WIDGET_IMAGE = "image"  # Visual	Image
WIDGET_SPINNER = "spinner"  # Visual	Spinner
WIDGET_PANEL = "panel"

WIDGET_LED = "led"  # Visual	LED
WIDGET_LABEL = "label"  # Visual	Label
WIDGET_DATETIME = "datetime"
WIDGET_PAGETITLE = "pagetitle"
WIDGET_BUTTON = "button"  # Binary	Button
WIDGET_SWITCH = "switch"  # Toggle	Switch
WIDGET_CHECKBOX = "checkbox"  # Toggle	Checkbox
WIDGET_DROPDOWN = "dropdown"  # Selector	Dropdown List
WIDGET_ROLLER = "roller"  # Selector	Roller
WIDGET_CPICKER = "colorpicker"  # Selector	Color picker

WIDGET_SLIDER = "slider"  # Range	Slider
WIDGET_PROGRESSBAR = "progressbar"  # Range	Progress Bar
WIDGET_ARC = "arc"  # Range	Arc
WIDGET_LINEMETER = "linemeter"  # Range	Line Meter
WIDGET_GUAUGE = "gauge"  # Range	Gauge

WIDGET_BTNMATRIX = "btnmatrix"  # Selector	Button Matrix
WIDGET_MSGBOX = "msgbox"  # Selector	Messagebox


CONF_ON_PAGE_CHANGE = "on_page_change"

CONF_TITLE = "title"
CONF_WIDGET_ID = "widget_id"
CONF_DISPLAY_DEFINE = "display_id"
CONF_HEADER = "header"
CONF_HEADER_HEIGHT = "header_height"
CONF_FOOTER = "footer"
CONF_FOOTER_HEIGHT = "footer_height"
CONF_TABVIEW = "tabview"

CONF_X = "x"
CONF_Y = "y"
CONF_WIDTH = "width"
CONF_HEIGHT = "height"
CONF_CONTENT = "content"

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

CONF_IMAGE_ID = "image_id"
CONF_SHIFT_X = "shift_x"
CONF_SHIFT_Y = "shift_y"

CONF_THEMA = "thema"
CONF_STYLE = "style"

openHASP_ns = cg.esphome_ns.namespace("switch_plate")

SwitchPlate = openHASP_ns.class_("SwitchPlate", cg.Component)

StyleStruct = openHASP_ns.struct("Style")

SwitchPlateItem = openHASP_ns.class_("SwitchPlateItem")
SwitchPlateItemConstPtr = SwitchPlateItem.operator("ptr").operator("const")

SwitchPlateGroup = openHASP_ns.class_("SwitchPlateGroup")

SwitchPlatePage = openHASP_ns.class_("SwitchPlatePage")

SwitchPlateIsPageCondition = openHASP_ns.class_(
    "SwitchPlateIsPageCondition", automation.Action
)
SwitchPlateShowNextAction = openHASP_ns.class_(
    "SwitchPlateShowNextAction", automation.Action
)
SwitchPlateShowPrevAction = openHASP_ns.class_(
    "SwitchPlateShowPrevAction", automation.Action
)
SwitchPlateOnPageChangeTrigger = openHASP_ns.class_(
    "SwitchPlateOnPageChangeTrigger", automation.Trigger
)

WidgetMode_ = openHASP_ns  # .enum("Mode", is_class=True)
CONF_SPINNER_MODES = {
    "constant": WidgetMode_.CONSTANT,
    "slowdown": WidgetMode_.SLOWDOWN,
    "slowdown_stratch": WidgetMode_.SLOWSTRATCH,
}


Direction_ = openHASP_ns  # .enum("Direction", is_class=True)
CONF_DROPDOWN_DIRECTIONS = {
    "DOWN": Direction_.DOWN,
    "UP": Direction_.UP,
    "LEFT": Direction_.LEFT,
    "RIGHT": Direction_.RIGHT,
}
CONF_SPINNER_DIRECTIONS = {
    "CW": Direction_.CW,
    "CCW": Direction_.CCW,
}

Style_ = openHASP_ns.enum("Style", is_class=True)
CONF_STYLE_SECTION = {
    "background": Style_.BACKGROUND,
    "border": Style_.BORDER,
    "text": Style_.TEXT,
    "header": Style_.HEADER,
    "footer": Style_.FOOTER,
    "foreground": Style_.FOREGROUND,
    "color": Style_.COLOR,
    "image": Style_.IMAGE,
    "-sect-00c0": 0x00c0,
    "-sect-00e0": 0x00e0,
}
CONF_STYLE_ARTIFACT = {
    "id": Style_.ID,
    "from": Style_.FROM,
    "to": Style_.TO,
    "direction": Style_.DIRECTION,
    "font": Style_.FONT,
    "align": Style_.ALIGN,
    "mode": Style_.MODE,
    "radius": Style_.RADIUS,
    "-art-0900": 0x0900,
    "shift_x": Style_.SHIFT_X,
    "shift_y": Style_.SHIFT_Y,
    "heigth": Style_.HEIGTH,
    "width": Style_.WIDTH,
}
CONF_STYLE_STATUS = {
    "none": Style_.NONE,
    "pressed": Style_.PRESS,
    "selected": Style_.SELECT,
    "disabled": Style_.DISABLE,
    "pressed_selected": Style_.PRESS | Style_.SELECT,
    "selected_pressed": Style_.PRESS | Style_.SELECT,
    "disabled_selected": Style_.DISABLE | Style_.SELECT,
    "selected_disabled": Style_.DISABLE | Style_.SELECT,
}
CONF_STYLE_WIDGETS = {
    "switchplate": Style_.BASE_SWITCHPLATE,
    WIDGET_LABEL: Style_.WIDGET_LABEL,
    WIDGET_DATETIME: Style_.WIDGET_DATETIME,
    WIDGET_BUTTON: Style_.WIDGET_BUTTON,
    "page": Style_.WIDGET_PAGE,
    WIDGET_PAGETITLE: Style_.WIDGET_PAGETITLE,
    WIDGET_PANEL: Style_.WIDGET_PANEL,
    WIDGET_IMAGE: Style_.WIDGET_IMAGE,
}

WidgetMode_ = openHASP_ns.enum("Mode", is_class=True)
ENUM_MODES = {
    "expand": WidgetMode_.EXPAND,
    "break": WidgetMode_.BREAK,
    "dots": WidgetMode_.DOTS,
    "scroll": WidgetMode_.SCROLL,
    "loop": WidgetMode_.LOOP,
    "crop": WidgetMode_.CROP,
}

WidgetAlign_ = openHASP_ns.enum("Align", is_class=True)
ENUM_ALIGN = {
    "top"               : WidgetAlign_.TOP,
    "center_vertical"   : WidgetAlign_.CENTER_VERTICAL,
    "baseline"          : WidgetAlign_.BASELINE,
    "bottom"            : WidgetAlign_.BOTTOM,
    "left"              : WidgetAlign_.LEFT,
    "center_horizontal" : WidgetAlign_.CENTER_HORIZONTAL,
    "right"             : WidgetAlign_.RIGHT,
    "top_left"          : WidgetAlign_.TOP_LEFT,
    "top_center"        : WidgetAlign_.TOP_CENTER,
    "top_right"         : WidgetAlign_.TOP_RIGHT,
    "center_left"       : WidgetAlign_.CENTER_LEFT,
    "center"            : WidgetAlign_.CENTER,
    "center_right"      : WidgetAlign_.CENTER_RIGHT,
    "baseline_left"     : WidgetAlign_.BASELINE_LEFT,
    "baseline_center"   : WidgetAlign_.BASELINE_CENTER,
    "baseline_right"    : WidgetAlign_.BASELINE_RIGHT,
    "bottom_left"       : WidgetAlign_.BOTTOM_LEFT,
    "bottom_center"     : WidgetAlign_.BOTTOM_CENTER,
    "bottom_right"      : WidgetAlign_.BOTTOM_RIGHT,
}

Direction_ = openHASP_ns.enum("Direction", is_class=True)
CONF_GRANDIENT_DIRECTIONS = {
    "horizontal": Direction_.HORIZONTAL,
    "vertical": Direction_.VERTICAL,
    "both": Direction_.BOTH,
}


CONF_WIDGET_CLASSES = {
    WIDGET_LINE: openHASP_ns.class_("SwitchPlateLine", SwitchPlateItem),
    WIDGET_IMAGE: openHASP_ns.class_("SwitchPlateImage", SwitchPlateItem),
    WIDGET_SPINNER: openHASP_ns.class_("SwitchPlateSpinner", SwitchPlateItem),
    WIDGET_PANEL: openHASP_ns.class_("SwitchPlateGroup", SwitchPlateItem),
    WIDGET_LED: openHASP_ns.class_("SwitchPlateLed", SwitchPlateItem),
    WIDGET_LABEL: openHASP_ns.class_("SwitchPlateLabel", SwitchPlateItem),
    WIDGET_PAGETITLE: openHASP_ns.class_("SwitchPlatePageTitle", SwitchPlateItem),
    WIDGET_DATETIME: openHASP_ns.class_("SwitchPlateDateTime", SwitchPlateItem),
    WIDGET_BUTTON: openHASP_ns.class_("SwitchPlateButton", SwitchPlateItem),
    WIDGET_SWITCH: openHASP_ns.class_("SwitchPlateSwitch", SwitchPlateItem),
    WIDGET_CHECKBOX: openHASP_ns.class_("SwitchPlateCheckbox", SwitchPlateItem),
    WIDGET_DROPDOWN: openHASP_ns.class_("SwitchPlateDropdown", SwitchPlateItem),
    WIDGET_ROLLER: openHASP_ns.class_("SwitchPlateRoller", SwitchPlateItem),
    WIDGET_CPICKER: openHASP_ns.class_("SwitchPlateColorPicker", SwitchPlateItem),
    WIDGET_PROGRESSBAR: openHASP_ns.class_("SwitchPlateProgressBar", SwitchPlateItem),
    WIDGET_SLIDER: openHASP_ns.class_("SwitchPlateSlider", SwitchPlateItem),
    WIDGET_ARC: openHASP_ns.class_("SwitchPlateArc", SwitchPlateItem),
    WIDGET_LINEMETER: openHASP_ns.class_("SwitchPlateLineMeter", SwitchPlateItem),
    WIDGET_GUAUGE: openHASP_ns.class_("SwitchPlateGauge", SwitchPlateItem),
}

def valid_color(value):
    """Validate that the value is a hex RGB color.
    value can be a color_id, a Hex value like #RRGGBB, a RGB(10,10,10), HSL(350deg,50%,100%)
    or just a positive integer value"""
    if not isinstance(value, int):
        if value.startswith("#"):
            value = value[1:]
            if len(value) == 3:
                value = "".join(value[i] * 2 for i in range(3))
            try:
                value = int(value, 16)
            except ValueError:
                # pylint: disable=raise-missing-from
                raise cv.Invalid("invalid hex color")

        elif value.startswith("RGB("):
            value = value[4:-1].split(",", 3)
            value = (value[0] << 16) + (value[1] << 8) + value[2]

        elif value.startswith("HSL("):
            value = value[4:-1].split(",", 3)
            value[0] = cv.angle(value[0]) / 360
            value[1] = cv.percentage(value[1])
            value[2] = cv.percentage(value[2])
            clr = hls_to_rgb(value[0], value[1], value[2])

            value = (
                (int(clr[0] * 255) << 16) + (int(clr[1] * 255) << 8) + int(clr[0] * 255)
            )

        else:
            if (
                isinstance(value, core.ID)
                and value.is_declaration is False
                and value.type is color.ColorStruct
            ):
                return value
            return core.ID(
                cv.validate_id_name(value), is_declaration=False, type=color.ColorStruct
            )
    if not isinstance(value, int):
        raise cv.Invalid("invalid color definition.")
    if value < 0 or value > 0xFFFFFF:
        raise cv.Invalid("invalid color value.")
    return value


def add_style_statuses(validator):
    return  cv.Schema(validator).extend(
            {
                cv.Optional("pressed"): cv.Schema(validator),
                cv.Optional("selected"): cv.Schema(validator),
                cv.Optional("disabled"): cv.Schema(validator),
                cv.Optional("pressed_selected"): cv.Schema(validator),
                cv.Optional("disabled_selected"): cv.Schema(validator),
            }
    )

STYLE_COLOR_SCHEMA = cv.Schema(
    {
        cv.Optional("from"): valid_color,
        cv.Optional("to"): valid_color,
        cv.Optional("direction"): cv.enum(CONF_GRANDIENT_DIRECTIONS),
    },
    #key="from",
)
def style_color_schema(value):
    return STYLE_COLOR_SCHEMA(value)


STYLE_TEXT_SCHEMA = cv.Schema(
    {
        cv.Optional("font"): cv.use_id(font.Font),
        cv.Optional("align"): cv.enum(ENUM_ALIGN),
        cv.Optional("mode"): cv.enum(ENUM_MODES),
        cv.Optional("color"): style_color_schema,
    },
    #key="font",
)
def style_text_schema(value):
    return STYLE_TEXT_SCHEMA(value)


STYLE_IMAGE_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_ID): cv.use_id(image.Image_),
        cv.Optional("shift_x"): cv.int_range(-1024, 1024),
        cv.Optional("shift_y"): cv.int_range(-1024, 1024),
        cv.Optional("background"): style_color_schema,
        cv.Optional("foreground"): style_color_schema,
    },
    #key=CONF_ID,
)
def style_image_schema(value):
    return STYLE_IMAGE_SCHEMA(value)


STYLE_BACKGROUND_SCHEMA = cv.Schema(
    {
        cv.Optional("color"): style_color_schema,
        cv.Optional("image"): style_image_schema,
    },
    #key="color",
)
def style_background_schema(value):
    return STYLE_BACKGROUND_SCHEMA(value)

STYLE_BORDER_SCHEMA = cv.Schema(
    {
        cv.Optional("color"): style_color_schema,
        cv.Optional("radius"): cv.int_range(-1024, 1024),
    },
    #key="color",
)
def style_border_schema(value):
    return STYLE_BORDER_SCHEMA(value)


WIDGET_SWITCHPLATE_SCHEMA = cv.Schema(
    {
        cv.Required("default_font"): cv.use_id(font.Font),
        cv.Optional("header_color"): style_color_schema,
        cv.Optional("footer_color"): style_color_schema,
        cv.Optional(CONF_HEADER_HEIGHT): cv.uint8_t,
        cv.Optional(CONF_FOOTER_HEIGHT): cv.uint8_t,

    }
)
def widget_switchplate_schema(value):
    return WIDGET_SWITCHPLATE_SCHEMA(value)

WIDGET_LABEL_SCHEMA = add_style_statuses(
    {
        cv.Optional("text"): style_text_schema,
        cv.Optional("background"): style_background_schema,
        cv.Optional("border"): style_border_schema,
    }
)
def widget_label_schema(value):
    return WIDGET_LABEL_SCHEMA(value)

WIDGET_DATETIME_SCHEMA = WIDGET_LABEL_SCHEMA
def widget_datetime_schema(value):
    return WIDGET_DATETIME_SCHEMA(value)

WIDGET_BUTTON_SCHEMA = WIDGET_LABEL_SCHEMA
def widget_button_schema(value):
    return WIDGET_BUTTON_SCHEMA(value)

WIDGET_PAGETITLE_SCHEMA = WIDGET_LABEL_SCHEMA
def widget_pagetittle_schema(value):
    return WIDGET_PAGETITLE_SCHEMA(value)

WIDGET_PAGE_SCHEMA = add_style_statuses(
    {
        cv.Optional("background"): style_background_schema,
    }
)
def widget_page_schema(value):
    return WIDGET_PAGE_SCHEMA(value)

WIDGET_PANEL_SCHEMA = WIDGET_PAGE_SCHEMA
def widget_panel_schema(value):
    return WIDGET_PANEL_SCHEMA(value)

WIDGET_IMAGE_SCHEMA = add_style_statuses(
    {
        cv.Required("image"): style_image_schema,
    }
)
def widget_image_schema(value):
    return WIDGET_IMAGE_SCHEMA(value)

STYLE_THEMA_SCHEMA = cv.Schema(
    {
        cv.Optional("switchplate"): widget_switchplate_schema,
        cv.Optional(WIDGET_LABEL): widget_label_schema,
        cv.Optional(WIDGET_DATETIME): widget_datetime_schema,
        cv.Optional(WIDGET_BUTTON): widget_button_schema,
        cv.Optional("page"): widget_page_schema,
        cv.Optional(WIDGET_PAGETITLE): widget_pagetittle_schema,
        cv.Optional(WIDGET_PANEL): widget_panel_schema,
        cv.Optional(WIDGET_IMAGE): widget_image_schema,
    }
)
def style_thema_schema(value):
    return STYLE_THEMA_SCHEMA(value)

async def setup_style(var, config, style = Style_.NONE):
    for key, value in config.items():
        if key in CONF_STYLE_STATUS.keys():
            await setup_style(var, value, style | CONF_STYLE_STATUS[key])
        elif key in CONF_STYLE_SECTION.keys():
            await setup_style(var, value, style | CONF_STYLE_SECTION[key])
        elif key in CONF_STYLE_ARTIFACT.keys():
            if (isinstance(value, core.ID)):
                value = await cg.get_variable(value)
            cg.add(var.set_style(style | CONF_STYLE_ARTIFACT[key], StyleStruct(value)))
        elif key == "header_color":
            if (isinstance(value, core.ID)):
                value = await cg.get_variable(value)
            cg.add(var.set_style(style | Style_.HEADER | Style_.COLOR, StyleStruct(value)))
        elif key == "footer_color":
            if (isinstance(value, core.ID)):
                value = await cg.get_variable(value)
            cg.add(var.set_style(style | Style_.FOOTER | Style_.COLOR, StyleStruct(value)))
        elif key == "default_font":
            if (isinstance(value, core.ID)):
                value = await cg.get_variable(value)
            cg.add(var.set_style(style | Style_.FONT, StyleStruct(value)))

        else:
           pass

async def setup_thema(var, config):
    for key, value in config.items():
        if key in CONF_STYLE_WIDGETS.keys():
            await setup_style(var, value, CONF_STYLE_WIDGETS[key])
        else:
           pass



def validate_min_max(config):
    if config[CONF_MAX_VALUE] <= config[CONF_MIN_VALUE]:
        raise cv.Invalid("max_value must be greater than min_value")
    if not isinstance(config[CONF_VALUE], core.Lambda):
        if config[CONF_VALUE] < config[CONF_MIN_VALUE]:
            raise cv.Invalid(CONF_VALUE + " must be greater or equel than min_value")
        if config[CONF_VALUE] > config[CONF_MAX_VALUE]:
            raise cv.Invalid(CONF_VALUE + " must be equel or less than max_value")
    return config


# Use a simple indirection to circumvent the recursion limitation


def switchplate_item_schema(value):
    return SWITCHPLATE_ITEM_SCHEMA(value)


CONF_DIMENSION = "dimension"
SWITCHPLATE_ITEM_COMMON_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_DIMENSION): cv.Schema(
            {
                cv.Optional(CONF_X, default=0): cv.int_range(-1024, 1024),
                cv.Optional(CONF_Y, default=0): cv.int_range(-1024, 1024),
                cv.Optional(CONF_WIDTH, default=-1): cv.int_range(-3, 1024),
                cv.Optional(CONF_HEIGHT, default=-1): cv.int_range(-3, 1024),
            }
        ),
        cv.Optional(CONF_VISIBLE): cv.boolean,
        cv.Optional(CONF_DISABLED): cv.boolean,
        cv.Optional(CONF_SELECTED): cv.boolean,
        cv.Optional(CONF_SELECTABLE): cv.boolean,
        cv.Optional(CONF_CLICKABLE): cv.boolean,
    }
)

SWITCHPLATE_ITEM_MINMAX_SCHEMA = SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
    {
        cv.Optional(CONF_MIN_VALUE): cv.int_,
        cv.Optional(CONF_MAX_VALUE): cv.int_,
        cv.Optional(CONF_VALUE): cv.templatable(cv.int_),
    }
).add_extra(validate_min_max)

SWITCHPLATE_ITEM_TEXT_SCHEMA = SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
    {
        cv.Optional(CONF_CONTENT): cv.templatable(cv.string),
    }
)


SWITCHPLATE_ITEM_SCHEMA = cv.All(
    cv.typed_schema(
        {
            WIDGET_PANEL: cv.Schema(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(
                        CONF_WIDGET_CLASSES[WIDGET_PANEL]
                    ),
                    cv.Required(CONF_WIDGETS): cv.All(
                        cv.ensure_list(switchplate_item_schema), cv.Length(min=1)
                    ),
                    cv.Optional(CONF_STYLE): widget_panel_schema,
                }
            ),
            WIDGET_LABEL: SWITCHPLATE_ITEM_TEXT_SCHEMA.extend(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(
                        CONF_WIDGET_CLASSES[WIDGET_LABEL]
                    ),
                    cv.Optional(CONF_STYLE): widget_label_schema,
                }
            ),
            WIDGET_PAGETITLE: SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(
                        CONF_WIDGET_CLASSES[WIDGET_PAGETITLE]
                    ),
                    cv.Optional(CONF_STYLE): widget_pagetittle_schema,
                }
            ),
            WIDGET_DATETIME: SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(
                        CONF_WIDGET_CLASSES[WIDGET_DATETIME]
                    ),
                    cv.Optional(CONF_FORMAT): cv.string,
                    cv.Required(CONF_TIME_ID): cv.use_id(time.RealTimeClock),
                    cv.Optional(CONF_STYLE): widget_datetime_schema,

                }
            ),
            WIDGET_BUTTON: SWITCHPLATE_ITEM_TEXT_SCHEMA.extend(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(
                        CONF_WIDGET_CLASSES[WIDGET_BUTTON]
                    ),
                    cv.Optional(CONF_STYLE): widget_button_schema,

                }
            ),
            WIDGET_IMAGE: SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(
                        CONF_WIDGET_CLASSES[WIDGET_IMAGE]
                    ),
                    cv.Optional(CONF_STYLE): widget_image_schema,
                }
            ),
        },
        default_type="label",
        lower=True,
    )
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(SwitchPlate),
        cv.Optional(CONF_DISPLAY_DEFINE): cv.use_id(display.DisplayBuffer),
        cv.Optional(touchscreen.CONF_TOUCHSCREEN_ID): cv.use_id(
            touchscreen.Touchscreen
        ),
        cv.Optional(CONF_TABVIEW): cv.boolean,
        cv.Optional(CONF_HEADER): cv.All(
            cv.ensure_list(switchplate_item_schema), cv.Length(min=1)
        ),
        cv.Optional(CONF_FOOTER): cv.All(
            cv.ensure_list(switchplate_item_schema), cv.Length(min=1)
        ),
        cv.Optional(CONF_THEMA): style_thema_schema,
        cv.Optional(CONF_STYLE): widget_switchplate_schema,
        cv.Required(CONF_PAGES): cv.All(
            cv.ensure_list(
                cv.Schema({
                    cv.GenerateID(): cv.declare_id(SwitchPlatePage),
                    cv.Optional(CONF_WIDGETS): cv.All(
                        cv.ensure_list(switchplate_item_schema), cv.Length(min=1)
                    ),
                    cv.Optional(CONF_TITLE): cv.string,
                    cv.Optional(CONF_SELECTABLE, default=TRUE): cv.boolean,
                    cv.Optional(CONF_VISIBLE): cv.boolean,
                    cv.Optional(CONF_DISABLED): cv.boolean,
                    cv.Optional(CONF_STYLE): widget_page_schema,
                }),
            ),
            cv.Length(min=1),
        )
    }
).extend(
    cv.COMPONENT_SCHEMA,
    WIDGET_SWITCHPLATE_SCHEMA
)  # .add_extra(cv.has_exactly_one_key([CONF_DISPLAY_DEFINE, touchscreen.CONF_TOUCHSCREEN_ID]))

WIDGET_STATUS_OPTIONS = {
    CONF_SELECTED: 2,
    CONF_DISABLED: 4,
    CONF_VISIBLE: 8,
    CONF_CLICKABLE: 9,
    CONF_SELECTABLE: 10,
}

async def item_to_code(config):
    item = cg.new_Pvariable(config[CONF_ID])
    if CONF_TITLE in config:
        cg.add(item.set_title(config[CONF_TITLE]))
    if CONF_DIMENSION in config:
        dim = config[CONF_DIMENSION]
        cg.add(
            item.set_dimension(
                dim[CONF_X], dim[CONF_Y], dim[CONF_WIDTH], dim[CONF_HEIGHT]
            )
        )
    if CONF_CONTENT in config:
        if isinstance(config[CONF_CONTENT], core.Lambda):
            template_ = await cg.templatable(
                config[CONF_CONTENT], [(SwitchPlateItemConstPtr, "it")], cg.std_string
            )
            cg.add(item.set_text(template_))
        else:
            cg.add(item.set_text(config[CONF_CONTENT]))
    if CONF_VALUE in config:
        if isinstance(config[CONF_VALUE], core.Lambda):
            template_ = await cg.templatable(
                config[CONF_VALUE], [(SwitchPlateItemConstPtr, "it")], cg.int32
            )
            cg.add(item.set_state(template_))
        else:
            cg.add(item.set_state(config[CONF_VALUE]))

    if CONF_WIDGETS in config:
        for c in config[CONF_WIDGETS]:
            sub = await item_to_code(c)
            cg.add(item.add_widget(sub))

    if CONF_MIN_VALUE in config:
        cg.add(item.set_min_value(config[CONF_MIN_VALUE]))
    if CONF_MAX_VALUE in config:
        cg.add(item.set_max_value(config[CONF_MAX_VALUE]))
    if CONF_FORMAT in config:
        cg.add(item.set_time_format(config[CONF_FORMAT]))
    if CONF_TIME_ID in config:
        time_ = await cg.get_variable(config[CONF_TIME_ID])
        cg.add(item.set_time(time_))
    for key in WIDGET_STATUS_OPTIONS.keys():
        if key not in config:
            continue
        cg.add(item.set_widget_status(WIDGET_STATUS_OPTIONS[key], config[key]))
    if CONF_STYLE in config:
        await setup_style(item, config[CONF_STYLE])
    return item

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    if CONF_DISPLAY_DEFINE in config:
        parent = await cg.get_variable(config[CONF_DISPLAY_DEFINE])
        cg.add(var.set_display(parent))
    if touchscreen.CONF_TOUCHSCREEN_ID in config:
        parent = await cg.get_variable(config[touchscreen.CONF_TOUCHSCREEN_ID])
        cg.add(var.set_touchscreen(parent))
    if CONF_PAGES in config:
        for c in config[CONF_PAGES]:
            item = await item_to_code(c)
            cg.add(var.add_page(item))
    if CONF_HEADER in config:
        for c in config[CONF_HEADER]:
            item = await item_to_code(c)
            cg.add(var.add_header_widget(item))
    if CONF_FOOTER in config:
        for c in config[CONF_FOOTER]:
            item = await item_to_code(c)
            cg.add(var.add_footer_widget(item))
    if CONF_DEFAULT_FONT in config:
        parent = await cg.get_variable(config[CONF_DEFAULT_FONT])
        cg.add(var.set_default_font(parent))
    if CONF_TABVIEW in config:
        cg.add(var.set_tabview(config[CONF_TABVIEW]))
    if CONF_STYLE in config:
        await setup_style(var, config[CONF_STYLE])
    if CONF_THEMA in config:
        await setup_thema(var, config[CONF_THEMA])
