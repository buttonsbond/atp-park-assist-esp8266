//
// strategies to preserve battery life
//
// 1. wakeup less frequently between inactive hours when house occupants likely in bed
//      for example between say 01:00 and 06:00
// 2. if battery level falls below a certain level, increase wakeup delay
//      and reduce brightness of LEDs as appropriate
// 3. if battery level falls below a certain level, adjust LED brightness and use less
//      leds - ie. just one strip for side and say 4 central LEDs instead of advancing arrow
// 4. on wakeup check that only the front sensor is activated for reading (already implemented)
// 5. If several repeated checks keep confirming that the car is actually parked (front and side sensors <= to the limits)
//      then increase the sleep time to inactive sleep value
// 6. Also set the board clock to 80mhz instead of 160mhz (will make the advance arrow go slower)

void Power(void) {
  GetVoltage();
  if (voltage >= StandardSleepVolt) {
    // battery is good
    CurrentSleep = StandardSleep;
    ledbrightness=5;
  }
  if (voltage < StandardSleepVolt) {
    CurrentSleep = LowPowerSleep;
    ledbrightness=3;
  }
  // if it is 20 hrs, inactivestart is 1 and end is 6
  if ((CurrentHour >= InactiveStart) && (CurrentHour <= InactiveEnd)) {
    CurrentSleep = InactiveSleep;
    ledbrightness=1;
  }

  if (successivechecks > 30) {
    CurrentSleep = InactiveSleep;
    ledbrightness=1;
  }
}

void GetHour(void) {
  // We probably don't want to poll the network time each and every time, so let's only do it every so often
  // GetHour will now only be called when nochange is 0 (ie. sensors being read or have changed) 
  if (nochange == 0) {
  timeClient.update();
  CurrentHour = timeClient.getHours();
  CurrentMin = timeClient.getMinutes();
  sprintf(mytime,"%i:%i",CurrentHour,CurrentMin);
  }
}
