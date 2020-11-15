//
// PARKING SENSOR
//
// if the sensor reads 1010 - that means that something is covering the sensor - could use this to begin calibration
// cover sensor, countdown on LEDS 5,4,3,2,1 take measurement to see the closest distance the car can be


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "settings.h"
#ifdef ENABLE_OTA
  #include <WiFiUdp.h>
  #include <ArduinoOTA.h>
  #include <ESP8266mDNS.h>
#endif

ESP8266WebServer server(80);   //instantiate server at port 80 (http port)
 
String page = "";

void setup(void){

FastLED.addLeds<WS2811,DATA_PIN,GRB>(leds, NUM_LEDS);
//lightsoff();
 
pinMode(trigPinFront, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPinFront, INPUT);
pinMode(trigPinSide, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPinSide, INPUT);
pinMode(VOLTAGE_IN, INPUT); // read the voltage across the voltage divider to help monitor battery level
  
  Serial.begin(115200);
  WiFi.begin(ssid, password); //begin WiFi connection
  Serial.println("");
  
// Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", [](){
    page = "<h4>adhanu99.blogspot.com</h4><h1>Sensor to Node MCU Web Server</h1><h3>ultrasonic distance:</h3> <h1>"+String(dataFront)+String(dataSide)+"</h1><head><META HTTP-EQUIV=\"refresh\" CONTENT=\"1\"></head>"; 
    server.send(200, "text/html", page);
    
  });
  
  server.begin();
  Serial.println("Web server started!");
  //testleds();
  otasetup(); // allow updating sketch over the air
}
 
void loop(void){
  //dataFront = distanceFront;
  //dataSide = distanceSide;

  GetDistances();
  ArduinoOTA.handle();
  // Serial.println(analogRead(VOLTAGE_IN));
  delay(200);
//  server.handleClient();
}
