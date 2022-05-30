from colorsys import hls_to_rgb
import esphome.config_validation as cv
from collections import namedtuple, OrderedDict

__all__ = ["color"]


def color(value):
    """Validate that the value is a hex RGB color.
        value can be a color name, a Hex value like #RRGGBB, a RGB(10,10,10), HSL(350deg,50%,100%)
        or just a positive integer value """
    if isinstance(value, str):
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
            value = value[4:-1].split(",",3)
            value = (value[0] << 16) +(value[1] <<8) + value[2]

        elif value.startswith("HSL("):
            value = value[4:-1].split(",",3)
            value[0] = cv.angle(value[0])/360
            value[1] = cv.percentage(value[1])
            value[2] = cv.percentage(value[2])
            clr = hls_to_rgb(value[0], value[1],value[2])

            value = (int(clr[0]* 255) << 16) +(int(clr[1]* 255) <<8) + int(clr[0]* 255)

        else:
            try:
                value = colors[value]
            except KeyError:
                # pylint: disable=raise-missing-from
                raise cv.Invalid("Color name does not exist. ("+value+")")
            value = value.int_format()
    if not isinstance(value, int):
        raise cv.Invalid("invalid color definition.")
    if value < 0 or value > 0xFFFFFF:
        raise cv.Invalid("invalid color value.")
    return value


Color = namedtuple('RGB','red, green, blue')
colors = {} #dict of colors
class RGB(Color):
    def hex_format(self):
        '''Returns color in hex format'''
        return '#{:02X}{:02X}{:02X}'.format(self.red,self.green,self.blue)
    def int_format(self):
        return (self.red << 16) +(self.green<<8) + self.blue

