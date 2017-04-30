/*
 * Smeter.cpp
 * S-meter class functions
 *
 * (c) KG5NII November 2016
 *
 * GNU Public License
 */

#include "Smeter.h"
#include "VfoConfig.h"

/* Enable the lcd class to be visible and accessable in this file. */
#ifdef LCD_I2C
  #include <LiquidCrystal_I2C.h>
/*
 * Since LiquidCrystal_I2C is initialized in VFO-KG5NII.ino file as a global,
 * it must be defined here in this file as an external global so we can access
 * the lcd class functions.
 */
  extern LiquidCrystal_I2C lcd;
#else
  #include <LiquidCrystal.h>
  extern LiquidCrystal lcd;
#endif




Smeter::Smeter()
{
    pinMode( SMETER_INPUT_PIN, INPUT_PULLUP );
    pinMode( XMITstatusLED, OUTPUT );
    pinMode( RECVstatusLED, OUTPUT );
    lastInputVal = 0;
    XMITplus =  false;
    XMITinputVal = 0;
    digitalWrite( XMITstatusLED, XMITplus );
    digitalWrite( RECVstatusLED, !XMITplus );
}

int Smeter::readSmeterInput()
{

/*
 * On Arduino you can convert an analog pin into a digital pin by using the analog pin alias (A0, A1, A2 etc)
 * to digitalRead() or digitalWrite().  This has several advantages in that  one can easy swap digital devices
 * with analogue ones and vice versa without having to manually change the pin it is connected to.   IN the code
 * below  SMETER_INPUT_PIN is utualized as a digita read pin.  I have a cw decoder that outputs a low when it receives audio
 * of the proper tone frequency, thus requiring that SMETER_INPUT_PIN be configured with INPUT_PULLUP; see the Smeter constructor above.
 *
 */

int cwdc = digitalRead(SMETER_INPUT_PIN);

if(lastInputVal != cwdc)
{

    lcd.setCursor(9,0);
   switch(cwdc)
   {
        case false: lcd.print("[+]");
                    delay(125);
                    break;

        case true: lcd.print("   ");
                   break;
   }
}
 lastInputVal = cwdc;
 return cwdc;
}


/*
 * getXMITstatus()  get the transceiver transmitter status.
 * returns:
 *        true,  if the transceiver is in transmit mode
 *        false, if the transceiver is in receive mode
 */

bool Smeter::getXMITstatus()
{
  XMITinputVal = analogRead(XMIT_PLUS_PIN);
  if( XMITinputVal <= 1024  &&  XMITinputVal > 64 )
  {
    XMITplus = true;
    return true;
  }
  else {
    XMITplus = false;
    return false;
  }
}

void Smeter::writeMeter( int level, char label )
{
  lcd.setCursor(0,0);
  lcd.write( label );
  lcd.setCursor(1,0);
  lcd.write( ' ' );
  lcd.setCursor(2,0);
  lcd.write( ' ' );
  lcd.setCursor(3,0);
  lcd.write( ' ' );
  lcd.setCursor(4,0);
  lcd.write( ' ' );
  lcd.setCursor(5,0);
  lcd.write( ' ' );
  switch( level )
  {
    case 40: lcd.createChar(4, meterS30); lcd.setCursor(5,0); lcd.write( 4 );
    case 30: lcd.createChar(4, meterS30);
    case 20: lcd.createChar(4, meterS20); lcd.setCursor(4,0); lcd.write( 4 );
    case 10: lcd.createChar(3, meterS10);
    case 9:  lcd.createChar(3, meterS9);  lcd.setCursor(3,0); lcd.write( 3 );
    case 7:  lcd.createChar(2, meterS7);
    case 5:  lcd.createChar(2, meterS5); lcd.setCursor(2,0); lcd.write( 2 );
    case 3:  lcd.createChar(1, meterS3);
    case 1:  lcd.createChar(1, meterS1); lcd.setCursor(1,0); lcd.write( 1 );
    case 0:
    default: break;
  }

}

void Smeter::updateMeter(int AdcVal, float volts)
{
  lcd.setCursor(7,0);
  lcd.print(AdcVal);
  lcd.print(",");
  lcd.print(volts);
  lcd.print("v ");
}


void Smeter::updateLEDxmitStatus()
{
      switch(XMITplus)
      {
        case true:
                        digitalWrite( XMITstatusLED, XMITplus );
                        digitalWrite( RECVstatusLED, !XMITplus );
                        break;

        case false:
                        digitalWrite( XMITstatusLED, XMITplus );
                        digitalWrite( RECVstatusLED, !XMITplus );
                        break;
      }

}
