/*
    ARDUINO TRIGGER BOUNCE by CRUNCHYSTEVE, shinyhappyrainbows.com
    ©2023 Crunchysteve filthynoisesmusic@gmail.com
    Takes a falling or rising edge trigger input & creates a fixed width pulse 
    that can't be retriggered until after the output pulse the next falling edge.
    The pulsewidth is set by the "period" variable, input is on digital pin 2 
    & output is on digital pin 13. It needs no external libraries, all timing is 
    completely non-blocking & all code is pure C. See lICENCE.md for usage terms.
*/

#include "Arduino.h"

//  Trigger input and output pins
#define INPUT_PIN         2               //  Set trigger input pin to D2
#define OUTPUT_PIN       13               //  Set LED output pin    to D13
//  If falling edge(s) permanently needed, short either or both pins to ground.
#define IN_SWITCH         4               //  Set Input polarity switch to D4 - No switch = rising only.
#define OUT_SWITCH        7               //  Set Input polarity switch to D7 - No switch = rising only.
//  Optional potentiometer for setting pulsewidth value, "period".
#define POT_PIN          A0               //  Set analog pin used for pulsewidth pot

bool      inEdge       = true;            //  "true" = rising edge, "false" = falling edge
bool      outEdge      = true;            //  "true" = rising edge, "false" = falling edge
bool      state        = LOW;             //  input trigger state rest state is opposite
bool      test         = false;           //  Variable to store last input state

uint32_t  rightNow     =   0;             //  Set pulse width timer comparison variable
uint32_t  backThen     =   0;             //  Set pulse width timer start variable
uint32_t  period       =  26;             //  Length of output pulse width in milliseconds
                                          //  (Can be longer: is set to timing of 16s at 285BPM)
void setup(){
  //  set fast analogRead (1uS) as per https://www.gammon.com.au/adc
  ADCSRA &= ~(bit (ADPS0) | bit (ADPS1) | bit (ADPS2));   //  clear analogRead prescaler defaults
  ADCSRA |= bit (ADPS2);                  //  set at 6.5uS analogRead() time, instead of 104uS,
                                          //  to significantly reduce latency of reading A0.
  pinMode(IN_SWITCH,INPUT_PULLUP);        //  Input polarity switch set as INPUT_PULLUP)
  pinMode(INPUT_PIN, INPUT_PULLUP);       //  set trigger input MODE as INPUT_PULLUP
  pinMode(OUT_SWITCH,INPUT_PULLUP);       //  Output polarity switch set as INPUT_PULLUP)
  pinMode(OUTPUT_PIN, OUTPUT);            //  output setup as OUTPUT
    //  IN_SWITCH and OUT_SWITCH set the input and/or output active state polarities,
    //  as rising edge (default) or falling edge. Can be individually set with 
    //  2x SPST switches or both can be set the same, using 1x DPST switch.
}

void loop(){
  //  Set edge detection preference from switch inputs.
  if(IN_SWITCH){inEdge = false;} else {inEdge = true;}    //  Set input edge.
  if(OUT_SWITCH){outEdge = false;} else {outEdge = true;} //  Set output edge.

  //  Detect working input edge settings and detect a triggering edge.
  bool input;                             
  if(inEdge){                            //  Get edge state of INPUT_PIN...
    input = digitalRead(INPUT_PIN);       //  (default ->) for rising edge or
  } else {
    input = !digitalRead(INPUT_PIN);      //  for falling edge.
  }
  if(input != test){                      //  Detect rising edges...
    if(input == HIGH &&                   //  Detect only if rising edge AND
       state == LOW){                     //  LED state is not triggered, then
      state = HIGH;                       //  trigger LED state and
      backThen = millis();                //  initialise pulse timer start time.
    }
    test = input;                         //  Store this input edge state.
  }
  //  Timeout last triggered edge.
  rightNow = millis();                    //  Measure time "now."
  if(              state == HIGH &&       //  If LED is HIGH and
     rightNow - backThen >= period){      //  if pulse timer has elapsed by period then
    state = LOW;                          //  set LED to off.
  }

  //  Detect active rising/falling pulse state of output and set required state
  if(outEdge){
    digitalWrite(OUTPUT_PIN,state);       //  (default ->) Write LED state to OUTPUT_PIN for "period" mS
  } else {
    digitalWrite(OUTPUT_PIN,!state);      //  Write LED invert-state to OUTPUT_PIN for "period" mS.
  }

  //  If potentiometer state has changed, set new period.
  uint32_t potRead = map(analogRead(POT_PIN),0,1023,20,800);   //  Get potentiometer value, map to useful
  if(period != potRead) period = potRead;                 //  range and, if different, update period.
}