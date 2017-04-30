/*
 * VFO.h
 * VFO class declarations
 *
 * (c) KG5NII November 2016
 *
 * GNU Public License
 *
 * Based on code by AD7C
 */

#ifndef VFO_H
#define VFO_H

#include <Arduino.h>


class VFO {
	public:
      		VFO();
      		void setIncrementUp();
      		void setIncrementDn();
          void updateFilterStatus();
					#ifdef JOYSTIK_DEFINED
      			int readJoyStik();
					#endif
          void setLCDdefaults();
          void displayFreq(uint32_t currFreq);
          volatile uint32_t setFrequency(uint32_t currFreq, short direction);

      		volatile bool    changed_f;
      		volatile int32_t timepassed;
      		volatile int32_t increment;
					volatile uint32_t rx;

      		// value to notify if memory is current or old. 0=old, 1=current.
      		volatile int  memstatus;
      		volatile int  hertzPosition;
          byte   ones, tens, hundreds, thousands, tenthousands, hundredthousands, millions;
          String hz01,hz10,hz50,hz100,hz500,hz1kHZ,period,hz,eraseLine;
          String  hertz,freq,band,mode;

  private:
          void   updateLCDIncrement();
          bool   filterState;

};

#endif
