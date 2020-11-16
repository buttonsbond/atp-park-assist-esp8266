//
// PARKING SENSOR
//
// if the sensor reads 1010 - that means that something is covering the sensor - could use this to begin calibration
// cover sensor, countdown on LEDS 5,4,3,2,1 take measurement to see the closest distance the car can be

#include <hcsr04.h>
//#include <HCSR04.h>  // trying this library to see if results any better than standard pulsein
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "settings.h"
#ifdef ENABLE_OTA
  #include <WiFiUdp.h>
  #include <ArduinoOTA.h>
  #include <ESP8266mDNS.h>
#endif

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
 
String page = "";

void setup(void){
hc_f.init(trigPinFront, echoPinFront);
hc_s.init(trigPinSide, echoPinSide);

FastLED.addLeds<WS2811,DATA_PIN,GRB>(leds, NUM_LEDS);
//lightsoff();
 
pinMode(trigPinFront, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPinFront, INPUT);
pinMode(trigPinSide, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPinSide, INPUT);
pinMode(VOLTAGE_IN, INPUT); // read the voltage across the voltage divider to help monitor battery level
  
  Serial.begin(115200);
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
  #ifdef LCD_DEBUG
    showsensors();
  #endif
  debugmessages("Volts: " + String(analogRead(VOLTAGE_IN)));

//  delay(200);
//  server.handleClient();
}
