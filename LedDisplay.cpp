/*
 * LedDisplay.cpp
 * LED Display class functions
 *
 * (c) KG5NII April 2017
 *
 * GNU Public License
 */
#include "LedDisplay.h"
#include <TimerOne.h>

byte col[NDIGITS] = {
                          0b10000000,             //128    0x80
                          0b01000000,             // 64    0x40
                          0b00100000,             // 32    0x20
                          0b00010000,            // 16     0x10
                          0b00001000,            // 8      0x08
                          0b00000100,            // 4      0x04
                          0b00000010,            // 2      0x02
                          0b00000001             // 1      0x01
                    } ;

      byte seg[] = {
                          0xC0, 0xF9, 0xA4, 0xB0,
                          0x99, 0x92, 0x82, 0xF8,
                          0X80, 0X90, 136,  131,
                          167,  161,  134,  142,
                          144,  139,  207,  241,
                          182,  199,  182,  171,
                          163,  140,  152,  175,
                          146,  135,  227,  182,
                          182,  182,  145,  182
                  };

/* Segment byte maps for numbers 0 to 9 */
const byte SEGMENT_MAP_DIGIT[] = {
                                    0xC0, 0xF9,
                                    0xA4, 0xB0,
                                    0x99, 0x92,
                                    0x82, 0xF8,
                                    0X80, 0X90
                                 };
/* Segment byte maps for alpha a-z */
const byte SEGMENT_MAP_ALPHA[] = {
                                    136,  131,  167,  161,
                                    134,  142,  144,  139,
                                    207, 241,   182,  199,
                                    182, 171,   163,  140,
                                    152, 175,   146,  135,
                                    227, 182,   182,  182,
                                    145, 182
                                 };

byte segbuf[NDIGITS];
int segcnt = 0;

volatile void LED_irq(void)
{
  digitalWrite(LED_LATCHPIN , LOW);
  // select the digit...
  shiftOut(
            LED_DATAPIN,
            LED_CLOCKPIN,
            MSBFIRST,
            col[segcnt]
          );
  // and select the segments
  shiftOut(
            LED_DATAPIN,
            LED_CLOCKPIN,
            MSBFIRST,
            segbuf[segcnt]
          );

  digitalWrite(LED_LATCHPIN, HIGH);
  segcnt ++;
  segcnt &= NDIGITS_MASK;
}

LedDisplay::LedDisplay()
{
  pinMode(LED_LATCHPIN, OUTPUT);
  pinMode(LED_CLOCKPIN, OUTPUT);
  pinMode(LED_DATAPIN,  OUTPUT);

  Timer1.initialize(500);      // 500 ms
  Timer1.attachInterrupt(LED_irq);
  Timer1.start();
}


void LedDisplay::displayString(char *inString)
{
   int8_t len = strlen(inString);
  if (len > 8)
    return;

  char str[9];
  switch (len)
  {
        case 8:
                strcpy(str, inString);
                break;
        case 7:
                strcpy(str, " ");
                strcat(str, inString);
                segbuf[0] = 0xc0;
                break;
        case 6:
                strcpy(str, "  ");
                strcat(str, inString);
                segbuf[0] = 0xc0;
                segbuf[1] = 0xc0;
                break;
        case 5:
                strcpy(str, "   ");
                strcat(str, inString);
                segbuf[0] = 0xc0;
                segbuf[1] = 0xc0;
                segbuf[2] = 0xc0;
                break;

        case 4:
                strcpy(str, "    ");
                strcat(str, inString);
                segbuf[0] = 0xc0;
                segbuf[1] = 0xc0;
                segbuf[2] = 0xc0;
                segbuf[3] = 0xc0;
                break;
        case 3:
                strcpy(str, "     ");
                strcat(str, inString);
                segbuf[0] = 0xc0;
                segbuf[1] = 0xc0;
                segbuf[2] = 0xc0;
                segbuf[3] = 0xc0;
                segbuf[4] = 0xc0;
                break;
        case 2:
                strcpy(str, "      ");
                strcat(str, inString);
                segbuf[0] = 0xc0;
                segbuf[1] = 0xc0;
                segbuf[2] = 0xc0;
                segbuf[3] = 0xc0;
                segbuf[4] = 0xc0;
                segbuf[5] = 0xc0;
                break;
        case 1:
                strcpy(str, "       ");
                strcat(str, inString);
                break;
  }


  len = strlen(str);
  for ( register int8_t i = 0; i < len; i++ )
    segbuf[7 - i] = AsciiToSegmentValue( byte( str[i] ) );
 }


