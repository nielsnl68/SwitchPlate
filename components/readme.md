some test sources and work in progress.



## Cheatsheet

| widgets       | Type   | Description
|:----------|:-------|:-----------
| frame       | Visual | [Base Object](#base-object)
| line | Visual | [Line](#line)
| img | Visual | [Image](#image)
| button    | Binary | [Button](#button)
| switch    | Toggle | [Switch](#switch)
| checkbox  | Toggle | [Checkbox](#checkbox)
| label     | Visual | [Label](#text-label)
| led       | Visual | [LED](#led-indicator)
| spinner   | Visual | [Spinner](#spinner)
| dropdown  | Selector | [Dropdown List](#dropdown-list)
| roller    | Selector | [Roller](#roller)
| cpicker   | Selector | [Color picker](#color-picker)
| bar       | Range | [Progress Bar](#progress-bar)
| slider    | Range | [Slider](#slider)
| arc       | Range | [Arc](#arc)
| linemeter | Range | [Line Meter](#line-meter)
| gauge     | Range | [Gauge](#gauge)

| tabs       | Type   | Description
|:----------|:-------|:-----------
| tabview | Selector | [Tabview](#tabview)
| tab      | Selector | [Tab](#tab)

| dialogs       | Type   | Description
|:----------|:-------|:-----------
| btnmatrix | Selector | [Button Matrix](#button-matrix)
| msgbox | Selector | [Messagebox](#messagebox)

## Common

| Property | Value        | Required | Default | Description
|:---------|:------------:|:--------:|:-------:|:----
| `id`       | 1..254       | yes      | n/a     | ID of the object on this page. `0` for the page itself.
| `type`      | widgets | yes      | n/a     | Name of the widgets _(see above)_
| groupid  | 0..15        | no       | 0 (none)| ID of the [GPIO group][3] the object belongs to
| `left`        | int16   | no       | 0       | Horizontal position on the page
| `top`        | int16   | no       | 0       | Vertical position on the page
| `width`        | int16   | no       | 0       | Width of the object
| `height`        | int16   | no       | 0       | Height of the object
| `enabled`  | bool    | no       | true    | Object is touchable, if `false`, a _disabled_ style is applied
| `visible`   | bool   | no       | true   | is Object visible
| opacity  | 0..255       | no       | 255     | How much the the object is opaque

| Method   | Description |
|:---------|:------------|
| `clear`    | Delete the children from the object
| `delete`   | Delete the object and its children from the page
| `to_front` | Bring the object to the front on the page
| `to_back`  | Send the object to the back on the page

| Actions    | Description
|------------|-----------------
| `p1` to `p12` | to switch to the corresponding page numbers directly (for ESP32; in case of ESP8266 it's only `p1` to `p4`)
| `prev`     | to switch to the previous page
| `next`     | to switch to the next page
| `home`     | to go back to the home page

| Modes | Description
|-------|------------------------
| `expand` | expand the object size to the text size
| `break` | keep the object width, break the too long lines and expand the object height
| `dots` | keep the size and write dots at the end if the text is too long
| `scroll`| Keep the size and roll the text back and forth
| `loop` | Keep the size and roll the text circularly
| `crop` | Keep the size and crop the text out of it

| Directions  | Description  
| `0` | down
| `1` | up
| `2` | left
| `3` | right
_Note:_ up and down are superseeded by the screen size.

| Alignments | Description
|------------|------------------------
| `left`     |
| `center`   |
| `right`    |

## frame

## Spinner
| Property    | Value      | Default | Description
|-------------|------------|---------|--------------
| `speed`       | int16 | 1000    | The time for 1 turn in ms
| `direction`   | int16 | 0       | `0` = clockwise, `1` = counter-clockwise
| `angle`       | 0-360      | 60      | The length of the spinning segment in degrees
| `mode`        | 0-2        | 0       | `0` = slow down on the top, `1` = slow down and stretch, `2` = constant speed no stretch
| `line_width`  | int16 | 20    | The width of the background circle
| `line_width1` | int16 | 20    | The width of the spinning segment
| `line_color`  | color | themed | color of the background circle
| `line_color1` | color | themed | color of the spinning segment

## polyLine

| Property | Value        | Default | Description
|----------|--------------|---------|--------------------------
| `points`   | JSON array | ""  | The name of the image file
| `auto_size` | bool    | true    | Automatically set the size of the line object to the outer bounds
| `y_invert` | bool    | false   | The y direction might be counter-intuitive in some cases so the y coordinates can be inverted

## Image

| Property | Value        | Default | Description
|----------|--------------|---------|--------------------------
| `src`       | string | ""      | The name of the image file
| `auto_size` | bool    | true    | Automatically set the size of the image object to the image source
| `offset_x` | int16   | 0       | Shift the picture horizontally relative to the image object
| `offset_y` | int16   | 0       | Shift the picture vertically relative to the image object
| `zoom`    | uint16  | 256     | A larger value enlarges the images (e.g. `512` double size), a smaller value shrinks it (e.g. `128` half size). Fractional scale works as well. E.g. `281` for `10%` enlargement.
| `angle`    | int16   | 0       | Rotate the picture around its pivot point. Angle has `0.1` degree precision, so for `45.8°` use `458`.
| `pivot_x`  | int16   | H center| The pivot point of the rotation, by default centered
| `pivot_y`  | int16   | V center| The pivot point of the rotation, by default centered
| `antialias` | bool    | false   | The quality of the angle and zoom transformation. With enabled anti-aliasing the transformations has a higher quality but they are slower.

# input/display widgets

## LED Indicator

| Property   | Value      | Default | Description
|------------|------------|---------|---------------
| `value`    | byte       | 0       | The brightness of the indicator [`0..255`]

## Text Label

| Property | Value      | Default    | Description
|----------|------------|------------|--------------
| `text`     | string | "Text" | The text of the label, `\n` for line break. Can also be a variable.
| `mode`     | Modes  | `crop` | The wrapping mode of long text
| `align`    | string | `left` | Text alignment

## Button

| Property | Value      | Default | Description
|----------|------------|---------|--------------
| `toggle`   | bool  | false   | When enabled, creates a toggle-on/toggle-off button. If false, creates a normal button
| `value`    | int16      | 0       | The value: `0` = untoggled, `1` = toggled

| `text`     | string     | ""      | The text of the label
| `mode`     | Modes      | `expand`| The wrapping mode of long text
| `align`    | string     | `left`  | Text alignment

## Switch

| Property   | Value      | Default | Description
|------------|------------|---------|---------------
| `value`        | bool  | false   | `1` = on, `0` = off
| **bg_color1**  | color | 0       | changes indicator color
| **bg_color2**  | color | 0       | changes knob color
| `radius2`      | int16 | themed  | changes knob corner radius

## Checkbox

| Property | Value      | Default    | Description
|----------|------------|------------|--------------
| `value`  | bool       | false      | `1` = checked, `0` = unchecked

| `text`   | string     | "Checkbox" | The label of the checkbox
| `mode`   | Modes      | `expand`| The wrapping mode of long text
| `align`  | string     | `left`  | Text alignment


## Dropdown List

| Property   | Value      | Default | Description
|------------|------------|---------|--------------------------
| `direction`  | Directions | down  | Direction where the dropdown expands
| `options`    | string     | ""    | List of items separated by `\n`
| `value`      | int16      | 0     | The number of the selected item
| `text`       | string     | ""    | **RO** The text of the selected item
| `show_selected` | bool | true  | Show the selected option or a static text
| `max_height` | int16      | 3/4 of screen height | The maximum height of the open drop-down list 

| Method   | Parameters |  Description
|----------|----------|--------------------------
| open     |          | To manually open the drop-down list
| close    |          | To manually close the drop-down list

## Roller

| Property | Value      | Default | Description
|----------|------------|---------|--------------------------
| `options`  | string   | ""      | List of items separated by `\n`
| `rows`     | int8     | 3       | The number of rows that are visible. | `value`    | int16    | 0       | The number of the selected item
Use this property instead of `height` to set object height
| `text`     | string     | ""       | **RO** The text of the selected
| `infinite` | bool       | false    | Roller mode: normal or infinite
| `align`    | Alighments | `center` | Text alignment

## Color picker

| Property | Value      | Default | Description
|----------|------------|---------|--------------
| `color`    | color | 0       | The selected color in html format #rrggbb
| `scale_width` | uint16     | 25      | The width of the color gradient of the circle
| `pad_inner` | int16       | 10      | The padding between the circle and the inner preview circle

| Event     | Description
|-----------|------------
| down    | (old color),
| changed | (repeatedly until released)
| up      |  (value at the moment of releasing it):

# value indicators

## Slider

| Property | Value      | Default | Description
|----------|------------|---------|---------------
| `min`      | int16      | 0       | minimum value of the indicator
| `max`      | int16      | 100     | maximum value of the indicator
| `value`      | int16      | 0       | current value of the indicator
| **start_value** | int16   | 0       | optional minimal allowed value of the indicator

## Progress Bar

| Property | Value      | Default | Description
|----------|------------|---------|---------------
| `min`      | int16      | 0       | minimum value of the indicator
| `max`      | int16      | 100     | maximum value of the indicator
| `value`      | int16      | 0       | current value of the indicator
| **start_value** | int16   | 0       | optional minimal allowed value of the indicator

## Gauge

| Property       | Value      | Default | Description
|----------------|------------|---------|---------------
| `min`          | int16      | 0       | minimum value of the indicator
| `max`          | int16      | 100     | maximum value of the indicator
| `value`        | int16      | 0       | current value of the indicator

| `critical_value` | int16      | 80      | scale color will be changed to scale_end_color after this value
| `label_count`    | uint8      |         | number of labels (and major ticks) of the scale
| `line_count`     | uint16     | 31      | number of minor ticks of the entire scale
| `angle`          | 0-360      | 240     | angle between start and end of the scale
| `rotation`       | 0-360      | 0       | offset for the gauge's angles to rotate it
| `format`         | uint16     | 0       | divider for major tick values<br>  `0` : print the major tick value as is<br>  `1` : strip 1 zero, i.e. divide tick value by 10 before printing the major tick label<br>  `2` : strip 2 zeros, i.e. divide tick value by 100 before printing the major tick label<br>  `3` : strip 3 zeros, i.e. divide tick value by 1000 before printing the major tick label<br>  4` : strip 4 zeros, i.e. divide tick value by 10000 before printing the major tick label

## Arc

| Property  | Value      | Default | Description
|-----------|------------|---------|--------------
| `min`       | int16 | 0       | minimum value of the indicator
| `max`       | int16 | 100     | maximum value of the indicator
| `value`     | int16 | 0       | current value of the indicator

| `rotation`  | int16 | 0       | offset to the 0 degree position
| `mode`      | 0-2        | 0       | `0` = normal, `1` = symmetrical, `2` = reverse
| `adjustable` | bool  | false   | Add knob that the user can operate to change the value
| `start_angle` | 0-360      |         | start angle of the arc background (see note)
| `end_angle`  | 0-360      |         | end angle of the arc background (see note)
| `start_angle1` | 0-360     |         | start angle of the arc indicator (see note)
| `end_angle1` | 0-360     |         | end angle of the arc indicator (see note)

## Line Meter

| Property       | Value      | Default | Description
|----------------|------------|---------|---------------
| min            | int16      | 0       | minimum value of the indicator
| max            | int16     | 100     | maximum value of the indicator
| val            | int16      | 0       | current value of the indicator
| angle          | 0-360      | 240     | angle between start and end of the scale
| line_count     | uint16     | 31      | tick count of the scale
| rotation       | 0-360      | 0       | offset for the scale angles to rotate it
| type           | 0-1        | 0       | `0` = indicator lines are activated clock-wise<br>`1` = indicator lines are activated counter-clock-wise


# popup dialogs

## Button Matrix

| Property | Value            | Default  | Description
|----------|------------------|----------|--------------
| options  | json array  | "Text"   | Json array of [strings] where each element is the label of a button. Use `"\n"` for a new line of buttons
| align    | string     | `center` | Text alignment: `left`, `center`, `right` 
| toggle   | bool        | false    | All buttons behave as toggle buttons or normal buttons
| one_check| bool        | false    | Allow only one button to be checked (toggled) at once
| val      | int8       | 0        | The number of the active button, starting at 0. In conjunction with `toggle` and `one_check` set it to `-1` to deactivate all buttons

## Messagebox
| Property   | Value           | Default | Description
|------------|-----------------|---------|--------------------------
| text       | string    | ""      | The text of the message to be displayed.
| options    | json array ["OK"]  | Json array of [string] where each element is the label of a button
| ~~modal~~  | bool       | false   | Make the messagebox a modal dialog requiring user input
| auto_close | int16      | 0       | Close the pop-up message automatically after this number of milliseconds have passed

# obsolite

## Tabview

| Property | Value        | Default | Description
|----------|--------------|---------|--------------------------
| val      | int8    | 0       | The number of the active tab, starting at 0
| text     | string | ""      | The name of the active tab
| btn_pos  | 0..4         | 1       | Position of the tab buttons:</br>`0` = none</br>`1` = top</br>`2` = bottom</br>`3` = left</br>`4` = right
| count    | uint16  | 0       | **RO** The number of tabs of the tabview

## Tab  

| Property | Value        | Default | Description
|----------|--------------|---------|--------------------------
| parentid | int8    | 0       | The `id` of the tabview object to which this tab is added
| text     | string | "Tab"   | The name of tab button

# Implementation

## prototype

```yaml
  external_components:
    # use rtttl and dfplayer from ESPHome's dev branch in GitHub
    # equivalent shorthand for GitHub
    - source: github://nielsnl68/SwitchPlate

  switchplate:
    display: [name of display] # Req
    id: ... # Opt.
    header:
      objects:
        ...
    footer:
      objects:
        ...  
    pages:
      - id: ... # Opt.
        name: myName
        firstpage: false;
        selectable: true    # can be opened with next and previous page selectors
        objects:
          - type: button
            id: btnHello
            left: 0          # Opt. Horizontal position on the page
            top: 0           # Opt. Vertical position on the page
            width: 0         # Opt. Width of the object
            height: 0        # Opt. Height of the object
            toggle: false    # True: creates a toggle-on/toggle-off button. False: creates a normal button
            state: 0         # 0 = untoggled, 1 = toggled
            text: ""         # The text of the label
            align: left      # Text alignment: left, center, right
            text_mode: expand     # The wrapping mode of long text labels

           - type: button
              ...
 
  binary_sensor:
    - platform: openHASP
      object_id: btnHello
      id: key4
      name:  Hello screen button            
      
```
