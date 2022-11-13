from colorsys import hls_to_rgb

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation, core
from esphome.automation import maybe_simple_id
from esphome.components import color, display, font, image, time, touchscreen
from esphome.const import (
    CONF_FORMAT,
    CONF_ID,
    CONF_MAX_VALUE,
    CONF_MIN_VALUE,
    CONF_PAGE_ID,
    CONF_PAGES,
    CONF_TIME_ID,
)
from esphome.core import coroutine_with_priority, HexInt

# from . import styles as style    # "myapp" case

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

CONF_SWITCHPLATE = "switchplate"
WIDGET_PAGE = "page"
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
CONF_DISPLAY_ID = "display_id"
CONF_HEADER = "header"
CONF_HEADER_HEIGHT = "header_height"
CONF_FOOTER = "footer"
CONF_FOOTER_HEIGHT = "footer_height"
CONF_TABVIEW = "tabview"
CONF_ACTION = "action"

CONF_X = "x"
CONF_Y = "y"
CONF_WIDTH = "width"
CONF_HEIGHT = "height"

CONF_CONTENT = "content"

CONF_STATE = "state"
CONF_VISIBLE = "is_visible"
CONF_DISABLE = "is_disabled"

CONF_SELECTABLE = "selectable"
CONF_SELECT = "is_selected"

CONF_CLICKABLE = "clickable"
CONF_PRESSED = "is_pressed"

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
CONF_OFFSET_X = "offset_x"
CONF_OFFSET_Y = "offset_y"

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

WidgetMode_ = openHASP_ns.enum("Mode", is_class=True)
CONF_SPINNER_MODES = {
    "constant": WidgetMode_.CONSTANT,
    "slowdown": WidgetMode_.SLOWDOWN,
    "slowdown_stratch": WidgetMode_.SLOWSTRATCH,
}

CONF_SWITCH_MODES = {
    "ROUND_MODE": WidgetMode_.ROUND_MODE,
    "RECT_MODE": WidgetMode_.RECT_MODE,
}

Direction_ = openHASP_ns.enum("Direction", is_class=True)
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

DoAction_ = openHASP_ns.enum("DoAction", is_class=True)
ENUM_ACTION = {
    "DO_NOTTING": DoAction_.DO_NOTTING,
    "HOME": DoAction_.SHOW_HOME,
    "PREV": DoAction_.SHOW_PREV,
    "PREVIOUS": DoAction_.SHOW_PREV,
    "NEXT": DoAction_.SHOW_NEXT,
    "HOME_PAGE": DoAction_.SHOW_HOME,
    "PREV_PAGE": DoAction_.SHOW_PREV,
    "PREVIOUS_PAGE": DoAction_.SHOW_PREV,
    "NEXT_PAGE": DoAction_.SHOW_NEXT,
}


