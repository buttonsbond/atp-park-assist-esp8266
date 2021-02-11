void GetDistances(void) {
  TestFront();
  topright(); // output for LCD if enabled
  bottomright(); // output for LCD if enabled
  if (nochange==0) {
  TestSide(); // only test side when the whole shebang is up and running - otherwise if its just a wakeup to check, just test the front sensor
  }
}

void TestFront(void) {
  boolean done = false;
  Front();
  FastLED.setBrightness(ledbrightness);
  FastLED.delay(1000/FRAMES_PER_SECOND);

// only do the LEDs if change in reading
if (nochange==0) {
 
 if (distanceFront <= SetDistanceFront) {
    advance('R'); // Side 'STOP'
    debugmessages("FT: STOP  ");
    done = true;
  }
  if ((distanceFront > MidFront) && (!done)) {
    advance('G'); // Front is miles away
    debugmessages("FT: FAR   ");
    done = true;
  }

  if ((distanceFront <= MidFront) && (!done)) {
    advance('Y'); // front is somewhere between half way there and optimal
    debugmessages("FT: NEAR  ");
    done = true;
  }

}

  if ((distanceFront > CalibrateDistance)) {
      // we will check 1 more time after a delay to make sure this really is a calibration request
      debounce('F');
        if (distanceFront > CalibrateDistance) {
            debugmessages("FRONT CALIBRATION");
        calibrateFront('F');
        Front();
        // the value just read needs to be set to the minimum now
        SetDistanceFront=distanceFront;
        if (SetDistanceFront > SetDistanceFrontMax) { SetDistanceFrontMax = SetDistanceFront * 2; } // hopefully stops any minus values
        MidFront = SetDistanceFront + ((SetDistanceFrontMax - SetDistanceFront) / 2); 
        #ifdef SERIAL_DEBUG       
          Serial.print("Front values set to min distance front ");
          Serial.print(SetDistanceFront);
          Serial.print(" and half way to ");
          Serial.println(MidFront);
        #endif
           debugmessages("                  ");
        #ifdef LCD_DEBUG
           lcdmessage(2,"Front min " + String(SetDistanceFront));
           lcdmessage(3,"Front hwy " + String(MidFront));
        #endif       
        lightsoff();
        delay(3000);
        }
  }

 
}

void TestSide(void) {
  boolean done = false;
  Side();
  FastLED.setBrightness(ledbrightness);
  FastLED.delay(1000/FRAMES_PER_SECOND);

  if (distanceSide <= SetDistanceSide) {
    side('B'); // Side 'STOP'
    debugmessages2("SD: STOP  ");
    done = true;
  }
  if ((distanceSide > MidSide) && (!done)) {
    side('G'); // Side is miles away
    debugmessages2("SD: FAR   ");
    done = true;
  }

  if ((distanceSide <= MidSide) && (!done)) {
    side('Y'); // side is somewhere between half way there and optimal
    debugmessages2("SD: NEAR  ");
    done = true;
  }

  if ((distanceSide > CalibrateDistance)) {
      // we will check 1 more time after a delay to make sure this really is a calibration request
      debounce('S');
        if (distanceSide > CalibrateDistance) {
        debugmessages("SIDE CALIBRATION");
        calibrateFront('S');
        Side();
        // the value just read needs to be set to the minimum now
        SetDistanceSide=distanceSide;
        if (SetDistanceSide > SetDistanceSideMax) { SetDistanceSideMax = SetDistanceSide * 2; } // hopefully stops any minus values
        MidSide = SetDistanceSide + ((SetDistanceSideMax - SetDistanceSide) / 2); 
        #ifdef SERIAL_DEBUG   
          Serial.print("Side values set to min distance side ");
          Serial.print(SetDistanceSide);
          Serial.print(" and half way to ");
          Serial.println(MidSide);
        #endif
        debugmessages("                  ");
        #ifdef LCD_DEBUG
           lcdmessage(2,"Side min " + String(SetDistanceSide));
           lcdmessage(3,"Side hwy " + String(MidSide));
        #endif       
        lightsoff();
        delay(3000);
        }
  }
}

