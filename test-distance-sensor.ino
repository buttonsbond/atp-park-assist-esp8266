//
// PARKING SENSOR
//
// if the sensor reads 1010 - that means that something is covering the sensor - could use this to begin calibration
// cover sensor, countdown on LEDS 5,4,3,2,1 take measurement to see the closest distance the car can be

#include <hcsr04.h>
//#include <HCSR04.h>  // trying this library to see if results any better than standard pulsein
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include "settings.h"
#ifdef ENABLE_OTA
  #include <WiFiUdp.h>
  #include <ArduinoOTA.h>
  #include <ESP8266mDNS.h>
#endif
// this is for saving variables in RTC memory when the module sleeps to save battery
#include <RTCVars.h>
// setup RTC variable saving
RTCVars state; // create the state object

#ifdef LCD_DEBUG
  #include <Wire.h>
  #include <LiquidCrystal_I2C.h>
  LiquidCrystal_I2C lcd(0x27,16,4); // set the LCD address to 0x3F for a 16 chars and 4 line display
#endif

HCSR04 hc_f;
HCSR04 hc_s;
unsigned short distance;

//UltraSonicDistanceSensor hc_f(trigPinFront,echoPinFront); //initialisation class HCSR04 (trig pin , echo pin)
//UltraSonicDistanceSensor hc_s(trigPinSide,echoPinSide);

ESP8266WebServer server(80);   //instantiate server at port 80 (http port)

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", utcOffsetInSeconds, 60000);
 
String page = "";

void setup(void){
//
// before we do anything else check for variables saved in RTC
  state.registerVar( &distanceFront );  // we send a pointer to each of our variables
  state.registerVar( &distanceSide );
  state.registerVar( &nochange );
  state.registerVar( &SetDistanceSide);
  state.registerVar( &SetDistanceFront);
  state.registerVar( &SetDistanceSideMax);
  state.registerVar( &SetDistanceFrontMax);
  state.registerVar( &MidFront);
  state.registerVar( &MidSide);
  state.registerVar( &samereading);
  state.registerVar( &CurrentHour);
  state.registerVar( &CurrentMin);
  state.registerVar( &successivechecks);
  state.loadFromRTC();
  sprintf(mytime,"%i:%i",CurrentHour,CurrentMin);
hc_f.init(trigPinFront, echoPinFront);
hc_s.init(trigPinSide, echoPinSide);

FastLED.addLeds<WS2811,DATA_PIN,GRB>(leds, NUM_LEDS);
//lightsoff();
 
pinMode(trigPinFront, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPinFront, INPUT);
pinMode(trigPinSide, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPinSide, INPUT);
pinMode(VOLTAGE_IN, INPUT); // read the voltage across the voltage divider to help monitor battery level
pinMode(D8,OUTPUT);

#ifdef SERIAL_DEBUG  
  Serial.begin(115200);
#endif
  WiFi.begin(ssid, password); //begin WiFi connection
 // Serial.println("");
  
// Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    debugmessages(".");
  }
  //Serial.println("");
  debugmessages("Connected to ");
  debugmessages(ssid);
  debugmessages("IP address: ");
  debugmessages(WiFi.localIP().toString().c_str());

  // get the time
  timeClient.begin();
  GetHour();
  debugmessages(mytime);
  //Serial.println(CurrentHour);
  #ifdef ENABLE_MQTT
    mqtt_setup();
    mqttpublish();
  #endif
  server.on("/", [](){
    page = "webpage"; 
    server.send(200, "text/html", page);
    
  });
  
  server.begin();
  //Serial.println("Web server started!");
  //testleds();
  otasetup(); // allow updating sketch over the air

  #ifdef LCD_DEBUG
    //Serial.println("Starting LCD monitor");
    lcdsetup();
  #endif
}
 
void loop(void){
  //dataFront = distanceFront;
  //dataSide = distanceSide;

  GetDistances();
  ArduinoOTA.handle();
  GetVoltage();
  GetHour();
  #ifdef LCD_DEBUG
    showsensors();
    showvolts("VO: " + String(voltage));
  #endif
  #ifdef ENABLE_MQTT
    mqttpublish();
  #endif

//  delay(200);
//  server.handleClient();
}
