struct pcolour {int r; int g; int b;} __attribute__((packed));
typedef struct pcolour PC;
PC rgb;
 

// Replace with your network credentials
const char* ssid = "Telfy_3316";
const char* password = "jonesxa1";

// Settings for the HC-SR04 sensors
//
// setup for ultrasonic detector 1 - this will be use to measure distance to front of car
const int trigPinFront = D0;
const int echoPinFront = D1;
// setup for ultrasonic detector 2 - this will be used to measure distance of side of car to check for correct door opening
const int trigPinSide = D8;
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

//
// setup for the FastLED system
//
#define FASTLED_INTERNAL // this disables the pragma messages during compile time
#include <FastLED.h>
FASTLED_USING_NAMESPACE
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
//#define FASTLED_ESP8266_D1_PIN_ORDER
#define DATA_PIN 5   // D5
const int NUM_LEDS = 64; // this is an 8x8 pixel

uint8_t gHue = 100; // rotating "base color" used by many of the patterns

#define BRIGHTNESS  50 
#define FRAMES_PER_SECOND 30

// declare the array of LEDs
CRGB leds[NUM_LEDS];
