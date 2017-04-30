/*
 * VfoConfig.h
 * Button,joystik hw pin definitions
 * This is the file where all hardware pins are defined
 *
 * (c) KG5NII November 2016, April 2017
 *
 * GNU Public License
 */

#ifndef VFOCONFIG_H
#define VFOCONFIG_H

/* Your callsign  is configured here */
#define USER_CALLSIGN   "KG5NII"


/* If using the Mega2560 controller uncomment else comment it out for UNO */
#define MEGA2560


/*  Min elapsed base time in ms before writing last frequency  to FRAM */
#define WRITE_FRAM_TIME   4000


/* define if using I2C interface with LCD otherwise comment out */
#define LCD_I2C

/* Size of the LCD in rows X columns*/
#define LCD_ROWS    4
#define LCD_COLS    20


/* +++++++++++++++++++++ Begin hardware pin & I2C addr assignments +++++++++++++++++++++ */

//LCD type and pinouts
#ifdef LCD_I2C
  #define LCD_I2C_ADDR        0x27        /* I2C address of the LCD */
#else
/*
 * If you are not using I2C for your LCD ,
 * then define how its wired to your Arduino here
 */
  #define RS   7
  #define EN   8
  #define D4   9
  #define D5  10
  #define D6  11
  #define D7  12
#endif

/*
 * Quadrature encoder pin assignments.
 * Note the use of two IRQ pins for best performance!
 */
#ifdef MEGA2560
  #define ENCODER_A_PIN         18
  #define ENCODER_B_PIN         19
#else
  #define ENCODER_A_PIN          2
  #define ENCODER_B_PIN          3
#endif


#define FRAM_CS_PIN             53
#define INC_PUSH_BUTTON          8
#define JOYSTIK_X               A1
#define JOYSTIK_Y               A2

#define SMETER_INPUT_PIN        A5
#define FILTER_PUSH_BUTTON      A6
#define FILTER_ENABLE_PIN       34
#define FREQ_UP_PIN             30
#define FREQ_DN_PIN             31

#define DS3231_I2C_ADDRESS    0x68


/* The RED 7 segment LED Display */
#define LED_LATCHPIN             9      // connect to RCK
#define LED_CLOCKPIN            11      // connect to SCK
#define LED_DATAPIN             10      // connect to DIO


/* The AD9850 DDS module */
#define DDS_RESET                7
#define DDS_DATA                 6
#define DDS_FQ_UD                5
#define DDS_W_CLK                4


/*
 * XMIT+ 12V  to tell the controller when the transceiver is in transmit mode
 * NOTE:  DO NOT CONNECT this lead directly to the micro controller!!!!
 * Use a voltage divider as such:
 *
 *                        XMIT_PLUS_PIN
 *                              o
 *                              |
 *                              |
 *      XMIT+ 12V<------[6K2]---+-----[4K2]------\
 *                              |                | GND
 *                              |-----[47uF]-----/
 *
 *  This is part of the Smeter class.
 */

#define XMIT_PLUS_PIN           A0



 /*
  *  Front panel has a RGB LED
  *  Each color has a 10K resistor to keep it dim
  *  RED = XMITstatusLED = Transceiver is currently Transmitting
  *  GREEN = RESERVEDstatusLED  = Reserved for future use
  *  BLUE = RECVstatusLED = Transceiver is currently in receive mode.
  */

#define XMITstatusLED           33
#define RESERVEDstatusLED       35
#define RECVstatusLED           37



/*
 *
 * Define the direct frequency keypad pins.
 * The  keypad consists of 4 rows and 3 columns.
 *
 */

#define KEYPAD_ROW0               21
#define KEYPAD_ROW1               23
#define KEYPAD_ROW2               25
#define KEYPAD_ROW3               27

#define KEYPAD_COL0               20
#define KEYPAD_COL1               22
#define KEYPAD_COL2               24

#define KEYPAD_ROWS                4
#define KEYPAD_COLS                3





/* +++++++++++++++++++++ End of hardware pin & I2C addr assignments +++++++++++++++++++++ */


/* uncomment to enable JoyStik support */
//#define JOYSTIK_DEFINED

/* The default frequency step rate whenenever the system is stated with factory defaults */
#define DEFAULT_FREQ_STEP      "50 Hz "
/* frequency change direction*/
#define UP_FREQ     1
#define DN_FREQ    -1


/* keyswitch definitions. */
#define btnRIGHT    0
#define btnUP       1
#define btnDOWN     2
#define btnLEFT     3
#define btnSELECT   4
#define btnNONE     5

/* Definitions for joystik positions */
#define joyN        0
#define joyU        1
#define joyD        2
#define joyL        3
#define joyR        4
#define joyDiagUL   5
#define joyDiagUR   6
#define joyDiagLL   7
#define joyDiagLR   8

/* Vfo.readJoyStik() output definitions */
#define UP          1
#define DOWN        2
#define LFT         3
#define RGT         4
#define UPPERLEFT   5
#define UPPERRIGHT  6
#define LOWERLEFT   7
#define LOWERRIGHT  8


/*
 * Comment out the define below if you want the transceiver to recover
 * if the FRAM fails to initialize at startup.  Otherwise the transceiver
 * will not come up, so uncommenting this directive causes the FRAM initialization
 * failure to do a while(1) loop after the failure, forcing the user to take action.
*/
//#define LOOP_ON_FRAM_FAIL

#define OFF LOW
#define ON  HIGH

#define  CW_FILTER_300HZ     1
#define  CW_FILTER_WIDE      0


#define PULLUP true
#define INVERT true
#define DEBOUNCE_MS    15
#define REPEAT_FIRST  500
#define REPEAT_INCR   100






const byte rowPins[KEYPAD_ROWS] = { 23,25,27,29 };

const byte colPins[KEYPAD_COLS] = { 26,24,22 };

const byte keys[KEYPAD_ROWS][KEYPAD_COLS] = {
                                      {'1','2','3'},
                                      {'4','5','6'},
                                      {'7','8','9'},
                                      {'*','0','#'}
                                  };

const byte block[8] = {
                                    B11111,
                                    B11111,
                                    B11111,
                                    B11111,
                                    B11111,
                                    B11111,
                                    B11111,
                                    B11111
                      };



/* For those too lazy to type Serial.print[ln] and friends */
#ifndef SPP
  #define SPP  Serial.print
#endif

#ifndef SPPL
  #define SPPL Serial.println
#endif

#endif
