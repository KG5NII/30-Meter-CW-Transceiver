/*
 * LedDisplay.h
 * LED Display class declarations
 *
 * (c) KG5NII April 2017
 *
 * GNU Public License
 */


#ifndef _LEDDISPLAY_H
#define _LEDDISPLAY_H

#include <Arduino.h>
#include "VfoConfig.h"

#define NDIGITS         8
#define NDIGITS_MASK    7

volatile void LED_irq(void);

class LedDisplay {

      public:
                LedDisplay();
                void displayString(char *inString);
                void displayFreq(long freq);

     private:
                byte  AsciiToSegmentValue(byte ascii);
};

#endif