Style_ = openHASP_ns.enum("Style", is_class=True)
ENUM_STYLE_ARTIFACT = {
    "background_color": Style_.BACKGROUND | Style_.COLOR,
    "background_color_from": Style_.BACKGROUND | Style_.COLOR | Style_.FROM,
    "background_color_to": Style_.BACKGROUND | Style_.COLOR | Style_.TO,
    "background_color_direction": Style_.BACKGROUND | Style_.COLOR | Style_.DIRECTION,
    "foreground_color": Style_.BACKGROUND | Style_.COLOR,
    "foreground_color_from": Style_.BACKGROUND | Style_.COLOR | Style_.FROM,
    "foreground_color_to": Style_.BACKGROUND | Style_.COLOR | Style_.TO,
    "foreground_color_direction": Style_.BACKGROUND | Style_.COLOR | Style_.DIRECTION,
    "image_id": Style_.IMAGE | Style_.ID,
    "image_offset_x": Style_.IMAGE | Style_.OFFSET_X,
    "image_offset_y": Style_.IMAGE | Style_.OFFSET_Y,
    "image_color": Style_.IMAGE | Style_.COLOR,
    "image_color_from": Style_.IMAGE | Style_.COLOR | Style_.FROM,
    "image_color_to": Style_.IMAGE | Style_.COLOR | Style_.TO,
    "image_color_direction": Style_.IMAGE | Style_.COLOR | Style_.DIRECTION,
    "border_color": Style_.BORDER | Style_.COLOR,
    "border_color_from": Style_.BORDER | Style_.COLOR | Style_.FROM,
    "border_color_to": Style_.BORDER | Style_.COLOR | Style_.TO,
    "border_color_direction": Style_.BORDER | Style_.COLOR | Style_.DIRECTION,
    "border_radius": Style_.BORDER | Style_.RADIUS,
    "text_font": Style_.TEXT | Style_.FONT,
    "text_align": Style_.TEXT | Style_.ALIGN,
    "text_mode": Style_.TEXT | Style_.MODE,
    "text_color": Style_.BACKGROUND | Style_.COLOR,
    "text_color_from": Style_.BACKGROUND | Style_.COLOR | Style_.FROM,
    "text_color_to": Style_.BACKGROUND | Style_.COLOR | Style_.TO,
    "text_color_direction": Style_.BACKGROUND | Style_.COLOR | Style_.DIRECTION,
    "header_heigth": Style_.HEADER | Style_.HEIGTH,
    "header_color": Style_.HEADER | Style_.COLOR,
    "header_color_from": Style_.HEADER | Style_.COLOR | Style_.FROM,
    "header_color_to": Style_.HEADER | Style_.COLOR | Style_.TO,
    "header_color_direction": Style_.HEADER | Style_.COLOR | Style_.DIRECTION,
    "footer_heigth": Style_.FOOTER | Style_.HEIGTH,
    "footer_color": Style_.FOOTER | Style_.COLOR,
    "footer_color_from": Style_.FOOTER | Style_.COLOR | Style_.FROM,
    "footer_color_to": Style_.FOOTER | Style_.COLOR | Style_.TO,
    "footer_color_direction": Style_.FOOTER | Style_.COLOR | Style_.DIRECTION,
    "default_font": Style_.FONT,
    "switch_mode": Style_.SWITCH | Style_.MODE,
    "checkbox_mode": Style_.CHECKBOX | Style_.MODE,
}

ENUM_STYLE_STATUS = {
    "pressed": Style_.PRESS,
    "selected": Style_.SELECT,
    "disabled": Style_.DISABLE,
    "pressed_selected": Style_.PRESS | Style_.SELECT,
    "disabled_selected": Style_.DISABLE | Style_.SELECT,
}

