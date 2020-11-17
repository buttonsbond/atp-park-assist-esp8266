void GetDistances(void) {
  TestFront();
  topright();
  bottomright();
  if (nochange==0) {
  TestSide(); // only test side when the whole shebang is up and running - otherwise if its just a wakeup to check, just test the front sensor
  }
}

void TestFront(void) {
  boolean done = false;
  Front();
  FastLED.setBrightness(BRIGHTNESS);
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
        calibrateFront();
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
  FastLED.setBrightness(BRIGHTNESS);
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
        calibrateFront();
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
  
  for (int test=0; test < 8; test++) {
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
        if (distanceFront < 1000) {
          test=8;
          falsereading=true;
          } else {
          falsereading=false;
          }
        } else
        {
        if (distanceSide < 1000) {
          test=8;
          falsereading=true;
          } else {
          falsereading=false;
          }
        }
  if (frontorside == 'F') {
    if (!falsereading) { distanceFront=1001; } else { distanceFront=100; }
  } else {
    if (!falsereading) { distanceSide=1001; } else { distanceSide=100; }    
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
  int low = samereading - 5;
  int high = samereading + 5;  
  if ((distanceFront > low) && (distanceFront < high) && (distanceFront < 1000)) {
    samereadingcounter ++;
  } else {
    samereading = distanceFront;
    samereadingcounter = 0;
    nochange=0;
  }
  if (samereadingcounter > 10) {
    nochange=1;
    lightsoff();
    FastLED.show();
    #ifdef LCD_DEBUG
      lcdmessage(3," -- SLEEP -- ");
      lcd.backlight(); 
    #endif
    // turned off deep sleep as need to implement a way to recover variable states otherwise the program just starts afresh
    state.saveToRTC();
    ESP.deepSleep(5 * 1000000); // rst pin has to be connected to d0 in order for deepsleep to work apparently
    //Serial.println("nothing has changed in a while");
    //Serial.print("Reading was : ");
    //Serial.println(distanceFront);
   // Serial.print("hc ");
   // Serial.println(hc_f.dist());
  }
 // Serial.print("Same reading counter :");
 // Serial.println(samereadingcounter);
 // Serial.print("Reading was : ");
 // Serial.print(distanceFront);
  //Serial.print("Distance front: ");
  //Serial.println(distanceFront);      
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
