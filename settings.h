struct pcolour {int r; int g; int b;} __attribute__((packed));
typedef struct pcolour PC;
PC rgb;
 

// Replace with your network credentials
const char* ssid = "Telfy_3316";
const char* password = "jonesxa1";

// Settings for the HC-SR04 sensors
//
// setup for ultrasonic detector 1 - this will be use to measure distance to front of car
const int trigPinFront = D4;
const int echoPinFront = D5;
// setup for ultrasonic detector 2 - this will be used to measure distance of side of car to check for correct door opening
const int trigPinSide = D6;
const int echoPinSide = D7;
//
// variables for timings for sound to bounce back and be detected and then to store the distance in
//
long durationFront;
int distanceFront;
long durationSide;
int distanceSide;

int SetDistanceSide = 10; // default distance to passenger door
int SetDistanceFront = 30; // default distance value
int SetDistanceSideMax = 50; // if the distance from the side of the car is more than this, probably no car in the drive!
int SetDistanceFrontMax = 200; // if any more than this then probably no car in drive
int MidFront = (SetDistanceFrontMax - SetDistanceFront) / 2;
int MidSide = (SetDistanceSideMax - SetDistanceSide) / 2;

double dataFront;
double dataSide; 

// D0
//
// apparently if I want to make use of deep sleep, I need to use D0 and RST joined together
// there should be a switch that can be turned on/off on this otherwise you can't do OTA
// or USB upload for that matter.
//
// D1 and D2 (SCL / SDA)
//
// These pins used for LCD panel which I just use for displaying debug messages
//
// comment out if you don't want to send message to an LCD panel.
#define LCD_DEBUG
// remove comment if you want serial debugging
//#define SERIAL_DEBUG

//
// setup for the FastLED system
//
#define FASTLED_INTERNAL // this disables the pragma messages during compile time
#include <FastLED.h>
FASTLED_USING_NAMESPACE
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
//#define FASTLED_ESP8266_D1_PIN_ORDER
#define DATA_PIN 8   // D8
const int NUM_LEDS = 64; // this is an 8x8 pixel

uint8_t gHue = 100; // rotating "base color" used by many of the patterns

#ifdef LCD_DEBUG
  #define BRIGHTNESS  1 // not sure if this will help with the 5v to power the lcd panel by making LEDs dimm
#else if
  #define BRIGHTNESS 30
#endif 
#define FRAMES_PER_SECOND 30

// declare the array of LEDs
CRGB leds[NUM_LEDS];


// OTA updates
#define ENABLE_OTA      // comment out if you don't want OTA updates
int OTAport = 8266;

// monitor battery voltage
// in the circuit I used 2 12k resistors one from the 3.3v rail the other to the ground rail, with a wire
// connecting from middle of those (voltage divider) to the D1 mini A0 analog input
const int VOLTAGE_IN = A0;


//
// 
boolean nochange = false;
int samereading = 0;
int samereadingcounter = 0;
int CalibrateDistance = 1000; 
