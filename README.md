# SwitchPlate
This component allows a similar method of creating complex displays for espHome, as done in the openHASP project

# Discussions
 * [ESPHome-Switchplate channel](https://discord.com/channels/538814618106331137/981129448394993664) in the openHASP Discord
 * [Display architecture Changes needed? Thread](https://discord.com/channels/429907082951524364/960438726067114024) in the ESPHome Discord
# Example Relevant YAML

```yaml

# custom colors are defined here
color:
  - id: my_light_red
    red: 100%
    
# Adds the switchplate component to your instance of esphome 
external_components:
  # if doing local development of the component
  #- source:
  #    type: local
  #    path: C:\development\projects\switchplate\components
  - source: github://nielsnl68/SwitchPlate
    # make sure that you are always on the latest code
    refresh: 0s
    
# Communication path that may be required for your display, This is an example, change to suit your display
spi:
  clk_pin: 14
  miso_pin: 15
  mosi_pin: 16

# Currently requires an existing working display component
display:
  - id: my_display
    platform: ili9341
    model: tft 2.4
    dc_pin: 10

# Actual Switchplate yaml

switchplate:
  display: my_display # Req 
  id: abcname # Opt.
  header:
    - type: label
      id: header_title
      text: SwitchPlate test
    - type: label
      id: header_time
      text: !lambda return "test";
  pages:
    - id: myMainPage # Opt.
      name: myName
      selectable: true # can be opened with next and previous page selectors
      objects:
        - type: button
          id: btnHello
          x: 10 # Opt. Horizontal position on the page
          y: 10 # Opt. Vertical position on the page
          width: 60 # Opt. Width of the object
          height: 20 # Opt. Height of the object
          toggle: false # True: creates a toggle-on/toggle-off button. False: creates a normal button
          state: false # 0 = untoggled, 1 = toggled
          text: "Hello " # The text of the label
          align: left # Text alignment: left, center, right
          mode: expand # The wrapping mode of long text labels
        - type: slider
          max_value: 100
          min_value: 00
          value: !lambda return 10;
    - id: otherPage
      objects:
        - type: label
          text:  !lambda return "Hello World";
        - type: spinner
          x: -10
          y: 20
          width: 60
          height: 60
          #circle_color: "#704020"
          #segment_color: red
```
