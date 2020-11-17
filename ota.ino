void otasetup() {
   ArduinoOTA.setPort(OTAport);
   ArduinoOTA.setHostname("parking");
   //ArduinoOTA.setPassword((const char *)OTAPassword);
   ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }
     debugmessages("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
      debugmessages("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    debugmessages("Progress: %u%%\r" + (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    debugmessages("Error[%u]: "+ error);
    if (error == OTA_AUTH_ERROR) {
      debugmessages("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      debugmessages("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      debugmessages("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      debugmessages("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      debugmessages("End Failed");
    }
  });
  ArduinoOTA.begin(); 
}
void debugmessages(String msg) {
  #ifdef SERIAL_DEBUG
    Serial.println(msg);
  #endif
  #ifdef LCD_DEBUG
    lcdmessage(2,msg);
  #endif
}
void debugmessages2(String msg) {
  #ifdef SERIAL_DEBUG
    Serial.println(msg);
  #endif
  #ifdef LCD_DEBUG
    lcdmessage(3,msg);
  #endif 
}
