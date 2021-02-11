//
// this section holds the sub procedures for displaying the feedback on the LED panel
//

void lightsoff() {
  FastLED.clear();
// for (int lightoff=0; lightoff < NUM_LEDS; ++lightoff) { leds[lightoff] = CRGB(0,0,0); }
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

// I borrowed this code from another project, I forget where I got it

void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
  if (nochange==0) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
  }
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

void testleds(int r,int g, int b, int del) {
  for (int i =0; i < NUM_LEDS; i++) {
    setPixel(i, r,g,b); // should be white
    FastLED.show();
    delay(del);
    setPixel(i, 0,0,0); // should be black or off
    FastLED.show();
  }
}

void arrowadvance(int step, char col) {

  colours(col);
  int r=rgb.r;
  int g=rgb.g;
  int b=rgb.b;
  
  int st = 0 + step;
  int pos = st+1;
  st=st+1;
  // first row
  for (int i = st; i < st+6; i++) {
    setPixel(i, r,g,b); 
    pos++;
  }
  // second row
  st = pos + 2;
  pos = st;
  for (int i = st; i < st+6; i++) {
    setPixel(i, r,g,b); 
    pos ++;
  }
  st = pos + 3;
  pos = st;
  for (int i = st; i < st+4; i++) {
    setPixel(i, r,g,b); 
    pos ++;
  }
  st = pos + 5;
  pos = st;
  for (int i = st; i < st+2; i++) {
    setPixel(i,r,g,b);
    pos ++;
  }
  FastLED.show();
  // Serial.print("advance...");
  delay(50);
}

void advance(char col) {
      arrowadvance(0, col);
      arrowadvance(0, 'b');
      arrowadvance(8, col);
      arrowadvance(8, 'b');
      arrowadvance(16, col);
      arrowadvance(16, 'b');
      arrowadvance(24, col);
      arrowadvance(24, 'b');
      arrowadvance(32, col);
      arrowadvance(32, 'b');
}

void side(char col) {
  sideindicators(col);
}
void sideindicators(char col) {

  colours(col); 
  int r=rgb.r;
  int g=rgb.g;
  int b=rgb.b;
  
  for (int i=0; i < NUM_LEDS; i = i+8 ) {
    setPixel(i,r,g,b);
    setPixel(i+7,r,g,b);
  }
  FastLED.show();
}

void calibrateFront(char x) {
int brightness = 0;
int fadeAmount = 100;
int st = 1;
if (x == 'S') {
  st = 2;
}
lightsoff();

for (int fadeloop = 0; fadeloop < 10; fadeloop ++) {
for(int i = 0; i < NUM_LEDS; i+=st ) {
    leds[i].setRGB(0,255,250); // Set Color HERE!!!
    leds[i].fadeLightBy(brightness);
  }
  FastLED.show();
  brightness = brightness + fadeAmount;
  // reverse the direction of the fading at the ends of the fade:
  if(brightness == 0 || brightness == 255)
  {
  fadeAmount = -fadeAmount ;
  }
  delay(500); // 500 is half a second so this loop will wait 5 seconds
}
// 5 seconds have gone, measure distance
//Serial.println("MEASURING");

}


void colours(char col) {
  switch (col) {
    case 'G':
      rgb.r=0;
      rgb.g=255;
      rgb.b=0;
      break;
    case 'R':
      rgb.r=255;
      rgb.g=0;
      rgb.b=0;
      break;
    case 'Y':
      rgb.r=255;
      rgb.g=255;
      rgb.b=0;
      break;
    case 'b':
      rgb.r=0;
      rgb.g=0;
      rgb.b=0;
      break;
    case 'B':
      rgb.r=0;
      rgb.g=0;
      rgb.b=255;
      break;
  }
  //return rgb;
}

void ShowSleep(void) {
  int n=8;
  if (CurrentSleep == StandardSleep) {
    // standardsleep
  }
  if (CurrentSleep == LowPowerSleep) {
    n=16;
  }
  if (CurrentSleep == InactiveSleep) {
    n=32;
  }
  colours('B'); 
  int r=rgb.r;
  int g=rgb.g;
  int b=rgb.b;
  for (int f = 1; f < CurrentSleep; f++) {
    setPixel(f,r,g,b);
  }
    FastLED.show();
}
