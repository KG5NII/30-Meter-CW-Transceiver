/*
   VFO-KG5NII.ino
   Main VFO sketch

   (c) KG5NII November 2016, April 2017

   GNU Public License

   Based on code by AD7C
*/
#include "AD9850.h"             /* AD9850 DDS class */
#include "VFO.h"                /* VFO class */
#include "VfoDebug.h"           /* Enable varioddds Debugging options */
#include "rtc.h"                /* Real Time Clock Class */
#include "Smeter.h"             /* Smeter class */
#include "LedDisplay.h"         /* 7 Segment, 8 Digit LED Display */
#include "VfoConfig.h"          /* Hardware pin, buttons and other definitions */
#include "FreqOpts.h"           /* Frequency options, band, freq start ETC definitions */

#include <SPI.h>
#include <Wire.h>
#include <rotary.h>
#include <TimerOne.h>
#include <Keypad.h>
#include <Button.h>
#include <Pushbutton.h>


#ifdef LCD_I2C
    #include <LiquidCrystal_I2C.h>
    LiquidCrystal_I2C lcd(LCD_I2C_ADDR, LCD_COLS, LCD_ROWS);
#else
    #include <LiquidCrystal.h>
    LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
#endif


/*
   Maybe one day will move the Fram Hackscribble_Ferro stuff to its
   own file, but for now it reamins here.
*/
#include <Hackscribble_Ferro.h>
Hackscribble_Ferro fram(MB85RS64,FRAM_CS_PIN);
ferroResult result;
Hackscribble_FerroArray vfoArray(fram, 2, sizeof(uint32_t), result);
Hackscribble_FerroArray freqMemArray(fram, 20, sizeof(uint32_t), result);

#ifndef GLOBAL_NO_DEBUG
  #define SPP  Serial.print
  #define SPPL Serial.println
#endif

Rotary encoder = Rotary(ENCODER_A_PIN, ENCODER_B_PIN);

//volatile uint32_t rx = START_FREQ;


/* Initialize an instance of the AD9850. Class will automatically initialize the hardware when called like shown below */
AD9850 DDS;

/*
   Initialize an instance of the VFO.  Class will automatically initialize all necessary varialbles when called as shown below
   NOTE:  this is absolutely no reason preventing one from making the declaration as thus:  VFO Vfo[1];  This will give us two
   VFO's Vfo[0] and Vfo[1] respectively.HOWEVER:  I have not tested this.  YET.  I will eventually need multiple VFO support for
   split frequency operation, and I will deal with the issues when that time comes.
*/
VFO Vfo;

/*
   Initialize an instance of Smeter class;
   This is kicking my butt and I could use some help!
   Suggestions, and preferably code is welcome!
   I don't know why I am drawing a blank on the Smeter issue.
   If it's not accurate than what good is it?
*/
Smeter smeter;

Keypad kpd  = Keypad(
                      makeKeymap(keys),
                      rowPins,
                      colPins,
                      KEYPAD_ROWS,
                      KEYPAD_COLS
                    );

int curPos, keypadDigits = 0;
bool keypadEntry = false,keypadActive = false;
char keyEntryArray[10] = { 0 };


Pushbutton incButton( INC_PUSH_BUTTON );
Pushbutton filterButton( FILTER_PUSH_BUTTON );
Button freqUpButton( FREQ_UP_PIN, PULLUP, INVERT, DEBOUNCE_MS );
Button freqDnButton( FREQ_DN_PIN, PULLUP, INVERT, DEBOUNCE_MS );

enum { WAIT, INCR, DECR };
uint8_t STATE;
unsigned long rpt = REPEAT_FIRST;


/* Initialize an instance of the LED class */
LedDisplay LED;

/* Initialize an instance of the RTC class */
RTC rtc;

/*
 * setup()
 */
void setup() {
  analogReference(DEFAULT);
  Vfo.memstatus = 1;
  Vfo.rx = START_FREQ;

#ifndef GLOBAL_NO_DEBUG
  Serial.begin(9600);
  #ifdef SETUP_DEBUG
    SPPL(F("Serial initialized"));
  #endif
#endif

//lcd.createChar(0,block);
#ifdef LCD_I2C
  lcd.begin();
  lcd.backlight();
#else
  lcd.begin(LCD_COLS, LCD_ROWS);
#endif
  lcd.clear();

  /*
     If FRAM fails to initialize just loop forever forcing us to address the issue
     This could use a little cleanup due to the redudant use of strings just wasting
     memory.
  */
if (fram.begin() == ferroBadResponse)
{
#ifndef GLOBAL_NO_DEBUG
    #ifdef SETUP_DEBUG
      SPPL(F("Damn. No Fram.  Cannot Continue!"));
    #endif
#endif

    lcd.setCursor(0, 0);
    lcd.print(F("Damn No FRAM"));
    lcd.setCursor(0, 1);
    lcd.print(F("Cannot continue!"));

    #ifdef LOOP_ON_FRAM_FAIL
      while (1);
    #else
      delay(5000);
    #endif
}
 lcd.clear();

  /*
     EncodeIsr() is the Interrupt Service Routine for the tuning encoder.
     Could use just a single IRQ but it seems to work smoother with using 2.
  */
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN), EncoderIsr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN), EncoderIsr, CHANGE);

  /*
     Retrieve the last stored frequency from the FRAM
     and then make sanity checks. Default it to START_FREQ
     if sanity checks fail.
  */
  Vfo.rx = readMEM(VFOA);

