/*
 * RTC.CPP
 * Real Time Clock class functions
 *
 * (c) KG5NII April 2017
 *
 * GNU Public License
 *
 */

#include <Arduino.h>
#include "rtc.h"
#include <Wire.h>

/* Enable the LCD class declaired in sketch to be visible in this file too */
#ifdef LCD_I2C
  #include <LiquidCrystal_I2C.h>
  extern LiquidCrystal_I2C lcd;
#else
  #include <LiquidCrystal.h>
  extern LiquidCrystal lcd;
#endif


RTC::RTC()
{
  clkIGN = false;

}

void RTC::setDS3231time(  byte second,
                          byte minute,
                          byte hour,
                          byte dayOfWeek,
                          byte dayOfMonth,
                          byte month,
                          byte year )
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0);                  // set next input to start at the seconds register
  Wire.write(decToBcd(second));   // set seconds
  Wire.write(decToBcd(minute));   // set minutes
  Wire.write(decToBcd(hour));     // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month));      // set month
  Wire.write(decToBcd(year));       // set year (0 to 99)
  Wire.endTransmission();
}

void RTC::readDS3231time( byte *second,
                          byte *minute,
                          byte *hour,
                          byte *dayOfWeek,
                          byte *dayOfMonth,
                          byte *month,
                          byte *year  )
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0);                    // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second     =   bcdToDec(Wire.read() & 0x7f);
  *minute     =   bcdToDec(Wire.read());
  *hour       =   bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek  =   bcdToDec(Wire.read());
  *dayOfMonth =   bcdToDec(Wire.read());
  *month      =   bcdToDec(Wire.read());
  *year       =   bcdToDec(Wire.read());
}

void RTC::displayTime()
{
    if(clkIGN == true)
      return;

      byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
      // retrieve data from DS3231
      readDS3231time( &second,
                      &minute,
                      &hour,
                      &dayOfWeek,
                      &dayOfMonth,month,
                      &year );


     if(minute == lastMinute)
          return

      lastMinute = minute;
      lcd.setCursor(15,0);

      if (hour < 10)
          lcd.print("0");

      lcd.print(hour,DEC);
      lcd.print(":");

        if(minute < 10)
            lcd.print("0");

      lcd.print(minute,DEC);
}


// Convert decimal numbers to BCD.
byte RTC::decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}

// Convert BCD to deccimal numbers
byte RTC::bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}

