# SwitchPlate for esphome

This component allows a similar method of creating complex displays for espHome, as done in the openHASP project
It allows you to add buttons on the screen hook them up with esphome's switches and control them in you HA installation.

The best way to import this switchplate component is using the `external_components:` class:

Enjou playing with the current version

## How to start using the switchplate

First of all make sure that you testing this out on the DEV version of esphome.
As you can see there are a couple of PR's needed to have a good experience with the switchplate component. And the switchplate has only been tested with the ili9341 which is now part as a model of the ili9xxx family and the changed xpt2046 touchscreen driver.

**NOTE:** Also make sure you use the arduino version `2.0.3` and and platform version `4.4.0`. I make use of code that os only supported with this version combination.

```yaml
esp32:
  board: firebeetle32
  framework:
    type: arduino
    version: 2.0.3
    platform_version: 4.4.0

external_components:
  - source: github://pr#3796
    components: [display, animation]
    refresh: 10s
  - source: github://pr#3793
    components: [xpt2046]
    refresh: 10s
  - source: github://nielsnl68/Switchplate
    components: [switchplate, ili9xxx]
    refresh: 10s
 ```

## Config the switchplate

Currently the SwitchPlate supports the `Label`, `Button`, `Image`, `switch`, `checkbox` and `panel` widgets and has also 2 special use case widgets to show the page title and one for showing the current time or date.
This is just a start in the upcoming releases different widget's will be added. Which ones that will be is also depending on you asking for them.

With the following properties you can now setup the switchplate in the YAML config files:

```yaml
switchplate:
  id: my_plate
  display_id: my_display # Req 
  touchscreen_id: my_toucher
  default_font: my_font
  tabview: true/false
  thema:
     ... 
  pages:
    - id: myButtons
      title: Main buttons
      is_visible: true/false
      is_disabled: true/false
      selectable: true 
      widgets: 
          ...
  header_color: my_color
  header_height: 25
  header:
    - type: pagetitle 
      dimension:
        x: 2
        y: 0
        width: 198
        height: 20
    - type: datetime
      id: theTime
      time_id: the_time
      format: "%X"
      dimension:
        x: 240
        y: 0
        width: 120 
        height: 20
  footer_color: my_color
  footer_height: 25
  footer:
     ....
```

### The label widget

the following widgets can be set at the current time.
The below settings are for `label`, `pagetitle` and `datetime`.

```yaml
        - type: label
          id: txtHello
          content: "I am your SwitchPlate"
          is_pressed: true/false
          is_visible: true/false
          is_disabled: true/false
          is_selected: true/false
          clickable: true/false 
          selectable: true
          action: noting/home/prev/next

          background_color: my_color
          background_color_from: my_color
          background_color_to: my_color
          background_color_direction: horizontal/vertical

          border_color: my_color
          border_color_from: my_color
          border_color_to: my_color
          border_color_direction: horizontal/vertical

          text_fond: my_font
          text_align: top left
          text_mode: crop
          text_color_from: my_color
          text_color_to: my_color
          text_color_direction: horizontal/vertical

          dimension:
            x: 10
            y: 30
            width: 300 
            height: 20
```

### The button widget

Basicly every label or image component can be made `clickable` to become a button widget by it self.
The difference is that the button component is already setup to show the button on the screen.
You can use all the settings as shown with the label component.

When you want to use a image as button then use the image widget instead and make it `clickable`.

### The image widget

The following properties are for the `image` widget:

```yaml
        - type: image
          id: my_image_widget
          is_pressed: true/false
          is_visible: true/false
          is_disabled: true/false
          is_selected: true/false
          clickable: true/false 
          selectable: true/false
          action: noting/home/prev/next
          background_color: my_color  #styleable
          background_color_from: my_color #styleable
          background_color_to: my_color #styleable
          background_color_direction: horizontal/vertical #styleable

          border_color: my_color #styleable
          border_color_from: my_color #styleable
          border_color_to: my_color #styleable
          border_color_direction: horizontal/vertical #styleable

          image_id: my_image
          image_offset_x: 0
          image_offset_y: 0
          image_color: my_color #styleable
          image_color_from: my_color #styleable
          image_color_to: my_color #styleable
          image_color_direction: horizontal/vertical #styleable
          dimension:
            x: 10
            y: 30
            width: 300 
            height: 20
```

