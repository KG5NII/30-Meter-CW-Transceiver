# 30-Meter CW Transceiver
This is the C/C++ code for my 30 meter Transceiver project.  The majority of
the code is not really frequency or band specific but one will have to make
changes to the VFO code to support different hardware and or bands.

The current code supports the AD9850 DDS which is widely available on the
internet.  However, I _WILL_ be switching over to the Si5351 DDS due to it's
cleaner output and more importantly, its multiple port capability.

The currently supported micro controller is the Arduino Mega2560. This is simply
just too big of a project to support the UNO, however, there is probably not
any technical reason the code could not be made to work on a UNO, but one would
have to cut down on the number of supported devices due to the UNO's limited
number digital I/O  pins.  YMMV,and you are on your own in that regard.

 The hardware used is the following:

1 Watter QRPp Transceiver at http://partsandkits.com/1watter-V5.php    ($49.00)

5 Watt 2-30 Mhz Classic C Amp at http://partsandkits.com/5W-A1-Amp.php ($29.00)

AD9850 DDS module as can be found on numerous places on Ebay           ($8-$20)

3x4 matrix keypad

4x20 LCD, I2c, SPI or 8 bit whatever.

8 digit 7 segment LED board using 74HC595 8-Bit Shift Registers (Ebay, amazon)

quadrature encoder adafruit .com

SPI or I2c FRAM  adafruit.com

HI-PER-MITE 200 hz audio CW Filter  http://www.4sqrp.com/HiPerMite.php

DS3231 Precision RTC adafruit.com and others

Voltage Regulators:  (2) 7812, (1) 7809

Capacitors various electrolytic: (1) 670 uF 16V or higher, (3) 100 uF 16V

Various 1 nF, 10 nF and 100 nF @50 volts

Various switches and controls

An aluminum chassis suitable in size with a front panel large enough to fit
your hardware.  Various knobs for the controls and tuning.

I do not as of yet have a BOM but hope to provide one on some level.


Other requirements:
While an amateur radio license is not required to own or build a transceiver,
an amateur radio license is necessary in order to put it on the air.  One can
use the receiver and use a dummy load in order to test the transmitter without
a license, but what is the purpose of building a transceiver and not put it on
the air?  In addition, this is a CW only transceiver. No SSB is currently
planned.  Please refer to the American Radio Relay League at
http://www.arrl.org/what-is-ham-radio for more information on ham radio and how
to get your amateur radio license.

Experience Level:
This is not a project for the beginner.   I suggest you have some from of kit
building experience, know how to solder electronic components, understand how
to work with and program Arduino micro controllers, some general understanding
of construction techniques, the use of hand and power tools and some basic
understanding how to work with metal (drilling, cutting etc).  Lots and lots of
patience!  This is not to say that a beginner should not attempt the project,
but I would highly suggest such a person have an Elmer (an experienced person
advising you) lest the mistakes you will inevitably make be costly ones!

In a nut shell:  If you can't read schematics, read and somewhat understand
component data sheets, improvise, adapt and overcome without having your hand
held, this project may not be for you.  I am not trying to discourage anyone,
but rather trying to keep anyone who doesn't possess the necessary skills to
successfully complete a project such as this, from becoming discouraged!  On
the other hand read this:

Can you take a completed PCB and install it onto a chassis, complete all the
necessary connections to it?  Can you interface the VFO you are going to
put together in the project to this PCB?   Can you deal with the wiring from
the micro controller to all of it's devices?  This project currently implements
point to point wiring for all module interconnections.  While this might change
in the future, I cannot be for certain when if ever.  Do you have an idea when
you should be using bypass capacitors and the reasons for using them?  Do you
enough about construction layout to keep the digital electronics and the analog
electronics electrically isolated such that they will not interfere with each
other? I suppose I can provide photographs of how I did it.  I am not the final
authority on how it should be done. The only wrong way of doing things in the
end is whether it works as expected.  There will always and I mean always be
trade offs.    You could do as I have done and use this as a baseboard for
experimentation and learning.  There is just no substitute in doing projects in
this manner and while it can be massively frustrating, time consuming and  some-
times expensive, there is just no better way to learn.  You have been advised
accordingly.  I would suggest slowly building up to something better and better
as your skill sets continue to improve.  We are all at various levels after all
but everyone has to start somewhere.

I will be glad to answer questions if I am able to answer them, but do not
expect me to do your project for you!


Required Arduino Libraries:

NOTE: (LM) means look in the Arduino Library Manager


Hackscribble Ferro Library v1.5 www.hackscribble.com   FRAM support lib.

Keypad Library v3.0 2015-09-18 http://playground.arduino.cc/Code/Keypad  (LM)

Button library v1.0 http://www.michael.net.nz/ (LM)

PushButton library v 2.0.0 (LM)

Rotary encoder handler v1.1 https://github.com/dapicester/Rotary

TimerOne v1.1.0 (LM)

LiquidCrystal I2C  www.dfrobot.com

LiquidCrystal   Built into Arduino IDE


Latest Arduino IDE 1.6.x
