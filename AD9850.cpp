                                                                                                                                                                             /*
 * AD9850.cpp
 *
 * (c) KG5NII November 2016
 *
 * GNU Public License
 *
 * Based on code by AD7C
 */

//Make the Vfo class visible and accessable in this file.
#include "VFO.h"
extern VFO Vfo;

#include "AD9850.h"


/*
 *
 * AD9850() constructor is automatically executed upon initializion.
 * There it always runs init() automatically.
 *
 */


AD9850::AD9850()
{
  pinMode(DDS_FQ_UD, OUTPUT);
  pinMode(DDS_W_CLK, OUTPUT);
  pinMode(DDS_DATA,  OUTPUT);
  pinMode(DDS_RESET, OUTPUT);

  pulseHigh(DDS_RESET);
  pulseHigh(DDS_W_CLK);
  pulseHigh(DDS_FQ_UD);        // this pulse enables serial mode on the AD9850 - Datasheet page 12.

  iffreq = IF_FREQ;
}

/*
 * sendFrequency sends a unit32_t frequency to the DDS.
 * Vfo.changed_f & Vfo.memstatus are both set to 0
 * in this function.
 *
 */
void AD9850::sendFrequency(uint32_t frequency)
{
   frequency +=  fcal + iffreq;

// note 125 MHz clock on 9850.  You can make 'slight' tuning variations here by adjusting the clk freq.
   uint32_t freq = (frequency + fcal) * 4294967295 / 125000000;

  for (register int b = 0; b < 4; b++, freq >>= 8)
  {
    tfr_byte(freq & 0xFF);
  }

  tfr_byte(0x000);    // Final control byte, all 0 for 9850 chip
  pulseHigh(DDS_FQ_UD);   // Done!  Should see output

  Vfo.changed_f = 0;  //Since the freq is now updated in the DDS, reset Vfo.changed_f to 0
  Vfo.memstatus = 0; // Enable a possible memory write to FRAM.

}

/*
 *
 * Transfer a byte of data to the DDS
 *
 */
void AD9850::tfr_byte(byte data)
{
	for (register int i = 0; i < 8; i++, data >>= 1) {
    		digitalWrite(DDS_DATA, data & 0x01);
    		pulseHigh(DDS_W_CLK);         //after each bit sent, CLK is pulsed high
  	}
}
