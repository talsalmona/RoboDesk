# RoboDesk
Arduino code for LOGICDATA protocol and remote handset hacking for automated standing desk

Mirrored from [mtfurlan/RoboDesk](https://github.com/mtfurlan/RoboDesk)

# Demo
![demo](https://github.com/talsalmona/RoboDesk/blob/master/demo/photo.jpg)
![demo](https://youtu.be/RAITo_iL8os)

# Current Functionality
* Preset low / high position by clicking up and down buttons simulataniously - audio feedback provided for successful storing of position. Last direciton in which the table moved defines if the high or low position is storred.
* Double tap up / down buttons to go to the high / low preset position

# Board
Currently works with an Arduino Uno or Nano because it requires 5v logic.
In the future we intend to move to an ESP8266 board.

# Current Hardware
* Arduino Nano
* DIN 7 connector
* Piezo buzzer

# Wiring
Can be found in [pins.h](pins.h)
