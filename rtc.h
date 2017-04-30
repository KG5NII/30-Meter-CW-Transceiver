/*
 * RTC.h
 *
 * (c) KG5NII April 2017
 *
 * GNU Public License
 *
 */

#ifndef RTC_H
#define RTC_H

  #include "VfoConfig.h"

  class RTC {

                public:
                            RTC();
                            void displayTime();
                            void setDS3231time(   byte second,
                                                  byte minute,
                                                  byte hour,
                                                  byte dayOfWeek,
                                                  byte dayOfMonth,
                                                  byte month,
                                                  byte year );

                private:
                            byte decToBcd(byte val);
                            byte bcdToDec(byte val);
                            void readDS3231time(  byte *second,
                                                  byte *minute,
                                                  byte *hour,
                                                  byte *dayOfWeek,
                                                  byte *dayOfMonth,
                                                  byte *month,
                                                  byte *year  );

                          bool clkIGN;
                          byte lastMinute;

  };

 #endif
