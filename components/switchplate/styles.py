import esphome.codegen as cgborder
import esphome.config_validation as cv
from esphome import core, automation
from esphome.automation import maybe_simple_id
from esphome.core import coroutine_with_priority
from esphome.components import font, image

from components.switchplate import CONF_DISABLED
#from rgb_color import color
try:
     from . import rgb_color as rc    # "myapp" case
except:
     import rgb_color as rc

openHASP_ns = cg.esphome_ns.namespace('switch_plate')

CONF_PRESSED = "pressed"
CONF_SELECTED = "selected"
CONF_DISABLED = "disabled"

SWITCHPLATE_COMMON_STYLE_SCHEMA = cv.Schema({})
SWITCHPLATE_TEXT_STYLE_SCHEMA = cv.Schema({})

def styles(orign, base, validator):
    orign.extend( {
        cv.Optional(base): validator,
        cv.Optional(base + CONF_PRESSED): validator,
        cv.Optional(base + CONF_SELECTED): validator,
        cv.Optional(base + CONF_PRESSED + CONF_SELECTED): validator
    })

CONF_ALIGN_LEFT = "left"
CONF_ALIGN_CENTER = "center"
CONF_ALIGN_RIGHT = "right"


CONF_GRADIENT_HORIZONTAL = "horizontal"
CONF_GRADIENT_VERTICAL = "vertical"

Direction_ = openHASP_ns.enum("Direction", is_class=True)
CONF_GRANDIENT_DIRECTIONS = {
    CONF_GRADIENT_HORIZONTAL : Direction_.GRADIENT_HORIZONTAL,
    CONF_GRADIENT_VERTICAL : Direction_.GRADIENT_VERTICAL,
}



CONF_BACKGROUND_COLOR = "background_color"
styles(SWITCHPLATE_COMMON_STYLE_SCHEMA, CONF_BACKGROUND_COLOR, rc.color )

CONF_BACKGROUND_GRADIENT_FROM = "background_gradient_from"
styles(SWITCHPLATE_COMMON_STYLE_SCHEMA, CONF_BACKGROUND_GRADIENT_FROM, rc.color )

CONF_BACKGROUND_GRADIENT_TO = "background_gradient_to"
styles(SWITCHPLATE_COMMON_STYLE_SCHEMA, CONF_BACKGROUND_GRADIENT_TO, rc.color )

CONF_BACKGROUND_GRADIENT_DIR = "background_gradient_direction"
styles(SWITCHPLATE_COMMON_STYLE_SCHEMA, CONF_BACKGROUND_GRADIENT_DIR, cv.enum(CONF_GRANDIENT_DIRECTIONS) )

CONF_BACKGROUND_IMAGE = "background_image"
styles(SWITCHPLATE_COMMON_STYLE_SCHEMA, CONF_GRANDIENT_DIRECTIONS, cv.use_id(image.Image_) )

CONF_BACKGROUND_IMAGE_SHIFT_X = "background_image_shift_x"
styles(SWITCHPLATE_COMMON_STYLE_SCHEMA, CONF_BACKGROUND_IMAGE_SHIFT_X, cv.int_range(-512,512) )

CONF_BACKGROUND_IMAGE_SHIFT_Y = "background_image_shift_x"
styles(SWITCHPLATE_COMMON_STYLE_SCHEMA, CONF_BACKGROUND_IMAGE_SHIFT_Y, cv.int_range(-512,512) )


CONF_BORDER_COLOR = "border_color"
styles(SWITCHPLATE_COMMON_STYLE_SCHEMA, CONF_BORDER_COLOR, rc.color )

CONF_BORDER_GRADIENT_FROM = "border_gradient_from"
styles(SWITCHPLATE_COMMON_STYLE_SCHEMA, CONF_BORDER_GRADIENT_FROM, rc.color )

CONF_BORDER_GRADIENT_TO = "border_gradient_to"
styles(SWITCHPLATE_COMMON_STYLE_SCHEMA, CONF_BORDER_GRADIENT_TO, rc.color )

CONF_BORDER_GRADIENT_DIR = "border_gradient_direction"
styles(SWITCHPLATE_COMMON_STYLE_SCHEMA, CONF_BORDER_GRADIENT_DIR, cv.enum(CONF_GRANDIENT_DIRECTIONS)  )

CONF_BORDER_RADIUS = "border_radius"
styles(SWITCHPLATE_COMMON_STYLE_SCHEMA, CONF_BACKGROUND_COLOR, cv.uint8_t )


CONF_TEXT_COLOR = "text_color"
styles(SWITCHPLATE_TEXT_STYLE_SCHEMA, CONF_TEXT_COLOR, rc.color )

CONF_TEXT_FONT  = "text_font"
styles(SWITCHPLATE_TEXT_STYLE_SCHEMA, CONF_TEXT_FONT, cv.use_id(font.Font) )

CONF_CIRCLE_WIDTH = "circle_width"
CONF_CIRCLE_COLOR = "circle_color"

CONF_SEGMENT_WIDTH = "segment_width"
CONF_SEGMENT_COLOR = "segment_color"

CONF_INDICATPOR_COLOR= "indicator_color"
CONF_KNOB_COLOR = "knob_color"
