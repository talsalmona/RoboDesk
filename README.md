[![Build Status](https://travis-ci.org/talsalmona/RoboDesk.svg?branch=master)](https://travis-ci.org/talsalmona/RoboDesk)

# RoboDesk
Arduino code for LOGICDATA protocol and remote handset hacking for automated standing desk

(LogicData data sheet can be found [here](https://web.archive.org/web/20180514132622/http://www.logicdata.net/wp-content/uploads/2017/05/Datasheet_Compact_English-Rev4.pdf))

Based on the great work of [mtfurlan](https://github.com/mtfurlan/RoboDesk) and [phord](https://github.com/phord/RoboDesk)

# Demo
![demo](https://github.com/talsalmona/RoboDesk/blob/master/demo/photo.jpg)
[![demo](https://img.youtube.com/vi/RAITo_iL8os/0.jpg)](https://www.youtube.com/watch?v=RAITo_iL8os)

# Current Functionality
* Preset low / high position by clicking up and down buttons simulataniously - audio feedback provided for successful storing of position. Last direciton in which the table moved defines if the high or low position is storred.
* Double tap up / down buttons to go to the high / low preset position

# Board
Currently works with an Arduino Uno or Nano because it requires 5v logic.

# Step by Step Setup
## Buy the Following Components
* [DIN 7 connector](https://www.aliexpress.com/item/OOTDTY-1Pc-Male-Female-DIN-Plug-Socket-Connector-3-4-5-6-7-8-PIN-Chassis/32860020971.html?spm=a2g0s.9042311.0.0.6c234c4djEkykF) - Make sure to get the 7 pins
* Some [header wires](https://www.aliexpress.com/item/40pcs-lot-10cm-2-54mm-1pin-Female-to-Female-jumper-wire-Dupont-cable/32378478740.html?spm=a2g0s.9042311.0.0.6c234c4djEkykF)
* Optional [Piezo Buzzer](https://www.aliexpress.com/item/5pcs-3-V-Active-Buzzer-Electromagnetic-SOT-Laminate-Tube-3-V-Is-The-Real-Thing/32651928778.html?spm=a2g0s.9042311.0.0.27424c4djytNqk)
* [Soldering iron](https://www.aliexpress.com/item/220V-60W-Portable-Electric-Soldering-Iron-Adjustable-Temperature-Solder-Iron-5pcs-Tips-Welding-Repair-Tool-kit/32815385816.html?spm=a2g0s.9042311.0.0.27424c4dAZ936J) - or borrow from a friend
* [Arduino Nano](https://www.aliexpress.com/item/Freeshipping-Nano-3-0-controller-compatible-for-arduino-nano-CH340-USB-driver-NO-CABLE/32341832857.html)


## Prepare the DIN7 Connector
1. Take note of the following diagram - it represents the pins when looking at the connector from the back (where you would need to solder the wires)

![DIN7-wiring](https://github.com/talsalmona/RoboDesk/blob/master/demo/din7-layout.png)

2. You will need to wire +5V, Ground, HS1, HS2 and RxD.

3. Cut one end of the wires and solder onto the DIN7 connector

4. Optionally, use epoxy glue to make sure the soldering holds

![DIN7-a](https://github.com/talsalmona/RoboDesk/blob/master/demo/din7a.jpg)
![DIN7-b](https://github.com/talsalmona/RoboDesk/blob/master/demo/din7b.jpg)

## Wiring the Board
Wire the board according to the mapping that can be found in [pins.h](pins.h)

## Upload the Code
The easiest way to upload the code is to open it in the Arduino IDE and flash it onto the board.

## Print a Case (WIP)
Use a 3D printer to print a case
Models are available in the [box](https://github.com/talsalmona/RoboDesk/blob/master/box) folder


