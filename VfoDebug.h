/*
 * VfoDebug.h
 * Enable/Disable Debug in across all classes
 *
 * (c) KG5NII April 2017
 *
 * GNU Public License
 */

#ifndef VFODEBUG_H
#define VFODEBUG_H

#define GLOBAL_NO_DEBUG             /*
                                     * If defined, then everything is turned
                                     * off including Serial.begin()
                                    */

#ifndef GLOBAL_NO_DEBUG
   /* Uncomment to enable */
   //#define   JOYSTIK_DEBUG        /* VFO::readJoyStik() debug */
    #define   SETUP_DEBUG           /* setup() debug in main sketch */
    #define   READMEM_DEBUG         /* FRAM readMEM() Debug in main sketch */
    #define   WRITEMEM_DEBUG        /* FRAM writeMEM() Debug in main sketch */
    #define   INC_BUTTON_DEBUG      /* Debug  in VFO::readJoyStikButton() */
#endif

#endif
