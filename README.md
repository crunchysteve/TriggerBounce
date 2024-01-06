## ARDUINO TRIGGER BOUNCE
©2023 Crunchysteve<br />
filthynoisesmusic@gmail.com
    
A Eurorack trigger pulse width limiter, based on an Arduino nano. (Will work with most Arduino and related MCUs, as it's pure C) It takes a falling-edge trigger input and repeats (bounces) a finite width, falling edge pulse to the output, regardless of the width of the input pulse. Now adapted to PlatformIO, such that you will need to change ```src/triggerBounce.cpp``` to
```triggerBounce/triggerBounce.ino``` after downloading/cloning and you may need to remove the
```#include "Arduino.h"``` line from the beginning of the file.

TriggerBounce is a Eurorack trigger pulse width limiter, based on an Arduino nano. (Will work with most Arduino and related MCUs, as it's pure C) It takes a falling-edge trigger input and repeats (bounces) a finite width, falling edge pulse to the output, regardless of the width of the input pulse. It cannot be retriggered until the input trigger is released and retriggered.

The triggerBounce.cpp file is a generalised, non retriggerable pulsewidth constrainer, called ```triggerBounce.ino```, that allows for optional polarity switches to set a rising or falling edge input and separate rising or falling edge output, as well as using analog input A0 to read a pot and map period to between 20mS and 800mS. You can change the mapping vales to longer or shorter amounts, if needed.

All three versions run on a basic circuit like below, but the generalised one will default to rising edge input and output, as well as 26mS, if no switches and no pot are added to the appropriate pins.

<img width="912" alt="triggerBounce" src="./img/triggerBounce.png">
Above is the circuit diagram for triggerBounce.ino projects. Add switches to pins D4 (input polarity) and D7 (output polarity), wired to be open to select rising edge and closed (grounded) to select falling edge, then add a 50kΩ potentiometer with low to ground, high to 5V and wiper to A0 to set the pulsewidth period. Or, just build it without, and preset your preferred values in the declarations and variable initialisations. As you can see from the waveforms, when the yellow trace (input) falls (falling edge), this triggers a shorter descending pulse (green) on the output. This pulse doesn't retrigger until another falling edge on the input. The risingTriggerBounce.ino project would show positive going pulses on the traces.

TriggerBounce uses the Chrono library, by Thomas Ouellet Fredericks. Find it at [https://github.com/SofaPirate/Chrono](https://github.com/SofaPirate/Chrono), load it into ArduinoIDE with the Library Manager or load it in VSCode/PlatformIO via "PlatformIO Home/Libraries".
<hr />

*Further explanation and operation details soon. The code is fully (possibly overly) commented.*
