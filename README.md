# SwitchPlate
This component allows a similar method of creating complex displays for espHome, as done in the openHASP project

# Discussions
 * [ESPHome-Switchplate channel](https://discord.com/channels/538814618106331137/981129448394993664) in the openHASP Discord
 * [Display architecture Changes needed? Thread](https://discord.com/channels/429907082951524364/960438726067114024) in the ESPHome Discord
 * [Show-off thread](https://discord.com/channels/429907082951524364/1019881727159697418) in the ESPHome Discord

# Example Relevant YAML

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
# More Examples

~~For further examples of what this component can do, check out the [documentation here](https://github.com/nielsnl68/SwitchPlate/blob/main/components/readme.md)~~