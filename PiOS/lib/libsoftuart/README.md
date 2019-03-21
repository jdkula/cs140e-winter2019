>(back to [project readme](../../PROJECT.md))

LibSoftUART
============
LibSoftUART contains an implemented bit-bang software UART,
complete with TX _and_ RX. LibSoftUART depends heavily on
GPIO and interrupt capabilities provided by [libpi](../libpi).

Implementation Notes
---------------------
This software UART makes use of the built-in ARM timer provided by
libpi's [timer.c](../libpi/src/timer.c), which gives us decent
millisecond-to-microsecond timings, which works okay for slower
UART speeds but not for faster ones. Faster GPIO code with more
accurate timing (for example, using the ARM's instruction counter...)
will allow for faster UART capability.

I also noticed that when I implemented pushing data out in the timer
interrupt, it appeared to process things more slowly. I'm not 100% sure
why this is the case, but it bears noting. Receiving does not seem
affected.

By far the most difficult part of implementing this was going back
and making GPIO interrupts work properly (I was getting some stack
weirdness on return, which was *very* difficult for me to debug...).

Reference Documentation
------------------------
* [The ARM Manual](../../../docs/armv6.pdf)
* [The ARM Peripherals Manual](../../../docs/BCM2835-ARM-Peripherals.pdf)
* [Wikipedia's UART page](https://en.wikipedia.org/wiki/Universal_asynchronous_receiver-transmitter)
* [This Pi forum thread about GPIO interrupts](https://www.raspberrypi.org/forums/viewtopic.php?t=67457)