#ifndef GLOBAL_NO_DEBUG
  #ifdef SETUP_DEBUG
    SPP(F("Vfo.rx: "));
    SPPL(Vfo.rx);
  #endif
#endif

  /* Do some sanity checks */
  if (Vfo.rx > 0 )
  {
    if (Vfo.rx < LowerLimit || Vfo.rx > UpperLimit)
      Vfo.rx = START_FREQ;
  }
  else
      Vfo.rx = START_FREQ;

  /*
     Note: For some reason it is necessary to send the frequency
     to the DDS twice else it will not initialize right at startup.
     Wat sup wit dat?
  */
  DDS.sendFrequency(Vfo.rx);
  LED.displayFreq(Vfo.rx);
  //Vfo.displayFreq(Vfo.rx);
  DDS.sendFrequency(Vfo.rx);

  delay(100);
  Vfo.memstatus = 1;

  lcd.setCursor(0, 0);
  //	smeter.writeMeter(40,'s');
  Vfo.setLCDdefaults();

// set the initial time here:
// DS3231 seconds, minutes, hours, dayInWeek, day, month, year
//rtc.setDS3231time(00,29,04,6,15,04,17);
//rtc.displayTime();

}


void loop()
{
      bool xmitStatus = smeter.getXMITstatus();
      smeter.updateLEDxmitStatus();
      readKeyPad();


  /*
     The code below will write the last dialed in frequency to the FRAM
     SO.... Change the frequency with the tuning dial,the EncoderISR
     will set Vfo.memstatus to 0.  If more than Vfo.timepased + WRITE_FRAN_TIME milliseconds
     have passd since the frequency was changed, it will be written to FRAM.
  */
  if (Vfo.memstatus == 0 )
  {
    if ( Vfo.timepassed  + WRITE_FRAM_TIME < millis() )
      writeMEM(Vfo.rx, VFOA);
  }



  /*
     Experimental.  I have an homebrew LM567 tone decoder pcb  connected to the audio output
     of rig.  It has an single output pin that outputs low when it receives a valid cw tone,
     otherwise it outputs high. I connect this the the Analog A5 line (see the comments in Smeter as to why I
     connect a digital line to an Arduino analogue input line.   The code below displays [+] in the center
     of the first line on the display whenever it receives a valid cw tone.  I use this as a tuning indicator
     since I'm kinda tone death from too many years of listening to rock n roll music that was just way too loud.
     It only requires a single line of code as the class takes care of the rest of the implementation.

  */

    int SMstatus = smeter.readSmeterInput();

    checkButtons();

    #ifdef JOYSTIK_DEFINED
      checkJoyStik();
    #endif

  /*
     Update the display if the frequency has been changed.
     A change in frequency is denoted by the Vfo.changed_f flag.

     If the Encoder is envoked, Vfo.changed_f flag is changed in EncoderIsr(). If
     the JoyStik is envoked, Vfo.changed_f flag is changed in Vfo.setFrequency().
     Once the Vfo.changed_f flag is set to 1, the code below is executed; The LCD
     Display is Updated to the new frequency and the new frequency is sent to
     DDS.sendFrequency() in order to change the actual frequency of the DDS. After
     all this magick occurs,the Vfo.changed_f flag is reset to zero and the main
     loop execution is then continued at the top of the loop.
  */
  if (Vfo.changed_f)
  {
    LED.displayFreq(Vfo.rx);
//  Vfo.displayFreq(Vfo.rx);
    DDS.sendFrequency(Vfo.rx);
    Vfo.changed_f = 0;
  }
  rtc.displayTime();
} // loop()



/*
   EncoderIsr() is the Interupt Service Routine for the rotary encoder.
   Try to refrain from calling any functions other than encoder.process()
   from within the ISR.  Note that the Vfo.changed_f flag is set to 1.  This
   flag being set to 1 will cause the new frequency to be sent to the DDS via
   DDS.sendFrequency() located in the main loop() function.
*/
void EncoderIsr() {
  unsigned char result = encoder.process();
  switch (result)
  {
    case DIR_CW:    Vfo.rx += Vfo.increment;
      break;

    case DIR_CCW:   Vfo.rx -= Vfo.increment;
      break;

    case DIR_NONE:
      break;
  }

  Vfo.changed_f = 1;

  /* Frequency sanity checks */
  if (Vfo.rx >= UpperLimit) {
    Vfo.rx = UpperLimit;
  }
  else if (Vfo.rx <= LowerLimit) {
    Vfo.rx = LowerLimit;
  }
}



