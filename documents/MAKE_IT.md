# How to make the your own ?

All electronic designs and software are open source. So, you can easily make your own board if you have some electronic skills.

## Produce the board with [JLCPBC](https://jlcpcb.com/)

The first step is : produce your board.
JLCPCB has interesting capacities : [PCB assembly](https://jlcpcb.com/smt-assembly).
They print your PCB and solder components on it.

It cost arround 40/50€ to produce the board.

The issue : they don't have all components.

You'll have to order the missing components on aliexpress or any electronic store and solder it yourself.

The major non included components are :
- SMD relays
- connectors
- Power supply capacitor
- Solid State Relay (DIP4 package)

Check the project here : https://oshwlab.com/Koxx3/smartlcd-v1-3_copy

## After ordering the board PCB

Order the missing components !

Don't forget to order connectors :
- JST SM (2/3/6 pins)
- JST PH 2.0 (2/3/6 pins)

You can also add the external sensors :
- WCS1700 : current sensor
- DHT22 : temperature and humity sensor

## Minimal soldering

### Jumper for direct throlle
![Jumper for direct throlle](/img/pcb_typon_mini1.png)

### Power supply capacitor
![Capacitor](/img/pcb_typon_mini2.png)

### JST-SM cables - display 
![JST-SM cables - display](/img/pcb_typon_mini4.png)

Minimotors wiring (left to right) :
- 1 - black - serial / ESP to LCD
- 2 - red - serial / LCD to ESP
- 3 - orange or blue - throttle signal
- 4 - white - GND
- 5 - yellow - battery feedback
- 6 - green - battery power

VESC wiring (left to right) :
- 1 - not connected
- 2 - not connected
- 3 - not connected
- 4 - not connected
- 5 - power switch
- 6 - power switch

### JST-SM cables - controller
![JST-SM cables - controller](/img/pcb_typon_mini5.png)

Minimotors wiring (left to right) :
- 1 - black - serial / controller to ESP
- 2 - red - serial / ESP to controller
- 3 - orange or blue - throttle signal
- 4 - white - GND
- 5 - yellow - battery feedback
- 6 - green - battery power

VESC wiring (left to right) :
- 1 - yellow - RX ESP / TX VESC
- 2 - white - TX ESP / RX VESC
- 3 - red - throttle
- 4 - black - GND
- 5 - not connected
- 6 - not connected

## Other components/connectors

### Brake output
![ESP32](/img/pcb_typon_max1.png)

Wiring :
- no pin order (relay)
- max voltage/current : depends of Solid state relay you placed

### Relay output
![ESP32](/img/pcb_typon_max2.png)

Wiring :
- no pin order (relay)
- max voltage/current : depends of Solid state relay you placed

### Brake input
![ESP32](/img/pcb_typon_max3.png)

Analog brake wiring (right to left):
- 1 - 5V
- 2 - GND
- 3 - signal

Digital brake wiring (right to left):
- 1 - brake witch
- 2 - not connected
- 3 - brake witch

### DHT22 external temperature/humidity sensor
![ESP32](/img/pcb_typon_max4.png)

Wiring (right to left):
- 1 - 5V
- 2 - GND
- 3 - signal


### WCS1700 current sensor
![ESP32](/img/pcb_typon_max5.png)

Wiring (right to left):
- 1 - 5V
- 2 - GND
- 3 - signal


### LCD display
![ESP32](/img/pcb_typon_max6.png)

Wiring (down to up):
- TBD

### Throttle power
![ESP32](/img/pcb_typon_max7.png)

Wiring (left to right):
- 1 - 5V
- 2 - GND

### Throttle signals
![ESP32](/img/pcb_typon_max8.png)

Wiring (left to right):
- 1 - ESP to controller
- 2 - output to controller
- 3 - input from display
- 4 - display to ESP

### Buttons & LEDs
![ESP32](/img/pcb_typon_max9.png)

Wiring (left to right):
- 1 - GND
- 2 - button 2
- 3 - button 1
- 4 - button 1 LED
- 5 - not connected
- 6 - not connected

### Solid State relays
![ESP32](/img/pcb_typon_max10.png)

Wiring :
- usually pin 1/2 are at the bottom. Check the SSR datasheet.
- 1 (bottom left) - IO 3.3V
- 2 (bottom right) - GND
- 3 (upper right) - IO out
- 4 (upper left) - IO out

## Finish with ESP32
![ESP32](/img/pcb_typon_mini3.png)

- leave right pin on the right unconnected (unused)
- USB port on the left side

## Wrap it
You need a wrap for 10.5cm / diameter 33mm
[Link](https://fr.aliexpress.com/item/4000787511699.html)

## Once you assembled the board and the components 

Upload the applications :D

[Next steps ...](RUN.md)

