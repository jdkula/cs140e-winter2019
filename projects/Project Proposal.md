XBee Project
=============
CS140E - Winter Quarter
Jonathan Kula

Overview
---------
I'll be configuring two Raspberry Pis to communicate wirelessly, using two XBees for the physical layer and ZigBee as the transport protocol (as this is protocol is built into the XBees I'm using).

I am going to attempt to have the XBees communicate at close to their theoretical maximum, 35Kbps (XBee pg. 11).

I will do this by enabing caching and using UART interrupts to respond quickly to new information.

This also means I will need to add guards against livelock.


Details
--------
I'm going to start out with my already-extant libpi and three blank projects: `libxbee`, `libzigbee`, and a project that uses the aforementioned three libraries. 

`libxbee` will contain everything needed to setup and configure the physical XBee chip. The XBee chip is configurable using AT commands over the UART. I'll provide an abstraction for this (probably in the form of several structs), as well as functions that will update configurations using those structs. I'll be looking at the command reference in the XBee documentation to establish what is configurable and how to configure it. 

`libzigbee` will contain functions needed to query, transmit, and recieve from the Zigbee network. Many of its functions will depend on `libxbee`, since most of the Zigbee network configuration is stored/configured on the XBee itself. Thus, `libzigbee` will be more focused on operations that require sending and recieving information from other devices. For example, getting information from the network coordinator or router about the state of the rest of the network. It will also provide support for API-style network packets.

libzigbee will by default work asynchronously, using UART interrupts to store incoming data. 

Finally, the project will likely be a simple testing harness, which will attempt to send and receive data multiple times and check for accuracy, as well as speed. I'll make it increase the transfer speed over time, and see when it breaks (and then try to make it break less). My goal is to get as close to the maximum possible, which is 35 Kbps.


I think this project is interesting mostly because it involves mesh/IoT-style networking, with no external dependencies on the internet, routers, etc. Furthermore, XBee Pros can communicate at distances upwards of a mile, which makes this very appealing for point-to-point or in-the-field/robotics applications.

Hardware
---------

2x Raspberry Pi A+
2x XBee S2
2x XBee Adapter (2mm pins -> 0.1in pins)
4x XBee 2mm pin socket (so I don't solder the XBees directly into the board)
1x FTDI Serial Cable (for debugging and firmware updates)


Supporting Documents
---------------------

Digi's XBee + ZigBee documentation - https://www.digi.com/resources/documentation/digidocs/PDFs/90000976.pdf
- Page 11: Theoretical max speed and other overview info
- (will fill in pages as I get around to it)

Older Digi documentation - http://www.farnell.com/datasheets/27606.pdf
- (will fill in pages as I get around to it)

ARM documentation for caching (+ lab 12!) + Interrupts - XYZ
- (will fill in pages as I get around to it)

ARM Peripherals documentation for UART interrupts - ABC
- (will fill in pages as I get around to it)
