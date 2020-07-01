# cpp-instrumentation

Package-free timers and Instrumentation. This contains the code for timers, countdowns, moving averages and rate calculation in a context free of any dependencies. It should be trivial to drop this code into any C++ project on any platform.

This is meant for instrumenting interactive applications like games so we're mainly interested in the millisecond to seconds range, this has been optimized for those kinds of time ranges.

## Requires

C++ 11 standard

## Tools:

* Countdown - a stopwatch, lets you know when some number of seconds have elapsed.
* AvgTimer - A timer with start/stop method. Returns average time that the timer was on.
* Counter - Returns the rate that "tick" is called per second
* Moving Average - Returns a moving average of a float value that you "push" to it over a number of seconds. Uses EMA