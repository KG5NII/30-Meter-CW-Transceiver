/*
 * Smeter.h
 * S-meter class declarations
 *
 * (c) KG5NII November 2016
 *
 * GNU Public License
 */


#ifndef _SMETER_H
#define _SMETER_H

#include <Arduino.h>
#include "VfoConfig.h"



class Smeter {

    public:
             int   sMeterInputVal;
             bool  XMITplus;
             float voltage;

             Smeter();
             int  readSmeterInput();
             bool getXMITstatus();
             void updateLEDxmitStatus();
             void writeMeter( int level, char label );
             void updateMeter(int AdcVal, float volts);

    private:
             int lastInputVal, lastDB, XMITinputVal;
             const float vref = 4.994;

             byte meterS1[8] = {
                          B00000,
                          B00000,
                          B00000,
                          B00000,
                          B00000,
                          B00000,
                          B11000,
                          B11000
              };

              byte meterS3[8] = {
                          B00000,
                          B00000,
                          B00000,
                          B00000,
                          B00000,
                          B00011,
                          B11011,
                          B11011
              };

              byte meterS5[8] = {
                          B00000,
                          B00000,
                          B00000,
                          B00000,
                          B11000,
                          B11000,
                          B11000,
                          B11000
              };

              byte meterS7[8] = {
                          B00000,
                          B00000,
                          B00000,
                          B00011,
                          B11011,
                          B11011,
                          B11011,
                          B11011
              };

              byte meterS9[8] = {
                          B00000,
                          B00000,
                          B11000,
                          B11000,
                          B11000,
                          B11000,
                          B11000,
                          B11000
              };

              byte meterS10[8] = {
                          B00000,
                          B00011,
                          B11011,
                          B11011,
                          B11011,
                          B11011,
                          B11011,
                          B11011
              };

              byte meterS20[8] = {
                          B11000,
                          B11000,
                          B11000,
                          B11000,
                          B11000,
                          B11000,
                          B11000,
                          B11000
              };

              byte meterS30[8] = {
                          B11011,
                          B11011,
                          B11011,
                          B11011,
                          B11011,
                          B11011,
                          B11011,
                          B11011
              };
};

#endif
