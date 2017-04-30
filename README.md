# 30-Meter CW Transceiver
This is the C/C++ code for my 30 meter Transceiver project.  The majority of the code is not
really frequency or band specific but one will have to make changes to the vfo code to
support different hardware and or bands. 

The current code supports the AD9850 DDS which is widely available on the internet.
However, I _WILL_ be switching over to the Si5351 DDS due to it's cleaner output and
more imporantly, its multiple port capability. 

The currently supported microcontroller is the Arduno Mega2560. This is simply just too 
big of a project to support the UNO, however, there is probably not technical reason the
code could not be made to work on a UNO, but one would have to cut down on the number of
supported devices due to the UNO's limited number digital I/O  pins.  YMMV but you are on
your own in that reguard. 

 The hardware used is the following:
 
Universal 1 Watter QRPp Transceiver at http://partsandkits.com/1watter-V5.php  ($49.00)

5 Watt 2 - 30 Mhz Classic C Amp at http://partsandkits.com/5W-A1-Amp.php       ($29.00)

AD9850 DDS module as can be found on numerous places on Ebay and Amazon        ($8 - $20)

3x4 matrix keypad

4x20 LCD

8 digit 7 segment LED board using 74HC595 8-Bit Shift Registers (Ebay, amazon)

quadrature encoder adafruit .com 

SPI or I2c FRAM  adafruit.com

HI-PER-MITE 200 hz audio CW Filter  http://www.4sqrp.com/HiPerMite.php

DS3231 Precision RTC adafruit.com and others

Voltage Regulators:  (2) 7812, (1) 7909

Capacitors various electrolytics: (1) 670 Uf 16V or higher, (3) 100 uf 16V 

Various 1 nf, 10 nf and 100 nf @50 volts 

Various switches and controls 

An aluminum chassis suitable in size with a front panel large enough to fit your hardware.
Various knobs for the controls and tuning. 

I do not as of yet have a BOM but hope to provide one on some level.


Other requirements:
While an amateur radio license is not required to own or build a transceiver,
an amateur radio license is necessary in order to put it on the air.  One can
use the receiver and use a dummy load in order to test the transmitter without
a license, but what is the purpose of building a transceiver and not put it on
the air?  In addition, this is a CW only transceiver. No SSB is currently planned.
Please refer to the American Radio Relay League at http://www.arrl.org/what-is-ham-radio
for more information on ham radio and how to get your amateur radio license. 

Experience Level:
This is not a project for the beginner.   I suggest you have some from of kit building experience,
know how to solder electronic components, understand how to work with and program Arduino micro
controllers, some general understanding of construction techniques, the use of hand and power
tools and some basic understanding how to work with metal (drilling, cutting etc).  Lots and
lots of patience!  This is not to say that a beginner should not attempt the project, but I would
highly suggest such a person have an elmer (an experienced person advising you) lest the mistakes
you will inevitably make be costly ones! 


