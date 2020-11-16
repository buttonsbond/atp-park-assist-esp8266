# atp-park-assist-esp8266
Parking assist using 2 ultrasonic sensors, 8x8 RGB pixel and d1 mini clone

Just had an automatic rolling door fitted to the drive. Parking is a bit tight due to the car port we have, the middle posts get in the way of the drivers/passenger door opening. It is possible, if you park just right to get in and out of at least one of those doors. A parking sensor to help would be just the job, one to judge if the car is far enough forward, and another at the side to judge if over to the right enough. This project utilises 2 ultrasonic 5v sensors (HC-SR04), a D1 Mini clone (4Mb version), and an 8x8 RGB pixel panel.

most of default settings can be changed in settings.h

TO DO

at the moment the unit is on all the time - need to implement a sleep routine to periodically check for car presence as well as if distance is further than
the garage door which should indicate car is on its way in.

save settings to EEPROM so that they persist


anyhow, to configure either sensor distance, hold your hand over the sensor until panel flashes blue, then remove your hand, it will then measure the distance and
set that as the final distance.

the rgb panel has a green, yellow, red advancing arrow to indicate forward movement (red is stop!)

the sides of the rgb panel, have green, yellow, blue to indicate side distance (blue is stop)

You'll need this library: https://github.com/bbkbarbar/Arduino-HC-SR04-library