#Color Contants
ALICEBLUE = RGB(240, 248, 255)
ANTIQUEWHITE = RGB(250, 235, 215)
AQUA = RGB(0, 255, 255)
AQUAMARINE = RGB(127, 255, 212)
AZURE = RGB(240, 255, 255)
BANANA = RGB(227, 207, 87)
BEIGE = RGB(245, 245, 220)
BISQUE = RGB(255, 228, 196)
BLACK = RGB(0, 0, 0)
BLANCHEDALMOND = RGB(255, 235, 205)
BLUE = RGB(0, 0, 255)
BLUEVIOLET = RGB(138, 43, 226)
BRICK = RGB(156, 102, 31)
BROWN = RGB(165, 42, 42)
BURLYWOOD = RGB(222, 184, 135)
BURNTSIENNA = RGB(138, 54, 15)
BURNTUMBER = RGB(138, 51, 36)
CADETBLUE = RGB(95, 158, 160)
CADMIUMORANGE = RGB(255, 97, 3)
CADMIUMYELLOW = RGB(255, 153, 18)
CARROT = RGB(237, 145, 33)
CHARTREUSE = RGB(127, 255, 0)
CHOCOLATE = RGB(210, 105, 30)
COBALT = RGB(61, 89, 171)
COBALTGREEN = RGB(61, 145, 64)
COLDGREY = RGB(128, 138, 135)
CORAL = RGB(255, 127, 80)
CORNFLOWERBLUE = RGB(100, 149, 237)
CORNSILK = RGB(255, 248, 220)
CRIMSON = RGB(220, 20, 60)
CYAN = RGB(0, 238, 238)
DARKGOLDENROD = RGB(184, 134, 11)
DARKGRAY = RGB(169, 169, 169)
DARKGREEN = RGB(0, 100, 0)
DARKKHAKI = RGB(189, 183, 107)
DARKOLIVEGREEN = RGB(85, 107, 47)
DARKORANGE = RGB(255, 140, 0)
DARKORCHID = RGB(153, 50, 204)
DARKSALMON = RGB(233, 150, 122)
DARKSEAGREEN = RGB(143, 188, 143)
DARKSLATEBLUE = RGB(72, 61, 139)
DARKSLATEGRAY = RGB(47, 79, 79)
DARKTURQUOISE = RGB(0, 206, 209)
DARKVIOLET = RGB(148, 0, 211)
DEEPPINK = RGB(255, 20, 147)
DEEPSKYBLUE = RGB(0, 191, 255)
DIMGRAY = RGB(105, 105, 105)
DODGERBLUE = RGB(30, 144, 255)
EGGSHELL = RGB(252, 230, 201)
EMERALDGREEN = RGB(0, 201, 87)
FIREBRICK = RGB(178, 34, 34)
FLESH = RGB(255, 125, 64)
FLORALWHITE = RGB(255, 250, 240)
FORESTGREEN = RGB(34, 139, 34)
GAINSBORO = RGB(220, 220, 220)
GHOSTWHITE = RGB(248, 248, 255)
GOLD = RGB(255, 215, 0)
GOLDENROD = RGB(218, 165, 32)
GRAY = RGB(128, 128, 128)
GREEN = RGB(0, 128, 0)
GREENYELLOW = RGB(173, 255, 47)
HONEYDEW = RGB(240, 255, 240)
HOTPINK = RGB(255, 105, 180)
INDIANREDED = RGB(176, 23, 31)
INDIANRED = RGB(205, 92, 92)
INDIGO = RGB(75, 0, 130)
IVORY = RGB(255, 255, 240)
IVORYBLACK = RGB(41, 36, 33)
KHAKI = RGB(240, 230, 140)
LAVENDER = RGB(230, 230, 250)
LAVENDERBLUSH = RGB(255, 240, 245)
LAWNGREEN = RGB(124, 252, 0)
LEMONCHIFFON = RGB(255, 250, 205)
LIGHTBLUE = RGB(173, 216, 230)
LIGHTCORAL = RGB(240, 128, 128)
LIGHTCYAN = RGB(224, 255, 255)
LIGHTGOLDENROD = RGB(255, 236, 139)
LIGHTGOLDENRODYELLOW = RGB(250, 250, 210)
LIGHTGREY = RGB(211, 211, 211)
LIGHTPINK = RGB(255, 182, 193)
LIGHTSALMON = RGB(255, 160, 122)
LIGHTSEAGREEN = RGB(32, 178, 170)
LIGHTSKYBLUE = RGB(135, 206, 250)
LIGHTSLATEBLUE = RGB(132, 112, 255)
LIGHTSLATEGRAY = RGB(119, 136, 153)
LIGHTSTEELBLUE = RGB(176, 196, 222)
LIGHTYELLOW = RGB(255, 255, 224)
LIMEGREEN = RGB(50, 205, 50)
LINEN = RGB(250, 240, 230)
MAGENTA = RGB(255, 0, 255)
MANGANESEBLUE = RGB(3, 168, 158)
MAROON = RGB(128, 0, 0)
MEDIUMORCHID = RGB(186, 85, 211)
MEDIUMPURPLE = RGB(147, 112, 219)
MEDIUMSEAGREEN = RGB(60, 179, 113)
MEDIUMSLATEBLUE = RGB(123, 104, 238)
MEDIUMSPRINGGREEN = RGB(0, 250, 154)
MEDIUMTURQUOISE = RGB(72, 209, 204)
MEDIUMVIOLETRED = RGB(199, 21, 133)
MELON = RGB(227, 168, 105)
MIDNIGHTBLUE = RGB(25, 25, 112)
MINT = RGB(189, 252, 201)
MINTCREAM = RGB(245, 255, 250)
MISTYROSE = RGB(255, 228, 225)
MOCCASIN = RGB(255, 228, 181)
NAVAJOWHITE = RGB(255, 222, 173)
NAVY = RGB(0, 0, 128)
OLDLACE = RGB(253, 245, 230)
OLIVE = RGB(128, 128, 0)
OLIVEDRAB = RGB(107, 142, 35)
ORANGE = RGB(255, 128, 0)
ORANGERED = RGB(255, 69, 0)
ORCHID = RGB(218, 112, 214)
PALEGOLDENROD = RGB(238, 232, 170)
PALEGREEN = RGB(152, 251, 152)
PALETURQUOISE = RGB(187, 255, 255)
PALEVIOLETRED = RGB(219, 112, 147)
PAPAYAWHIP = RGB(255, 239, 213)
PEACHPUFF = RGB(255, 218, 185)
PEACOCK = RGB(51, 161, 201)
PINK = RGB(255, 192, 203)
PLUM = RGB(221, 160, 221)
POWDERBLUE = RGB(176, 224, 230)
PURPLE = RGB(128, 0, 128)
RASPBERRY = RGB(135, 38, 87)
RAWSIENNA = RGB(199, 97, 20)
RED = RGB(255, 0, 0)
ROSYBROWN = RGB(188, 143, 143)
ROYALBLUE = RGB(65, 105, 225)
SALMON = RGB(250, 128, 114)
SANDYBROWN = RGB(244, 164, 96)
SAPGREEN = RGB(48, 128, 20)
SEAGREEN = RGB(84, 255, 159)
SEASHELL = RGB(255, 245, 238)
SEPIA = RGB(94, 38, 18)
SGIBEET = RGB(142, 56, 142)
SGIBRIGHTGRAY = RGB(197, 193, 170)
SGICHARTREUSE = RGB(113, 198, 113)
SGILIGHTBLUE = RGB(125, 158, 192)
SGILIGHTGRAY = RGB(170, 170, 170)
SGIOLIVEDRAB = RGB(142, 142, 56)
SGISALMON = RGB(198, 113, 113)
SGISLATEBLUE = RGB(113, 113, 198)
SGITEAL = RGB(56, 142, 142)
SIENNA = RGB(160, 82, 45)
SILVER = RGB(192, 192, 192)
SKYBLUE = RGB(135, 206, 235)
SLATEBLUE = RGB(106, 90, 205)
SLATEGRAY = RGB(112, 128, 144)
SNOW = RGB(255, 250, 250)
SPRINGGREEN = RGB(0, 255, 127)
STEELBLUE = RGB(70, 130, 180)
TAN = RGB(210, 180, 140)
TEAL = RGB(0, 128, 128)
THISTLE = RGB(216, 191, 216)
TOMATO = RGB(255, 99, 71)
TURQUOISE = RGB(64, 224, 208)
TURQUOISEBLUE = RGB(0, 199, 140)
VIOLET = RGB(238, 130, 238)
VIOLETRED = RGB(208, 32, 144)
WARMGREY = RGB(128, 128, 105)
WHEAT = RGB(245, 222, 179)
WHITE = RGB(255, 255, 255)
WHITESMOKE = RGB(245, 245, 245)
YELLOW = RGB(255, 255, 0)

