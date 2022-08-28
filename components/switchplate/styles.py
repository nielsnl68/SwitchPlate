import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import core, automation
from esphome.automation import maybe_simple_id
from esphome.core import coroutine_with_priority



#from rgb_color import color
try:
     from . import rgb_color as rc    # "myapp" case
except:
     import rgb_color as rc


SWITCHPLATE_STYLE_SCHEMA = cv.Schema(
    {
    }
)

