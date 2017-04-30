/*
 * VFO.cpp
 * VFO class functions
 *
 * (c) KG5NII November 2016, April 2017
 *
 * GNU Public License
 *
 * Based on code by AD7C
 *
 * Please note that noone is trying to rip off someone elses code
 * Please let me know of any code attribution(s) I've missed
 */

#include "VFO.h"
#include "VfoDebug.h"
#include "AD9850.h"
#include "FreqOpts.h"
#include "VfoConfig.h"

#include <string.h>

/* Enable the LCD class declaired in sketch to be visible in this file too */
#ifdef LCD_I2C
  #include <LiquidCrystal_I2C.h>
  extern LiquidCrystal_I2C lcd;
#else
  #include <LiquidCrystal.h>
  extern LiquidCrystal lcd;
#endif

#include <Keypad.h>

VFO::VFO()
{
  increment     =   50;
  hertzPosition =   5;
  changed_f     =   0;
  memstatus     =   1;

  hz01      =   F("01 Hz ");
  hz10      =   F("10 Hz ");
  hz50      =   F("50 Hz ");
  hz100     =   F("100 Hz");
  hz500     =   F("500 Hz");
  hz1kHZ    =   F(" 1 kHz");
  hertz     =   DEFAULT_FREQ_STEP;
  band      =   F("30M");
  mode      =   F("CW");
  period    =   F(".");
  hz        =   F("   Hz");
  eraseLine =  F("                    ");
  filterState = false;
  timepassed = millis();
}


/*
 * DisplayFreq() will display the frequency passed
 * to it on the LCD.  The timepassed variable is
 * updated at the end of this function.
 *
 * Based on code by AD7C
 */

void VFO::displayFreq(uint32_t currFreq)
{
	millions          =   int(currFreq / 1000000);
	hundredthousands  =   ((currFreq / 100000) % 10);
	tenthousands      =   ((currFreq / 10000) % 10);
	thousands         =   ((currFreq / 1000) % 10);
	hundreds          =   ((currFreq / 100) % 10);
	tens              =   ((currFreq / 10) % 10);
	ones              =   ((currFreq / 1) % 10);

lcd.setCursor(0,1);
lcd.print(eraseLine);

  if (millions > 9)
      lcd.setCursor(5,1);
   else
      lcd.setCursor(6,1);

  lcd.print(millions);
  lcd.print(period);
  lcd.print(hundredthousands);
  lcd.print(tenthousands);
  lcd.print(thousands);
  lcd.print(period);
  lcd.print(hundreds);
  lcd.print(tens);
  lcd.print(ones);
 // lcd.print(hz);
  timepassed = millis();
}




/*
 * setFrequency() is like the EncoderISR() function only
 * it is not an Interrupt Service Routine.  It is used
 * by the joyStik up & down positions for frequency change
 * via the joyStik. And just like EncoderISR() the flags
 * changed_f and memstatus are set to 1 and 0 respectively.
 * Additionally the timepassed variable is updated.
 * This function retuns the uint32_t  frequency which is the
 * new updated frequency
 */
volatile uint32_t VFO::setFrequency(uint32_t currFreq,short direction)
{

  switch(direction)
  {
  	case DN_FREQ:
  		            currFreq -= increment;
  		            break;
  	case UP_FREQ:
  		            currFreq += increment;
  		            break;
  }

/* Frequency sanity checks */
  	if(currFreq > F_MAX)
  	  currFreq = UpperLimit;
  else
  	if(currFreq < F_MIN)
      currFreq = LowerLimit;

changed_f = 1;
memstatus = 0;
timepassed = millis();

return currFreq;
}


void VFO::setIncrementUp()
{
  switch (increment)
  {

      case 1:     increment = 10;
                  hertz = hz10;
                  break;

      case 10:    increment = 50;
                  hertz = hz50;
                  break;

      case 50:    increment = 100;
                  hertz = hz100;
                  break;

      case 100:   increment = 500;
                  hertz = hz500;
                  break;

      case 500:
                  increment = 1000;
                  hertz = hz1kHZ;
                  break;

     case 1000:
                  increment = 1;
                  hertz = hz01;
                  break;
  }

  updateLCDIncrement();

}



void VFO::setIncrementDn()
{
  switch (increment)
  {

      case 1:       increment = 500;
                    hertz = hz500;
                    break;

      case 10:      increment = 1;
                    hertz = hz01;
                    break;

      case 50:      increment = 10;
                    hertz = hz10;
                    break;

      case 100:     increment = 50;
                    hertz = hz50;
                    break;

      case 500:     increment = 100;
                    hertz = hz100;
                    break;
  }

  updateLCDIncrement();
}


void VFO::updateLCDIncrement()
{
  lcd.setCursor(0,3);
  lcd.print(hertz);
}