### The switch widget

The switch widget is basicly an extended version of the button component with the extra options how to show the switch. it can be shown horisontal and vertical based on the dimentsions set for the switch.
Making the width and heidth the same will show the switch as checkbox or radiobox.
Future more you can define of it is an rounded or a rectangled switch.

```yaml
        - type: switch
          ...
          id: my_switch
          switch_mode: rounded_mode/rect_mode #styleable
          foreground_color: my_color #styleable
          foreground_color_from: my_color #styleable
          foreground_color_to: my_color #styleable
          foreground_color_direction: horizontal/vertical #styleable

```

The `foreground` color is used to display the switch dot.

## Access buttons and Switches.

To control the buttons and switches you can use the `switch:` or the `binary_sensor` component. Those will act as a bridge between ESPHone, Home-Assistant and the switchplate. 
Using the `switch:` you can set the (select) state of the switch's switchplate platform where ever you you want and the change will be visable on the switchplate and in Home-Assistant.

```yaml
switch:
   - platform: switchplate
     widget_id: my_switch
     name: access my switch button
     restore_mode: RESTORE_FROM_SERVER | RESTORE_DEFAULT_OFF | RESTORE_DEFAULT_ON | ALWAYS_OFF | ALWAYS_ON | RESTORE_INVERTED_DEFAULT_OFF | RESTORE_INVERTED_DEFAULT_ON
     duration: 1s
```
All other properties of the `switch:` are valid as well. 

The other way is using the `binary_sensor` component:
```yaml
binary_sensor:
   - platform: switchplate
     widget_id: my_switch
     name: access my switch button
```
And all the properies avialable from the `binary_sensor` component. 

## Using styling properties

With the styleing properties you can define how the widget lookslike per widget status.
There are 5 different statuses, the default one, when pressed, when selected, when disabled, a combination of selected and pressed or disabled.
Per widget status you can use the properies that are marked `# styleable`.
To setup the styling for the different statussen just put the name of the status and added the style propety below it with the changed value.

```yaml
        - type: image
          id: banklamp
          image_id: my_image4
          image_color: my_light_blue
          border_radius: 8
          pressed:
            image_color: my_blue
            border_color: my_blue
          selected:
            image_color: my_yellow
            border_color: my_yellow
          pressed_selected:
            border_color: my_red

          selectable: true
          clickable: true
```

When there is no style property set for a special status then it will find the nearest status for pressed, selected, disabled it will check the default status. for the combi statuses first it checks the individual status before the default status.
When you use the `thema property` then that will be check first as well, see below.

### Theming your whole switchplate

When you want to do styling central at on place then you can use the `thema:` property. Per widget type you can exactly set how the display should be shown. How every you can still override this setting with the styling options in the widget it self.

```yaml
     thema:
        label: 
           ...
        button:
          ...
        etc.
```

## Example Relevant YAML