#Add colors to colors dictionary
colors['aliceblue'] = ALICEBLUE
colors['antiquewhite'] = ANTIQUEWHITE
colors['aqua'] = AQUA
colors['aquamarine'] = AQUAMARINE
colors['azure'] = AZURE
colors['banana'] = BANANA
colors['beige'] = BEIGE
colors['bisque'] = BISQUE
colors['black'] = BLACK
colors['blanchedalmond'] = BLANCHEDALMOND
colors['blue'] = BLUE
colors['blueviolet'] = BLUEVIOLET
colors['brick'] = BRICK
colors['brown'] = BROWN
colors['burlywood'] = BURLYWOOD
colors['burntsienna'] = BURNTSIENNA
colors['burntumber'] = BURNTUMBER
colors['cadetblue'] = CADETBLUE
colors['cadmiumorange'] = CADMIUMORANGE
colors['cadmiumyellow'] = CADMIUMYELLOW
colors['carrot'] = CARROT
colors['chocolate'] = CHOCOLATE
colors['cobalt'] = COBALT
colors['cobaltgreen'] = COBALTGREEN
colors['coldgrey'] = COLDGREY
colors['coral'] = CORAL
colors['cornflowerblue'] = CORNFLOWERBLUE
colors['cornsilk'] = CORNSILK
colors['crimson'] = CRIMSON
colors['cyan'] = CYAN
colors['darkgoldenrod'] = DARKGOLDENROD
colors['darkgray'] = DARKGRAY
colors['darkgreen'] = DARKGREEN
colors['darkkhaki'] = DARKKHAKI
colors['darkolivegreen'] = DARKOLIVEGREEN
colors['darkorange'] = DARKORANGE
colors['darkorchid'] = DARKORCHID
colors['darksalmon'] = DARKSALMON
colors['darkseagreen'] = DARKSEAGREEN
colors['darkslateblue'] = DARKSLATEBLUE
colors['darkslategray'] = DARKSLATEGRAY
colors['darkturquoise'] = DARKTURQUOISE
colors['darkviolet'] = DARKVIOLET
colors['deeppink'] = DEEPPINK
colors['deepskyblue'] = DEEPSKYBLUE
colors['dimgray'] = DIMGRAY
colors['dodgerblue'] = DODGERBLUE
colors['eggshell'] = EGGSHELL
colors['emeraldgreen'] = EMERALDGREEN
colors['firebrick'] = FIREBRICK
colors['flesh'] = FLESH
colors['floralwhite'] = FLORALWHITE
colors['forestgreen'] = FORESTGREEN
colors['gainsboro'] = GAINSBORO
colors['ghostwhite'] = GHOSTWHITE
colors['gold'] = GOLD
colors['goldenrod'] = GOLDENROD
colors['gray'] = GRAY
colors['green'] = GREEN
colors['greenyellow'] = GREENYELLOW
colors['honeydew'] = HONEYDEW
colors['hotpink'] = HOTPINK
colors['indianred'] = INDIANRED
colors['indigo'] = INDIGO
colors['ivory'] = IVORY
colors['ivoryblack'] = IVORYBLACK
colors['khaki'] = KHAKI
colors['lavender'] = LAVENDER
colors['lavenderblush'] = LAVENDERBLUSH
colors['lawngreen'] = LAWNGREEN
colors['lemonchiffon'] = LEMONCHIFFON
colors['lightblue'] = LIGHTBLUE
colors['lightcoral'] = LIGHTCORAL
colors['lightcyan'] = LIGHTCYAN
colors['lightgoldenrod'] = LIGHTGOLDENROD
colors['lightgoldenrodyellow'] = LIGHTGOLDENRODYELLOW
colors['lightgrey'] = LIGHTGREY
colors['lightpink'] = LIGHTPINK
colors['lightsalmon'] = LIGHTSALMON
colors['lightseagreen'] = LIGHTSEAGREEN
colors['lightskyblue'] = LIGHTSKYBLUE
colors['lightslateblue'] = LIGHTSLATEBLUE
colors['lightslategray'] = LIGHTSLATEGRAY
colors['lightsteelblue'] = LIGHTSTEELBLUE
colors['lightyellow'] = LIGHTYELLOW
colors['limegreen'] = LIMEGREEN
colors['linen'] = LINEN
colors['magenta'] = MAGENTA
colors['manganeseblue'] = MANGANESEBLUE
colors['maroon'] = MAROON
colors['mediumorchid'] = MEDIUMORCHID
colors['mediumpurple'] = MEDIUMPURPLE
colors['mediumseagreen'] = MEDIUMSEAGREEN
colors['mediumslateblue'] = MEDIUMSLATEBLUE
colors['mediumspringgreen'] = MEDIUMSPRINGGREEN
colors['mediumturquoise'] = MEDIUMTURQUOISE
colors['mediumvioletred'] = MEDIUMVIOLETRED
colors['melon'] = MELON
colors['midnightblue'] = MIDNIGHTBLUE
colors['mint'] = MINT
colors['mintcream'] = MINTCREAM
colors['moccasin'] = MOCCASIN
colors['navajowhite'] = NAVAJOWHITE
colors['navy'] = NAVY
colors['oldlace'] = OLDLACE
colors['olive'] = OLIVE
colors['olivedrab'] = OLIVEDRAB
colors['orange'] = ORANGE
colors['orangered'] = ORANGERED
colors['orchid'] = ORCHID
colors['palegoldenrod'] = PALEGOLDENROD
colors['palegreen'] = PALEGREEN
colors['paleturquoise'] = PALETURQUOISE
colors['palevioletred'] = PALEVIOLETRED
colors['papayawhip'] = PAPAYAWHIP
colors['peachpuff'] = PEACHPUFF
colors['peacock'] = PEACOCK
colors['pink'] = PINK
colors['plum'] = PLUM
colors['powderblue'] = POWDERBLUE
colors['purple'] = PURPLE
colors['raspberry'] = RASPBERRY
colors['rawsienna'] = RAWSIENNA
colors['red'] = RED
colors['rosybrown'] = ROSYBROWN