ENUM_STYLE_WIDGETS = {
    CONF_SWITCHPLATE: Style_.BASE_SWITCHPLATE,
    WIDGET_LABEL: Style_.WIDGET_LABEL,
    WIDGET_DATETIME: Style_.WIDGET_DATETIME,
    WIDGET_BUTTON: Style_.WIDGET_BUTTON,
    WIDGET_PAGE: Style_.WIDGET_PAGE,
    WIDGET_PAGETITLE: Style_.WIDGET_PAGETITLE,
    WIDGET_PANEL: Style_.WIDGET_PANEL,
    WIDGET_IMAGE: Style_.WIDGET_IMAGE,
    WIDGET_SWITCH: Style_.WIDGET_SWITCH,
    WIDGET_CHECKBOX: Style_.WIDGET_CHECKBOX,
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
    "top": WidgetAlign_.TOP,
    "center_vertical": WidgetAlign_.CENTER_VERTICAL,
    "baseline": WidgetAlign_.BASELINE,
    "bottom": WidgetAlign_.BOTTOM,
    "left": WidgetAlign_.LEFT,
    "center_horizontal": WidgetAlign_.CENTER_HORIZONTAL,
    "right": WidgetAlign_.RIGHT,
    "top_left": WidgetAlign_.TOP_LEFT,
    "top_center": WidgetAlign_.TOP_CENTER,
    "top_right": WidgetAlign_.TOP_RIGHT,
    "center_left": WidgetAlign_.CENTER_LEFT,
    "center": WidgetAlign_.CENTER,
    "center_right": WidgetAlign_.CENTER_RIGHT,
    "baseline_left": WidgetAlign_.BASELINE_LEFT,
    "baseline_center": WidgetAlign_.BASELINE_CENTER,
    "baseline_right": WidgetAlign_.BASELINE_RIGHT,
    "bottom_left": WidgetAlign_.BOTTOM_LEFT,
    "bottom_center": WidgetAlign_.BOTTOM_CENTER,
    "bottom_right": WidgetAlign_.BOTTOM_RIGHT,
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
    # print(validator.keys())
    return {
        cv.Optional("pressed"): validator,
        cv.Optional("selected"): validator,
        cv.Optional("disabled"): validator,
        cv.Optional("pressed_selected"): validator,
        cv.Optional("disabled_selected"): validator,
    } | validator


def style_color_schema(key=""):
    return {
        cv.Optional(key + "color"): valid_color,
        cv.Optional(key + "color_from"): valid_color,
        cv.Optional(key + "color_to"): valid_color,
        cv.Optional(key + "color_direction"): cv.enum(
            CONF_GRANDIENT_DIRECTIONS, upper=False, space="_"
        ),
    }


def style_text_schema(key=""):
    return {
        cv.Optional(key + "text_font"): cv.use_id(font.Font),
        cv.Optional(key + "text_align"): cv.enum(ENUM_ALIGN, upper=False, space="_"),
        cv.Optional(key + "text_mode"): cv.enum(ENUM_MODES, upper=False, space="_"),
    } | style_color_schema(key + "text_")


def style_image_schema(key=""):
    return {
        cv.Optional(key + CONF_IMAGE_ID): cv.use_id(image.Image_),
        cv.Optional(key + "image_offset_x"): cv.int_range(-1024, 1024),
        cv.Optional(key + "image_offset_y"): cv.int_range(-1024, 1024),
    } | style_color_schema(key + "image_")


def style_background_schema(key=""):
    return style_color_schema(key + "background_")


def style_border_schema(key=""):
    return {
        cv.Optional(key + "border_radius"): cv.int_range(-1024, 1024),
    } | style_color_schema(key + "border_")


def widget_switchplate_schema():
    # print("=> SWITCHPLATE:")
    return (
        {
            cv.Required("default_font"): cv.use_id(font.Font),
            cv.Optional("header_height"): cv.uint8_t,
            cv.Optional("footer_height"): cv.uint8_t,
        }
        | style_color_schema("header_")
        | style_color_schema("footer_")
    )


def widget_page_schema():
    # print("=> PAGE:")
    return add_style_statuses(
        style_background_schema(),
    )


def widget_label_schema():
    # print("=> LABEL:")
    return add_style_statuses(
        style_text_schema() | style_background_schema() | style_border_schema()
    )


def widget_datetime_schema():
    # print("=> DATETIME:")
    return add_style_statuses(
        style_text_schema() | style_background_schema() | style_border_schema()
    )


def widget_pagetittle_schema():
    # print("=> PAGETITLE:")
    return add_style_statuses(
        style_text_schema() | style_background_schema() | style_border_schema()
    )


def widget_button_schema():
    # print("=> BUTTON:")
    return add_style_statuses(
        style_text_schema() | style_background_schema() | style_border_schema()
    )


def widget_panel_schema():
    # print("=> PANEL:")
    return add_style_statuses(
        style_background_schema(),
    )


def widget_image_schema():
    # print("=> IMAGE:")
    return add_style_statuses(
        style_image_schema() | style_background_schema() | style_border_schema()
    )


def widget_switch_schema():
    return add_style_statuses(
        {cv.Optional("switch_mode"): cv.enum(CONF_SWITCH_MODES, upper=True, space="_")}
        | style_image_schema()
        | style_background_schema()
        | style_border_schema()
        | style_color_schema("foreground_")
    )


def widget_checkbox_schema():
    return add_style_statuses(
        {cv.Optional("checkbox_mode"): cv.enum(CONF_SWITCH_MODES, upper=True, space="_")}
        | style_image_schema()
        | style_background_schema()
        | style_border_schema()
        | style_color_schema("foreground_")
    )


def style_thema_schema():
    return {
        cv.Optional(CONF_SWITCHPLATE): widget_switchplate_schema(),
        cv.Optional(WIDGET_LABEL): widget_label_schema(),
        cv.Optional(WIDGET_DATETIME): widget_datetime_schema(),
        cv.Optional(WIDGET_BUTTON): widget_button_schema(),
        cv.Optional(WIDGET_PAGE): widget_page_schema(),
        cv.Optional(WIDGET_PAGETITLE): widget_pagetittle_schema(),
        cv.Optional(WIDGET_PANEL): widget_panel_schema(),
        cv.Optional(WIDGET_IMAGE): widget_image_schema(),
        cv.Optional(WIDGET_SWITCH): widget_switch_schema(),
        cv.Optional(WIDGET_IMAGE): widget_checkbox_schema(),
    }


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
        cv.Optional(CONF_PRESSED): cv.boolean,
        cv.Optional(CONF_VISIBLE): cv.boolean,
        cv.Optional(CONF_DISABLE): cv.boolean,
        cv.Optional(CONF_SELECT): cv.boolean,
        cv.Optional(CONF_SELECTABLE): cv.boolean,
        cv.Optional(CONF_CLICKABLE): cv.boolean,
        cv.Optional(CONF_ACTION): cv.enum(ENUM_ACTION, upper=True, space="_"),
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
                }
            ).extend(widget_panel_schema()),
            WIDGET_LABEL: SWITCHPLATE_ITEM_TEXT_SCHEMA.extend(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(
                        CONF_WIDGET_CLASSES[WIDGET_LABEL]
                    ),
                }
            ).extend(widget_label_schema()),
            WIDGET_PAGETITLE: SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(
                        CONF_WIDGET_CLASSES[WIDGET_PAGETITLE]
                    ),
                }
            ).extend(widget_pagetittle_schema()),
            WIDGET_DATETIME: SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(
                        CONF_WIDGET_CLASSES[WIDGET_DATETIME]
                    ),
                    cv.Optional(CONF_FORMAT): cv.string,
                    cv.Required(CONF_TIME_ID): cv.use_id(time.RealTimeClock),
                }
            ).extend(widget_datetime_schema()),
            WIDGET_BUTTON: SWITCHPLATE_ITEM_TEXT_SCHEMA.extend(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(
                        CONF_WIDGET_CLASSES[WIDGET_BUTTON]
                    ),
                    cv.Optional(CONF_STYLE): widget_button_schema(),
                }
            ).extend(widget_button_schema()),
            WIDGET_IMAGE: SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(
                        CONF_WIDGET_CLASSES[WIDGET_IMAGE]
                    ),
                }
            ).extend(widget_image_schema()),
            WIDGET_SWITCH: SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(
                        CONF_WIDGET_CLASSES[WIDGET_SWITCH]
                    ),
                }
            ).extend(widget_switch_schema()),
            WIDGET_CHECKBOX: SWITCHPLATE_ITEM_COMMON_SCHEMA.extend(
                {
                    cv.GenerateID(CONF_ID): cv.declare_id(
                        CONF_WIDGET_CLASSES[WIDGET_CHECKBOX]
                    ),
                }
            ).extend(widget_checkbox_schema()),
        },
        default_type="label",
        lower=True,
    )
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(SwitchPlate),
            cv.Optional(CONF_DISPLAY_ID): cv.use_id(display.DisplayBuffer),
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
            cv.Required(CONF_PAGES): cv.All(
                cv.ensure_list(
                    cv.Schema(
                        {
                            cv.GenerateID(): cv.declare_id(SwitchPlatePage),
                            cv.Optional(CONF_WIDGETS): cv.All(
                                cv.ensure_list(switchplate_item_schema),
                                cv.Length(min=1),
                            ),
                            cv.Optional(CONF_TITLE): cv.string,
                            cv.Optional(CONF_SELECTABLE, default=True): cv.boolean,
                            cv.Optional(CONF_VISIBLE): cv.boolean,
                            cv.Optional(CONF_DISABLE): cv.boolean,
                        }
                    ).extend(widget_page_schema()),
                ),
                cv.Length(min=1),
            ),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(
        widget_switchplate_schema(),
    )
)
# .add_extra(cv.has_exactly_one_key([CONF_DISPLAY_ID, touchscreen.CONF_TOUCHSCREEN_ID]))
Status_ = openHASP_ns.enum("Status", is_class=True)
WIDGET_STATUS_OPTIONS = {
    CONF_PRESSED: Status_.PRESSED_,
    CONF_SELECT: Status_.SELECTED_,
    CONF_DISABLE: Status_.DISABLED_,
    CONF_VISIBLE: Status_.VISIBLE_,
    CONF_CLICKABLE: Status_.CLICKABLE_,
    CONF_SELECTABLE: Status_.SELECTABLE_,
}


