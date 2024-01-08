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

Chrono pulsewidth;

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
bool      pulseState   = LOW;             //  input trigger state rest state is opposite
bool      testCon      = false;           //  Variable to store last input state

uint32_t  rightNow     =   0;             //  Set pulse width timer comparison variable
uint32_t  backThen     =   0;             //  Set pulse width timer start variable
uint32_t  period       =  26;             //  Length of output pulse width in milliseconds
                                          //  (Can be longer: is set to timing of 16s at 285BPM)
void setup(){
  //  set fast analogRead (1uS) as per https://www.gammon.com.au/adc
  ADCSRA &= ~(bit (ADPS0) | bit (ADPS1) | bit (ADPS2));   //  clear analogRead prescaler defaults
  ADCSRA |= bit (ADPS2);                  //  set at 6.5uS analogRead() time, instead of 104uS,
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
  if(input != testCon && !pulsewidth.isRunning()){  //  Detect any edge and act...
                                                    //  only if timer is stopped.
    if(input == HIGH && !pulseState){     //  Act only for a rising edge AND...
                                          //  if LED state is not triggered...
      pulseState = HIGH;                  //  then trigger LED state and...
      pulsewidth.start();                 //  start the pulse timer.
    }
    testCon = input;                      //  Store input as testCon for all 
                                          //  detected edges.
  }

  if(pulsewidth.hasPassed(period) && pulseState){   //  detect timer if state is HIGH
    pulseState = LOW;                     //  if above is true, turn off pulse and...
    pulsewidth.stop();                    //  stop timer
  }

  //  Convert internal logic to rising or falling edge, as per switches.
  if(outEdge){
    digitalWrite(OUTPUT_PIN,pulseState);       //  (default ->) Write LED state to OUTPUT_PIN for "period" mS
  } else {
    digitalWrite(OUTPUT_PIN,!pulseState);      //  Write LED invert-state to OUTPUT_PIN for "period" mS.
  }

  //  If potentiometer state has changed, set new period.
  //  Get potentiometer value, map to useful range and...
  uint32_t potRead = map(analogRead(POT_PIN),0,1023,20,800);  //  (output range = mS)
  //  if potentiometer map value different, update period value.
  if(period != potRead) period = potRead;
}