void LedDisplay::displayFreq(long  freq)
{
  char Freq[50] =  {0};
  ltoa(freq, Freq, 10);
  int8_t len = strlen(Freq);
  char str[50];

  switch (len)
  {
        case 8:
                break;
        case 7:
                strcpy(str, " ");
                strcat(str, Freq);
                strcpy(Freq, str);
                segbuf[0] = 0xc0;
                break;
        case 6:
                strcpy(str, "  ");
                strcat(str, Freq);
                strcpy(Freq, str);
                segbuf[0] = 0xc0;
                segbuf[1] = 0xc0;
                break;
        case 5:
                strcpy(str, "   ");
                strcat(str, Freq);
                strcpy(Freq, str);
                segbuf[0] = 0xc0;
                segbuf[1] = 0xc0;
                segbuf[2] = 0xc0;
                break;

        case 4:
                strcpy(str, "    ");
                strcat(str, Freq);
                strcpy(Freq, str);
                segbuf[0] = 0xc0;
                segbuf[1] = 0xc0;
                segbuf[2] = 0xc0;
                segbuf[3] = 0xc0;
                break;
        case 3:
                strcpy(str, "     ");
                strcat(str, Freq);
                strcpy(Freq, str);
                segbuf[0] = 0xc0;
                segbuf[1] = 0xc0;
                segbuf[2] = 0xc0;
                segbuf[3] = 0xc0;
                segbuf[4] = 0xc0;
                break;
        case 2:
                strcpy(str, "      ");
                strcat(str, Freq);
                strcpy(Freq, str);
                segbuf[0] = 0xc0;
                segbuf[1] = 0xc0;
                segbuf[2] = 0xc0;
                segbuf[3] = 0xc0;
                segbuf[4] = 0xc0;
                segbuf[5] = 0xc0;
                break;
        case 1:
                strcpy(str, "       ");
                strcat(str, Freq);
                strcpy(Freq, str);
                break;
  }
  len = strlen(Freq);
  for ( register int8_t i = 0; i < len; i++)
  {
      segbuf[7 - i] = AsciiToSegmentValue(byte(Freq[i]));
      switch (i)
      {
        case 1:
                if (Freq[i - 1] ==  ' ' && len < 6)
                break;
        case 4:
                if (Freq[i] ==  ' ' && len > 4)
                break;

         segbuf[7 - i] = AsciiToSegmentValue(byte(Freq[i])) & 127;
      }
  }
}


byte LedDisplay::AsciiToSegmentValue (byte ascii)
{
    byte segmentValue = 182;

  if(ascii >= '0' && ascii <= '9')
      segmentValue = SEGMENT_MAP_DIGIT[ascii - '0'];

  else if (ascii >= 'a' && ascii <= 'z')
      segmentValue = SEGMENT_MAP_ALPHA[ascii - 'a'];

  else if (ascii >= 'A' && ascii <= 'Z')
      segmentValue = SEGMENT_MAP_ALPHA[ascii - 'A'];
  else
  {
    switch (ascii)
    {
      case '-':
                  segmentValue = 191;
                  break;
      case '.':
                  segmentValue = 127;
                  break;
      case '_':
                  segmentValue = 247;
                  break;
      case ' ':
                  segmentValue = 255;
                  break;
    }
  }
  return segmentValue;
}