/*
   Writing to arduino EEPROM is lame in this application as it has a finite
   number of write cycles. After that, it will no longer function.  *BAD*!
   Ferroelectric RAM or FRAM for short is non-volatile random access memory, is
   cheap and is several orders of magnitude faster & more efficient than EEPROM.
   FRAMs have data retention times of more than 10 years at +85°C  and up to
   many decades at lower temperatures. Additionally FRAMs have lower power
   requirements than FLASH memory.  Currently, for the Arduino there are two
   types of FRAM interfaces available: SPI & I2C. This implementation uses the
   I2C version as the use of SPI is just overkill.

   The long term goal is to create data structures that contain all the
   parameters (be it defaults or user set via menus) the transceiver needs
   in order to operate. We will write this data to nonvolatile FRAM memory and
   read it from the FRAM upon powering up the transceiver.  The code as it is
   right now, stores the frequency as discussed in earlier comments.  Also,
   the stored frequency is recalled from FRAM when the transceiver is first
   turned on so as to remember the last frequency that was in the display before
   the rig was shutdown.  So this is only the beginning, but it's a start.

   There are currently 2 functions in this implementation supporting FRAM:
   readMEM() & writeMEM() respectively.

   In readMEM(), note that both the Vfo.memstatus flag AND the Vfo.changed_f
   flag are set to 1.

   The Vfo.changed_f flag is set to 1 because the frequency in memory has
   changed, and setting this flag to 1 ensures the frequency change is sent to
   the DDS via DDS.sendFrequency().

   The Vfo.memstatus flag is set to 1 since we have already read data from FRAM,
   we have no need to read it again at this particular point in time.

   readMEM() returns the data read from FRAM as a uint32_t instead of directly
   changing the global frequency variable Vfo.rx.

   readMEM() returns 0 if the read data fails the sanity checks. Else it returns
   the uint32_t frequency that was read from the FRAM.

   In writeMEM(), the Vfo.memstatus flag is set to 1 since we have already
   written data to the FRAM and have no need to write any more data at this
   particular point in time.

   writeMEM() currently does not return any status code on successful or
   unsuccessful write to the FRAM.  This may change in future revisions. If you
   absolutely need to know if the write was successful, one could read the value
   just written and compare it to the original value.

   Since we are using VFO as a class, it is entirely a design criteria that
   multiple VFO arrays be supported.  This is why both of these routines accept
   a VfoNum parameter.
*/
uint32_t readMEM(uint8_t VfoNum)
{
  uint32_t tVfo = 0;
  vfoArray.readElement(VfoNum, (uint8_t*)&tVfo, result);

#ifndef GLOBAL_NO_DEBUG
    #ifdef READMEM_DEBUG
      SPP(F("Value read into vfo: "));
      SPPL(tVfo);
    #endif
#endif

  if (tVfo < F_MIN || tVfo > F_MAX)
  {

#ifndef GLOBAL_NO_DEBUG
  #ifdef READMEM_DEBUG
      SPP(F("readMEM(): value read was either too high or too low.  Value: "));
      SPPL(tVfo);
  #endif
#endif

    return 0;
  }
  Vfo.memstatus = 1;
  Vfo.changed_f = 1;
  return tVfo;
}


void writeMEM(uint32_t Tvfo, uint8_t VfoNum)
{

#ifndef GLOBAL_NO_DEBUG
  #ifdef WRITEMEM_DEBUG
    SPP(F("writeMEM( "));
    SPP(Tvfo); SPP(F(","));
    SPP(VfoNum);
    SPPL(F(")"));
  #endif
#endif

  vfoArray.writeElement(VfoNum, (uint8_t *)&Tvfo, result);

#ifndef GLOBAL_NO_DEBUG
  #ifdef WRITEMEM_DEBUG
    SPP(F("Write to VFOArray result code: "));
    SPPL(result);
    #endif
#endif

  Vfo.memstatus = 1;
}