void debounce(char frontorside) {
  // was getting a few false readings of out of range which I use to calibrate the sensors so need to
  // take a few readings to make sure it is a calibration request 
  boolean falsereading=true;

  // now the kit is in place, and when the drive is empty seems getting > CalibrateDistance readings so
  // going to force it to make more checks than 8, trying 20
  for (int test=0; test < 20; test++) {
    if (frontorside == 'F') {
      Front();
      //Serial.println(distanceFront);
    } else {
      Side();
      //Serial.println(distanceSide);
    }
    delay(100);
    if (frontorside == 'F')
        {
        if (distanceFront < CalibrateDistance) {
          test=20;
          falsereading=true;
          } else {
          falsereading=false;
          }
        } else
        {
        if (distanceSide < CalibrateDistance) {
          test=20;
          falsereading=true;
          } else {
          falsereading=false;
          }
        }
  if (frontorside == 'F') {
    // this part of the code will hike the measured reading if it has found that it seems to be a calibration request
    // it was +200, changing to multiply x 2
    if (!falsereading) { distanceFront=CalibrateDistance*2; } else { distanceFront=samereading; }
  } else {
    if (!falsereading) { distanceSide=CalibrateDistance*2; } else { distanceSide=samereading; }    
  }
}
}

void Front(void) {
  dataFront = distanceFront;
  //digitalWrite(trigPinFront, LOW);
  //delayMicroseconds(5);  
  //digitalWrite(trigPinFront, HIGH);
  //delayMicroseconds(10); // in example code was 10 but I think we need quicker resolution 
  //digitalWrite(trigPinFront, LOW);
  //durationFront=pulseIn(echoPinFront, HIGH);
  //distanceFront=durationFront*0.034/2;
  //distanceFront=hc_f.measureDistanceCm();
  distanceFront=hc_f.readAccurateDisctanceInCm();

 
  //
  // 
  // check to see if the measurement has changed - if it hasn't car has stopped or not there and we
  // should turn off the lights, save some battery and keep checking back every now and again
  // the sensitivity of successive readings could be changed here, when on the bench I had +/- 5 which worked well
  // but seems to fluctuate a lot more in physical setting
  int factor = 5;
  if (distanceFront > SetDistanceFrontMax) {
    // modify the plus and minus for same reading checks as it fluctuates so much at greater distances
    factor = 100;
  }
  int low = samereading - factor;
  int high = samereading + factor;  
  if ((distanceFront > low) && (distanceFront < high) && (distanceFront < CalibrateDistance)) {
    samereadingcounter ++;
  } else {
    samereading = distanceFront;
    samereadingcounter = 0;
    nochange=0;
    successivechecks=0;
  }
  // changed samereadingcounter from 10 to 5 to get it to go sleep quicker as reading fluctuates a lot when distance is over say 3 metres
  int sroffset = 10;
  if (distanceFront > SetDistanceFrontMax) {
    sroffset=5;
  }
   if (samereadingcounter > sroffset) {
    nochange=1;
    lightsoff();
    FastLED.show();
    #ifdef LCD_DEBUG
      lcdmessage(3," *SLEEP* ");
      lcd.backlight(); 
    #endif
    // save current variables needed to continue after sleep operation
    successivechecks++;
    state.saveToRTC();
    Power(); // check power status to determine how long to sleep
    ShowSleep();
    delay(500); // temporary delay so I can see what the 'sleep' setting is on the LEDs
    #ifndef SERIAL_DEBUG
      // if serial debug is enabled we won't sleep
 // turn sleeping off temporarily     ESP.deepSleep(CurrentSleep * 1000000); // rst pin has to be connected to d0 in order for deepsleep to work apparently
    #endif
  }  
}
void Side(void) {
  dataSide = distanceSide;
 // digitalWrite(trigPinSide, LOW);
 // delayMicroseconds(5);
 // digitalWrite(trigPinSide, HIGH);
 // delayMicroseconds(10); // in example code was 10 but I think we need quicker resolution
//  digitalWrite(trigPinSide, LOW); 
// durationSide=pulseIn(echoPinSide, HIGH);
//  distanceSide=durationSide*0.034/2;
  //distanceSide=hc_s.measureDistanceCm();
  distanceSide=hc_s.readAccurateDisctanceInCm();
  //Serial.print("Distance side: ");
  //Serial.println(distanceSide);
}

void GetVoltage(void) {
  // the 588 value here is the max value with the restistors I used for my voltage divider
  // if you use different resistors (or I understand in case of D1 mini there's a built in
  // divider - you'll need to observe what your max value is when your battery is fully charged
  //
  // it seems the D1 mini already has a built in voltage divider so may not be a need to connect anything to A0
  // just read the value - I think the max value is 1024 from what I can surmise
  // I connected 2 x 1000ah 3,7v Lion batteries in parallel which is giving me about 4 volts, I had to put my
  // divider back in - possibly due to the way i've wired the battery shield away from the d1 mini
  // 4.1 is fully charged under no load
  voltage = analogRead(VOLTAGE_IN) * (4.0 / 545);
  #ifdef SERIAL_DEBUG
    Serial.print("Voltage divider raw reading ");
    Serial.println(analogRead(VOLTAGE_IN));
  #endif
}