async def setup_style(var, config, style=None):
    # style = kwargs.get('style', None)

    for key, value in config.items():
        if key in ENUM_STYLE_STATUS.keys():
            if style is None:
                await setup_style(var, value, style=ENUM_STYLE_STATUS[key])
            else:
                await setup_style(var, value, style=(style | ENUM_STYLE_STATUS[key]))

        elif key in ENUM_STYLE_ARTIFACT.keys():
            if isinstance(value, core.ID):
                value = await cg.get_variable(value)
            if style is None:
                cg.add(var.set_style(ENUM_STYLE_ARTIFACT[key], StyleStruct(value)))
            else:
                cg.add(
                    var.set_style(style | ENUM_STYLE_ARTIFACT[key], StyleStruct(value))
                )
        else:
            pass


async def setup_thema(var, config):
    for key, value in config.items():
        if key in ENUM_STYLE_WIDGETS.keys():
            await setup_style(var, value, ENUM_STYLE_WIDGETS[key])
        else:
            pass


async def item_to_code(config):
    item = cg.new_Pvariable(config[CONF_ID])
    await setup_style(item, config)

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
        if key in config:
            cg.add(item.set_status(WIDGET_STATUS_OPTIONS[key], config[key]))
    for c in config.get(CONF_WIDGETS, []):
        sub = await item_to_code(c)
        cg.add(item.add_widget(sub))
    if CONF_ACTION in config:
        cg.add(item.set_action(config[CONF_ACTION]))
    return item


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await setup_style(var, config)

    if CONF_DISPLAY_ID in config:
        parent = await cg.get_variable(config[CONF_DISPLAY_ID])
        cg.add(var.set_display(parent))
    if touchscreen.CONF_TOUCHSCREEN_ID in config:
        parent = await cg.get_variable(config[touchscreen.CONF_TOUCHSCREEN_ID])
        cg.add(var.set_touchscreen(parent))
    if CONF_DEFAULT_FONT in config:
        parent = await cg.get_variable(config[CONF_DEFAULT_FONT])
        cg.add(var.set_default_font(parent))
    if CONF_TABVIEW in config:
        cg.add(var.set_tabview(config[CONF_TABVIEW]))
    if CONF_THEMA in config:
        await setup_thema(var, config[CONF_THEMA])

    for c in config.get(CONF_PAGES, []):
        item = await item_to_code(c)
        cg.add(var.add_page(item))
    for c in config.get(CONF_HEADER, []):
        item = await item_to_code(c)
        cg.add(var.add_header_widget(item))
    for c in config.get(CONF_FOOTER, []):
        item = await item_to_code(c)
        cg.add(var.add_footer_widget(item))
