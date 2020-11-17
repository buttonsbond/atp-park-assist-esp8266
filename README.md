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

TO DO
=====

1. Low battery warning
2. Try and further combat spurious readings from the sensors (tried a few libraries) - if anyone can offer a solution that'd be appreciated.
3. When red, instead of an advancing arrow maybe just flashing red square.

I recommend that once you configure for your car that you hard code those distances in settings.h

Libraries needed/used
=====================

Thanks to the respective authors of these libraries, and apologies if I missed any

https://github.com/bbkbarbar/Arduino-HC-SR04-library
https://github.com/highno/rtcvars


Caveat
I recommend you use the same pins as I did. I had an issue with one of the (now) unused pins when the LCD panel was also in use in that one of the sensors stopped working. Not sure why but changing it got over that.

Say hi
If you like my project, please send me a photo and say hi. If you have any improvements, be my guest and remember to share back!
