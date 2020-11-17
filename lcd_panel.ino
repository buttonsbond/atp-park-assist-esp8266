void lcdsetup(void) {
#ifdef LCD_DEBUG
Serial.begin(115200);
Wire.begin(D2, D1);   //Use predefined PINS consts
lcd.begin(20,4);      // The begin call takes the width and height. This
                      // Should match the number provided to the constructor.
lcd.backlight();      // Turn on the backlight.
//lcd.autoscroll();
lcd.home();
lcd.clear();
// lcd.setBacklight(LOW); have to connect LCD to an analog pin to control brightness



//lcd.init(); // initialize the lcd
//lcd.init();
// Print a message to the LCD.
//lcd.backlight();
#endif
}

void showsensors(void) {
#ifdef LCD_DEBUG
  lcd.setCursor(0,0);
  lcd.print("SIDE: " + String(distanceSide) + "  ");
  lcd.setCursor(0,1);
  lcd.print("FRNT: " + String(distanceFront) + "  ");
#endif
}

void lcdmessage(int line, String msg) {
#ifdef LCD_DEBUG
  lcd.setCursor(0,line);
  lcd.print(msg);
#endif
}

void topright() {
  lcd.setCursor(13,0);
  lcd.print("CH: " + String(nochange)+" ");
}

void bottomright() {
  lcd.setCursor(13,2);
  lcd.print("SF: " + String(SetDistanceFront)+" ");
}

void showvolts(String s) {
  lcd.setCursor(13,1);
  lcd.print(s);
}
