# atp-park-assist-esp8266
Parking assist using 2 ultrasonic sensors, 8x8 RGB pixel and d1 mini clone. Optionally you can connect a 4 panel LCD screen to help with debugging/calibrating when the equipment is in situ.
===========================
Just had an automatic rolling door fitted to the drive. Parking is a bit tight due to the car port we have, the middle posts get in the way of the drivers/passenger door opening. It is possible, if you park just right to get in and out of at least one of those doors. A parking sensor to help would be just the job, one to judge if the car is far enough forward, and another at the side to judge if over to the right enough. This project utilises 2 ultrasonic 5v sensors (HC-SR04), a D1 Mini clone (4Mb version), and an 8x8 RGB pixel panel.

most of default settings can be changed in settings.h

Features
========

1. Calibrate either sensor for its minimum distance by placing your fingers over each sensor. A reading of over 1000 will result triggering the calibrate routine.
2. Advancing arrow shown in green, yellow then red (on the 8x8 RGB) for the front sensor. When red, it's time to stop!
3. Left and right hand side of the RGB shown in green, yellow then blue for the side sensor. Blue means your car is far enough over.
4. When no change in forward movement is detected - ie. reading doesn't change much, the micro-controller will enter deepsleep (whether this helps save the battery only time will tell)
5. Various variables saved to RTC when the unit sleeps, and reloaded when it wakes up - a quick front reading is taken and if there still isn't any change the controller enters deepsleep again.
6. OTA updating is possible - it may fail if you happen to try and upload during a sleep operation.
7. An LCD display can be connected to monitor the units operation (recommended only for debugging/setup) - comment out in the code to disable support.
8. Voltage can be monitored if enabled - not fully implemented to do anything with yet but ideally for a low battery warning.
9. MQQT implemented to send front and side readings, f&s limits (after a calibration), time (periodically), whether parked or not (1 or 0), voltage level (see notes further down). 

3D Models
=========
The folder final-models contains some STL files of the enclosures I created and used - the originals are in Tinkercad. 
* remixed-from-tinkercad-user-weselle - in this folder 2 x STL files for the ultrasonic sensor enclosure. Bear in mind that I bent the pins almost fully back on themselves rather than sticking out of the top/bottom - if you still want the pins sticking out the bottom/top then checkout weselle on Tinkercad.
* boxslot.stl - the main box with a thin lip in the bottom to stop the LED grid falling out (you can optionally print insert.stl in white to create a diffuser to go in first ahead of the LED grid.
* mountsall.stl - this is an square 1mm insert that goes in on top of the LED grid, it has posts for mounting D1 mini (my version has holes for the posts to go through), posts for mounting standard DC buck converter, and a holder for 12V 23A battery, the battery found commonly in cheap doorbell push button units.
-- note, that original idea of using a buck converter with a 12V battery wasn't sound - novice that I am, didn't know that the converter would run the battery down even when nothing connected, so plan b is to add a battery shield to the D1 mini and a LIPO battery of 1200mah. I am hoping that with the deep sleep used in the programming that the battery will last a while without recharging. I will expose the usb charging port to allow recharging from something like a power bank.
* plug.stl - this is the mount I made to slot into the metal aluminium tubing I mount the main unit on, it slides into place (quite tightly)
* insert.stl - print this in white PLA for a diffuser (optional) - FILE NOT YET ADDED
* lid.stl - lid for the main box - FILE NOT YET ADDED

You'll need 2 each of the ultrasonic sensor models.

TO DO
=====

1. Low battery warning (see note under wiring up!)
2. Try and further combat spurious readings from the sensors (tried a few libraries) - if anyone can offer a solution that'd be appreciated. I've built in various checks to try and combat this. I read somewhere that the Newping library might be an option which I might try once I've got the power issues sorted.
3. When red, instead of an advancing arrow maybe just flashing red square.

I recommend that once you configure for your car that you hard code those distances in settings.h

Wiring up
=========
I've included a couple of exported Powerpoint slides to show how I wired up. However, it's not quite right - I need some help on this one. Basically I have a small solar panel repurposed from a solar outside light from LIDL - it gives out up to about 5 volts when in direct sun. No idea what the charge current is. It's enough to light the red LED on the battery shield but I don't think it is enough for charging.
I added in the power board so that the battery shield output should give 5 volts for as long as it can (this of course makes my voltage measurement incorrect, and since I haven't put the battery shield in the same enclosure as the D1 mini unless I run a separate wire for the 3v output of the battery shield module to the d1 mini I won't be able to get a reading from the batteries - maybe I don't need the voltage regulator if I can get the solar charging working). The battery drains after about 8 or 9 hours (2000mah 3.7v lion battery) at the moment.
I'm not an electronics expert by any stretch of the imagination so if someone can let me know what the setup should be - comment away!

Libraries needed/used
=====================

Thanks to the respective authors of these libraries, and apologies if I missed any

* https://github.com/bbkbarbar/Arduino-HC-SR04-library
* https://github.com/highno/rtcvars


Caveat
I recommend you use the same pins as I did. I had an issue with one of the (now) unused pins when the LCD panel was also in use in that one of the sensors stopped working. Not sure why but changing it got over that.

Say hi
If you like my project, please send me a photo and say hi. If you have any improvements, be my guest and remember to share back!
