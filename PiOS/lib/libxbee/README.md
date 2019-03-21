>(back to [project readme](../../PROJECT.md))

LibXBee
========
LibXBee contains code necessary to query and configure a local
XBee module. It currently has support for the XBee 2 (Pro) and other
modules that are similarly configurable.

Implementation Notes
---------------------
Configuring the XBees isn't too bad. They operate using AT commands
(like computer modems). If DIN (UART RX) is low when it powers on
(which it will be since the Pi configures/pulls up UART at runtime),
it communicates at 9600 baud regardless of the saved setting (p. 175),
so I just used 9600 baud throughout to simplify things. 

Reference Documentation
------------------------
XBee Docs: https://www.digi.com/resources/documentation/digidocs/PDFs/90000976.pdf