colors['royalblue'] = ROYALBLUE
colors['salmon'] = SALMON
colors['sandybrown'] = SANDYBROWN
colors['sapgreen'] = SAPGREEN
colors['seashell'] = SEASHELL
colors['sepia'] = SEPIA
colors['sgibeet'] = SGIBEET
colors['sgibrightgray'] = SGIBRIGHTGRAY
colors['sgichartreuse'] = SGICHARTREUSE
colors['sgilightblue'] = SGILIGHTBLUE
colors['sgilightgray'] = SGILIGHTGRAY
colors['sgiolivedrab'] = SGIOLIVEDRAB
colors['sgisalmon'] = SGISALMON
colors['sgislateblue'] = SGISLATEBLUE
colors['sgiteal'] = SGITEAL
colors['sienna'] = SIENNA
colors['silver'] = SILVER
colors['skyblue'] = SKYBLUE
colors['slateblue'] = SLATEBLUE
colors['slategray'] = SLATEGRAY
colors['snow'] = SNOW
colors['springgreen'] = SPRINGGREEN
colors['steelblue'] = STEELBLUE
colors['tan'] = TAN
colors['teal'] = TEAL
colors['thistle'] = THISTLE
colors['tomato'] = TOMATO
colors['turquoise'] = TURQUOISE
colors['turquoiseblue'] = TURQUOISEBLUE
colors['violet'] = VIOLET
colors['violetred'] = VIOLETRED
colors['warmgrey'] = WARMGREY
colors['wheat'] = WHEAT
colors['white'] = WHITE
colors['whitesmoke'] = WHITESMOKE
colors['yellow'] = YELLOW

colors = OrderedDict(sorted(colors.items(), key=lambda t: t[0]))