XBee & Soft UART Project
=============
CS140E - Winter Quarter

Jonathan Kula

Overview
---------
My original idea was to make two Raspberry Pis communicate with
each other wirelessly using XBees. While implementing this, I
discovered that I still needed to communicate with my computer;
and so, the Software UART project was born. 

Furthermore, I wanted the Pis to be able to configure and query the
current configuration of the XBees, so I created a framework for this 
as well.

Details
--------
Libraries:
* [LibXBee](lib/libxbee)
* [LibSoftUART](lib/libsoftuart)

Modules:
* [XBee AT Mode Test](modules/xbee-at-mode) - demonstrates getting config info from the XBee
* [XBee Echo](modules/xbee-echo) - echoes whatever the XBee receives to a software UART. Used to test sending and receiving.
* [XBee Hello](modules/xbee-send-hello) - echoes "Hello XBee!\n" at 1s intervals. Used to test receiving and sending.
* [XBee Presentation (RX/Remote)](modules/xbee-test-rx) - Code used on the remote RPi during the presentation.
* [XBee Presentation (TX/Local)](modules/xbee-test-tx) - Code used on the local RPi during the presentation.
* [SoftUART Echo](modules/softuart-echo) - Simply echoes what is received on the software UART. Additional information is echoed on the hardware UART.
* [SoftUART Hello](modules/softuart-hello) - Prints "Hello World!\r\n" on 1s intervals to both the software and hardware UART.

LibXBee makes extensive use of the UART, and LibSoftUART of
GPIO and timing. Both of these facilities are provided by
[LibPi](lib/libpi), which is mostly lab material. The following
are files particularly important to the functioning of the above
libraries:
* Interrupts:
  * [interrupts.c](lib/libpi/src/interrupts.c)
  * [interrupts.S](lib/libpi/sysspec/arm/interrupts.S)
  * [interrupt-vectors.c](lib/libpi/src/interrupt-vectors.c) -
      most of the changes to libpi made for this project are seen here.
  * [interrupts.h](lib/libpi/include/interrupts.h)
* GPIO
  * [gpio.c](lib/libpi/src/gpio.c)
  * [gpio.h](lib/libpi/include/gpio.h)
* UART
  * [uart.c](lib/libpi/src/uart.c)
  * [uart.h](lib/libpi/include/uart.h)
  * [aux.c](lib/libpi/src/aux.c)
  * [aux.h](lib/libpi/include/aux.h)

Hardware
---------

* 2x Raspberry Pi A+
* 2x XBee S2
* 2x XBee Adapter (2mm pins -> 0.1in pins)
* 4x XBee 2mm pin socket (so I don't solder the XBees directly into the board)
* 1x FTDI Serial Cable (for debugging and firmware updates)


Supporting Documents
---------------------

Digi's XBee + ZigBee documentation - https://www.digi.com/resources/documentation/digidocs/PDFs/90000976.pdf
- Page 11: Theoretical max speed and other overview info
- (will fill in pages as I get around to it)

ARM documentation for caching (+ lab 12!) + Interrupts - XYZ
- (will fill in pages as I get around to it)

ARM Peripherals documentation for GPIO and timer interrupts - ABC
- (will fill in pages as I get around to it)