/*
    ARDUINO TRIGGER BOUNCE by CRUNCHYSTEVE, shinyhappyrainbows.com
    ©2023 Crunchysteve filthynoisesmusic@gmail.com
    Takes a falling or rising edge trigger input & creates a fixed width pulse 
    that can't be retriggered until after the output pulse the next falling edge.
    The pulsewidth is set by the "period" variable, input is on digital pin 2 
    & output is on digital pin 13. TriggerBounce uses the Chrono library, 
    by Sofian Audry and Thomas Ouellet Fredericks. Find it at...
      [https://github.com/SofaPirate/Chrono](https://github.com/SofaPirate/Chrono)
    All timing is completely non-blocking & all code is pure C. 
    See lICENCE.md for usage terms. (Project now ready for release. V1.0.0)
*/

#include <Arduino.h>
#include <Chrono.h>   //  Uses the amazing Chrono library by Sofian Audry and 
                      //  Thomas Ouellet Fredericks:- https://github.com/SofaPirate/Chrono/
#include <EdgieD.h>     //  Crunchysteve:- https://github.com/crunchysteve/EdgieD/
#include <data.h>     //  Program data, declarations, constants and variables

Chrono pulsewidth;
Edge edge;

void setup(){
  Serial.begin(115200);
  //  set fast analogRead (1uS) as per https://www.gammon.com.au/adc
  ADCSRA &= ~(bit (ADPS0) | bit (ADPS1) | bit (ADPS2));   //  clear analogRead prescaler defaults
  ADCSRA |= bit (ADPS2);                  //  set to 6.5uS analogRead() time, instead of 104uS,
                                          //  to significantly reduce latency of reading A0.
                                          //  (Also reduces accuracy, but not entirely needed for
                                          //  a potentiometer read to set the pulsewidth.)
  pinMode(IN_SWITCH,INPUT_PULLUP);        //  Input polarity switch set as INPUT_PULLUP)
  pinMode(INPUT_PIN, INPUT_PULLUP);       //  set trigger input MODE as INPUT_PULLUP
  pinMode(OUT_SWITCH,INPUT_PULLUP);       //  Output polarity switch set as INPUT_PULLUP)
  pinMode(OUTPUT_PIN, OUTPUT);            //  output setup as OUTPUT
    //  IN_SWITCH and OUT_SWITCH set the input and/or output active state polarities,
    //  as rising edge (default) or falling edge. Can be individually set with 
    //  2x SPST switches or both can be set the same, using 1x DPST switch.
  pulsewidth.start();
}

void loop(){
  //  Set edge detection preference from switch inputs.
  if(IN_SWITCH){inEdge = false;} else {inEdge = true;}    //  Set input edge.
  if(OUT_SWITCH){outEdge = false;} else {outEdge = true;} //  Set output edge.

  //  Read input, as per switches, and detect a triggering edge.
  bool input;                             
  if(inEdge){                             //  Get edge state of INPUT_PIN...
    input = digitalRead(INPUT_PIN);       //  (default ->) for rising edge or...
  } else {
    input = !digitalRead(INPUT_PIN);      //  invert logic for falling edge.
  }

  //  Internal logic only detects rising edges, hence above "if/else."
  if(edge.detect(input,inEdge) && !pulsewidth.isRunning()){
                                  //  ^ Detect any edge and act only if timer is stopped.
    pulseState = inEdge;                  //  then trigger LED state and...
    // triggered = true;                  //  timer is triggered
    pulsewidth.start();                   //  start the pulse timer.
    delay(DEBOUNCE_DLY);                  //  default debounce delay
  }

  if(pulsewidth.hasPassed(period) && pulsewidth.isRunning()){   
                                          //  detect timer if triggered is HIGH
    pulseState = !inEdge;                     //  if above is true, turn off pulse and...
    // triggered = false;                    //  timer is no longer triggered, so...
    pulsewidth.stop();                    //  stop the timer
  }

  //  Convert internal logic to rising or falling edge, as per switches.
  if(outEdge){
    digitalWrite(OUTPUT_PIN,pulseState);       //  (default ->) Write LED state to OUTPUT_PIN for "period" mS
  } else {
    digitalWrite(OUTPUT_PIN,!pulseState);      //  Write LED invert-state to OUTPUT_PIN for "period" mS.
  }

  //  Get potentiometer value and map output range from 20mS to 1043 mS
  uint32_t potRead = map(analogRead(POT_PIN),POT_MIN,POT_MAX,MAP_MIN,MAP_MAX);
                                               //  (^)
  //  if potentiometer map value different, update period value.
  if(period != potRead) period = potRead;
}