void VFO::setLCDdefaults()
{
  lcd.setCursor(0,0);
  lcd.print(USER_CALLSIGN);

  /*
  lcd.setCursor(17,1);
  lcd.print(band);
*/

  lcd.setCursor(0,3);
  lcd.print(hertz);

  lcd.setCursor(9,3);
  lcd.print(F("F: off"));

  lcd.setCursor(18,3);
  lcd.print(mode);
}

/*
 * WTF is this?  I am currently using the JoyStik Button
 * to toggle inserting a 300hz CW filter into the audio chain via
 * a 5v relay and a 2n2222 transistor. The audio output of the rig
 * connects to my own audio amplifier circuit AND also the CW Filter
 * too, so that they are both operational at the same time.  This is lame
 * I know but I just want to KISS and be done with it.   The 5v relay
 * switches the speaker between the audio amp output or the CW filter output
 * The NC contacts on the relay connect the rigs speaker to the normal audio
 * output.  If the relay is engaged,by setting FILTER_PIN to HIGH (+5V) it
 * turns on the transistor and  the speaker is then connected to the output
 * of the CW filter which contains it's own little LM386 audio output chip.
 * .
 * This circuit is described as follows:
 *
 *                               [+5V]
 *                                 |
 *                              ___|___
 *                             | 5V    |
 *                             | Relay |
 *                             | Coil  |
 *                             |_______|
 *                                 |
 *                                 |
 *                              | / C
 * +[FILTER_PIN]------[150R]---B|         2N2222
 *                              | \ E
 *                                |
 *                                |
 *                              [GND]
 *
 *
 *                                           [NO Term]
 *                                            /     \
 *                                           o------o-[CW Filter Output]
 *                                                  /-[CW Filter Output GND]
 *                                                 |
 *                                               [GND]
 *
 *                                           [NC Term]
 *                     /-[Common Term]-\      /     \
 * [RIG SPKR]-<-------o----------------o---->o------o-[Rig Audio Output]
 * [RIG SPKR]--\                                    /-[Rig Audio Output GND]
 *             |                                   |
 *           [GND]                               [GND]
 *
 *
 * NOTE:  The above diagram neglects to show the snubber diode  that is placed across the coil
 * of the relay.  Any diode should be OK.  A 1N4148A small signal diode will work fine.  Connect
 * the diode's cathode to the relay coil pin that is connected to +5V.  Connect the diode's anode
 * to the remaining relay coil pin (The one connected to the 2N2222's Collector). The [150R] is a
 * 150 Ohm resistor with one end connected to the base of the transistor and the other end connected
 * to the preferred microcontroller digital output pin (the one configured as FILTER_PIN in VfoConfig.h).
 *
 * updateFilterStatus() updates the LCD showing the status of the filter  I.E.  on or off.
 * Adjust this function to suite your configuration.
 *
 */

void VFO::updateFilterStatus()
{
  String filterStatusStr,filterStatusBuf;

  filterState = !filterState;

  switch(filterState)
  {
      case CW_FILTER_WIDE:
                              digitalWrite(FILTER_ENABLE_PIN, CW_FILTER_WIDE);
                              filterStatusStr = F("off");
                              break;

      case CW_FILTER_300HZ:
                              digitalWrite(FILTER_ENABLE_PIN, CW_FILTER_300HZ);
                              filterStatusStr = F("on ");
                              break;
  }
  filterStatusBuf = F("F: ");
  filterStatusBuf += filterStatusStr;

  lcd.setCursor(9,3);
  lcd.print(filterStatusBuf);
}


/*
 * Read the joystick positions.
 * Please see VfoButtons.h for the joyStik
 * pin & position definitions..
 */

#ifdef JOYSTIK_DEFINED
int VFO::readJoyStik()
{

int JoyX = 0;
int JoyY = 0;

JoyX = analogRead(JOYSTIK_X);
JoyY = analogRead(JOYSTIK_Y);


#ifndef GLOBAL_NO_DEBUG
  #ifdef JOYSTIK_DEBUG
      SPP(F("JoyX: "));
      SPPL(JoyX);
      SPP(F("JoyY: "));
      SPPL(JoyY);
  #endif
#endif

  if(JoyX == 0)
  {
  		if(JoyY == 0)
  			return joyDiagUL;
  	else
  		if(JoyY >= 384 && JoyY <= 512)
  			return joyL;
  	else
  		if(JoyY >= 992 && JoyY <= 1024)
  			return joyDiagLL;
  } else

  if(JoyX >= 480 && JoyX <= 512)
  {
  		if(JoyY >= 480 && JoyY <= 512)
  			return joyN;
  	else
  		if(JoyY >=0 && JoyY <= 16)
  			return joyU;
  	else
  		if(JoyY >= 992 && JoyY <=1024)
  			return joyD;
  } else

  if(JoyX >=992 && JoyX <= 1024)
  {
  		if(JoyY >=0 && JoyY <= 512)
  			return joyR;
  	else
  		if(JoyY > 992 && JoyY <= 1024)
  			return joyDiagLR;
  }

return joyN;
}
#endif
