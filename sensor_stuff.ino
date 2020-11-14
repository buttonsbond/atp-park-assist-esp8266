void GetDistances(void) {
  TestFront();
  TestSide(); 
}

void TestFront(void) {
  boolean done = false;
  Front();
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.delay(1000/FRAMES_PER_SECOND);
 
 if (distanceFront <= SetDistanceFront) {
    advance('R'); // Side 'STOP'
    //Serial.println("Front: STOP");
    done = true;
  }
  if ((distanceFront > MidFront) && (!done)) {
    advance('G'); // Front is miles away
    //Serial.println("Front: Not close enough");
    done = true;
  }

  if ((distanceFront < MidFront) && (!done)) {
    advance('Y'); // front is somewhere between half way there and optimal
    //Serial.println("Front: Almost");
    done = true;
  }

  if ((distanceFront > 1000)) {
      // we will check 1 more time after a delay to make sure this really is a calibration request
      delay(2000);
        Front();
        if (distanceFront > 1000) {
        Serial.println("FRONT CALIBRATION");
        calibrateFront();
        Front();
        // the value just read needs to be set to the minimum now
        SetDistanceFront=distanceFront;
        if (SetDistanceFront > SetDistanceFrontMax) { SetDistanceFrontMax = SetDistanceFront * 2; } // hopefully stops any minus values
        MidFront = SetDistanceFront + ((SetDistanceFrontMax - SetDistanceFront) / 2); 
        Serial.print("Front values set to min distance front ");
        Serial.print(SetDistanceFront);
        Serial.print(" and half way to ");
        Serial.println(MidFront);
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
    //Serial.println("SIDE: STOP");
    done = true;
  }
  if ((distanceSide > MidSide) && (!done)) {
    side('G'); // Side is miles away
    //Serial.println("SIDE: Not close enough");
    done = true;
  }

  if ((distanceSide < MidSide) && (!done)) {
    side('Y'); // side is somewhere between half way there and optimal
    //Serial.println("SIDE: Almost");
    done = true;
  }

  if ((distanceSide > 1000)) {
      // we will check 1 more time after a delay to make sure this really is a calibration request
      delay(2000);
        Side();
        if (distanceSide > 1000) {
        Serial.println("SIDE CALIBRATION");
        calibrateFront();
        Side();
        // the value just read needs to be set to the minimum now
        SetDistanceSide=distanceSide;
        if (SetDistanceSide > SetDistanceSideMax) { SetDistanceSideMax = SetDistanceSide * 2; } // hopefully stops any minus values
        MidSide = SetDistanceSide + ((SetDistanceSideMax - SetDistanceSide) / 2); 
        Serial.print("Side values set to min distance side ");
        Serial.print(SetDistanceSide);
        Serial.print(" and half way to ");
        Serial.println(MidSide);
        lightsoff();
        delay(3000);
        }
  }
}

void Front(void) {
  dataFront = distanceFront;
  digitalWrite(trigPinFront, LOW);
  delayMicroseconds(5);  
  digitalWrite(trigPinFront, HIGH);
  delayMicroseconds(10); // in example code was 10 but I think we need quicker resolution 
  digitalWrite(trigPinFront, LOW);
  durationFront=pulseIn(echoPinFront, HIGH);
  distanceFront=durationFront*0.034/2;
  //Serial.print("Distance front: ");
  //Serial.println(distanceFront);      
}
void Side(void) {
  dataSide = distanceSide;
  digitalWrite(trigPinSide, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPinSide, HIGH);
  delayMicroseconds(10); // in example code was 10 but I think we need quicker resolution
  digitalWrite(trigPinSide, LOW); 
  durationSide=pulseIn(echoPinSide, HIGH);
  distanceSide=durationSide*0.034/2;
  //Serial.print("Distance side: ");
  //Serial.println(distanceSide);
}
