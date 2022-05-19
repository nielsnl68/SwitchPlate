some test sources and work in progress.


prototype:
```yaml
  openHASP:
     display: [name of display] # Req
     id: ... # Opt.
     
     pages:
       - id: ... # Opt.
         objects:
           - object: button
               id: btnHello
               left: 0	     # Opt. Horizontal position on the page
               top: 0	       # Opt. Vertical position on the page
               width: 0	     # Opt. Width of the object
               height: 0	   # Opt. Height of the object
               toggle: false # When enabled, creates a toggle-on/toggle-off button. If false, creates a normal button
               state:	0	     # The value: 0 = unt oggled, 1 = toggled
               text: "" 	   # The text of the label
               mode: expand  # The wrapping mode of long text labels.
                             # 'expand' Expand the object size to the text size
                             # 'break' Keep the object width, break the too long lines and expand the object height
                             # 'dots' Keep the size and write dots at the end if the text is too long
                             # 'scroll' Keep the size and roll the text back and forth
                             # 'loop' Keep the size and roll the text circularly
                             # 'crop' Keep the size and crop the text out of it
               align: left   # Text alignment: left, center, right
           - object: button
              ...
 
  binary_sensor:
    - platform: openHASP
      object_id: btnHello
      id: key4
      name:  Hello screen button            
      
```
