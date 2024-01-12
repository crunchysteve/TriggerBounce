## TriggerBounce Roadmap

TriggerBounce began as a quick fix to a crude prototype of a (currently) top secret guitar pedal I'm working on with my best mate (and once bandmate), Wayne. So, how it stands now is exactly that, a quick and dirty, input-synchonised, fixed-width pulse generator, or "pulse constrainer." It's non-retriggerable until the input signal presents another edge that matches the chosen edge condition, ```Rising``` or ```Falling```, as per these macros in the library.

As anything which quickly solves a problem can usually be generalised, it may solve problems for other people, too, hence this repo. To this end, I'd like to advance the feature set.

As I describe it as a "Eurorack trigger pulse width limiter", one of the first and most useful features will be to create a ```repeat(int count = 0, bool sync = true, uint32_t time = 20)``` function with the default condition for ```count``` being ```0```, enabling a continuous repeat, while ```sync``` in default being ```true``` would make it operate as it currently does. On ```sync``` being set to true and ```count``` being set greater than 0, repeats would happen on an edge trigger for as many output pulses as the value of ```count```, and not retrigger until the repeats were completed.

This would be useful as a Class, so that several such outputs could be individually generated. It may be more of a Class for the EdgieD library, or as well as.

Speaking of EdgieD, the first item in its roadmap is pulsewidth setting. Currently, it takes 6uS to detect an edge (+/- 2uS), so non-blocking pulsewidth extension is coming, and will be added to TriggerBounce natively, probably as a call from TriggerBounce to ```edge.detect(input, inEdge, true)``` then adding the method ```edge.timeout( uint32_t pulsewidth = timeInMicros)``` to TriggerBounce to stop the pulse, insted of using Chrono.

So, the current plan is to make TriggerBounce more useful to synthHeads, but I'm hoping it'll have broader hobbyist, or even professional, engineering applications. The multiple pulse plan could be useful for those who restore antique telephone exchanges and may need a "decadic" dialler tool. Who knows?! '<laughs />'

So, if you have any ideas, or improvements, feel free to open an issue, or fork and pull if you're confident enough to add ideas to the code yourself.

Cheers,
Crunchy.