void checkButtons()
{
    if( incButton.getSingleDebouncedPress() )
        Vfo.setIncrementUp();

    if( filterButton.getSingleDebouncedPress() )
        Vfo.updateFilterStatus();

  freqUpButton.read();
  freqDnButton.read();

  switch(STATE)
  {
      case WAIT:
                  if( freqUpButton.wasPressed() )
                  STATE = INCR;
                  else if( freqDnButton.wasPressed() )
                          STATE = DECR;
                else if( freqUpButton.wasReleased() )
                          rpt = REPEAT_FIRST ;
                else if( freqDnButton.wasReleased() )
                          rpt = REPEAT_FIRST;
                else if( freqUpButton.pressedFor(rpt) )
                    {
                          rpt += REPEAT_INCR;
                          STATE = INCR;
                    }
              else if ( freqDnButton.pressedFor(rpt) )
                    {
                          rpt += REPEAT_INCR;
                          STATE = DECR;
                   }
                break;

    case INCR:
                Vfo.rx = Vfo.setFrequency( Vfo.rx, UP_FREQ );
                STATE = WAIT;
                break;

   case DECR:
                Vfo.rx = Vfo.setFrequency( Vfo.rx, DN_FREQ );
                STATE = WAIT;
                break;
    }
}



    /*
       The joyStik adheres to the following definitions:

                       UL  U  UR
                        +  +  +
                         \ | /
                          \|/
                     L+----+----+R
                          /|\
                         / | \
                        +  +  +
                       LL  D  LR


         U  = Change Freq up
         D  = Change Freq down
         L  = Cycle through step rates in decreasing order
         R  = Cycle through step rates in increasing order

         UL = Upper Left
         LL = Lower Left
         UR = Upper Right
         LR = Lower Right

         Vfo.setFrequency() sets Vfo.changed_f to 1 & memstatus to 0
         And this is the mechanism to used in the main loop to send
         the new frequency to the DDS and to write this new value to
         FRAM (if necessary). Please refer to  the previous comments
         at the beginning of loop() on how all this works and ties together.
    */
void CheckJoyStik()
{
#ifdef JOYSTIK_DEFINED
      int joyStick = Vfo.readJoyStik();

      switch(joyStick)
      {
            case UP:
                              Vfo.rx = Vfo.setFrequency(Vfo.rx, UP_FREQ);
                              delay(30);
                              break;

            case DOWN:
                              Vfo.rx = Vfo.setFrequency(Vfo.rx, DN_FREQ);
                              delay(30);
                              break;

            case LFT:
                              Vfo.setIncrementUp();
                              delay(100);
                              break;

            case RGT:
                              Vfo.setIncrementDn();
                              delay(100);
                              break;

            case UPPERLEFT:
                              break;

            case UPPERRIGHT:
                              break;

            case LOWERLEFT:
                              Vfo.setIncrementUp();
                              delay(100);
                              break;

            case LOWERRIGHT:
                             Vfo.setIncrementDn();
                             delay(100);
                             break;
    }
  #endif
}


bool readKeyPad()
{
    uint32_t keypadFreq;
    char key = kpd.getKey();
    if(key)
    {
      switch(key)
      {
        case '*':                                             // clear key
                  for(register int i = 0; i < 10; i++)
                     keyEntryArray[i] = '\0';

                  keypadDigits = 0;
                  keypadEntry = 0;
                  keypadActive = false;
                  curPos=7;
                  clearBlock();
                  break;

        case '#':                                             // enter key
                  clearBlock();
                  keypadFreq = (uint32_t) atol(keyEntryArray);

                  if (keypadFreq < F_MIN || keypadFreq > F_MAX)
                  {

                    Vfo.memstatus = 1;
                  }
                  else
                    {
                      Vfo.rx = keypadFreq;
                      Vfo.changed_f = 1;
                      Vfo.memstatus = 0;
                    }

                   for(register int i = 0; i < 10; i++)
                      keyEntryArray[i] = '\0';

                   keypadEntry =  0;
                   keypadDigits = 0;
                   curPos = 7;
                   keypadActive = false;
                   return 0;

        default:
                  keypadActive = true;
                  if(keypadEntry == 0 && keypadDigits == 0)
                  {

                /*
                 * Since we are a monobander, there is no point in having to
                 * enter the first three digits of the frequency since they
                 * will always be 101.  This saves 3 keystrokes.
                 */
                    keyEntryArray[0] = '1';
                    keyEntryArray[1] = '0';
                    keyEntryArray[2] = '1';
                    keyEntryArray[3] = key;
                    clearBlock();
                    lcd.setCursor(0,1);
                    lcd.print(F("Entry: "));
                    curPos = 7;
                    lcd.setCursor(curPos,1);
                    lcd.print(keyEntryArray);
                    curPos = 11;
                    keypadEntry = 1;
                    keypadDigits = 4;
                  }
                  else
                       if(keypadDigits < 8 && keypadDigits > 0)
                      {
                        lcd.setCursor(curPos,1);
                        lcd.print(key);
                        keyEntryArray[keypadDigits] = key;
                        keypadDigits++;
                        curPos++;
                      }
                      break;
      }
        return 1;
   }
}


void clearBlock()
{
  lcd.noBlink();
  lcd.setCursor(0,1);
  lcd.print(F("               "));
}