```yaml
# to run everything you need to use a resent version of the arduino platform and ioplatform
esp32:
  board: firebeetle32
  framework:
    type: arduino
    version: 2.0.3
    platform_version: 4.4.0


# Adds the switchplate component to your instance of esphome 
external_components:
  # development of the updated components needed to use the SwitchPlate 
  - source: github://pr#3796
    components: [display, animation]
    refresh: 10s
  - source: github://pr#3793
    components: [xpt2046]
    refresh: 10s
  # development of the SwitchPlate it self
  - source: github://nielsnl68/SwitchPlate
    # make sure that you are always on the latest code
    components: [switchplate, ili9xxx]
    refresh: 10s

time:
  - platform: homeassistant
    id: the_time

api:

color:
  - id: my_light_red
    red: 100%
  - id: my_black
    blue: 0%
    green: 0%
    red: 0%
  - id: my_red
    red: 100%
  - id: color_blue
    blue: 86%
  - id: my_white
    red: 100%
    blue: 100%
    green: 100%
  - id: my_blue
    red: 0%
    blue: 100%
    green: 0%
  - id: my_green
    red: 0%
    blue: 0%
    green: 100%
  - id: my_yellow
    red: 75%
    green: 75%
  - id: my_light_blue
    red_int: 96
    green_int: 96
    blue_int: 255

spi:
  mosi_pin: 23
  miso_pin: 19
  clk_pin: 18

font:
  - file: "gfonts://Roboto"
    id: my_font
    size: 20

display:
  - id: my_display
    platform: ili9xxx
    model: TFT 2.4
    cs_pin: 5
    dc_pin: 16
    rotation: 90
    auto_clear_enabled: false

touchscreen:
  - platform: xpt2046
    display: my_display
    id: my_toucher
    cs_pin: 14
    #irq_pin: 16
    setup_priority: 10
    report_interval: 50ms
    threshold: 300
    calibration_x_min: 3860
    calibration_x_max: 340
    calibration_y_min: 3860
    calibration_y_max: 340
    swap_x_y: false

switchplate:
  id: my_plate
  display_id: my_display # Req
  touchscreen_id: my_toucher
  default_font: my_font
  header:
    - type: pagetitle
      dimension:
        x: 2
        y: 0
        width: 198
        height: 20
    - type: datetime
      id: theTime
      time_id: the_time
      dimension:
        x: 240
        y: 0
        width: 120
        height: 20

  pages:
    - id: myButtons
      title: Woonkamer lampen
      selectable: true # can be opened with next and previous page selectors
      widgets:
        - type: image
          id: rgbLamp
          image_id: my_image0
          image_color: my_light_blue
          dimension:
            x: 5
            y: 30
            width: 96
            height: 96

    - id: myMainPage # Opt.
      title: Basic widgets
      selectable: true # can be opened with next and previous page selectors
      widgets:
        - type: label
          id: txtHello
          dimension:
            x: 10
            y: 30
            width: 300
            height: 20
          content: "I am your SwitchPlate"
        - type: button
          id: btnClick
          dimension:
            x: 60
            y: 180
            width: 200
            height: 40
          border_color: my_yellow
          content: "Click me"
          action: home

        - type: image
          id: my_sw_image
          image_id: my_image
          image_color: my_green
          border_radius: 8
          pressed:
            image_color: my_blue
            border_color: my_blue
          selected:
            image_color: my_yellow
            border_color: my_yellow

          selectable: true
          clickable: true
          dimension:
            x: 10
            y: 70
            width: 100
            height: 100
        - type: image
          image_id: my_animation
          image_color: my_blue
          dimension:
            x: 170
            y: 70
            width: 100
            height: 100

image:
  - file: "images/logo.png"
    id: my_image
    resize: 100x100
    type: TRANSPARENT_BINARY

animation:
  - file: "images/writingpen.gif"
    id: my_animation
    resize: 100x100
    type: TRANSPARENT_BINARY

interval:
  - interval: 200ms
    then:
      lambda: |-
        id(my_animation).next_frame();
        id(my_display).call_update();

switch:
  - platform: switchplate
    widget_id: my_sw_image
    name: ${device} my_sw_image
 
```

## Discussions

I love to hear from you when you have testing it. You can post your finding here or ping me on discord at:

* [ESPHome-Switchplate channel](https://discord.com/channels/538814618106331137/981129448394993664) in the openHASP Discord
* [Display architecture Changes needed? Thread](https://discord.com/channels/429907082951524364/960438726067114024) in the ESPHome Discord
* [Show-off thread](https://discord.com/channels/429907082951524364/1019881727159697418) in the ESPHome Discord

## Contributors

* @cpyarger made their first contribution in https://github.com/nielsnl68/SwitchPlate/pull/4
**I like to thank @fvanroie for helping me where he could. Without his work i never started this endeavour. Also i want to thank everyone that helped me while i was stuck with parts of the development. Without them i would ended where i am now.**
