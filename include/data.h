//  Program Data Library

#ifndef DATA_H
#define DATA_H
// ***"HARDWIRED" PROGRAM MACRO VALUES*** //
//  Trigger input and output pins
#define INPUT_PIN         2               //  Set trigger input pin to D2
#define OUTPUT_PIN       13               //  Set pulse output pin to LED pin, D13 (visibility)
//  If falling edge(s) permanently needed, short either or both pins to ground.
#define IN_SWITCH         4               //  Set Input polarity switch to D4 - No switch = rising only.
#define OUT_SWITCH        7               //  Set Input polarity switch to D7 - No switch = rising only.
//  Optional potentiometer for setting pulsewidth value, "period".
#define POT_PIN          A0               //  Set analog pin used for pulsewidth pot
#define POT_MIN           0               //  Set potentiometer minimum read value
#define POT_MAX        1023               //  Set potentiometer maximum read value
#define MAP_MIN          20               //  Set potentiometer minimum read value
#define MAP_MAX        1043               //  Set potentiometer minimum read value
//  Other functional configuration values
#define DEBOUNCE_DLY      2               //  Debounce delay in edge test

//  PROGRAM VARIABLES
//  Booleans
bool      inEdge       = true;            //  "true" = rising edge, "false" = falling edge
bool      outEdge      = true;            //  "true" = rising edge, "false" = falling edge
                                          //  Set to "true" for internal logic. INPUT_PULLUP
                                          //  and switches set the input and output edges.

bool      pulseState   = LOW;             //  Variable sets output value at given trigger states
bool      testCon      = false;           //  Variable to store last input state

//  32 bit, unsigned timing integers (eg unsigned long values but use fewer characters to declare)
uint32_t  rightNow     =   0;             //  Set pulse width timer comparison variable
uint32_t  backThen     =   0;             //  Set pulse width timer start variable
uint32_t  period       =  20;             //  Length of output pulse width in milliseconds
                                          //  (Can be longer: is set to timing of 16s at 285BPM)

#endif