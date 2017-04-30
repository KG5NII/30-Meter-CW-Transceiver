/*
 * AD9850.h
 *
 * (c) KG5NII November 2016
 *
 * GNU Public License
 *
 * Based on code by AD7C
 */

#ifndef AD9850_H
#define AD9850_H

#include <SPI.h>
#include "VfoConfig.h"
#include "FreqOpts.h"


class AD9850 {
	public:
           AD9850();
           void sendFrequency(uint32_t frequency);

          uint32_t iffreq, Frequency;
          const int fcal = -685;

	private:
          // Pinouts for our AD9850 DDS chip
          #define pulseHigh(pin) {digitalWrite(pin, HIGH); digitalWrite(pin, LOW);}
		      void tfr_byte(byte data);
};

#endif
